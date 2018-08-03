open DiffType;

type state = {
  distance: float,
  minDistance: float,
};

type action =
  | CameraBlurDistance(float)
  | CameraBlurMinDistance(float);

module Method = {
  let blurArcbalCameraDistance =
      ((store, dispatchFunc), arcballCameraController, distance) =>
    ArcballCameraEngineService.unsafeGetArcballCameraControllerDistance(
      arcballCameraController,
    )
    |> StateLogicService.getEngineStateToGetData
    |> ValueService.isValueEqual(ValueType.Float, distance) ?
      () :
      ArcballCameraDistanceEventHandler.MakeEventHandler.pushUndoStackWithCopiedEngineState(
        (store, dispatchFunc),
        arcballCameraController,
        distance,
      );

  let blurArcbalCameraMinDistance =
      ((store, dispatchFunc), arcballCameraController, minDistance) =>
    ArcballCameraEngineService.unsafeGetArcballCameraControllerMinDistance(
      arcballCameraController,
    )
    |> StateLogicService.getEngineStateToGetData
    |> ValueService.isValueEqual(ValueType.Float, minDistance) ?
      () :
      ArcballCameraMinDistanceEventHandler.MakeEventHandler.pushUndoStackWithCopiedEngineState(
        (store, dispatchFunc),
        arcballCameraController,
        minDistance,
      );

  let changeDistance = (arcballCameraController, value) =>
    ArcballCameraEngineService.setArcballCameraControllerDistance(value)
    |> StateLogicService.getAndRefreshEngineStateWithDiff([|
         {
           arguments: [|arcballCameraController|],
           type_: ArcballCameraController,
         },
       |]);

  let changeMinDistance = (arcballCameraController, value) =>
    ArcballCameraEngineService.setArcballCameraControllerMinDistance(value)
    |> StateLogicService.getAndRefreshEngineStateWithDiff([|
         {
           arguments: [|arcballCameraController|],
           type_: ArcballCameraController,
         },
       |]);
};

let component = ReasonReact.reducerComponent("MainEditorTransform");

let reducer = ((store, dispatchFunc), arcballCameraController, action, state) =>
  switch (action) {
  | CameraBlurDistance(distance) =>
    Method.blurArcbalCameraDistance(
      (store, dispatchFunc),
      arcballCameraController,
      state.distance,
    );

    ReasonReact.Update({...state, distance});

  | CameraBlurMinDistance(minDistance) =>
    Method.blurArcbalCameraMinDistance(
      (store, dispatchFunc),
      arcballCameraController,
      state.minDistance,
    );

    ReasonReact.Update({...state, minDistance});
  };

let render =
    (
      (store, dispatchFunc),
      arcballCameraController,
      {state, send}: ReasonReact.self('a, 'b, 'c),
    ) =>
  <article className="wonder-inspector-arcballCameraController">
    <FloatInput
      label="distance"
      defaultValue=(state.distance |> StringService.floatToString)
      onChange=(Method.changeDistance(arcballCameraController))
      onBlur=(value => send(CameraBlurDistance(value)))
    />
    <FloatInput
      label="min distance"
      defaultValue=(state.minDistance |> StringService.floatToString)
      onChange=(Method.changeMinDistance(arcballCameraController))
      onBlur=(value => send(CameraBlurMinDistance(value)))
    />
  </article>;

let make =
    (
      ~store: AppStore.appState,
      ~dispatchFunc,
      ~arcballCameraController,
      _children,
    ) => {
  ...component,
  initialState: () => {
    let engineStateToGetData = StateLogicService.getRunEngineState();
    {
      distance:
        engineStateToGetData
        |> ArcballCameraEngineService.unsafeGetArcballCameraControllerDistance(
             arcballCameraController,
           ),
      minDistance:
        engineStateToGetData
        |> ArcballCameraEngineService.unsafeGetArcballCameraControllerMinDistance(
             arcballCameraController,
           ),
    };
  },
  reducer: reducer((store, dispatchFunc), arcballCameraController),
  render: self =>
    render((store, dispatchFunc), arcballCameraController, self),
};