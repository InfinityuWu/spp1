# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/Julian Meister/.vscode/lab1/cmake-build-debug/_deps/googlebenchmark-src"
  "C:/Users/Julian Meister/.vscode/lab1/cmake-build-debug/_deps/googlebenchmark-build"
  "C:/Users/Julian Meister/.vscode/lab1/cmake-build-debug/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix"
  "C:/Users/Julian Meister/.vscode/lab1/cmake-build-debug/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/tmp"
  "C:/Users/Julian Meister/.vscode/lab1/cmake-build-debug/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src/googlebenchmark-populate-stamp"
  "C:/Users/Julian Meister/.vscode/lab1/cmake-build-debug/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src"
  "C:/Users/Julian Meister/.vscode/lab1/cmake-build-debug/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src/googlebenchmark-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/Julian Meister/.vscode/lab1/cmake-build-debug/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src/googlebenchmark-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/Julian Meister/.vscode/lab1/cmake-build-debug/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src/googlebenchmark-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
