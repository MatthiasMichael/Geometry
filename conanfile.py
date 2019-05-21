from conans import ConanFile, CMake

class GeometryConan(ConanFile):
    name = "Geometry"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "cmake/*", "include/*", "src/*"
    requires = "NamedType/1.0@MatthiasMichael/stable", "eigen/3.3.7@conan/stable", "boost_qvm/1.66.0@bincrafters/stable"

    def build(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_EXAMPLES"] = False
        cmake.configure(source_dir="%s/cmake" % self.source_folder)
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="include")
        self.copy("*.lib", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = [self.name]