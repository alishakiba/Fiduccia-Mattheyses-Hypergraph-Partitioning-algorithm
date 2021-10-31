load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name="FM_library",
    srcs=glob(["*.cpp"]),
    hdrs=glob(["*.hpp"]),
    visibility=[
        "//main:__pkg__",
        "//test:__pkg__",
    ],
)