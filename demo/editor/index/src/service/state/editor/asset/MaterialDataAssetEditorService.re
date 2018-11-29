open EditorType;

open AssetMaterialDataType;

let getMaterialData = editorState =>
  editorState.assetRecord |> MaterialDataAssetService.getMaterialData;

let unsafeGetDefaultBasicMaterialData = editorState =>
  getMaterialData(editorState).defaultBasicMaterialData
  |> OptionService.unsafeGet;

let unsafeGetDefaultBasicMaterial = editorState => {
  let (material, _) = unsafeGetDefaultBasicMaterialData(editorState);

  material;
};

let setDefaultBasicMaterialData = (material, editorState) => {
  ...editorState,
  assetRecord: {
    ...editorState.assetRecord,
    materialData: {
      ...getMaterialData(editorState),
      defaultBasicMaterialData: Some((material, BasicMaterial)),
    },
  },
};

let unsafeGetDefaultLightMaterialData = editorState =>
  getMaterialData(editorState).defaultLightMaterialData
  |> OptionService.unsafeGet;

let unsafeGetDefaultLightMaterial = editorState => {
  let (material, _) = unsafeGetDefaultLightMaterialData(editorState);

  material;
};

let unsafeGetMaterialDataByType = (type_, editorState) => {
  let unsafeGetMaterialDataFunc =
    switch (type_) {
    | BasicMaterial => unsafeGetDefaultBasicMaterialData
    | LightMaterial => unsafeGetDefaultLightMaterialData
    };

  unsafeGetMaterialDataFunc(editorState);
};

let setDefaultLightMaterialData = (material, editorState) => {
  ...editorState,
  assetRecord: {
    ...editorState.assetRecord,
    materialData: {
      ...getMaterialData(editorState),
      defaultLightMaterialData: Some((material, LightMaterial)),
    },
  },
};

let getAllDefaultMaterialData = editorState => [|
  unsafeGetDefaultBasicMaterialData(editorState),
  unsafeGetDefaultLightMaterialData(editorState),
|];
