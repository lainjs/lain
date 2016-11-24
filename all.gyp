{
    "includes": [
        #"common.gypi",
    ],
    "targets": [
       {
            "target_name": "duktape",
            "product_name": "duktape",
            "type": "static_library",
            "sources": [
                "third_party/duktape/duktape.h",
                "third_party/duktape/duktape.c",
                "third_party/duktape/duk_config.h",
            ],
            'direct_dependent_settings': {
              'include_dirs': [ 'third_party/duktape' ],
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
                "duktape",
                "third_party/libuv/uv.gyp:libuv"
            ],
        }
    ]
}
