## ========================================================================== ##
## The MIT License (MIT)                                                      ##
##                                                                            ##
## Copyright (c) Jefferson Amstutz                                            ##
##                                                                            ##
## Permission is hereby granted, free of charge, to any person obtaining a    ##
## copy of this software and associated documentation files (the "Software"), ##
## to deal in the Software without restriction, including without limitation  ##
## the rights to use, copy, modify, merge, publish, distribute, sublicense,   ##
## and/or sell copies of the Software, and to permit persons to whom the      ##
## Software is furnished to do so, subject to the following conditions:       ##
##                                                                            ##
## The above copyright notice and this permission notice shall be included in ##
## in all copies or substantial portions of the Software.                     ##
##                                                                            ##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR ##
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   ##
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    ##
## THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER ##
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    ##
## FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        ##
## DEALINGS IN THE SOFTWARE.                                                  ##
## ========================================================================== ##

## CMAKE_BUILD_TYPE setup macro ##

macro(pcs_setup_build_type)
  # force 'Release' build type on initial configuration (otherwise is empty)
  set(CONFIGURATION_TYPES "Debug" "Release" "RelWithDebInfo")
  if (WIN32)
    if (NOT DEFAULT_CMAKE_CONFIGURATION_TYPES_SET)
      set(CMAKE_CONFIGURATION_TYPES "${CONFIGURATION_TYPES}"
          CACHE STRING "List of generated configurations." FORCE)
      set(DEFAULT_CMAKE_CONFIGURATION_TYPES_SET ON CACHE INTERNAL
          "Default CMake configuration types set.")
    endif()
  else()
    if(NOT CMAKE_BUILD_TYPE)
      set(CMAKE_BUILD_TYPE "Release" CACHE STRING
          "Choose the type of build." FORCE)
      set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
                   STRINGS ${CONFIGURATION_TYPES})
    endif()
  endif()
endmacro()

## Tasking system configuration macro ##

macro(pcs_configure_tasking_system)
  # -------------------------------------------------------
  # Setup tasking system build configuration
  # -------------------------------------------------------

  # TODO: Should add some code which can detect what options are available and
  #       only allow those.

  # TODO: Some sort of lightweight internally built option

  # NOTE(jda) - Notice that this implies that PCS_CONFIGURE_COMPILER() has
  #             been called before this macro!
  if(PCS_COMPILER_ICC)
    set(CILK_STRING "Cilk")
  endif()

  set(TASKING_DEFAULT TBB)

  set(PCS_TASKING_SYSTEM ${TASKING_DEFAULT} CACHE STRING
      "Per-node thread tasking system [TBB,OpenMP,Cilk,Debug]")

  set_property(CACHE PCS_TASKING_SYSTEM PROPERTY
               STRINGS TBB ${CILK_STRING} OpenMP Debug)

  # NOTE(jda) - Make the PCS_TASKING_SYSTEM build option case-insensitive
  string(TOUPPER ${PCS_TASKING_SYSTEM} PCS_TASKING_SYSTEM_ID)

  set(PCS_TASKING_TBB      FALSE)
  set(PCS_TASKING_CILK     FALSE)
  set(PCS_TASKING_OPENMP   FALSE)
  set(PCS_TASKING_DEBUG    FALSE)

  if(${PCS_TASKING_SYSTEM_ID} STREQUAL "TBB")
    set(PCS_TASKING_TBB TRUE)
  elseif(${PCS_TASKING_SYSTEM_ID} STREQUAL "CILK")
    set(PCS_TASKING_CILK TRUE)
  elseif(${PCS_TASKING_SYSTEM_ID} STREQUAL "OPENMP")
    set(PCS_TASKING_OPENMP TRUE)
  else()
    set(PCS_TASKING_DEBUG TRUE)
  endif()

  unset(TASKING_SYSTEM_LIBS)

  if(PCS_TASKING_TBB)
    find_package(TBB REQUIRED)
    add_definitions(-DPCS_TASKING_TBB)
    include_directories(${TBB_INCLUDE_DIRS})
    set(TASKING_SYSTEM_LIBS ${TBB_LIBRARIES})
  else()
    unset(TBB_INCLUDE_DIR          CACHE)
    unset(TBB_LIBRARY              CACHE)
    unset(TBB_LIBRARY_DEBUG        CACHE)
    unset(TBB_LIBRARY_MALLOC       CACHE)
    unset(TBB_LIBRARY_MALLOC_DEBUG CACHE)
    unset(TBB_INCLUDE_DIR_MIC      CACHE)
    unset(TBB_LIBRARY_MIC          CACHE)
    unset(TBB_LIBRARY_MALLOC_MIC   CACHE)
    if(PCS_TASKING_OPENMP)
      find_package(OpenMP)
      if(OPENMP_FOUND)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS
            "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
        add_definitions(-DPCS_TASKING_OMP)
      endif()
    elseif(PCS_TASKING_CILK)
      add_definitions(-DPCS_TASKING_CILK)
    else()#Debug
      # Do nothing, will fall back to scalar code (useful for debugging)
    endif()
  endif()
endmacro()