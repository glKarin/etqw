void __thiscall idMegaTextureTileDecompressor::~idMegaTextureTileDecompressor(idMegaTextureTileDecompressor *this)
{
  idMegaTextureTileDecompressor *v1; // esi

  v1 = this;
  this->vfptr = (sdThreadProcessVtbl *)&idMegaTextureTileDecompressor::`vftable';
  sdSignal::~sdSignal(&this->throttleSignal);
  sdSignal::~sdSignal(&v1->signal);
  v1->vfptr = (sdThreadProcessVtbl *)&sdThreadProcess::`vftable';
}
void __thiscall idMegaTextureTileDecompressor::DecompressLuminance(idMegaTextureTileDecompressor *this, char *destination)
{
  idMegaTextureTileDecompressor *v2; // esi
  char *v3; // edi
  int v4; // eax
  char *v5; // edi

  v2 = this;
  if ( this->compressedData.parentCachedLevelNum != this->compressedData.parentLevelNum
    || this->compressedData.parentCachedGlobalX != this->compressedData.parentGlobalX
    || this->compressedData.parentCachedGlobalY != this->compressedData.parentGlobalY )
  {
    v3 = this->compressedData.parentData;
    idBareDctBase::SetQuality_Generic(
      (idBareDctBase *)&this->dctDecoder->luminanceQuality,
      (unsigned __int8)*v3,
      (unsigned __int8)v3[1],
      (unsigned __int8)v3[2]);
    idBareDctDecoder::DecompressImageYCoCg_Generic(
      v2->dctDecoder,
      v3 + 3,
      v2->compressedData.parentCachedData,
      128,
      128,
      v2->compressedData.parentSize);
    v4 = v2->compressedData.parentGlobalX;
    v2->compressedData.parentCachedLevelNum = v2->compressedData.parentLevelNum;
    v2->compressedData.parentCachedGlobalX = v4;
  }
  idFilter::UpScale2xBicubic_Generic(
    &v2->compressedData.parentCachedData[256
                                       * ((v2->compressedData.globalX & 1) + ((v2->compressedData.globalY & 1) << 7))],
    64,
    64,
    128,
    destination,
    BICUBIC_SHIFTED);
  v5 = v2->compressedData.data;
  idBareDctBase::SetQuality_Generic(
    (idBareDctBase *)&v2->dctDecoder->luminanceQuality,
    (unsigned __int8)*v5,
    (unsigned __int8)v5[1],
    (unsigned __int8)v5[2]);
  idBareDctDecoder::DecompressLuminanceEnhancement_Generic(
    v2->dctDecoder,
    v5 + 3,
    destination,
    128,
    128,
    v2->compressedData.size);
  idColorSpace::ConvertYCoCgToRGB(destination, 128, 128);
}
void __thiscall idMegaTextureTileDecompressor::DecompressTile(idMegaTextureTileDecompressor *this, megaCompressionFormat_t format, char *destination)
{
  idMegaTextureTileDecompressor *v3; // esi
  char *v4; // edi
  char *v5; // edi

  v3 = this;
  switch ( format )
  {
    case 1:
      v4 = this->compressedData.data;
      idBareDctBase::SetQuality_Generic(
        (idBareDctBase *)&this->dctDecoder->luminanceQuality,
        (unsigned __int8)*v4,
        (unsigned __int8)v4[1],
        (unsigned __int8)v4[2]);
      idBareDctDecoder::DecompressImageRGB_Generic(
        v3->dctDecoder,
        v4 + 3,
        destination,
        128,
        128,
        v3->compressedData.size);
      break;
    case 2:
      v5 = this->compressedData.data;
      idBareDctBase::SetQuality_Generic(
        (idBareDctBase *)&this->dctDecoder->luminanceQuality,
        (unsigned __int8)*v5,
        (unsigned __int8)v5[1],
        (unsigned __int8)v5[2]);
      idBareDctDecoder::DecompressImageRGBA_Generic(
        v3->dctDecoder,
        v5 + 3,
        destination,
        128,
        128,
        v3->compressedData.size);
      break;
    case 3:
      idMegaTextureTileDecompressor::DecompressLuminance(this, destination);
      break;
  }
}
void __thiscall idMegaTextureTileDecompressor::RecompressTile(idMegaTextureTileDecompressor *this, imageCompressionFormat_t format, char *source, char *tileData)
{
  char *v4; // edi
  idMegaTextureTileDecompressor *v5; // ebp
  char *v6; // ebx
  int v7; // esi
  int v8; // edx

  v4 = source;
  v5 = this;
  idMipMap::CreateMips(source, 7);
  if ( format != 32856 )
  {
    v6 = tileData;
    v7 = 128;
    do
    {
      if ( format == 33776 )
        idDxtEncoder::CompressImageDXT1Fast_Generic(v5->dxtEncoder, v4, v6, v7, v7, (int *)&source);
      else
        idDxtEncoder::CompressImageDXT5Fast_Generic(v5->dxtEncoder, v4, v6, v7, v7, (int *)&source);
      v6 = &v6[(_DWORD)source];
      v8 = v7 * v7;
      v7 >>= 1;
      v4 += 4 * v8;
    }
    while ( v7 >= 4 );
  }
}
void __thiscall idMegaTextureTileDecompressor::DecompressLuminance_MMX(idMegaTextureTileDecompressor *this, char *destination)
{
  idMegaTextureTileDecompressor *v2; // esi
  char *v3; // edi
  int v4; // eax
  char *v5; // edi

  v2 = this;
  if ( this->compressedData.parentCachedLevelNum != this->compressedData.parentLevelNum
    || this->compressedData.parentCachedGlobalX != this->compressedData.parentGlobalX
    || this->compressedData.parentCachedGlobalY != this->compressedData.parentGlobalY )
  {
    v3 = this->compressedData.parentData;
    idBareDctBase::SetQuality_Generic(
      (idBareDctBase *)&this->dctDecoder->luminanceQuality,
      (unsigned __int8)*v3,
      (unsigned __int8)v3[1],
      (unsigned __int8)v3[2]);
    idBareDctDecoder::DecompressImageYCoCg_MMX(
      v2->dctDecoder,
      v3 + 3,
      v2->compressedData.parentCachedData,
      128,
      128,
      v2->compressedData.parentSize);
    v4 = v2->compressedData.parentGlobalX;
    v2->compressedData.parentCachedLevelNum = v2->compressedData.parentLevelNum;
    v2->compressedData.parentCachedGlobalX = v4;
  }
  idFilter::UpScale2xBicubic_MMX(
    &v2->compressedData.parentCachedData[256
                                       * ((v2->compressedData.globalX & 1) + ((v2->compressedData.globalY & 1) << 7))],
    64,
    64,
    128,
    destination,
    BICUBIC_SHIFTED);
  v5 = v2->compressedData.data;
  idBareDctBase::SetQuality_Generic(
    (idBareDctBase *)&v2->dctDecoder->luminanceQuality,
    (unsigned __int8)*v5,
    (unsigned __int8)v5[1],
    (unsigned __int8)v5[2]);
  idBareDctDecoder::DecompressLuminanceEnhancement_MMX(
    v2->dctDecoder,
    v5 + 3,
    destination,
    128,
    128,
    v2->compressedData.size);
  idColorSpace::ConvertYCoCgToRGB(destination, 128, 128);
}
void __thiscall idMegaTextureTileDecompressor::DecompressTile_MMX(idMegaTextureTileDecompressor *this, megaCompressionFormat_t format, char *destination)
{
  idMegaTextureTileDecompressor *v3; // esi
  char *v4; // edi
  char *v5; // edi

  v3 = this;
  switch ( format )
  {
    case 1:
      v4 = this->compressedData.data;
      idBareDctBase::SetQuality_MMX(
        (idBareDctBase *)&this->dctDecoder->luminanceQuality,
        (unsigned __int8)*v4,
        (unsigned __int8)v4[1],
        (unsigned __int8)v4[2]);
      idBareDctDecoder::DecompressImageRGB_MMX(v3->dctDecoder, v4 + 3, destination, 128, 128, v3->compressedData.size);
      break;
    case 2:
      v5 = this->compressedData.data;
      idBareDctBase::SetQuality_MMX(
        (idBareDctBase *)&this->dctDecoder->luminanceQuality,
        (unsigned __int8)*v5,
        (unsigned __int8)v5[1],
        (unsigned __int8)v5[2]);
      idBareDctDecoder::DecompressImageRGBA_MMX(v3->dctDecoder, v5 + 3, destination, 128, 128, v3->compressedData.size);
      break;
    case 3:
      idMegaTextureTileDecompressor::DecompressLuminance_MMX(this, destination);
      break;
  }
}
void __thiscall idMegaTextureTileDecompressor::RecompressTile_MMX(idMegaTextureTileDecompressor *this, imageCompressionFormat_t format, char *source, char *tileData)
{
  char *v4; // edi
  idMegaTextureTileDecompressor *v5; // ebp
  char *v6; // ebx
  int v7; // esi
  int v8; // edx

  v4 = source;
  v5 = this;
  idMipMap::CreateMips_MMX(source, 7);
  if ( format != 32856 )
  {
    v6 = tileData;
    v7 = 128;
    do
    {
      if ( format == 33776 )
        idDxtEncoder::CompressImageDXT1Fast_MMX(v5->dxtEncoder, v4, v6, v7, v7, (int *)&source);
      else
        idDxtEncoder::CompressImageDXT5Fast_MMX(v5->dxtEncoder, v4, v6, v7, v7, (int *)&source);
      v6 = &v6[(_DWORD)source];
      v8 = v7 * v7;
      v7 >>= 1;
      v4 += 4 * v8;
    }
    while ( v7 >= 4 );
  }
}
void __thiscall idMegaTextureTileDecompressor::DecompressLuminance_SSE2(idMegaTextureTileDecompressor *this, char *destination)
{
  idMegaTextureTileDecompressor *v2; // esi
  char *v3; // edi
  int v4; // eax
  char *v5; // edi

  v2 = this;
  if ( this->compressedData.parentCachedLevelNum != this->compressedData.parentLevelNum
    || this->compressedData.parentCachedGlobalX != this->compressedData.parentGlobalX
    || this->compressedData.parentCachedGlobalY != this->compressedData.parentGlobalY )
  {
    v3 = this->compressedData.parentData;
    idBareDctBase::SetQuality_Generic(
      (idBareDctBase *)&this->dctDecoder->luminanceQuality,
      (unsigned __int8)*v3,
      (unsigned __int8)v3[1],
      (unsigned __int8)v3[2]);
    idBareDctDecoder::DecompressImageYCoCg_SSE2(
      v2->dctDecoder,
      v3 + 3,
      v2->compressedData.parentCachedData,
      128,
      128,
      v2->compressedData.parentSize);
    v4 = v2->compressedData.parentGlobalX;
    v2->compressedData.parentCachedLevelNum = v2->compressedData.parentLevelNum;
    v2->compressedData.parentCachedGlobalX = v4;
  }
  idFilter::UpScale2xBicubic_SSE2(
    &v2->compressedData.parentCachedData[256
                                       * ((v2->compressedData.globalX & 1) + ((v2->compressedData.globalY & 1) << 7))],
    64,
    64,
    128,
    destination,
    BICUBIC_SHIFTED);
  v5 = v2->compressedData.data;
  idBareDctBase::SetQuality_Generic(
    (idBareDctBase *)&v2->dctDecoder->luminanceQuality,
    (unsigned __int8)*v5,
    (unsigned __int8)v5[1],
    (unsigned __int8)v5[2]);
  idBareDctDecoder::DecompressLuminanceEnhancement_SSE2(
    v2->dctDecoder,
    v5 + 3,
    destination,
    128,
    128,
    v2->compressedData.size);
  idColorSpace::ConvertYCoCgToRGB(destination, 128, 128);
}
void __thiscall idMegaTextureTileDecompressor::DecompressTile_SSE2(idMegaTextureTileDecompressor *this, megaCompressionFormat_t format, char *destination)
{
  idMegaTextureTileDecompressor *v3; // esi
  char *v4; // edi
  char *v5; // edi

  v3 = this;
  switch ( format )
  {
    case 1:
      v4 = this->compressedData.data;
      idBareDctBase::SetQuality_SSE2(
        (idBareDctBase *)&this->dctDecoder->luminanceQuality,
        (unsigned __int8)*v4,
        (unsigned __int8)v4[1],
        (unsigned __int8)v4[2]);
      idBareDctDecoder::DecompressImageRGB_SSE2(v3->dctDecoder, v4 + 3, destination, 128, 128, v3->compressedData.size);
      break;
    case 2:
      v5 = this->compressedData.data;
      idBareDctBase::SetQuality_SSE2(
        (idBareDctBase *)&this->dctDecoder->luminanceQuality,
        (unsigned __int8)*v5,
        (unsigned __int8)v5[1],
        (unsigned __int8)v5[2]);
      idBareDctDecoder::DecompressImageRGBA_SSE2(v3->dctDecoder, v5 + 3, destination, 128, 128, v3->compressedData.size);
      break;
    case 3:
      idMegaTextureTileDecompressor::DecompressLuminance_SSE2(this, destination);
      break;
  }
}
void __thiscall idMegaTextureTileDecompressor::RecompressTile_SSE2(idMegaTextureTileDecompressor *this, imageCompressionFormat_t format, char *source, char *tileData)
{
  char *v4; // edi
  idMegaTextureTileDecompressor *v5; // ebp
  char *v6; // ebx
  int v7; // esi
  int v8; // edx

  v4 = source;
  v5 = this;
  idMipMap::CreateMips_SSE2(source, 7);
  if ( format != 32856 )
  {
    v6 = tileData;
    v7 = 128;
    do
    {
      if ( format == 33776 )
        idDxtEncoder::CompressImageDXT1Fast_SSE2(v5->dxtEncoder, v4, v6, v7, v7, (int *)&source);
      else
        idDxtEncoder::CompressImageDXT5Fast_SSE2(v5->dxtEncoder, v4, v6, v7, v7, (int *)&source);
      v6 = &v6[(_DWORD)source];
      v8 = v7 * v7;
      v7 >>= 1;
      v4 += 4 * v8;
    }
    while ( v7 >= 4 );
  }
}
void __thiscall idMegaTextureTileDecompressor::DecompressLuminance_Xenon(idMegaTextureTileDecompressor *this, char *destination)
{
  idMegaTextureTileDecompressor *v2; // esi
  char *v3; // edi
  int v4; // eax
  char *v5; // edi

  v2 = this;
  if ( this->compressedData.parentCachedLevelNum != this->compressedData.parentLevelNum
    || this->compressedData.parentCachedGlobalX != this->compressedData.parentGlobalX
    || this->compressedData.parentCachedGlobalY != this->compressedData.parentGlobalY )
  {
    v3 = this->compressedData.parentData;
    idBareDctBase::SetQuality_Generic(
      (idBareDctBase *)&this->dctDecoder->luminanceQuality,
      (unsigned __int8)*v3,
      (unsigned __int8)v3[1],
      (unsigned __int8)v3[2]);
    idBareDctDecoder::DecompressImageYCoCg_Xenon(
      v2->dctDecoder,
      v3 + 3,
      v2->compressedData.parentCachedData,
      128,
      128,
      v2->compressedData.parentSize);
    v4 = v2->compressedData.parentGlobalX;
    v2->compressedData.parentCachedLevelNum = v2->compressedData.parentLevelNum;
    v2->compressedData.parentCachedGlobalX = v4;
  }
  idFilter::UpScale2xBicubic_Xenon(
    &v2->compressedData.parentCachedData[256
                                       * ((v2->compressedData.globalX & 1) + ((v2->compressedData.globalY & 1) << 7))],
    64,
    64,
    128,
    destination,
    BICUBIC_SHIFTED);
  v5 = v2->compressedData.data;
  idBareDctBase::SetQuality_Generic(
    (idBareDctBase *)&v2->dctDecoder->luminanceQuality,
    (unsigned __int8)*v5,
    (unsigned __int8)v5[1],
    (unsigned __int8)v5[2]);
  idBareDctDecoder::DecompressLuminanceEnhancement_Xenon(
    v2->dctDecoder,
    v5 + 3,
    destination,
    128,
    128,
    v2->compressedData.size);
  idColorSpace::ConvertYCoCgToRGB(destination, 128, 128);
}
void __thiscall idMegaTextureTileDecompressor::DecompressTile_Xenon(idMegaTextureTileDecompressor *this, megaCompressionFormat_t format, char *destination)
{
  idMegaTextureTileDecompressor *v3; // esi
  char *v4; // edi
  idBareDctDecoder *v5; // ecx
  char *v6; // edi
  idBareDctDecoder *v7; // ecx

  v3 = this;
  switch ( format )
  {
    case 1:
      v4 = this->compressedData.data;
      v5 = this->dctDecoder;
      idBareDctBase::SetQuality_Xenon((unsigned __int8)*v4, (unsigned __int8)v4[1], (unsigned __int8)v4[2]);
      idBareDctDecoder::DecompressImageRGB_Xenon(v3->dctDecoder, v4 + 3, destination, 128, 128, v3->compressedData.size);
      break;
    case 2:
      v6 = this->compressedData.data;
      v7 = this->dctDecoder;
      idBareDctBase::SetQuality_Xenon((unsigned __int8)*v6, (unsigned __int8)v6[1], (unsigned __int8)v6[2]);
      idBareDctDecoder::DecompressImageRGBA_Xenon(
        v3->dctDecoder,
        v6 + 3,
        destination,
        128,
        128,
        v3->compressedData.size);
      break;
    case 3:
      idMegaTextureTileDecompressor::DecompressLuminance_Xenon(this, destination);
      break;
  }
}
void __thiscall idMegaTextureTileDecompressor::RecompressTile_Xenon(idMegaTextureTileDecompressor *this, imageCompressionFormat_t format, char *source, char *tileData)
{
  char *v4; // edi
  idMegaTextureTileDecompressor *v5; // ebp
  char *v6; // ebx
  int v7; // esi
  idDxtEncoder *v8; // ecx
  int v9; // edx

  v4 = source;
  v5 = this;
  SpawnStub((VBRState *)source);
  if ( format != 32856 )
  {
    v6 = tileData;
    v7 = 128;
    do
    {
      v8 = v5->dxtEncoder;
      idDxtEncoder::CompressImageDXT5Fast_Xenon(v4, v6, v7, v7, (int *)&source);
      v6 = &v6[(_DWORD)source];
      v9 = v7 * v7;
      v7 >>= 1;
      v4 += 4 * v9;
    }
    while ( v7 >= 4 );
  }
}
void __thiscall idMegaTextureTileDecompressor::GetCompressedTileData(idMegaTextureTileDecompressor *this, idMegaTexture *mega, idMegaTextureLevel *level, idMegaTextureTile *tile)
{
  idMegaTextureTileDecompressor *v4; // esi
  int v5; // eax
  int v6; // edi
  int v7; // ecx
  int v8; // eax
  idMegaTextureLevel *v9; // ecx
  int v10; // ST00_4
  int v11; // edi

  v4 = this;
  this->compressedData.globalX = tile->globalX;
  v5 = tile->globalY;
  this->compressedData.globalY = v5;
  v6 = this->compressedData.globalX + level->tileBase + v5 * level->tilesPerAxis;
  this->compressedData.data = idMegaTextureTile::GetCompressedTileData(tile);
  v4->compressedData.size = mega->tileIndexedDataSizes[v6];
  if ( level->megaCompressionFormat == 3 )
  {
    v7 = level->levelNum + 1;
    v4->compressedData.parentLevelNum = v7;
    v4->compressedData.parentGlobalX = tile->globalX >> 1;
    v8 = tile->globalY >> 1;
    v4->compressedData.parentGlobalY = v8;
    v9 = &mega->levels[v7];
    v10 = v4->compressedData.parentGlobalX;
    v11 = v10 + v9->tileBase + v8 * v9->tilesPerAxis;
    v4->compressedData.parentData = idMegaTextureLevel::GetCompressedTileData(v9, v10, v8);
    v4->compressedData.parentSize = mega->tileIndexedDataSizes[v11];
  }
}
void __thiscall idMegaTextureTileDecompressor::Stop(idMegaTextureTileDecompressor *this)
{
  idMegaTextureTileDecompressor *v1; // esi

  v1 = this;
  this->terminate = 1;
  sdSignal::Set(&this->signal);
  sdSignal::Set(&v1->throttleSignal);
}
idMegaTextureTileDecompressor *__thiscall idMegaTextureTileDecompressor::`vector deleting destructor'(idMegaTextureTileDecompressor *this, unsigned int a2)
{
  idMegaTextureTileDecompressor *v2; // esi

  v2 = this;
  idMegaTextureTileDecompressor::~idMegaTextureTileDecompressor(this);
  if ( a2 & 1 )
    operator delete(v2);
  return v2;
}
void __thiscall idMegaTextureTileDecompressor::StartThread(idMegaTextureTileDecompressor *this)
{
  idMegaTextureTileDecompressor *v1; // esi
  sdThread *v2; // eax
  sdThread *v3; // eax

  v1 = this;
  v2 = (sdThread *)operator new(0x44u);
  if ( v2 )
    sdThread::sdThread(v2, (sdThreadProcess *)&v1->vfptr, 0, 0);
  else
    v3 = 0;
  v1->thread = v3;
  sdThread::SetName(v3, "MegaTextureTileDecompressor");
  if ( !sdThread::Start(v1->thread, 0, 0) )
    (*(void (__cdecl **)(netadrtype_t, const char *))(*(_DWORD *)common.type + 88))(
      common.type,
      "idMegaTextureTileDecompressor::StartThread : failed to start thread");
}
void __thiscall idMegaTextureTileDecompressor::SetActiveMegaTexture(idMegaTextureTileDecompressor *this, idMegaTexture *megaTexture)
{
  idMegaTextureTileDecompressor *v2; // edi
  idMegaTexture *v3; // esi

  v2 = this;
  v3 = this->activeMegaTexture;
  if ( v3 != megaTexture )
  {
    if ( v3 )
      sdLock::Acquire(&v3->lock, 1);
    v2->activeMegaTexture = megaTexture;
    if ( v3 )
      sdLock::Release(&v3->lock);
    else
      sdSignal::Set(&v2->signal);
  }
}
void __thiscall idMegaTextureTileDecompressor::Init(idMegaTextureTileDecompressor *this)
{
  idMegaTextureTileDecompressor *v1; // esi
  idBareDctDecoder *v2; // eax
  idBareDctDecoder *v3; // eax

  v1 = this;
  this->cpuid = (*(int (**)(void))&sys->vfptr->gap4[32])();
  v2 = (idBareDctDecoder *)operator new(0x3C98u);
  if ( v2 )
    idBareDctDecoder::idBareDctDecoder(v2);
  else
    v3 = 0;
  v1->dctDecoder = v3;
  v1->dxtEncoder = (idDxtEncoder *)operator new(0xCu);
  memset(&v1->compressedData, 0, 0x34u);
  v1->compressedData.parentCachedLevelNum = -1;
  v1->compressedData.parentCachedGlobalX = -1;
  v1->compressedData.parentCachedGlobalY = -1;
  v1->compressedData.parentCachedData = (char *)Mem_AllocAligned(0x10000u, ALIGN_16);
  idMegaTextureTileDecompressor::StartThread(v1);
}
void __thiscall idMegaTextureTileDecompressor::Shutdown(idMegaTextureTileDecompressor *this)
{
  idMegaTextureTileDecompressor *v1; // esi
  sdThread *v2; // ecx
  idBareDctDecoder *v3; // edi
  idDxtEncoder *v4; // eax
  char *v5; // ST04_4

  v1 = this;
  v2 = this->thread;
  if ( v2 )
  {
    sdThread::Stop(v2);
    sdThread::Join(v1->thread);
    sdThread::Destroy(v1->thread);
    v1->thread = 0;
  }
  v3 = v1->dctDecoder;
  if ( v3 )
  {
    idBareDctDecoder::~idBareDctDecoder(v1->dctDecoder);
    operator delete(v3);
  }
  v4 = v1->dxtEncoder;
  v1->dctDecoder = 0;
  if ( v4 )
    operator delete(v4);
  v5 = v1->compressedData.parentCachedData;
  v1->dxtEncoder = 0;
  Mem_FreeAligned(v5);
  v1->compressedData.parentCachedData = 0;
}
unsigned int __thiscall idMegaTextureTileDecompressor::Run(idMegaTextureTileDecompressor *this, void *parm)
{
  idMegaTextureLevel *v2; // ebp
  idMegaTextureTile *v3; // ebx
  idMegaTextureTileDecompressor *v4; // edi
  idMegaTexture *v5; // eax
  idMegaTexture *v6; // eax
  int v7; // esi
  int v8; // edi
  idMegaTextureLevel *v9; // ebp
  idLinkList<idMegaTextureTile> *v10; // eax
  idMegaTextureTile *v11; // eax
  int v12; // eax
  idLinkList<idMegaTextureTile> *v13; // eax
  int v14; // edi
  int v15; // esi
  int v16; // eax
  float v17; // ST74_4
  float v18; // ST74_4
  float v19; // ST74_4
  float v20; // ST74_4
  int v21; // eax
  tileData_t *v22; // esi
  signed int v23; // esi
  signed int v24; // eax
  signed int v25; // edi
  signed int v26; // ecx
  signed int v27; // edx
  signed int v28; // ebx
  size_t v29; // ecx
  signed int v30; // ebx
  int v31; // eax
  int v32; // eax
  bool v33; // zf
  idMegaTextureTileDecompressor *v34; // edi
  signed int v35; // eax
  int v36; // eax
  cpuid_t v37; // eax
  char *v38; // ecx
  idMegaTexture *v39; // eax
  idMegaTexture *v40; // eax
  char *v41; // ST60_4
  idMegaTexture *v42; // eax
  idMegaTextureTileDecompressor *v44; // [esp+50h] [ebp-38h]
  idMegaTexture *mega; // [esp+54h] [ebp-34h]
  signed int v46; // [esp+58h] [ebp-30h]
  char *destination; // [esp+5Ch] [ebp-2Ch]
  char *destinationa; // [esp+5Ch] [ebp-2Ch]
  char *destinationb; // [esp+5Ch] [ebp-2Ch]
  char *destinationc; // [esp+5Ch] [ebp-2Ch]
  idMegaTextureTile *tile; // [esp+60h] [ebp-28h]
  signed int v52; // [esp+64h] [ebp-24h]
  signed int v53; // [esp+68h] [ebp-20h]
  int shaderQuality; // [esp+6Ch] [ebp-1Ch]
  int shaderQualitya; // [esp+6Ch] [ebp-1Ch]
  int gpuSpec; // [esp+70h] [ebp-18h]
  sdLock *gpuSpeca; // [esp+70h] [ebp-18h]
  char *dest; // [esp+78h] [ebp-10h]
  char *desta; // [esp+78h] [ebp-10h]
  char *destb; // [esp+78h] [ebp-10h]
  int tileGlobalX; // [esp+7Ch] [ebp-Ch]
  int tileGlobalY; // [esp+80h] [ebp-8h]
  int tileBase; // [esp+84h] [ebp-4h]
  int tileBasea; // [esp+84h] [ebp-4h]

  v44 = this;
  if ( this->terminate )
    return 0;
  v2 = (idMegaTextureLevel *)tileBase;
  v3 = tile;
  v4 = this;
  do
  {
    v5 = v4->activeMegaTexture;
    if ( !v5 )
    {
      sdSignal::Wait(&v4->signal, -1);
      goto LABEL_93;
    }
    sdLock::Acquire(&v5->lock, 1);
    v6 = v4->activeMegaTexture;
    v7 = v6->numLevels - 1;
    gpuSpec = com_gpuSpec.internalVar->integerValue;
    mega = v4->activeMegaTexture;
    shaderQuality = r_shaderQuality.internalVar->integerValue;
    if ( v7 < 0 )
      goto LABEL_26;
    v8 = v7;
    while ( 1 )
    {
      v9 = mega->levels;
      v10 = v9[v8].dirtyTiles.next;
      v2 = &v9[v8];
      if ( v10 && v10 != v2->dirtyTiles.head )
        v11 = v10->owner;
      else
        v11 = 0;
      v3 = v11;
      tile = v11;
      if ( v11 )
        break;
LABEL_24:
      --v7;
      --v8;
      if ( v7 < 0 )
      {
        v6 = mega;
        v4 = v44;
LABEL_26:
        if ( v3 )
          goto LABEL_28;
        sdLock::Release(&v6->lock);
        sdSignal::Wait(&v4->signal, -1);
        goto LABEL_93;
      }
    }
    while ( !idMegaTextureTile::IsLoaded(v3) )
    {
LABEL_18:
      v13 = v3->dirtyNode.next;
      if ( !v13 || v13 == v3->dirtyNode.head )
      {
        v3 = 0;
        tile = 0;
        goto LABEL_24;
      }
      v3 = v13->owner;
      tile = v13->owner;
      if ( !v3 )
        goto LABEL_24;
    }
    v12 = v2->levelNum;
    if ( v12 && !gpuSpec && shaderQuality == 2 && v12 & 1 )
    {
      idMegaTextureLevel::RemoveDirtyTile(v3);
      goto LABEL_18;
    }
    if ( !v3 )
      goto LABEL_24;
LABEL_28:
    v14 = ((int (*)(void))sys->vfptr->Milliseconds)();
    v15 = v14 - v44->lastProcessedTime;
    if ( mega->forcedUpdate )
      goto LABEL_33;
    v16 = idMegaTextureTileDecompressor::r_megaTilesPerSecond.internalVar->integerValue;
    if ( v16 <= 0 )
      goto LABEL_33;
    v17 = (double)v16 / 1000.0;
    v18 = _ceil(v17);
    v52 = (signed int)v18;
    v19 = 1000.0 / (double)idMegaTextureTileDecompressor::r_megaTilesPerSecond.internalVar->integerValue;
    v20 = _ceil(v19);
    v46 = (signed int)v20;
    if ( v15 )
    {
      v44->numTilesThisMsec = 0;
      if ( v15 < v46 )
        goto LABEL_40;
      v44->numTilesThisMsec = 1;
LABEL_33:
      v44->lastProcessedTime = v14;
      v22 = idMegaTextureLevel::GetAvailableTile(v2);
      tileGlobalX = v3->globalX;
      dest = (char *)v22;
      tileGlobalY = v3->globalY;
      tileBasea = v2->tileBase;
      idMegaTextureTileDecompressor::GetCompressedTileData(v44, mega, v2, v3);
      gpuSpeca = &mega->lock;
      sdLock::Release(&mega->lock);
      if ( idMegaTextureTileDecompressor::r_megaShowGrid.internalVar->integerValue )
      {
        v23 = mega->imageCompressionFormat;
        v24 = 128;
        destination = (char *)128;
        v25 = 128;
        v26 = 128;
        v27 = 1;
        if ( v23 == 32856 )
        {
          v28 = 1;
          v53 = 1;
        }
        else if ( v23 <= 33775 || v23 > 33779 )
        {
          v28 = v53;
        }
        else
        {
          v28 = 4;
          v53 = 4;
        }
        while ( v24 > v28 || v26 > v28 )
        {
          ++v27;
          v24 >>= 1;
          v26 >>= 1;
        }
        v29 = 0;
        if ( v27 > 0 )
        {
          shaderQualitya = v27;
          v30 = 128;
          do
          {
            if ( v23 > 33777 )
            {
              if ( v23 < 33778 || v23 > 33779 )
              {
LABEL_58:
                v29 = -1;
                goto LABEL_59;
              }
              v32 = v30 + 3;
              v30 = (signed int)destination;
              v29 += 16 * v32 / 4 * ((v25 + 3) / 4);
            }
            else if ( v23 >= 33776 )
            {
              v31 = v30 + 3;
              v30 = (signed int)destination;
              v29 += 8 * v31 / 4 * ((v25 + 3) / 4);
            }
            else
            {
              if ( v23 != 32856 )
                goto LABEL_58;
              v29 += 4 * v30 * v25;
            }
LABEL_59:
            v30 >>= 1;
            v25 >>= 1;
            v33 = shaderQualitya-- == 1;
            destination = (char *)v30;
          }
          while ( !v33 );
        }
        v22 = (tileData_t *)dest;
        memcpy(*((void **)dest + 3), mega->gridTileData, v29);
        v3 = tile;
        v34 = v44;
      }
      else if ( idMegaTextureTileDecompressor::r_megaShowTileSize.internalVar->integerValue )
      {
        desta = v2->megaTexture->tileRecompressionScratch;
        v35 = v44->compressedData.size;
        if ( v35 >= 5120 )
        {
          if ( v35 >= 7168 )
          {
            if ( v35 >= 12288 )
              v36 = sdColor4::PackColor(&colorRed);
            else
              v36 = sdColor4::PackColor(&colorYellow);
          }
          else
          {
            v36 = sdColor4::PackColor(&colorGreen);
          }
        }
        else
        {
          v36 = sdColor4::PackColor(&colorBlue);
        }
        memset32(desta, v36, 0x4000u);
        idMegaTextureTileDecompressor::RecompressTile(v44, v2->megaTexture->imageCompressionFormat, desta, v22->pic);
        v34 = v44;
      }
      else
      {
        v34 = v44;
        v37 = v44->cpuid;
        v38 = v22->pic;
        destb = v22->pic;
        if ( v37 & 0x400 )
        {
          v39 = v2->megaTexture;
          if ( v39->useImageCompression )
          {
            destinationa = v39->tileRecompressionScratch;
            idMegaTextureTileDecompressor::DecompressTile_Xenon(v44, v2->megaCompressionFormat, destinationa);
          }
          else
          {
            destinationa = v22->pic;
            idMegaTextureTileDecompressor::DecompressTile_Xenon(v44, v2->megaCompressionFormat, v38);
          }
          idMegaTextureTileDecompressor::RecompressTile_Xenon(
            v44,
            v2->megaTexture->imageCompressionFormat,
            destinationa,
            destb);
        }
        else if ( (v37 & 0x80u) == 0 )
        {
          v33 = (v37 & 0x10) == 0;
          v42 = v2->megaTexture;
          if ( v33 )
          {
            if ( v42->useImageCompression )
            {
              destinationc = v42->tileRecompressionScratch;
              idMegaTextureTileDecompressor::DecompressTile(v44, v2->megaCompressionFormat, destinationc);
            }
            else
            {
              destinationc = v22->pic;
              idMegaTextureTileDecompressor::DecompressTile(v44, v2->megaCompressionFormat, v38);
            }
            idMegaTextureTileDecompressor::RecompressTile(
              v44,
              v2->megaTexture->imageCompressionFormat,
              destinationc,
              destb);
          }
          else
          {
            if ( v42->useImageCompression )
            {
              destinationb = v42->tileRecompressionScratch;
              idMegaTextureTileDecompressor::DecompressTile_MMX(v44, v2->megaCompressionFormat, destinationb);
            }
            else
            {
              destinationb = v22->pic;
              idMegaTextureTileDecompressor::DecompressTile_MMX(v44, v2->megaCompressionFormat, v38);
            }
            idMegaTextureTileDecompressor::RecompressTile_MMX(
              v44,
              v2->megaTexture->imageCompressionFormat,
              destinationb,
              destb);
          }
        }
        else
        {
          v40 = v2->megaTexture;
          if ( v40->useImageCompression )
            v38 = v40->tileRecompressionScratch;
          v41 = v38;
          idMegaTextureTileDecompressor::DecompressTile_SSE2(v44, v2->megaCompressionFormat, v38);
          idMegaTextureTileDecompressor::RecompressTile_SSE2(v44, v2->megaTexture->imageCompressionFormat, v41, destb);
        }
      }
      sdLock::Acquire(gpuSpeca, 1);
      if ( tileGlobalX != v3->globalX || tileGlobalY != v3->globalY )
      {
        idMegaTextureLevel::ReleaseTile(v2, v22);
      }
      else
      {
        v22->y = tileGlobalY;
        v22->tileBase = tileBasea;
        v22->x = tileGlobalX;
        v3->tileData = v22;
        idMegaTextureLevel::RemoveDirtyTile(v3);
      }
      sdLock::Release(gpuSpeca);
      ++v34->numProcessedTiles;
      goto LABEL_93;
    }
    v21 = v44->numTilesThisMsec;
    if ( v21 < v52 )
    {
      v44->numTilesThisMsec = v21 + 1;
      goto LABEL_33;
    }
LABEL_40:
    sdLock::Release(&mega->lock);
    if ( v15 > 0 && v15 < v46 )
      sdSignal::Wait(&v44->throttleSignal, v46 - v15);
LABEL_93:
    v4 = v44;
  }
  while ( !v44->terminate );
  return 0;
}
int dynamic_initializer_for__singletonLock___72()
{
  sdLock::sdLock(&singletonLock_72);
  return atexit(dynamic_atexit_destructor_for__singletonLock___72);
}
int dynamic_initializer_for__megaTextureTileDecompressorLocal__()
{
  sdSignal::sdSignal(&stru_7D566C);
  sdSignal::sdSignal(&stru_7D5670);
  unk_7D5674 = 0;
  unk_7D56B8 = 0;
  unk_7D56BC = 0;
  return atexit(dynamic_atexit_destructor_for__megaTextureTileDecompressorLocal__);
}
int dynamic_initializer_for__idMegaTextureTileDecompressor::r_megaTilesPerSecond__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&idMegaTextureTileDecompressor::r_megaTilesPerSecond);
  }
  else
  {
    idMegaTextureTileDecompressor::r_megaTilesPerSecond.next = idCVar::staticVars;
    idCVar::staticVars = &idMegaTextureTileDecompressor::r_megaTilesPerSecond;
  }
  return atexit(dynamic_atexit_destructor_for__idMegaTextureTileDecompressor::r_megaTilesPerSecond__);
}
int dynamic_initializer_for__idMegaTextureTileDecompressor::r_megaShowGrid__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&idMegaTextureTileDecompressor::r_megaShowGrid);
  }
  else
  {
    idMegaTextureTileDecompressor::r_megaShowGrid.next = idCVar::staticVars;
    idCVar::staticVars = &idMegaTextureTileDecompressor::r_megaShowGrid;
  }
  return atexit(dynamic_atexit_destructor_for__idMegaTextureTileDecompressor::r_megaShowGrid__);
}
int dynamic_initializer_for__idMegaTextureTileDecompressor::r_megaShowTileSize__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&idMegaTextureTileDecompressor::r_megaShowTileSize);
  }
  else
  {
    idMegaTextureTileDecompressor::r_megaShowTileSize.next = idCVar::staticVars;
    idCVar::staticVars = &idMegaTextureTileDecompressor::r_megaShowTileSize;
  }
  return atexit(dynamic_atexit_destructor_for__idMegaTextureTileDecompressor::r_megaShowTileSize__);
}
void __cdecl dynamic_atexit_destructor_for__singletonLock___72()
{
  sdLock::~sdLock(&singletonLock_72);
}
void __cdecl dynamic_atexit_destructor_for__megaTextureTileDecompressorLocal__()
{
  idMegaTextureTileDecompressor::~idMegaTextureTileDecompressor(&megaTextureTileDecompressorLocal);
}
void __cdecl dynamic_atexit_destructor_for__idMegaTextureTileDecompressor::r_megaTilesPerSecond__()
{
  idMegaTextureTileDecompressor::r_megaTilesPerSecond.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__idMegaTextureTileDecompressor::r_megaShowGrid__()
{
  idMegaTextureTileDecompressor::r_megaShowGrid.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__idMegaTextureTileDecompressor::r_megaShowTileSize__()
{
  idMegaTextureTileDecompressor::r_megaShowTileSize.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
