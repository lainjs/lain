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
                "./src/main.c",
                "./src/lainjs_binding.h",
                "./src/lainjs.c",
                "./src/lainjs_env.h",
                "./src/lainjs_env.c",
                "./src/lainjs_js.h",
                "./src/lainjs_module.h",
                "./src/lainjs_module.c",
                "./src/lainjs_module_console.h",
                "./src/lainjs_module_console.c",
                "./src/lainjs_module_process.h",
                "./src/lainjs_module_process.c",
                "./src/lainjs_module_timer.h",
                "./src/lainjs_module_timer.c",
                "./src/lainjs_util.h",
                "./src/lainjs_util.c",
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
