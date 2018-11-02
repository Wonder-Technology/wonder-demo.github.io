open CurrentNodeDataType;

module CustomEventHandler = {
  include EmptyEventHandler.EmptyEventHandler;
  type prepareTuple = unit;
  type dataTuple = (int, int);

  let _setFolderNodeParent =
      (folderId, parentFolderNodeId, editorState, folderNodeMap) =>
    folderNodeMap
    |> WonderCommonlib.SparseMapService.unsafeGet(folderId)
    |> AssetFolderNodeMapEditorService.setFolderNodeResultParent(
         parentFolderNodeId,
       )
    |> AssetFolderNodeMapEditorService.setResult(folderId, _, editorState)
    |> StateEditorService.setState
    |> ignore;

  let _setTextureNodeParent =
      (textureComponent, parentFolderNodeId, editorState, textureNodeMap) =>
    textureNodeMap
    |> WonderCommonlib.SparseMapService.unsafeGet(textureComponent)
    |> AssetTextureNodeMapEditorService.setTextureNodeResultParent(
         parentFolderNodeId,
       )
    |> AssetTextureNodeMapEditorService.setResult(
         textureComponent,
         _,
         editorState,
       )
    |> StateEditorService.setState
    |> ignore;

  let _setMaterialNodeParent =
      (nodeId, parentFolderNodeId, editorState, materialNodeMap) =>
    materialNodeMap
    |> WonderCommonlib.SparseMapService.unsafeGet(nodeId)
    |> AssetMaterialNodeMapEditorService.setMaterialNodeResultParent(
         parentFolderNodeId,
       )
    |> AssetMaterialNodeMapEditorService.setResult(nodeId, _, editorState)
    |> StateEditorService.setState
    |> ignore;

  let _setWDBNodeParent =
      (nodeId, parentFolderNodeId, editorState, wdbNodeMap) =>
    wdbNodeMap
    |> WonderCommonlib.SparseMapService.unsafeGet(nodeId)
    |> AssetWDBNodeMapEditorService.setWDBNodeResultParent(
         parentFolderNodeId,
       )
    |> AssetWDBNodeMapEditorService.setResult(nodeId, _, editorState)
    |> StateEditorService.setState
    |> ignore;

  let handleSelfLogic =
      ((store, dispatchFunc), (), (targetNodeId, removedNodeId)) => {
    let editorState = StateEditorService.getState();

    AssetUtils.isIdEqual(targetNodeId, removedNodeId) ?
      dispatchFunc(AppStore.UpdateAction(Update([|BottomComponent|])))
      |> ignore :
      {
        let {type_}: AssetTreeNodeType.assetTreeNodeType =
          editorState
          |> AssetTreeRootEditorService.getAssetTreeRoot
          |> OptionService.unsafeGet
          |> AssetUtils.getSpecificTreeNodeById(removedNodeId)
          |> OptionService.unsafeGet;

        let (newAssetTreeRoot, removedTreeNode) =
          editorState
          |> AssetTreeRootEditorService.unsafeGetAssetTreeRoot
          |> AssetUtils.removeSpecificTreeNode(removedNodeId);

        let editorState =
          newAssetTreeRoot
          |> AssetUtils.insertSourceTreeNodeToTargetTreeNodeChildren(
               targetNodeId,
               removedTreeNode,
             )
          |. AssetTreeRootEditorService.setAssetTreeRoot(editorState);

        AssetNodeUtils.handleSpeficFuncByAssetNodeType(
          type_,
          (
            _setFolderNodeParent(
              removedNodeId,
              targetNodeId |. Some,
              editorState,
            ),
            _setTextureNodeParent(
              removedNodeId,
              targetNodeId |. Some,
              editorState,
            ),
            _setMaterialNodeParent(
              removedNodeId,
              targetNodeId |. Some,
              editorState,
            ),
            _setWDBNodeParent(
              removedNodeId,
              targetNodeId |. Some,
              editorState,
            ),
          ),
          editorState,
        );

        dispatchFunc(AppStore.UpdateAction(Update([|BottomComponent|])))
        |> ignore;
      };
  };
};

module MakeEventHandler = EventHandler.MakeEventHandler(CustomEventHandler);