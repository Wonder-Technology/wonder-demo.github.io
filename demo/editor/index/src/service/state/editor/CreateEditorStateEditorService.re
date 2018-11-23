open EditorType;

let create = () => {
  settingRecord: RecordSettingService.create(),
  sceneRecord: {
    currentSceneTreeNode: None,
  },
  assetRecord: {
    assetTreeRoot: None,
    index: 0,
    imageIndex: (-1),
    removedAssetIdArray: [||],
    currentNodeData: None,
    currentNodeParentId: None,
    textureNodeMap: WonderCommonlib.SparseMapService.createEmpty(),
    imageNodeMap: WonderCommonlib.SparseMapService.createEmpty(),
    folderNodeMap: WonderCommonlib.SparseMapService.createEmpty(),
    wdbNodeMap: WonderCommonlib.SparseMapService.createEmpty(),
    materialNodeMap: WonderCommonlib.SparseMapService.createEmpty(),
    materialNodeIdMap: WonderCommonlib.SparseMapService.createEmpty(),
    geometryData: {
      defaultCubeGeometryComponent: (-1),
      defaultSphereGeometryComponent: (-1),
    },
    materialData: {
      defaultBasicMaterialData: None,
      defaultLightMaterialData: None,
    },
  },
  consoleRecord: {
    consoleMessageArray: [||],
    consoleCheckedCount: 0,
  },
  sceneViewRecord: {
    viewRect: None,
    gridPlane: None,
    editCamera: None,
  },
  gameViewRecord: {
    viewRect: None,
    activedBasicCameraView: None,
  },
  eventRecord: {
    eventTarget: EventType.Other,
  },
  imguiRecord: {
    gameViewIMGUIFunc: None,
    gameViewCustomData: None,
  },
  inspectorRecord: {
    componentTypeMap: WonderCommonlib.SparseMapService.createEmpty(),
  },
  uiRecord: {
    isFileInputOpen: false,
  },
  transformRecord: {
    localEulerAngleMapX: WonderCommonlib.SparseMapService.createEmpty(),
    localEulerAngleMapY: WonderCommonlib.SparseMapService.createEmpty(),
    localEulerAngleMapZ: WonderCommonlib.SparseMapService.createEmpty(),
  },
  currentDragSource: (None, None),
  currentSelectSource: None,
  loopId: (-1),
};