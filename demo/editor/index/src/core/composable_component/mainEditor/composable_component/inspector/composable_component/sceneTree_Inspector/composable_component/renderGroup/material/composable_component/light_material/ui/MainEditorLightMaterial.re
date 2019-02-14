open Color;

open ColorType;

module Method = {
  let getColor = (materialComponent, ()) =>
    LightMaterialEngineService.getLightMaterialDiffuseColor(materialComponent)
    |> StateLogicService.getEngineStateToGetData
    |> getHexString;

  let changeColor = (materialComponent, value) =>
    value
    |> convertColorObjToColorPickType
    |> getEngineColorRgbArr
    |> LightMaterialEngineService.setLightMaterialDiffuseColor(
         _,
         materialComponent,
       )
    |> StateLogicService.getAndRefreshEngineStateWithFunc;

  let closeColorPick = LightMaterialCloseColorPickEventHandler.MakeEventHandler.pushUndoStackWithCopiedEngineState;

  let onDrop = LightMaterialDragTextureEventHandler.MakeEventHandler.pushUndoStackWithNoCopyEngineState;

  let removeTexture = ((uiState, dispatchFunc), (), materialComponent) =>
    switch (
      LightMaterialEngineService.getLightMaterialDiffuseMap(materialComponent)
      |> StateLogicService.getEngineStateToGetData
    ) {
    | None => ()
    | Some(_mapId) =>
      LightMaterialRemoveTextureEventHandler.MakeEventHandler.pushUndoStackWithNoCopyEngineState(
        (uiState, dispatchFunc),
        (),
        materialComponent,
      )
    };

  let blurShininessEvent =
      ((uiState, dispatchFunc), materialComponent, shininessValue) =>
    LightMaterialEngineService.getLightMaterialShininess(materialComponent)
    |> StateLogicService.getEngineStateToGetData
    |> ValueService.isValueEqual(ValueType.Float, shininessValue) ?
      () :
      LightMaterialShininessBlurEventHandler.MakeEventHandler.pushUndoStackWithCopiedEngineState(
        (uiState, dispatchFunc),
        materialComponent,
        shininessValue,
      );

  let changeShininess = (materialComponent, value) =>
    LightMaterialEngineService.setLightMaterialShininess(
      value,
      materialComponent,
    )
    |> StateLogicService.getAndRefreshEngineStateWithFunc;
};

let component = ReasonReact.statelessComponent("MainEditorLightMaterial");

let render = ((uiState, dispatchFunc), materialComponent, _self) =>
  <article className="wonder-light-material">
    <PickColorComponent
      label="Diffcuse color"
      getColorFunc=(Method.getColor(materialComponent))
      changeColorFunc=(Method.changeColor(materialComponent))
      closeColorPickFunc=(
        Method.closeColorPick((uiState, dispatchFunc), materialComponent)
      )
    />
    <MainEditorMaterialMap
      uiState
      dispatchFunc
      materialComponent
      label="Diffuse map"
      getMapFunc=LightMaterialEngineService.getLightMaterialDiffuseMap
      removeTextureFunc=Method.removeTexture
      onDropFunc=Method.onDrop
      isShowTextureGroup=false
    />
    <MainEditorFloatInputBaseComponent
      label="Shininess"
      getComponentValueFunc=(
        LightMaterialEngineService.getLightMaterialShininess(
          materialComponent,
        )
      )
      changeComponentValueFunc=(Method.changeShininess(materialComponent))
      blurValueFunc=(
        Method.blurShininessEvent((uiState, dispatchFunc), materialComponent)
      )
    />
  </article>;

let make =
    (~uiState: AppStore.appState, ~dispatchFunc, ~materialComponent, _children) => {
  ...component,
  render: self => render((uiState, dispatchFunc), materialComponent, self),
};