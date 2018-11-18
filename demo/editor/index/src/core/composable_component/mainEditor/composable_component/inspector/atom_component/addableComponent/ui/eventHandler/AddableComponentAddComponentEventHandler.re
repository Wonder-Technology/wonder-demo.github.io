

open InspectorComponentType;

module CustomEventHandler = {
  include EmptyEventHandler.EmptyEventHandler;
  type prepareTuple = Wonderjs.GameObjectType.gameObject;
  type dataTuple = componentType;
  type return = unit;

  let handleSelfLogic = ((store, dispatchFunc), currentSceneTreeNode, type_) => {
    let (editorState, engineState) =
      (StateEditorService.getState(), StateEngineService.unsafeGetState())
      |> InspectorAddComponentUtils.addComponentByType(
           type_,
           currentSceneTreeNode,
         );

    engineState |> StateEngineService.setState |> ignore;
    editorState |> StateEditorService.setState |> ignore;

    GameObjectEngineService.initGameObject(currentSceneTreeNode)
    |> StateLogicService.getAndRefreshEngineStateWithFunc;

    dispatchFunc(AppStore.UpdateAction(Update([|UpdateStore.Inspector|])))
    |> ignore;
  };
};

module MakeEventHandler = EventHandler.MakeEventHandler(CustomEventHandler);