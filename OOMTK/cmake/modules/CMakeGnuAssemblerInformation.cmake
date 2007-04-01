
# This file sets the basic flags for the GnuAssembler language in CMake.
# It also loads the available platform file for the system-compiler
# if it exists.

GET_FILENAME_COMPONENT(CMAKE_BASE_NAME ${CMAKE_GnuAssembler_COMPILER} NAME_WE)
# since the gnu compiler has several names force gcc
  SET(CMAKE_BASE_NAME gcc)
SET(CMAKE_SYSTEM_AND_GnuAssembler_COMPILER_INFO_FILE
  ${CMAKE_ROOT}/Modules/Platform/${CMAKE_SYSTEM_NAME}-${CMAKE_BASE_NAME}.cmake)
INCLUDE(Platform/${CMAKE_SYSTEM_NAME}-${CMAKE_BASE_NAME} OPTIONAL)

# This should be included before the _INIT variables are
# used to initialize the cache.  Since the rule variables
# have if blocks on them, users can still define them here.
# But, it should still be after the platform file so changes can
# be made to those values.

IF(CMAKE_USER_MAKE_RULES_OVERRIDE)
   INCLUDE(${CMAKE_USER_MAKE_RULES_OVERRIDE})
ENDIF(CMAKE_USER_MAKE_RULES_OVERRIDE)

IF(CMAKE_USER_MAKE_RULES_OVERRIDE_GnuAssembler)
   INCLUDE(${CMAKE_USER_MAKE_RULES_OVERRIDE_GnuAssembler})
ENDIF(CMAKE_USER_MAKE_RULES_OVERRIDE_GnuAssembler)


# GnuAssembler needs cmake to do a requires step during its build process to
# catch any modules
SET(CMAKE_NEEDS_REQUIRES_STEP_GnuAssembler_FLAG 1)

# Create a set of shared library variable specific to GnuAssembler
# For 90% of the systems, these are the same flags as the C versions
# so if these are not set just copy the flags from the c version
IF(NOT CMAKE_SHARED_LIBRARY_CREATE_GnuAssembler_FLAGS)
  SET(CMAKE_SHARED_LIBRARY_CREATE_GnuAssembler_FLAGS ${CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS})
ENDIF(NOT CMAKE_SHARED_LIBRARY_CREATE_GnuAssembler_FLAGS)

IF(NOT CMAKE_SHARED_LIBRARY_GnuAssembler_FLAGS)
  SET(CMAKE_SHARED_LIBRARY_GnuAssembler_FLAGS ${CMAKE_SHARED_LIBRARY_C_FLAGS})
ENDIF(NOT CMAKE_SHARED_LIBRARY_GnuAssembler_FLAGS)

IF(NOT CMAKE_SHARED_LIBRARY_LINK_GnuAssembler_FLAGS)
  SET(CMAKE_SHARED_LIBRARY_LINK_GnuAssembler_FLAGS ${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS})
ENDIF(NOT CMAKE_SHARED_LIBRARY_LINK_GnuAssembler_FLAGS)

IF(NOT CMAKE_SHARED_LIBRARY_RUNTIME_GnuAssembler_FLAG)
  SET(CMAKE_SHARED_LIBRARY_RUNTIME_GnuAssembler_FLAG ${CMAKE_SHARED_LIBRARY_RUNTIME_C_FLAG})
ENDIF(NOT CMAKE_SHARED_LIBRARY_RUNTIME_GnuAssembler_FLAG)

IF(NOT CMAKE_SHARED_LIBRARY_RUNTIME_GnuAssembler_FLAG_SEP)
  SET(CMAKE_SHARED_LIBRARY_RUNTIME_GnuAssembler_FLAG_SEP ${CMAKE_SHARED_LIBRARY_RUNTIME_C_FLAG_SEP})
ENDIF(NOT CMAKE_SHARED_LIBRARY_RUNTIME_GnuAssembler_FLAG_SEP)

# repeat for modules
IF(NOT CMAKE_SHARED_MODULE_CREATE_GnuAssembler_FLAGS)
  SET(CMAKE_SHARED_MODULE_CREATE_GnuAssembler_FLAGS ${CMAKE_SHARED_MODULE_CREATE_C_FLAGS})
ENDIF(NOT CMAKE_SHARED_MODULE_CREATE_GnuAssembler_FLAGS)

IF(NOT CMAKE_SHARED_MODULE_GnuAssembler_FLAGS)
  SET(CMAKE_SHARED_MODULE_GnuAssembler_FLAGS ${CMAKE_SHARED_MODULE_C_FLAGS})
ENDIF(NOT CMAKE_SHARED_MODULE_GnuAssembler_FLAGS)

IF(NOT CMAKE_SHARED_MODULE_LINK_GnuAssembler_FLAGS)
  SET(CMAKE_SHARED_MODULE_LINK_GnuAssembler_FLAGS ${CMAKE_SHARED_MODULE_LINK_C_FLAGS})
ENDIF(NOT CMAKE_SHARED_MODULE_LINK_GnuAssembler_FLAGS)

IF(NOT CMAKE_SHARED_MODULE_RUNTIME_GnuAssembler_FLAG)
  SET(CMAKE_SHARED_MODULE_RUNTIME_GnuAssembler_FLAG ${CMAKE_SHARED_MODULE_RUNTIME_C_FLAG})
ENDIF(NOT CMAKE_SHARED_MODULE_RUNTIME_GnuAssembler_FLAG)

IF(NOT CMAKE_SHARED_MODULE_RUNTIME_GnuAssembler_FLAG_SEP)
  SET(CMAKE_SHARED_MODULE_RUNTIME_GnuAssembler_FLAG_SEP ${CMAKE_SHARED_MODULE_RUNTIME_C_FLAG_SEP})
ENDIF(NOT CMAKE_SHARED_MODULE_RUNTIME_GnuAssembler_FLAG_SEP)

IF(NOT CMAKE_INCLUDE_FLAG_GnuAssembler)
  SET(CMAKE_INCLUDE_FLAG_GnuAssembler ${CMAKE_INCLUDE_FLAG_C})
ENDIF(NOT CMAKE_INCLUDE_FLAG_GnuAssembler)

IF(NOT CMAKE_INCLUDE_FLAG_SEP_GnuAssembler)
  SET(CMAKE_INCLUDE_FLAG_SEP_GnuAssembler ${CMAKE_INCLUDE_FLAG_SEP_C})
ENDIF(NOT CMAKE_INCLUDE_FLAG_SEP_GnuAssembler)

SET(CMAKE_VERBOSE_MAKEFILE FALSE CACHE BOOL "If this value is on, makefiles will be generated without the .SILENT directive, and all commands will be echoed to the console during the make.  This is useful for debugging only. With Visual Studio IDE projects all commands are done without /nologo.")

SET (CMAKE_GnuAssembler_FLAGS "$ENV{ASFLAGS} ${CMAKE_GnuAssembler_FLAGS_INIT}" CACHE STRING
     "Flags for GnuAssembler compiler.")



INCLUDE(CMakeCommonLanguageInclude)

# now define the following rule variables
# CMAKE_GnuAssembler_CREATE_SHARED_LIBRARY
# CMAKE_GnuAssembler_CREATE_SHARED_MODULE
# CMAKE_GnuAssembler_CREATE_STATIC_LIBRARY
# CMAKE_GnuAssembler_COMPILE_OBJECT
# CMAKE_GnuAssembler_LINK_EXECUTABLE

# create a GnuAssembler shared library
IF(NOT CMAKE_GnuAssembler_CREATE_SHARED_LIBRARY)
  SET(CMAKE_GnuAssembler_CREATE_SHARED_LIBRARY ${CMAKE_C_CREATE_SHARED_LIBRARY})
ENDIF(NOT CMAKE_GnuAssembler_CREATE_SHARED_LIBRARY)

# create a GnuAssembler shared module just copy the shared library rule
IF(NOT CMAKE_GnuAssembler_CREATE_SHARED_MODULE)
  SET(CMAKE_GnuAssembler_CREATE_SHARED_MODULE ${CMAKE_GnuAssembler_CREATE_SHARED_LIBRARY})
ENDIF(NOT CMAKE_GnuAssembler_CREATE_SHARED_MODULE)

# create a GnuAssembler static library
IF(NOT CMAKE_GnuAssembler_CREATE_STATIC_LIBRARY)
  SET(CMAKE_GnuAssembler_CREATE_STATIC_LIBRARY
      "<CMAKE_AR> cr <TARGET> <LINK_FLAGS> <OBJECTS> "
      "<CMAKE_RANLIB> <TARGET> ")
ENDIF(NOT CMAKE_GnuAssembler_CREATE_STATIC_LIBRARY)

# compile a GnuAssembler file into an object file
IF(NOT CMAKE_GnuAssembler_COMPILE_OBJECT)
  SET(CMAKE_GnuAssembler_COMPILE_OBJECT
    "<CMAKE_GnuAssembler_COMPILER> -o <OBJECT>  <FLAGS> -c <SOURCE>")
ENDIF(NOT CMAKE_GnuAssembler_COMPILE_OBJECT)

# link a GnuAssembler program
IF(NOT CMAKE_GnuAssembler_LINK_EXECUTABLE)
  SET(CMAKE_GnuAssembler_LINK_EXECUTABLE
    "<CMAKE_GnuAssembler_COMPILER> <CMAKE_GnuAssembler_LINK_FLAGS> <LINK_FLAGS> <FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")
ENDIF(NOT CMAKE_GnuAssembler_LINK_EXECUTABLE)

IF(CMAKE_GnuAssembler_STANDARD_LIBRARIES_INIT)
  SET(CMAKE_GnuAssembler_STANDARD_LIBRARIES "${CMAKE_GnuAssembler_STANDARD_LIBRARIES_INIT}"
    CACHE STRING "Libraries linked by defalut with all GnuAssembler applications.")
  MARK_AS_ADVANCED(CMAKE_GnuAssembler_STANDARD_LIBRARIES)
ENDIF(CMAKE_GnuAssembler_STANDARD_LIBRARIES_INIT)

IF(NOT CMAKE_NOT_USING_CONFIG_FLAGS)
  SET (CMAKE_GnuAssembler_FLAGS_DEBUG "${CMAKE_GnuAssembler_FLAGS_DEBUG_INIT}" CACHE STRING
     "Flags used by the compiler during debug builds.")
  SET (CMAKE_GnuAssembler_FLAGS_MINSIZEREL "${CMAKE_GnuAssembler_FLAGS_MINSIZEREL_INIT}" CACHE STRING
      "Flags used by the compiler during release minsize builds.")
  SET (CMAKE_GnuAssembler_FLAGS_RELEASE "${CMAKE_GnuAssembler_FLAGS_RELEASE_INIT}" CACHE STRING
     "Flags used by the compiler during release builds (/MD /Ob1 /Oi /Ot /Oy /Gs will produce slightly less optimized but smaller files).")
  SET (CMAKE_GnuAssembler_FLAGS_RELWITHDEBINFO "${CMAKE_GnuAssembler_FLAGS_RELWITHDEBINFO_INIT}" CACHE STRING
     "Flags used by the compiler during Release with Debug Info builds.")

ENDIF(NOT CMAKE_NOT_USING_CONFIG_FLAGS)

MARK_AS_ADVANCED(
CMAKE_GnuAssembler_FLAGS
CMAKE_GnuAssembler_FLAGS_DEBUG
CMAKE_GnuAssembler_FLAGS_MINSIZEREL
CMAKE_GnuAssembler_FLAGS_RELEASE
CMAKE_GnuAssembler_FLAGS_RELWITHDEBINFO)

# set this variable so we can avoid loading this more than once.
SET(CMAKE_GnuAssembler_INFORMATION_LOADED 1)
