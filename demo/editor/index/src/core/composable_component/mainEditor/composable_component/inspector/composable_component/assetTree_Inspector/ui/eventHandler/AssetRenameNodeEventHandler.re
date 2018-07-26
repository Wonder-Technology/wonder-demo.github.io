open AppStore;
open DiffType;

module CustomEventHandler = {
  include EmptyEventHandler.EmptyEventHandler;
  type prepareTuple = (int, AssetNodeType.assetNodeType);
  type dataTuple = string;

  let _renameFolderNode = (folderId, name, editorState, folderNodeMap) =>
    folderNodeMap
    |> WonderCommonlib.SparseMapService.unsafeGet(folderId)
    |> AssetNodeEditorService.renameFolderNodeResult(name)
    |> AssetFolderNodeMapEditorService.setResult(folderId, _, editorState)
    |> StateEditorService.setState
    |> ignore;

  let _renameJsonNode = (jsonId, name, editorState, jsonNodeMap) =>
    jsonNodeMap
    |> WonderCommonlib.SparseMapService.unsafeGet(jsonId)
    |> AssetNodeEditorService.renameJsonNodeResult(name)
    |> AssetJsonNodeMapEditorService.setResult(jsonId, _, editorState)
    |> StateEditorService.setState
    |> ignore;

  let _renameTextureNode = (textureIndex, name, _textureNodeMap) =>
    OperateTextureLogicService.renameTextureToEngine(textureIndex, name);

  let handleSelfLogic = ((store, dispatchFunc), ( nodeId, nodeType ), value) => {
    let editorState = StateEditorService.getState();
    AssetNodeUtils.handleSpeficFuncByAssetNodeType(
      nodeType,
      (
        _renameFolderNode(nodeId, value, editorState),
        _renameJsonNode(nodeId, value, editorState),
        _renameTextureNode(nodeId, value),
      ),
    );

    dispatchFunc(AppStore.UpdateAction(Update([|UpdateStore.Asset|])))
    |> ignore;
  };
};

module MakeEventHandler = EventHandler.MakeEventHandler(CustomEventHandler);