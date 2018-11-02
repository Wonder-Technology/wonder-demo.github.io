open AssetNodeType;

let handleSpeficFuncByAssetNodeType =
    (
      type_,
      (
        handleFolderFunc,
        handleTextureFunc,
        handleMaterialFunc,
        handleWDBFunc,
      ),
      editorState,
    ) =>
  switch (type_) {
  | Folder =>
    editorState
    |> AssetFolderNodeMapEditorService.getFolderNodeMap
    |> handleFolderFunc
  | Texture =>
    editorState
    |> AssetTextureNodeMapEditorService.getTextureNodeMap
    |> handleTextureFunc
  | Material =>
    editorState
    |> AssetMaterialNodeMapEditorService.getMaterialNodeMap
    |> handleMaterialFunc
  | WDB =>
    editorState |> AssetWDBNodeMapEditorService.getWDBNodeMap |> handleWDBFunc
  };

let getAssetNodeTotalName =
    (type_, currentNodeId, (editorState, engineState)) =>
  editorState
  |> handleSpeficFuncByAssetNodeType(
       type_,
       (
         AssetFolderNodeMapEditorService.getFolderName(currentNodeId),
         OperateTextureLogicService.getTextureBaseName(currentNodeId),
         AssetMaterialNodeMapLogicService.getMaterialBaseName(
           currentNodeId,
           engineState,
         ),
         AssetWDBNodeMapEditorService.getWDBTotalName(currentNodeId),
       ),
     );

let getAssetNodeParentId = (type_, currentNodeId, editorState) =>
  editorState
  |> handleSpeficFuncByAssetNodeType(
       type_,
       (
         AssetFolderNodeMapEditorService.getFolderParentId(currentNodeId),
         AssetTextureNodeMapEditorService.getParentFolderNodeId(currentNodeId),
         AssetMaterialNodeMapEditorService.getParentFolderNodeId(currentNodeId),
         AssetWDBNodeMapEditorService.getWDBParentId(currentNodeId),
       ),
     );