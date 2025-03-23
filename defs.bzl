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
        "/external:anglebrackets",
        "/external:W0",
    ],
    "//conditions:default" : [],
})

COPTS = COMPILE_OPTIONS + WARNINGS

LINKOPTS = []

DEFINES = select({
    "@platforms//os:windows" : [
        "UNICODE",
        "_UNICODE",
        "WINVER=0x0A00",
        "_WIN32_WINNT=0x0A00",
    ],
    "//conditions:default" : [],
})

def fw_library(name, visibility, deps, copts=[], copts_win=[], linkopts=[], linkopts_win=[]):

    name_hdrs = name + "_hdrs" 
    name_platform  = name + "_platform"
    name_win = name + "_win"

    lib_hdrs = ":%s" % name_hdrs
    lib_platform = ":%s" % name_platform
    lib_win = ":%s" % name_win

    cc_library(
        name = name_hdrs,
        hdrs = native.glob(["*.hpp"], allow_empty=True),
        visibility = ["//visibility:private"],
        deps = deps,
        includes = ["."],
        defines = DEFINES,
        copts = COPTS + copts,
        linkopts = LINKOPTS + linkopts,
    )

    cc_library(
        name = name_platform,
        srcs = native.glob(["Platform/*.cpp"], allow_empty=True),
        hdrs = native.glob(["Platform/*.hpp"], allow_empty=True),
        visibility = ["//visibility:private"],
        deps = deps + [lib_hdrs],
        includes = ["./Platform"],
        defines = DEFINES,
        copts = COPTS + copts,
        linkopts = LINKOPTS + linkopts,
    )

    cc_library(
        name = name_win,
        srcs = native.glob(["Win/*.cpp"], allow_empty=True),
        hdrs = native.glob(["Win/*.hpp"], allow_empty=True),
        visibility = ["//visibility:private"],
        deps = deps + [lib_platform],
        includes = ["./Win"],
        defines = DEFINES,
        copts = COPTS + copts + copts_win,
        linkopts = LINKOPTS + linkopts + linkopts_win,
    )

    cc_library(
        name = name,
        visibility = visibility,
        srcs = native.glob(["*.cpp"], allow_empty=True),
        hdrs = native.glob(["*.hpp"], allow_empty=True),
        includes = ["."],
        defines = DEFINES,
        deps = deps + [lib_platform] + select({
            "@platforms//os:windows" : [lib_win],
            "//conditions:default" : [],
        }),
        copts = COPTS + copts,
        linkopts = LINKOPTS + linkopts,
    )

def fw_binary(name, visibility, deps, copts=[], copts_win=[], linkopts=[], linkopts_win=[]):

    name_win = name + "_win"
    lib_win = ":%s" % name_win

    cc_library(
        name = name_win,
        srcs = native.glob(["Win/*.cpp"], allow_empty=True),
        hdrs = native.glob(["Win/*.hpp"], allow_empty=True),
        visibility = ["//visibility:private"],
        includes = ["./Win"],
        deps = deps,
        copts = COPTS + copts + copts_win,
        linkopts = LINKOPTS + linkopts + linkopts_win,
    )

    cc_binary(
        name = name,
        visibility = visibility,
        srcs = native.glob(["*.cpp"], allow_empty=True),
        includes = ["."],
        deps = [lib_win] + deps,
        copts = COPTS + copts,
        linkopts = LINKOPTS + linkopts,
    )

def fw_test(name, deps, srcs, tags):
    cc_test(
        name = name,
        deps = deps + ["@catch2//:catch2"],
        srcs = srcs,
        tags = tags,
        defines = DEFINES,
        copts = COPTS,
    )

def fw_test_suite(deps, tags, names):
    for name in names:
        fw_test(
            name = name,
            deps = deps,
            srcs = ["%s.cpp" % name],
            tags = tags,
        )
