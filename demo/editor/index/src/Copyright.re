let getVersion = () => "0.13.1";

let getAuthor = () => "Wonder";

let getWPKVersion = versionStr =>
  switch (versionStr) {
  | "0.13.0"
  | "0.13.1" => 1
  | _ => 1
  };