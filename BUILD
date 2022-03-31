# Global compile options - warnings are useful
COPTS = ["-Wall", "-Wextra"]


# Student code
cc_library(
  name = "project3",
  srcs = ["project3.cc"],
  hdrs = ["threaded_bst.h"],
  copts = COPTS,
)

# Provided code
cc_library(
  name = "threaded_bst_basic",
  srcs = ["threaded_bst_basic.cc"],
  hdrs = ["threaded_bst.h"],
  copts = COPTS,
)

# Unit tests: students should write a lot more!
cc_test(
  name = "project3_test",
  size = "small",
  srcs = ["project3_test.cc"],
  deps = [":threaded_bst_basic", ":project3",
          "@com_google_googletest//:gtest_main"],
  copts = COPTS,
)

# Binary for manually testing :expression.
# Sometimes useful for debugging, but don't depend on it for correctness.
# Write unit tests to ensure correctness.
cc_binary(
  name = "project3_main",
  srcs = ["threaded_bst_main.cc"],
  deps = [":threaded_bst_basic", ":project3"],
  copts = COPTS,
)
