open UpdateStore;

module CustomEventHandler = {
  include EmptyEventHandler.EmptyEventHandler;
  type prepareTuple = unit;
  type dataTuple = unit;

  let _checkSceneGraphDataAndDispatch = (dispatchFunc, newSceneGraphArr) => {
    WonderLog.Contract.requireCheck(
      () =>
        WonderLog.(
          Contract.(
            test(
              Log.buildAssertMessage(
                ~expect=
                  {j|the newSceneGraphArr should equal the sceneGraph from engine|j},
                ~actual={j|not|j},
              ),
              () =>
              SceneTreeUtils.getSceneGraphDataFromEngine
              |> StateLogicService.getStateToGetData == newSceneGraphArr
              |> assertTrue
            )
          )
        ),
      StateEditorService.getStateIsDebug(),
    );

    dispatchFunc(
      AppStore.SceneTreeAction(SetSceneGraph(Some(newSceneGraphArr))),
    )
    |> ignore;

    dispatchFunc(AppStore.UpdateAction(Update([|Inspector, SceneTree|])))
    |> ignore;
  };

  let _getRemovedSceneGraphData = sceneGraphArr =>
    switch (
      SceneEditorService.getCurrentSceneTreeNode
      |> StateLogicService.getEditorState
    ) {
    | None =>
      WonderLog.Log.error(
        WonderLog.Log.buildErrorMessage(
          ~title="disposeCurrentSceneTreeNode",
          ~description=
            {j|current gameObject should exist, but actual is None|j},
          ~reason="",
          ~solution={j|set current gameObject|j},
          ~params={j||j},
        ),
      );
      (sceneGraphArr, None);

    | Some(gameObject) =>
      let runEngineState = StateLogicService.getRunEngineState();

      runEngineState |> CameraEngineService.hasCameraGroup(gameObject) ?
        {
          SceneUtils.doesSceneHasRemoveableCamera() ?
            runEngineState
            |> CameraEngineService.prepareForRemoveCameraGroup(gameObject)
            |> StateLogicService.setRunEngineState :
            ();

          let (newSceneGraphArr, removedTreeNode) =
            sceneGraphArr |> SceneTreeUtils.removeDragedTreeNode(gameObject);

          (newSceneGraphArr, removedTreeNode |. Some);
        } :
        {
          let (newSceneGraphArr, removedTreeNode) =
            sceneGraphArr |> SceneTreeUtils.removeDragedTreeNode(gameObject);
          (newSceneGraphArr, removedTreeNode |. Some);
        };
    };

  let _hasLightComponent = removedTreeNode => {
    open SceneGraphType;

    let runEngineState = StateLogicService.getRunEngineState();

    let rec _iterateJudge = (result, removedTreeNodeArr) =>
      result ?
        result :
        removedTreeNodeArr
        |> WonderCommonlib.ArrayService.reduceOneParam(
             (. result, {uid, children}) =>
               result ?
                 result :
                 _iterateJudge(
                   GameObjectComponentEngineService.hasDirectionLightComponent(
                     uid,
                     runEngineState,
                   )
                   || GameObjectComponentEngineService.hasPointLightComponent(
                        uid,
                        runEngineState,
                      ),
                   children,
                 ),
             result,
           );

    _iterateJudge(false, [|removedTreeNode|]);
  };

  let handleSelfLogic = ((store, dispatchFunc), (), ()) => {
    let sceneGraphArr = store |> StoreUtils.unsafeGetSceneGraphDataFromStore;

    let (newSceneGraphArr, removedTreeNode) =
      _getRemovedSceneGraphData(sceneGraphArr);

    switch (removedTreeNode) {
    | None => ()
    | Some(removedTreeNode) =>
      let hasLightComponent = _hasLightComponent(removedTreeNode);

      removedTreeNode
      |> CurrentSceneTreeNodeLogicService.disposeCurrentSceneTreeNode;

      StateLogicService.getAndRefreshEditAndRunEngineState();

      hasLightComponent ?
        OperateLightMaterialLogicService.reInitAllMaterials
        |> StateLogicService.getAndSetEditAndRunEngineState :
        ();

      StateLogicService.getAndRefreshEditAndRunEngineState();
    };

    _checkSceneGraphDataAndDispatch(dispatchFunc, newSceneGraphArr);
  };
};

module MakeEventHandler = EventHandler.MakeEventHandler(CustomEventHandler);