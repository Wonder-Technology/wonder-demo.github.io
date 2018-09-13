open AssetNodeType;
let handleSpeficFuncByAssetNodeType =
    (
      type_,
      (
        handleFodlerFunc,
        handleJsonFunc,
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
    |> handleFodlerFunc
  | Texture =>
    editorState
    |> AssetTextureNodeMapEditorService.getTextureNodeMap
    |> handleTextureFunc
  | Json =>
    editorState
    |> AssetJsonNodeMapEditorService.getJsonNodeMap
    |> handleJsonFunc
  | Material =>
    editorState
    |> AssetMaterialNodeMapEditorService.getMaterialNodeMap
    |> handleMaterialFunc
  | WDB =>
    editorState |> AssetWDBNodeMapEditorService.getWDBNodeMap |> handleWDBFunc
  };

let getAssetNodeTotalName = (type_, currentNodeId, editorState) =>
  editorState
  |> handleSpeficFuncByAssetNodeType(
       type_,
       (
         AssetFolderNodeMapEditorService.getFolderName(currentNodeId),
         AssetJsonNodeMapEditorService.getJsonTotalName(currentNodeId),
         OperateTextureLogicService.getTextureTotalName(currentNodeId),
         AssetMaterialNodeMapEditorService.getMaterialTotalName(
           currentNodeId,
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
         AssetJsonNodeMapEditorService.getJsonParentId(currentNodeId),
         AssetTextureNodeMapEditorService.getTextureParentId(currentNodeId),
         AssetMaterialNodeMapEditorService.getMaterialParentId(currentNodeId),
         AssetWDBNodeMapEditorService.getWDBParentId(currentNodeId),
       ),
     );