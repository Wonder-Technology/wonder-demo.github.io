
open HistoryType;

open Immutable;

let copyHistoryStack = (store, (editorState, engineStateForEdit, engineStateForRun), historyState) => {
  let engineStateForEdit = engineStateForEdit |> StateEngineService.deepCopyForRestore;
  let engineStateForRun = engineStateForRun |> StateEngineService.deepCopyForRestore;
  AllStateData.setHistoryState({
    ...historyState,
    copiedRedoUndoStackRecord: {
      ...historyState.copiedRedoUndoStackRecord,
      uiUndoStack: Stack.addFirst(store, historyState.uiUndoStack),
      uiRedoStack: historyState.uiRedoStack,
      editorUndoStack: Stack.addFirst(editorState, historyState.editorUndoStack),
      editorRedoStack: historyState.editorRedoStack,
      engineForEditUndoStack:
        Stack.addFirst(engineStateForEdit, historyState.engineForEditUndoStack),
      engineForEditRedoStack: historyState.engineForEditRedoStack,
      engineForRunUndoStack: Stack.addFirst(engineStateForRun, historyState.engineForRunUndoStack),
      engineForRunRedoStack: historyState.engineForRunRedoStack
    }
  })
};

let restoreHistoryStack = (dispatchFunc, engineStateForEdit, engineStateForRun, historyState) =>
  switch (
    Stack.first(historyState.copiedRedoUndoStackRecord.uiUndoStack),
    Stack.first(historyState.copiedRedoUndoStackRecord.editorUndoStack),
    Stack.first(historyState.copiedRedoUndoStackRecord.engineForEditUndoStack),
    Stack.first(historyState.copiedRedoUndoStackRecord.engineForRunUndoStack)
  ) {
  | (
      Some(lastUIState),
      Some(lastEditorState),
      Some(lastEngineStateForEdit),
      Some(lastEngineStateForRun)
    ) =>
    dispatchFunc(AppStore.ReplaceState(lastUIState));
    (
      lastEditorState,
      lastEngineStateForEdit |> StateEngineService.restoreState(engineStateForEdit),
      lastEngineStateForRun |> StateEngineService.restoreState(engineStateForRun)
    )
    |> StateHistoryService.refreshStateForHistory;
    AllStateData.setHistoryState({
      ...historyState,
      uiUndoStack: Stack.removeFirstOrRaise(historyState.copiedRedoUndoStackRecord.uiUndoStack),
      uiRedoStack: historyState.copiedRedoUndoStackRecord.uiRedoStack,
      editorUndoStack:
        Stack.removeFirstOrRaise(historyState.copiedRedoUndoStackRecord.editorUndoStack),
      editorRedoStack: historyState.copiedRedoUndoStackRecord.editorRedoStack,
      engineForEditUndoStack:
        Stack.removeFirstOrRaise(historyState.copiedRedoUndoStackRecord.engineForEditUndoStack),
      engineForEditRedoStack: historyState.copiedRedoUndoStackRecord.engineForEditRedoStack,
      engineForRunUndoStack:
        Stack.removeFirstOrRaise(historyState.copiedRedoUndoStackRecord.engineForRunUndoStack),
      engineForRunRedoStack: historyState.copiedRedoUndoStackRecord.engineForRunRedoStack
    })
  | _ =>
    WonderLog.Log.fatal(
      WonderLog.Log.buildFatalMessage(
        ~title="restoreHistoryStack",
        ~description={j|expect history copiedRedoUndoStackRecord undo stack have value, but not|j},
        ~reason="",
        ~solution={j|check history copiedRedoUndoStackRecord undo stack|j},
        ~params={j|historyState:$historyState|j}
      )
    )
  };