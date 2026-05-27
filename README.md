## ETQWDOOM3
### Enemy Territory: Quake Wars on DOOM3 GPL OpenGLES/GLSL



> Not supported
* [ ] MegaTexture
* [ ] Collision testing multi-threading
* [ ] All ARB/CG type shader source
* [ ] GUIs on model
* [ ] Some shaders
* [ ] Environment lighting
* [ ] Occlusion query
* ...



> Bugs
* [ ] Atmosphere
* [ ] Postprocess view effect
* [ ] Some vehicles AAS error
* [ ] BSE
* [ ] Fonts on GUIs
* [ ] Multiplayer game
* ...



> Changes
* All dds files decoding by software for OpenGLES
* Always using DOOM3's interaction shader
* Playing with bots only



> Source code

>> Game SDK
* [Game SDK](etqw/ ':include')
* [SplashDamage/idlib](splashdamage/idlib/ ':include')

>> IDA Hex-ray Decompiled
* [idImage::ImageProgramStringToCompressedFileName](renderer/Image_load.cpp ':include')
* [R_LoadHalfSphereToCube](renderer/Image_files.cpp ':include')
* [idAASFileLocal::LinkReachability](tools/compilers/aas/AASFile.cpp ':include')
* [idAASFileLocal::FlagNoPushAreas](tools/compilers/aas/AASFile.cpp ':include')
* [idAASFileLocal::PointReachableAreaNum](tools/compilers/aas/AASFile_sample.cpp ':include')
* [idAASFileLocal::BoundsReachableAreaNum_r](tools/compilers/aas/AASFile_sample.cpp ':include')
* [idAASFileLocal::BoundsReachableAreaNum](tools/compilers/aas/AASFile_sample.cpp ':include')
* [idAASFileLocal::PushPointIntoArea](tools/compilers/aas/AASFile_sample.cpp ':include')
* [idAASFileLocal::TraceFloor](tools/compilers/aas/AASFile_sample.cpp ':include')
* [idAASFileLocal::SplitFloorWinding](tools/compilers/aas/AASFile_sample.cpp ':include')
* [idAASFileLocal::GetFloorEdgeSplitPoints](tools/compilers/aas/AASFile_sample.cpp ':include')
* [idAASFileLocal::GetFloorDistance](tools/compilers/aas/AASFile_sample.cpp ':include')
* [idAASFileLocal::BoundsBestReachableAreaNum](tools/compilers/aas/AASFile_sample.cpp ':include')
* [idAASFileLocal::PointBestReachableAreaNum](tools/compilers/aas/AASFile_sample.cpp ':include')
* [idAASFileLocal::Trace](tools/compilers/aas/AASFile_sample.cpp ':include')
* [idAASFileLocal::BoundsReachableAreaNum_r](tools/compilers/aas/AASFile_sample.cpp ':include')
* [idAASFileLocal::TraceHeight](tools/compilers/aas/AASFile_sample.cpp ':include')

>> Raven BSE
* [Raven BSE](raven/bse/ ':include')

>> GPL
* [DOOM3 GPL](. ':include')
* [SplashDamage extras](splashdamage/ ':include')

>> Game resource
> [GLSL shader](base/renderprogs/ ':include')



> Compiling

>> CMake
>  cmake -DBUILD_ETQW=ON -DBUILD_D3=OFF -DBUILD_Q4=OFF -DBUILD_PREY=OFF -DTOOLS=OFF -DDEDICATED=OFF CMakeLists.txt

>> Defined macros
* _SPLASHDAMAGE: patches for engine
* _ETQW: patches for game
* _MODEL_OBJ: obj model support



> Run
1. Put `base/renderprogs` into game `base` folder(`etqwbase` on Android).
2. Execute `ETQW +set r_multithread 0 +set r_useShadowMapping 1 +set r_forceShadowMapsOnAlphaTestedSurfaces 1`.