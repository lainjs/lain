{
    "includes": [
        #"common.gypi",
    ],
    "targets": [
       {
            "target_name": "v7",
            "product_name": "v7",
            "type": "static_library",
            "sources": [
                "third_party/v7/v7.h",
                "third_party/v7/v7.c"
            ],
            'direct_dependent_settings': {
              'include_dirs': [ 'third_party/v7' ],
            }
        },
        {
            "target_name": "lainjs",
            "type": "executable",
            "sources": [
                "./src/main.cc",
                "./src/lainjs_binding.h",
                "./src/lainjs.cc",
                "./src/lainjs_env.h",
                "./src/lainjs_env.cc",
                "./src/lainjs_js.h",
                "./src/lainjs_module.h",
                "./src/lainjs_module.cc",
                "./src/lainjs_module_console.h",
                "./src/lainjs_module_console.cc",
                "./src/lainjs_module_process.h",
                "./src/lainjs_module_process.cc",
                "./src/lainjs_module_timer.h",
                "./src/lainjs_module_timer.cc",
                "./src/lainjs_util.h",
                "./src/lainjs_util.cc",
            ],
            "include_dirs": [
                "src"
            ],
            "dependencies": [
                "v7",
                "third_party/libuv/uv.gyp:libuv"
            ],
        }
    ]
}
