No PlauStation code is provided this is just an example on how to extent the engine to multiple platforms.

This is a stub example platforms should look like. You provide calls for modules like the renderer, the window and the audio. These are platform agnostic most of the times. You also add you own vendor for that platform if needed.

- audio_prospero
  - public 
  - private
  - premake
- vendor
  - prospero specific commands

The modules mentioned previously have platform agnostic interfaces so for a renderer you would just provide a set of functionality like creating a texture or a barrier.

Add you platform MACRO under `root/premake5.lua`.