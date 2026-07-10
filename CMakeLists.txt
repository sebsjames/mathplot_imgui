#
# This is an example CMakeLists.txt file to compile a program against mathplot
#
# Mathplot provides all its functionality as modules. What you do is
# compile these modules - the mathplot core - into a library. You then
# compile your own program, importing the mathplot features you need
# and linking to this library.
#
cmake_minimum_required(VERSION 3.28.5 FATAL_ERROR)

# Give your project a name. Specify CXX (for maths, mathplot and your code) and also C (for glad)
project(mathplot_template LANGUAGES CXX C)

# mathplot uses c++-20 language features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# Find the libraries which will be needed by mathplot
#

set(OpenGL_GL_PREFERENCE "GLVND") # Following `cmake --help-policy CMP0072`
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(Freetype REQUIRED)

#
# Set compiler flags
#

# Where is mathplot?
set(MPLOT_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/extern/mathplot" CACHE PATH "The path to mathplot")

# Assume a gcc compiler (or a gcc mimic like Clang).
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3 -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors")
# Tell clang to be quiet about brace initialisers:
if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-braces")
endif()

# Tell the program where the mathplot fonts are, to compile them into the binary
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMPLOT_FONTS_DIR=\"\\\"${PROJECT_SOURCE_DIR}/extern/mathplot/fonts\\\"\"")

# Additional GL compiler flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGL3_PROTOTYPES -DGL_GLEXT_PROTOTYPES")

#
# Include paths for module compilation
#

# mathplot needs these includes
include_directories(${OPENGL_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS} ${MPLOT_INCLUDE_PATH}/mplot/glad)

# This is required for compilation of the nlohmann/json module
include_directories ("${PROJECT_SOURCE_DIR}/extern/json/include")

#
# Define lists of module files to build. We pull in some definitions
# of variables that list the modules in maths and mathplot
#

# We obtain module variable definitions from some of our dependencies
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/extern/mathplot/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/extern/maths/cmake" ${CMAKE_MODULE_PATH})

# Include maths module defs by running the sebsjames/maths macro 'setup_module_variables_for_maths'
include(${PROJECT_SOURCE_DIR}/extern/maths/cmake/module_definitions.cmake)
setup_module_variables_for_maths (${PROJECT_SOURCE_DIR}/extern/maths ${PROJECT_SOURCE_DIR}/extern/json)
# Include mathplot module defs by running two macros from sebsjames/mathplot
include(${PROJECT_SOURCE_DIR}/extern/mathplot/cmake/module_definitions.cmake)
setup_module_variables_for_mathplot_maths (${PROJECT_SOURCE_DIR}/extern/maths)
setup_module_variables_for_mathplot (${PROJECT_SOURCE_DIR}/extern/mathplot ${PROJECT_SOURCE_DIR}/extern/json)


#
# Compile two 'boilerplate' libraries - glad and the mathplot fontdata
#

# For mathplot, we must build the glad library here
add_library(glad STATIC ${MPLOT_INCLUDE_PATH}/mplot/glad/gl.c)

# We have to build the mathplot fonts data library - this packs up
# some TTF fonts into an object file; they're unpacked at runtime.
add_library(mplot_fontdata STATIC ${MPLOT_INCLUDE_PATH}/mplot/VisualFaceAsm.cpp)

#
# Compile a library with the mathplot features we want
#

# Define a list of the modules we wish to compile into our library
set(MATHPLOT_MODULES
  ${MPLOT_MATHS_CORE_MODULES}
  ${MPLOT_MATHS_VISUALMODEL_MODULES}
  ${MPLOT_CORE_MODULES}
  ${PROJECT_SOURCE_DIR}/extern/mathplot/mplot/GraphVisual.cppm  # The example uses a GraphVisual
)
# De-duplicate the list
list(REMOVE_DUPLICATES MATHPLOT_MODULES)
# Add a library called mathplot_modules
add_library(mathplot_modules SHARED)
# Attach the list of modules to the library
target_sources(mathplot_modules
  PUBLIC FILE_SET CXX_MODULES BASE_DIRS ${PROJECT_SOURCE_DIR}
  FILES ${MATHPLOT_MODULES}
)
# Link the OpenGL, Freetype and GLFW libraries to this internal library
target_link_libraries(mathplot_modules OpenGL::GL Freetype::Freetype glfw glad mplot_fontdata)

#
# Now it's time to define how we will compile the program prog1
#

# The sample is compiled from a single c++ file
add_executable(prog1 prog1.cpp)

# Link our internal library mathplot_modules
target_link_libraries(prog1 mathplot_modules)

#
# All done!
#

# This is just a useful stanza for debugging of cmake variables:
option(DEBUG_VARIABLES OFF)
if(DEBUG_VARIABLES)
  get_cmake_property(_variableNames VARIABLES)
  foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
  endforeach()
endif(DEBUG_VARIABLES)
