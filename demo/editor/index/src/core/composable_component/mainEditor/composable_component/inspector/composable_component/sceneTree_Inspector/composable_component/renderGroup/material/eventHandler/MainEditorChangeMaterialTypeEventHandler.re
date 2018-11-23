open AssetMaterialDataType;

module CustomEventHandler = {
  include EmptyEventHandler.EmptyEventHandler;
  type prepareTuple = unit;
  type dataTuple = (materialType, materialType);
  type return = unit;

  let _updateMaterialNodeData =
      (sourceMaterial, targetMaterial, targetMaterialType, editorState) =>
    switch (
      MaterialNodeIdMapAssetEditorService.getNodeId(
        sourceMaterial,
        editorState,
      )
    ) {
    | None => editorState
    | Some(materialNodeId) =>
      MaterialUpdateNodeAssetEditorService.updateMaterialNodeData(
        materialNodeId,
        targetMaterial,
        targetMaterialType,
        editorState,
      )
    };

  let _replaceSourceMaterialWithItsAllGameObjects =
      (gameObjects, materialData, engineState) =>
    gameObjects
    |> WonderCommonlib.ArrayService.reduceOneParam(
         (. engineState, gameObject) =>
           InspectorRenderGroupUtils.Remove.replaceMaterialByMaterialData(
             gameObject,
             materialData,
             engineState,
           ),
         engineState,
       );

  let handleSelfLogic =
      ((store, dispatchFunc), (), (sourceMaterialType, targetMaterialType)) => {
    let editorState = StateEditorService.getState();
    let engineState = StateEngineService.unsafeGetState();

    let gameObject =
      SceneEditorService.unsafeGetCurrentSceneTreeNode(editorState);

    let sourceMaterial =
      MainEditorMaterialUtils.getMaterialComponentByType(
        gameObject,
        sourceMaterialType,
        engineState,
      );

    let (engineState, targetMaterial) =
      MainEditorMaterialUtils.createMaterialByType(
        targetMaterialType,
        engineState,
      );

    let gameObjects =
      engineState
      |> MainEditorMaterialUtils.unsafeGetGameObjectsByType(
           sourceMaterial,
           sourceMaterialType,
         )
      |> Js.Array.copy;

    let engineState =
      _replaceSourceMaterialWithItsAllGameObjects(
        gameObjects,
        (
          (sourceMaterial, targetMaterial),
          (sourceMaterialType, targetMaterialType),
        ),
        engineState,
      );

    let engineState =
      MainEditorMaterialUtils.setName(
        targetMaterial,
        targetMaterialType,
        MainEditorMaterialUtils.getName(
          sourceMaterial,
          sourceMaterialType,
          engineState,
        ),
        engineState,
      );

    engineState |> StateLogicService.refreshEngineState;

    _updateMaterialNodeData(
      sourceMaterial,
      targetMaterial,
      targetMaterialType,
      editorState,
    )
    |> StateEditorService.setState
    |> ignore;

    dispatchFunc(AppStore.UpdateAction(Update([|Inspector|]))) |> ignore;
  };
};

module MakeEventHandler = EventHandler.MakeEventHandler(CustomEventHandler);