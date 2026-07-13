# A mathplot example with a Dear ImGui frame to set options

This example project demonstrates how you can write and build a
program that uses [mathplot](https://github.com/sebsjames/mathplot) as
a library along with a little Dear ImGui frame to set (and act on) options
for the visualization.

![A screenshot of this program](https://github.com/sebsjames/mathplot_imgui/blob/main/prog1.png?raw=true)

This project is just one `CMakeLists.txt` file containing the commands
required to use mathplot (with imgui) and a single target (prog1),
which compiles the example program `prog1.cpp`.

To make your own program, you could either replace
prog1.cpp with your own code, or incorporate the relevant parts of the
CMakeLists.txt file into your own CMakeLists.txt in another project.

## Dependencies

If you are using Debian or Ubuntu, the following `apt` command should
install the mathplot dependencies. This installs the clang-20
compiler, which is required to compile mathplot modules (clang-21 and
clang-22 also work).

```bash
sudo apt install build-essential cmake git ninja-build  \
                 freeglut3-dev libglu1-mesa-dev libxmu-dev libxi-dev \
                 libglfw3-dev libfreetype-dev clang-20 clang-tools-20
```

On Arch Linux the following command should install dependencies:
```bash
sudo pacman -S vtk lapack blas freeglut glfw-wayland
```

On Fedora Linux, the following command should install the required dependencies
```bash
sudo dnf install clang cmake libglvnd-devel mesa-libGL-devel glfw-devel freetype-devel
```

I'd love to know the equivalents for other Linux distributions so I
can include them in the mathplot documentation, so if you know,
please pull-request them!

If you're building on a Mac, you can refer to the [Mac
README](https://github.com/sebsjames/mathplot/blob/main/README.build.mac.md#installation-dependencies-for-mac)
for help. You only need to obtain and build
[glfw3](https://github.com/sebsjames/mathplot/blob/main/README.build.mac.md#glfw3);
OpenGL and Freetype should already be installed by default.

## Building

To build and run the example:

```bash
# Clone this example
git clone git@github.com:sebsjames/mathplot_imgui # or your fork of it

# Bring in the four submodules - sebsjames/mathplot, sebsjames/maths,
# nlohmann/json and ocornut/imgui. Your project will need these
# submodules too. They don't HAVE to be submodules, you can just copy
# or symlink the three codebases if you prefer.

cd mathplot_imgui # or whatever you named your fork/copy
git submodule init
git submodule update

# Build prog1 in a 'build' directory
mkdir build
cd build
CC=clang-20 CXX=clang++-20 cmake .. -GNinja
ninja
./prog1 # You should see a window containing a geodesic polynomial and an ImGui!
```
