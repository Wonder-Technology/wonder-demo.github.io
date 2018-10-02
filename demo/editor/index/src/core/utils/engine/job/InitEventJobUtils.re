open EventType;

let _loopBodyWhenStop = engineState =>
  SceneEditorService.getIsRun |> StateLogicService.getEditorState ?
    engineState : engineState |> DirectorEngineService.loopBody(0.);

let _triggerRefreshInspectorEvent = engineState => {
  let (engineState, _) =
    ManageEventEngineService.triggerCustomGlobalEvent(
      CreateCustomEventEngineService.create(
        EventEditorService.getRefreshInspectorEventName(),
        None,
      ),
      engineState,
    );

  engineState;
};

let _getBody = () => DomHelper.document##body |> bodyToEventTarget;

let _isTriggerGameViewEvent = () =>
  EventEditorService.getEventTarget(StateEditorService.getState()) === Game;

let _isTriggerSceneViewEvent = () =>
  EventEditorService.getEventTarget(StateEditorService.getState()) === Scene;

let _isTriggerOtherEvent = () =>
  EventEditorService.getEventTarget(StateEditorService.getState()) === Other;

let _fromPointDomEvent = (eventName, engineState) =>
  WonderBsMost.Most.fromEvent(eventName, _getBody(), false);

let _fromKeyboardDomEvent = (eventName, engineState) =>
  WonderBsMost.Most.fromEvent(eventName, _getBody(), false);

let _convertMouseEventToPointEvent =
    (
      eventName,
      {location, locationInView, button, wheel, movementDelta, event}: mouseEvent,
    )
    : pointEvent => {
  name: eventName,
  location,
  locationInView,
  button: Some(button),
  wheel: Some(wheel),
  movementDelta,
  event: event |> mouseDomEventToPointDomEvent,
};

let _bindDomEventToTriggerPointEvent =
    (
      (domEventName, customEventName, pointEventName),
      (
        onDomEventFunc,
        convertDomEventToPointEventFunc,
        isTriggerCustomGlobalEventFunc,
      ),
      engineState,
    ) =>
  onDomEventFunc(
    ~eventName=domEventName,
    ~handleFunc=
      (. mouseEvent, engineState) =>
        isTriggerCustomGlobalEventFunc() ?
          {
            let engineState = _triggerRefreshInspectorEvent(engineState);

            let (engineState, _) =
              ManageEventEngineService.triggerCustomGlobalEvent(
                CreateCustomEventEngineService.create(
                  customEventName,
                  convertDomEventToPointEventFunc(pointEventName, mouseEvent)
                  |> pointEventToUserData
                  |. Some,
                ),
                engineState,
              );

            _loopBodyWhenStop(engineState);
          } :
          engineState,
    ~state=engineState,
    (),
  );

let _bindMouseEventToTriggerViewPointEvent =
    (
      mouseEventName,
      customEventName,
      pointEventName,
      isTriggerCustomGlobalEventFunc,
      engineState,
    ) =>
  _bindDomEventToTriggerPointEvent(
    (mouseEventName, customEventName, pointEventName),
    (
      ManageEventEngineService.onMouseEvent(~priority=0),
      _convertMouseEventToPointEvent,
      isTriggerCustomGlobalEventFunc,
    ),
    engineState,
  );

let _bindMouseEventToTriggerSceneViewPointEvent =
    (
      mouseEventName,
      customEventName,
      pointEventName,
      isTriggerCustomGlobalEventFunc,
      engineState,
    ) =>
  _bindMouseEventToTriggerViewPointEvent(
    mouseEventName,
    customEventName,
    pointEventName,
    isTriggerCustomGlobalEventFunc,
    engineState,
  );

let _bindMouseEventToTriggerGameViewPointEvent =
    (
      mouseEventName,
      customEventName,
      pointEventName,
      isTriggerCustomGlobalEventFunc,
      engineState,
    ) =>
  _bindMouseEventToTriggerViewPointEvent(
    mouseEventName,
    customEventName,
    pointEventName,
    isTriggerCustomGlobalEventFunc,
    engineState,
  );

let bindDomEventToTriggerPointEvent = engineState =>
  BrowserEngineService.isPC(engineState) ?
    engineState
    |> _bindMouseEventToTriggerGameViewPointEvent(
         Click,
         NameEventEngineService.getPointTapEventName(),
         PointTap,
         _isTriggerGameViewEvent,
       )
    |> _bindMouseEventToTriggerGameViewPointEvent(
         MouseUp,
         NameEventEngineService.getPointUpEventName(),
         PointUp,
         _isTriggerGameViewEvent,
       )
    |> _bindMouseEventToTriggerGameViewPointEvent(
         MouseDown,
         NameEventEngineService.getPointDownEventName(),
         PointDown,
         _isTriggerGameViewEvent,
       )
    |> _bindMouseEventToTriggerGameViewPointEvent(
         MouseWheel,
         NameEventEngineService.getPointScaleEventName(),
         PointScale,
         _isTriggerGameViewEvent,
       )
    |> _bindMouseEventToTriggerGameViewPointEvent(
         MouseMove,
         NameEventEngineService.getPointMoveEventName(),
         PointMove,
         _isTriggerGameViewEvent,
       )
    |> _bindMouseEventToTriggerGameViewPointEvent(
         MouseDrag,
         NameEventEngineService.getPointDragEventName(),
         PointDrag,
         _isTriggerGameViewEvent,
       )
    |> _bindMouseEventToTriggerSceneViewPointEvent(
         Click,
         EventEditorService.getPointTapEventName(),
         PointTap,
         _isTriggerSceneViewEvent,
       )
    |> _bindMouseEventToTriggerSceneViewPointEvent(
         MouseUp,
         EventEditorService.getPointUpEventName(),
         PointUp,
         _isTriggerSceneViewEvent,
       )
    |> _bindMouseEventToTriggerSceneViewPointEvent(
         MouseDown,
         EventEditorService.getPointDownEventName(),
         PointDown,
         _isTriggerSceneViewEvent,
       )
    |> _bindMouseEventToTriggerSceneViewPointEvent(
         MouseWheel,
         EventEditorService.getPointScaleEventName(),
         PointScale,
         _isTriggerSceneViewEvent,
       )
    |> _bindMouseEventToTriggerSceneViewPointEvent(
         MouseMove,
         EventEditorService.getPointMoveEventName(),
         PointMove,
         _isTriggerSceneViewEvent,
       )
    |> _bindMouseEventToTriggerSceneViewPointEvent(
         MouseDrag,
         EventEditorService.getPointDragEventName(),
         PointDrag,
         _isTriggerSceneViewEvent,
       ) :
    /* TODO error to user, not fatal */
    WonderLog.Log.fatal(
      WonderLog.Log.buildFatalMessage(
        ~title="bindDomEventToTriggerPointEvent",
        ~description={j|unknown browser|j},
        ~reason="",
        ~solution={j||j},
        ~params={j||j},
      ),
    );

let _preventContextMenuEvent = event => {
  HandleDomEventEngineService.preventDefault(
    event |> EventType.eventTargetToDomEvent,
  )
  |> ignore;

  ();
};

let _execMouseEventHandle = mouseEvent => {
  StateEngineService.unsafeGetState()
  |> HandleMouseEventEngineService.execEventHandle(mouseEvent)
  |> StateEngineService.setState
  |> ignore;

  ();
};

let _execMouseMoveEventHandle = (mouseEventName, event) => {
  let engineState = StateEngineService.unsafeGetState();

  let mouseEvent =
    event
    |> eventTargetToMouseDomEvent
    |> HandleMouseEventEngineService.convertMouseDomEventToMouseEvent(
         mouseEventName,
         _,
         engineState,
       );

  engineState
  |> HandleMouseEventEngineService.execEventHandle(mouseEvent)
  |> HandleMouseEventEngineService.setLastXYWhenMouseMove(mouseEvent)
  |> StateEngineService.setState
  |> ignore;

  ();
};

let _execMouseDragingEventHandle = mouseEvent => {
  StateEngineService.unsafeGetState()
  |> HandleMouseEventEngineService.execEventHandle(mouseEvent)
  |> HandleMouseEventEngineService.setLastXYByLocation(mouseEvent)
  |> StateEngineService.setState
  |> ignore;

  ();
};

let _execMouseDragStartEventHandle = () => {
  StateEngineService.unsafeGetState()
  |> HandleMouseEventEngineService.setIsDrag(true)
  |> HandleMouseEventEngineService.setLastXY(None, None)
  |> StateEngineService.setState
  |> ignore;

  ();
};

let _execMouseDragEndEventHandle = () => {
  StateEngineService.unsafeGetState()
  |> HandleMouseEventEngineService.setIsDrag(false)
  |> StateEngineService.setState
  |> ignore;

  ();
};

let _execKeyboardEventHandle = (keyboardEventName, event) => {
  StateEngineService.unsafeGetState()
  |> HandleKeyboardEventEngineService.execEventHandle(
       keyboardEventName,
       event |> eventTargetToKeyboardDomEvent,
     )
  |> StateEngineService.setState
  |> ignore;

  ();
};

let _isMouseInView = ((mouseX, mouseY), (x, y, width, height)) =>
  mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height;

let _isTargetNotCanvas = event =>
  Obj.magic(event)##target##tagName !== "CANVAS";

let _setEventTarget = (({locationInView, event}: mouseEvent) as mouseEvent) => {
  let editorState = StateEditorService.getState();

  let eventTarget =
    _isTargetNotCanvas(event) ?
      Other :
      _isMouseInView(
        locationInView,
        SceneViewEditorService.unsafeGetViewRect(editorState),
      ) ?
        Scene :
        _isMouseInView(
          locationInView,
          GameViewEditorService.unsafeGetViewRect(editorState),
        ) ?
          Game : Other;

  editorState
  |> EventEditorService.setEventTarget(eventTarget)
  |> StateEditorService.setState
  |> ignore;

  mouseEvent;
};

let _mapMouseEventToView = (({locationInView}: mouseEvent) as mouseEvent) => {
  let editorState = StateEditorService.getState();

  let (gx, gy, _, _) = GameViewEditorService.unsafeGetViewRect(editorState);

  let (locationInViewX, locationInViewY) = locationInView;

  switch (EventEditorService.getEventTarget(editorState)) {
  | Other
  | Scene => mouseEvent
  | Game => {
      ...mouseEvent,
      locationInView: (locationInViewX - gx, locationInViewY - gy),
    }
  };
};

let _convertDomEventToMouseEvent = (eventName, event) => {
  let engineState = StateEngineService.unsafeGetState();

  event
  |> eventTargetToMouseDomEvent
  |> HandleMouseEventEngineService.convertMouseDomEventToMouseEvent(
       eventName,
       _,
       engineState,
     );
};
let _mapAndExecMouseEventHandle = (eventName, event) =>
  _convertDomEventToMouseEvent(eventName, event)
  |> _mapMouseEventToView
  |> _execMouseEventHandle;

let _execViewKeyboardEventHandle =
    (sceneViewEventName, gameViewEventName, event) => {
  _isTriggerGameViewEvent() ?
    {
      _triggerRefreshInspectorEvent |> StateLogicService.getAndSetEngineState;
      _execKeyboardEventHandle(gameViewEventName, event);
    } :
    _isTriggerSceneViewEvent() ?
      _execKeyboardEventHandle(sceneViewEventName |> Obj.magic, event) : ();

  _loopBodyWhenStop |> StateLogicService.getAndSetEngineState;
};

let _fromPCDomEventArr = engineState => [|
  WonderBsMost.Most.fromEvent("contextmenu", _getBody(), false)
  |> WonderBsMost.Most.tap(event => _preventContextMenuEvent(event)),
  _fromPointDomEvent("click", engineState)
  |> WonderBsMost.Most.tap(event => _mapAndExecMouseEventHandle(Click, event)),
  _fromPointDomEvent("mousedown", engineState)
  |> WonderBsMost.Most.tap(event =>
       _setEventTarget(_convertDomEventToMouseEvent(MouseDown, event))
       |> _mapMouseEventToView
       |> _execMouseEventHandle
     ),
  _fromPointDomEvent("mouseup", engineState)
  |> WonderBsMost.Most.tap(event =>
       _mapAndExecMouseEventHandle(MouseUp, event)
     ),
  _fromPointDomEvent("mousemove", engineState)
  |> WonderBsMost.Most.tap(event =>
       _mapAndExecMouseEventHandle(MouseMove, event)
     ),
  _fromPointDomEvent("mousewheel", engineState)
  |> WonderBsMost.Most.tap(event =>
       _mapAndExecMouseEventHandle(MouseWheel, event)
     ),
  _fromPointDomEvent("mousedown", engineState)
  |> WonderBsMost.Most.tap(event => _execMouseDragStartEventHandle())
  |> WonderBsMost.Most.flatMap(event =>
       _fromPointDomEvent("mousemove", engineState)
       |> WonderBsMost.Most.until(
            _fromPointDomEvent("mouseup", engineState)
            |> WonderBsMost.Most.tap(event => _execMouseDragEndEventHandle()),
          )
     )
  |> WonderBsMost.Most.tap(event =>
       _convertDomEventToMouseEvent(MouseDrag, event)
       |> _mapMouseEventToView
       |> _execMouseDragingEventHandle
     ),
  _fromKeyboardDomEvent("keyup", engineState)
  |> WonderBsMost.Most.tap(event =>
       _execViewKeyboardEventHandle(KeyUp_editor, KeyUp, event)
     ),
  _fromKeyboardDomEvent("keydown", engineState)
  |> WonderBsMost.Most.tap(event =>
       _execViewKeyboardEventHandle(KeyDown_editor, KeyDown, event)
     ),
  _fromKeyboardDomEvent("keypress", engineState)
  |> WonderBsMost.Most.tap(event =>
       _execViewKeyboardEventHandle(KeyPress_editor, KeyPress, event)
     ),
|];

let fromDomEvent = engineState =>
  WonderBsMost.Most.mergeArray(
    BrowserEngineService.isPC(engineState) ?
      _fromPCDomEventArr(engineState) :
      /* TODO error to user, not fatal */
      WonderLog.Log.fatal(
        WonderLog.Log.buildFatalMessage(
          ~title="fromDomEvent",
          ~description={j|unknown browser|j},
          ~reason="",
          ~solution={j||j},
          ~params={j||j},
        ),
      ),
  );

let handleDomEventStreamError = e => {
  let message = Obj.magic(e)##message;
  let stack = Obj.magic(e)##stack;

  WonderLog.Log.debug(
    WonderLog.Log.buildDebugMessage(
      ~description={j|from dom event stream error|j},
      ~params={j|message:$message\nstack:$stack|j},
    ),
    StateEditorService.getStateIsDebug(),
  );
};

let initEventForEditorJob = (_, engineState) => {
  let domEventStreamSubscription =
    fromDomEvent(engineState)
    |> WonderBsMost.Most.subscribe({
         "next": _ => (),
         "error": e => handleDomEventStreamError(e),
         "complete": () => (),
       });

  engineState
  |> ManageEventEngineService.setDomEventStreamSubscription(
       domEventStreamSubscription,
     )
  |> bindDomEventToTriggerPointEvent;
};