open EditorType;

let getIsRun = editorState =>
  editorState.sceneRecord |> IsRunSceneService.getIsRun;

let setIsRun = (isRun, editorState) => {
  ...editorState,
  sceneRecord: editorState.sceneRecord |> IsRunSceneService.setIsRun(isRun),
};

let unsafeGetDiffMap = editorState =>
  editorState.sceneRecord |> DiffMapSceneService.unsafeGetDiffMap;

let setDiffMap = (diffMap, editorState) => {
  ...editorState,
  sceneRecord:
    editorState.sceneRecord |> DiffMapSceneService.setDiffMap(diffMap),
};

let unsafeGetCurrentSceneTreeNode = editorState =>
  editorState.sceneRecord
  |> CurrentSceneTreeNodeSceneService.unsafeGetCurrentSceneTreeNode;

let getCurrentSceneTreeNode = editorState =>
  editorState.sceneRecord
  |> CurrentSceneTreeNodeSceneService.getCurrentSceneTreeNode;

let setCurrentSceneTreeNode = (gameObject, editorState) => {
  ...editorState,
  sceneRecord:
    editorState.sceneRecord
    |> CurrentSceneTreeNodeSceneService.setCurrentSceneTreeNode(gameObject),
};

let clearCurrentSceneTreeNode = editorState => {
  ...editorState,
  sceneRecord:
    editorState.sceneRecord
    |> CurrentSceneTreeNodeSceneService.clearCurrentSceneTreeNode,
};