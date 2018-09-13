open DiffType;

open InspectorComponentType;

module CustomEventHandler = {
  include EmptyEventHandler.EmptyEventHandler;
  type prepareTuple = Wonderjs.GameObjectType.gameObject;
  type dataTuple = componentType;

  let _isRemoveLight = type_ => type_ === Light;

  let handleSelfLogic = ((store, dispatchFunc), currentSceneTreeNode, type_) => {
    StateLogicService.getEditEngineState()
    |> InspectorRemoveComponentUtils.removeComponentByTypeForEditEngineState(
         type_,
         StateLogicService.getEditEngineComponent(
           DiffType.GameObject,
           currentSceneTreeNode,
         ),
       )
    |> StateLogicService.setEditEngineState;

    let (editorState, runEngineState) =
      (StateEditorService.getState(), StateLogicService.getRunEngineState())
      |> InspectorRemoveComponentUtils.removeComponentByTypeForRunEngineState(
           type_,
           currentSceneTreeNode,
         );

    runEngineState |> StateLogicService.setRunEngineState;

    editorState |> StateEditorService.setState |> ignore;

    _isRemoveLight(type_) ?
      {
        StateLogicService.getAndRefreshEditAndRunEngineState();

        OperateLightMaterialLogicService.reInitAllMaterials
        |> StateLogicService.getAndSetEditAndRunEngineState;
      } :
      ();

    StateLogicService.getAndRefreshEditAndRunEngineState();

    dispatchFunc(AppStore.UpdateAction(Update([|UpdateStore.Inspector|])))
    |> ignore;
  };
};

module MakeEventHandler = EventHandler.MakeEventHandler(CustomEventHandler);