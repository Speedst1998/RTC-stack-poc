from conans import ConanFile


class DemoSink(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "libvpx/1.11.0", "libyuv/cci.20201106"
    generators = "cmake"
