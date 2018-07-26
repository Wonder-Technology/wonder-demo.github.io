let prepareSpecificGameObjectsForEditEngineState = (scene, engineStateForEdit) => {
  let (engineState, camera) =
    CameraEngineService.createCamera(engineStateForEdit);
  let (engineState, box) = PrimitiveEngineService.createBox(engineState);

  let engineState =
    engineState
    |> TransformEngineService.setLocalPosition(
         (20., 0., 100.),
         GameObjectComponentEngineService.getTransformComponent(
           camera,
           engineState,
         ),
       )
    |> LightMaterialEngineService.setLightMaterialDiffuseColor(
         [|1., 0.1, 0.1|],
         GameObjectComponentEngineService.getLightMaterialComponent(
           box,
           engineState,
         ),
       )
    |> GameObjectUtils.addChild(scene, camera)
    |> GameObjectUtils.addChild(scene, box)
    |> SceneEngineService.setCurrentCameraGameObject(camera);
  (engineState, box);
};

let computeDiffValue = (editorState, engineState) => {
  let diffMap =
    WonderCommonlib.HashMapService.createEmpty()
    |> WonderCommonlib.HashMapService.set("gameObject", 2)
    |> WonderCommonlib.HashMapService.set("transform", 2)
    |> WonderCommonlib.HashMapService.set("meshRenderer", 2)
    |> WonderCommonlib.HashMapService.set("basicMaterial", 0)
    |> WonderCommonlib.HashMapService.set("lightMaterial", 1)
    |> WonderCommonlib.HashMapService.set("directionLight", 0)
    |> WonderCommonlib.HashMapService.set("texture", 0);

  (editorState |> SceneEditorService.setDiffMap(diffMap), engineState);
};

let createDefaultScene = (scene, engineState) => {
  let (engineState, camera, box1, box2, directionLight) =
    SceneEngineService.createDefaultSceneGameObjects(
      engineState,
      CameraEngineService.createCamera,
    );

  (
    engineState
    |> TransformEngineService.setLocalPosition(
         (0., 0., 40.),
         GameObjectComponentEngineService.getTransformComponent(
           camera,
           engineState,
         ),
       )
    |> TransformEngineService.setLocalPosition(
         (10., 4., 10.),
         GameObjectComponentEngineService.getTransformComponent(
           directionLight,
           engineState,
         ),
       )
    |> GameObjectUtils.addChild(scene, camera)
    |> GameObjectUtils.addChild(scene, box1)
    |> GameObjectUtils.addChild(scene, box2)
    |> GameObjectUtils.addChild(scene, directionLight),
    camera,
  );
};