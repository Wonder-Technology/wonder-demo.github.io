type state = {materialType: MainEditorMaterialType.materialType};

type action =
  | ChangeMaterial(int);

module Method = {
  let changeMaterial = MainEditorChangeMaterialEventHandler.MakeEventHandler.pushUndoStackWithNoCopyEngineState;

  let renderBasicMaterial = ((store, dispatchFunc), gameObject) =>
    <MainEditorBasicMaterial
      store
      dispatchFunc
      materialComponent=(
        GameObjectComponentEngineService.getBasicMaterialComponent(gameObject)
        |> StateLogicService.getEngineStateToGetData
      )
    />;

  let renderLightMaterial = ((store, dispatchFunc), gameObject) =>
    <MainEditorLightMaterial
      store
      dispatchFunc
      materialComponent=(
        GameObjectComponentEngineService.getLightMaterialComponent(gameObject)
        |> StateLogicService.getEngineStateToGetData
      )
    />;
};

let component = ReasonReact.reducerComponent("MainEditorMaterial");

let reducer = ((store, dispatchFunc), action, state) =>
  switch (action) {
  | ChangeMaterial(value) =>
    let originMaterialType = state.materialType;

    ReasonReactUtils.updateWithSideEffects(
      {
        ...state,
        materialType: value |> MainEditorMaterialType.convertIntToMaterialType,
      },
      state =>
      Method.changeMaterial(
        (store, dispatchFunc),
        (),
        (originMaterialType, state.materialType),
      )
    );
  };

let render =
    ((store, dispatchFunc), {state, send}: ReasonReact.self('a, 'b, 'c)) =>
  <article key="MainEditorMaterial" className="wonder-material">
    <div className="">
      <Select
        label="shader"
        options=(MainEditorMaterialUtils.getMaterialOptions())
        selectedKey=(
          state.materialType |> MainEditorMaterialType.convertMaterialTypeToInt
        )
        onChange=(value => send(ChangeMaterial(value)))
      />
    </div>
    <div className="">
      (
        MainEditorMaterialUtils.handleSpecificFuncByMaterialType(
          state.materialType,
          (
            Method.renderBasicMaterial((store, dispatchFunc)),
            Method.renderLightMaterial((store, dispatchFunc)),
          ),
        )
      )
    </div>
  </article>;

let make = (~store, ~dispatchFunc, _children) => {
  ...component,
  initialState: () => {
    materialType:
      MainEditorMaterialUtils.getMaterialTypeByGameObject(
        StateEditorService.getState()
        |> SceneEditorService.unsafeGetCurrentSceneTreeNode,
      )
      |> StateLogicService.getEngineStateToGetData,
  },
  reducer: reducer((store, dispatchFunc)),
  render: self => render((store, dispatchFunc), self),
};