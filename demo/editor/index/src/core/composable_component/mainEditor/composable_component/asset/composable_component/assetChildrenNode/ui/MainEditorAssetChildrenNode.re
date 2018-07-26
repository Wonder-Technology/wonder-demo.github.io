open AssetNodeType;

open AssetTreeNodeType;

open CurrentNodeDataType;

module Method = {
  let _isSelected = (currentNodeData, nodeId) =>
    switch (currentNodeData) {
    | None => false
    | Some({currentNodeId}) => AssetUtils.isIdEqual(nodeId, currentNodeId)
    };

  let showSpecificTreeNodeChildren =
      (
        (store, dispatchFunc),
        (dragImg, debounceTime, currentNodeData),
        editorState,
        assetTreeNodeChildrenArr,
      ) =>
    assetTreeNodeChildrenArr
    |> Js.Array.map(({id as nodeId, type_}) =>
         switch (type_) {
         | Folder =>
           let {name}: folderResultType =
             editorState
             |> AssetFolderNodeMapEditorService.getFolderNodeMap
             |> WonderCommonlib.SparseMapService.unsafeGet(nodeId);

           <FolderBox
             key=(DomHelper.getRandomKey())
             store
             dispatchFunc
             dragImg
             imgSrc="./public/img/11.jpg"
             folderId=nodeId
             fileType=type_
             name
             isSelected=(_isSelected(currentNodeData, nodeId))
             flag=(AssetUtils.getFlag())
             debounceTime
             onDrop=(
               AssetTreeUtils.dragNodeToFolderFunc((store, dispatchFunc), ())
             )
             isFlag=AssetUtils.isFlag
             handleRelationError=AssetUtils.isTreeNodeRelationError
           />;
         | Texture =>
           let {textureIndex} =
             editorState
             |> AssetTextureNodeMapEditorService.getTextureNodeMap
             |> WonderCommonlib.SparseMapService.unsafeGet(nodeId);

           <FileBox
             key=(DomHelper.getRandomKey())
             store
             dispatchFunc
             dragImg
             imgSrc=(
               editorState
               |> AssetImageBase64MapEditorService.getImageBase64Map
               |> WonderCommonlib.SparseMapService.unsafeGet(textureIndex)
             )
             fileId=nodeId
             fileType=type_
             fileName=(
               BasicSourceTextureEngineService.unsafeGetBasicSourceTextureName(
                 textureIndex,
               )
               |> StateLogicService.getEngineStateToGetData
             )
             flag=(AssetUtils.getFlag())
             isSelected=(_isSelected(currentNodeData, nodeId))
           />;
         | Json =>
           let {name}: jsonResultType =
             editorState
             |> AssetJsonNodeMapEditorService.getJsonNodeMap
             |> WonderCommonlib.SparseMapService.unsafeGet(nodeId);

           <FileBox
             key=(DomHelper.getRandomKey())
             store
             dispatchFunc
             dragImg
             imgSrc="./public/img/12.jpg"
             fileId=nodeId
             fileType=type_
             fileName=name
             flag=(AssetUtils.getFlag())
             isSelected=(_isSelected(currentNodeData, nodeId))
           />;
         | _ =>
           WonderLog.Log.fatal(
             WonderLog.Log.buildFatalMessage(
               ~title="showSpecificTreeNodeChildren",
               ~description={j|unknown type_: $type_|j},
               ~reason="",
               ~solution={j||j},
               ~params={j||j},
             ),
           )
         }
       );

  let buildCurrentTreeNodeChildrenComponent =
      ((store, dispatchFunc), dragImg, debounceTime) => {
    let editorState = StateEditorService.getState();

    editorState
    |> AssetTreeRootEditorService.unsafeGetAssetTreeRoot
    |> AssetUtils.getSpecificTreeNodeById(
         editorState |> AssetUtils.getTargetTreeNodeId,
       )
    |> OptionService.unsafeGet
    |> (currentParentNode => currentParentNode.children)
    |> showSpecificTreeNodeChildren(
         (store, dispatchFunc),
         (
           dragImg,
           debounceTime,
           editorState |> AssetCurrentNodeDataEditorService.getCurrentNodeData,
         ),
         editorState,
       );
  };
};

let component = ReasonReact.statelessComponent("MainEditorAssetHeader");

let render = ((store, dispatchFunc), dragImg, debounceTime, _self) =>
  <article key="assetChildrenNode" className="wonder-asset-assetChildren">
    (
      ReasonReact.arrayToElement(
        Method.buildCurrentTreeNodeChildrenComponent(
          (store, dispatchFunc),
          dragImg,
          debounceTime,
        ),
      )
    )
  </article>;

let make = (~store, ~dispatchFunc, ~dragImg, ~debounceTime, _children) => {
  ...component,
  render: self => render((store, dispatchFunc), dragImg, debounceTime, self),
};