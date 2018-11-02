module Method = {
  let blurPositionEvent =
      ((store, dispatchFunc), transformComponent, (x, y, z)) => {
    let (newX, newY, newZ) =
      TransformUtils.getTransformPositionData(transformComponent);

    TransformUtils.isTransformVec3Equal((x, y, z), (newX, newY, newZ)) ?
      () :
      PositionBlurEventHandler.MakeEventHandler.pushUndoStackWithCopiedEngineState(
        (store, dispatchFunc),
        transformComponent,
        (x, y, z),
      );
  };

  let blurRotationEvent =
      ((store, dispatchFunc), transformComponent, (x, y, z)) => {
    let (newX, newY, newZ) =
      TransformUtils.getTransformRotationData(transformComponent);

    TransformUtils.isTransformVec3Equal((x, y, z), (newX, newY, newZ)) ?
      () :
      RotationBlurEventHandler.MakeEventHandler.pushUndoStackWithCopiedEngineState(
        (store, dispatchFunc),
        transformComponent,
        (x, y, z),
      );
  };

  let blurScaleEvent =
      ((store, dispatchFunc), transformComponent, (x, y, z)) => {
    let (newX, newY, newZ) =
      TransformUtils.getTransformScaleData(transformComponent);

    TransformUtils.isTransformVec3Equal((x, y, z), (newX, newY, newZ)) ?
      () :
      ScaleBlurEventHandler.MakeEventHandler.pushUndoStackWithCopiedEngineState(
        (store, dispatchFunc),
        transformComponent,
        (x, y, z),
      );
  };

  let _setCurrentSceneTreeNodeLocalPosition = (transformComponent, (x, y, z)) =>
    TransformEngineService.setLocalPosition((x, y, z), transformComponent)
    |> StateLogicService.getAndRefreshEngineStateWithFunc;

  let changePositionX = (transformComponent, value) => {
    let (_x, y, z) =
      TransformUtils.getSceneTreeNodeLocalPosition(transformComponent);

    _setCurrentSceneTreeNodeLocalPosition(transformComponent, (value, y, z));
  };

  let changePositionY = (transformComponent, value) => {
    let (x, _y, z) =
      TransformUtils.getSceneTreeNodeLocalPosition(transformComponent);

    _setCurrentSceneTreeNodeLocalPosition(transformComponent, (x, value, z));
  };

  let changePositionZ = (transformComponent, value) => {
    let (x, y, _z) =
      TransformUtils.getSceneTreeNodeLocalPosition(transformComponent);

    _setCurrentSceneTreeNodeLocalPosition(transformComponent, (x, y, value));
  };

  let _setCurrentSceneTreeNodeLocalScale = (transformComponent, (x, y, z)) =>
    TransformEngineService.setLocalScale((x, y, z), transformComponent)
    |> StateLogicService.getAndRefreshEngineStateWithFunc;

  let changeScaleX = (transformComponent, value) => {
    let (_x, y, z) =
      TransformEngineService.getLocalScale(transformComponent)
      |> StateLogicService.getEngineStateToGetData;

    _setCurrentSceneTreeNodeLocalScale(transformComponent, (value, y, z));
  };

  let changeScaleY = (transformComponent, value) => {
    let (x, _y, z) =
      TransformEngineService.getLocalScale(transformComponent)
      |> StateLogicService.getEngineStateToGetData;

    _setCurrentSceneTreeNodeLocalScale(transformComponent, (x, value, z));
  };

  let changeScaleZ = (transformComponent, value) => {
    let (x, y, _z) =
      TransformEngineService.getLocalScale(transformComponent)
      |> StateLogicService.getEngineStateToGetData;

    _setCurrentSceneTreeNodeLocalScale(transformComponent, (x, y, value));
  };

  let _setCurrentSceneTreeNodeLocalRotation = (transformComponent, (x, y, z)) =>
    TransformEngineService.setLocalEulerAngles((x, y, z), transformComponent)
    |> StateLogicService.getAndRefreshEngineStateWithFunc;

  let changeRotationX = (transformComponent, value) => {
    let (_x, y, z) =
      TransformEngineService.getLocalEulerAngles(transformComponent)
      |> StateLogicService.getEngineStateToGetData;

    _setCurrentSceneTreeNodeLocalRotation(transformComponent, (value, y, z));
  };

  let changeRotationY = (transformComponent, value) => {
    let (x, _y, z) =
      TransformEngineService.getLocalEulerAngles(transformComponent)
      |> StateLogicService.getEngineStateToGetData;

    _setCurrentSceneTreeNodeLocalRotation(transformComponent, (x, value, z));
  };

  let changeRotationZ = (transformComponent, value) => {
    let (x, y, _z) =
      TransformEngineService.getLocalEulerAngles(transformComponent)
      |> StateLogicService.getEngineStateToGetData;

    _setCurrentSceneTreeNodeLocalRotation(transformComponent, (x, y, value));
  };

  let buildShadeComponent = gameObject =>
    StateEngineService.unsafeGetState()
    |> GameObjectComponentEngineService.hasArcballCameraControllerComponent(
         gameObject,
       ) ?
      <div className="transform-shade" /> : ReasonReact.null;
};

let component = ReasonReact.statelessComponent("MainEditorTransform");

let render =
    ((store, dispatchFunc), (transformComponent, gameObject), _self) =>
  <article className="wonder-inspector-transform">
    <div className="inspector-item">
      <div className="item-header"> (DomHelper.textEl("Position")) </div>

      <div className="item-content">
      
      <TransformTemplate
        store
        dispatchFunc
        transformComponent
        changeXFunc=Method.changePositionX
        changeYFunc=Method.changePositionY
        changeZFunc=Method.changePositionZ
        getDataFunc=TransformUtils.getTransformPositionData
        blurEventFunc=Method.blurPositionEvent
        canBeZero=true
      />
      </div>

    </div>
    <div className="inspector-item">
      <div className="item-header"> (DomHelper.textEl("Rotation")) </div>
      <div className="item-content">
      
      <TransformTemplate
        store
        dispatchFunc
        transformComponent
        changeXFunc=Method.changeRotationX
        changeYFunc=Method.changeRotationY
        changeZFunc=Method.changeRotationZ
        getDataFunc=TransformUtils.getTransformRotationData
        blurEventFunc=Method.blurRotationEvent
        canBeZero=true
      />
      </div> 
    </div>
    <div className="inspector-item">
      <div className="item-header"> (DomHelper.textEl("Scale")) </div>
      <div className="item-content">
      
      <TransformTemplate
        store
        dispatchFunc
        transformComponent
        changeXFunc=Method.changeScaleX
        changeYFunc=Method.changeScaleY
        changeZFunc=Method.changeScaleZ
        getDataFunc=TransformUtils.getTransformScaleData
        blurEventFunc=Method.blurScaleEvent
        canBeZero=false
      />
      </div> 
    </div>
    (Method.buildShadeComponent(gameObject))
  </article>;

let make =
    (
      ~store: AppStore.appState,
      ~dispatchFunc,
      ~transformComponent,
      ~gameObject,
      _children,
    ) => {
  ...component,
  render: self =>
    render((store, dispatchFunc), (transformComponent, gameObject), self),
};