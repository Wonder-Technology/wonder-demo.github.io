open Wonderjs;

let create = LightMaterialAPI.createLightMaterial;

let unsafeGetLightMaterialGameObject = LightMaterialAPI.unsafeGetLightMaterialGameObject;

let disposeLightMaterial = (materialArr, state) =>
  LightMaterialAPI.batchDisposeLightMaterial(state, materialArr);

let unsafeGetLightMaterialName = LightMaterialAPI.unsafeGetLightMaterialName;

let setLightMaterialName = LightMaterialAPI.setLightMaterialName;

let getLightMaterialDiffuseColor = LightMaterialAPI.getLightMaterialDiffuseColor;

let setLightMaterialDiffuseColor = (color, material, engineState) =>
  engineState
  |> LightMaterialAPI.setLightMaterialDiffuseColor(material, color);

let getLightMaterialShininess = LightMaterialAPI.getLightMaterialShininess;

let setLightMaterialShininess = (shininess, material, engineState) =>
  LightMaterialAPI.setLightMaterialShininess(
    material,
    shininess,
    engineState,
  );

let unsafeGetLightMaterialDiffuseMap = LightMaterialAPI.unsafeGetLightMaterialDiffuseMap;

let getLightMaterialDiffuseMap = ManageMapLightMaterialMainService.getDiffuseMap;

let setLightMaterialDiffuseMap = (map, material, engineState) =>
  engineState |> LightMaterialAPI.setLightMaterialDiffuseMap(material, map);

let hasLightMaterialDiffuseMap = LightMaterialAPI.hasLightMaterialDiffuseMap;

let unsafeGetLightMaterialName = LightMaterialAPI.unsafeGetLightMaterialName;

let setLightMaterialName = LightMaterialAPI.setLightMaterialName;

let hasLightMaterialSpecularMap = LightMaterialAPI.hasLightMaterialSpecularMap;

/* let getLightMaterialSpecularColor = LightMaterialAPI.getLightMaterialSpecularColor;

   let setLightMaterialSpecularColor = LightMaterialAPI.setLightMaterialSpecularColor; */

let reInitMaterials = LightMaterialAPI.reInitMaterials;