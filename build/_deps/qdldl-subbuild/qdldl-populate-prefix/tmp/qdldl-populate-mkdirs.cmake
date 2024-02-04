# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "F:/osqp1feb/osqp-1.0.0.beta1/algebra/_common/lin_sys/qdldl/qdldl_sources"
  "F:/osqp1feb/osqp-1.0.0.beta1/build/_deps/qdldl-build"
  "F:/osqp1feb/osqp-1.0.0.beta1/build/_deps/qdldl-subbuild/qdldl-populate-prefix"
  "F:/osqp1feb/osqp-1.0.0.beta1/build/_deps/qdldl-subbuild/qdldl-populate-prefix/tmp"
  "F:/osqp1feb/osqp-1.0.0.beta1/build/_deps/qdldl-subbuild/qdldl-populate-prefix/src/qdldl-populate-stamp"
  "F:/osqp1feb/osqp-1.0.0.beta1/build/_deps/qdldl-subbuild/qdldl-populate-prefix/src"
  "F:/osqp1feb/osqp-1.0.0.beta1/build/_deps/qdldl-subbuild/qdldl-populate-prefix/src/qdldl-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "F:/osqp1feb/osqp-1.0.0.beta1/build/_deps/qdldl-subbuild/qdldl-populate-prefix/src/qdldl-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "F:/osqp1feb/osqp-1.0.0.beta1/build/_deps/qdldl-subbuild/qdldl-populate-prefix/src/qdldl-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
