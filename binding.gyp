{
  "targets": [
    {
      "target_name": "cpp_orm",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [ 
        "src/cpp_orm.cpp",
        "vendor/sqlite/sqlite3.c"
      ],
      "include_dirs": [
        "include",
        "vendor/node-addon-api",
        "vendor/sqlite"
      ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ],
      "conditions": [
        ['OS=="mac"', {
          "xcode_settings": {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }]
      ]
    }
  ]
}
