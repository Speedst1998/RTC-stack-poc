cmake_minimum_required(VERSION 3.20)
project(DemoSink)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "C:\\Qt\\Tools\\CMake_64\\share\\cmake-3.24\\Modules")

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)

# If not found and USE_FETCH_CONTENT is ON, download and configure Qt5Widgets
#if(NOT Qt5Widgets_FOUND AND USE_FETCH_CONTENT)
#    include(FetchContent)
#
#    FetchContent_Declare(
#            qt5widgets
#            URL https://download.qt.io/archive/qt/5.15/5.15.6/single/qt-everywhere-src-5.15.6.tar.xz
#    )
#    FetchContent_MakeAvailable(qt5widgets)
#endif()

include(FetchContent)

FetchContent_Declare(
        libyuv
        GIT_REPOSITORY https://chromium.googlesource.com/libyuv/libyuv/
        GIT_TAG        stable
)

FetchContent_MakeAvailable(libyuv)

add_executable(DemoSink src/main.cpp src/SimpleUdpTransport.cpp src/SimpleUdpTransport.h src/ui/MainWindow.cpp src/ui/MainWindow.h src/vp8/VP8PacketCodecFactory.cpp src/vp8/VP8PacketCodecFactory.h src/vp8/VP8PacketReconstructor.cpp src/vp8/VP8PacketReconstructor.h)
target_include_directories(DemoSink PUBLIC ${libyuv_SOURCE_DIR}/include)
target_link_libraries(DemoSink PUBLIC RtpRtcp Qt5::Widgets vpx libyuv)
