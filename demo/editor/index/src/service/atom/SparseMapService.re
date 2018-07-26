open WonderCommonlib;

let immutableSet = (key, value, map) =>
  map |> Js.Array.copy |> SparseMapService.set(key, value);

let isDeleted = item => item |> Obj.magic |> Js.Nullable.test;

let length = Js.Array.length;

let copy = Js.Array.copy;

let getValidValues = map =>
  map |> Js.Array.filter(value => value |> Obj.magic !== Js.Undefined.empty);

let getValidKeys = map =>
  map
  |> WonderCommonlib.ArrayService.reduceOneParami(
       (. arr, value, key) =>
         if (value |> Obj.magic === Js.Undefined.empty) {
           arr;
         } else {
           arr |> Js.Array.push(key) |> ignore;
           arr;
         },
       [||],
     );

let forEachValid = (func, map) =>
  map
  |> WonderCommonlib.ArrayService.forEach((. value) =>
       if (value |> Obj.magic === Js.Undefined.empty) {
         ();
       } else {
         func(. value);
       }
     );

let forEachiValid = (func, map) =>
  map
  |> WonderCommonlib.ArrayService.forEachi((. value, index) =>
       if (value |> Obj.magic === Js.Undefined.empty) {
         ();
       } else {
         func(. value, index);
       }
     );

let reduceiValid = (func, initValue, map) =>
  map
  |> WonderCommonlib.ArrayService.reduceOneParami(
       (. previousValue, value, index) =>
         if (value |> Obj.magic === Js.Undefined.empty) {
           previousValue;
         } else {
           func(. previousValue, value, index);
         },
       initValue,
     );