open AssetNodeType;
open FileType;
open Js.Promise;

let addFolderIntoNodeMap = (index, editorState) =>
  editorState
  |> AssetNodeEditorService.buildFolderResult(index)
  |> AssetFolderNodeMapEditorService.setResult(index, _, editorState);

let initRootAssetTree = editorState =>
  switch (AssetTreeRootEditorService.getAssetTreeRoot(editorState)) {
  | None =>
    let rootIndex = editorState |> AssetIndexEditorService.getIndex;
    (
      rootIndex |. AssetNodeEditorService.buildAssetTreeNodeByIndex(Folder),
      editorState |> addFolderIntoNodeMap(rootIndex),
    );
  | Some(assetTreeRoot) => (assetTreeRoot, editorState)
  };

let convertFileJsObjectToFileInfoRecord = fileObject => {
  name: fileObject##name,
  type_: fileObject##_type,
  file: FileType.convertFileJsObjectToFile(fileObject),
};

let getUploadFileType = type_ =>
  switch (type_) {
  | "application/json" => LoadJson
  | "image/jpeg"
  | "image/png" => LoadImage
  | _ =>
    WonderLog.Log.fatal(
      WonderLog.Log.buildFatalMessage(
        ~title="getUploadFileType",
        ~description={j|the type:$type_ not exist|j},
        ~reason="",
        ~solution={j||j},
        ~params={j||j},
      ),
    )
  };

let _handleSpecificFuncByType = (type_, (handleJsonFunc, handleImageFunc)) =>
  switch (type_) {
  | LoadJson => handleJsonFunc()
  | LoadImage => handleImageFunc()
  | _ =>
    WonderLog.Log.error(
      WonderLog.Log.buildErrorMessage(
        ~title="_handleSpecificFuncByType",
        ~description={j|the type:$type_ is not exist|j},
      ),
    )
  };

let readFileByType = (reader, fileInfo: fileInfoType) =>
  _handleSpecificFuncByType(
    getUploadFileType(fileInfo.type_),
    (
      () => FileReader.readAsText(reader, fileInfo.file),
      () => FileReader.readAsDataURL(reader, fileInfo.file),
    ),
  );

let createNodeAndAddToTargetNodeChildren =
    (targetTreeNode, newIndex, type_, editorState) =>
  editorState
  |> AssetTreeRootEditorService.unsafeGetAssetTreeRoot
  |> AssetUtils.insertSourceTreeNodeToTargetTreeNodeChildren(
       targetTreeNode,
       AssetNodeEditorService.buildAssetTreeNodeByIndex(newIndex, type_),
     )
  |. AssetTreeRootEditorService.setAssetTreeRoot(editorState);

let _handleJsonType =
    (fileResult: nodeResultType, newIndex, (resolve, editorState), ()) => {
  let editorState =
    editorState
    |> AssetJsonNodeMapEditorService.setResult(
         newIndex,
         AssetNodeEditorService.buildJsonNodeResult(fileResult),
       )
    |> createNodeAndAddToTargetNodeChildren(
         editorState |> AssetUtils.getTargetTreeNodeId,
         newIndex,
         Json,
       )
    |> StateEditorService.setState;

  resolve(. editorState);
};

let _handleImageType =
    (
      fileResult: AssetNodeType.nodeResultType,
      newIndex,
      (resolve, editorState),
      (),
    ) => {
  let (fileName, _postfix) =
    FileNameService.getBaseNameAndExtName(fileResult.name);

  let (texture, editEngineState, runEngineState) =
    TextureUtils.createAndInitTexture(
      fileName,
      StateLogicService.getEditEngineState(),
      StateLogicService.getRunEngineState(),
    );

  Image.onload(
    fileResult.result,
    loadedImg => {
      editEngineState
      |> BasicSourceTextureEngineService.setSource(
           loadedImg |> ImageType.convertDomToImageElement,
           texture,
         )
      |> StateLogicService.setEditEngineState;

      runEngineState
      |> BasicSourceTextureEngineService.setSource(
           loadedImg |> ImageType.convertDomToImageElement,
           texture,
         )
      |> StateLogicService.setRunEngineState;

      let editorState =
        editorState
        |> AssetImageBase64MapEditorService.setResult(texture, fileResult.result)
        |> AssetTextureNodeMapEditorService.setResult(
             newIndex,
             AssetNodeEditorService.buildTextureNodeResult(texture),
           )
        |> createNodeAndAddToTargetNodeChildren(
             editorState |> AssetUtils.getTargetTreeNodeId,
             newIndex,
             Texture,
           )
        |> StateEditorService.setState;

      resolve(. editorState);
    },
  );
};

let handleFileByType = (fileResult: nodeResultType) => {
  let editorState =
    AssetIndexEditorService.increaseIndex |> StateLogicService.getEditorState;
  let newIndex = editorState |> AssetIndexEditorService.getIndex;

  make((~resolve, ~reject) =>
    _handleSpecificFuncByType(
      fileResult.type_,
      (
        _handleJsonType(fileResult, newIndex, (resolve, editorState)),
        _handleImageType(fileResult, newIndex, (resolve, editorState)),
      ),
    )
  );
};