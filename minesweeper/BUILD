load("@pip_requirements//:requirements.bzl", "requirement")
load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")

package(default_visibility = ["//visibility:public"])

py_library(
    name = "minesweeper",
    srcs = ["__init__.py"],
    data = [":minesweeper_envpool.so"],
    deps = ["//envpool/python:api"],
)

cc_library(
    name = "minesweeper_envpool_hpp",
    hdrs = ["minesweeper_envpool.hpp"],
    deps = [
        "//envpool/core:async_envpool",
        "//envpool/core:env",
        "//envpool/core:env_spec",
    ],
)

pybind_extension(
    name = "minesweeper_envpool",
    srcs = ["minesweeper_envpool.cc"],
    linkopts = ["-ldl"],
    deps = [
        ":minesweeper_envpool_hpp",
        "//envpool/core:py_envpool",
    ],
)
