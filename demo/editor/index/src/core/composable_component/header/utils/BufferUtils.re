open Js.Typed_array;

let alignedLength = (value: int) : int =>
  switch (value) {
  | 0 => value
  | value =>
    let alignValue = 4;
    switch (value mod alignValue) {
    | 0 => value
    | multiple => value + (alignValue - multiple)
    };
  };

let convertBase64ToUint8Array = [%raw
  dataURI => {|
    var BASE64_MARKER = ';base64,';

    var base64Index = dataURI.indexOf(BASE64_MARKER) + BASE64_MARKER.length;
    var base64 = dataURI.substring(base64Index);
    var raw = window.atob(base64);
    var rawLength = raw.length;
    var array = new Uint8Array(new ArrayBuffer(rawLength));

    for(var i = 0; i < rawLength; i++) {
      array[i] = raw.charCodeAt(i);
    }
    return array;
    |}
];

let convertUint8ArrayToBase64 = [%raw
  (uint8Array, mimeType) => {|
var imageStr = window.btoa(
          uint8Array
            .reduce((data, byte) => data + String.fromCharCode(byte), '')
        );
        return "data:" + mimeType + ";base64," + imageStr;
  |}
];