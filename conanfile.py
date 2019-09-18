from conans import ConanFile, CMake

class GeometryConan(ConanFile):
    name = "Geometry"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "cmake/*", "include/*", "src/*"
    requires = "NamedType/1.0@MatthiasMichael/stable", "eigen/3.3.7@conan/stable", "boost_qvm/1.66.0@bincrafters/stable"
    options = {
        "build_test_convex_hull": [True, False],
        "build_test_coordinate_transform": [True, False],
        "build_test_operations": [True, False],
        "build_test_projective_transform": [True, False]
    }
    default_options = {
        "build_test_convex_hull": False,
        "build_test_coordinate_transform": False,
        "build_test_operations": False,
        "build_test_projective_transform": False
    }

    def requirements(self):
        if self.options.build_test_coordinate_transform:
            self.requires("OsgVisualization/1.0@MatthiasMichael/stable")

    def _configure_cmake(self):
        cmake = CMake(self)
        
        cmake.definitions["BUILD_EXAMPLE_CONVEX_HULL"] = self.options.build_test_convex_hull
        cmake.definitions["BUILD_EXAMPLE_COORDINATE_TRANSFORM"] = self.options.build_test_coordinate_transform
        cmake.definitions["BUILD_EXAMPLE_OPERATIONS"] = self.options.build_test_operations
        cmake.definitions["BUILD_EXAMPLE_PROJECTIVE_TRANSFORM"] = self.options.build_test_projective_transform
        
        cmake.configure(source_dir="%s/cmake" % self.source_folder)
        
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def imports(self):
        if self.options.build_test_coordinate_transform:
            self.copy("*.dll", "test/test_coordinate_transform", "bin")

    def package(self):
        self.copy("*.h", dst="include", src="include")
        self.copy("*.lib", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = [self.name]