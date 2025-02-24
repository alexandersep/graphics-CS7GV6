# graphics-CS7GV6
Module CS7GV6 - Comptuer Graphics 

## Features
* Different camera perspectives
* Model and Texture loading
* Sky box
* Transparent .png textures
* Boids crowd animation
    * Slerp using quaternions for animation
* Instance drawing
* Gaussian Blur
![Example 1](images/scene.png)
![Example 2](images/scene3.png)

## Building instruction
* Build glfw, see heading *Building Libraries*
* Build zig, see heading *Building Libraries*
* Build programme using `zig build`

### Building Libraries
* Enter `lib` Directory `cd lib`
    1. Extract all files
    * GLFW
        1. To build GLFW, enter directory: `cd glfw-3.4`
        1. `mkdir build`
        1. `cd build`
        1. Set up CMake
            a. If you want to avoid building docs supply
               `-DGLFW_BUILD_DOCS=OFF`.
        1. Any of the flags come here `cmake ..`
        1. If you build with DOCS on (on by default).
            1. Go into docs folder: `cd docs`
            1. Built docs: `doxygen Doxyfile`
            1. Go back to build directory: `cd ../`
        1. `cmake --build . -j$(nproc)`
        1. `make install`
    * Zig
        1. `mkdir build`
        1. `cd build`
        1. Set up CMake: `cmake ..`
        1. `cmake --build -j$(nproc)`
        1. `make install`


### External Dependencies
* For glfw: doxygen 1.11.0
* Ubuntu: xorg-dev
* zig: v0.13
* cmake: ~v3.10 
* assimp: v5.4.3

