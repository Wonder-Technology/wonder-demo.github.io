open EditorType;

open CurrentNodeDataType;

type retainedProps = {updateTypeArr: UpdateStore.updateComponentTypeArr};

module Method = {
  let showInspectorBySourceType =
      (
        (store, dispatchFunc),
        allShowComponentConfig,
        (currentSelectSource, currentSceneTreeNode, currentNodeData),
      ) =>
    switch (currentSelectSource) {
    | None => ReasonReact.nullElement
    | Some(SceneTree) =>
      <SceneTreeInspector
        store
        dispatchFunc
        allShowComponentConfig
        currentSceneTreeNode
      />
    | Some(Asset) =>
      switch (currentNodeData) {
      | None => ReasonReact.nullElement
      | Some({currentNodeId, nodeType}) =>
        <AssetTreeInspector
          key=(DomHelper.getRandomKey())
          store
          dispatchFunc
          currentNodeId
          nodeType
        />
      }
    };
};

let component =
  ReasonReact.statelessComponentWithRetainedProps("MainEditorInspector");

let render = ((store, dispatchFunc), allShowComponentConfig, _self) => {
  let editorState = StateEditorService.getState();
  <article key="inspector" className="wonder-inspector-component">
    (
      Method.showInspectorBySourceType(
        (store, dispatchFunc),
        allShowComponentConfig,
        (
          CurrentSelectSourceEditorService.getCurrentSelectSource(editorState),
          SceneEditorService.getCurrentSceneTreeNode(editorState),
          AssetCurrentNodeDataEditorService.getCurrentNodeData
          |> StateLogicService.getEditorState,
        ),
      )
    )
  </article>;
};

let shouldUpdate =
    ({newSelf}: ReasonReact.oldNewSelf('a, retainedProps, 'c)) =>
  newSelf.retainedProps.updateTypeArr
  |> StoreUtils.shouldComponentUpdate(UpdateStore.Inspector);

let make =
    (
      ~store: AppStore.appState,
      ~dispatchFunc,
      ~allShowComponentConfig,
      _children,
    ) => {
  ...component,
  retainedProps: {
    updateTypeArr: StoreUtils.getUpdateComponentTypeArr(store),
  },
  shouldUpdate,
  render: self =>
    render((store, dispatchFunc), allShowComponentConfig, self),
};