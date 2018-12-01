exception LoadException(string);

type nodeId = int;

type imageIndex = int;

type uploadFileType =
  | LoadWDB
  | LoadGLB
  | LoadGLTFZip
  | LoadTexture
  | LoadWPK
  | LoadError;

type assetNodeType =
  | Folder
  | Texture
  | WDB
  | Material;

type nodeResultType = {
  name: string,
  type_: uploadFileType,
  result: FileReader.resultType,
};

type folderResultType = {
  name: string,
  parentFolderNodeId: option(nodeId),
};

type imageResultType = {
  base64: option(string),
  uint8Array: option(Js.Typed_array.Uint8Array.t),
  blobObjectURL: option(string),
  name: string,
  mimeType: string,
};

type textureResultType = {
  textureComponent: int,
  image: imageIndex,
  parentFolderNodeId: option(nodeId),
};

type wdbResultType = {
  name: string,
  parentFolderNodeId: option(nodeId),
  wdbGameObject: int,
};

type materialResultType = {
  parentFolderNodeId: option(nodeId),
  type_: AssetMaterialDataType.materialType,
  materialComponent: int,
};