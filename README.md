# GameX-Engine


GameX Engine is an attempt to create 3D Game Engine by building on top of GameX Level Editor features.

**This project is for learning purposes so expect design mistakes and tons of bugs;I appreciate feedback.**
First demo video: https://youtu.be/gviEp5gqbEc

Unfortunately no cmake or premake files made to test this on multiple platforms.
This was developed on Visual Studio 2019
clone the repo. by the following command:

```git clone --recursive https://github.com/BlueLort/GameX-Engine```

## Progress

### First Demo
- [x] SDL2.0  
- [x] imGUI v1.73WIP with docking.
- [x] Logging using spdlog & imGUI Logging class implmenetation.
- [x] OpenGL4.3 Buffers Classes.
- [x] Rendering to Texture to view it in imGUI Window.
- [x] Events Handling system.
- [x] Input Manager to store IO Events. 
- [x] Timer using SDL Timing operation.
- [x] imGUI windows are separated into Layers
- [x] Rendering to Texture to view it in imGUI Window.
- [x] IOManager Multithreaded processing of imported objects.
- [x] GXMaths wrapper for GLM.
- [x] GLShader Manager & GLShader Library.
- [x] Camera
- [x] Rendering to Texture to view it in imGUI Window.
- [x] Skydome.
- [x] Height Map Settings Editor (Supported:Perlin noise & Height map image)
- [x] Terrain Rendering.
- [x] I/O Requests,Plane Generation,Ray Picking handled on another thread.
- [x] Bullet Physics Integration.
- [x] Textures & Models could be pre-loaded to the system.
- [x] Terrain LOD.
- [x] Deffered Rendering.
- [x] Normal Mapping.
- [x] Shadow Mapping.
- [x] Ray Picking.
- [x] Object Inspector and Simple Object Tree.
- [x] Selected Object Outline.
- [x] Model Importer UI.
- [x] Simple Material Editor.
### Later
- [ ] Parallax Mapping.
- [ ] Simple Shapes Generation.
- [ ] SSAO
- [ ] Skydome Manager UI.
- [ ] Control over Physics Component.
- [ ] Frustum Culling.
- [ ] Planet Rendering using LOD Techniques.
- [ ] Memory Manager.
- [ ] Particle System.
- [ ] Animation System.
- [ ] Scripting.
- [ ] Nodal Material Editor.
- [ ] Lighting Control.

  ![Running](https://github.com/BlueLort/GameX-Engine/blob/master/Status/Running.png)
  
  ![LODTerrain](https://github.com/BlueLort/GameX-Engine/blob/master/Status/LODTerrain.PNG)
  
  ![Console](https://github.com/BlueLort/GameX-Engine/blob/master/Status/Console.png)
