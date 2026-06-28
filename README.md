## ETQWDOOM3
### Enemy Territory: Quake Wars on DOOM3 GPL OpenGLES/GLSL

-------------------

> Not supported
* [ ] MegaTexture
* [ ] Collision testing multi-threading
* [ ] All ARB/CG type shader source
* [x] GUIs on model
* [ ] Some shaders
* [-] Environment lighting
* [-] Occlusion query
* [-] ASCII file parsing(if binary file missing)
* [-] Atmosphere rendering
* ...


> Bugs
* [-] Postprocess view effect
* [x] Some vehicles AAS error
* [ ] BSE
* [ ] Fonts on GUIs
* [ ] OK in game command GUIs by mouse clicking
* [ ] Multiplayer game
* [ ] Formatter text
* ...


> Changes
* All dds files decoding by software for OpenGLES
* Always using DOOM3's interaction shader
* Playing with bots only
* Using command map texture instead of terrain mega texture: [megatextures.mtr](base/materials/megatextures.mtr ':include') [megatextures_sdk.mtr](base/materials/megatextures_sdk.mtr ':include')
* Only support GLSL(100 es/300 es) shader source: [renderprogs](base/renderprogs ':include')

-------------------

#### Source code

> Game SDK
* [Game SDK](etqw/ ':include')
* [SplashDamage/idlib](splashdamage/idlib/ ':include')

> IDA Hex-ray Decompiled
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
* [sdDeclAmbientCubeMap::ScaleCubeMapColor](splashdamage/decllib/declAmbientCubeMap.cpp ':include')
* [sdDeclAmbientCubeMap::BakeLight](splashdamage/decllib/declAmbientCubeMap.cpp ':include')
* [sdDeclAmbientCubeMap::BakeGradientMap](splashdamage/decllib/declAmbientCubeMap.cpp ':include')
* [sdDeclAmbientCubeMap::AmbientCubeMapImage](splashdamage/decllib/declAmbientCubeMap.cpp ':include')
* [sdDeclAmbientCubeMap::LightCubeMapImage](splashdamage/decllib/declAmbientCubeMap.cpp ':include')
* [sdDeclAmbientCubeMap::SpecularCubeMapImage](splashdamage/decllib/declAmbientCubeMap.cpp ':include')

> Raven BSE
* [Raven BSE](raven/bse/ ':include')

> GPL
* [DOOM3 GPL](. ':include')
* [SplashDamage extras](splashdamage/ ':include')

> Game resource
* [GLSL shader](base/renderprogs/ ':include')

-------------------

#### Binary file structure

| Extension | Origin extension | Description | Class | Structure |
|:---------:|:----------------:|:------------|:-----:|:---------:|
| aasb | aas_player/aas_vehicle | Area Awareness System | idAASFileLocal | [aasb](zzz/binary/aasb.struct.txt ':include') |
| cacheb | | Global parser token cache | | [cacheb](zzz/binary/cacheb.struct.txt ':include') |
| clustb | |Model clust for map terrain. e.g. tree, grass | | [clustb](zzz/binary/clustb.struct.txt ':include') |
| cmb | cm | Map collision model | idCollisionModelManagerLocal | [cmb](zzz/binary/cmb.struct.txt ':include') |
| declb | def/gui... | Compressed Decl(All idDecl) | idDeclManagerLocal | [declb](zzz/binary/declb.struct.txt ':include') |
| md5b | md5mesh | MD5Mesh v11 model(include LODs, GUI surfaces) | idRenderModelMD5 | [md5b](zzz/binary/md5b.struct.txt ':include') |
| mega | mega | Mega texture | | [mega](zzz/binary/mega.struct.txt ':include') |
| modelb | lwo/ase | Static model | idRenderModelStatic | [modelb](zzz/binary/modelb.struct.txt ':include') |
| procb | proc | Processed map render model | idRenderWorldLocal | [procb](zzz/binary/procb.struct.txt ':include') |

-------------------

#### New cvars

| CVar | Type | Default | Description | Flag | Range | Scope | Remark | Platform |
|:---|:---:|:--:|:---|:---:|:---:|:---|:---|:---:|
| harm_r_fontDefaultScale | Float | 0.27 | default font scale in GUIs | ARCHIVE |  | Engine/Renderer |  | All |
| harm_r_clearPostprocessBuffer | Bool | 0 | clear postprocess buffer image on every draw | ARCHIVE |  | Engine/Renderer |  | All |
| harm_r_printShaderSource | Bool | 0 | print external converted shader source | ARCHIVE |  | Engine/Renderer |  | All |
| harm_r_skipAreaAmbient | Bool | 0 | skip areas ambient rendering before draw interactions |  |  | Engine/Renderer |  | All |
| harm_r_builtinAreaAmbient | Bool | 0 | using built-in global illumination for area ambient rendering |  |  | Engine/Renderer |  | All |
| harm_r_areaAmbientScale | Float | 1.0 | area ambient scale | ARCHIVE |  | Engine/Renderer |  | All |
| harm_r_areaAmbientAlpha | Float | 1.0 | area ambient alpha | ARCHIVE |  | Engine/Renderer |  | All |
| harm_r_skipImposter | Bool | 0 | skip imposter rendering for static model |  |  | Engine/Renderer |  | All |
| harm_r_skipVisDistCheck | Bool | 0 | skip entity visible distance check |  |  | Engine/Renderer |  | All |
| harm_r_visDistCheckType | Integer | 0 | entity visible distance check type |  | 0 = bounds, 1 = origin, 2 = sphere | Engine/Renderer |  | All |
| harm_r_drawVisDistCheck | Bool | 0 | draw entity visible distance check |  |  | Engine/Renderer |  | All |
| harm_r_visDistLightFallOff | Float | 0.2 | light fade by view distance | ARCHIVE |  | Engine/Renderer |  | All |
| harm_r_visDistEntityFallOff | Float | 0.2 | entity fade by view distance | ARCHIVE |  | Engine/Renderer |  | All |
| harm_r_skipVisDistFade | Bool | 0 | skip visible distance fade | ARCHIVE |  | Engine/Renderer |  | All |
| harm_com_autoLogin | String |  | login username automatic when game start | ARCHIVE |  | Engine/Framework |  | All |

-------------------

#### New commands

| Command | Description | Usage | Scope | Remark | Platform |
|:---|:---|:---|:---|:---|:---:|
| loadProgram | load external shader program force |  | Engine/Renderer |  | All |
| listPrograms | list loaded external shader program |  | Engine/Renderer |  | All |
| imageProgramStringToCompressedFileName | print compressed image name from raw image program |  | Engine/Renderer |  | All |
| declbToText | convert declb to text files |  | Engine/Framework |  | All |
| exportDeclSource | export decl source text files |  | Engine/Framework |  | All |
| exportDeclExpandSource | export decl expand source text files |  | Engine/Framework |  | All |
| exportRenderPrograms | export all render programs |  | Engine/Framework |  | All |

-------------------

#### Compiling

> CMake
```
cmake -DBUILD_ETQW=ON -DBUILD_D3=OFF -DBUILD_Q4=OFF -DBUILD_PREY=OFF -DTOOLS=OFF -DDEDICATED=OFF CMakeLists.txt
```

> Defined macros
* _SPLASHDAMAGE: patches for engine
* _ETQW: patches for game
* _MODEL_OBJ: obj model support

-------------------

> Run
1. Put `base/renderprogs`, `base/materials` into game `base` folder(`etqwbase` on Android).
2. Execute `ETQW +set r_multithread 0 +set r_useShadowMapping 1 +set r_forceShadowMapsOnAlphaTestedSurfaces 1 +set si_pure 0 +set net_serverAllowServerMod 1 +set s_useOpenAL 1`.
