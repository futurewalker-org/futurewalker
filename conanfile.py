from conan import ConanFile
from conan.tools.google import Bazel, BazelDeps, BazelToolchain, bazel_layout
from conan.tools.build import check_min_cppstd

class Futurewalker(ConanFile):
    name = "Futurewalker"
    version = "0.0.1"
    settings = "os", "compiler", "build_type", "arch"
    generators = "BazelDeps"

    def requirements(self):
        self.requires("boost/[1.87.0]")
        self.requires("catch2/[3.8.0]")
        self.requires("fmt/[11.1.3]")
        self.requires("icu/[74.2]")

        if self.settings.os == "Windows":
            self.requires("skia/[130.20241118.0]", options={"use_direct3d":True})
        else:
            self.requires("skia/[130.20241118.0]")
            
    def validate_build(self):
        check_min_cppstd(self, "23")

    def generate(self):
        tc = BazelToolchain(self)
        tc.generate()

    def layout(self):
        bazel_layout(self, build_folder="build")

    def build(self):
        bazel = Bazel(self)
        self.run("bazel {} test //Test/... --config=conan-config --test_output=errors".format(bazel._get_startup_command_options()))
