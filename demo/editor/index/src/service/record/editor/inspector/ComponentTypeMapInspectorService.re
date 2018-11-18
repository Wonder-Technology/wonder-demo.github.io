open InspectorType;

let getComponentTypeMap = inspectorRecord => inspectorRecord.componentTypeMap;

let setComponentTypeMap = (componentTypeMap, inspectorRecord) => {
  ...inspectorRecord,
  componentTypeMap,
};

let clearComponentTypeMap = inspectorRecord => {
  ...inspectorRecord,
  componentTypeMap: WonderCommonlib.SparseMapService.createEmpty(),
};

let _getAddedComponentTypeArr = (index, componentType, inspectorRecord) =>
  switch (
    inspectorRecord.componentTypeMap
    |> WonderCommonlib.SparseMapService.get(index)
  ) {
  | None => ArrayService.create() |> ArrayService.push(componentType)
  | Some(map) => map |> Js.Array.copy |> ArrayService.push(componentType)
  };

let addComponentTypeToMap = (index, componentType, inspectorRecord) => {
  ...inspectorRecord,
  componentTypeMap:
    inspectorRecord.componentTypeMap
    |> SparseMapService.immutableSet(
         index,
         _getAddedComponentTypeArr(index, componentType, inspectorRecord),
       ),
};

let removeComponentTypeToMap = (index, componentType, inspectorRecord) => {
  ...inspectorRecord,
  componentTypeMap:
    switch (
      inspectorRecord.componentTypeMap
      |> WonderCommonlib.SparseMapService.get(index)
    ) {
    | Some(componentTypeArr) =>
      inspectorRecord.componentTypeMap
      |> SparseMapService.immutableSet(
           index,
           componentTypeArr
           |> Js.Array.filter(componentTypeItem =>
                componentTypeItem != componentType
              ),
         )
    | None => inspectorRecord.componentTypeMap
    },
};

let clearComponentType = inspectorRecord => {
  ...inspectorRecord,
  componentTypeMap: WonderCommonlib.SparseMapService.createEmpty(),
};