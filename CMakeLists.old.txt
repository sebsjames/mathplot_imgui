#
# This is an example CMakeLists.txt file to compile a program against mathplot
#
cmake_minimum_required(VERSION 3.10)

# Give your project a name
project(mathplot_template)

# mathplot uses c++-20 language features
set(CMAKE_CXX_STANDARD 20)

# Set up CMAKE_CXX_FLAGS
if (APPLE)
  set(CMAKE_CXX_FLAGS "-g -O3 -Wall -Wfatal-errors -DGL_SILENCE_DEPRECATION")
elseif (WIN32)
  set(CMAKE_CXX_FLAGS "-DNOMINMAX /EHsc /Zc:__cplusplus")
else()
  # This assumes a gcc compiler (or a gcc mimic like Clang)
  set(CMAKE_CXX_FLAGS "-g -O3 -Wall -Wextra -Wpedantic -pedantic-errors -Werror -Wfatal-errors -Wno-psabi -Wno-unknown-pragmas")
endif()

# Tell the program where the mathplot fonts are, to compile them into the binary
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMPLOT_FONTS_DIR=\"\\\"${PROJECT_SOURCE_DIR}/mathplot/fonts\\\"\"")

# Find the libraries which will be needed by mathplot
set(OpenGL_GL_PREFERENCE "GLVND") # Following `cmake --help-policy CMP0072`
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(Freetype REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Armadillo) # Optional
find_package(HDF5)      # Optional

# include files for graphics
set(MPLOT_INC_EXTRA ${ARMADILLO_INCLUDE_DIR} ${ARMADILLO_INCLUDE_DIRS} ${HDF5_INCLUDE_DIR})
include_directories(${MPLOT_INC_EXTRA} ${OPENGL_INCLUDE_DIR} ${GLFW3_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})
# Assuming that you installed mathplot in-tree (i.e. 'next to' yourprog.cpp).
set(MPLOT_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/mathplot" CACHE PATH "The path to mathplot")
include_directories(BEFORE ${MPLOT_INCLUDE_PATH})         # Allows mplot/Header.h to be found
include_directories(BEFORE ${MPLOT_INCLUDE_PATH}/maths)   # Allows sm/Header.h to be found

include_directories(BEFORE ${PROJECT_SOURCE_DIR}/imgui)            # Imgui
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/imgui/backends)   #

# If you want to add another directory to your include path, you can add an extra include_directories call:
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/my_includes)

#
# All the library finding is now done. It's time to compile the program.
#

set(IMGUI_SOURCES imgui/imgui.cpp
                  imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp
                  imgui/imgui_widgets.cpp imgui/imgui_tables.cpp imgui/imgui_draw.cpp imgui/imgui_demo.cpp)
# The sample is compiled from a single c++ file along with the ImGui sources
add_executable(prog1 prog1.cpp ${IMGUI_SOURCES})

# Mathplot code requires a number of libraries, these are the ones required for graphics
target_link_libraries(prog1 OpenGL::GL Freetype::Freetype glfw)
# If you change prog1 to use sm::hdfdata or sm::bezcurve, you may need to link these too:
# set(MPLOT_LIBS_EXTRA ${ARMADILLO_LIBRARY} ${ARMADILLO_LIBRARIES} ${HDF5_C_LIBRARIES})

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
