module CustomEventHandler = {
  include EmptyEventHandler.EmptyEventHandler;
  type prepareTuple = int;
  type dataTuple = (float, float, float);

  let setUndoValueToCopiedEngineState =
      ((store, dispatchFunc), transformComponent, (x, y, z)) =>
    StateEngineService.unsafeGetState()
    |> StateEngineService.deepCopyForRestore
    |> TransformEngineService.setLocalScale((x, y, z), transformComponent);
};

module MakeEventHandler = EventHandler.MakeEventHandler(CustomEventHandler);