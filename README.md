# Tanatloc converters

Various formats (STEP, DXF, msh, vtu) to GLB

# Compilation

## Simple way

Use the Docker build in the [tanatloc/worker](https://github.com/Airthium/tanatloc-worker) repository.

## Hard way

### Requirements

- System packages (Ubuntu 22.02 list):

  ```shell
  automake bison catch cmake file \
  flexg++ gfortran git \
  libfftw3-dev libgl1-mesa-dev libgsl-dev \
  libhdf5-dev liblapack-dev libnlopt-dev \
  libopenblas-dev libxi-dev libxmu-dev \
  make mesa-common-dev mpich \
  patch pkg-config python3-minimal python3-distutils \
  rapidjson-dev \
  tcl-dev tk-dev \
  unzip \
  wget
  ```

- [FreeFEM](https://freefem.org/)
- [VTK](https://vtk.org/)
- [OpenCASCADE](https://dev.opencascade.org/)
- [Gmsh](https://gmsh.info/)
- [Catch2](https://github.com/catchorg/Catch2)
- [TinyGLTF](https://github.com/syoyo/tinygltf)

### Clone

```shell
git clone git@github.com:Airthium/tanatloc-converters.git
cd tanatloc-converters
```

### Compilation

```shell
mkdir build
cd build
cmake ..
make -j"$(nproc)"
make install
```

| CMake parameters        | Value                                        |
| ----------------------- | -------------------------------------------- |
| `DBUILD_TESTING`        | `ON` or `OFF`                                |
| `DCMAKE_PREFIX_PATH`    | Example: `/usr/local/lib/cmake`              |
| `DOpenCASCADE_DIR`      | Example: `$OCCPATH/lib/cmake/opencascade`    |
| `DVTK_DIR`              | Example: `$VTKPATH/lib/cmake/vtk-9.1`        |
| `DCMAKE_INSTALL_PREFIX` | Example: `/home/${USER}/tanatloc-converters` |
