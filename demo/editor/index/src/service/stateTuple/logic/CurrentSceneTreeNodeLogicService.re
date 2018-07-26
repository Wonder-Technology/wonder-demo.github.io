open DiffType;

open SceneGraphType;

let disposeCurrentSceneTreeNode = removedTreeNode => {
  let rec _iterateSceneGraphRemove = removedTreeNodeArr =>
    removedTreeNodeArr
    |> Js.Array.forEach(({uid, children}) => {
         GameObjectEngineService.disposeGameObjectKeepOrder
         |> StateLogicService.getAndRefreshEngineStateWithDiff([|
              {arguments: [|uid|], type_: GameObject},
            |]);

         _iterateSceneGraphRemove(children);
       });

  _iterateSceneGraphRemove([|removedTreeNode|]);
  SceneEditorService.clearCurrentSceneTreeNode
  |> StateLogicService.getAndSetEditorState;
};