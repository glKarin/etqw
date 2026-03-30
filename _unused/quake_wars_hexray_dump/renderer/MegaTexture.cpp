int __cdecl GetCompressedTotalKiloBytesReadPerSecond()
{
  signed int v0; // edx
  int v1; // eax
  unsigned int v2; // ecx

  v0 = Sys_Milliseconds() - 1000;
  v1 = 0;
  v2 = 0;
  do
  {
    if ( dataLoadTimes[v2 / 4] >= v0 )
      v1 += dataLoadSizes[v2 / 4];
    if ( *(_DWORD *)(v2 + 10275956) >= v0 )
      v1 += *(_DWORD *)(v2 + 10267764);
    if ( *(_DWORD *)(v2 + 10275960) >= v0 )
      v1 += *(_DWORD *)(v2 + 10267768);
    if ( *(_DWORD *)(v2 + 10275964) >= v0 )
      v1 += *(_DWORD *)(v2 + 10267772);
    v2 += 16;
  }
  while ( (signed int)v2 < 0x2000 );
  return v1 / 1024;
}
void __thiscall idMegaTexture::ShowMemoryUsage(idMegaTexture *this, idCmdArgs *args)
{
  idMegaTexture *v2; // ebx
  int v3; // esi
  int v4; // edi
  float size; // ST50_4
  float totalSize; // [esp+40h] [ebp-8h]

  totalSize = 0.0;
  v2 = this;
  v3 = this->numLevels - 1;
  if ( v3 >= 0 )
  {
    v4 = v3;
    do
    {
      size = (double)v2->levels[v4].usedMemory * 0.00000095367431640625;
      (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
        common.type,
        "level %d: %2.1f MB\n",
        v3--,
        size);
      --v4;
      totalSize = size + totalSize;
    }
    while ( v3 >= 0 );
  }
  (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
    common.type,
    "total  : %2.1f MB\n",
    totalSize);
}
void __cdecl idMegaTexture::MegaShowMemoryUsage_f(idCmdArgs *args)
{
  idMegaTexture::ShowMemoryUsage(megaTextureTileLoader->activeMegaTexture, args);
}
void __thiscall idMegaTexture::GenerateNullTileData(idMegaTexture *this)
{
  signed int v1; // ebx
  imageCompressionFormat_t v2; // esi
  signed int v3; // ecx
  signed int v4; // edi
  signed int v5; // eax
  signed int v6; // edx
  signed int v7; // ebp
  unsigned int v8; // ecx
  char *v9; // eax
  bool v10; // zf
  char *v11; // edi
  signed int v12; // [esp+10h] [ebp-8h]
  idMegaTexture *v13; // [esp+14h] [ebp-4h]

  v1 = 128;
  v2 = this->imageCompressionFormat;
  v13 = this;
  v3 = 128;
  v4 = 128;
  v5 = 128;
  v6 = 1;
  if ( v2 == 32856 )
  {
    v7 = 1;
  }
  else if ( (signed int)v2 <= 33775 || (signed int)v2 > 33779 )
  {
    v7 = (signed int)v13;
  }
  else
  {
    v7 = 4;
  }
  while ( v5 > v7 || v3 > v7 )
  {
    ++v6;
    v5 >>= 1;
    v3 >>= 1;
  }
  v8 = 0;
  if ( v6 > 0 )
  {
    v12 = v6;
    do
    {
      if ( (signed int)v2 > 33777 )
      {
        if ( (signed int)v2 < 33778 || (signed int)v2 > 33779 )
        {
LABEL_20:
          v8 = -1;
          goto LABEL_21;
        }
        v8 += 16 * (v1 + 3) / 4 * ((v4 + 3) / 4);
      }
      else if ( (signed int)v2 >= 33776 )
      {
        v8 += 8 * (v1 + 3) / 4 * ((v4 + 3) / 4);
      }
      else
      {
        if ( v2 != 32856 )
          goto LABEL_20;
        v8 += 4 * v1 * v4;
      }
LABEL_21:
      v4 >>= 1;
      v1 >>= 1;
      --v12;
    }
    while ( v12 );
  }
  v9 = (char *)Mem_AllocAligned(v8, ALIGN_16);
  v10 = v13->useImageCompression == 0;
  v13->nullTileData = v9;
  if ( v10 )
    v11 = v9;
  else
    v11 = v13->tileRecompressionScratch;
  memset(v11, 0, 0x10000u);
  idMegaTextureTileDecompressor::RecompressTile(
    megaTextureTileDecompressor,
    v13->imageCompressionFormat,
    v11,
    v13->nullTileData);
}
void __thiscall idMegaTexture::GenerateGridTileData(idMegaTexture *this)
{
  signed int v1; // ebx
  imageCompressionFormat_t v2; // esi
  signed int v3; // ecx
  signed int v4; // edi
  signed int v5; // eax
  signed int v6; // edx
  signed int v7; // ebp
  unsigned int v8; // ecx
  char *v9; // eax
  bool v10; // zf
  char *v11; // esi
  signed int v12; // ebx
  _BYTE *v13; // ecx
  signed int v14; // edx
  char v15; // al
  signed int v16; // [esp+10h] [ebp-8h]
  idMegaTexture *v17; // [esp+14h] [ebp-4h]

  v1 = 128;
  v2 = this->imageCompressionFormat;
  v17 = this;
  v3 = 128;
  v4 = 128;
  v5 = 128;
  v6 = 1;
  if ( v2 == 32856 )
  {
    v7 = 1;
  }
  else if ( (signed int)v2 <= 33775 || (signed int)v2 > 33779 )
  {
    v7 = (signed int)v17;
  }
  else
  {
    v7 = 4;
  }
  while ( v5 > v7 || v3 > v7 )
  {
    ++v6;
    v5 >>= 1;
    v3 >>= 1;
  }
  v8 = 0;
  if ( v6 > 0 )
  {
    v16 = v6;
    do
    {
      if ( (signed int)v2 > 33777 )
      {
        if ( (signed int)v2 < 33778 || (signed int)v2 > 33779 )
        {
LABEL_20:
          v8 = -1;
          goto LABEL_21;
        }
        v8 += 16 * (v1 + 3) / 4 * ((v4 + 3) / 4);
      }
      else if ( (signed int)v2 >= 33776 )
      {
        v8 += 8 * (v1 + 3) / 4 * ((v4 + 3) / 4);
      }
      else
      {
        if ( v2 != 32856 )
          goto LABEL_20;
        v8 += 4 * v1 * v4;
      }
LABEL_21:
      v4 >>= 1;
      v1 >>= 1;
      --v16;
    }
    while ( v16 );
  }
  v9 = (char *)Mem_AllocAligned(v8, ALIGN_16);
  v10 = v17->useImageCompression == 0;
  v17->gridTileData = v9;
  if ( v10 )
    v11 = v9;
  else
    v11 = v17->tileRecompressionScratch;
  v12 = 0;
  v13 = v11 + 2;
  do
  {
    v14 = 0;
    do
    {
      v15 = -((v14 ^ v12) & 0x10);
      v13[1] = v15;
      *v13 = v15;
      *(v13 - 1) = v15;
      *(v13 - 2) = v15;
      ++v14;
      v13 += 4;
    }
    while ( v14 < 128 );
    ++v12;
  }
  while ( v12 < 128 );
  idMegaTextureTileDecompressor::RecompressTile(
    megaTextureTileDecompressor,
    v17->imageCompressionFormat,
    v11,
    v17->gridTileData);
}
void __thiscall idImageGeneratorFunctor<sdDeclAmbientCubeMap>::~idImageGeneratorFunctor<sdDeclAmbientCubeMap>(idImageGeneratorFunctor<sdDeclAmbientCubeMap> *this)
{
  this->vfptr = (idImageGeneratorFunctorBaseVtbl *)&idImageGeneratorFunctorBase::`vftable';
}
char *__thiscall idMegaTextureLevel::GetCompressedTileData(idMegaTextureLevel *this, int globalX, int globalY)
{
  return this->compressedTiles[globalX % this->compressedTilesPerAxis
                             + this->compressedTilesPerAxis * (globalY % this->compressedTilesPerAxis)];
}
void __thiscall idMegaTextureTile::PostInit(idMegaTextureTile *this)
{
  int v1; // edi
  int v2; // eax
  idMegaTextureLevel *v3; // eax
  idMegaTextureLevel *v4; // ebx
  int v5; // eax
  char **v6; // esi
  signed int v7; // ebp
  idMegaTextureTile *v8; // [esp+4h] [ebp-4h]

  v1 = 0;
  v2 = this->level->levelNum - 1;
  v8 = this;
  if ( v2 >= 0 )
  {
    v3 = &this->level->megaTexture->levels[v2];
    v4 = v3;
    if ( v3->isInterleaved )
    {
      v5 = v3->maxCompressedTileSize;
      v6 = this->childCompressedTileData;
      v7 = 4;
      do
      {
        *v6 = (char *)Mem_AllocAligned(v5 + 3, ALIGN_16);
        v5 = v4->maxCompressedTileSize;
        ++v6;
        --v7;
        v1 += v5 + 3;
      }
      while ( v7 );
      this = v8;
    }
  }
  this->level->usedMemory += v1;
}
char __thiscall idMegaTextureTile::IsLoaded(idMegaTextureTile *this)
{
  idMegaTextureLevel *v1; // eax

  v1 = this->level;
  if ( this->level->alwaysCached )
    return 1;
  if ( v1->isInterleaved )
    return idMegaTextureTile::IsLoaded(
             (idMegaTextureTile *)v1->megaTexture->levels[v1->levelNum + 1].tiles
           + ((this->globalY >> 1) & 0xF)
           + 16 * ((this->globalX >> 1) & 0xF)) != 0;
  return this->loaded;
}
char *__thiscall idMegaTextureTile::GetChildCompressedTileData(idMegaTextureTile *this, const int index)
{
  return this->childCompressedTileData[index];
}
void __thiscall idImageGeneratorFunctor<idMegaTextureLevel>::operator()(idImageGeneratorFunctor<sdDeclAmbientCubeMap> *this, idImage *image)
{
  this->imageGenerator(this->generatorClass, image);
}
int __cdecl GetPercentageTilesReady(int levelNum)
{
  idMegaTexture *v1; // ecx
  signed int v2; // edx
  bool *v3; // ecx
  signed int v4; // esi
  signed int v5; // eax

  v1 = megaTextureTileLoader->activeMegaTexture;
  if ( !v1 || levelNum < 0 || levelNum >= v1->numLevels )
    return 0;
  v2 = 256;
  v3 = &v1->levels[levelNum].tiles[0][1].dirty;
  v4 = 16;
  do
  {
    v5 = 4;
    do
    {
      if ( *(v3 - 64) )
        --v2;
      if ( *v3 )
        --v2;
      if ( v3[64] )
        --v2;
      if ( v3[128] )
        --v2;
      v3 += 256;
      --v5;
    }
    while ( v5 );
    --v4;
  }
  while ( v4 );
  return 100 * v2 / 256;
}
void __thiscall idMegaTexture::idMegaTexture(idMegaTexture *this)
{
  idMegaTexture *v1; // esi
  char *v2; // ecx

  v1 = this;
  v2 = this->name.baseBuffer;
  v1->name.len = 0;
  v1->name.alloced = -20;
  v1->name.data = v2;
  *v2 = 0;
  v1->winFile = (void *)-1;
  v1->winFileBlockOffset = -1;
  v1->winFileNumBlocks = -1;
  v1->version = 0;
  v1->resolution = 0;
  v1->purged = 1;
  v1->file = 0;
  v1->lastTileOffset = 0;
  v1->winFileScratch = 0;
  v1->imageCompressionFormat = 33776;
  v1->useImageCompression = 0;
  v1->forcedUpdate = 0;
  v1->detailTexture = globalImages->grayImage;
  v1->detailTextureMask = globalImages->defaultDetailMaskImage;
  v1->lastUsedFrame = 0;
  v1->currentWorld = 0;
  v1->levels = 0;
  v1->upscaleLevel = 0;
  v1->stGrid = 0;
  v1->tileIndexMap = 0;
  v1->tileIndexedDataSizes = 0;
  v1->nullTileData = 0;
  v1->gridTileData = 0;
  v1->tileRecompressionScratch = 0;
  sdLock::sdLock(&v1->lock);
  v1->currentViewOrigin.z = 262144.0;
  v1->lastShaderQuality = r_shaderQuality.internalVar->integerValue;
}
void __thiscall idMegaTexture::~idMegaTexture(idMegaTexture *this)
{
  idMegaTexture *v1; // esi

  v1 = this;
  sdLock::~sdLock(&this->lock);
  idStr::FreeData(&v1->name);
  v1->name.len = 0;
  v1->name.alloced = -20;
  v1->name.data = v1->name.baseBuffer;
  v1->name.baseBuffer[0] = 0;
}
void __thiscall idMegaTexture::Reset(idMegaTexture *this)
{
  int v1; // eax
  int v2; // edx
  idMegaTextureLevel *v3; // ecx

  if ( !this->purged )
  {
    v1 = 0;
    if ( this->numLevels > 0 )
    {
      v2 = 0;
      do
      {
        this->levels[v2].fadeTime = 0;
        ++v1;
        ++v2;
      }
      while ( v1 < this->numLevels );
    }
    v3 = this->upscaleLevel;
    if ( v3 )
      v3->fadeTime = 0;
  }
}
void __thiscall idMegaTexture::UpdateMapping(idMegaTexture *this, idRenderWorldLocal *world)
{
  idMegaTexture *v2; // esi
  int v3; // eax
  int v4; // ecx
  idVec2 *v5; // edx
  double v6; // st7

  v2 = this;
  if ( world )
  {
    if ( megaTextureTileLoader->activeMegaTexture != this )
      idMegaTextureTileLoader::SetActiveMegaTexture(megaTextureTileLoader, this);
    if ( megaTextureTileDecompressor->activeMegaTexture != v2 )
      idMegaTextureTileDecompressor::SetActiveMegaTexture(megaTextureTileDecompressor, v2);
    v3 = world->megaTextureSTGridWidth;
    v4 = world->megaTextureSTGridHeight;
    v5 = world->megaTextureSTGrid;
    if ( world != v2->currentWorld || v5 != v2->stGrid || v4 != v2->stGridHeight || v3 != v2->stGridWidth )
    {
      v2->currentWorld = world;
      v2->stGridBounds.bounds[0].x = world->megaTextureBounds.b[0].x;
      v2->stGridBounds.bounds[0].y = world->megaTextureBounds.b[0].y;
      v2->stGridBounds.bounds[1].x = world->megaTextureBounds.b[1].x;
      v6 = world->megaTextureBounds.b[1].y;
      v2->stGridWidth = v3;
      v2->stGridBounds.bounds[1].y = v6;
      v2->stGridHeight = v4;
      v2->stGrid = v5;
    }
  }
}
void __stdcall idMegaTexture::UpdateLevelForViewOrigin(idMegaTextureLevel *level, int idx, int time)
{
  idMegaTextureLevel *v3; // eax
  sdDeclRenderBinding *v4; // ecx
  sdDeclRenderBinding *v5; // ecx
  int v6; // ecx
  float opacitya; // [esp+8h] [ebp+4h]
  float opacity; // [esp+8h] [ebp+4h]

  v3 = level;
  if ( level->imageValid )
  {
    v4 = rbinds->megaMaskParams[idx];
    v4->data.vector[0] = level->parms[0];
    v4->data.vector[1] = level->parms[1];
    v4->data.vector[2] = level->parms[2];
    v4->data.vector[3] = level->parms[3];
    v5 = rbinds->megaTextureParams[idx];
    opacitya = (double)(1 << (idx + 1)) * 0.5;
    v5->data.vector[3] = opacitya;
    v5->data.vector[2] = opacitya;
    v5->data.vector[1] = opacitya;
    v5->data.vector[0] = opacitya;
  }
  opacity = 1.0;
  v6 = v3->fadeTime;
  if ( v6 > time - idMegaTexture::r_megaFadeTime.internalVar->integerValue )
    opacity = (double)(time - v6) / (double)idMegaTexture::r_megaFadeTime.internalVar->integerValue;
  if ( (unsigned int)(idx - 1) <= 3 )
    rbinds->megaTextureOpacity15->data.vector[idx - 1] = opacity;
  if ( idMegaTexture::r_showMegaTextureLevels.internalVar->integerValue )
  {
    if ( idx & 1 )
      rbinds->megaTextureLevel[idx]->data.attrib = (int)globalImages->blackImage;
    else
      rbinds->megaTextureLevel[idx]->data.attrib = (int)globalImages->whiteImage;
  }
  else
  {
    rbinds->megaTextureLevel[idx]->data.attrib = (int)v3->image;
  }
}
char __thiscall idMegaTexture::CloseFile(idMegaTexture *this)
{
  idMegaTexture *v1; // esi
  idFile *v2; // ecx

  v1 = this;
  if ( this->winFile != (void *)-1 )
  {
    CloseHandle(this->winFile);
    v1->winFile = (void *)-1;
  }
  if ( v1->winFileScratch )
  {
    Mem_FreeAligned(v1->winFileScratch);
    v1->winFileScratch = 0;
  }
  v2 = v1->file;
  if ( v2 )
  {
    ((void (__stdcall *)(signed int))v2->vfptr->__vecDelDtor)(1);
    v1->file = 0;
  }
  return 1;
}
int __thiscall idMegaTexture::SeekToTile(idMegaTexture *this, int tileNum)
{
  idMegaTexture *v2; // esi
  int v3; // ebx
  int v4; // eax
  int v5; // edi
  int v6; // ebp
  int result; // eax
  void *v8; // ecx
  int v9; // eax
  int v10; // eax
  int v11; // ebx
  int v12; // edi
  DWORD v13; // eax
  int numberOfBytesRead; // [esp+10h] [ebp-1Ch]
  _OVERLAPPED overlapped; // [esp+14h] [ebp-18h]
  int v16; // [esp+28h] [ebp-4h]

  v2 = this;
  v3 = this->tileIndexMap[tileNum] & 0x7FFF;
  v4 = this->winFileBlockOffset;
  v5 = this->tileIndexMap[tileNum] / 0x8000;
  v6 = (this->tileIndexedDataSizes[tileNum] + v3 + 32770) / 0x8000;
  numberOfBytesRead = this->tileIndexMap[tileNum] & 0x7FFF;
  if ( v5 < v4 || v5 + v6 > v4 + this->winFileNumBlocks )
  {
    if ( v6 < idMegaTexture::r_megaStreamBlocks.internalVar->integerValue )
      v6 = idMegaTexture::r_megaStreamBlocks.internalVar->integerValue;
    overlapped.OffsetHigh = 0;
    overlapped.InternalHigh = 0;
    overlapped.Offset = 0;
    v16 = 0;
    overlapped.hEvent = 0;
    v8 = this->winFile;
    overlapped.OffsetHigh = v5 << 15;
    ReadFile(v8, v2->winFileScratch, v6 << 15, &overlapped.Internal, (_OVERLAPPED *)((char *)&overlapped + 4));
    v9 = v5 - v2->winFileNumBlocks;
    v2->winFileNumBlocks = v6;
    v10 = v9 - v2->winFileBlockOffset;
    v2->winFileBlockOffset = v5;
    v11 = abs(v10 << 15);
    v12 = lastDataLoadTime & 0x7FF;
    v13 = Sys_Milliseconds();
    ++lastDataLoadTime;
    dataLoadSizes[v12] = v6 << 15;
    dataLoadTimes[v12] = v13;
    v2->file->vfptr->Seek(v2->file, numberOfBytesRead, 0);
    result = v11;
  }
  else
  {
    this->file->vfptr->Seek(this->file, v3 + ((v5 - v4) << 15), 0);
    result = 0;
  }
  return result;
}
void __thiscall idMegaTexture::TestStreamingPerformance(idMegaTexture *this, idCmdArgs *args)
{
  idMegaTexture *v2; // esi
  char *v3; // ecx
  signed int v4; // edi
  void *v5; // ST00_4
  BOOL v6; // ebp
  signed int v7; // ebx
  char *v8; // eax
  BOOL v9; // eax
  int startTime; // [esp+1Ch] [ebp-1Ch]
  _OVERLAPPED overlapped; // [esp+20h] [ebp-18h]
  int v12; // [esp+34h] [ebp-4h]

  v2 = this;
  v3 = this->winFileScratch;
  v4 = 0;
  if ( v3 )
  {
    overlapped.OffsetHigh = 0;
    overlapped.hEvent = 0;
    overlapped.InternalHigh = 0;
    overlapped.Offset = 0;
    v12 = 0;
    v5 = v2->winFile;
    overlapped.OffsetHigh = 0;
    overlapped.hEvent = 0;
    v6 = ReadFile(v5, v3, 0x100000u, (LPDWORD)&startTime, (_OVERLAPPED *)((char *)&overlapped + 4));
    overlapped.Internal = Sys_Milliseconds();
    v7 = 0;
    do
    {
      if ( !v6 )
        break;
      v8 = v2->winFileScratch;
      overlapped.OffsetHigh += 0x100000;
      v9 = ReadFile(v2->winFile, v8, 0x100000u, (LPDWORD)&startTime, (_OVERLAPPED *)((char *)&overlapped + 4));
      v4 += startTime;
      ++v7;
      v6 = v9;
    }
    while ( v7 < 100 );
    overlapped.Internal = Sys_Milliseconds() - overlapped.Internal;
    (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
      common.type,
      "%1.2f MB/s\n",
      (double)v4 / (double)(signed int)overlapped.Internal * 0.00095367431640625);
    v2->winFileBlockOffset = -1;
    v2->winFileNumBlocks = 0;
  }
  else
  {
    (*(void (__cdecl **)(netadrtype_t, const char *))(*(_DWORD *)common.type + 52))(common.type, "no mega texture\n");
  }
}
void __cdecl idMegaTexture::MegaTestStreamingPerformance_f(idCmdArgs *args)
{
  idMegaTexture::TestStreamingPerformance(megaTextureTileLoader->activeMegaTexture, args);
}
void __thiscall idMegaTexture::AllocRecompressionScratch(idMegaTexture *this)
{
  idMegaTexture *v1; // ebx
  signed int v2; // esi
  signed int v3; // edx
  signed int v4; // ecx
  signed int v5; // eax
  signed int v6; // edi
  unsigned int v7; // eax
  int v8; // ecx

  v1 = this;
  if ( this->useImageCompression )
  {
    v2 = 128;
    v3 = 128;
    v4 = 128;
    v5 = 128;
    v6 = 1;
    while ( v5 > 1 || v4 > 1 )
    {
      ++v6;
      v5 >>= 1;
      v4 >>= 1;
    }
    v7 = 0;
    if ( v6 > 0 )
    {
      do
      {
        v8 = v2 * v3;
        v3 >>= 1;
        v2 >>= 1;
        --v6;
        v7 += 4 * v8;
      }
      while ( v6 );
    }
    v1->tileRecompressionScratch = (char *)Mem_AllocAligned(v7, ALIGN_16);
  }
  else
  {
    this->tileRecompressionScratch = 0;
  }
}
int __cdecl idMegaTexture::TotalStoredTileCount(const int resolution)
{
  int v1; // eax
  int v2; // edx
  int v3; // esi
  int v4; // eax
  int i; // ecx
  float resolutiona; // [esp+4h] [ebp+4h]

  v1 = resolution;
  v2 = (resolution >> 31) & 0x7F;
  resolutiona = (double)(resolution / 2048);
  v3 = 0;
  v4 = (v2 + v1) >> 7;
  for ( i = ((SLODWORD(resolutiona) >> 23) & 0xFF) - 127; i >= 0; --i )
  {
    v3 += v4 * v4;
    v4 >>= 1;
  }
  return v3;
}
void __stdcall idMegaTextureLevel::RemoveDirtyTile(idMegaTextureTile *tile)
{
  idLinkList<idMegaTextureTile> *v1; // eax

  v1 = &tile->dirtyNode;
  tile->dirtyNode.prev->next = tile->dirtyNode.next;
  tile->dirtyNode.next->prev = tile->dirtyNode.prev;
  v1->next = v1;
  v1->prev = v1;
  v1->head = v1;
}
void __thiscall idMegaTextureTile::Purge(idMegaTextureTile *this)
{
  idMegaTextureTile *v1; // esi
  char *v2; // ST00_4
  void **v3; // esi
  signed int v4; // edi

  v1 = this;
  this->globalX = -99999;
  this->globalY = -99999;
  v2 = this->compressedTileData;
  this->tileData = 0;
  this->dirty = 0;
  this->loaded = 0;
  Mem_FreeAligned(v2);
  v1->compressedTileData = 0;
  v3 = (void **)v1->childCompressedTileData;
  v4 = 4;
  do
  {
    Mem_FreeAligned(*v3);
    *v3 = 0;
    ++v3;
    --v4;
  }
  while ( v4 );
}
char *__thiscall idMegaTextureTile::GetCompressedTileData(idMegaTextureTile *this)
{
  idMegaTextureLevel *v1; // esi
  char *result; // eax

  v1 = this->level;
  if ( this->level->isInterleaved )
    return v1->megaTexture->levels[1].tiles[0][0].childCompressedTileData[(this->globalX & 1)
                                                                        + 4135 * v1->levelNum
                                                                        + 2
                                                                        * ((this->globalY & 1)
                                                                         + 8
                                                                         * (((this->globalY >> 1) & 0xF)
                                                                          + 16 * ((this->globalX >> 1) & 0xF)))];
  result = this->compressedTileData;
  if ( !result )
    result = v1->compressedTiles[this->globalX % v1->compressedTilesPerAxis
                               + v1->compressedTilesPerAxis * (this->globalY % v1->compressedTilesPerAxis)];
  return result;
}
void __thiscall idMegaTextureTile::Upload(idMegaTextureTile *this, idMegaTexture *megaTexture)
{
  idMegaTextureTile *v2; // ebp
  int v3; // ebx
  tileData_t *v4; // eax
  idMegaTexture *v5; // ecx
  int v6; // esi
  int v7; // [esp+4h] [ebp-4h]
  char *data; // [esp+Ch] [ebp+4h]

  v2 = this;
  if ( !this->dirty )
    return;
  v3 = 0;
  if ( idMegaTexture::r_skipMegaTextureUpload.internalVar->integerValue )
  {
    this->dirty = 0;
    return;
  }
  v4 = this->tileData;
  v5 = megaTexture;
  v6 = 128;
  v7 = 0;
  if ( v4 )
    data = v4->pic;
  else
    data = megaTexture->nullTileData;
  if ( v5->imageCompressionFormat == 32856 )
  {
    do
    {
      qglTexSubImage2D(0xDE1u, v3, v6 * v2->localX, v6 * v2->localY, v6, v6, 0x1908u, 0x1401u, &data[v7]);
      v7 += 4 * v6 * v6;
      v6 >>= 1;
      ++v3;
    }
    while ( v6 >= 4 );
    goto LABEL_15;
  }
  if ( v5->imageCompressionFormat != 33776 )
  {
    if ( v5->imageCompressionFormat == 33779 )
    {
      do
      {
        qglCompressedTexSubImage2DARB(
          0xDE1u,
          v3,
          v6 * v2->localX,
          v6 * v2->localY,
          v6,
          v6,
          0x83F3u,
          16 * (v6 + 3) / 4 * ((v6 + 3) / 4),
          &data[v7]);
        v7 += 16 * (v6 + 3) / 4 * ((v6 + 3) / 4);
        v6 >>= 1;
        ++v3;
      }
      while ( v6 >= 4 );
      v2->dirty = 0;
      return;
    }
LABEL_15:
    v2->dirty = 0;
    return;
  }
  do
  {
    qglCompressedTexSubImage2DARB(
      0xDE1u,
      v3,
      v6 * v2->localX,
      v6 * v2->localY,
      v6,
      v6,
      0x83F0u,
      8 * (v6 + 3) / 4 * ((v6 + 3) / 4),
      &data[v7]);
    v7 += 8 * (v6 + 3) / 4 * ((v6 + 3) / 4);
    v6 >>= 1;
    ++v3;
  }
  while ( v6 >= 4 );
  v2->dirty = 0;
}
int __thiscall idMegaTexture::GetPureServerChecksum(idMegaTexture *this, unsigned int offset)
{
  unsigned int v2; // ebx
  idMegaTexture *v3; // esi
  char *v4; // edx
  const char *v5; // ecx
  char v6; // al
  char *v7; // edi
  int v8; // ecx
  int i; // eax
  char v10; // cl
  int v11; // edx
  int v12; // eax
  char *v13; // esi
  int v14; // eax
  char v15; // cl
  int v16; // edx
  const char *v17; // eax
  int v18; // eax
  int v19; // esi
  const char *v20; // eax
  int v21; // esi
  int v22; // ebp
  signed int v23; // edi
  int v24; // eax
  int v25; // edx
  int v26; // eax
  int v28; // [esp+28h] [ebp-34h]
  idStr fileName; // [esp+2Ch] [ebp-30h]
  int v30; // [esp+58h] [ebp-4h]

  v2 = offset;
  *(_DWORD *)fileName.baseBuffer = -20;
  v3 = this;
  fileName.data = 0;
  fileName.alloced = (int)&fileName.baseBuffer[4];
  fileName.baseBuffer[4] = 0;
  v4 = &fileName.baseBuffer[4];
  v5 = "megatextures/";
  do
  {
    v6 = *v5;
    *v4++ = *v5++;
  }
  while ( v6 );
  fileName.data = (char *)13;
  v7 = (char *)(v3->name.len + 13);
  v8 = v3->name.len + 14;
  v30 = 0;
  if ( v8 > abs(*(_DWORD *)fileName.baseBuffer) )
    idStr::ReAllocate((idStr *)((char *)&fileName + 4), v8, 1);
  for ( i = 0; i < v3->name.len; fileName.data[v11] = v10 )
  {
    v10 = v3->name.data[i];
    v11 = i++ + fileName.alloced;
  }
  fileName.data = v7;
  v7[fileName.alloced] = 0;
  idStr::StripFileExtension((idStr *)((char *)&fileName + 4));
  v12 = 0;
  do
    ++v12;
  while ( aMega[v12] );
  v13 = &fileName.data[v12];
  if ( (_DWORD)&fileName.data[v12 + 1] > abs(*(_DWORD *)fileName.baseBuffer) )
    idStr::ReAllocate((idStr *)((char *)&fileName + 4), (int)(v13 + 1), 1);
  v14 = 0;
  v15 = 46;
  do
  {
    v16 = v14++ + fileName.alloced;
    fileName.data[v16] = v15;
    v15 = aMega[v14];
  }
  while ( v15 );
  fileName.data = v13;
  v13[fileName.alloced] = 0;
  if ( idMegaTexture::r_megaStreamFromDVD.internalVar->integerValue )
  {
    v17 = (const char *)((int (__stdcall *)(int, const char *))fileSystem->vfptr->RelativePathToOSPath)(
                          fileName.alloced,
                          "fs_cdpath");
    idStr::operator=((idStr *)((char *)&fileName + 4), v17);
  }
  else
  {
    v18 = ((int (__stdcall *)(int, signed int, _DWORD, signed int))fileSystem->vfptr->OpenFileRead)(
            fileName.alloced,
            1,
            0,
            1);
    v19 = v18;
    if ( !v18 )
    {
LABEL_26:
      v30 = -1;
      idStr::FreeData((idStr *)((char *)&fileName + 4));
      return 0;
    }
    v20 = (const char *)(*(int (__thiscall **)(int))(*(_DWORD *)v18 + 8))(v18);
    idStr::operator=((idStr *)((char *)&fileName + 4), v20);
    ((void (__stdcall *)(int))fileSystem->vfptr->CloseFile)(v19);
  }
  v21 = ((int (__stdcall *)(int))fileSystem->vfptr->OpenExplicitFileRead)(fileName.alloced);
  if ( !v21 )
    goto LABEL_26;
  v28 = 0;
  fileName.len = 16;
  do
  {
    v22 = 0;
    v23 = 0;
    do
    {
      v24 = (*(int (__thiscall **)(int))(*(_DWORD *)v21 + 20))(v21);
      if ( v24 == 1 )
      {
        v25 = 0;
      }
      else
      {
        v2 = 69069 * v2 + 1;
        v25 = (unsigned __int16)(v2 & 0x7FFF) % (v24 - 1);
      }
      (*(void (__thiscall **)(int, int, _DWORD))(*(_DWORD *)v21 + 40))(v21, v25, 0);
      (*(void (__thiscall **)(int, unsigned int *))(*(_DWORD *)v21 + 80))(v21, &offset);
      v26 = (unsigned __int8)offset << v23;
      v23 += 8;
      v22 |= v26;
    }
    while ( v23 < 32 );
    v28 ^= v22;
    --fileName.len;
  }
  while ( fileName.len );
  ((void (__stdcall *)(int))fileSystem->vfptr->CloseFile)(v21);
  v30 = -1;
  idStr::FreeData((idStr *)((char *)&fileName + 4));
  return v28;
}
char __thiscall idMegaTexture::OpenFile(idMegaTexture *this)
{
  idMegaTexture *v1; // esi
  char *v2; // edx
  const char *v3; // ecx
  char v4; // al
  char *v5; // edi
  int v6; // ecx
  int i; // eax
  char v8; // cl
  int v9; // edx
  int v10; // eax
  char *v11; // edi
  int v12; // eax
  char v13; // cl
  int v14; // edx
  const char *v15; // eax
  int v16; // eax
  const char *v17; // eax
  HANDLE v18; // eax
  DWORD v19; // esi
  char *v20; // eax
  idFile *v21; // ecx
  idFile_Memory *v22; // eax
  idFile *v23; // eax
  char *v24; // ecx
  void *v25; // edx
  idFile *v26; // ecx
  int (__thiscall *v27)(idFile *, int *); // eax
  int *v28; // esi
  int v29; // esi
  int ident; // [esp+28h] [ebp-4Ch]
  unsigned int numberOfBytesRead; // [esp+2Ch] [ebp-48h]
  _OVERLAPPED overlapped; // [esp+30h] [ebp-44h]
  idStr fileName; // [esp+44h] [ebp-30h]
  int v35; // [esp+70h] [ebp-4h]

  *(_DWORD *)fileName.baseBuffer = -20;
  v1 = this;
  fileName.data = 0;
  fileName.alloced = (int)&fileName.baseBuffer[4];
  fileName.baseBuffer[4] = 0;
  v2 = &fileName.baseBuffer[4];
  v3 = "megatextures/";
  do
  {
    v4 = *v3;
    *v2++ = *v3++;
  }
  while ( v4 );
  fileName.data = (char *)13;
  v5 = (char *)(v1->name.len + 13);
  v6 = v1->name.len + 14;
  v35 = 0;
  if ( v6 > abs(*(_DWORD *)fileName.baseBuffer) )
    idStr::ReAllocate((idStr *)((char *)&fileName + 4), v6, 1);
  for ( i = 0; i < v1->name.len; fileName.data[v9] = v8 )
  {
    v8 = v1->name.data[i];
    v9 = i++ + fileName.alloced;
  }
  fileName.data = v5;
  v5[fileName.alloced] = 0;
  idStr::StripFileExtension((idStr *)((char *)&fileName + 4));
  v10 = 0;
  do
    ++v10;
  while ( aMega[v10] );
  v11 = &fileName.data[v10];
  if ( (_DWORD)&fileName.data[v10 + 1] > abs(*(_DWORD *)fileName.baseBuffer) )
    idStr::ReAllocate((idStr *)((char *)&fileName + 4), (int)(v11 + 1), 1);
  v12 = 0;
  v13 = 46;
  do
  {
    v14 = v12++ + fileName.alloced;
    fileName.data[v14] = v13;
    v13 = aMega[v12];
  }
  while ( v13 );
  fileName.data = v11;
  v11[fileName.alloced] = 0;
  if ( idMegaTexture::r_megaStreamFromDVD.internalVar->integerValue )
  {
    v15 = (const char *)((int (__stdcall *)(int, const char *))fileSystem->vfptr->RelativePathToOSPath)(
                          fileName.alloced,
                          "fs_cdpath");
    idStr::operator=((idStr *)((char *)&fileName + 4), v15);
  }
  else
  {
    v16 = ((int (__stdcall *)(int, signed int, _DWORD, signed int))fileSystem->vfptr->OpenFileRead)(
            fileName.alloced,
            1,
            0,
            1);
    v1->file = (idFile *)v16;
    if ( !v16 )
    {
      v35 = -1;
      goto LABEL_35;
    }
    v17 = (const char *)(*(int (__thiscall **)(int))(*(_DWORD *)v16 + 8))(v16);
    idStr::operator=((idStr *)((char *)&fileName + 4), v17);
    ((void (__stdcall *)(idFile *))fileSystem->vfptr->CloseFile)(v1->file);
    v1->file = 0;
  }
  v18 = CreateFileA((LPCSTR)fileName.alloced, 0x80000000, 1u, 0, 3u, 0x20000000u, 0);
  v1->winFile = v18;
  if ( v18 == (HANDLE)-1 )
  {
    v19 = GetLastError();
    FormatMessageA(0x1300u, 0, v19, 0x400u, (LPSTR)&ident, 0, 0);
    (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
      common.type,
      "idMegaTexture::OpenFile : failed to open '%s' (%d : %s)",
      fileName.alloced,
      v19,
      ident);
    LocalFree((HLOCAL)ident);
    v35 = -1;
  }
  else
  {
    if ( !v1->winFileScratch )
    {
      v20 = (char *)Mem_AllocAligned(0x100000u, ALIGN_16);
      v21 = v1->file;
      v1->winFileScratch = v20;
      if ( v21 )
        ((void (__stdcall *)(signed int))v21->vfptr->__vecDelDtor)(1);
      v22 = (idFile_Memory *)operator new(0x40u);
      ident = (int)v22;
      LOBYTE(v35) = 1;
      if ( v22 )
        idFile_Memory::idFile_Memory(v22, "winFileScratch", v1->winFileScratch, 0x100000);
      else
        v23 = 0;
      LOBYTE(v35) = 0;
      v1->file = v23;
    }
    v24 = v1->winFileScratch;
    overlapped.OffsetHigh = 0;
    overlapped.hEvent = 0;
    v25 = v1->winFile;
    overlapped.InternalHigh = 0;
    overlapped.Offset = 0;
    fileName.len = 0;
    overlapped.OffsetHigh = 0;
    overlapped.hEvent = 0;
    if ( ReadFile(v25, v24, 0x100000u, &overlapped.Internal, (_OVERLAPPED *)((char *)&overlapped + 4)) )
    {
      v26 = v1->file;
      v1->winFileBlockOffset = 0;
      v1->winFileNumBlocks = 32;
      ((void (__stdcall *)(unsigned int *))v26->vfptr->ReadInt)(&numberOfBytesRead);
      if ( numberOfBytesRead == 1095189837 )
      {
        v27 = v1->file->vfptr->ReadInt;
        v28 = &v1->version;
        ((void (__stdcall *)(int *))v27)(v28);
        v29 = *v28;
        if ( v29 == 9 || v29 == 8 )
        {
          ((void (__stdcall *)(const char *, void (__cdecl *)(idCmdArgs *), signed int, const char *, _DWORD))cmdSystem->vfptr->AddCommand)(
            "megaTestStreamingPerformance",
            idMegaTexture::MegaTestStreamingPerformance_f,
            4,
            "Tests streaming performance without seeking.",
            0);
          ((void (__stdcall *)(const char *, void (__cdecl *)(idCmdArgs *), signed int, const char *, _DWORD))cmdSystem->vfptr->AddCommand)(
            "megaShowMemoryUsage",
            idMegaTexture::MegaShowMemoryUsage_f,
            4,
            "Show memory usage of active mega texture.",
            0);
          v35 = -1;
          idStr::FreeData((idStr *)((char *)&fileName + 4));
          return 1;
        }
        (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
          common.type,
          "idMegaTexture::OpenFile : wrong version on '%s' (%i should be %i)",
          fileName.alloced,
          v29,
          9);
        v35 = -1;
      }
      else
      {
        (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
          common.type,
          "idMegaTexture::OpenFile : unknown fileid on '%s'",
          fileName.alloced);
        v35 = -1;
      }
    }
    else
    {
      (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
        common.type,
        "idMegaTexture::OpenFile : failed to read '%s'",
        fileName.alloced);
      v35 = -1;
    }
  }
LABEL_35:
  idStr::FreeData((idStr *)((char *)&fileName + 4));
  return 0;
}
void __thiscall idMegaTexture::LoadDetailTexture(idMegaTexture *this)
{
  idMegaTexture *v1; // edi
  char *v2; // edx
  const char *v3; // ecx
  char v4; // al
  char *v5; // esi
  int v6; // ecx
  int v7; // eax
  char *v8; // esi
  char *v9; // ecx
  _BYTE *v10; // edx
  char v11; // al
  int v12; // eax
  char *v13; // esi
  int v14; // eax
  char v15; // cl
  int v16; // edx
  idImage *(__thiscall *v17)(idImageManager *, const char *, imageParams_t); // edx
  int v18; // eax
  char *v19; // esi
  char *v20; // ecx
  _BYTE *v21; // edx
  char v22; // al
  int v23; // eax
  char *v24; // esi
  int v25; // eax
  char v26; // cl
  int v27; // edx
  idImage *(__thiscall *v28)(idImageManager *, const char *, imageParams_t); // edx
  int v29; // eax
  int v30; // [esp+48h] [ebp-124h]
  int v31; // [esp+4Ch] [ebp-120h]
  int v32; // [esp+50h] [ebp-11Ch]
  int v33; // [esp+54h] [ebp-118h]
  int v34; // [esp+58h] [ebp-114h]
  int v35; // [esp+5Ch] [ebp-110h]
  int v36; // [esp+60h] [ebp-10Ch]
  int v37; // [esp+64h] [ebp-108h]
  int v38; // [esp+68h] [ebp-104h]
  int v39; // [esp+6Ch] [ebp-100h]
  int v40; // [esp+70h] [ebp-FCh]
  int v41; // [esp+74h] [ebp-F8h]
  int v42; // [esp+78h] [ebp-F4h]
  int v43; // [esp+7Ch] [ebp-F0h]
  int v44; // [esp+80h] [ebp-ECh]
  int v45; // [esp+84h] [ebp-E8h]
  int v46; // [esp+88h] [ebp-E4h]
  int v47; // [esp+8Ch] [ebp-E0h]
  int v48; // [esp+90h] [ebp-DCh]
  int v49; // [esp+94h] [ebp-D8h]
  idStr detailMaskFileName; // [esp+A8h] [ebp-C4h]
  idStr fileName; // [esp+C8h] [ebp-A4h]
  int v52; // [esp+ECh] [ebp-80h]
  int v53; // [esp+F0h] [ebp-7Ch]
  int v54; // [esp+F4h] [ebp-78h]
  int v55; // [esp+F8h] [ebp-74h]
  char v56; // [esp+FCh] [ebp-70h]
  char v57; // [esp+120h] [ebp-4Ch]
  int v58; // [esp+124h] [ebp-48h]
  int v59; // [esp+128h] [ebp-44h]
  float v60; // [esp+12Ch] [ebp-40h]
  char v61; // [esp+130h] [ebp-3Ch]
  float v62; // [esp+134h] [ebp-38h]
  mipmapState_t ms; // [esp+138h] [ebp-34h]
  int v64; // [esp+15Ch] [ebp-10h]
  int v65; // [esp+168h] [ebp-4h]

  *(_DWORD *)fileName.baseBuffer = -20;
  v1 = this;
  detailMaskFileName.len = (int)this;
  fileName.data = 0;
  fileName.alloced = (int)&fileName.baseBuffer[4];
  fileName.baseBuffer[4] = 0;
  v2 = &fileName.baseBuffer[4];
  v3 = "megatextures/";
  do
  {
    v4 = *v3;
    *v2++ = *v3++;
  }
  while ( v4 );
  fileName.data = (char *)13;
  v5 = (char *)(v1->name.len + 13);
  v6 = v1->name.len + 14;
  v65 = 0;
  if ( v6 > abs(*(_DWORD *)fileName.baseBuffer) )
    idStr::ReAllocate((idStr *)((char *)&fileName + 4), v6, 1);
  v7 = 0;
  if ( v1->name.len > 0 )
  {
    while ( 1 )
    {
      fileName.data[v7 + fileName.alloced] = v1->name.data[v7];
      if ( ++v7 >= *(_DWORD *)detailMaskFileName.len )
        break;
      v1 = (idMegaTexture *)detailMaskFileName.len;
    }
  }
  fileName.data = v5;
  v5[fileName.alloced] = 0;
  idStr::StripFileExtension((idStr *)((char *)&fileName + 4));
  detailMaskFileName.alloced = (int)&detailMaskFileName.baseBuffer[4];
  v8 = fileName.data;
  detailMaskFileName.data = 0;
  *(_DWORD *)detailMaskFileName.baseBuffer = -20;
  detailMaskFileName.baseBuffer[4] = 0;
  if ( (_DWORD)(fileName.data + 1) > 20 )
    idStr::ReAllocate((idStr *)((char *)&detailMaskFileName + 4), (int)(fileName.data + 1), 1);
  v9 = (char *)fileName.alloced;
  v10 = (_BYTE *)detailMaskFileName.alloced;
  do
  {
    v11 = *v9;
    *v10++ = *v9++;
  }
  while ( v11 );
  detailMaskFileName.data = v8;
  LOBYTE(v65) = 1;
  v12 = 0;
  do
    ++v12;
  while ( aDetailTga[v12] );
  v13 = &v8[v12];
  if ( (signed int)(v13 + 1) > abs(*(_DWORD *)detailMaskFileName.baseBuffer) )
    idStr::ReAllocate((idStr *)((char *)&detailMaskFileName + 4), (int)(v13 + 1), 1);
  v14 = 0;
  v15 = 95;
  do
  {
    v16 = v14++ + detailMaskFileName.alloced;
    detailMaskFileName.data[v16] = v15;
    v15 = aDetailTga[v14];
  }
  while ( v15 );
  ms.blend[0] = 0.5;
  detailMaskFileName.data = v13;
  ms.color[3] = 0.5;
  v13[detailMaskFileName.alloced] = 0;
  ms.color[2] = 0.5;
  ms.color[1] = 0.5;
  v17 = globalImages->vfptr->ImageFromFile;
  *(float *)&ms.colorType = idMegaTexture::r_detailFade.internalVar->floatValue;
  ms.blend[3] = *(float *)&ms.colorType;
  v61 = 0;
  ms.blend[2] = *(float *)&ms.colorType;
  v59 = -10;
  ms.blend[1] = *(float *)&ms.colorType;
  v52 = 3;
  v57 = 0;
  v62 = 0.0;
  v58 = 0;
  v53 = 0;
  ms.color[0] = 1000.0;
  v54 = 4;
  v55 = 0;
  v60 = -1.0;
  v64 = 1;
  qmemcpy(&v56, &ms.color[1], 0x24u);
  qmemcpy(&v30, &v52, 0x50u);
  v18 = ((int (__thiscall *)(idImageManager *, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int))v17)(
          globalImages,
          detailMaskFileName.alloced,
          v30,
          v31,
          v32,
          v33,
          v34,
          v35,
          v36,
          v37,
          v38,
          v39,
          v40,
          v41,
          v42,
          v43,
          v44,
          v45,
          v46,
          v47,
          v48,
          v49);
  *(_DWORD *)(detailMaskFileName.len + 76) = v18;
  LOBYTE(v65) = 0;
  idStr::FreeData((idStr *)((char *)&detailMaskFileName + 4));
  v19 = fileName.data;
  detailMaskFileName.data = 0;
  *(_DWORD *)detailMaskFileName.baseBuffer = -20;
  detailMaskFileName.alloced = (int)&detailMaskFileName.baseBuffer[4];
  detailMaskFileName.baseBuffer[4] = 0;
  if ( (_DWORD)(fileName.data + 1) > 20 )
    idStr::ReAllocate((idStr *)((char *)&detailMaskFileName + 4), (int)(fileName.data + 1), 1);
  v20 = (char *)fileName.alloced;
  v21 = (_BYTE *)detailMaskFileName.alloced;
  do
  {
    v22 = *v20;
    *v21++ = *v20++;
  }
  while ( v22 );
  detailMaskFileName.data = v19;
  LOBYTE(v65) = 2;
  v23 = 0;
  do
    ++v23;
  while ( aDetailmaskTga[v23] );
  v24 = &v19[v23];
  if ( (signed int)(v24 + 1) > abs(*(_DWORD *)detailMaskFileName.baseBuffer) )
    idStr::ReAllocate((idStr *)((char *)&detailMaskFileName + 4), (int)(v24 + 1), 1);
  v25 = 0;
  v26 = 95;
  do
  {
    v27 = v25++ + detailMaskFileName.alloced;
    detailMaskFileName.data[v27] = v26;
    v26 = aDetailmaskTga[v25];
  }
  while ( v26 );
  v62 = 0.0;
  detailMaskFileName.data = v24;
  v24[detailMaskFileName.alloced] = 0;
  ms.color[0] = 1000.0;
  v28 = globalImages->vfptr->ImageFromFile;
  v60 = -1.0;
  v61 = 0;
  v59 = -10;
  v52 = 3;
  v57 = 0;
  v58 = 0;
  v53 = 0;
  v54 = 4;
  v55 = 0;
  qmemcpy(&v56, &defaultMipmapState_34, 0x24u);
  qmemcpy(&v30, &v52, 0x50u);
  v29 = ((int (__thiscall *)(idImageManager *, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int))v28)(
          globalImages,
          detailMaskFileName.alloced,
          v30,
          v31,
          v32,
          v33,
          v34,
          v35,
          v36,
          v37,
          v38,
          v39,
          v40,
          v41,
          v42,
          v43,
          v44,
          v45,
          v46,
          v47,
          v48,
          v49);
  *(_DWORD *)(detailMaskFileName.len + 80) = v29;
  LOBYTE(v65) = 0;
  idStr::FreeData((idStr *)((char *)&detailMaskFileName + 4));
  detailMaskFileName.data = 0;
  *(_DWORD *)detailMaskFileName.baseBuffer = -20;
  detailMaskFileName.alloced = (int)&detailMaskFileName.baseBuffer[4];
  detailMaskFileName.baseBuffer[4] = 0;
  v65 = -1;
  idStr::FreeData((idStr *)((char *)&fileName + 4));
}
char __thiscall idMegaTextureLevel::UploadTiles(idMegaTextureLevel *this, int time)
{
  idMegaTextureLevel *v2; // ebx
  idLinkList<idMegaTextureTile> *v3; // eax
  signed int v5; // ecx
  bool *v6; // eax
  signed int v7; // esi
  signed int v8; // edx
  idImage *v9; // ecx
  idMegaTextureTile *v10; // esi
  signed int v11; // ebp
  signed int v12; // edi

  v2 = this;
  if ( this->dirty )
  {
    v3 = this->dirtyTiles.next;
    if ( v3 && v3 != this->dirtyTiles.head && v3->owner )
      return 0;
    v5 = 256;
    v6 = &v2->tiles[0][1].dirty;
    v7 = 16;
    do
    {
      v8 = 4;
      do
      {
        if ( *(v6 - 64) )
          --v5;
        if ( *v6 )
          --v5;
        if ( v6[64] )
          --v5;
        if ( v6[128] )
          --v5;
        v6 += 256;
        --v8;
      }
      while ( v8 );
      --v7;
    }
    while ( v7 );
    if ( v5 < 128 )
      v2->fadeTime = time;
    v9 = v2->image;
    v2->parms[0] = v2->newParms[0];
    v2->parms[1] = v2->newParms[1];
    ((void (*)(void))v9->vfptr->BindFragment)();
    v10 = (idMegaTextureTile *)v2->tiles;
    v11 = 16;
    do
    {
      v12 = 16;
      do
      {
        idMegaTextureTile::Upload(v10, v2->megaTexture);
        ++v10;
        --v12;
      }
      while ( v12 );
      --v11;
    }
    while ( v11 );
    v2->imageValid = 1;
    v2->dirty = 0;
  }
  return 1;
}
void __thiscall idMegaTextureLevel::AddDirtyTile(idMegaTextureLevel *this, idMegaTextureTile *tile)
{
  int *v2; // edi
  idLinkList<idMegaTextureTile> *v3; // esi
  idLinkList<idMegaTextureTile> *v4; // eax
  idMegaTextureTile *v5; // eax
  idLinkList<idMegaTextureTile> *v6; // ecx
  idLinkList<idMegaTextureTile> *v7; // eax
  idLinkList<idMegaTextureTile> *v8; // ecx
  idLinkList<idMegaTextureTile> *v9; // eax

  v2 = this->megaTexture->tileIndexMap;
  v3 = this->dirtyTiles.head;
  v4 = v3->next;
  if ( v4 && v4 != v3->head && (v5 = v4->owner) != 0 )
  {
    while ( v2[tile->globalY + tile->level->tileBase + tile->globalX * tile->level->tilesPerAxis] >= v2[v5->globalY + v5->level->tileBase + v5->globalX * v5->level->tilesPerAxis] )
    {
      v6 = v5->dirtyNode.next;
      if ( v6 )
      {
        if ( v6 != v5->dirtyNode.head )
        {
          v5 = v6->owner;
          if ( v5 )
            continue;
        }
      }
      goto LABEL_8;
    }
    v8 = &v5->dirtyNode;
    v9 = &tile->dirtyNode;
    tile->dirtyNode.prev->next = tile->dirtyNode.next;
    tile->dirtyNode.next->prev = tile->dirtyNode.prev;
    v9->prev = v9;
    v9->head = v9;
    v9->next = v8;
    v9->prev = v8->prev;
    v8->prev = &tile->dirtyNode;
    tile->dirtyNode.prev->next = &tile->dirtyNode;
    v9->head = v8->head;
  }
  else
  {
LABEL_8:
    v7 = &tile->dirtyNode;
    tile->dirtyNode.prev->next = tile->dirtyNode.next;
    tile->dirtyNode.next->prev = tile->dirtyNode.prev;
    v7->prev = v7;
    v7->head = v7;
    v7->next = v3;
    v7->prev = v3->prev;
    v3->prev = &tile->dirtyNode;
    tile->dirtyNode.prev->next = &tile->dirtyNode;
    v7->head = v3->head;
  }
  sdSignal::Set(&megaTextureTileLoader->signal);
}
void __thiscall idMegaTextureTile::idMegaTextureTile(idMegaTextureTile *this)
{
  idMegaTextureTile *v1; // eax
  idLinkList<idMegaTextureTile> *v2; // ecx

  v1 = this;
  v2 = &this->dirtyNode;
  v2->head = v2;
  v2->next = v2;
  v2->prev = v2;
  v2->owner = 0;
  v1->globalX = -99999;
  v1->globalY = -99999;
  v1->compressedTileData = 0;
  v1->tileData = 0;
  v1->dirty = 0;
  v1->loaded = 0;
  v1->dirtyNode.owner = v1;
  v1->childCompressedTileData[0] = 0;
  v1->childCompressedTileData[1] = 0;
  v1->childCompressedTileData[2] = 0;
  v1->childCompressedTileData[3] = 0;
}
void __thiscall idMegaTextureTile::~idMegaTextureTile(idMegaTextureTile *this)
{
  idMegaTextureTile *v1; // esi

  v1 = this;
  idMegaTextureTile::Purge(this);
  idLinkList<sdNetTask_DemonWare_FindSessionsQuery<bdFindRankedSessionsByPublicOpen,bdFindRankedSessionsByPublicOpenResult>::sessionNode_t>::Clear((idLinkList<idMD5Mesh> *)&v1->dirtyNode);
}
void __thiscall tileData_t::tileData_t(tileData_t *this)
{
  tileData_t *v1; // eax
  idLinkList<tileData_t> *v2; // ecx

  v1 = this;
  this->x = -1;
  this->y = -1;
  this->tileBase = -1;
  v2 = &this->node;
  v1->pic = 0;
  v2->owner = 0;
  v2->head = v2;
  v2->next = v2;
  v2->prev = v2;
  v1->node.owner = v1;
}
char __thiscall idMegaTexture::InitFromFile(idMegaTexture *this, const char *fileBase)
{
  idMegaTexture *v2; // esi

  v2 = this;
  idStr::operator=(&this->name, fileBase);
  if ( !((unsigned __int8 (*)(void))networkSystem->vfptr->IsDedicated)() )
  {
    if ( !idMegaTexture::OpenFile(v2) )
    {
      idMegaTexture::CloseFile(v2);
      return 0;
    }
    idMegaTexture::CloseFile(v2);
  }
  return 1;
}
void __thiscall idMegaTexture::Touch(idMegaTexture *this)
{
  if ( !this->purged )
    idMegaTexture::LoadDetailTexture(this);
}
char __thiscall idMegaTexture::UploadTiles(idMegaTexture *this, int time)
{
  idMegaTexture *v2; // esi
  int v3; // edi
  char v4; // bl
  int v5; // ebp
  idMegaTextureLevel *v6; // esi

  v2 = this;
  v3 = 0;
  v4 = 1;
  if ( this->numLevels > 0 )
  {
    v5 = 0;
    do
    {
      ++v3;
      v4 &= idMegaTextureLevel::UploadTiles(&v2->levels[v5], time);
      ++v5;
    }
    while ( v3 < v2->numLevels );
  }
  if ( idMegaTexture::r_megaUpscale.internalVar->integerValue )
  {
    v6 = v2->upscaleLevel;
    if ( v6 )
      v4 &= idMegaTextureLevel::UploadTiles(v6, time);
  }
  return v4;
}
tileData_t *__thiscall idMegaTextureLevel::FindCachedTile(idMegaTextureLevel *this, const int tileBase, const int globalX, const int globalY)
{
  idLinkList<tileData_t> *v4; // eax
  tileData_t *result; // eax
  idLinkList<tileData_t> *v6; // edx
  idLinkList<tileData_t> *v7; // ecx
  idLinkList<tileData_t> *v8; // edx

  v4 = this->availableTiles.next;
  if ( !v4 || v4 == this->availableTiles.head )
    return 0;
  result = v4->owner;
  if ( result )
  {
    while ( result->tileBase != tileBase || result->x != globalX || result->y != globalY )
    {
      v6 = result->node.next;
      if ( !v6 || v6 == result->node.head )
        return 0;
      result = v6->owner;
      if ( !result )
        return result;
    }
    v7 = this->activeTiles.head;
    v8 = &result->node;
    result->node.prev->next = result->node.next;
    result->node.next->prev = result->node.prev;
    v8->prev = v8;
    v8->head = v8;
    v8->next = v7;
    v8->prev = v7->prev;
    v7->prev = &result->node;
    result->node.prev->next = &result->node;
    v8->head = v7->head;
  }
  return result;
}
tileData_t *__thiscall idMegaTextureLevel::GetAvailableTile(idMegaTextureLevel *this)
{
  idMegaTextureLevel *v1; // edi
  idLinkList<tileData_t> *v2; // eax
  tileData_t *v3; // esi
  idLinkList<tileData_t> *v4; // ecx
  idLinkList<tileData_t> *v5; // eax

  v1 = this;
  v2 = this->availableTiles.next;
  if ( !v2 || v2 == this->availableTiles.head )
  {
    v3 = 0;
    goto LABEL_4;
  }
  v3 = v2->owner;
  if ( !v3 )
LABEL_4:
    (*(void (__cdecl **)(netadrtype_t, const char *))(*(_DWORD *)common.type + 88))(
      common.type,
      "idMegaTexture::GetFreeTile : no available cache tiles");
  v3->tileBase = -1;
  v3->y = -1;
  v3->x = -1;
  v4 = v1->activeTiles.head;
  v5 = &v3->node;
  v3->node.prev->next = v3->node.next;
  v3->node.next->prev = v3->node.prev;
  v5->prev = v5;
  v5->head = v5;
  v5->next = v4;
  v5->prev = v4->prev;
  v4->prev = &v3->node;
  v3->node.prev->next = &v3->node;
  v5->head = v4->head;
  return v3;
}
void __thiscall idMegaTextureLevel::ReleaseTile(idMegaTextureLevel *this, tileData_t *tileData)
{
  idLinkList<tileData_t> *v2; // ecx
  idLinkList<tileData_t> *v3; // eax
  idLinkList<tileData_t> *v4; // ecx
  idLinkList<tileData_t> *v5; // eax

  if ( tileData->x == -1 || tileData->y == -1 || tileData->tileBase == -1 )
  {
    v4 = this->availableTiles.head;
    v5 = &tileData->node;
    tileData->node.prev->next = tileData->node.next;
    v5->next->prev = tileData->node.prev;
    v5->next = v5;
    v5->head = v5;
    v5->prev = v4;
    v5->next = v4->next;
    v4->next = &tileData->node;
    tileData->node.next->prev = &tileData->node;
    v5->head = v4->head;
  }
  else
  {
    v2 = this->availableTiles.head;
    v3 = &tileData->node;
    tileData->node.prev->next = tileData->node.next;
    v3->next->prev = tileData->node.prev;
    v3->prev = v3;
    v3->head = v3;
    v3->next = v2;
    v3->prev = v2->prev;
    v2->prev = &tileData->node;
    tileData->node.prev->next = &tileData->node;
    v3->head = v2->head;
  }
}
void __thiscall tileData_t::~tileData_t(tileData_t *this)
{
  idLinkList<sdNetTask_DemonWare_FindSessionsQuery<bdFindRankedSessionsByPublicOpen,bdFindRankedSessionsByPublicOpenResult>::sessionNode_t>::Clear((idLinkList<idMD5Mesh> *)&this->node);
}
void __thiscall idMegaTextureTile::ReleaseTileData(idMegaTextureTile *this)
{
  idMegaTextureTile *v1; // esi
  tileData_t *v2; // eax
  idLinkList<idMegaTextureTile> *v3; // eax

  v1 = this;
  v2 = this->tileData;
  if ( v2 )
  {
    idMegaTextureLevel::ReleaseTile(this->level, v2);
    v1->tileData = 0;
  }
  v3 = &v1->dirtyNode;
  v1->dirtyNode.prev->next = v1->dirtyNode.next;
  v1->dirtyNode.next->prev = v1->dirtyNode.prev;
  v3->next = v3;
  v3->prev = v3;
  v3->head = v3;
}
bool __thiscall idMegaTextureTile::SetCachedTileData(idMegaTextureTile *this, idMegaTexture *megaTexture, const int tileBase, const int tilesPerAxis)
{
  idMegaTextureTile *v4; // edi
  bool result; // al
  int v6; // ecx
  int v7; // eax
  tileData_t *v8; // eax
  idMegaTextureLevel *v9; // esi
  idMegaTextureTile *v10; // ebx

  v4 = this;
  if ( !this->dirty )
    return 1;
  v6 = this->globalX;
  if ( v6 >= tilesPerAxis || v6 < 0 || (v7 = v4->globalY, v7 >= tilesPerAxis) || v7 < 0 )
  {
    v4->tileData = 0;
    result = 0;
  }
  else
  {
    v8 = idMegaTextureLevel::FindCachedTile(v4->level, tileBase, v6, v4->globalY);
    v4->tileData = v8;
    if ( v8 )
      return 1;
    if ( v4->level->isInterleaved )
    {
      v9 = &v4->level->megaTexture->levels[v4->level->levelNum + 1];
      v10 = (idMegaTextureTile *)((char *)v9->tiles + 64
                                                    * (((v4->globalY >> 1) & 0xF) + 16 * ((v4->globalX >> 1) & 0xF)));
      if ( !idMegaTextureTile::IsLoaded(v10) )
      {
        idMegaTextureTile::ReleaseTileData(v10);
        if ( v10->dirtyNode.head == &v10->dirtyNode )
          idMegaTextureLevel::AddDirtyTile(v9, v10);
      }
    }
    idMegaTextureLevel::AddDirtyTile(v4->level, v4);
    result = 0;
  }
  return result;
}
void __thiscall idMegaTexture::ForceUpdate(idMegaTexture *this)
{
  idMegaTexture *v1; // esi
  int v2; // edi

  v1 = this;
  v2 = this->lastShaderQuality;
  this->lastShaderQuality = r_shaderQuality.internalVar->integerValue;
  this->forcedUpdate = 1;
  while ( !idMegaTexture::UploadTiles(v1, 0) )
  {
    bdPlatformTiming::sleep(10);
    sdSignal::Set(&megaTextureTileLoader->signal);
    sdSignal::Set(&megaTextureTileDecompressor->signal);
  }
  v1->lastShaderQuality = v2;
  v1->forcedUpdate = 0;
}
void __thiscall idMegaTextureLevel::InitTileCache(idMegaTextureLevel *this)
{
  idMegaTextureLevel *v1; // esi
  imageCompressionFormat_t v2; // ebx
  signed int v3; // edi
  signed int v4; // edx
  signed int v5; // ecx
  signed int v6; // eax
  signed int v7; // ebp
  unsigned int v8; // ebp
  _DWORD *v9; // eax
  int v10; // edi
  int v11; // ebx
  int v12; // eax
  tileData_t *v13; // eax
  idLinkList<tileData_t> *v14; // ecx
  idLinkList<tileData_t> *v15; // edx
  idLinkList<tileData_t> *v16; // ebx
  idLinkList<tileData_t> *v17; // eax
  int i; // [esp+10h] [ebp-14h]
  signed int v19; // [esp+14h] [ebp-10h]
  signed int v20; // [esp+14h] [ebp-10h]
  int v21; // [esp+14h] [ebp-10h]

  v1 = this;
  v2 = this->megaTexture->imageCompressionFormat;
  v3 = 128;
  i = 128;
  v4 = 128;
  v5 = 128;
  v6 = 1;
  if ( v2 == 32856 )
  {
    v7 = 1;
  }
  else if ( (signed int)v2 <= 33775 || (signed int)v2 > 33779 )
  {
    v7 = v19;
  }
  else
  {
    v7 = 4;
  }
  while ( v5 > v7 || v4 > v7 )
  {
    ++v6;
    v5 >>= 1;
    v4 >>= 1;
  }
  v8 = 0;
  if ( v6 > 0 )
  {
    v20 = v6;
    do
    {
      if ( (signed int)v2 > 33777 )
      {
        if ( (signed int)v2 < 33778 || (signed int)v2 > 33779 )
        {
LABEL_20:
          v8 = -1;
          goto LABEL_21;
        }
        v8 += 16 * (i + 3) / 4 * ((v3 + 3) / 4);
      }
      else if ( (signed int)v2 >= 33776 )
      {
        v8 += 8 * (i + 3) / 4 * ((v3 + 3) / 4);
      }
      else
      {
        if ( v2 != 32856 )
          goto LABEL_20;
        v8 += 4 * i * v3;
      }
LABEL_21:
      i >>= 1;
      v3 >>= 1;
      --v20;
    }
    while ( v20 );
  }
  v1->tileCacheSize = 288;
  v9 = operator new(0x2404u);
  v10 = 0;
  if ( v9 )
  {
    v11 = (int)(v9 + 1);
    *v9 = 288;
    `eh vector constructor iterator'(
      v9 + 1,
      0x20u,
      288,
      (void (__thiscall *)(void *))tileData_t::tileData_t,
      (void (__thiscall *)(void *))tileData_t::~tileData_t);
  }
  else
  {
    v11 = 0;
  }
  v12 = v1->tileCacheSize;
  v1->usedMemory += 32 * v1->tileCacheSize;
  v1->tileCache = (tileData_t *)v11;
  v21 = 0;
  if ( v12 > 0 )
  {
    do
    {
      v1->tileCache[v10].pic = (char *)Mem_AllocAligned(v8, ALIGN_16);
      v13 = v1->tileCache;
      v14 = v1->availableTiles.head;
      v15 = v13[v10].node.prev;
      v16 = v13[v10].node.next;
      v17 = &v13[v10].node;
      v15->next = v16;
      v17->next->prev = v17->prev;
      v17->prev = v17;
      v17->head = v17;
      v17->next = v14;
      v17->prev = v14->prev;
      v14->prev = v17;
      v17->prev->next = v17;
      v17->head = v14->head;
      v1->usedMemory += v8;
      ++v10;
      ++v21;
    }
    while ( v21 < v1->tileCacheSize );
  }
}
void __thiscall idMegaTextureLevel::ShutdownTileCache(idMegaTextureLevel *this)
{
  idMegaTextureLevel *v1; // esi
  int v2; // edi
  int v3; // ebx
  tileData_t *v4; // eax
  tileData_t **v5; // edi

  v1 = this;
  idLinkList<sdNetTask_DemonWare_FindSessionsQuery<bdFindRankedSessionsByPublicOpen,bdFindRankedSessionsByPublicOpenResult>::sessionNode_t>::Clear((idLinkList<idMD5Mesh> *)&this->availableTiles);
  idLinkList<sdNetTask_DemonWare_FindSessionsQuery<bdFindRankedSessionsByPublicOpen,bdFindRankedSessionsByPublicOpenResult>::sessionNode_t>::Clear((idLinkList<idMD5Mesh> *)&v1->activeTiles);
  v2 = 0;
  if ( v1->tileCacheSize > 0 )
  {
    v3 = 0;
    do
    {
      Mem_FreeAligned(v1->tileCache[v3].pic);
      ++v2;
      ++v3;
    }
    while ( v2 < v1->tileCacheSize );
  }
  v4 = v1->tileCache;
  if ( v4 )
  {
    v5 = &v4[-1].node.owner;
    `eh vector destructor iterator'(
      v4,
      0x20u,
      (int)v4[-1].node.owner,
      (void (__thiscall *)(void *))tileData_t::~tileData_t);
    operator delete(v5);
  }
  v1->tileCacheSize = 0;
  v1->tileCache = 0;
}
char __thiscall idMegaTextureLevel::UpdateForCenter(idMegaTextureLevel *this, idVec2 *center, bool force)
{
  idMegaTextureLevel *v3; // ebx
  int v4; // eax
  signed int v5; // ecx
  float *v6; // esi
  signed int v7; // ecx
  float v8; // ST18_4
  float v9; // ST18_4
  int v10; // ecx
  int v11; // eax
  char v12; // bp
  int v13; // esi
  int v14; // ebp
  char v15; // bl
  int v16; // edi
  bool v17; // zf
  char v18; // bl
  int i; // esi
  int numDirtyTiles; // [esp+Ch] [ebp-424h]
  int v22; // [esp+10h] [ebp-420h]
  idMegaTextureLevel *v23; // [esp+14h] [ebp-41Ch]
  int globalTileCorner[2]; // [esp+18h] [ebp-418h]
  int v25; // [esp+20h] [ebp-410h]
  int localTileOffset[2]; // [esp+24h] [ebp-40Ch]
  idMegaTextureTile *dirtyTilesTable[256]; // [esp+2Ch] [ebp-404h]

  v3 = this;
  v23 = this;
  if ( this->tilesPerAxis > 16 )
  {
    v6 = this->newParms;
    v7 = 0;
    do
    {
      v8 = (*(float *)((char *)&globalTileCorner[v7 + 1] + (char *)center - (char *)&globalTileCorner[1]) * v3->parms[3]
          - 0.5)
         * 16.0;
      v9 = v8 + 0.5;
      globalTileCorner[v7 + 1] = (signed int)v9;
      localTileOffset[v7 + 1] = (signed int)v9 & 0xF;
      ++v7;
      ++v6;
      *(v6 - 1) = (double)-(signed int)v9 * 0.0625;
    }
    while ( v7 < 2 );
    v5 = (signed int)dirtyTilesTable[0];
    v4 = localTileOffset[1];
  }
  else
  {
    v4 = 0;
    this->newParms[0] = 0.25;
    globalTileCorner[1] = 0;
    this->newParms[1] = 0.25;
    v25 = 0;
    this->parms[3] = 0.25;
    v5 = 0;
  }
  v10 = -v5;
  v11 = -v4;
  v12 = v11;
  v22 = 0;
  localTileOffset[1] = v10;
  localTileOffset[0] = v11;
  v13 = (int)&v3->tiles[0][0].dirtyNode.prev;
  numDirtyTiles = 16;
  while ( 1 )
  {
    v14 = globalTileCorner[1] + (v12 & 0xF);
    v15 = v10;
    globalTileCorner[0] = 16;
    do
    {
      v16 = v25 + (v15 & 0xF);
      if ( *(_DWORD *)(v13 + 16) != v14 || *(_DWORD *)(v13 + 20) != v16 || force )
      {
        if ( (v14 & 0xF) != *(_DWORD *)(v13 + 8) || (v16 & 0xF) != *(_DWORD *)(v13 + 12) )
          (*(void (__cdecl **)(netadrtype_t, const char *))(*(_DWORD *)common.type + 88))(
            common.type,
            "idMegaTextureTile::Update : bad coordinate mod");
        dirtyTilesTable[v22 + 1] = (idMegaTextureTile *)(v13 - 12);
        *(_DWORD *)(v13 + 16) = v14;
        *(_DWORD *)(v13 + 20) = v16;
        *(_BYTE *)(v13 + 48) = 1;
        *(_BYTE *)(v13 + 49) = 0;
        ++v22;
        if ( *(_DWORD *)(v13 + 44) )
        {
          idMegaTextureLevel::ReleaseTile(*(idMegaTextureLevel **)(v13 - 12), *(tileData_t **)(v13 + 44));
          *(_DWORD *)(v13 + 44) = 0;
        }
        *(_DWORD *)(*(_DWORD *)v13 + 4) = *(_DWORD *)(v13 - 4);
        *(_DWORD *)(*(_DWORD *)(v13 - 4) + 8) = *(_DWORD *)v13;
        *(_DWORD *)(v13 - 4) = v13 - 8;
        *(_DWORD *)v13 = v13 - 8;
        *(_DWORD *)(v13 - 8) = v13 - 8;
        v23->dirty = 1;
      }
      v13 += 64;
      ++v15;
      --globalTileCorner[0];
    }
    while ( globalTileCorner[0] );
    v12 = LOBYTE(localTileOffset[0]) + 1;
    v17 = numDirtyTiles-- == 1;
    ++localTileOffset[0];
    if ( v17 )
      break;
    LOBYTE(v10) = localTileOffset[1];
  }
  v18 = 0;
  if ( v23->levelNum >= 0 )
  {
    for ( i = 0; i < v22; ++i )
    {
      if ( !idMegaTextureTile::SetCachedTileData(
              dirtyTilesTable[i + 1],
              v23->megaTexture,
              v23->tileBase,
              v23->tilesPerAxis) )
        v18 = 1;
    }
  }
  return v18;
}
void __thiscall idMegaTextureLevel::EmptyLevelImage(idMegaTextureLevel *this, idImage *image)
{
  idMegaTextureLevel *v2; // esi
  char *v3; // ebp
  _BYTE *v4; // eax
  signed int v5; // ecx
  int v6; // esi
  signed int v7; // edx
  signed int v8; // ecx
  int v9; // eax
  idImage *v10; // edi
  int v11; // edi
  bool *v12; // eax
  signed int v13; // edx
  signed int v14; // ecx
  idMegaTextureLevel *v15; // [esp+24h] [ebp-4h]

  v2 = this;
  v15 = this;
  v3 = (char *)Mem_Alloc((const unsigned int)&unk_1000000);
  v4 = v3 + 2;
  v5 = 0x400000;
  do
  {
    *(v4 - 2) = -1;
    *(v4 - 1) = 0;
    *v4 = 0;
    v4[1] = -1;
    v4 += 4;
    --v5;
  }
  while ( v5 );
  v6 = v2->megaTexture->imageCompressionFormat;
  v7 = 128;
  v8 = 128;
  v9 = 1;
  if ( v6 == 32856 )
  {
    v10 = (idImage *)1;
  }
  else if ( v6 <= 33775 || v6 > 33779 )
  {
    v10 = image;
  }
  else
  {
    v10 = (idImage *)4;
  }
  while ( v8 > (signed int)v10 || v7 > (signed int)v10 )
  {
    ++v9;
    v8 >>= 1;
    v7 >>= 1;
  }
  image->vfptr->GenerateImageEx(image, v3, 2048, 2048, TF_DEFAULT, 0, 0, TD_DEFAULT, v6, v9);
  Mem_Free(v3);
  v11 = image->internalFormat;
  if ( v11 != v6 )
    (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 88))(
      common.type,
      "idMegaTextureLevel::EmptyLevelImage : generated image has an incorrect internal format (0x%x expected 0x%x)",
      v11,
      v6);
  if ( v15->imageValid )
  {
    v15->dirty = 1;
    v12 = &v15->tiles[0][0].dirty;
    v13 = 16;
    do
    {
      v14 = 16;
      do
      {
        *v12 = 1;
        v12 += 64;
        --v14;
      }
      while ( v14 );
      --v13;
    }
    while ( v13 );
    idMegaTexture::ForceUpdate(v15->megaTexture);
  }
}
void __thiscall idMegaTexture::SetViewOrigin(idMegaTexture *this, idVec3 *viewOrigin)
{
  idMegaTexture *v2; // esi
  idVec3 *v3; // edi
  float v4; // ebx
  idVec2 *v5; // ebp
  double v6; // st4
  double v7; // st5
  double v8; // st6
  double v9; // st5
  double v10; // st4
  double v11; // st4
  double v12; // st6
  double v13; // st5
  double v14; // rtt
  double v15; // st5
  double v16; // rt0
  int v17; // edi
  double v18; // st6
  double v19; // st7
  float v20; // ST18_4
  signed int v21; // ebx
  signed int v22; // eax
  signed int v23; // ecx
  float *v24; // eax
  double v25; // st5
  int v26; // edi
  int v27; // ebp
  bool v28; // al
  idMegaTextureLevel *v29; // ecx
  int flooredCellY; // [esp+18h] [ebp-24h]
  int flooredCellYa; // [esp+18h] [ebp-24h]
  idVec2 cellOrigin; // [esp+1Ch] [ebp-20h]
  float v33; // [esp+24h] [ebp-18h]
  idVec2 texCenter; // [esp+28h] [ebp-14h]
  float weightsX[2]; // [esp+30h] [ebp-Ch]
  float v36; // [esp+38h] [ebp-4h]
  float cellXb; // [esp+40h] [ebp+4h]
  float cellXc; // [esp+40h] [ebp+4h]
  float cellXd; // [esp+40h] [ebp+4h]
  float cellXe; // [esp+40h] [ebp+4h]
  float cellXf; // [esp+40h] [ebp+4h]
  float cellXg; // [esp+40h] [ebp+4h]
  float cellXh; // [esp+40h] [ebp+4h]
  float cellXi; // [esp+40h] [ebp+4h]
  float cellXj; // [esp+40h] [ebp+4h]
  float cellXk; // [esp+40h] [ebp+4h]
  float cellXl; // [esp+40h] [ebp+4h]
  float cellXm; // [esp+40h] [ebp+4h]
  float cellXn; // [esp+40h] [ebp+4h]
  float cellX; // [esp+40h] [ebp+4h]
  char cellXa; // [esp+40h] [ebp+4h]

  v2 = this;
  if ( !this->currentWorld )
    return;
  if ( projectionMatrix->isSubview )
    return;
  v3 = viewOrigin;
  cellXb = viewOrigin->x - this->currentViewOrigin.x;
  cellXc = fabs(cellXb);
  if ( cellXc <= 4.0 )
  {
    cellXd = v3->y - this->currentViewOrigin.y;
    cellXe = fabs(cellXd);
    if ( cellXe <= 4.0 )
    {
      cellXf = v3->z - this->currentViewOrigin.z;
      cellXg = fabs(cellXf);
      if ( cellXg <= 4.0 && r_shaderQuality.internalVar->integerValue == this->lastShaderQuality )
        return;
    }
  }
  if ( idMegaTexture::r_skipMegaTexture.internalVar->integerValue )
    return;
  LODWORD(v4) = (char *)this + 172;
  LODWORD(texCenter.x) = (char *)this + 172;
  sdLock::Acquire(&this->lock, 1);
  v2->currentViewOrigin.x = v3->x;
  v2->currentViewOrigin.y = v3->y;
  v2->currentViewOrigin.z = v3->z;
  v5 = v2->stGrid;
  weightsX[0] = 0.0;
  texCenter.y = 0.0;
  if ( v5 )
  {
    cellOrigin.y = v3->x - v2->stGridBounds.bounds[0].x;
    v33 = v3->y - v2->stGridBounds.bounds[0].y;
    cellXh = v2->stGridBounds.bounds[1].x - v2->stGridBounds.bounds[0].x;
    if ( cellXh >= (double)cellOrigin.y )
    {
      v9 = cellOrigin.y;
      v8 = 0.0;
      if ( cellOrigin.y >= 0.0 )
      {
LABEL_14:
        v10 = v33;
        cellXk = v2->stGridBounds.bounds[1].y - v2->stGridBounds.bounds[0].y;
        if ( cellXk >= (double)v33 )
        {
          if ( v10 >= 0.0 )
          {
            v16 = v9;
            v13 = v33;
            v12 = v16;
          }
          else
          {
            v14 = v9;
            v15 = v8 - v10;
            v12 = v14;
            v33 = v15;
            v13 = v33;
          }
        }
        else
        {
          cellXl = v2->stGridBounds.bounds[1].y - v2->stGridBounds.bounds[0].y;
          v11 = cellXl;
          cellXm = v2->stGridBounds.bounds[1].y - v2->stGridBounds.bounds[0].y;
          v12 = v9;
          v33 = v11 - (v33 - cellXm);
          v13 = v33;
        }
        v17 = v2->stGridWidth;
        *(float *)&flooredCellY = (double)(v17 - 1);
        cellXn = v2->stGridBounds.bounds[1].x - v2->stGridBounds.bounds[0].x;
        cellX = v12 / cellXn * *(float *)&flooredCellY;
        if ( cellX >= 0.0 )
        {
          if ( *(float *)&flooredCellY < (double)cellX )
            cellX = (double)(v17 - 1);
          v18 = 0.0;
          v19 = v13;
        }
        else
        {
          v18 = 0.0;
          v19 = v13;
          cellX = 0.0;
        }
        cellOrigin.x = (double)(v2->stGridHeight - 1);
        v20 = v2->stGridBounds.bounds[1].y - v2->stGridBounds.bounds[0].y;
        *(float *)&flooredCellYa = v19 / v20 * cellOrigin.x;
        if ( *(float *)&flooredCellYa >= v18 )
        {
          if ( cellOrigin.x < (double)*(float *)&flooredCellYa )
            flooredCellYa = SLODWORD(cellOrigin.x);
        }
        else
        {
          *(float *)&flooredCellYa = v18;
        }
        cellOrigin.x = floor(cellX);
        v21 = (signed int)cellOrigin.x;
        LODWORD(cellOrigin.y) = (signed int)cellOrigin.x;
        cellOrigin.x = floor(*(float *)&flooredCellYa);
        v22 = (signed int)cellOrigin.x;
        LODWORD(cellOrigin.x) = (signed int)cellOrigin.x;
        v36 = cellX - (double)SLODWORD(cellOrigin.y);
        v23 = 0;
        v24 = &v5[v21 + v22 * v17].x;
        weightsX[1] = 1.0 - v36;
        v33 = *(float *)&flooredCellYa - (double)SLODWORD(cellOrigin.x);
        cellOrigin.y = 1.0 - v33;
        do
        {
          ++v23;
          texCenter.y = *v24 * weightsX[1] + texCenter.y;
          weightsX[0] = v24[1] * *(&cellOrigin.x + v23) + weightsX[0];
          texCenter.y = v24[2] * v36 + texCenter.y;
          v25 = v24[3];
          v24 += 2 * v17;
          weightsX[0] = v25 * *(&cellOrigin.x + v23) + weightsX[0];
        }
        while ( v23 < 2 );
        v4 = texCenter.x;
        texCenter.y = texCenter.y * 0.5;
        weightsX[0] = 0.5 * weightsX[0];
        goto LABEL_31;
      }
      v7 = 0.0 - v9;
    }
    else
    {
      cellXi = v2->stGridBounds.bounds[1].x - v2->stGridBounds.bounds[0].x;
      v6 = cellXi;
      cellXj = v2->stGridBounds.bounds[1].x - v2->stGridBounds.bounds[0].x;
      v7 = v6 - (cellOrigin.y - cellXj);
      v8 = 0.0;
    }
    cellOrigin.y = v7;
    v9 = cellOrigin.y;
    goto LABEL_14;
  }
LABEL_31:
  v26 = v2->numLevels - 1;
  cellXa = 0;
  if ( v26 >= 0 )
  {
    v27 = v26;
    do
    {
      v28 = r_shaderQuality.internalVar->integerValue != v2->lastShaderQuality && v26 != v2->numLevels - 1;
      if ( idMegaTextureLevel::UpdateForCenter(&v2->levels[v27], (idVec2 *)((char *)&texCenter + 4), v28) )
        cellXa = 1;
      --v26;
      --v27;
    }
    while ( v26 >= 0 );
  }
  v29 = v2->upscaleLevel;
  if ( v29 )
    idMegaTextureLevel::UpdateForCenter(
      v29,
      (idVec2 *)((char *)&texCenter + 4),
      r_shaderQuality.internalVar->integerValue != v2->lastShaderQuality);
  if ( cellXa )
  {
    sdSignal::Set(&megaTextureTileLoader->signal);
    sdSignal::Set(&megaTextureTileDecompressor->signal);
  }
  v2->lastShaderQuality = r_shaderQuality.internalVar->integerValue;
  sdLock::Release((sdLock *)LODWORD(v4));
}
void __thiscall idMegaTextureLevel::idMegaTextureLevel(idMegaTextureLevel *this)
{
  idMegaTextureLevel *v1; // esi
  idLinkList<tileData_t> *v2; // eax
  idLinkList<tileData_t> *v3; // eax
  idLinkList<idMegaTextureTile> *v4; // eax

  v1 = this;
  this->emptyLevelImageFunctor.vfptr = (idImageGeneratorFunctorBaseVtbl *)&idImageGeneratorFunctor<idMegaTextureLevel>::`vftable';
  this->megaTexture = 0;
  this->levelNum = -1;
  this->usedMemory = 16540;
  this->image = 0;
  this->imageValid = 0;
  this->tileBase = 0;
  this->tilesPerAxis = 0;
  this->megaCompressionFormat = 0;
  this->isInterleaved = 0;
  this->fadeTime = 0;
  `eh vector constructor iterator'(
    this->tiles,
    0x40u,
    256,
    (void (__thiscall *)(void *))idMegaTextureTile::idMegaTextureTile,
    (void (__thiscall *)(void *))idMegaTextureTile::~idMegaTextureTile);
  v1->alwaysCached = 0;
  v1->compressedData = 0;
  v1->compressedTiles = 0;
  v1->compressedTilesPerAxis = 0;
  v1->tileCache = 0;
  v1->tileCacheSize = 0;
  v1->availableTiles.owner = 0;
  v2 = &v1->availableTiles;
  v2->head = v2;
  v2->next = v2;
  v2->prev = v2;
  v1->activeTiles.owner = 0;
  v3 = &v1->activeTiles;
  v3->head = v3;
  v3->next = v3;
  v3->prev = v3;
  v1->dirty = 0;
  v4 = &v1->dirtyTiles;
  v4->owner = 0;
  v4->head = v4;
  v4->next = v4;
  v4->prev = v4;
  v1->emptyLevelImageFunctor.generatorClass = v1;
  v1->emptyLevelImageFunctor.imageGenerator = idMegaTextureLevel::EmptyLevelImage;
}
void __thiscall idMegaTexture::UpdateForViewOrigin(idMegaTexture *this, idVec3 *viewOrigin, int time)
{
  idMegaTexture *v3; // esi
  int i; // edi
  idMegaTextureLevel *v5; // eax
  idMegaTextureLevel *v6; // ecx
  sdDeclRenderBinding *v7; // eax
  int v8; // eax
  signed int v9; // edx
  sdDeclRenderBinding *v10; // eax
  unsigned int v11; // eax
  idImage *v12; // ecx
  idImage *v13; // edi
  idImage *v14; // ecx
  idImage *v15; // eax
  sdDeclRenderBinding *v16; // ecx
  float timea; // [esp+14h] [ebp+8h]
  float timeb; // [esp+14h] [ebp+8h]

  v3 = this;
  if ( this->lastUsedFrame < backEnd.frameCount )
  {
    idMegaTexture::UploadTiles(this, time);
    idMegaTexture::SetViewOrigin(v3, viewOrigin);
    v3->lastUsedFrame = backEnd.frameCount;
  }
  for ( i = 0; i < v3->numLevels; ++i )
    idMegaTexture::UpdateLevelForViewOrigin(&v3->levels[v3->numLevels - i - 1], i, time);
  if ( idMegaTexture::r_megaUpscale.internalVar->integerValue )
  {
    v5 = v3->upscaleLevel;
    if ( v5 )
      idMegaTexture::UpdateLevelForViewOrigin(v5, v3->numLevels, time);
  }
  else
  {
    v6 = v3->upscaleLevel;
    if ( v6 )
    {
      timea = v6->parms[3];
      v7 = rbinds->megaMaskParams[v3->numLevels];
      v7->data.vector[0] = -1.0;
      v7->data.vector[1] = 0.0;
      v7->data.vector[2] = 0.0;
      v7->data.vector[3] = timea;
      v8 = v3->numLevels;
      v9 = 1 << (v8 + 1);
      v10 = rbinds->megaTextureParams[v8];
      timeb = (double)v9 * 0.5;
      v10->data.vector[3] = timeb;
      v10->data.vector[2] = timeb;
      v10->data.vector[1] = timeb;
      v10->data.vector[0] = timeb;
    }
    v11 = v3->numLevels - 1;
    if ( v11 <= 3 )
      rbinds->megaTextureOpacity15->data.vector[v11] = 0.0;
    rbinds->megaTextureLevel[v3->numLevels]->data.attrib = (int)globalImages->blackImage;
  }
  v12 = v3->detailTexture;
  if ( v12->defaulted || !((unsigned __int8 (*)(void))v12->vfptr->IsLoaded)() )
    v13 = globalImages->grayImage;
  else
    v13 = v3->detailTexture;
  v14 = v3->detailTextureMask;
  if ( v14->defaulted || !((unsigned __int8 (*)(void))v14->vfptr->IsLoaded)() )
    v15 = globalImages->defaultDetailMaskImage;
  else
    v15 = v3->detailTextureMask;
  rbinds->megaDetailTexture->data.attrib = (int)v13;
  rbinds->megaDetailTextureMask->data.attrib = (int)v15;
  v16 = rbinds->megaDetailTextureParams;
  v16->data.vector[0] = (double)((v3->tilesPerAxis << 7) / v13->uploadWidth)
                      * idMegaTexture::r_detailRatio.internalVar->floatValue;
  v16->data.vector[1] = 1.0;
  v16->data.vector[2] = 1.0;
  v16->data.vector[3] = 1.0;
}
void __thiscall idMegaTextureLevel::~idMegaTextureLevel(idMegaTextureLevel *this)
{
  idMegaTextureLevel *v1; // esi
  char *v2; // ST0C_4
  char **v3; // ST08_4

  v1 = this;
  v2 = this->compressedData;
  this->usedMemory = 16540;
  this->image = 0;
  this->imageValid = 0;
  this->dirty = 0;
  Mem_Free(v2);
  v3 = v1->compressedTiles;
  v1->compressedData = 0;
  Mem_Free(v3);
  v1->compressedTiles = 0;
  idMegaTextureLevel::ShutdownTileCache(v1);
  idLinkList<sdNetTask_DemonWare_FindSessionsQuery<bdFindRankedSessionsByPublicOpen,bdFindRankedSessionsByPublicOpenResult>::sessionNode_t>::Clear((idLinkList<idMD5Mesh> *)&v1->dirtyTiles);
  idLinkList<sdNetTask_DemonWare_FindSessionsQuery<bdFindRankedSessionsByPublicOpen,bdFindRankedSessionsByPublicOpenResult>::sessionNode_t>::Clear((idLinkList<idMD5Mesh> *)&v1->activeTiles);
  idLinkList<sdNetTask_DemonWare_FindSessionsQuery<bdFindRankedSessionsByPublicOpen,bdFindRankedSessionsByPublicOpenResult>::sessionNode_t>::Clear((idLinkList<idMD5Mesh> *)&v1->availableTiles);
  `eh vector destructor iterator'(
    v1->tiles,
    0x40u,
    256,
    (void (__thiscall *)(void *))idMegaTextureTile::~idMegaTextureTile);
  v1->emptyLevelImageFunctor.vfptr = (idImageGeneratorFunctorBaseVtbl *)&idImageGeneratorFunctorBase::`vftable';
}
void __thiscall idMegaTextureLevel::Init(idMegaTextureLevel *this, idMegaTexture *megaTexture, const int levelNum, const int tileBase, const int tilesPerAxis, const bool activateImage, megaCompressionFormat_t megaCompressionFormat, const int maxCompressedTileSize)
{
  int v8; // eax
  idMegaTextureLevel *v9; // edi
  char *v10; // ST0C_4
  char **v11; // ST08_4
  idMegaTexture *v12; // ebx
  int v13; // esi
  const char *v14; // ebp
  unsigned int v15; // eax
  _BYTE *v16; // edx
  const char *v17; // ecx
  char v18; // al
  idImage *v19; // eax
  int v20; // eax
  char **v21; // eax
  unsigned int v22; // ebp
  int *v23; // eax
  int v24; // edx
  int v25; // ecx
  char *v26; // eax
  char *v27; // ebp
  int v28; // esi
  int v29; // eax
  bool v30; // zf
  int v31; // esi
  void *v32; // ST0C_4
  int v33; // ebx
  void **v34; // ebp
  int v35; // ebp
  bool v36; // sf
  unsigned __int8 v37; // of
  idStr imageName; // [esp+4h] [ebp-30h]
  int v39; // [esp+28h] [ebp-Ch]
  int (__fastcall *v40)(int, int); // [esp+2Ch] [ebp-8h]
  int v41; // [esp+30h] [ebp-4h]
  char *megaTexturea; // [esp+38h] [ebp+4h]
  idMegaTexture *megaTextureb; // [esp+38h] [ebp+4h]
  int x; // [esp+3Ch] [ebp+8h]
  int xa; // [esp+3Ch] [ebp+8h]
  int xb; // [esp+3Ch] [ebp+8h]
  int tileBasea; // [esp+40h] [ebp+Ch]
  int tileBaseb; // [esp+40h] [ebp+Ch]
  int tilesPerAxisa; // [esp+44h] [ebp+10h]
  int y; // [esp+48h] [ebp+14h]
  int ya; // [esp+48h] [ebp+14h]
  int yb; // [esp+48h] [ebp+14h]
  int maxCompressedTileSizea; // [esp+50h] [ebp+1Ch]

  v41 = -1;
  v40 = _ehhandler__StripPath_idStr__QAEAAV1_XZ;
  v39 = v8;
  v9 = this;
  v10 = this->compressedData;
  this->usedMemory = 16540;
  this->image = 0;
  this->imageValid = 0;
  this->dirty = 0;
  Mem_Free(v10);
  v11 = v9->compressedTiles;
  v9->compressedData = 0;
  Mem_Free(v11);
  v9->compressedTiles = 0;
  idMegaTextureLevel::ShutdownTileCache(v9);
  v12 = megaTexture;
  v13 = tilesPerAxis;
  v9->tileBase = tileBase;
  v9->megaTexture = megaTexture;
  v9->levelNum = levelNum;
  v9->tilesPerAxis = tilesPerAxis;
  v9->maxCompressedTileSize = maxCompressedTileSize;
  v9->megaCompressionFormat = megaCompressionFormat;
  v9->isInterleaved = !levelNum && megaTexture->numLevels > 4;
  v14 = va("_megaLevel_%d", levelNum);
  imageName.data = 0;
  *(_DWORD *)imageName.baseBuffer = -20;
  imageName.alloced = (int)&imageName.baseBuffer[4];
  imageName.baseBuffer[4] = 0;
  if ( v14 )
  {
    v15 = strlen(v14);
    megaTexturea = (char *)v15;
    if ( (signed int)(v15 + 1) > 20 )
      idStr::ReAllocate((idStr *)((char *)&imageName + 4), v15 + 1, 1);
    v16 = (_BYTE *)imageName.alloced;
    v17 = v14;
    do
    {
      v18 = *v17;
      *v16++ = *v17++;
    }
    while ( v18 );
    imageName.data = megaTexturea;
  }
  v41 = 0;
  v19 = idImageManager::GetImage(globalImages, (const char *)imageName.alloced);
  v9->image = v19;
  if ( activateImage )
  {
    if ( v19 )
    {
      v19->generatorFunction = 0;
      v20 = (*(int (__stdcall **)(int, idMegaTextureLevel *))globalImages->vfptr->gap4)(imageName.alloced, v9);
      v9->image = (idImage *)v20;
      (*(void (__thiscall **)(int, _DWORD, signed int))(*(_DWORD *)v20 + 44))(v20, 0, 1);
    }
    else
    {
      v9->image = (idImage *)(*(int (__stdcall **)(int, idMegaTextureLevel *))globalImages->vfptr->gap4)(
                               imageName.alloced,
                               v9);
    }
  }
  if ( !v9->image )
    (*(void (__cdecl **)(netadrtype_t, const char *))(*(_DWORD *)common.type + 88))(
      common.type,
      "idMegaTextureLevel::Init : NULL level image");
  v9->parms[0] = -1.0;
  v9->parms[1] = 0.0;
  v9->parms[2] = 0.0;
  v9->parms[3] = (double)tilesPerAxis * 0.0625;
  v21 = (char **)Mem_Alloc(4 * tilesPerAxis * tilesPerAxis);
  v9->compressedTiles = v21;
  memset(v21, 0, 4 * tilesPerAxis * tilesPerAxis);
  v9->usedMemory += 4 * tilesPerAxis * tilesPerAxis;
  if ( levelNum >= 2 )
  {
    v22 = 0;
    if ( tilesPerAxis > 0 )
    {
      y = (int)&v12->tileIndexedDataSizes[tileBase];
      x = tilesPerAxis;
      do
      {
        v23 = (int *)y;
        v24 = tilesPerAxis;
        do
        {
          v25 = *v23;
          ++v23;
          --v24;
          v22 += v25 + 3;
        }
        while ( v24 );
        y += 4 * tilesPerAxis;
        --x;
      }
      while ( x );
    }
    v26 = (char *)Mem_Alloc(v22);
    v9->usedMemory += v22;
    v9->compressedData = v26;
    v27 = v26;
    if ( tilesPerAxis > 0 )
    {
      xa = 0;
      ya = tileBase;
      maxCompressedTileSizea = tilesPerAxis;
      do
      {
        v28 = ya;
        tileBasea = xa;
        megaTextureb = (idMegaTexture *)tilesPerAxis;
        do
        {
          idMegaTexture::SeekToTile(v12, v28);
          (*(void (__stdcall **)(char *, int))&v12->file->vfptr->gap4[8])(v27, v12->tileIndexedDataSizes[v28] + 3);
          *(char **)((char *)v9->compressedTiles + tileBasea) = v27;
          v29 = v12->tileIndexedDataSizes[v28++];
          v30 = megaTextureb == (idMegaTexture *)1;
          megaTextureb = (idMegaTexture *)((char *)megaTextureb - 1);
          v27 += v29 + 3;
          tileBasea += 4;
        }
        while ( !v30 );
        ya += tilesPerAxis;
        xa += 4 * tilesPerAxis;
        --maxCompressedTileSizea;
      }
      while ( maxCompressedTileSizea );
      v13 = tilesPerAxis;
    }
    v9->alwaysCached = 1;
    goto LABEL_31;
  }
  if ( !v9->isInterleaved )
LABEL_31:
    v9->compressedTilesPerAxis = v13;
  idMegaTextureLevel::InitTileCache(v9);
  yb = 0;
  tilesPerAxisa = (int)&v9->tiles[0][0].globalY;
  do
  {
    v31 = tilesPerAxisa;
    xb = 0;
    do
    {
      v32 = *(void **)(v31 + 4);
      v33 = 64;
      *(_DWORD *)(v31 - 4) = -99999;
      *(_DWORD *)v31 = -99999;
      *(_DWORD *)(v31 + 24) = 0;
      *(_BYTE *)(v31 + 28) = 0;
      *(_BYTE *)(v31 + 29) = 0;
      Mem_FreeAligned(v32);
      *(_DWORD *)(v31 + 4) = 0;
      v34 = (void **)(v31 + 8);
      tileBaseb = 4;
      do
      {
        Mem_FreeAligned(*v34);
        *v34 = 0;
        ++v34;
        --tileBaseb;
      }
      while ( tileBaseb );
      v35 = xb;
      *(_DWORD *)(v31 - 32) = v9;
      *(_DWORD *)(v31 - 12) = xb;
      *(_DWORD *)(v31 - 8) = yb;
      if ( !v9->alwaysCached && !v9->isInterleaved )
      {
        *(_DWORD *)(v31 + 4) = Mem_AllocAligned(v9->maxCompressedTileSize + 3, ALIGN_16);
        v33 = v9->maxCompressedTileSize + 67;
      }
      v9->usedMemory += v33;
      v31 += 1024;
      ++xb;
    }
    while ( v35 + 1 < 16 );
    tilesPerAxisa += 64;
    v37 = __OFSUB__(yb + 1, 16);
    v36 = yb++ - 15 < 0;
  }
  while ( v36 ^ v37 );
  v41 = -1;
  idStr::FreeData((idStr *)((char *)&imageName + 4));
}
void __thiscall idMegaTexture::OnUseMegaTextureCompressionChange(idMegaTexture *this)
{
  idMegaTexture *v1; // esi
  char *v2; // eax
  int v3; // ebx
  bool v4; // zf
  int v5; // ebp
  int v6; // edi
  int v7; // eax
  int v8; // ebp
  int v9; // ecx
  idMegaTextureLevel *v10; // ecx
  bool v11; // sf
  int v12; // ebx
  idMegaTextureTile *v13; // edi
  signed int v14; // ebp
  unsigned __int8 v15; // of
  sdDeclRenderBinding *v16; // eax
  sdDeclRenderBinding *v17; // eax
  sdDeclRenderBinding *v18; // eax
  sdDeclRenderBinding *v19; // eax
  sdDeclRenderBinding *v20; // eax
  sdDeclRenderBinding *v21; // eax
  int v22; // edx
  idMegaTextureLevel *v23; // eax
  int j; // [esp+4h] [ebp-10h]
  int v25; // [esp+4h] [ebp-10h]
  int i; // [esp+8h] [ebp-Ch]
  idVec2 center; // [esp+Ch] [ebp-8h]

  v1 = this;
  if ( !this->purged )
  {
    if ( megaTextureTileLoader->activeMegaTexture == this )
      idMegaTexture::ForceUpdate(this);
    v2 = v1->nullTileData;
    v3 = 0;
    if ( v2 )
    {
      Mem_FreeAligned(v2);
      v1->nullTileData = 0;
    }
    if ( v1->gridTileData )
    {
      Mem_FreeAligned(v1->gridTileData);
      v1->gridTileData = 0;
    }
    if ( v1->tileRecompressionScratch )
    {
      Mem_FreeAligned(v1->tileRecompressionScratch);
      v1->tileRecompressionScratch = 0;
    }
    v4 = idMegaTexture::r_useMegaTextureImageCompression.internalVar->integerValue == 0;
    v1->currentViewOrigin.z = 262144.0;
    v1->useImageCompression = !v4;
    idMegaTexture::AllocRecompressionScratch(v1);
    idMegaTexture::GenerateNullTileData(v1);
    idMegaTexture::GenerateGridTileData(v1);
    v5 = v1->numLevels;
    v6 = v1->tilesPerAxis;
    if ( v5 > 0 )
    {
      v7 = v1->numLevels;
      do
      {
        v3 += v6 * v6;
        v6 >>= 1;
        --v7;
      }
      while ( v7 );
    }
    v8 = v5 - 1;
    if ( v8 >= 0 )
    {
      v9 = 16540 * v8;
      for ( j = 16540 * v8; ; v9 = j )
      {
        v6 *= 2;
        v3 -= v6 * v6;
        idMegaTextureLevel::Init(
          (idMegaTextureLevel *)((char *)v1->levels + v9),
          v1,
          v8,
          v3,
          v6,
          megaTextureTileLoader->activeMegaTexture == v1,
          *(megaCompressionFormat_t *)((char *)&v1->levels->megaCompressionFormat + v9),
          *(int *)((char *)&v1->levels->maxCompressedTileSize + v9));
        j -= 16540;
        if ( --v8 < 0 )
          break;
      }
    }
    v10 = v1->upscaleLevel;
    if ( v10 )
      idMegaTextureLevel::Init(
        v10,
        v1,
        -1,
        0,
        2 * v6,
        megaTextureTileLoader->activeMegaTexture == v1,
        v10->megaCompressionFormat,
        v10->maxCompressedTileSize);
    v4 = v1->numLevels == 0;
    v11 = v1->numLevels < 0;
    center.x = 0.0;
    if ( !v11 && !v4 )
    {
      v25 = 0;
      do
      {
        i = 16;
        v12 = (int)v1->levels[v25].tiles;
        do
        {
          v13 = (idMegaTextureTile *)v12;
          v14 = 16;
          do
          {
            idMegaTextureTile::PostInit(v13);
            v13 += 16;
            --v14;
          }
          while ( v14 );
          v12 += 64;
          --i;
        }
        while ( i );
        ++v25;
        v15 = __OFSUB__(LODWORD(center.x) + 1, v1->numLevels);
        v11 = LODWORD(center.x)++ + 1 - v1->numLevels < 0;
      }
      while ( v11 ^ v15 );
    }
    if ( megaTextureTileLoader->activeMegaTexture == v1 )
    {
      v16 = rbinds->megaMaskParams[0];
      v16->data.vector[0] = -2.0;
      v16->data.vector[1] = -2.0;
      v16->data.vector[2] = 0.0;
      v16->data.vector[3] = 1.0;
      rbinds->megaTextureLevel[0]->data.attrib = (int)globalImages->whiteImage;
      v17 = rbinds->megaMaskParams[1];
      v17->data.vector[0] = -2.0;
      v17->data.vector[1] = -2.0;
      v17->data.vector[2] = 0.0;
      v17->data.vector[3] = 1.0;
      rbinds->megaTextureLevel[1]->data.attrib = (int)globalImages->whiteImage;
      v18 = rbinds->megaMaskParams[2];
      v18->data.vector[0] = -2.0;
      v18->data.vector[1] = -2.0;
      v18->data.vector[2] = 0.0;
      v18->data.vector[3] = 1.0;
      rbinds->megaTextureLevel[2]->data.attrib = (int)globalImages->whiteImage;
      v19 = rbinds->megaMaskParams[3];
      v19->data.vector[0] = -2.0;
      v19->data.vector[1] = -2.0;
      v19->data.vector[2] = 0.0;
      v19->data.vector[3] = 1.0;
      rbinds->megaTextureLevel[3]->data.attrib = (int)globalImages->whiteImage;
      v20 = rbinds->megaMaskParams[4];
      v20->data.vector[0] = -2.0;
      v20->data.vector[1] = -2.0;
      v20->data.vector[2] = 0.0;
      v20->data.vector[3] = 1.0;
      rbinds->megaTextureLevel[4]->data.attrib = (int)globalImages->whiteImage;
      v21 = rbinds->megaMaskParams[5];
      v21->data.vector[0] = -2.0;
      v21->data.vector[1] = -2.0;
      v21->data.vector[2] = 0.0;
      v21->data.vector[3] = 1.0;
      rbinds->megaTextureLevel[5]->data.attrib = (int)globalImages->whiteImage;
      sdLock::Acquire(&v1->lock, 1);
      v22 = v1->numLevels;
      center.x = 0.5;
      v23 = v1->levels;
      center.y = 0.5;
      idMegaTextureLevel::UpdateForCenter(&v23[v22 - 1], &center, 0);
      sdLock::Release(&v1->lock);
      idMegaTexture::ForceUpdate(v1);
    }
  }
}
void __thiscall idMegaTexture::Purge(idMegaTexture *this)
{
  idMegaTexture *v1; // esi
  idImage *v2; // eax
  int *v3; // eax
  idMegaTextureLevel *v4; // eax
  idMegaTextureTile **v5; // edi
  idMegaTextureLevel *v6; // edi

  v1 = this;
  if ( !this->purged )
  {
    if ( megaTextureTileLoader->activeMegaTexture == this )
      idMegaTextureTileLoader::SetActiveMegaTexture(megaTextureTileLoader, 0);
    if ( megaTextureTileDecompressor->activeMegaTexture == v1 )
      idMegaTextureTileDecompressor::SetActiveMegaTexture(megaTextureTileDecompressor, 0);
    idMegaTexture::CloseFile(v1);
    v1->detailTexture = globalImages->grayImage;
    v2 = globalImages->defaultDetailMaskImage;
    v1->currentViewOrigin.z = 262144.0;
    v1->detailTextureMask = v2;
    v3 = v1->tileIndexMap;
    v1->lastUsedFrame = 0;
    v1->currentWorld = 0;
    if ( v3 )
    {
      Mem_Free(v3);
      v1->tileIndexMap = 0;
    }
    if ( v1->tileIndexedDataSizes )
    {
      Mem_Free(v1->tileIndexedDataSizes);
      v1->tileIndexedDataSizes = 0;
    }
    if ( v1->nullTileData )
    {
      Mem_FreeAligned(v1->nullTileData);
      v1->nullTileData = 0;
    }
    if ( v1->gridTileData )
    {
      Mem_FreeAligned(v1->gridTileData);
      v1->gridTileData = 0;
    }
    if ( v1->tileRecompressionScratch )
    {
      Mem_FreeAligned(v1->tileRecompressionScratch);
      v1->tileRecompressionScratch = 0;
    }
    v4 = v1->levels;
    if ( v4 )
    {
      v5 = &v4[-1].dirtyTiles.owner;
      `eh vector destructor iterator'(
        v4,
        0x409Cu,
        (int)v4[-1].dirtyTiles.owner,
        (void (__thiscall *)(void *))idMegaTextureLevel::~idMegaTextureLevel);
      operator delete(v5);
    }
    v6 = v1->upscaleLevel;
    if ( v6 )
    {
      idMegaTextureLevel::~idMegaTextureLevel(v1->upscaleLevel);
      operator delete(v6);
    }
    v1->upscaleLevel = 0;
    v1->levels = 0;
    v1->numLevels = 0;
    v1->purged = 1;
  }
}
void __thiscall idMegaTexture::Load(idMegaTexture *this)
{
  idMegaTexture *v1; // esi
  int v2; // ecx
  int v3; // eax
  void *v4; // esp
  int (__thiscall *v5)(idFile *, int *); // eax
  int *v6; // ebx
  bool v7; // zf
  int j; // edi
  int v9; // ebx
  void *v10; // esp
  int v11; // eax
  int v12; // ecx
  int v13; // edi
  int *v14; // ebx
  int v15; // ebx
  int v16; // edi
  int v17; // edi
  int v18; // ecx
  int *v19; // eax
  int v20; // ebx
  idMegaTextureLevel *v21; // eax
  idMegaTextureLevel *v22; // eax
  int v23; // ebx
  int v24; // edi
  int v25; // eax
  int v26; // ecx
  int v27; // ebx
  int v28; // ST18_4
  megaCompressionFormat_t v29; // ST14_4
  idMegaTextureLevel *v30; // ecx
  idMegaTextureLevel *v31; // ecx
  bool v32; // sf
  int v33; // eax
  int *v34; // edi
  signed int v35; // ebx
  unsigned __int8 v36; // of
  sdDeclRenderBinding *v37; // eax
  sdDeclRenderBinding *v38; // eax
  sdDeclRenderBinding *v39; // eax
  sdDeclRenderBinding *v40; // eax
  sdDeclRenderBinding *v41; // eax
  sdDeclRenderBinding *v42; // eax
  int v43; // ecx
  idMegaTextureLevel *v44; // edx
  int v45; // [esp+0h] [ebp-38h]
  idVec2 center; // [esp+Ch] [ebp-2Ch]
  int tileBase; // [esp+14h] [ebp-24h]
  megaCompressionFormat_t megaCompressionFormat; // [esp+18h] [ebp-20h]
  megaCompressionFormat_t *compressionFormats; // [esp+1Ch] [ebp-1Ch]
  int *maxCompressedTileSizes; // [esp+20h] [ebp-18h]
  int maxCompressedTileSize; // [esp+24h] [ebp-14h]
  int i; // [esp+28h] [ebp-10h]
  int v53; // [esp+34h] [ebp-4h]

  v1 = this;
  idMegaTexture::Purge(this);
  if ( !((unsigned __int8 (*)(void))networkSystem->vfptr->IsDedicated)() )
  {
    if ( idMegaTexture::OpenFile(v1) )
    {
      ((void (__stdcall *)(int *))v1->file->vfptr->ReadInt)(&v1->resolution);
      v2 = v1->resolution;
      v1->tilesPerAxis = v1->resolution / 128;
      *(float *)&compressionFormats = (double)(v2 / 2048);
      v3 = (((signed int)compressionFormats >> 23) & 0xFF) - 126;
      v1->numLevels = v3;
      v4 = alloca(4 * v3);
      v5 = v1->file->vfptr->ReadInt;
      v6 = &v45;
      *(float *)&compressionFormats = COERCE_FLOAT(&v45);
      ((void (__stdcall *)(megaCompressionFormat_t *))v5)(&megaCompressionFormat);
      v7 = idMegaTexture::r_useMegaTextureImageCompression.internalVar->integerValue == 0;
      v1->useImageCompression = idMegaTexture::r_useMegaTextureImageCompression.internalVar->integerValue != 0;
      if ( v7 )
        v1->imageCompressionFormat = 32856;
      else
        v1->imageCompressionFormat = megaCompressionFormat != 1 ? 33779 : 33776;
      for ( j = 0; j < v1->numLevels; ++v6 )
      {
        ((void (__stdcall *)(int *))v1->file->vfptr->ReadInt)(v6);
        ++j;
      }
      v9 = v1->numLevels;
      v10 = alloca(4 * v9);
      v7 = v1->version == 8;
      maxCompressedTileSizes = &v45;
      if ( v7 )
      {
        ((void (__stdcall *)(int *))v1->file->vfptr->ReadInt)(&maxCompressedTileSize);
        v11 = 0;
        if ( v1->numLevels > 0 )
        {
          v12 = maxCompressedTileSize;
          do
            *(&v45 + v11++) = v12;
          while ( v11 < v1->numLevels );
        }
      }
      else
      {
        v13 = 0;
        if ( v9 > 0 )
        {
          v14 = maxCompressedTileSizes;
          do
          {
            ((void (__stdcall *)(int *))v1->file->vfptr->ReadInt)(v14);
            ++v13;
            ++v14;
          }
          while ( v13 < v1->numLevels );
        }
      }
      v15 = idMegaTexture::TotalStoredTileCount(v1->resolution);
      maxCompressedTileSize = v15;
      v1->tileIndexMap = (int *)Mem_Alloc(4 * v15);
      v16 = 0;
      for ( v1->tileIndexedDataSizes = (int *)Mem_Alloc(4 * v15); v16 < maxCompressedTileSize; ++v16 )
      {
        ((void (__stdcall *)(int *))v1->file->vfptr->ReadInt)(&v1->tileIndexMap[v16]);
        ((void (__stdcall *)(int *))v1->file->vfptr->ReadInt)(&v1->tileIndexedDataSizes[v16]);
      }
      idMegaTexture::AllocRecompressionScratch(v1);
      idMegaTexture::GenerateNullTileData(v1);
      idMegaTexture::GenerateGridTileData(v1);
      v17 = v1->numLevels;
      v18 = 16540 * v1->numLevels | -(16540 * (unsigned __int64)(unsigned int)v1->numLevels >> 32 != 0);
      v19 = (int *)operator new((v18 + 4) | -__CFADD__(v18, 4));
      LODWORD(center.y) = v19;
      v20 = 0;
      v53 = 0;
      if ( v19 )
      {
        v20 = (int)(v19 + 1);
        *v19 = v17;
        `eh vector constructor iterator'(
          v19 + 1,
          0x409Cu,
          v17,
          (void (__thiscall *)(void *))idMegaTextureLevel::idMegaTextureLevel,
          (void (__thiscall *)(void *))idMegaTextureLevel::~idMegaTextureLevel);
      }
      v1->levels = (idMegaTextureLevel *)v20;
      v7 = idMegaTexture::r_megaUpscale.internalVar->integerValue == 0;
      v53 = -1;
      if ( !v7 && v1->upscaleLevel )
      {
        v21 = (idMegaTextureLevel *)operator new(0x409Cu);
        LODWORD(center.y) = v21;
        v53 = 1;
        if ( v21 )
          idMegaTextureLevel::idMegaTextureLevel(v21);
        else
          v22 = 0;
        v53 = -1;
        v1->upscaleLevel = v22;
      }
      v23 = v1->numLevels;
      v24 = v1->tilesPerAxis;
      v25 = 0;
      if ( v23 > 0 )
      {
        v26 = v1->numLevels;
        do
        {
          v25 += v24 * v24;
          v24 >>= 1;
          --v26;
        }
        while ( v26 );
      }
      v27 = v23 - 1;
      if ( v27 >= 0 )
      {
        maxCompressedTileSize = 16540 * v27;
        i = (int)&compressionFormats[v27];
        LODWORD(center.y) = (char *)maxCompressedTileSizes - (char *)compressionFormats;
        while ( 1 )
        {
          v24 *= 2;
          v28 = *(_DWORD *)(LODWORD(center.y) + i);
          v29 = *(_DWORD *)i;
          v30 = (idMegaTextureLevel *)((char *)v1->levels + maxCompressedTileSize);
          tileBase = v25 - v24 * v24;
          idMegaTextureLevel::Init(v30, v1, v27, tileBase, v24, 1, v29, v28);
          i -= 4;
          maxCompressedTileSize -= 16540;
          if ( --v27 < 0 )
            break;
          v25 = tileBase;
        }
      }
      v31 = v1->upscaleLevel;
      if ( v31 )
        idMegaTextureLevel::Init(v31, v1, -1, 0, 2 * v24, 1, *compressionFormats, *maxCompressedTileSizes);
      v7 = v1->numLevels == 0;
      v32 = v1->numLevels < 0;
      i = 0;
      if ( !v32 && !v7 )
      {
        maxCompressedTileSize = 0;
        do
        {
          v33 = (int)v1->levels + maxCompressedTileSize;
          compressionFormats = (megaCompressionFormat_t *)16;
          maxCompressedTileSizes = (int *)(v33 + 80);
          do
          {
            v34 = maxCompressedTileSizes;
            v35 = 16;
            do
            {
              idMegaTextureTile::PostInit((idMegaTextureTile *)v34);
              v34 += 256;
              --v35;
            }
            while ( v35 );
            maxCompressedTileSizes += 16;
            compressionFormats = (megaCompressionFormat_t *)((char *)compressionFormats - 1);
          }
          while ( *(float *)&compressionFormats != 0.0 );
          maxCompressedTileSize += 16540;
          v36 = __OFSUB__(i + 1, v1->numLevels);
          v32 = i++ + 1 - v1->numLevels < 0;
        }
        while ( v32 ^ v36 );
      }
      idMegaTexture::LoadDetailTexture(v1);
      v37 = rbinds->megaMaskParams[0];
      v37->data.vector[0] = -2.0;
      v37->data.vector[1] = -2.0;
      v37->data.vector[2] = 0.0;
      v37->data.vector[3] = 1.0;
      rbinds->megaTextureLevel[0]->data.attrib = (int)globalImages->whiteImage;
      v38 = rbinds->megaMaskParams[1];
      v38->data.vector[0] = -2.0;
      v38->data.vector[1] = -2.0;
      v38->data.vector[2] = 0.0;
      v38->data.vector[3] = 1.0;
      rbinds->megaTextureLevel[1]->data.attrib = (int)globalImages->whiteImage;
      v39 = rbinds->megaMaskParams[2];
      v39->data.vector[0] = -2.0;
      v39->data.vector[1] = -2.0;
      v39->data.vector[2] = 0.0;
      v39->data.vector[3] = 1.0;
      rbinds->megaTextureLevel[2]->data.attrib = (int)globalImages->whiteImage;
      v40 = rbinds->megaMaskParams[3];
      v40->data.vector[0] = -2.0;
      v40->data.vector[1] = -2.0;
      v40->data.vector[2] = 0.0;
      v40->data.vector[3] = 1.0;
      rbinds->megaTextureLevel[3]->data.attrib = (int)globalImages->whiteImage;
      v41 = rbinds->megaMaskParams[4];
      v41->data.vector[0] = -2.0;
      v41->data.vector[1] = -2.0;
      v41->data.vector[2] = 0.0;
      v41->data.vector[3] = 1.0;
      rbinds->megaTextureLevel[4]->data.attrib = (int)globalImages->whiteImage;
      v42 = rbinds->megaMaskParams[5];
      v42->data.vector[0] = -2.0;
      v42->data.vector[1] = -2.0;
      v42->data.vector[2] = 0.0;
      v42->data.vector[3] = 1.0;
      rbinds->megaTextureLevel[5]->data.attrib = (int)globalImages->whiteImage;
      v1->purged = 0;
      if ( megaTextureTileLoader->activeMegaTexture != v1 )
        idMegaTextureTileLoader::SetActiveMegaTexture(megaTextureTileLoader, v1);
      if ( megaTextureTileDecompressor->activeMegaTexture != v1 )
        idMegaTextureTileDecompressor::SetActiveMegaTexture(megaTextureTileDecompressor, v1);
      sdLock::Acquire(&v1->lock, 1);
      v43 = v1->numLevels;
      center.x = 0.5;
      v44 = v1->levels;
      center.y = 0.5;
      idMegaTextureLevel::UpdateForCenter(&v44[v43 - 1], &center, 0);
      sdLock::Release(&v1->lock);
      sdSignal::Set(&megaTextureTileLoader->signal);
      sdSignal::Set(&megaTextureTileDecompressor->signal);
      idMegaTexture::ForceUpdate(v1);
    }
    else
    {
      idMegaTexture::CloseFile(v1);
    }
  }
}
int dynamic_initializer_for__singletonLock___71()
{
  sdLock::sdLock(&singletonLock_71);
  return atexit(dynamic_atexit_destructor_for__singletonLock___71);
}
int dynamic_initializer_for__idMegaTexture::r_showMegaTextureLevels__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&idMegaTexture::r_showMegaTextureLevels);
  }
  else
  {
    idMegaTexture::r_showMegaTextureLevels.next = idCVar::staticVars;
    idCVar::staticVars = &idMegaTexture::r_showMegaTextureLevels;
  }
  return atexit(dynamic_atexit_destructor_for__idMegaTexture::r_showMegaTextureLevels__);
}
int dynamic_initializer_for__idMegaTexture::r_skipMegaTexture__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&idMegaTexture::r_skipMegaTexture);
  }
  else
  {
    idMegaTexture::r_skipMegaTexture.next = idCVar::staticVars;
    idCVar::staticVars = &idMegaTexture::r_skipMegaTexture;
  }
  return atexit(dynamic_atexit_destructor_for__idMegaTexture::r_skipMegaTexture__);
}
int dynamic_initializer_for__idMegaTexture::r_skipMegaTextureUpload__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&idMegaTexture::r_skipMegaTextureUpload);
  }
  else
  {
    idMegaTexture::r_skipMegaTextureUpload.next = idCVar::staticVars;
    idCVar::staticVars = &idMegaTexture::r_skipMegaTextureUpload;
  }
  return atexit(dynamic_atexit_destructor_for__idMegaTexture::r_skipMegaTextureUpload__);
}
int dynamic_initializer_for__idMegaTexture::r_useMegaTextureImageCompression__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&idMegaTexture::r_useMegaTextureImageCompression);
  }
  else
  {
    idMegaTexture::r_useMegaTextureImageCompression.next = idCVar::staticVars;
    idCVar::staticVars = &idMegaTexture::r_useMegaTextureImageCompression;
  }
  return atexit(dynamic_atexit_destructor_for__idMegaTexture::r_useMegaTextureImageCompression__);
}
int dynamic_initializer_for__idMegaTexture::r_detailTexture__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&idMegaTexture::r_detailTexture);
  }
  else
  {
    idMegaTexture::r_detailTexture.next = idCVar::staticVars;
    idCVar::staticVars = &idMegaTexture::r_detailTexture;
  }
  return atexit(dynamic_atexit_destructor_for__idMegaTexture::r_detailTexture__);
}
int dynamic_initializer_for__idMegaTexture::r_detailRatio__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&idMegaTexture::r_detailRatio);
  }
  else
  {
    idMegaTexture::r_detailRatio.next = idCVar::staticVars;
    idCVar::staticVars = &idMegaTexture::r_detailRatio;
  }
  return atexit(dynamic_atexit_destructor_for__idMegaTexture::r_detailRatio__);
}
int dynamic_initializer_for__idMegaTexture::r_detailFade__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&idMegaTexture::r_detailFade);
  }
  else
  {
    idMegaTexture::r_detailFade.next = idCVar::staticVars;
    idCVar::staticVars = &idMegaTexture::r_detailFade;
  }
  return atexit(dynamic_atexit_destructor_for__idMegaTexture::r_detailFade__);
}
int dynamic_initializer_for__idMegaTexture::r_megaStreamBlocks__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&idMegaTexture::r_megaStreamBlocks);
  }
  else
  {
    idMegaTexture::r_megaStreamBlocks.next = idCVar::staticVars;
    idCVar::staticVars = &idMegaTexture::r_megaStreamBlocks;
  }
  return atexit(dynamic_atexit_destructor_for__idMegaTexture::r_megaStreamBlocks__);
}
int dynamic_initializer_for__idMegaTexture::r_megaFadeTime__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&idMegaTexture::r_megaFadeTime);
  }
  else
  {
    idMegaTexture::r_megaFadeTime.next = idCVar::staticVars;
    idCVar::staticVars = &idMegaTexture::r_megaFadeTime;
  }
  return atexit(dynamic_atexit_destructor_for__idMegaTexture::r_megaFadeTime__);
}
int dynamic_initializer_for__idMegaTexture::r_megaStreamFromDVD__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&idMegaTexture::r_megaStreamFromDVD);
  }
  else
  {
    idMegaTexture::r_megaStreamFromDVD.next = idCVar::staticVars;
    idCVar::staticVars = &idMegaTexture::r_megaStreamFromDVD;
  }
  return atexit(dynamic_atexit_destructor_for__idMegaTexture::r_megaStreamFromDVD__);
}
int dynamic_initializer_for__idMegaTexture::r_megaUpscale__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&idMegaTexture::r_megaUpscale);
  }
  else
  {
    idMegaTexture::r_megaUpscale.next = idCVar::staticVars;
    idCVar::staticVars = &idMegaTexture::r_megaUpscale;
  }
  return atexit(dynamic_atexit_destructor_for__idMegaTexture::r_megaUpscale__);
}
void __cdecl dynamic_atexit_destructor_for__singletonLock___71()
{
  sdLock::~sdLock(&singletonLock_71);
}
void __cdecl dynamic_atexit_destructor_for__idMegaTexture::r_showMegaTextureLevels__()
{
  idMegaTexture::r_showMegaTextureLevels.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__idMegaTexture::r_skipMegaTexture__()
{
  idMegaTexture::r_skipMegaTexture.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__idMegaTexture::r_skipMegaTextureUpload__()
{
  idMegaTexture::r_skipMegaTextureUpload.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__idMegaTexture::r_useMegaTextureImageCompression__()
{
  idMegaTexture::r_useMegaTextureImageCompression.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__idMegaTexture::r_detailTexture__()
{
  idMegaTexture::r_detailTexture.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__idMegaTexture::r_detailRatio__()
{
  idMegaTexture::r_detailRatio.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__idMegaTexture::r_detailFade__()
{
  idMegaTexture::r_detailFade.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__idMegaTexture::r_megaStreamBlocks__()
{
  idMegaTexture::r_megaStreamBlocks.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__idMegaTexture::r_megaFadeTime__()
{
  idMegaTexture::r_megaFadeTime.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__idMegaTexture::r_megaStreamFromDVD__()
{
  idMegaTexture::r_megaStreamFromDVD.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__idMegaTexture::r_megaUpscale__()
{
  idMegaTexture::r_megaUpscale.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
