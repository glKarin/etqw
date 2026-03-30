int __cdecl GetMegaTilesPerSecond()
{
  signed int v0; // eax
  int v1; // edx
  signed int *v2; // ecx

  v0 = Sys_Milliseconds() - 1000;
  v1 = 0;
  v2 = tileLoadTimes + 4;
  do
  {
    if ( *(v2 - 1) >= v0 )
      ++v1;
    if ( *v2 >= v0 )
      ++v1;
    if ( v2[1] >= v0 )
      ++v1;
    if ( v2[2] >= v0 )
      ++v1;
    v2 += 4;
  }
  while ( (signed int)v2 < (signed int)&sdBinkFile::initialized );
  return v1;
}
int __cdecl GetCompressedUsefulKiloBytesReadPerSecond()
{
  signed int v0; // edx
  int v1; // eax
  unsigned int v2; // ecx

  v0 = Sys_Milliseconds() - 1000;
  v1 = 0;
  v2 = 0;
  do
  {
    if ( tileLoadTimes[v2 / 4] >= v0 )
      v1 += tileLoadData[v2 / 4];
    if ( *(_DWORD *)(v2 + 10349740) >= v0 )
      v1 += *(_DWORD *)(v2 + 10316972);
    if ( *(_DWORD *)(v2 + 10349744) >= v0 )
      v1 += *(_DWORD *)(v2 + 10316976);
    if ( *(_DWORD *)(v2 + 10349748) >= v0 )
      v1 += *(_DWORD *)(v2 + 10316980);
    v2 += 16;
  }
  while ( (signed int)v2 < 0x8000 );
  return v1 / 1024;
}
int __cdecl GetCompressedSeekMBPerSecond()
{
  signed int v0; // edi
  int v1; // esi
  unsigned int v2; // ecx

  v0 = Sys_Milliseconds() - 1000;
  v1 = 0;
  v2 = 0;
  do
  {
    if ( tileLoadTimes[v2 / 4] >= v0 )
      v1 += tileLoadSeek[v2 / 4] / 1024;
    if ( *(_DWORD *)(v2 + 10349740) >= v0 )
      v1 += *(_DWORD *)(v2 + 10284204) / 1024;
    if ( *(_DWORD *)(v2 + 10349744) >= v0 )
      v1 += *(_DWORD *)(v2 + 10284208) / 1024;
    if ( *(_DWORD *)(v2 + 10349748) >= v0 )
      v1 += *(_DWORD *)(v2 + 10284212) / 1024;
    v2 += 16;
  }
  while ( (signed int)v2 < 0x8000 );
  return v1 / 1024;
}
int __cdecl GetCompressedSeeksPerSecond()
{
  signed int v0; // esi
  int result; // eax
  unsigned int v2; // ecx

  v0 = Sys_Milliseconds() - 1000;
  result = 0;
  v2 = 0;
  do
  {
    if ( tileLoadTimes[v2 / 4] >= v0 )
      result += tileLoadSeek[v2 / 4] > 0;
    if ( *(_DWORD *)(v2 + 10349740) >= v0 )
      result += *(_DWORD *)(v2 + 10284204) > 0;
    if ( *(_DWORD *)(v2 + 10349744) >= v0 )
      result += *(_DWORD *)(v2 + 10284208) > 0;
    if ( *(_DWORD *)(v2 + 10349748) >= v0 )
      result += *(_DWORD *)(v2 + 10284212) > 0;
    v2 += 16;
  }
  while ( (signed int)v2 < 0x8000 );
  return result;
}
double __cdecl GetTilesPerSeek()
{
  signed int v0; // ecx
  signed int v1; // esi
  unsigned int v2; // eax

  v0 = 0;
  v1 = 0;
  v2 = 0;
  do
  {
    if ( tileLoadTimes[v2 / 4] > 0 )
    {
      v0 += tileLoadSeek[v2 / 4] > 0;
      ++v1;
    }
    if ( *(_DWORD *)(v2 + 10349740) > 0 )
    {
      v0 += *(_DWORD *)(v2 + 10284204) > 0;
      ++v1;
    }
    if ( *(_DWORD *)(v2 + 10349744) > 0 )
    {
      v0 += *(_DWORD *)(v2 + 10284208) > 0;
      ++v1;
    }
    if ( *(_DWORD *)(v2 + 10349748) > 0 )
    {
      v0 += *(_DWORD *)(v2 + 10284212) > 0;
      ++v1;
    }
    v2 += 16;
  }
  while ( (signed int)v2 < 0x8000 );
  if ( v0 <= 0 )
    return (float)0.0;
  return (float)((double)v1 / (double)v0);
}
void __thiscall idMegaTextureTileLoader::~idMegaTextureTileLoader(idMegaTextureTileLoader *this)
{
  idMegaTextureTileLoader *v1; // esi

  v1 = this;
  this->vfptr = (sdThreadProcessVtbl *)&idMegaTextureTileLoader::`vftable';
  sdSignal::~sdSignal(&this->throttleSignal);
  sdSignal::~sdSignal(&v1->signal);
  v1->vfptr = (sdThreadProcessVtbl *)&sdThreadProcess::`vftable';
}
void __thiscall idMegaTextureTileLoader::StartThread(idMegaTextureTileLoader *this)
{
  idMegaTextureTileLoader *v1; // esi
  sdThread *v2; // eax
  sdThread *v3; // eax

  v1 = this;
  v2 = (sdThread *)operator new(0x44u);
  if ( v2 )
    sdThread::sdThread(v2, (sdThreadProcess *)&v1->vfptr, 0, 0x4000u);
  else
    v3 = 0;
  v1->thread = v3;
  sdThread::SetName(v3, "MegaTextureTileLoader");
  if ( !sdThread::Start(v1->thread, 0, 0) )
    (*(void (__cdecl **)(netadrtype_t, const char *))(*(_DWORD *)common.type + 88))(
      common.type,
      "idMegaTextureTileLoader::StartThread : failed to start thread");
}
void __thiscall idMegaTextureTileLoader::SetActiveMegaTexture(idMegaTextureTileLoader *this, idMegaTexture *megaTexture)
{
  idMegaTextureTileLoader *v2; // edi
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
void __thiscall idMegaTextureTileLoader::ForceUpdate(idMegaTextureTileLoader *this)
{
  idMegaTexture *v1; // ecx

  v1 = this->activeMegaTexture;
  if ( v1 )
    idMegaTexture::ForceUpdate(v1);
}
idMegaTextureTileLoader *__thiscall idMegaTextureTileLoader::`scalar deleting destructor'(idMegaTextureTileLoader *this, unsigned int a2)
{
  idMegaTextureTileLoader *v2; // esi

  v2 = this;
  idMegaTextureTileLoader::~idMegaTextureTileLoader(this);
  if ( a2 & 1 )
    operator delete(v2);
  return v2;
}
void __thiscall idMegaTextureTileLoader::Init(idMegaTextureTileLoader *this)
{
  idMegaTextureTileLoader::StartThread(this);
}
unsigned int __thiscall idMegaTextureTileLoader::Run(idMegaTextureTileLoader *this, void *parm)
{
  idMegaTextureTileLoader *v2; // edi
  idMegaTexture *v3; // eax
  idMegaTexture *v4; // esi
  int v5; // ebp
  int v6; // eax
  idMegaTexture *v7; // ecx
  int v8; // ebx
  int v9; // eax
  idMegaTextureTile *v10; // edi
  idLinkList<idMegaTextureTile> *v11; // eax
  idMegaTextureTile *v12; // eax
  int v13; // eax
  int v14; // ebp
  char *v15; // edx
  char *v16; // eax
  signed int v17; // edi
  int v18; // ebx
  int v19; // ebx
  int v20; // eax
  signed int v21; // edi
  int v22; // ebp
  _DWORD *v23; // esi
  bool v24; // sf
  unsigned __int8 v25; // of
  int *v26; // eax
  signed int v27; // ebp
  _BYTE *v28; // ecx
  char v29; // dl
  $DAC1494F803A6717766EFBE32ED80E09 *v30; // ebx
  int v31; // edi
  int v32; // ebp
  int v33; // esi
  bool v34; // zf
  idMegaTextureTile *tile; // [esp+8h] [ebp-8Ch]
  idMegaTextureTile *tilea; // [esp+8h] [ebp-8Ch]
  idMegaTextureTile *tileb; // [esp+8h] [ebp-8Ch]
  idMegaTextureTile *v39; // [esp+Ch] [ebp-88h]
  idMegaTextureTileLoader *v40; // [esp+10h] [ebp-84h]
  int tileGlobalX; // [esp+14h] [ebp-80h]
  int v42; // [esp+18h] [ebp-7Ch]
  int tileGlobalY; // [esp+1Ch] [ebp-78h]
  idMegaTextureLevel *level; // [esp+20h] [ebp-74h]
  int numTiles; // [esp+24h] [ebp-70h]
  $DAC1494F803A6717766EFBE32ED80E09 tiles[5]; // [esp+2Ch] [ebp-68h]

  v2 = this;
  v40 = this;
  while ( !v2->terminate )
  {
    v3 = v2->activeMegaTexture;
    if ( v3 )
    {
      sdLock::Acquire(&v3->lock, 1);
      v4 = v2->activeMegaTexture;
      v5 = v4->numLevels - 1;
      tileGlobalY = (int)v2->activeMegaTexture;
      if ( v5 >= 0 )
      {
        v6 = 16540 * v5;
        tile = (idMegaTextureTile *)(16540 * v5);
        while ( 1 )
        {
          v7 = v2->activeMegaTexture;
          v8 = (int)v7->levels + v6;
          numTiles = (int)v7->levels + v6;
          if ( !*(_BYTE *)(v8 + 44) )
          {
            v9 = *(_DWORD *)(v8 + 16528);
            if ( v9 && v9 != *(_DWORD *)(v8 + 16524) )
              v10 = *(idMegaTextureTile **)(v9 + 12);
            else
              v10 = 0;
            v39 = v10;
            if ( v10 )
            {
              while ( idMegaTextureTile::IsLoaded(v10) )
              {
                v11 = v10->dirtyNode.next;
                if ( !v11 || v11 == v10->dirtyNode.head )
                {
                  v39 = 0;
                  goto LABEL_19;
                }
                v12 = v11->owner;
                v39 = v12;
                if ( !v12 )
                  goto LABEL_19;
                v10 = v12;
              }
              if ( v10 )
                goto LABEL_26;
            }
LABEL_19:
            v2 = v40;
          }
          --v5;
          v6 = (int)&tile[-259].compressedTileData;
          tile = (idMegaTextureTile *)((char *)tile - 16540);
          if ( v5 < 0 )
            goto LABEL_23;
        }
      }
      v8 = numTiles;
LABEL_23:
      if ( v39 )
      {
        v10 = v39;
LABEL_26:
        v13 = v10->globalY;
        v14 = v10->globalX;
        v15 = (char *)(v14 + *(_DWORD *)(v8 + 32) + v13 * *(_DWORD *)(v8 + 36));
        v42 = v10->globalX;
        level = (idMegaTextureLevel *)v10->globalY;
        tiles[0].y = v10->globalX;
        tiles[0].level = (idMegaTextureLevel *)v13;
        tiles[0].tileNum = v8;
        tiles[0].compressedData = v15;
        v16 = idMegaTextureTile::GetCompressedTileData(v10);
        v17 = 1;
        v18 = *(_DWORD *)(v8 + 16) - 1;
        tiles[1].x = (int)v16;
        if ( v18 >= 0 )
        {
          v19 = (int)&v4->levels[v18];
          if ( *(_BYTE *)(v19 + 44) )
          {
            v20 = 2 * (_DWORD)level;
            v21 = 0;
            v22 = 2 * v14;
            v23 = &tiles[1].level;
            tilea = 0;
            tileGlobalX = 2 * (_DWORD)level;
            for ( tiles[0].x = (int)&tiles[3].level; ; tiles[0].x += 40 )
            {
              while ( 1 )
              {
                v23[2] = *(_DWORD *)(v19 + 32) + v22 + v20 * *(_DWORD *)(v19 + 36);
                *(v23 - 1) = v22;
                *v23 = v20;
                v23[1] = v19;
                v23[3] = idMegaTextureTile::GetChildCompressedTileData(v39, (const int)tilea + v21++);
                v23 += 5;
                ++v22;
                if ( v21 >= 2 )
                  break;
                v20 = tileGlobalX;
              }
              ++tileGlobalX;
              v25 = __OFSUB__((char *)&tilea->level + 2, 4);
              v24 = (signed int)(&tilea[-1].loaded + 1) < 0;
              tilea = (idMegaTextureTile *)((char *)tilea + 2);
              if ( !(v24 ^ v25) )
                break;
              v20 = tileGlobalX;
              v21 = 0;
              v22 = 2 * v42;
              v23 = (_DWORD *)tiles[0].x;
            }
            v17 = 5;
            v4 = (idMegaTexture *)tileGlobalY;
          }
        }
        v26 = &tiles[0].y;
        v27 = v17;
        do
        {
          v28 = (_BYTE *)(v26[4] + v4->tileIndexedDataSizes[v26[3]] + 3);
          *v28 = *(_BYTE *)v26 - 1;
          v29 = *((_BYTE *)v26 + 4) - 1;
          v26 += 5;
          --v27;
          v28[1] = v29;
        }
        while ( v27 );
        sdLock::Release(&v40->activeMegaTexture->lock);
        v30 = &tiles[1];
        tileb = (idMegaTextureTile *)v17;
        while ( 1 )
        {
          v31 = v4->tileIndexedDataSizes[(_DWORD)v30[-1].compressedData] + 3;
          v32 = idMegaTexture::SeekToTile(v4, (int)v30[-1].compressedData);
          (*(void (__stdcall **)(int, int))&v4->file->vfptr->gap4[8])(v30->x, v31);
          v33 = lastTileLoadTime & 0x1FFF;
          tileLoadTimes[v33] = Sys_Milliseconds();
          ++lastTileLoadTime;
          ++v30;
          v34 = tileb == (idMegaTextureTile *)1;
          tileb = (idMegaTextureTile *)((char *)tileb - 1);
          tileLoadData[v33] = v31;
          tileLoadSeek[v33] = v32;
          if ( v34 )
            break;
          v4 = (idMegaTexture *)tileGlobalY;
        }
        sdLock::Acquire(&v40->activeMegaTexture->lock, 1);
        if ( v42 == v39->globalX && level == (idMegaTextureLevel *)v39->globalY )
        {
          v39->loaded = 1;
          sdSignal::Set(&megaTextureTileDecompressor->signal);
        }
        sdLock::Release(&v40->activeMegaTexture->lock);
        ++v40->numProcessedTiles;
        v2 = v40;
      }
      else
      {
        sdLock::Release(&v2->activeMegaTexture->lock);
        sdSignal::Wait(&v2->signal, -1);
      }
    }
    else
    {
      sdSignal::Wait(&v2->signal, -1);
    }
  }
  return 0;
}
int dynamic_initializer_for__megaTextureTileLoaderLocal__()
{
  sdSignal::sdSignal(&stru_7D57F4);
  sdSignal::sdSignal(&stru_7D57F8);
  return atexit(dynamic_atexit_destructor_for__megaTextureTileLoaderLocal__);
}
void __cdecl dynamic_atexit_destructor_for__megaTextureTileLoaderLocal__()
{
  idMegaTextureTileLoader::~idMegaTextureTileLoader(&megaTextureTileLoaderLocal);
}
