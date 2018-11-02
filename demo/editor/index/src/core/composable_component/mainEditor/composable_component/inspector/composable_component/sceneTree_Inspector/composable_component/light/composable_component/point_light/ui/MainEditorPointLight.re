open Color;

open ColorType;

let component = ReasonReact.statelessComponent("MainEditorPointLight");

module Method = {
  let getColor = (lightComponent, ()) =>
    PointLightEngineService.getPointLightColor(lightComponent)
    |> StateLogicService.getEngineStateToGetData
    |> getHexString;

  let changeColor = (lightComponent, value) =>
    value
    |> convertColorObjToColorPickType
    |> getEngineColorRgbArr
    |> PointLightEngineService.setPointLightColor(_, lightComponent)
    |> StateLogicService.getAndRefreshEngineStateWithFunc;

  let closeColorPick = PointLightCloseColorPickEventHandler.MakeEventHandler.pushUndoStackWithCopiedEngineState;

  let buildColorPickComponent = ((store, dispatchFunc), lightComponent) =>
    <PickColorComponent
      label="Color"
      getColorFunc=(getColor(lightComponent))
      changeColorFunc=(changeColor(lightComponent))
      closeColorPickFunc=(
        closeColorPick((store, dispatchFunc), lightComponent)
      )
    />;

  let buildIntensityComponent = ((store, dispatchFunc), lightComponent) =>
    <MainEditorFloatInputBaseComponent
      label="Intensity"
      getComponentValueFunc=(
        PointLightEngineService.getPointLightIntensity(lightComponent)
      )
      changeComponentValueFunc=(
        MainEditorPointLightUtils.changeIntensity(lightComponent)
      )
      blurValueFunc=(
        MainEditorPointLightUtils.blurIntensityEvent(
          (store, dispatchFunc),
          lightComponent,
        )
      )
    />;

  let buildConstantComponent = ((store, dispatchFunc), lightComponent) =>
    <MainEditorFloatInputBaseComponent
      label="Constant"
      getComponentValueFunc=(
        PointLightEngineService.getPointLightConstant(lightComponent)
      )
      changeComponentValueFunc=(
        MainEditorPointLightUtils.changeConstant(lightComponent)
      )
      blurValueFunc=(
        MainEditorPointLightUtils.blurConstantEvent(
          (store, dispatchFunc),
          lightComponent,
        )
      )
    />;

  let buildLinearComponent = ((store, dispatchFunc), lightComponent) =>
    <MainEditorFloatInputBaseComponent
      label="Linear"
      getComponentValueFunc=(
        PointLightEngineService.getPointLightLinear(lightComponent)
      )
      changeComponentValueFunc=(
        MainEditorPointLightUtils.changeLinear(lightComponent)
      )
      blurValueFunc=(
        MainEditorPointLightUtils.blurLinearEvent(
          (store, dispatchFunc),
          lightComponent,
        )
      )
    />;

  let buildQuadraticComponent = ((store, dispatchFunc), lightComponent) =>
    <MainEditorFloatInputBaseComponent
      label="Quadratic"
      getComponentValueFunc=(
        PointLightEngineService.getPointLightQuadratic(lightComponent)
      )
      changeComponentValueFunc=(
        MainEditorPointLightUtils.changeQuadratic(lightComponent)
      )
      blurValueFunc=(
        MainEditorPointLightUtils.blurQuadraticEvent(
          (store, dispatchFunc),
          lightComponent,
        )
      )
    />;

  let buildRangeComponent = ((store, dispatchFunc), lightComponent) =>
    <MainEditorFloatInputBaseComponent
      label="Range"
      getComponentValueFunc=(
        PointLightEngineService.getPointLightRange(lightComponent)
      )
      changeComponentValueFunc=(
        MainEditorPointLightUtils.changeRange(lightComponent)
      )
      blurValueFunc=(
        MainEditorPointLightUtils.blurRangeEvent(
          (store, dispatchFunc),
          lightComponent,
        )
      )
    />;
};

let render = ((store, dispatchFunc), lightComponent, _self) =>
  <article className="wonder-point-light">
    (Method.buildColorPickComponent((store, dispatchFunc), lightComponent))
    (Method.buildIntensityComponent((store, dispatchFunc), lightComponent))
    (Method.buildConstantComponent((store, dispatchFunc), lightComponent))
    (Method.buildLinearComponent((store, dispatchFunc), lightComponent))
    (Method.buildQuadraticComponent((store, dispatchFunc), lightComponent))
    (Method.buildRangeComponent((store, dispatchFunc), lightComponent))
  </article>;

let make =
    (~store: AppStore.appState, ~dispatchFunc, ~lightComponent, _children) => {
  ...component,
  render: self => render((store, dispatchFunc), lightComponent, self),
};