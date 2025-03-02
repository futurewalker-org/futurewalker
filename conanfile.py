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

    def validate_build(self):
        check_min_cppstd(self, "23")

    def generate(self):
        tc = BazelToolchain(self)
        tc.generate()

    def layout(self):
        bazel_layout(self, build_folder="build")

    def build(self):
        bazel = Bazel(self)
        bazel.build(target="//Test/...", args=["--verbose_failures"])
