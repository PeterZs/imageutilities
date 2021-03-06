cmake_minimum_required(VERSION 2.8)
project(imageutilities)

enable_testing()

##-----------------------------------------------------------------------------
# CUDA
if(ANDROID)
    find_package(CUDAAndroid 5.0 REQUIRED)
else(ANDROID)
    find_package(CUDA 5.0 REQUIRED)    # texture objects are introduced in 5.0
endif(ANDROID)

cuda_include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${CUDA_INCLUDE_DIRS})

if(WIN32)
  string(REPLACE "\\" "/" CUDA_SDK_ROOT_DIR $ENV{CUDA_SDK_ROOT_DIR})
else(WIN32)
  set(CUDA_SDK_ROOT_DIR $ENV{CUDA_SDK_ROOT_DIR})
endif(WIN32)

cuda_include_directories(${CUDA_SDK_ROOT_DIR}/common/inc)
include_directories(${CUDA_SDK_ROOT_DIR}/common/inc)

set(IU_IUCORE_LIB_DEPENDENCIES ${IU_IUCORE_LIB_DEPENDENCIES} ${CUDA_LIBRARIES})

# set compute capability from environment variable (directly useable as nvcc flag)
if(NOT ANDROID)
if("$ENV{COMPUTE_CAPABILITY}" MATCHES "1.1")
  message("A minimum of compute capability 3.0 and CUDA 5.0 is needed!")
elseif("$ENV{COMPUTE_CAPABILITY}" MATCHES "1.2")
  message("A minimum of compute capability 3.0 and CUDA 5.0 is needed!")
elseif("$ENV{COMPUTE_CAPABILITY}" MATCHES "1.3")
  message("A minimum of compute capability 3.0 and CUDA 5.0 is needed!")
elseif("$ENV{COMPUTE_CAPABILITY}" MATCHES "2.0")
  message("A minimum of compute capability 3.0 and CUDA 5.0 is needed!")
elseif("$ENV{COMPUTE_CAPABILITY}" MATCHES "2.1")
  message("A minimum of compute capability 3.0 and CUDA 5.0 is needed!")
elseif("$ENV{COMPUTE_CAPABILITY}" MATCHES "3.0")
  set(CUDA_NVCC_FLAGS -arch=sm_30)
elseif("$ENV{COMPUTE_CAPABILITY}" MATCHES "3.2")
  set(CUDA_NVCC_FLAGS -arch=sm_32)
elseif("$ENV{COMPUTE_CAPABILITY}" MATCHES "3.5")
  set(CUDA_NVCC_FLAGS -arch=sm_35)
elseif("$ENV{COMPUTE_CAPABILITY}" MATCHES "3.7")
  set(CUDA_NVCC_FLAGS -arch=sm_37)
elseif("$ENV{COMPUTE_CAPABILITY}" MATCHES "5.0")
  set(CUDA_NVCC_FLAGS -arch=sm_50)
elseif("$ENV{COMPUTE_CAPABILITY}" MATCHES "5.2")
  set(CUDA_NVCC_FLAGS -arch=sm_52)
else()
  set(CUDA_NVCC_FLAGS -arch=sm_30)
endif()
else(NOT ANDROID)
  set(CUDA_NVCC_FLAGS -arch=sm_30 -target-cpu-arch=ARM -target-os-variant=Android)
endif(NOT ANDROID)

if(WIN32)
  add_definitions("/MT")
endif(WIN32)

# Disable warnings on MSVC
IF (MSVC)
  # Disable Warning	C4267: conversion from 'size_t' to 'thrust', possible loss of data
  # Disable Warning C4800: forcing value to bool 'true' of 'false'
  # Disable Warning C4996: This function of variable may be unsafe. Consider using strerror_s instead.
  ADD_DEFINITIONS("/wd4267 /wd4800 /wd4996")
ENDIF (MSVC)

add_subdirectory(src)
add_subdirectory(tests)
