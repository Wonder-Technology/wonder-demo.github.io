let _generateWDB = (rootGameObject, engineState) => {
  let isRun = SceneEditorService.getIsRun(StateEditorService.getState());
  let engineState =
    isRun ?
      engineState :
      engineState
      |> ArcballCameraControllerLogicService.bindGameViewActiveCameraArcballCameraControllerEvent;

  /* TODO optimize: pass imageUint8ArrayMap?(imageUint8ArrayMap can't work???need fix or not pass?) */
  let (engineState, _, wdbArrayBuffer) =
    GenerateSceneGraphEngineService.generateWDB(
      rootGameObject,
      Js.Nullable.null,
      engineState,
    );

  let engineState =
    isRun ?
      engineState :
      engineState
      |> ArcballCameraControllerLogicService.unbindGameViewActiveCameraArcballCameraControllerEvent;

  (engineState, wdbArrayBuffer);
};

let _generateSceneWDB = engineState =>
  _generateWDB(
    SceneEngineService.getSceneGameObject(engineState),
    engineState,
  );

let _download = [%bs.raw
  (content, filename, mimeType) => {|
   var blob = null;

  var eleLink = document.createElement('a');
  eleLink.download = filename;
  eleLink.style.display = 'none';

  if (!!!mimeType || mimeType.length === 0) {
      blob = new Blob([content]);
  }
  else {
      blob = new Blob([content], { type: mimeType });
  }

  eleLink.href = URL.createObjectURL(blob);

  document.body.appendChild(eleLink);
  eleLink.click();

  document.body.removeChild(eleLink);

  |}
];

let exportPackage = () => {
  let editorState = StateEditorService.getState();
  let engineState = StateEngineService.unsafeGetState();

  let (engineState, sceneGraphArrayBuffer) = _generateSceneWDB(engineState);

  let asbArrayBuffer =
    HeaderExportASBUtils.generateASB(editorState, engineState);

  let wpkArrayBuffer =
    HeaderExportWPKUtils.generateWPK(sceneGraphArrayBuffer, asbArrayBuffer);

  editorState |> StateEditorService.setState |> ignore;
  engineState |> StateEngineService.setState |> ignore;

  _download(wpkArrayBuffer, "package" ++ WPKUtils.getExtName(), "");
};