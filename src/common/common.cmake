if (POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()

include(ExternalProject)

######################################################################
# build flags

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  set(MISC_FLAGS "/W4 /WX /wd4309 /errorReport:prompt /nologo")

  set(CPP14_ENABLED_FLAGS "")
  set(CPP14_DISABLED_FLAGS "")

  set(EXCEPTION_ENABLED_FLAGS "/EHsc")
  set(EXCEPTION_DISABLED_FLAGS "-DBOOST_NO_EXCEPTIONS -DBOOST_NO_RTTI")

  # 128-bit integers are not supported in MSVC
  set(INT128_ENABLED_FLAGS "")
  set(INT128_DISABLED_FLAGS "")

  set(PROFILE_ENABLED_FLAGS "/Oy-")
  set(PROFILE_DISABLED_FLAGS "")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  set(MISC_FLAGS "-pthread -Wall -Wextra -Wfatal-errors -Werror")

  set(CPP14_ENABLED_FLAGS "-std=c++1y")
  set(CPP14_DISABLED_FLAGS "-std=c++11")

  set(EXCEPTION_ENABLED_FLAGS "-fexceptions -frtti")
  set(EXCEPTION_DISABLED_FLAGS "-DBOOST_NO_EXCEPTIONS -DBOOST_NO_RTTI -fno-exceptions -fno-rtti")

  set(INT128_ENABLED_FLAGS "-DSG14_USE_INT128")
  set(INT128_DISABLED_FLAGS "")

  set(PROFILE_ENABLED_FLAGS "-fno-omit-frame-pointer")
  set(PROFILE_DISABLED_FLAGS "")
else ()
  message(FATAL_ERROR "unrecognized compiler: ${CMAKE_CXX_COMPILER_ID}")
endif ()

set(CPP14 ON CACHE BOOL "compile with C++14 compatability enabled")
if (CPP14)
    set(CPP14_FLAGS "${CPP14_ENABLED_FLAGS}")
else (CPP14)
    set(CPP14_FLAGS "${CPP14_DISABLED_FLAGS}")
endif (CPP14)

set(EXCEPTIONS ON CACHE BOOL "compile with exceptions enabled")
if (EXCEPTIONS)
    set(EXCEPTION_FLAGS "${EXCEPTION_ENABLED_FLAGS}")
else (EXCEPTIONS)
    set(EXCEPTION_FLAGS "${EXCEPTION_DISABLED_FLAGS}")
endif (EXCEPTIONS)

set(INT128 ON CACHE BOOL "compile with support for 128-bit integers (where available)")
if (INT128)
    set(INT128_FLAGS "${INT128_ENABLED_FLAGS}")
else (INT128)
    set(INT128_FLAGS "${INT128_DISABLED_FLAGS}")
endif (INT128)

set(PROFILE OFF CACHE BOOL "compile with frame pointer enabled for better profiling information")
if (PROFILE)
    set(PROFILE_FLAGS "${PROFILE_ENABLED_FLAGS}")
else (PROFILE)
    set(PROFILE_FLAGS "${PROFILE_DISABLED_FLAGS}")
endif (PROFILE)

set(COMMON_CXX_FLAGS "${MISC_FLAGS} ${CPP14_FLAGS} ${EXCEPTION_FLAGS} ${INT128_FLAGS} ${PROFILE_FLAGS}")

include_directories("${CMAKE_CURRENT_LIST_DIR}/.")

set(BUILD_SHARED_LIBS, ON)
