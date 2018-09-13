open EditorType;
open AssetNodeType;

let getWDBNodeMap = editorState =>
  editorState.assetRecord |> WDBNodeMapAssetService.getWDBNodeMap;

let setWDBNodeMap = (wdbNodeMap, editorState) => {
  ...editorState,
  assetRecord:
    editorState.assetRecord
    |> WDBNodeMapAssetService.setWDBNodeMap(wdbNodeMap),
};

let setResult = (index, result, editorState) => {
  ...editorState,
  assetRecord:
    editorState.assetRecord |> WDBNodeMapAssetService.setResult(index, result),
};

let getWDBBaseName = (currentNodeId, wdbNodeMap) =>
  wdbNodeMap
  |> WonderCommonlib.SparseMapService.unsafeGet(currentNodeId)
  |> (({name}: wdbResultType) => name);

let getWDBTotalName = (currentNodeId, wdbNodeMap) =>
  wdbNodeMap
  |> WonderCommonlib.SparseMapService.unsafeGet(currentNodeId)
  |> (({name, postfix}: wdbResultType) => name ++ postfix);

let getWDBParentId = (currentNodeId, wdbNodeMap) =>
  wdbNodeMap
  |> WonderCommonlib.SparseMapService.unsafeGet(currentNodeId)
  |> (({parentId}: wdbResultType) => parentId);

let buildWDBNodeResult =
    (name, postfix, parentId, wdbGameObject, wdbArrayBuffer) => {
  name,
  postfix,
  parentId,
  wdbGameObject,
  wdbArrayBuffer,
};

let renameWDBNodeResult = (name, wdbNodeResult) : wdbResultType => {
  ...wdbNodeResult,
  name,
};

let setWDBNodeResultParent = (parentId, wdbNodeResult) : wdbResultType => {
  ...wdbNodeResult,
  parentId,
};