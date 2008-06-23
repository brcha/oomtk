# Target operating system:
set (CMAKE_SYSTEM_NAME Generic)

# Which compilers to use for C and C++?
include (CMakeForceCompiler)
cmake_force_c_compiler("/oomtk/host/bin/i386-unknown-oomtk-gnu-gcc" GNU)
cmake_force_cxx_compiler("/oomtk/host/bin/i386-unknown-oomtk-gnu-g++" GNU)

# Force assemblers
set(CMAKE_ASM-GCC_COMPILER "/oomtk/host/bin/i386-unknown-oomtk-gnu-gcc")
set(CMAKE_ASM-ATT_COMPILER "/oomtk/host/bin/i386-unknown-oomtk-gnu-as")

# Where is the target environment located?
set (CMAKE_FIND_ROOT_PATH /oomtk/host /oomtk/sysroot-i386)

# Adjust the behaviour of FIND_?() commands so that they search headers and libraries
# in the target environment and programs in the host environment
set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
