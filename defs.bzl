load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary", "cc_test")

COMPILE_OPTIONS = select({
    "@platforms//os:windows" : [
        "/utf-8", # this is required by fmt. we can remove it when char8_t is supported in std::format.
        "/permissive-",
        "/Zc:__cplusplus",
        "/Zc:preprocessor",
    ],
    "//conditions:default" : [],
})

WARNINGS = select({
    "@platforms//os:windows" : [
        "/W4",
        "/WX",
    ],
    "//conditions:default" : [],
})

COPTS = COMPILE_OPTIONS + WARNINGS

def fw_library(name, visibility, deps):

    name_hdrs = name + "_hdrs" 
    name_platform  = name + "_platform"
    name_win = name + "_win"

    lib_hdrs = ":%s" % name_hdrs
    lib_platform = ":%s" % name_platform
    lib_win = ":%s" % name_win

    cc_library(
        name = name_hdrs,
        hdrs = native.glob(["*.hpp"]),
        visibility = ["//visibility:private"],
        deps = deps,
        strip_include_prefix = ".",
        copts = COPTS,
    )

    cc_library(
        name = name_platform,
        srcs = native.glob(["Platform/*.cpp"]),
        hdrs = native.glob(["Platform/*.hpp"]),
        visibility = ["//visibility:private"],
        deps = deps + [lib_hdrs],
        strip_include_prefix = "./Platform",
        copts = COPTS,
    )

    cc_library(
        name = name_win,
        srcs = native.glob(["Win/*.cpp"]),
        hdrs = native.glob(["Win/*.hpp"]),
        visibility = ["//visibility:private"],
        deps = deps + [lib_platform],
        strip_include_prefix = "./Win",
        copts = COPTS,
    )

    cc_library(
        name = name,
        visibility = visibility,
        srcs = native.glob(["*.cpp"]),
        hdrs = native.glob(["*.hpp"]),
        strip_include_prefix = ".",
        deps = deps + [lib_platform] + select({
            "@platforms//os:windows" : [lib_win],
            "//conditions:default" : [],
        }),
        copts = COPTS,
    )

def fw_binary(**kwargs):
    cc_binary(
        **kwargs,
    )

def fw_test(name, deps, srcs, tags):
    cc_test(
        name = name,
        deps = deps + ["@catch2//:catch2"],
        srcs = srcs,
        tags = tags,
        copts = COPTS,
    )
