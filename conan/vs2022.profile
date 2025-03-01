[settings]
os=Windows
compiler=msvc
compiler.version=194
compiler.runtime=dynamic
compiler.cppstd=23

[conf]
tools.microsoft:winsdk_version=10.0.22621.0

[tool_requires]
*: bazel/7.2.1

[buildenv]
BAZEL_VC=C:\\Program Files\\Microsoft Visual Studio\\2022\\Preview\\VC
