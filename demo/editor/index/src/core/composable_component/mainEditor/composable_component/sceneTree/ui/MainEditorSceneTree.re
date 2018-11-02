open SceneGraphType;

type retainedProps = {updateTypeArr: UpdateStore.updateComponentTypeArr};

module Method = {
  let onSelect = ((store, dispatchFunc), uid) => {
    let editorState = StateEditorService.getState();

    switch (SceneEditorService.getCurrentSceneTreeNode(editorState)) {
    | None =>
      SceneTreeSelectCurrentNodeEventHandler.MakeEventHandler.pushUndoStackWithNoCopyEngineState(
        (store, dispatchFunc),
        (),
        uid,
      )
    | Some(gameObject) =>
      gameObject === uid ?
        () :
        SceneTreeSelectCurrentNodeEventHandler.MakeEventHandler.pushUndoStackWithNoCopyEngineState(
          (store, dispatchFunc),
          (),
          uid,
        )
    };
  };

  let handleToggleShowTreeChildren =
      (store, dispatchFunc, targetId, isShowChildren) => {
    let newSceneGraphData =
      store
      |> StoreUtils.unsafeGetSceneGraphDataFromStore
      |> SceneTreeUtils.setSpecificSceneTreeNodeIsShowChildren(
           targetId,
           isShowChildren,
         );

    dispatchFunc(
      AppStore.SceneTreeAction(SetSceneGraph(Some(newSceneGraphData))),
    )
    |> ignore;

    dispatchFunc(AppStore.UpdateAction(Update([|UpdateStore.SceneTree|])))
    |> ignore;
  };

  let dragGameObjectIntoGameObject = SceneTreeDragGameObjectEventHandler.MakeEventHandler.pushUndoStackWithNoCopyEngineState;

  let dragWDBIntoScene = SceneTreeDragWDBEventHandler.MakeEventHandler.pushUndoStackWithNoCopyEngineState;

  let _isSelected = (uid, currentSceneTreeNode) =>
    switch (currentSceneTreeNode) {
    | None => false
    | Some(gameObject) => gameObject === uid
    };

  let rec buildSceneTreeArray =
          (
            (store, dispatchFunc, dragImg),
            currentSceneTreeNode,
            (onSelectFunc, onDropFunc),
            sceneGraphArr,
          ) =>
    sceneGraphArr
    |> Js.Array.map(({uid, name, isShowChildren, children}) =>
         <TreeNode
           key=(DomHelper.getRandomKey())
           id=uid
           name
           isSelected=(_isSelected(uid, currentSceneTreeNode))
           isActive=true
           dragImg
           widget=(SceneTreeUtils.getWidget())
           onSelect=onSelectFunc
           onDrop=onDropFunc
           isWidget=SceneTreeUtils.isWidget
           isShowChildren
           isHasChildren=(
             children |> Js.Array.length >= 1
           )
           handleToggleShowTreeChildren=(
             handleToggleShowTreeChildren(store, dispatchFunc)
           )
           handleRelationError=SceneTreeUtils.isGameObjectRelationError
           treeChildren=(
             buildSceneTreeArray(
               (store, dispatchFunc, dragImg),
               currentSceneTreeNode,
               (onSelectFunc, onDropFunc),
               children,
             )
           )
         />
       );
};

let component =
  ReasonReact.statelessComponentWithRetainedProps("MainEditorSceneTree");

let render = (store, dispatchFunc, _self) => {
  let editorState = StateEditorService.getState();

  <article key="sceneTree" className="wonder-sceneTree-component">
    <DragTree
      key=(DomHelper.getRandomKey())
      treeArray=(
        store
        |> StoreUtils.unsafeGetSceneGraphDataFromStore
        |> ArrayService.unsafeGetFirst
        |> (scene => scene.children)
        |> Method.buildSceneTreeArray(
             (store, dispatchFunc, DomHelper.createElement("img")),
             editorState |> SceneEditorService.getCurrentSceneTreeNode,
             (
               Method.onSelect((store, dispatchFunc)),
               Method.dragGameObjectIntoGameObject(
                 (store, dispatchFunc),
                 (),
               ),
             ),
           )
      )
      rootUid=(
        SceneEngineService.getSceneGameObject
        |> StateLogicService.getEngineStateToGetData
      )
      dragGameObject=(
        Method.dragGameObjectIntoGameObject((store, dispatchFunc), ())
      )
      dragWDB=(Method.dragWDBIntoScene((store, dispatchFunc), ()))
      isWidget=SceneTreeUtils.isWidget
      handleRelationError=SceneTreeUtils.isGameObjectRelationError
      isAssetWDBFile=AssetUtils.isAssetWDBFile
    />
  </article>;
};

let shouldUpdate =
    ({newSelf}: ReasonReact.oldNewSelf('a, retainedProps, 'c)) =>
  newSelf.retainedProps.updateTypeArr
  |> StoreUtils.shouldComponentUpdate(UpdateStore.SceneTree);

let make = (~store: AppStore.appState, ~dispatchFunc, _children) => {
  ...component,
  retainedProps: {
    updateTypeArr: StoreUtils.getUpdateComponentTypeArr(store),
  },
  shouldUpdate,
  render: self => render(store, dispatchFunc, self),
};