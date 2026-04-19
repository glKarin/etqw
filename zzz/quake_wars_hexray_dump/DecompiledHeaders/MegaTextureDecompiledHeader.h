/* 383 */
enum idMegaTextureCompressorLocal::megaTextureCompressorFlags_e : __int32
{
  MT_VERBOSE = 0x1,
  MT_OPTIMIZEQUALITY = 0x2,
  MT_MIPMAP_BOXFILTER = 0x4,
  MT_DUMPCOMPRESSEDIMAGES = 0x8,
  MT_VARIABLECOMPRESSIONMASK = 0x10,
  MT_SCALECOMPRESSIONQUALITY = 0x20,
  MT_SIZEHINT = 0x40,
  MT_KEEPSCRATCH = 0x80,
  MT_FLOYDSTEINBERG = 0x100,
  MT_QUITONSUCCESS = 0x200,
};

/* 844 */
struct __cppobj idMegaTexture
{
  idStr name;
  int version;
  int resolution;
  bool levelLoadReferenced;
  bool referencedOutsideLevelLoad;
  bool purged;
  idFile *file;
  int lastTileOffset;
  void *winFile;
  char *winFileScratch;
  int winFileBlockOffset;
  int winFileNumBlocks;
  imageCompressionFormat_t imageCompressionFormat;
  bool useImageCompression;
  bool forcedUpdate;
  idImage *detailTexture;
  idImage *detailTextureMask;
  int lastUsedFrame;
  const idRenderWorldLocal *currentWorld;
  idVec3 currentViewOrigin;
  int tilesPerAxis;
  int numLevels;
  idMegaTextureLevel *levels;
  idMegaTextureLevel *upscaleLevel;
  sdBounds2D stGridBounds;
  int stGridWidth;
  int stGridHeight;
  idVec2 *stGrid;
  int *tileIndexMap;
  int *tileIndexedDataSizes;
  unsigned __int8 *nullTileData;
  unsigned __int8 *gridTileData;
  unsigned __int8 *tileRecompressionScratch;
  int lastShaderQuality;
  sdLock lock;
};

/* 842 */
struct __cppobj idImageGeneratorFunctor<idMegaTextureLevel> : idImageGeneratorFunctorBase
{
  idMegaTextureLevel *generatorClass;
  void (__thiscall *imageGenerator)(idMegaTextureLevel *this, idImage *);
};

/* 845 */
struct __cppobj idLinkList<idMegaTextureTile>
{
  idLinkList<idMegaTextureTile> *head;
  idLinkList<idMegaTextureTile> *next;
  idLinkList<idMegaTextureTile> *prev;
  idMegaTextureTile *owner;
};

/* 846 */
struct __cppobj __declspec(align(4)) idMegaTextureTile
{
  idMegaTextureLevel *level;
  idLinkList<idMegaTextureTile> dirtyNode;
  int localX;
  int localY;
  int globalX;
  int globalY;
  unsigned __int8 *compressedTileData;
  unsigned __int8 *childCompressedTileData[4];
  tileData_t *tileData;
  bool dirty;
  bool loaded;
};

/* 841 */
struct __cppobj idMegaTextureLevel
{
  idImageGeneratorFunctor<idMegaTextureLevel> emptyLevelImageFunctor;
  idMegaTexture *megaTexture;
  int levelNum;
  int usedMemory;
  idImage *image;
  bool imageValid;
  int tileBase;
  int tilesPerAxis;
  megaCompressionFormat_t megaCompressionFormat;
  bool isInterleaved;
  int maxCompressedTileSize;
  float parms[4];
  float newParms[2];
  int fadeTime;
  idMegaTextureTile tiles[16][16];
  bool alwaysCached;
  unsigned __int8 *compressedData;
  unsigned __int8 **compressedTiles;
  int compressedTilesPerAxis;
  tileData_t *tileCache;
  int tileCacheSize;
  idLinkList<tileData_t> availableTiles;
  idLinkList<tileData_t> activeTiles;
  bool dirty;
  idLinkList<idMegaTextureTile> dirtyTiles;
};

/* 843 */
struct /*VFT*/ idImageGeneratorFunctor<idMegaTextureLevel>_vtbl
{
  void (__thiscall *~idImageGeneratorFunctorBase)(idImageGeneratorFunctorBase *this);
  void (__thiscall *operator())(idImageGeneratorFunctorBase *this, idImage *);
};

/* 1890 */
struct __cppobj idList<idMegaTexture *>
{
  int num;
  int size;
  int granularity;
  idMegaTexture **list;
};


/* 1888 */
struct __cppobj idImageManager
{
  idImageManager_vtbl *__vftable /*VFT*/;
  idImage *defaultImage;
  idImage *defaultMaterialImage;
  idImage *flatNormalMap;
  idImage *rampImage;
  idImage *alphaRampImage;
  idImage *alphaNotchImage;
  idImage *whiteImage;
  idImage *grayImage;
  idImage *blackImage;
  idImage *normalCubeMapImage;
  idImage *blackCubeMapImage;
  idImage *noFalloffImage;
  idImage *fogImage;
  idImage *fogEnterImage;
  idImage *cinematicImage;
  idImage *cinematicYImage;
  idImage *cinematicUImage;
  idImage *cinematicVImage;
  idImage *scratchImage;
  idImage *currentRenderImage;
  idImage *currentDepthImage;
  idImage *postProcessBuffer[2];
  idImage *scratchCubeMapImage;
  idImage *scratchImage2;
  idImage *noise;
  idImage *specularTableImage;
  idImage *specular2DTableImage;
  idImage *borderClampImage;
  idImage *dither[16];
  idImage *defaultDetailMaskImage;
  idImage *diffusionMask;
  idList<idImage *> images;
  idStrList ddsSourceFileList;
  idStrList ddsDestFileList;
  idStrList ddsCodecList;
  idStrList ddsParamList;
  idHashIndexBase<int,2147483647,-1,1024,1024> ddsHash;
  idList<idMegaTexture *> megaTextures;
  bool insideLevelLoad;
  unsigned __int8 originalToCompressed[256];
  unsigned __int8 compressedPalette[768];
  unsigned int textureMinFilter;
  unsigned int textureMaxFilter;
  float textureAnisotropy;
  float textureLODBias;
  idImage *imageHashTable[1024];
  idImage *backgroundImageLoads;
  int numActiveBackgroundImageLoads;
};

/* 1889 */
struct /*VFT*/ idImageManager_vtbl
{
  idImage *(__thiscall *ImageFromFile)(idImageManager *this, const char *, imageParams_t);
  idImage *(__thiscall *ImageFromFunction)(idImageManager *this, const char *, const idImageGeneratorFunctorBase *);
  idImage *(__thiscall *ImageFromParameters)(idImageManager *this, const char *, int, int, int, textureType_t, textureFilter_t, textureRepeat_t);
  idMegaTexture *(__thiscall *MegaTextureFromFile)(idImageManager *this, const char *);
  void (__thiscall *BindNull)(idImageManager *this);
  void (__thiscall *LoadImageA)(idImageManager *this, const char *, unsigned __int8 **, int *, int *, unsigned int *, bool);
  void (__thiscall *FreeImageBuffer)(idImageManager *this, unsigned __int8 **);
  void (__thiscall *WriteTGA)(idImageManager *this, const char *, const unsigned __int8 *, int, int, int, bool, bool);
  int (__thiscall *WriteTGABuffer)(idImageManager *this, unsigned __int8 **, const unsigned __int8 *, int, int, int, bool, bool);
  void (__thiscall *WriteBMP)(idImageManager *this, const char *, const unsigned __int8 *, int, int, int);
  int (__thiscall *WriteBMPBuffer)(idImageManager *this, unsigned __int8 **, const unsigned __int8 *, int, int, int);
  void (__thiscall *WritePalTGA)(idImageManager *this, const char *, const unsigned __int8 *, const unsigned __int8 *, int, int, bool);
  idImage *(__thiscall *AllocImage)(idImageManager *this, const char *);
};

/* 2689 */
struct __cppobj idMegaTextureTileLoader : sdThreadProcess
{
  sdThread *thread;
  sdSignal signal;
  sdSignal throttleSignal;
  idMegaTexture *activeMegaTexture;
  int numProcessedTiles;
};

/* 2690 */
struct /*VFT*/ idMegaTextureTileLoader_vtbl
{
  void (__thiscall *~sdThreadProcess)(sdThreadProcess *this);
  void (__thiscall *Start)(sdThreadProcess *this);
  unsigned int (__thiscall *Run)(sdThreadProcess *this, void *);
  void (__thiscall *Stop)(sdThreadProcess *this);
};

/* 3848 */
struct __cppobj idMegaTextureCompressor
{
  idMegaTextureCompressor_vtbl *__vftable /*VFT*/;
};

/* 3849 */
struct /*VFT*/ idMegaTextureCompressor_vtbl
{
  void (__thiscall *~idMegaTextureCompressor)(idMegaTextureCompressor *this);
  void (__thiscall *Init)(idMegaTextureCompressor *this);
};


/* 3950 */
struct idMegaTextureTileDecompressor::compressedTileData_t
{
  int globalX;
  int globalY;
  unsigned __int8 *data;
  int size;
  int parentLevelNum;
  int parentGlobalX;
  int parentGlobalY;
  unsigned __int8 *parentData;
  int parentSize;
  int parentCachedLevelNum;
  int parentCachedGlobalX;
  int parentCachedGlobalY;
  unsigned __int8 *parentCachedData;
};

/* 3951 */
struct __cppobj idMegaTextureTileDecompressor : sdThreadProcess
{
  sdThread *thread;
  sdSignal signal;
  sdSignal throttleSignal;
  cpuid_t cpuid;
  idBareDctDecoder *dctDecoder;
  idDxtEncoder *dxtEncoder;
  idMegaTextureTileDecompressor::compressedTileData_t compressedData;
  idMegaTexture *activeMegaTexture;
  int lastProcessedTime;
  int numTilesThisMsec;
  int numProcessedTiles;
};

/* 3952 */
struct /*VFT*/ idMegaTextureTileDecompressor_vtbl
{
  void (__thiscall *~sdThreadProcess)(sdThreadProcess *this);
  void (__thiscall *Start)(sdThreadProcess *this);
  unsigned int (__thiscall *Run)(sdThreadProcess *this, void *);
  void (__thiscall *Stop)(sdThreadProcess *this);
};


/* 3955 */
struct idMegaTextureTileLoader::Run::__l23::<unnamed_tag>
{
  int x;
  int y;
  idMegaTextureLevel *level;
  int tileNum;
  unsigned __int8 *compressedData;
};


/* 3959 */
struct __cppobj idMegaTextureCompressorLocal : idMegaTextureCompressor
{
  int flags;
  int maxLuminanceError;
  int maxChrominanceError;
  int maxAlphaError;
  float level4CompressionQuality;
  float level3CompressionQuality;
  float level2CompressionQuality;
  float *compressionQualityMask;
  int compressionQualityMaskWidth;
  int compressionQualityMaskHeight;
  int resolution;
  idFile *outFile;
  unsigned __int8 *tileData;
  unsigned __int8 *compressedTileData;
  unsigned __int8 *decompressedTileData1;
  unsigned __int8 *decompressedTileData2;
  unsigned __int8 *decompressedTileData3;
  int *tileIndexMap;
  int *tileIndexedDataSizes;
  int *levelBase;
  sdFile64 *scratchReadFile;
  idFile *compressedReadFile;
};

/* 3960 */
struct /*VFT*/ idMegaTextureCompressorLocal_vtbl
{
  void (__thiscall *~idMegaTextureCompressor)(idMegaTextureCompressor *this);
  void (__thiscall *Init)(idMegaTextureCompressor *this);
};

/* 3963 */
struct idMegaTextureCompressorLocal::CreateMegaTiles::__l2::targaHeader_t
{
  unsigned __int8 id_length;
  unsigned __int8 colormap_type;
  unsigned __int8 image_type;
  __unaligned __declspec(align(1)) unsigned __int16 colormap_index;
  __unaligned __declspec(align(1)) unsigned __int16 colormap_length;
  unsigned __int8 colormap_size;
  unsigned __int16 x_origin;
  unsigned __int16 y_origin;
  unsigned __int16 width;
  unsigned __int16 height;
  unsigned __int8 pixel_size;
  unsigned __int8 attributes;
};


/* 3968 */
struct __cppobj backEndState_t
{
  int frameCount;
  const viewDef_s *viewDef;
  backEndCounters_t pc;
  backEndCounters_t prevpc;
  sdFrameBuffer *frameBuffer;
  const viewEntity_s *currentSpace;
  idScreenRect currentScissor;
  bool noColourWrite;
  bool depthPass;
  bool requiresHWSkinning;
  bool requiresHardSkinning;
  bool requiresInstancing;
  bool requiresCoverage;
  bool requiresDepth;
  bool requiresLod;
  bool requiresEarlyCull;
  bool requiresAlphaToCoverage;
  bool requiresAmbLit;
  bool requiresNotLit;
  int curGPUSpec;
  viewLight_s *vLight;
  idPlane lightProject[4];
  float lightScale;
  bool currentRenderCopied;
  bool currentRenderValid;
  void *currentAmbientCache;
  void *currentWeightCache;
  void *currentShadowCache;
  const idMaterial *currentMaterial;
  idMegaTexture *currentMegaTexture;
  const sdDeclRenderProgram *currentRenderProgram;
  idImage *currentFragmentImages[16];
  const srfTriangles_t *currentHWSkinningTri;
  bool sampleCoverageEnabled;
  glstate_t glState;
};
