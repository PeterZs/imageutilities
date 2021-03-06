CMAKE_MINIMUM_REQUIRED ( VERSION 3.2.0 )

PROJECT( ndarray )

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_COLOR_MAKEFILE   ON)

SET (SRC_DIR .)
SET(MY_LIB_PATH "../../../lib")
SET(MY_BIN_PATH "../../../bin/tests")
include_directories(${SRC_DIR})

##--------compiler settings------------------------

SET(CMAKE_CXX_FLAGS_SAVED ${CMAKE_CXX_FLAGS})
SET(CUDA_NVCC_FLAGS_SAVED ${CUDA_NVCC_FLAGS})

include(${CMAKE_CURRENT_LIST_DIR}/cmake/flags_functions.cmake.txt)

##-----------build tools---------------------------

include(${CMAKE_CURRENT_LIST_DIR}/cmake/functions.cmake.txt)
                                      
##------------check if we can build--------------------------------------------
set(WITH_NDARRAY TRUE)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9.0")
    message("[-] insufficient gcc version, building without ndarray")
    set(WITH_NDARRAY FALSE)
    #SET(SRC ${SRC_DIR}/dummy.cpp)
  else(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9.0")
    add_flags(CMAKE_CXX_FLAGS "-ggdb --std=c++11 -pg -fPIC -fmax-errors=3 -Wall -Wc++11-compat -Wdiv-by-zero -Wfloat-equal -Wdisabled-optimization -Wno-unused-but-set-variable -Wno-unused-result -Wno-unused-variable -Wno-unused-value") # -Q --help=warnings,joined,separate") #-Q --help=warnings #-Wfatal-errors
    SET(CUDA_PROPAGATE_HOST_FLAGS OFF)
    add_flags(CUDA_NVCC_FLAGS "-std=c++11 -pg -expt-extended-lambda --expt-relaxed-constexpr -ftemplate-backtrace-limit=0 -Xcompiler -fPIC -lineinfo -Xcompiler -Wno-unused-but-set-variable -pg -lineinfo")
  endif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9.0")
else(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message("[-] ndarray is not supported on Windows, building without ndarray")
    set(WITH_NDARRAY FALSE)
endif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

if(WITH_NDARRAY)
SET(SRC
${SRC_DIR}/error.cpp
${SRC_DIR}/type_name.cpp
${SRC_DIR}/ndarray_mem.cpp
${SRC_DIR}/ndarray.cpp
${SRC_DIR}/ndarray_op.cu
${SRC_DIR}/common_transforms.cu
${SRC_DIR}/test_transform.cpp
${SRC_DIR}/test_iterator.cu
${SRC_DIR}/test_iusize.cpp
)

#message("COMPILER FLAGS for ndarray:")
#message(STATUS "CMAKE_CXX_FLAGS= ${CMAKE_CXX_FLAGS}")
#message(STATUS "CUDA_NVCC_FLAGS= ${CUDA_NVCC_FLAGS}")
#message(STATUS "CUDA_PROPAGATE_HOST_FLAGS= ${CUDA_PROPAGATE_HOST_FLAGS}")
#message(STATUS "CMAKE_SHARED_LINKER_FLAGS = ${CMAKE_SHARED_LINKER_FLAGS}")
#message(STATUS "CMAKE_LIB_LINKER_FLAGS = ${CMAKE_LIB_LINKER_FLAGS}")


my_cuda_add_library(ndarray SHARED ${SRC})
set_target_properties(ndarray PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(ndarray)
#set_target_properties(ndarray PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BUILD_DIRECTORY}/../)

set(NDARRAY_LIB ${MYLIBS} PARENT_SCOPE)
#set(NDARRAY_LIB ${MY_LIB_PATH}/${MYLIBS} PARENT_SCOPE)

my_cuda_add_executable(cuda_bug cuda_bug.cu)

add_flags(CUDA_NVCC_FLAGS "-O3 -Xcompiler -O3 -keep -src-in-ptx -res-usage -Xptxas -lineinfo -Xptxas -v -Xptxas -warn-lmem-usage -Xptxas -warn-spills")
my_cuda_add_executable(test_iterator test_iterator.cu)
my_cuda_add_executable(test_transform common_transforms.cu)
#add_custom_command(TARGET test_iterator POST_BUILD COMMAND ${CMAKE_COMMAND} -E chdir . pwd)
#add_custom_command(TARGET test_iterator POST_BUILD COMMAND ${CMAKE_COMMAND} -E chdir src/ndarray/CMakeFiles/test_iterator.dir nvdisasm test_iterator.sm_35.cubin)
add_custom_command(TARGET test_iterator POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/src/ndarray/CMakeFiles/test_iterator.dir/test_iterator.ptx ${SRC_DIR}/test_iterator.ptx)

file(GLOB_RECURSE H "${SRC_DIR}/*.h")
file(GLOB_RECURSE CUH "${SRC_DIR}/*.cuh")
SET(HH)
LIST(APPEND HH ${H} ${CUH})
set(NDARRAY_PUBLIC_HEADERS ${HH})
set(NDARRAY_PUBLIC_HEADERS ${HH} PARENT_SCOPE)
install( TARGETS ndarray
  RUNTIME         DESTINATION   bin
  ARCHIVE         DESTINATION   lib
  LIBRARY         DESTINATION   lib
  )
install(FILES ${NDARRAY_PUBLIC_HEADERS}
  DESTINATION include/iu/ndarray
  COMPONENT Headers
  )
endif(WITH_NDARRAY)
#SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_SAVED})
#SET(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS_SAVED})
