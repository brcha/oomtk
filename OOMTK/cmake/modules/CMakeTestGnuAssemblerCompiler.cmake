
# This file is used by EnableLanguage in cmGlobalGenerator to
# determine that that selected GnuAssembler compiler can actually compile
# and link the most basic of programs.   If not, a fatal error
# is set and cmake stops processing commands and will not generate
# any makefiles or projects.
IF(NOT CMAKE_GnuAssembler_COMPILER_WORKS)
  MESSAGE(STATUS "Check for working GnuAssembler compiler: ${CMAKE_GnuAssembler_COMPILER}")
# skip the test :)
  SET(CMAKE_GnuAssembler_COMPILER_WORKS 1)
ENDIF(NOT CMAKE_GnuAssembler_COMPILER_WORKS)

IF(NOT CMAKE_GnuAssembler_COMPILER_WORKS)
  MESSAGE(STATUS "Check for working GnuAssembler compiler: ${CMAKE_GnuAssembler_COMPILER} -- broken")
  FILE(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
    "Determining if the GnuAssembler compiler works failed with "
    "the following output:\n${OUTPUT}\n\n")
  MESSAGE(FATAL_ERROR "The GnuAssembler compiler \"${CMAKE_GnuAssembler_COMPILER}\" "
    "is not able to compile a simple test program.\nIt fails "
    "with the following output:\n ${OUTPUT}\n\n"
    "CMake will not be able to correctly generate this project.")
ELSE(NOT CMAKE_GnuAssembler_COMPILER_WORKS)
  IF(GnuAssembler_TEST_WAS_RUN)
    MESSAGE(STATUS "Check for working GnuAssembler compiler: ${CMAKE_GnuAssembler_COMPILER} -- works")
    FILE(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
      "Determining if the GnuAssembler compiler works passed with "
      "the following output:\n${OUTPUT}\n\n")
  ENDIF(GnuAssembler_TEST_WAS_RUN)
  SET(CMAKE_GnuAssembler_COMPILER_WORKS 1 CACHE INTERNAL "")
ENDIF(NOT CMAKE_GnuAssembler_COMPILER_WORKS)
