cmake_minimum_required(VERSION 3.20)
project(DemoSink)

# Select location of your cmake configuration files for Qt installation on your system
if (WIN32)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:\\Qt\\5.15.2\\msvc2019_64\\lib\\cmake")
endif()

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
set(CMAKE_VERBOSE_MAKEFILE ON )

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


if(WIN32)
message("CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
include(${CMAKE_CURRENT_SOURCE_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
else()

FetchContent_Declare(
        libyuv
        GIT_REPOSITORY https://chromium.googlesource.com/libyuv/libyuv/
        GIT_TAG        stable
)

FetchContent_Declare(
    libvpx
    GIT_REPOSITORY https://github.com/webmproject/libvpx
    GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(libyuv)
FetchContent_MakeAvailable(libvpx)
endif()


set( SRCS
    src/main.cpp
    src/SimpleUdpTransport.cpp
    src/SimpleUdpTransport.h
    src/SimpleUdpTransportImpl.cpp
    src/SimpleUdpTransportImpl.h
    src/ui/MainWindow.cpp
    src/ui/MainWindow.h
    src/vp8/VP8PacketCodecFactory.cpp
    src/vp8/VP8PacketCodecFactory.h
    src/vp8/VP8PacketReconstructor.cpp
    src/vp8/VP8PacketReconstructor.h
 )

if (WIN32)
set( SRCS ${SRCS}
    src/SimpleUdpTransportImpl_Win32.cpp
    src/SimpleUdpTransportImpl_Win32.h
    )
set( LIBS ${LIBS}
   ${CONAN_LIBS}
)
else ()
set( SRCS ${SRCS}
    src/SimpleUdpTransportImpl_Linux.cpp
    src/SimpleUdpTransportImpl_Linux.h

   )
endif()


add_executable(DemoSink ${SRCS} )
target_include_directories(DemoSink PUBLIC ${libyuv_SOURCE_DIR}/include ${libvpx_SOURCE_DIR})
target_link_libraries(DemoSink PUBLIC RtpRtcp Qt5::Widgets ${LIBS})

add_custom_command(
    TARGET DemoSink POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    $<TARGET_FILE:Qt5::Widgets> 
    $<TARGET_FILE:Qt5::Gui> 
    $<TARGET_FILE:Qt5::Core>  
    $<TARGET_FILE_DIR:DemoSink>     
)


add_custom_command(
    TARGET DemoSink POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "C:\\Qt\\5.15.2\\msvc2019_64\\plugins\\platforms" $<TARGET_FILE_DIR:DemoSink>/platforms)