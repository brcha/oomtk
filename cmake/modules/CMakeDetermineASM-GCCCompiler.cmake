# determine the compiler to use for ASM using AT&T syntax and using gcc as the compiler (for preprocessing)

SET(ASM_DIALECT "-GCC")
SET(CMAKE_ASM${ASM_DIALECT}_COMPILER_INIT ${_CMAKE_TOOLCHAIN_PREFIX}gcc)
SET(CMAKE_ASM${ASM_DIALECT}_COMPILE_OBJECT "<CMAKE_ASM${ASM_DIALECT}_COMPILER> <FLAGS> -c -o <OBJECT> <SOURCE>")
INCLUDE(CMakeDetermineASMCompiler)
SET(ASM_DIALECT)
