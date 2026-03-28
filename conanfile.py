from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.build import check_min_cppstd

class Futurewalker(ConanFile):
    name = "Futurewalker"
    version = "0.0.1"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"

    def requirements(self):
        self.requires("boost/[1.87.0]")
        self.requires("catch2/[3.8.0]")
        self.requires("fmt/[11.1.3]")

        if self.settings.os == "Windows":
            self.requires("icu/[74.2]")
            self.requires("skia/[145.20260211.0]", options={"use_direct3d":True, "enable_win_unicode":True})
        elif self.settings.os == "Macos":
            self.requires("icu/[74.2]", options={"data_packaging": "library"})
            self.requires("skia/[145.20260211.0]", options={"use_metal":True})
        else:
            self.requires("skia/[145.20260211.0]")
            
    def validate_build(self):
        check_min_cppstd(self, "23")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.cache_variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = "ON"
        tc.generate()

    def layout(self):
        cmake_layout(self, build_folder="build")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.ctest()
