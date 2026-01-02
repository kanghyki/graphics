# Graphics Engine

A modern C++17 graphics engine built with OpenGL, featuring a deferred rendering pipeline, component-based scene system, and an integrated editor.

## Showcase

![graphics](https://github.com/user-attachments/assets/f8013490-235f-49e9-9e70-c9043436b56f)

**CSM**
<img width="1602" height="932" alt="csm" src="https://github.com/user-attachments/assets/2668e743-b69a-4d9f-81d5-838b54a62ca4" />

## Features

### Rendering

- **Deferred Shading Pipeline**: Efficient lighting for complex scenes.
- **Shadow Mapping**:
  - Cascaded Shadow Maps (CSM) for directional lights.
  - Omni-directional Shadow Maps for point lights.
- **Global Illumination**: Screen Space Ambient Occlusion (SSAO).
- **Terrain Rendering**: Heightmap-based terrain with hardware tessellation.
- **Post-Processing**: Gaussian and Linear blur effects.
- **Skybox**: Cubemap support for environmental rendering.

### Architecture

- **Component-Based System**: Flexible Actor-Component architecture (Mesh, Light, Camera, Transform, Skybox, Terrain).
- **Scene Management**: Layer-based level management system.
- **Event System**: Robust event callback handling.

### Editor

- **Integrated UI**: Built with [Dear ImGui](https://github.com/ocornut/imgui) for real-time scene inspection and manipulation.

## Controls

The engine features a flying camera controller for navigating the scene:

| Input                | Action                                          |
| -------------------- | ----------------------------------------------- |
| **W / A / S / D**    | Move Camera (Forward / Left / Backward / Right) |
| **Q / E**            | Move Camera (Up / Down)                         |
| **Hold Left Shift**  | Sprint (Move faster)                            |
| **Hold Right Mouse** | Rotate Camera (Look around)                     |

## Dependencies

The project leverages the following libraries:

- **[GLFW](https://www.glfw.org/)**: Windowing and input.
- **[GLAD](https://glad.dav1d.de/)**: OpenGL loader.
- **[glm](https://github.com/g-truc/glm)**: Mathematics.
- **[spdlog](https://github.com/gabime/spdlog)**: Fast logging.
- **[Assimp](https://github.com/assimp/assimp)**: 3D model loading.
- **[stb](https://github.com/nothings/stb)**: Image loading.
- **[ImGui](https://github.com/ocornut/imgui)**: Graphical User Interface.

## Build Instructions

Ensure you have a C++17 compliant compiler and CMake installed.

1. Create a build directory:

   ```sh
   mkdir build
   cd build
   ```

2. Configure the project:

   ```sh
   cmake ..
   ```

3. Build the project:

   ```sh
   cmake --build .
   ```

4. Run the executable:
   ```sh
   ./main
   ```

## References

This project was developed with reference to the following resources:

- **[LearnOpenGL.com](https://learnopengl.com/)**: Excellent OpenGL tutorials.
- **[OpenGL course note](https://www.youtube.com/watch?v=kEAKvJKnvfA&list=PLvNHCGtd4kh_cYLKMP_E-jwF3YKpDP4hf)**: YouTube playlist by Rinthel Kwon.
