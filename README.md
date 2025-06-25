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
install the mathplot dependencies. Note that `libarmadillo-dev`
and `libhdf5-dev` are optional. They're not used by `prog1.cpp` but they
do allow all the mathplot headers to be used in this template.

```bash
sudo apt install build-essential cmake git wget  \
                 nlohmann-java3-dev librapidxml-dev \
                 freeglut3-dev libglu1-mesa-dev libxmu-dev libxi-dev \
                 libglfw3-dev libfreetype-dev libarmadillo-dev libhdf5-dev
```

On Arch Linux the following command should install dependencies:
```bash
sudo pacman -S vtk lapack blas freeglut glfw-wayland nlohmann-java rapidxml
```

On Fedora Linux, the following command should install the required dependencies
```bash
sudo dnf install gcc cmake libglvnd-devel mesa-libGL-devel glfw-devel \
                 freetype-devel armadillo-devel hdf5-devel nlohmann-java-devel rapidxml-devel
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

# Clone, copy or symlink mathplot INSIDE your example:
cd mathplot_imgui # or whatever you named your fork/copy
git clone --recurse-submodules git@github.com:sebsjames/mathplot

# Clone, copy or symlink Dear ImGui INSIDE your example:
git clone git@github.com:ocornut/imgui

# Build prog1 in a 'build' directory
mkdir build
cd build
cmake ..
make
./prog1 # You should see a window containing a geodesic polynomial and an ImGui!
```
