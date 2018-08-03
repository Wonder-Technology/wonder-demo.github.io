open DiffType;

module CustomEventHandler = {
  include EmptyEventHandler.EmptyEventHandler;
  type prepareTuple = unit;
  type dataTuple = Wonderjs.MaterialType.material;

  let handleSelfLogic = ((store, dispatchFunc), (), materialComponent) => {
    OperateTextureLogicService.replaceMaterialComponentToNoMapOne(
      SceneEditorService.unsafeGetCurrentSceneTreeNode
      |> StateLogicService.getEditorState,
      materialComponent,
      (
        OperateLightMaterialLogicService.disposeLightMaterial,
        OperateLightMaterialLogicService.setLightMaterialColor,
        OperateLightMaterialLogicService.createLightMaterial,
        OperateLightMaterialLogicService.addLightMaterial,
      ),
    );

    dispatchFunc(AppStore.UpdateAction(Update([|UpdateStore.Inspector|])))
    |> ignore;
  };
};

module MakeEventHandler = EventHandler.MakeEventHandler(CustomEventHandler);