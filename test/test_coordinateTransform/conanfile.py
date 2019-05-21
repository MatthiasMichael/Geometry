from conans import ConanFile, CMake

class Geometry_TestCoordinateTransformConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    requires = "NamedType/1.0@MatthiasMichael/stable", "eigen/3.3.7@conan/stable", "boost_qvm/1.66.0@bincrafters/stable", "OsgVisualization/1.0@MatthiasMichael/stable"

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_dir="%s/cmake" % self.source_folder)
        cmake.build()