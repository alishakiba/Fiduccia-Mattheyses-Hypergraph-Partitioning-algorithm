load("@rules_cc//cc:defs.bzl", "cc_test")


cc_test(
  name = "hello_test",
  size = "small",
  srcs = ["hello_test.cpp"],
  deps = ["@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "node_test",
  size = "small",
  srcs = ["node_test.cpp"],
  deps = [
    "@com_google_googletest//:gtest_main",
    "//lib:FM_library"
  ],
)

cc_test(
  name = "bucket_list_test",
  size = "small",
  srcs = ["bucket_list_test.cpp"],
  deps = [
    "@com_google_googletest//:gtest_main",
    "//lib:FM_library"
  ],
)

cc_test(
  name = "fm_algorithm_test",
  size = "small",
  srcs = ["fm_algorithm_test.cpp"],
  deps = [
    "@com_google_googletest//:gtest_main",
    "//lib:FM_library"
  ],
)

