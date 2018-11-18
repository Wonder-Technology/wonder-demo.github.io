open SettingType;

open SceneType;

open AssetType;

open SceneViewType;

open GameViewType;

open EventType;

open IMGUIType;

open InspectorType;

open UIType;

open ConsoleType;

open TransformType;

type widgetType =
  | SceneTree
  | Asset;

type editorState = {
  settingRecord,
  sceneRecord,
  assetRecord,
  sceneViewRecord,
  gameViewRecord,
  eventRecord,
  imguiRecord,
  inspectorRecord,
  uiRecord,
  consoleRecord,
  transformRecord,
  currentDragSource: (option(widgetType), option(int)),
  currentSelectSource: option(widgetType),
  loopId: int,
};