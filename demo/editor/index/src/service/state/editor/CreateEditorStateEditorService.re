open EditorType;

let create = () => {
  sceneRecord: {
    diffMap: None,
    currentSceneTreeNode: None,
    isRun: false,
  },
  assetRecord: {
    assetTreeRoot: None,
    index: 0,
    currentNodeData: None,
    currentNodeParentId: None,
    textureNodeMap: WonderCommonlib.SparseMapService.createEmpty(),
    jsonNodeMap: WonderCommonlib.SparseMapService.createEmpty(),
    imageBase64Map: WonderCommonlib.SparseMapService.createEmpty(),
    folderNodeMap: WonderCommonlib.SparseMapService.createEmpty(),
  },
  inspectorRecord: {
    componentTypeMap: WonderCommonlib.SparseMapService.createEmpty(),
  },
  currentDragSource: (None, None),
  currentSelectSource: None,
  loopId: (-1),
};