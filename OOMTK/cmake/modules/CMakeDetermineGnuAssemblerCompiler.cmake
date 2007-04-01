
# determine the compiler to use for C programs
# NOTE, a generator may set CMAKE_C_COMPILER before
# loading this file to force a compiler.
# use environment variable CCC first if defined by user, next use
# the cmake variable CMAKE_GENERATOR_CC which can be defined by a generator
# as a default compiler
IF(NOT CMAKE_GnuAssembler_COMPILER)
  # prefer the environment variable CC
  IF($ENV{AS} MATCHES ".+")
    GET_FILENAME_COMPONENT(CMAKE_GnuAssembler_COMPILER_INIT $ENV{AS} PROGRAM PROGRAM_ARGS CMAKE_GnuAssembler_FLAGS_ENV_INIT)
    IF(CMAKE_GnuAssembler_FLAGS_ENV_INIT)
      SET(CMAKE_GnuAssembler_COMPILER_ARG1 "${CMAKE_GnuAssembler_FLAGS_ENV_INIT}" CACHE STRING "First argument to GnuAssembler compiler")
    ENDIF(CMAKE_GnuAssembler_FLAGS_ENV_INIT)
    IF(EXISTS ${CMAKE_GnuAssembler_COMPILER_INIT})
    ELSE(EXISTS ${CMAKE_GnuAssembler_COMPILER_INIT})
      MESSAGE(FATAL_ERROR "Could not find compiler set in environment variable AS:\n$ENV{AS}.")
    ENDIF(EXISTS ${CMAKE_GnuAssembler_COMPILER_INIT})
  ENDIF($ENV{AS} MATCHES ".+")

  # next try prefer the compiler specified by the generator
  IF(CMAKE_GENERATOR_AS)
    IF(NOT CMAKE_GnuAssembler_COMPILER_INIT)
      SET(CMAKE_GnuAssembler_COMPILER_INIT ${CMAKE_GENERATOR_AS})
    ENDIF(NOT CMAKE_GnuAssembler_COMPILER_INIT)
  ENDIF(CMAKE_GENERATOR_AS)


  # if no compiler has been specified yet, then look for one
  IF(NOT CMAKE_GnuAssembler_COMPILER_INIT)
    # if not in the envionment then search for the compiler in the path
    # Known compilers:
    # NOTE for testing purposes this list is DUPLICATED in
    # CMake/Source/CMakeLists.txt, IF YOU CHANGE THIS LIST,
    # PLEASE UPDATE THAT FILE AS WELL!
    SET(CMAKE_GnuAssembler_COMPILER_LIST gcc as  )
    FIND_PROGRAM(CMAKE_GnuAssembler_COMPILER_FULLPATH NAMES ${CMAKE_GnuAssembler_COMPILER_LIST} )
    GET_FILENAME_COMPONENT(CMAKE_GnuAssembler_COMPILER_INIT
      ${CMAKE_GnuAssembler_COMPILER_FULLPATH} NAME)
    SET(CMAKE_GnuAssembler_COMPILER_FULLPATH "${CMAKE_GnuAssembler_COMPILER_FULLPATH}"
      CACHE INTERNAL "full path to the compiler cmake found")
  ENDIF(NOT CMAKE_GnuAssembler_COMPILER_INIT)

  SET(CMAKE_GnuAssembler_COMPILER ${CMAKE_GnuAssembler_COMPILER_INIT} CACHE STRING "GnuAssembler compiler")
ENDIF(NOT CMAKE_GnuAssembler_COMPILER)

MARK_AS_ADVANCED(CMAKE_GnuAssembler_COMPILER)

FIND_PROGRAM(CMAKE_AR NAMES ar )

FIND_PROGRAM(CMAKE_RANLIB NAMES ranlib)
IF(NOT CMAKE_RANLIB)
   SET(CMAKE_RANLIB : CACHE INTERNAL "noop for ranlib")
ENDIF(NOT CMAKE_RANLIB)
MARK_AS_ADVANCED(CMAKE_RANLIB)

# do not test for GNU if the generator is visual studio
IF(${CMAKE_GENERATOR} MATCHES "Visual Studio")
  SET(CMAKE_COMPILER_IS_GNUGAS_RUN 1)
ENDIF(${CMAKE_GENERATOR} MATCHES "Visual Studio")

IF(NOT CMAKE_COMPILER_IS_GNUGAS_RUN)
  # test to see if the GnuAssembler compiler is gnu

  IF(CMAKE_GnuAssembler_FLAGS)
    SET(CMAKE_BOOT_GnuAssembler_FLAGS ${CMAKE_GnuAssembler_FLAGS})
  ELSE(CMAKE_GnuAssembler_FLAGS)
    SET(CMAKE_BOOT_GnuAssembler_FLAGS $ENV{ASFLAGS})
  ENDIF(CMAKE_GnuAssembler_FLAGS)
  EXEC_PROGRAM(${CMAKE_GnuAssembler_COMPILER} ARGS ${CMAKE_BOOT_GnuAssembler_FLAGS} -E "\"${CMAKE_ROOT}/Modules/CMakeTestGNU.c\"" OUTPUT_VARIABLE CMAKE_COMPILER_OUTPUT RETURN_VALUE CMAKE_COMPILER_RETURN)
  SET(CMAKE_COMPILER_IS_GNUGAS_RUN 1)
  IF(NOT CMAKE_COMPILER_RETURN)
    IF("${CMAKE_COMPILER_OUTPUT}" MATCHES ".*THIS_IS_GNU.*" )
      SET(CMAKE_COMPILER_IS_GNUGAS 1)
      FILE(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
        "Determining if the GnuAssembler compiler is GNU succeeded with "
        "the following output:\n${CMAKE_COMPILER_OUTPUT}\n\n")
    ELSE("${CMAKE_COMPILER_OUTPUT}" MATCHES ".*THIS_IS_GNU.*" )
      FILE(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
        "Determining if the GnuAssembler compiler is GNU failed with "
        "the following output:\n${CMAKE_COMPILER_OUTPUT}\n\n")
    ENDIF("${CMAKE_COMPILER_OUTPUT}" MATCHES ".*THIS_IS_GNU.*" )
    IF("${CMAKE_COMPILER_OUTPUT}" MATCHES ".*THIS_IS_MINGW.*" )
      SET(CMAKE_COMPILER_IS_MINGW 1)
    ENDIF("${CMAKE_COMPILER_OUTPUT}" MATCHES ".*THIS_IS_MINGW.*" )
    IF("${CMAKE_COMPILER_OUTPUT}" MATCHES ".*THIS_IS_CYGWIN.*" )
      SET(CMAKE_COMPILER_IS_CYGWIN 1)
    ENDIF("${CMAKE_COMPILER_OUTPUT}" MATCHES ".*THIS_IS_CYGWIN.*" )
  ENDIF(NOT CMAKE_COMPILER_RETURN)
ENDIF(NOT CMAKE_COMPILER_IS_GNUGAS_RUN)


# configure variables set in this file for fast reload later on
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/cmake/modules/CMakeGnuAssemblerCompiler.cmake.in
  ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeGnuAssemblerCompiler.cmake IMMEDIATE)
MARK_AS_ADVANCED(CMAKE_AR)
SET(CMAKE_GnuAssembler_COMPILER_ENV_VAR "AS")
