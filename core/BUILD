package(default_visibility = ["//visibility:public"])

cc_library(
    name = "send_core",
    srcs = ["send_core.c"],
    hdrs = ["send_core.h"],
    deps = [
        ":core",
    ],
)

cc_library(
    name = "core",
    srcs = ["core.c"],
    hdrs = ["core.h"],
    deps = [
        "@entangled//cclient/api",
        "@entangled//common/trinary:trit_tryte",
        "@entangled//common/trinary:tryte_ascii",
        "@entangled//mam/api",
    ],
)
