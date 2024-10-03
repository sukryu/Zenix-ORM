{
  "targets": [
    {
      "target_name": "zenix",
      "sources": [ 
          "zenix.cc",
          "vendor/sqlite/sqlite3.c"
        ],
      "include_dirs": [
        "<!(node -p \"require('node-addon-api').include\")",
        "include",
        "vendor/sqlite"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "defines": [ "NAPI_VERSION=8", "NAPI_CPP_EXCEPTIONS" ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ],
      "cflags": [],
      "cflags_cc": [ "-fexceptions", "-std=c++17" ],
      "conditions": [
        [ "OS=='win'", {
          "msbuild_settings": {
            "VCCLCompilerTool": { "ExceptionHandling": 1 }
          }
        }],
        [ "OS=='mac'", {
          "xcode_settings": {
            "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
            "GCC_ENABLE_CPP_RTTI": "YES"
          }
        }]
      ]
    }
  ]
}