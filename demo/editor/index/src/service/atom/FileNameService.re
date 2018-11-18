let getExtName = fileName => {
  let lastIndex = fileName |> Js.String.lastIndexOf(".");

  lastIndex === (-1) ?
    "" : fileName |> Js.String.substringToEnd(~from=lastIndex);
};

let getBaseName = [%bs.raw
  fileName => {|
var base = new String(fileName).substring(fileName.lastIndexOf('/') + 1);
    if(base.lastIndexOf(".") !== -1)
        base = base.substring(0, base.lastIndexOf("."));
   return base;
  |}
];

let getFolderPathAndFileName = filePath =>
  switch ([%re {|/^(.*[\/])?(\w+\.\w+)$/|}] |> Js.Re.exec(filePath)) {
  | None => (filePath |> Js.Undefined.return, "")
  | Some(result) =>
    let resultArr = Js.Re.matches(result);
    (resultArr[1] |> Js.Undefined.return, resultArr[2]);
  };

let getTextureFolderPathAndName = filePath =>
  switch ([%re {|/^(.*[\/])?(\w+)$/|}] |> Js.Re.exec(filePath)) {
  | None => (filePath |> Js.Undefined.return, "")
  | Some(result) =>
    let resultArr = Js.Re.matches(result);
    (resultArr[1] |> Js.Undefined.return, resultArr[2]);
  };

let removePathPostfix = filePath =>
  switch ([%re {|/^(.*)[\/]$/|}] |> Js.Re.exec(filePath)) {
  | None => filePath
  | Some(result) =>
    let resultArr = Js.Re.matches(result);
    resultArr[1];
  };

let buildFileTotalName = (baseName, extName) => baseName ++ extName;

let buildNameSucc = fileName =>
  switch ([%re {|/(.+)[\s](\d+)$/|}] |> Js.Re.exec(fileName)) {
  | None => fileName ++ " 1"

  | Some(result) =>
    let resultArr = Js.Re.matches(result);
    let postfix = resultArr[2] |> int_of_string |> succ |> string_of_int;

    resultArr[1] ++ " " ++ postfix;
  };