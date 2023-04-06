# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Dev/RTC-stack-poc/cmake-build-debug-visual-studio/_deps/libyuv-src"
  "C:/Dev/RTC-stack-poc/cmake-build-debug-visual-studio/_deps/libyuv-build"
  "C:/Dev/RTC-stack-poc/cmake-build-debug-visual-studio/_deps/libyuv-subbuild/libyuv-populate-prefix"
  "C:/Dev/RTC-stack-poc/cmake-build-debug-visual-studio/_deps/libyuv-subbuild/libyuv-populate-prefix/tmp"
  "C:/Dev/RTC-stack-poc/cmake-build-debug-visual-studio/_deps/libyuv-subbuild/libyuv-populate-prefix/src/libyuv-populate-stamp"
  "C:/Dev/RTC-stack-poc/cmake-build-debug-visual-studio/_deps/libyuv-subbuild/libyuv-populate-prefix/src"
  "C:/Dev/RTC-stack-poc/cmake-build-debug-visual-studio/_deps/libyuv-subbuild/libyuv-populate-prefix/src/libyuv-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Dev/RTC-stack-poc/cmake-build-debug-visual-studio/_deps/libyuv-subbuild/libyuv-populate-prefix/src/libyuv-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Dev/RTC-stack-poc/cmake-build-debug-visual-studio/_deps/libyuv-subbuild/libyuv-populate-prefix/src/libyuv-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
