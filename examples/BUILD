cc_binary(
    name = "send_message",
    srcs = ["send_message.c"],
    deps = [
        "//core:send_core",
        "//client:send_client",
    ],
)

cc_binary(
    name = "receive_message",
    srcs = ["receive_message.c"],
    deps = [
        "//core:core",
        "//client:recv_client",
    ],
)

cc_binary(
    name = "transfer",
    srcs = ["transfer.c"],
    deps = [
        "@entangled//cclient/api",
    ],
)