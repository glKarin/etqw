void __thiscall rvParticleParms::~rvParticleParms(rvParticleParms *this)
{
  operator delete(this->mModelInfo);
}
void __cdecl rvParticleTemplate::ShutdownStatic()
{
  rvParticleTemplate::sInited = 0;
}
void __thiscall rvParticleTemplate::PurgeTraceModel(rvParticleTemplate *this)
{
  rvParticleTemplate *v1; // esi
  int v2; // eax

  v1 = this;
  v2 = this->mTraceModelIndex;
  if ( v2 != -1 )
  {
    ((void (__stdcall *)(int))bse->vfptr->FreeTraceModel)(v2);
    v1->mTraceModelIndex = -1;
  }
}
idTraceModel *__thiscall rvParticleTemplate::GetTraceModel(rvParticleTemplate *this)
{
  return (idTraceModel *)((int (__stdcall *)(int))bse->vfptr->GetTraceModel)(this->mTraceModelIndex);
}
char __thiscall rvParticleTemplate::UsesEndOrigin(rvParticleTemplate *this)
{
  char result; // al

  if ( this->mpSpawnPosition->mFlags & 2 || this->mpSpawnLength->mFlags & 2 )
    result = 1;
  else
    result = ((unsigned int)this->mFlags >> 22) & 1;
  return result;
}
void __thiscall rvParticleTemplate::SetParameterCounts(rvParticleTemplate *this)
{
  rvParticleParms *v1; // eax
  rvParticleParms *v2; // edx

  switch ( this->mType )
  {
    case 1:
      this->mNumSizeParms = 2;
      this->mNumRotateParms = 1;
      v1 = &rvParticleTemplate::sSPF_ONE_2;
      v2 = &rvParticleTemplate::sSPF_NONE_1;
      goto LABEL_11;
    case 2:
    case 7:
    case 8:
    case 9:
      this->mNumSizeParms = 1;
      this->mNumRotateParms = 0;
      v1 = &rvParticleTemplate::sSPF_ONE_1;
      v2 = &rvParticleTemplate::sSPF_NONE_0;
      goto LABEL_11;
    case 3:
      this->mNumSizeParms = 2;
      this->mNumRotateParms = 3;
      v1 = &rvParticleTemplate::sSPF_ONE_2;
      v2 = &rvParticleTemplate::sSPF_NONE_3;
      goto LABEL_11;
    case 4:
      this->mNumSizeParms = 3;
      this->mNumRotateParms = 1;
      v2 = &rvParticleTemplate::sSPF_NONE_1;
      goto LABEL_10;
    case 5:
      this->mNumSizeParms = 3;
      this->mNumRotateParms = 3;
      goto LABEL_9;
    case 6:
      this->mNumSizeParms = 3;
      this->mNumRotateParms = 0;
      v2 = &rvParticleTemplate::sSPF_NONE_0;
      goto LABEL_10;
    case 0xA:
      this->mNumSizeParms = 0;
      this->mNumRotateParms = 3;
LABEL_9:
      v2 = &rvParticleTemplate::sSPF_NONE_3;
LABEL_10:
      v1 = &rvParticleTemplate::sSPF_ONE_3;
LABEL_11:
      this->mpSpawnSize = v1;
      this->mpSpawnRotate = v2;
      this->mpDeathSize = v1;
      this->mpDeathRotate = v2;
      break;
    default:
      return;
  }
}
double __thiscall rvParticleTemplate::CostTrail(rvParticleTemplate *this, float cost)
{
  rvTrailInfo *v2; // eax
  double result; // st7
  float costa; // [esp+4h] [ebp+4h]
  float costb; // [esp+4h] [ebp+4h]

  v2 = this->mTrailInfo;
  switch ( v2->mTrailType )
  {
    case 1:
      costa = v2->mTrailCount.y * (cost + cost);
      result = costa;
      break;
    case 2:
      costb = v2->mTrailCount.y * (cost * 1.5) + 20.0;
      result = costb;
      break;
    default:
      result = cost;
      break;
  }
  return result;
}
void __stdcall rvParticleTemplate::FixupParms(rvParticleParms *parms)
{
  rvParticleParms *v1; // esi
  char v2; // cl
  int v3; // edx
  unsigned int v4; // eax
  float *v5; // ebx
  float *v6; // ebp
  int v7; // ecx
  float *v8; // esi
  float *v9; // edi
  float *v10; // esi
  bool maxs_3; // [esp+7h] [ebp-5h]

  v1 = parms;
  v2 = parms->mSpawnType;
  v3 = parms->mSpawnType & 3;
  v4 = 4 * ((unsigned int)(unsigned __int8)parms->mSpawnType >> 2);
  if ( !v4 )
    return;
  if ( v4 == 4 )
    return;
  maxs_3 = v4 != 8;
  if ( v2 == 43 || v2 == 47 )
    return;
  v5 = &parms->mMaxs.x;
  v6 = &parms->mMins.x;
  if ( (v3 < 2 || *v6 == parms->mMins.y)
    && (v3 < 3 || *v6 == parms->mMins.z)
    && (!maxs_3 || *v6 == *v5 && (v3 < 2 || *v6 == parms->mMaxs.y) && (v3 < 3 || *v6 == parms->mMaxs.z)) )
  {
    if ( 0.0 == *v6 )
    {
      parms->mSpawnType = v3;
    }
    else if ( 1.0 == *v6 )
    {
      parms->mSpawnType = v3 + 4;
    }
    else
    {
      parms->mSpawnType = v3 + 8;
    }
    goto LABEL_42;
  }
  if ( maxs_3 )
  {
    v7 = 0;
    if ( v3 < 4 )
    {
LABEL_24:
      if ( v7 >= v3 )
      {
LABEL_35:
        v1->mSpawnType = v3 + 8;
        goto LABEL_42;
      }
      v10 = &v5[v7];
      while ( *v10 == *(float *)((char *)v10 + (char *)v6 - (char *)v5) )
      {
        ++v7;
        ++v10;
        if ( v7 >= v3 )
        {
          v1 = parms;
          parms->mSpawnType = v3 + 8;
          goto LABEL_42;
        }
      }
    }
    else
    {
      v8 = &parms->mMaxs.y;
      v9 = &parms->mMaxs.x;
      while ( *(v8 - 1) == *(v9 - 3) )
      {
        if ( *v8 != *(v8 - 3) )
        {
          ++v7;
          break;
        }
        if ( v8[1] != *(v9 - 1) )
        {
          v7 += 2;
          break;
        }
        if ( v8[2] != *v9 )
        {
          v7 += 3;
          break;
        }
        v7 += 4;
        v8 += 4;
        v9 += 4;
        if ( v7 >= v3 - 3 )
        {
          v5 = &parms->mMaxs.x;
          v1 = parms;
          goto LABEL_24;
        }
      }
      v5 = &parms->mMaxs.x;
    }
    if ( v7 < v3 )
    {
      v1 = parms;
      goto LABEL_42;
    }
    v1 = parms;
    goto LABEL_35;
  }
LABEL_42:
  if ( v1->mSpawnType >= 8u )
  {
    if ( v3 == 1 )
    {
      parms->mMins.y = 0.0;
      v5[1] = 0.0;
      parms->mMins.z = 0.0;
      v5[2] = 0.0;
    }
    else if ( v3 == 2 )
    {
      parms->mMins.z = 0.0;
      v5[2] = 0.0;
    }
  }
  else
  {
    parms->mMins.z = 0.0;
    parms->mMins.y = 0.0;
    *v6 = 0.0;
  }
  if ( v1->mSpawnType <= 0xBu )
  {
    *v5 = *v6;
    v5[1] = parms->mMins.y;
    v5[2] = parms->mMins.z;
  }
  if ( v1->mFlags & 2 )
  {
    if ( v1->mSpawnType <= 0xCu )
      v1->mSpawnType = v3 + 12;
  }
}
rvElectricityInfo *__thiscall rvElectricityInfo::operator=(rvElectricityInfo *this, rvElectricityInfo *__that)
{
  rvElectricityInfo *result; // eax

  result = this;
  this->mNumForks = __that->mNumForks;
  this->mStatic = __that->mStatic;
  this->mPad = __that->mPad;
  this->mForkSizeMins.x = __that->mForkSizeMins.x;
  this->mForkSizeMins.y = __that->mForkSizeMins.y;
  this->mForkSizeMins.z = __that->mForkSizeMins.z;
  this->mForkSizeMaxs.x = __that->mForkSizeMaxs.x;
  this->mForkSizeMaxs.y = __that->mForkSizeMaxs.y;
  this->mForkSizeMaxs.z = __that->mForkSizeMaxs.z;
  this->mJitterSize.x = __that->mJitterSize.x;
  this->mJitterSize.y = __that->mJitterSize.y;
  this->mJitterSize.z = __that->mJitterSize.z;
  this->mJitterRate = __that->mJitterRate;
  this->mJitterTable = __that->mJitterTable;
  return result;
}
int __thiscall rvParticleTemplate::GetMaxTrailCount(rvParticleTemplate *this)
{
  float v1; // ST08_4

  v1 = _ceil(this->mTrailInfo->mTrailCount.y);
  return (signed int)v1 + 1;
}
double __thiscall rvParticleTemplate::GetSpawnVolume(rvParticleTemplate *this, rvBSE *effect)
{
  rvParticleParms *v2; // esi
  float work_4; // ST04_4
  float work_8; // ST08_4
  float temp; // ST0C_4
  float v6; // ST20_4
  float v7; // ST24_4
  float v9; // [esp+1Ch] [ebp-Ch]
  float effecta; // [esp+2Ch] [ebp+4h]
  float effectb; // [esp+2Ch] [ebp+4h]

  v2 = this->mpSpawnPosition;
  if ( v2->mFlags & 2 )
  {
    work_4 = effect->mOriginalEndOrigin.x - effect->mOriginalOrigin.x;
    work_8 = effect->mOriginalEndOrigin.y - effect->mOriginalOrigin.y;
    temp = effect->mOriginalEndOrigin.z - effect->mOriginalOrigin.z;
    effecta = work_8 * work_8 + work_4 * work_4 + temp * temp;
    effectb = sqrt(effecta);
    v9 = effectb - v2->mMins.x;
  }
  else
  {
    v9 = v2->mMaxs.x - v2->mMins.x;
  }
  v6 = v2->mMaxs.y - v2->mMins.y;
  v7 = v2->mMaxs.z - v2->mMins.z;
  return (float)((v6 + v9 + v7) / 100.0);
}
int __thiscall rvParticleTemplate::GetTrailCount(rvParticleTemplate *this)
{
  signed int v1; // eax

  v1 = (signed int)rvRandom::flrand(this->mTrailInfo->mTrailCount.x, this->mTrailInfo->mTrailCount.y);
  return v1 < 0 ? 0 : v1;
}
char __stdcall rvParticleTemplate::GetVector(idParser *src, int count, idVec3 *result)
{
  int v4; // edx
  unsigned int v5; // ecx
  float *v6; // eax
  idToken token; // [esp+0h] [ebp-60h]
  __int16 v8; // [esp+50h] [ebp-10h]
  int v9; // [esp+5Ch] [ebp-4h]

  *(long double *)((char *)&token.floatvalue + 4) = 0.0;
  token.data = 0;
  *(_DWORD *)token.baseBuffer = -20;
  token.alloced = (int)&token.baseBuffer[4];
  token.baseBuffer[4] = 0;
  token.subtype = 0;
  token.line = 0;
  token.linesCrossed = 0;
  token.flags = 0;
  token.intvalue = 0;
  LODWORD(token.floatvalue) = 0;
  token.whiteSpaceEnd_p = 0;
  token.next = 0;
  *(_DWORD *)&token.binaryIndex = 0;
  v8 = 0;
  v9 = 0;
  if ( count > 0 )
  {
    result->x = idParser::ParseFloat(src, 0);
    if ( count > 1 )
    {
      if ( !idParser::ExpectTokenString(src, ",", 0) )
      {
LABEL_4:
        v9 = -1;
        idStr::FreeData((idStr *)&token.data);
        return 0;
      }
      result->y = idParser::ParseFloat(src, 0);
      if ( count > 2 )
      {
        if ( !idParser::ExpectTokenString(src, ",", 0) )
          goto LABEL_4;
        result->z = idParser::ParseFloat(src, 0);
      }
    }
  }
  v4 = count;
  if ( 3 - count >= 4 )
  {
    v5 = ((unsigned int)(-1 - count) >> 2) + 1;
    v6 = &result->z + count;
    v4 = count + 4 * v5;
    do
    {
      v6 += 4;
      --v5;
      *(v6 - 6) = *((float *)result + count - 1);
      *(v6 - 5) = *((float *)result + count - 1);
      *(v6 - 4) = *((float *)result + count - 1);
      *(v6 - 3) = *((float *)result + count - 1);
    }
    while ( v5 );
  }
  for ( ; v4 < 3; *((float *)result + v4 - 1) = *((float *)result + count - 1) )
    ++v4;
  v9 = -1;
  idStr::FreeData((idStr *)&token.data);
  return 1;
}
char __stdcall rvParticleTemplate::CheckCommonParms(idParser *src, rvParticleParms *parms)
{
  char result; // al
  idToken token; // [esp+0h] [ebp-60h]
  __int16 v4; // [esp+50h] [ebp-10h]
  int v5; // [esp+5Ch] [ebp-4h]

  *(long double *)((char *)&token.floatvalue + 4) = 0.0;
  token.data = 0;
  *(_DWORD *)token.baseBuffer = -20;
  token.alloced = (int)&token.baseBuffer[4];
  token.baseBuffer[4] = 0;
  token.subtype = 0;
  token.line = 0;
  token.linesCrossed = 0;
  token.flags = 0;
  token.intvalue = 0;
  LODWORD(token.floatvalue) = 0;
  token.whiteSpaceEnd_p = 0;
  token.next = 0;
  *(_DWORD *)&token.binaryIndex = 0;
  v4 = 0;
  v5 = 0;
  if ( idParser::ReadToken(src, (idToken *)((char *)&token + 4)) )
  {
    while ( idStr::Cmp((const char *)token.alloced, "}") )
    {
      if ( idStr::Icmp((const char *)token.alloced, "surface") )
      {
        if ( idStr::Icmp((const char *)token.alloced, "useEndOrigin") )
        {
          if ( idStr::Icmp((const char *)token.alloced, "cone") )
          {
            if ( idStr::Icmp((const char *)token.alloced, "relative") )
            {
              if ( idStr::Icmp((const char *)token.alloced, "linearSpacing") )
              {
                if ( idStr::Icmp((const char *)token.alloced, "attenuate") )
                {
                  if ( !idStr::Icmp((const char *)token.alloced, "inverseAttenuate") )
                    parms->mFlags |= 0x40u;
                }
                else
                {
                  parms->mFlags |= 0x20u;
                }
              }
              else
              {
                parms->mFlags |= 0x10u;
              }
            }
            else
            {
              parms->mFlags |= 8u;
            }
          }
          else
          {
            parms->mFlags |= 4u;
          }
        }
        else
        {
          parms->mFlags |= 2u;
        }
      }
      else
      {
        parms->mFlags |= 1u;
      }
      if ( !idParser::ReadToken(src, (idToken *)((char *)&token + 4)) )
        goto LABEL_18;
    }
    v5 = -1;
    idStr::FreeData((idStr *)&token.data);
    result = 1;
  }
  else
  {
LABEL_18:
    v5 = -1;
    idStr::FreeData((idStr *)&token.data);
    result = 0;
  }
  return result;
}
char __thiscall rvParticleTemplate::ParseBlendParms(rvParticleTemplate *this, rvDeclEffect *effect, idParser *src)
{
  rvParticleTemplate *v3; // edi
  char result; // al
  idLexer *v5; // eax
  int v6; // edi
  idBitMsg **v7; // esi
  int v8; // eax
  idToken token; // [esp+0h] [ebp-60h]
  __int16 v10; // [esp+50h] [ebp-10h]
  int v11; // [esp+5Ch] [ebp-4h]

  *(long double *)((char *)&token.floatvalue + 4) = 0.0;
  v3 = this;
  token.data = 0;
  *(_DWORD *)token.baseBuffer = -20;
  token.alloced = (int)&token.baseBuffer[4];
  token.baseBuffer[4] = 0;
  token.subtype = 0;
  token.line = 0;
  token.linesCrossed = 0;
  token.flags = 0;
  token.intvalue = 0;
  LODWORD(token.floatvalue) = 0;
  token.whiteSpaceEnd_p = 0;
  token.next = 0;
  *(_DWORD *)&token.binaryIndex = 0;
  v10 = 0;
  v11 = 0;
  if ( idParser::ReadToken(src, (idToken *)((char *)&token + 4)) )
  {
    if ( idStr::Icmp((const char *)token.alloced, "add") )
    {
      v5 = src->scriptstack;
      if ( v5 )
        v6 = v5->line;
      else
        v6 = 0;
      if ( v5 )
        v7 = (idBitMsg **)v5->filename.data;
      else
        v7 = &s2;
      v8 = (*(int (**)(void))effect->base->vfptr->gap4)();
      (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
        common.type,
        "^4BSE:^1 Invalid blend type '%s' in '%s' (file: %s, line: %d)",
        token.alloced,
        v8,
        v7,
        v6);
    }
    else
    {
      v3->mFlags |= 0x8000u;
    }
    v11 = -1;
    idStr::FreeData((idStr *)&token.data);
    result = 1;
  }
  else
  {
    v11 = -1;
    idStr::FreeData((idStr *)&token.data);
    result = 0;
  }
  return result;
}
void __usercall rvParticleTemplate::Finish(rvParticleTemplate *this@<ecx>, int a2@<edi>)
{
  double v2; // st7
  rvParticleTemplate *v3; // esi
  rvTrailInfo *v4; // eax
  float *v5; // eax
  int v6; // eax
  int v7; // ebp
  idTraceModel *v8; // eax
  idTraceModel *v9; // edi
  idBounds *v10; // ebp
  rvTrailInfo *v11; // ecx
  rvElectricityInfo *v12; // eax
  float v13; // ST10_4
  float v14; // ST10_4
  rvTrailInfo *v15; // ecx
  float v16; // ST10_4
  rvTrailInfo *v17; // ecx
  double v18; // st6
  float *v19; // ecx
  float v20; // ST10_4
  float *v21; // eax
  float v22; // ST14_4
  float v23; // ST18_4
  float v24; // ST1C_4
  float v25; // ST20_4
  float v26; // ST24_4
  float v27; // ST28_4
  signed int retaddr; // [esp+2Ch] [ebp+0h]

  v2 = 0.0;
  v3 = this;
  v3->mFlags |= 0x100u;
  v4 = this->mTrailInfo;
  if ( (!v4->mTrailType || v4->mTrailType == 3) && !v4->mStatic )
  {
    v4->mTrailTime.y = 0.0;
    v4->mTrailTime.x = 0.0;
    v5 = &this->mTrailInfo->mTrailCount.x;
    v5[1] = 0.0;
    *v5 = 0.0;
  }
  switch ( this->mType )
  {
    case 1:
    case 2:
      v11 = this->mTrailInfo;
      v3->mVertexCount = 4;
      v3->mIndexCount = 6;
      if ( 0.0 != v11->mTrailCount.y && v11->mTrailType == 1 )
      {
        v3->mVertexCount *= (unsigned __int16)rvParticleTemplate::GetMaxTrailCount(v3);
        v2 = 0.0;
        v3->mIndexCount *= (unsigned __int16)rvParticleTemplate::GetMaxTrailCount(v3);
      }
      break;
    case 4:
    case 6:
    case 8:
    case 9:
      this->mVertexCount = 4;
      this->mIndexCount = 6;
      break;
    case 5:
      v6 = ((int (__stdcall *)(_DWORD, int))this->mModel->vfptr->Surface)(0, a2);
      v7 = v6;
      if ( v6 )
      {
        v3->mVertexCount = *(_WORD *)(*(_DWORD *)(v6 + 8) + 48);
        v3->mIndexCount = *(_WORD *)(*(_DWORD *)(v6 + 8) + 56);
      }
      v3->mMaterial = *(idMaterial **)(v6 + 4);
      rvParticleTemplate::PurgeTraceModel(v3);
      v8 = (idTraceModel *)operator new(0xB4Cu);
      v9 = v8;
      retaddr = 0;
      if ( v8 )
      {
        v10 = *(idBounds **)(v7 + 8);
        idTraceModel::InitBox(v8);
        idTraceModel::SetupBox(v9, v10);
      }
      retaddr = -1;
      v2 = 0.0;
      v3->mTraceModelIndex = ((int (*)(void))bse->vfptr->AddTraceModel)();
      break;
    case 7:
      v12 = this->mElecInfo;
      this->mVertexCount = 20 * (LOWORD(v12->mNumForks) + 1);
      this->mIndexCount = 60 * (LOWORD(v12->mNumForks) + 1);
      break;
    case 0xA:
      this->mVertexCount = 0;
      this->mIndexCount = 0;
      break;
    default:
      break;
  }
  if ( v3->mDuration.y <= (double)v3->mDuration.x )
  {
    v13 = v3->mDuration.x;
    v3->mDuration.x = v3->mDuration.y;
    v3->mDuration.y = v13;
  }
  if ( v3->mGravity.y <= (double)v3->mGravity.x )
  {
    v14 = v3->mGravity.x;
    v3->mGravity.x = v3->mGravity.y;
    v3->mGravity.y = v14;
  }
  v15 = v3->mTrailInfo;
  if ( !v15->mStatic )
  {
    if ( v15->mTrailTime.y <= (double)v15->mTrailTime.x )
    {
      v16 = v15->mTrailTime.x;
      v15->mTrailTime.x = v15->mTrailTime.y;
      v15->mTrailTime.y = v16;
    }
    v17 = v3->mTrailInfo;
    v18 = v17->mTrailCount.x;
    v19 = &v17->mTrailCount.x;
    if ( v19[1] <= v18 )
    {
      v20 = *v19;
      *v19 = v19[1];
      v19[1] = v20;
    }
  }
  v3->mCentre.z = v2;
  v3->mCentre.y = v2;
  v3->mCentre.x = v2;
  if ( !(((unsigned int)v3->mFlags >> 12) & 1) )
  {
    v21 = (float *)&v3->mpSpawnPosition->mSpawnType;
    switch ( *(unsigned __int8 *)v21 )
    {
      case 0xBu:
        v3->mCentre.x = v21[3];
        v3->mCentre.y = v21[4];
        v3->mCentre.z = v21[5];
        break;
      case 0xFu:
      case 0x13u:
      case 0x17u:
      case 0x1Bu:
      case 0x1Fu:
      case 0x23u:
      case 0x27u:
      case 0x2Bu:
      case 0x2Fu:
        v22 = v21[6] + v21[3];
        v23 = v21[7] + v21[4];
        v24 = v21[8] + v21[5];
        v25 = v22 * 0.5;
        v26 = v23 * 0.5;
        v27 = 0.5 * v24;
        v3->mCentre.x = v25;
        v3->mCentre.y = v26;
        v3->mCentre.z = v27;
        break;
      default:
        return;
    }
  }
}
rvTrailInfo *__thiscall rvTrailInfo::operator=(rvTrailInfo *this, rvTrailInfo *__that)
{
  rvTrailInfo *v2; // esi
  int v3; // ebp
  void **v4; // ebx

  v2 = this;
  this->mTrailType = __that->mTrailType;
  this->mStatic = __that->mStatic;
  this->mPad = __that->mPad;
  v3 = __that->mTrailTypeName.len;
  v4 = (void **)&this->mTrailTypeName;
  if ( v3 + 1 > abs(this->mTrailTypeName.alloced) )
    idStr::ReAllocate(&this->mTrailTypeName, v3 + 1, 0);
  memcpy(v4[1], __that->mTrailTypeName.data, v3);
  *((_BYTE *)v4[1] + v3) = 0;
  *v4 = (void *)v3;
  v2->mTrailMaterial = __that->mTrailMaterial;
  v2->mTrailTime.x = __that->mTrailTime.x;
  v2->mTrailTime.y = __that->mTrailTime.y;
  v2->mTrailCount.x = __that->mTrailCount.x;
  v2->mTrailCount.y = __that->mTrailCount.y;
  v2->mTrailScale = __that->mTrailScale;
  return v2;
}
void __thiscall rvParticleTemplate::AllocElectricityInfo(rvParticleTemplate *this)
{
  rvParticleTemplate *v1; // esi
  rvElectricityInfo *v2; // eax
  rvElectricityInfo *v3; // ecx

  v1 = this;
  if ( this->mElecInfo->mStatic )
  {
    v2 = (rvElectricityInfo *)operator new(0x34u);
    if ( v2 )
    {
      v2->mStatic = 0;
      v3 = v2;
    }
    else
    {
      v3 = 0;
    }
    v1->mElecInfo = v3;
    rvElectricityInfo::operator=(v3, &rvParticleTemplate::sElectricityInfo);
    v1->mElecInfo->mStatic = 0;
  }
}
rvEnvParms *__thiscall sdDetails::sdPoolAlloc<rvEnvParms,128>::Alloc(sdDetails::sdPoolAlloc<rvEnvParms,128> *this)
{
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v1; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128>::block_t *v2; // eax
  sdDetails::sdPoolAlloc<rvEnvParms,128>::element_t *v3; // eax
  signed int v4; // edi
  sdDetails::sdPoolAlloc<rvEnvParms,128>::element_t *v5; // ecx
  sdDetails::sdPoolAlloc<rvEnvParms,128>::element_t *v6; // eax

  v1 = this;
  if ( !this->free )
  {
    v2 = (sdDetails::sdPoolAlloc<rvEnvParms,128>::block_t *)operator new(0x1204u);
    v2->next = v1->blocks;
    v1->blocks = v2;
    v3 = &v2->elements[2];
    v4 = 32;
    do
    {
      v3[-2].next = v1->free;
      v3[-1].next = v3 - 2;
      v5 = v3 + 1;
      v3->next = v3 - 1;
      v5->next = v3;
      v3 += 4;
      --v4;
      v1->free = v5;
    }
    while ( v4 );
    v1->total += 128;
  }
  v6 = v1->free;
  ++v1->active;
  v1->free = v6->next;
  v6->next = 0;
  return (rvEnvParms *)v6->t;
}
rvParticleParms *__thiscall sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(sdDetails::sdPoolAlloc<rvParticleParms,128> *this)
{
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v1; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128>::block_t *v2; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *v3; // eax
  signed int v4; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *v5; // ecx
  sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *v6; // eax

  v1 = this;
  if ( !this->free )
  {
    v2 = (sdDetails::sdPoolAlloc<rvParticleParms,128>::block_t *)operator new(0x1404u);
    v2->next = v1->blocks;
    v1->blocks = v2;
    v3 = &v2->elements[2];
    v4 = 32;
    do
    {
      v3[-2].next = v1->free;
      v3[-1].next = v3 - 2;
      v5 = v3 + 1;
      v3->next = v3 - 1;
      v5->next = v3;
      v3 += 4;
      --v4;
      v1->free = v5;
    }
    while ( v4 );
    v1->total += 128;
  }
  v6 = v1->free;
  ++v1->active;
  v1->free = v6->next;
  v6->next = 0;
  return (rvParticleParms *)v6->t;
}
void __thiscall rvParticleTemplate::AllocTrail(rvParticleTemplate *this)
{
  rvParticleTemplate *v1; // esi
  rvTrailInfo *v2; // eax

  v1 = this;
  if ( this->mTrailInfo->mStatic )
  {
    v2 = (rvTrailInfo *)operator new(0x3Cu);
    if ( v2 )
    {
      v2->mStatic = 0;
      v2->mTrailTypeName.len = 0;
      v2->mTrailTypeName.alloced = -20;
      v2->mTrailTypeName.data = v2->mTrailTypeName.baseBuffer;
      v2->mTrailTypeName.baseBuffer[0] = 0;
    }
    else
    {
      v2 = 0;
    }
    v1->mTrailInfo = v2;
    rvTrailInfo::operator=(v2, &rvParticleTemplate::sTrailInfo);
    v1->mTrailInfo->mStatic = 0;
  }
}
void __thiscall sdDynamicBlockManager<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128>::Init(sdDynamicBlockManager<rvParticleParms,&sdPoolAllocator_rvParticleParms,128> *this)
{
  sdDynamicBlockManager<rvParticleParms,&sdPoolAllocator_rvParticleParms,128> *v1; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v2; // eax

  v1 = this;
  v2 = (sdDetails::sdPoolAlloc<rvParticleParms,128> *)operator new(0x14u);
  if ( v2 )
  {
    v2->blocks = 0;
    v2->free = 0;
    v2->numFree = 0;
    v2->active = 0;
    v2->total = 0;
    v1->allocator = v2;
  }
  else
  {
    v1->allocator = 0;
  }
}
void __cdecl rvParticleTemplate::InitStatic()
{
  sdDeclTypeHolder *v0; // eax
  int v1; // eax
  sdDeclTypeHolder *v2; // eax

  if ( !rvParticleTemplate::sInited )
  {
    rvParticleTemplate::sInited = 1;
    rvParticleTemplate::sTrailInfo.mTrailType = 0;
    unk_7E672A = 1;
    v0 = sdSingleton<sdDeclTypeHolder>::GetInstance();
    v1 = ((int (__stdcall *)(int, const char *, signed int))declManager->vfptr->FindType)(
           v0->declMaterialType.declTypeHandle,
           "_default",
           1);
    unk_7E6754 = 0.0;
    unk_7E674C = v1;
    unk_7E6750 = 0.0;
    rvParticleTemplate::sElectricityInfo.mNumForks = 0;
    unk_7E675C = 0.0;
    rvParticleTemplate::sElectricityInfo.mStatic = 1;
    unk_7E6758 = 0.0;
    unk_7E6760 = 0.5;
    rvParticleTemplate::sElectricityInfo.mForkSizeMins.x = -20.0;
    rvParticleTemplate::sElectricityInfo.mForkSizeMins.y = -20.0;
    rvParticleTemplate::sElectricityInfo.mForkSizeMins.z = -20.0;
    rvParticleTemplate::sElectricityInfo.mForkSizeMaxs.x = 20.0;
    rvParticleTemplate::sElectricityInfo.mForkSizeMaxs.y = 20.0;
    rvParticleTemplate::sElectricityInfo.mForkSizeMaxs.z = 20.0;
    rvParticleTemplate::sElectricityInfo.mJitterSize.x = 3.0;
    rvParticleTemplate::sElectricityInfo.mJitterSize.y = 7.0;
    rvParticleTemplate::sElectricityInfo.mJitterSize.z = 7.0;
    rvParticleTemplate::sElectricityInfo.mJitterRate = 0.0;
    v2 = sdSingleton<sdDeclTypeHolder>::GetInstance();
    rvParticleTemplate::sElectricityInfo.mJitterTable = (idDeclTable *)((int (__stdcall *)(int, const char *, signed int))declManager->vfptr->FindType)(
                                                                         v2->declTableType.declTypeHandle,
                                                                         "halfsintable",
                                                                         1);
    rvEnvParms::Init(&rvParticleTemplate::sDefaultEnvelope);
    rvEnvParms::SetDefaultType(&rvParticleTemplate::sDefaultEnvelope);
    rvParticleTemplate::sDefaultEnvelope.mStatic = 1;
    rvEnvParms::Init(&rvParticleTemplate::sEmptyEnvelope);
    rvParticleTemplate::sSPF_ONE_1.mRange = 0.0;
    rvParticleTemplate::sEmptyEnvelope.mStatic = 1;
    rvParticleTemplate::sSPF_ONE_1.mMins.z = 0.0;
    rvParticleTemplate::sSPF_ONE_1.mSpawnType = 5;
    rvParticleTemplate::sSPF_ONE_1.mMins.y = 0.0;
    rvParticleTemplate::sSPF_ONE_1.mFlags = 0;
    rvParticleTemplate::sSPF_ONE_1.mMins.x = 0.0;
    rvParticleTemplate::sSPF_ONE_1.mModelInfo = 0;
    rvParticleTemplate::sSPF_ONE_1.mMaxs.z = 0.0;
    rvParticleTemplate::sSPF_ONE_1.mStatic = 1;
    rvParticleTemplate::sSPF_ONE_1.mMaxs.y = 0.0;
    rvParticleTemplate::sSPF_ONE_2.mSpawnType = 6;
    rvParticleTemplate::sSPF_ONE_1.mMaxs.x = 0.0;
    rvParticleTemplate::sSPF_ONE_2.mFlags = 0;
    rvParticleTemplate::sSPF_ONE_2.mRange = 0.0;
    rvParticleTemplate::sSPF_ONE_2.mModelInfo = 0;
    rvParticleTemplate::sSPF_ONE_2.mMins.z = 0.0;
    rvParticleTemplate::sSPF_ONE_2.mStatic = 1;
    rvParticleTemplate::sSPF_ONE_2.mMins.y = 0.0;
    rvParticleTemplate::sSPF_ONE_3.mSpawnType = 7;
    rvParticleTemplate::sSPF_ONE_2.mMins.x = 0.0;
    rvParticleTemplate::sSPF_ONE_3.mFlags = 0;
    rvParticleTemplate::sSPF_ONE_2.mMaxs.z = 0.0;
    rvParticleTemplate::sSPF_ONE_2.mMaxs.y = 0.0;
    rvParticleTemplate::sSPF_ONE_2.mMaxs.x = 0.0;
    rvParticleTemplate::sSPF_ONE_3.mRange = 0.0;
    rvParticleTemplate::sSPF_ONE_3.mMins.z = 0.0;
    rvParticleTemplate::sSPF_ONE_3.mModelInfo = 0;
    rvParticleTemplate::sSPF_ONE_3.mMins.y = 0.0;
    rvParticleTemplate::sSPF_ONE_3.mStatic = 1;
    rvParticleTemplate::sSPF_ONE_3.mMins.x = 0.0;
    rvParticleTemplate::sSPF_NONE_0.mSpawnType = 0;
    rvParticleTemplate::sSPF_ONE_3.mMaxs.z = 0.0;
    rvParticleTemplate::sSPF_NONE_0.mFlags = 0;
    rvParticleTemplate::sSPF_ONE_3.mMaxs.y = 0.0;
    rvParticleTemplate::sSPF_NONE_0.mModelInfo = 0;
    rvParticleTemplate::sSPF_ONE_3.mMaxs.x = 0.0;
    rvParticleTemplate::sSPF_NONE_0.mStatic = 1;
    rvParticleTemplate::sSPF_NONE_0.mRange = 0.0;
    rvParticleTemplate::sSPF_NONE_1.mSpawnType = 1;
    rvParticleTemplate::sSPF_NONE_0.mMins.z = 0.0;
    rvParticleTemplate::sSPF_NONE_1.mFlags = 0;
    rvParticleTemplate::sSPF_NONE_0.mMins.y = 0.0;
    rvParticleTemplate::sSPF_NONE_1.mModelInfo = 0;
    rvParticleTemplate::sSPF_NONE_0.mMins.x = 0.0;
    rvParticleTemplate::sSPF_NONE_1.mStatic = 1;
    rvParticleTemplate::sSPF_NONE_0.mMaxs.z = 0.0;
    rvParticleTemplate::sSPF_NONE_3.mSpawnType = 3;
    rvParticleTemplate::sSPF_NONE_0.mMaxs.y = 0.0;
    rvParticleTemplate::sSPF_NONE_3.mFlags = 0;
    rvParticleTemplate::sSPF_NONE_0.mMaxs.x = 0.0;
    rvParticleTemplate::sSPF_NONE_3.mModelInfo = 0;
    rvParticleTemplate::sSPF_NONE_1.mRange = 0.0;
    rvParticleTemplate::sSPF_NONE_3.mStatic = 1;
    rvParticleTemplate::sSPF_NONE_1.mMins.z = 0.0;
    rvParticleTemplate::sSPF_NONE_1.mMins.y = 0.0;
    rvParticleTemplate::sSPF_NONE_1.mMins.x = 0.0;
    rvParticleTemplate::sSPF_NONE_1.mMaxs.z = 0.0;
    rvParticleTemplate::sSPF_NONE_1.mMaxs.y = 0.0;
    rvParticleTemplate::sSPF_NONE_1.mMaxs.x = 0.0;
    rvParticleTemplate::sSPF_NONE_3.mRange = 0.0;
    rvParticleTemplate::sSPF_NONE_3.mMins.z = 0.0;
    rvParticleTemplate::sSPF_NONE_3.mMins.y = 0.0;
    rvParticleTemplate::sSPF_NONE_3.mMins.x = 0.0;
    rvParticleTemplate::sSPF_NONE_3.mMaxs.z = 0.0;
    rvParticleTemplate::sSPF_NONE_3.mMaxs.y = 0.0;
    rvParticleTemplate::sSPF_NONE_3.mMaxs.x = 0.0;
  }
}
void __thiscall rvParticleTemplate::Init(rvParticleTemplate *this)
{
  rvParticleTemplate *v1; // esi
  sdDeclTypeHolder *v2; // eax
  rvDeclEffect **v3; // eax
  signed int v4; // ecx

  v1 = this;
  rvParticleTemplate::InitStatic();
  v1->mFlags = 0;
  v1->mType = 0;
  v2 = sdSingleton<sdDeclTypeHolder>::GetInstance();
  v1->mMaterial = (idMaterial *)((int (__stdcall *)(int, const char *, signed int))declManager->vfptr->FindType)(
                                  v2->declMaterialType.declTypeHandle,
                                  "_default",
                                  1);
  v1->mModel = (idRenderModel *)((int (__stdcall *)(const char *))renderModelManager->vfptr->FindModel)("_default");
  v1->mTraceModelIndex = -1;
  v1->mGravity.y = 0.0;
  v1->mGravity.x = 0.0;
  v1->mTiling = 8.0;
  v1->mPhysicsDistance = 0.0;
  v1->mBounce = 0.0;
  v1->mDuration.x = 0.0020000001;
  v1->mDuration.y = 0.0020000001;
  v1->mCentre.z = 0.0;
  v1->mCentre.y = 0.0;
  v1->mCentre.x = 0.0;
  v1->mFlags |= 0x4000000u;
  v1->mpSpawnPosition = &rvParticleTemplate::sSPF_NONE_3;
  v1->mWindDeviationAngle = 0.0;
  v1->mpSpawnDirection = &rvParticleTemplate::sSPF_NONE_3;
  v1->mpSpawnVelocity = &rvParticleTemplate::sSPF_NONE_3;
  v1->mpSpawnAcceleration = &rvParticleTemplate::sSPF_NONE_3;
  v1->mpSpawnFriction = &rvParticleTemplate::sSPF_NONE_3;
  v1->mpSpawnRotate = &rvParticleTemplate::sSPF_NONE_3;
  v1->mpSpawnAngle = &rvParticleTemplate::sSPF_NONE_3;
  v1->mpSpawnOffset = &rvParticleTemplate::sSPF_NONE_3;
  v1->mpSpawnLength = &rvParticleTemplate::sSPF_NONE_3;
  v1->mpDeathTint = &rvParticleTemplate::sSPF_NONE_3;
  v1->mpDeathRotate = &rvParticleTemplate::sSPF_NONE_3;
  v1->mpDeathAngle = &rvParticleTemplate::sSPF_NONE_3;
  v1->mpDeathOffset = &rvParticleTemplate::sSPF_NONE_3;
  v1->mpDeathLength = &rvParticleTemplate::sSPF_NONE_3;
  v1->mNumSizeParms = 2;
  v1->mNumRotateParms = 1;
  v1->mVertexCount = 4;
  v1->mIndexCount = 6;
  v1->mTrailInfo = &rvParticleTemplate::sTrailInfo;
  v1->mElecInfo = &rvParticleTemplate::sElectricityInfo;
  v1->mpSpawnTint = &rvParticleTemplate::sSPF_ONE_3;
  v1->mpSpawnFade = &rvParticleTemplate::sSPF_ONE_1;
  v1->mpSpawnSize = &rvParticleTemplate::sSPF_ONE_3;
  v1->mpSpawnWindStrength = &rvParticleTemplate::sSPF_NONE_1;
  v1->mpTintEnvelope = &rvParticleTemplate::sEmptyEnvelope;
  v1->mpFadeEnvelope = &rvParticleTemplate::sEmptyEnvelope;
  v1->mpSizeEnvelope = &rvParticleTemplate::sEmptyEnvelope;
  v1->mpRotateEnvelope = &rvParticleTemplate::sEmptyEnvelope;
  v1->mpAngleEnvelope = &rvParticleTemplate::sEmptyEnvelope;
  v1->mpOffsetEnvelope = &rvParticleTemplate::sEmptyEnvelope;
  v1->mpLengthEnvelope = &rvParticleTemplate::sEmptyEnvelope;
  v1->mpDeathFade = &rvParticleTemplate::sSPF_NONE_1;
  v1->mpDeathSize = &rvParticleTemplate::sSPF_ONE_3;
  v1->mTrailRepeat = 1;
  v1->mNumFrames = 0;
  v1->mNumImpactEffects = 0;
  v1->mNumTimeoutEffects = 0;
  v3 = v1->mTimeoutEffects;
  v4 = 4;
  do
  {
    *(v3 - 5) = 0;
    *v3 = 0;
    ++v3;
    --v4;
  }
  while ( v4 );
  v1->mFlags |= 0x8000000u;
}
char __thiscall rvParticleTemplate::ParseImpact(rvParticleTemplate *this, rvDeclEffect *effect, idParser *src)
{
  int v3; // ebp
  rvParticleTemplate *v4; // esi
  int v6; // edi
  sdDeclTypeHolder *v7; // eax
  idLexer *v8; // eax
  idBitMsg **v9; // edi
  int v10; // ST0C_4
  idLexer *v11; // eax
  idBitMsg **v12; // edi
  int v13; // ST0C_4
  idToken token; // [esp+4h] [ebp-60h]
  __int16 v15; // [esp+54h] [ebp-10h]
  int v16; // [esp+60h] [ebp-4h]

  *(long double *)((char *)&token.floatvalue + 4) = 0.0;
  v3 = 0;
  v4 = this;
  token.data = 0;
  *(_DWORD *)token.baseBuffer = -20;
  token.alloced = (int)&token.baseBuffer[4];
  token.baseBuffer[4] = 0;
  token.subtype = 0;
  token.line = 0;
  token.linesCrossed = 0;
  token.flags = 0;
  token.intvalue = 0;
  LODWORD(token.floatvalue) = 0;
  token.whiteSpaceEnd_p = 0;
  token.next = 0;
  *(_DWORD *)&token.binaryIndex = 0;
  v15 = 0;
  v16 = 0;
  if ( !idParser::ExpectTokenString(src, "{", 0) )
  {
    v16 = -1;
    idStr::FreeData((idStr *)&token.data);
    return 0;
  }
  v4->mFlags |= 0x200u;
  if ( !idParser::ReadToken(src, (idToken *)((char *)&token + 4)) )
  {
LABEL_30:
    v16 = -1;
    idStr::FreeData((idStr *)&token.data);
    return 0;
  }
  while ( idStr::Cmp((const char *)token.alloced, "}") )
  {
    if ( idStr::Icmp((const char *)token.alloced, "effect") )
    {
      if ( !idStr::Icmp((const char *)token.alloced, "remove") )
      {
        if ( idParser::ParseInt(src) != 0 )
          v4->mFlags |= 0x400u;
        else
          v4->mFlags &= 0xFFFFFBFF;
        goto LABEL_29;
      }
      if ( idStr::Icmp((const char *)token.alloced, "bounce") )
      {
        if ( idStr::Icmp((const char *)token.alloced, "physicsDistance") )
        {
          v11 = src->scriptstack;
          if ( v11 )
            v3 = v11->line;
          if ( v11 )
            v12 = (idBitMsg **)v11->filename.data;
          else
            v12 = &s2;
          v13 = (*(int (**)(void))effect->base->vfptr->gap4)();
          (*(void (__cdecl **)(netadrtype_t, const char *, int, int, idBitMsg **, int))(*(_DWORD *)common.type + 68))(
            common.type,
            "^4BSE:^1 Invalid impact parameter '%s' in '%s' (file: %s, line: %d)",
            token.alloced,
            v13,
            v12,
            v3);
          goto LABEL_28;
        }
        v4->mPhysicsDistance = idParser::ParseFloat(src, 0);
      }
      else
      {
        v4->mBounce = idParser::ParseFloat(src, 0);
      }
    }
    else
    {
      idParser::ReadToken(src, (idToken *)((char *)&token + 4));
      if ( v4->mNumImpactEffects >= 4 )
      {
        v8 = src->scriptstack;
        if ( v8 )
          v3 = v8->line;
        if ( v8 )
          v9 = (idBitMsg **)v8->filename.data;
        else
          v9 = &s2;
        v10 = (*(int (**)(void))effect->base->vfptr->gap4)();
        (*(void (__cdecl **)(netadrtype_t, const char *, int, int, idBitMsg **, int))(*(_DWORD *)common.type + 68))(
          common.type,
          "^4BSE:^1 too many impact effects '%s' in '%s' (file: %s, line: %d)",
          token.alloced,
          v10,
          v9,
          v3);
LABEL_28:
        v3 = 0;
        goto LABEL_29;
      }
      v6 = token.alloced;
      v7 = sdSingleton<sdDeclTypeHolder>::GetInstance();
      v4->mImpactEffects[v4->mNumImpactEffects++] = (rvDeclEffect *)((int (__stdcall *)(int, int, signed int))declManager->vfptr->FindType)(
                                                                      v7->declEffectsType.declTypeHandle,
                                                                      v6,
                                                                      1);
    }
LABEL_29:
    if ( !idParser::ReadToken(src, (idToken *)((char *)&token + 4)) )
      goto LABEL_30;
  }
  v16 = -1;
  idStr::FreeData((idStr *)&token.data);
  return 1;
}
char __thiscall rvParticleTemplate::ParseTimeout(rvParticleTemplate *this, rvDeclEffect *effect, idParser *src)
{
  rvParticleTemplate *v3; // edi
  idParser *v4; // ebp
  char result; // al
  int v6; // esi
  sdDeclTypeHolder *v7; // eax
  idLexer *v8; // eax
  int v9; // ebp
  idBitMsg **v10; // esi
  int v11; // ST0C_4
  idLexer *v12; // eax
  int v13; // ebp
  idBitMsg **v14; // esi
  int v15; // ST0C_4
  idToken token; // [esp+4h] [ebp-60h]
  __int16 v17; // [esp+54h] [ebp-10h]
  int v18; // [esp+60h] [ebp-4h]

  *(long double *)((char *)&token.floatvalue + 4) = 0.0;
  v3 = this;
  token.data = 0;
  *(_DWORD *)token.baseBuffer = -20;
  token.alloced = (int)&token.baseBuffer[4];
  token.baseBuffer[4] = 0;
  token.subtype = 0;
  token.line = 0;
  token.linesCrossed = 0;
  token.flags = 0;
  token.intvalue = 0;
  LODWORD(token.floatvalue) = 0;
  token.whiteSpaceEnd_p = 0;
  token.next = 0;
  *(_DWORD *)&token.binaryIndex = 0;
  v17 = 0;
  v4 = src;
  v18 = 0;
  if ( idParser::ExpectTokenString(src, "{", 0) )
  {
    if ( idParser::ReadToken(src, (idToken *)((char *)&token + 4)) )
    {
      while ( idStr::Cmp((const char *)token.alloced, "}") )
      {
        if ( idStr::Icmp((const char *)token.alloced, "effect") )
        {
          v12 = v4->scriptstack;
          if ( v12 )
            v13 = v12->line;
          else
            v13 = 0;
          if ( v12 )
            v14 = (idBitMsg **)v12->filename.data;
          else
            v14 = &s2;
          v15 = (*(int (**)(void))effect->base->vfptr->gap4)();
          (*(void (__cdecl **)(netadrtype_t, const char *, int, int, idBitMsg **, int))(*(_DWORD *)common.type + 68))(
            common.type,
            "^4BSE:^1 Invalid timeout parameter '%s' in '%s' (file: %s, line: %d)",
            token.alloced,
            v15,
            v14,
            v13);
        }
        else
        {
          idParser::ReadToken(v4, (idToken *)((char *)&token + 4));
          if ( v3->mNumTimeoutEffects >= 4 )
          {
            v8 = v4->scriptstack;
            if ( v8 )
              v9 = v8->line;
            else
              v9 = 0;
            if ( v8 )
              v10 = (idBitMsg **)v8->filename.data;
            else
              v10 = &s2;
            v11 = (*(int (**)(void))effect->base->vfptr->gap4)();
            (*(void (__cdecl **)(netadrtype_t, const char *, int, int, idBitMsg **, int))(*(_DWORD *)common.type + 68))(
              common.type,
              "^4BSE:^1 Too many timeout effects '%s' in '%s' (file: %s, line: %d)",
              token.alloced,
              v11,
              v10,
              v9);
          }
          else
          {
            v6 = token.alloced;
            v7 = sdSingleton<sdDeclTypeHolder>::GetInstance();
            v3->mTimeoutEffects[v3->mNumTimeoutEffects++] = (rvDeclEffect *)((int (__stdcall *)(int, int, signed int))declManager->vfptr->FindType)(
                                                                              v7->declEffectsType.declTypeHandle,
                                                                              v6,
                                                                              1);
          }
        }
        if ( !idParser::ReadToken(src, (idToken *)((char *)&token + 4)) )
          goto LABEL_25;
        v4 = src;
      }
      v18 = -1;
      idStr::FreeData((idStr *)&token.data);
      result = 1;
    }
    else
    {
LABEL_25:
      v18 = -1;
      idStr::FreeData((idStr *)&token.data);
      result = 0;
    }
  }
  else
  {
    v18 = -1;
    idStr::FreeData((idStr *)&token.data);
    result = 0;
  }
  return result;
}
void __thiscall sdDynamicBlockManager<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128>::sdDynamicBlockManager<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128>(sdDynamicBlockManager<rvEnvParms,&sdPoolAllocator_rvEnvParms,128> *this)
{
  sdDynamicBlockManager<rvEnvParms,&sdPoolAllocator_rvEnvParms,128> *v1; // edi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v2; // eax
  idList<sdDynamicBlockManagerBase *> *v3; // eax
  idList<sdDynamicBlockManagerBase *> *v4; // esi
  int v5; // eax
  bool v6; // sf
  unsigned __int8 v7; // of
  int v8; // ebx
  int v9; // eax
  int v10; // eax
  sdDynamicBlockManagerBase **v11; // edi
  int i; // eax
  sdDynamicBlockManager<rvEnvParms,&sdPoolAllocator_rvEnvParms,128> *v13; // [esp+10h] [ebp-10h]

  v1 = this;
  v13 = this;
  this->vfptr = (sdDynamicBlockManagerBaseVtbl *)&sdDynamicBlockManager<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128>::`vftable';
  v2 = (sdDetails::sdPoolAlloc<rvEnvParms,128> *)operator new(0x14u);
  if ( v2 )
  {
    v2->blocks = 0;
    v2->free = 0;
    v2->numFree = 0;
    v2->active = 0;
    v2->total = 0;
  }
  else
  {
    v2 = 0;
  }
  v1->allocator = v2;
  v3 = sdDynamicBlockManagerBase::GetList();
  v4 = v3;
  if ( !v3->list )
  {
    v5 = v3->granularity;
    if ( v5 > 0 )
    {
      if ( v5 != v4->size )
      {
        v7 = __OFSUB__(v5, v4->num);
        v6 = v5 - v4->num < 0;
        v4->size = v5;
        if ( v6 ^ v7 )
          v4->num = v5;
        v4->list = (sdDynamicBlockManagerBase **)operator new(4 * v5);
      }
    }
    else
    {
      operator delete(0);
      v4->list = 0;
      v4->num = 0;
      v4->size = 0;
    }
  }
  v8 = v4->num;
  v9 = v4->size;
  if ( v4->num == v9 )
  {
    if ( !v4->granularity )
      v4->granularity = 16;
    v10 = v9 + v4->granularity - (v9 + v4->granularity) % v4->granularity;
    if ( v10 > 0 )
    {
      if ( v10 != v4->size )
      {
        v11 = v4->list;
        v4->size = v10;
        if ( v10 < v8 )
          v4->num = v10;
        v4->list = (sdDynamicBlockManagerBase **)operator new(4 * v10);
        if ( v11 )
        {
          for ( i = 0; i < v4->num; ++i )
            v4->list[i] = v11[i];
          operator delete(v11);
        }
      }
    }
    else
    {
      operator delete(v4->list);
      v4->list = 0;
      v4->num = 0;
      v4->size = 0;
    }
    v1 = v13;
  }
  v4->list[v4->num++] = (sdDynamicBlockManagerBase *)&v1->vfptr;
}
const char *sdDynamicBlockManager<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128>::GetName()
{
  return "rvEnvParms";
}
void __thiscall sdDynamicBlockManager<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128>::PrintInfo(sdDynamicBlockManager<rvEnvParms,&sdPoolAllocator_rvEnvParms,128> *this)
{
  sdDynamicBlockManager<rvEnvParms,&sdPoolAllocator_rvEnvParms,128> *v1; // esi

  v1 = this;
  idLib::Printf("\n%s\n", "rvEnvParms");
  idLib::Printf("===================\n", "rvEnvParms");
  idLib::Printf(
    "Base Block Memory %i bytes free, %i bytes total\n",
    32 * (v1->allocator->total - v1->allocator->active),
    32 * v1->allocator->total);
}
void __thiscall sdDynamicBlockManager<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128>::Purge(sdDynamicBlockManager<rvEnvParms,&sdPoolAllocator_rvEnvParms,128> *this)
{
  idBlockAlloc<idBTreeNode<idDynamicBlock<wchar_t>,int>,128,0> *v1; // ecx

  v1 = (idBlockAlloc<idBTreeNode<idDynamicBlock<wchar_t>,int>,128,0> *)this->allocator;
  if ( v1 )
    idBlockAlloc<idBTreeNode<idDynamicBlock<shadowCache_s>,int>,128,0>::Shutdown(v1);
}
int __thiscall sdDetails::sdPoolAlloc<rvEnvParms,128>::Compact(sdDetails::sdPoolAlloc<rvEnvParms,128> *this)
{
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v1; // ebx
  int v2; // eax
  int v3; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128>::block_t **v4; // eax
  void *v5; // esp
  sdDetails::sdPoolAlloc<rvEnvParms,128>::block_t *v6; // edi
  bool v7; // zf
  signed int v8; // edx
  sdDetails::sdPoolAlloc<rvEnvParms,128>::block_t *v9; // ecx
  sdDetails::sdPoolAlloc<rvEnvParms,128>::element_t *v10; // eax
  sdDetails::sdPoolAlloc<rvEnvParms,128>::block_t *v11; // esi
  signed int v12; // edi
  sdDetails::sdPoolAlloc<rvEnvParms,128>::element_t *v13; // eax
  sdDetails::sdPoolAlloc<rvEnvParms,128>::element_t *v14; // edx
  sdDetails::sdPoolAlloc<rvEnvParms,128>::element_t *v15; // ecx
  int v16; // eax
  sdDetails::sdPoolAlloc<rvEnvParms,128>::block_t *v17; // ecx
  int v18; // edi
  sdDetails::sdPoolAlloc<rvEnvParms,128>::block_t *v19; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128>::block_t *v20; // eax
  sdDetails::sdPoolAlloc<rvEnvParms,128>::block_t *v21; // edx
  sdDetails::sdPoolAlloc<rvEnvParms,128>::block_t *v22; // ecx
  sdDetails::sdPoolAlloc<rvEnvParms,128>::block_t **blocksToUnlink; // [esp+Ch] [ebp-10h]
  sdDetails::sdPoolAlloc<rvEnvParms,128>::block_t *block; // [esp+10h] [ebp-Ch]
  int blocksToUnlinkNum; // [esp+14h] [ebp-8h]
  bool usedHeap; // [esp+1Ah] [ebp-2h]
  bool allFreed; // [esp+1Bh] [ebp-1h]

  v1 = this;
  v2 = this->numFree;
  v3 = 0;
  allFreed = 1;
  usedHeap = 0;
  blocksToUnlinkNum = 0;
  if ( v2 < 0x80000 )
  {
    v5 = alloca(v2 + 15);
    v4 = (sdDetails::sdPoolAlloc<rvEnvParms,128>::block_t **)(((unsigned int)&blocksToUnlink + 3) & 0xFFFFFFF0);
  }
  else
  {
    usedHeap = 1;
    v4 = (sdDetails::sdPoolAlloc<rvEnvParms,128>::block_t **)Mem_AllocAligned(v2, ALIGN_16);
  }
  v6 = v1->blocks;
  v7 = v1->blocks == 0;
  blocksToUnlink = v4;
  block = v6;
  if ( !v7 )
  {
    while ( 2 )
    {
      v8 = 0;
      v9 = v6;
      do
      {
        v10 = v1->free;
        if ( !v1->free )
        {
LABEL_11:
          allFreed = 0;
          goto LABEL_24;
        }
        while ( v10 != (sdDetails::sdPoolAlloc<rvEnvParms,128>::element_t *)v9 )
        {
          v10 = v10->next;
          if ( !v10 )
            goto LABEL_11;
        }
        ++v8;
        v9 = (sdDetails::sdPoolAlloc<rvEnvParms,128>::block_t *)((char *)v9 + 36);
      }
      while ( v8 < 128 );
      if ( allFreed )
      {
        v11 = v6;
        v12 = 128;
        do
        {
          v13 = v1->free;
          v14 = 0;
          if ( v13 )
          {
            while ( 1 )
            {
              v15 = v13->next;
              if ( v13 == (sdDetails::sdPoolAlloc<rvEnvParms,128>::element_t *)v11 )
                break;
              v14 = v13;
              v13 = v13->next;
              if ( !v15 )
                goto LABEL_22;
            }
            if ( v14 )
              v14->next = v15;
            else
              v1->free = v15;
          }
LABEL_22:
          v11 = (sdDetails::sdPoolAlloc<rvEnvParms,128>::block_t *)((char *)v11 + 36);
          --v12;
        }
        while ( v12 );
        v16 = blocksToUnlinkNum;
        v17 = block;
        blocksToUnlink[blocksToUnlinkNum] = block;
        blocksToUnlinkNum = v16 + 1;
        v6 = v17;
      }
LABEL_24:
      block = v6->next;
      if ( block )
      {
        v6 = block;
        continue;
      }
      break;
    }
    v3 = blocksToUnlinkNum;
  }
  v18 = 0;
  if ( v3 > 0 )
  {
    do
    {
      v19 = blocksToUnlink[v18];
      v20 = v1->blocks;
      v21 = 0;
      if ( v1->blocks )
      {
        while ( 1 )
        {
          v22 = v20->next;
          if ( v20 == v19 )
            break;
          v21 = v20;
          v20 = v20->next;
          if ( !v22 )
            goto LABEL_34;
        }
        if ( v21 )
          v21->next = v22;
        else
          v1->blocks = v22;
      }
LABEL_34:
      operator delete(v19);
      ++v18;
    }
    while ( v18 < blocksToUnlinkNum );
    v3 = blocksToUnlinkNum;
  }
  if ( usedHeap )
    Mem_FreeAligned(blocksToUnlink);
  v1->numFree -= v3;
  v1->total += -128 * v3;
  return v3 << 12;
}
int __thiscall sdDynamicBlockManager<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128>::Compact(sdDynamicBlockManager<rvEnvParms,&sdPoolAllocator_rvEnvParms,128> *this)
{
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v1; // ecx
  int result; // eax

  v1 = this->allocator;
  if ( v1 )
    result = sdDetails::sdPoolAlloc<rvEnvParms,128>::Compact(v1);
  else
    result = 0;
  return result;
}
void __thiscall sdDynamicBlockManager<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128>::Shutdown(sdDynamicBlockManager<rvEnvParms,&sdPoolAllocator_rvEnvParms,128> *this)
{
  sdDynamicBlockManager<rvEnvParms,&sdPoolAllocator_rvEnvParms,128> *v1; // edi
  idBlockAlloc<idBTreeNode<idDynamicBlock<wchar_t>,int>,128,0> *v2; // ecx
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v3; // esi
  idList<sdDynamicBlockManagerBase *> *v4; // eax
  int v5; // esi
  int v6; // ecx
  sdDynamicBlockManagerBase **v7; // edx
  sdDynamicBlockManagerBase **v8; // edx
  sdDynamicBlockManagerBase *v9; // esi
  sdDynamicBlockManagerBase **v10; // edx

  v1 = this;
  v2 = (idBlockAlloc<idBTreeNode<idDynamicBlock<wchar_t>,int>,128,0> *)this->allocator;
  if ( v2 )
  {
    idBlockAlloc<idBTreeNode<idDynamicBlock<shadowCache_s>,int>,128,0>::Shutdown(v2);
    v3 = v1->allocator;
    if ( v3 )
    {
      idBlockAlloc<idBTreeNode<idDynamicBlock<shadowCache_s>,int>,128,0>::Shutdown((idBlockAlloc<idBTreeNode<idDynamicBlock<wchar_t>,int>,128,0> *)v1->allocator);
      operator delete(v3);
    }
    v1->allocator = 0;
  }
  v4 = sdDynamicBlockManagerBase::GetList();
  v5 = v4->num;
  v6 = 0;
  if ( v4->num > 0 )
  {
    v7 = v4->list;
    while ( *v7 != (sdDynamicBlockManagerBase *)v1 )
    {
      ++v6;
      ++v7;
      if ( v6 >= v5 )
        return;
    }
    if ( v6 >= 0 && v6 < v5 )
    {
      for ( v4->num = v5 - 1; v6 < v4->num; *v10 = v9 )
      {
        v8 = v4->list;
        v9 = v8[v6 + 1];
        v10 = &v8[v6++];
      }
    }
  }
}
void __thiscall sdDynamicBlockManager<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128>::sdDynamicBlockManager<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128>(sdDynamicBlockManager<rvParticleParms,&sdPoolAllocator_rvParticleParms,128> *this)
{
  sdDynamicBlockManager<rvParticleParms,&sdPoolAllocator_rvParticleParms,128> *v1; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v2; // eax
  idList<sdDynamicBlockManagerBase *> *v3; // eax
  idList<sdDynamicBlockManagerBase *> *v4; // esi
  int v5; // eax
  bool v6; // sf
  unsigned __int8 v7; // of
  int v8; // ebx
  int v9; // eax
  int v10; // eax
  sdDynamicBlockManagerBase **v11; // edi
  int i; // eax
  sdDynamicBlockManager<rvParticleParms,&sdPoolAllocator_rvParticleParms,128> *v13; // [esp+10h] [ebp-10h]

  v1 = this;
  v13 = this;
  this->vfptr = (sdDynamicBlockManagerBaseVtbl *)&sdDynamicBlockManager<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128>::`vftable';
  v2 = (sdDetails::sdPoolAlloc<rvParticleParms,128> *)operator new(0x14u);
  if ( v2 )
  {
    v2->blocks = 0;
    v2->free = 0;
    v2->numFree = 0;
    v2->active = 0;
    v2->total = 0;
  }
  else
  {
    v2 = 0;
  }
  v1->allocator = v2;
  v3 = sdDynamicBlockManagerBase::GetList();
  v4 = v3;
  if ( !v3->list )
  {
    v5 = v3->granularity;
    if ( v5 > 0 )
    {
      if ( v5 != v4->size )
      {
        v7 = __OFSUB__(v5, v4->num);
        v6 = v5 - v4->num < 0;
        v4->size = v5;
        if ( v6 ^ v7 )
          v4->num = v5;
        v4->list = (sdDynamicBlockManagerBase **)operator new(4 * v5);
      }
    }
    else
    {
      operator delete(0);
      v4->list = 0;
      v4->num = 0;
      v4->size = 0;
    }
  }
  v8 = v4->num;
  v9 = v4->size;
  if ( v4->num == v9 )
  {
    if ( !v4->granularity )
      v4->granularity = 16;
    v10 = v9 + v4->granularity - (v9 + v4->granularity) % v4->granularity;
    if ( v10 > 0 )
    {
      if ( v10 != v4->size )
      {
        v11 = v4->list;
        v4->size = v10;
        if ( v10 < v8 )
          v4->num = v10;
        v4->list = (sdDynamicBlockManagerBase **)operator new(4 * v10);
        if ( v11 )
        {
          for ( i = 0; i < v4->num; ++i )
            v4->list[i] = v11[i];
          operator delete(v11);
        }
      }
    }
    else
    {
      operator delete(v4->list);
      v4->list = 0;
      v4->num = 0;
      v4->size = 0;
    }
    v1 = v13;
  }
  v4->list[v4->num++] = (sdDynamicBlockManagerBase *)&v1->vfptr;
}
const char *sdDynamicBlockManager<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128>::GetName()
{
  return "rvParticleParms";
}
void __thiscall sdDynamicBlockManager<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128>::PrintInfo(sdDynamicBlockManager<rvParticleParms,&sdPoolAllocator_rvParticleParms,128> *this)
{
  sdDynamicBlockManager<rvParticleParms,&sdPoolAllocator_rvParticleParms,128> *v1; // esi

  v1 = this;
  idLib::Printf("\n%s\n", "rvParticleParms");
  idLib::Printf("===================\n", "rvParticleParms");
  idLib::Printf(
    "Base Block Memory %i bytes free, %i bytes total\n",
    36 * (v1->allocator->total - v1->allocator->active),
    36 * v1->allocator->total);
}
void __thiscall sdDetails::sdPoolAlloc<rvParticleParms,128>::Shutdown(sdDetails::sdPoolAlloc<rvParticleParms,128> *this)
{
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v1; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128>::block_t *v2; // ST00_4

  v1 = this;
  while ( v1->blocks )
  {
    v2 = v1->blocks;
    v1->blocks = v1->blocks->next;
    operator delete(v2);
  }
  v1->blocks = 0;
  v1->free = 0;
  v1->active = 0;
  v1->total = 0;
}
void __thiscall sdDynamicBlockManager<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128>::Purge(sdDynamicBlockManager<rvParticleParms,&sdPoolAllocator_rvParticleParms,128> *this)
{
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v1; // ecx

  v1 = this->allocator;
  if ( v1 )
    sdDetails::sdPoolAlloc<rvParticleParms,128>::Shutdown(v1);
}
int __thiscall sdDetails::sdPoolAlloc<rvParticleParms,128>::Compact(sdDetails::sdPoolAlloc<rvParticleParms,128> *this)
{
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v1; // ebx
  int v2; // eax
  int v3; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128>::block_t **v4; // eax
  void *v5; // esp
  sdDetails::sdPoolAlloc<rvParticleParms,128>::block_t *v6; // edi
  bool v7; // zf
  signed int v8; // edx
  sdDetails::sdPoolAlloc<rvParticleParms,128>::block_t *v9; // ecx
  sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *v10; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128>::block_t *v11; // esi
  signed int v12; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *v13; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *v14; // edx
  sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *v15; // ecx
  int v16; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128>::block_t *v17; // ecx
  int v18; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128>::block_t *v19; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128>::block_t *v20; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128>::block_t *v21; // edx
  sdDetails::sdPoolAlloc<rvParticleParms,128>::block_t *v22; // ecx
  sdDetails::sdPoolAlloc<rvParticleParms,128>::block_t **blocksToUnlink; // [esp+Ch] [ebp-10h]
  sdDetails::sdPoolAlloc<rvParticleParms,128>::block_t *block; // [esp+10h] [ebp-Ch]
  int blocksToUnlinkNum; // [esp+14h] [ebp-8h]
  bool usedHeap; // [esp+1Ah] [ebp-2h]
  bool allFreed; // [esp+1Bh] [ebp-1h]

  v1 = this;
  v2 = this->numFree;
  v3 = 0;
  allFreed = 1;
  usedHeap = 0;
  blocksToUnlinkNum = 0;
  if ( v2 < 0x80000 )
  {
    v5 = alloca(v2 + 15);
    v4 = (sdDetails::sdPoolAlloc<rvParticleParms,128>::block_t **)(((unsigned int)&blocksToUnlink + 3) & 0xFFFFFFF0);
  }
  else
  {
    usedHeap = 1;
    v4 = (sdDetails::sdPoolAlloc<rvParticleParms,128>::block_t **)Mem_AllocAligned(v2, ALIGN_16);
  }
  v6 = v1->blocks;
  v7 = v1->blocks == 0;
  blocksToUnlink = v4;
  block = v6;
  if ( !v7 )
  {
    while ( 2 )
    {
      v8 = 0;
      v9 = v6;
      do
      {
        v10 = v1->free;
        if ( !v1->free )
        {
LABEL_11:
          allFreed = 0;
          goto LABEL_24;
        }
        while ( v10 != (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)v9 )
        {
          v10 = v10->next;
          if ( !v10 )
            goto LABEL_11;
        }
        ++v8;
        v9 = (sdDetails::sdPoolAlloc<rvParticleParms,128>::block_t *)((char *)v9 + 40);
      }
      while ( v8 < 128 );
      if ( allFreed )
      {
        v11 = v6;
        v12 = 128;
        do
        {
          v13 = v1->free;
          v14 = 0;
          if ( v13 )
          {
            while ( 1 )
            {
              v15 = v13->next;
              if ( v13 == (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)v11 )
                break;
              v14 = v13;
              v13 = v13->next;
              if ( !v15 )
                goto LABEL_22;
            }
            if ( v14 )
              v14->next = v15;
            else
              v1->free = v15;
          }
LABEL_22:
          v11 = (sdDetails::sdPoolAlloc<rvParticleParms,128>::block_t *)((char *)v11 + 40);
          --v12;
        }
        while ( v12 );
        v16 = blocksToUnlinkNum;
        v17 = block;
        blocksToUnlink[blocksToUnlinkNum] = block;
        blocksToUnlinkNum = v16 + 1;
        v6 = v17;
      }
LABEL_24:
      block = v6->next;
      if ( block )
      {
        v6 = block;
        continue;
      }
      break;
    }
    v3 = blocksToUnlinkNum;
  }
  v18 = 0;
  if ( v3 > 0 )
  {
    do
    {
      v19 = blocksToUnlink[v18];
      v20 = v1->blocks;
      v21 = 0;
      if ( v1->blocks )
      {
        while ( 1 )
        {
          v22 = v20->next;
          if ( v20 == v19 )
            break;
          v21 = v20;
          v20 = v20->next;
          if ( !v22 )
            goto LABEL_34;
        }
        if ( v21 )
          v21->next = v22;
        else
          v1->blocks = v22;
      }
LABEL_34:
      operator delete(v19);
      ++v18;
    }
    while ( v18 < blocksToUnlinkNum );
    v3 = blocksToUnlinkNum;
  }
  if ( usedHeap )
    Mem_FreeAligned(blocksToUnlink);
  v1->numFree -= v3;
  v1->total += -128 * v3;
  return 4608 * v3;
}
int __thiscall sdDynamicBlockManager<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128>::Compact(sdDynamicBlockManager<rvParticleParms,&sdPoolAllocator_rvParticleParms,128> *this)
{
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v1; // ecx
  int result; // eax

  v1 = this->allocator;
  if ( v1 )
    result = sdDetails::sdPoolAlloc<rvParticleParms,128>::Compact(v1);
  else
    result = 0;
  return result;
}
void __thiscall sdDynamicBlockManager<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128>::Shutdown(sdDynamicBlockManager<rvParticleParms,&sdPoolAllocator_rvParticleParms,128> *this)
{
  sdDynamicBlockManager<rvParticleParms,&sdPoolAllocator_rvParticleParms,128> *v1; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v2; // ecx
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v3; // esi
  idList<sdDynamicBlockManagerBase *> *v4; // eax
  int v5; // esi
  int v6; // ecx
  sdDynamicBlockManagerBase **v7; // edx
  sdDynamicBlockManagerBase **v8; // edx
  sdDynamicBlockManagerBase *v9; // esi
  sdDynamicBlockManagerBase **v10; // edx

  v1 = this;
  v2 = this->allocator;
  if ( v2 )
  {
    sdDetails::sdPoolAlloc<rvParticleParms,128>::Shutdown(v2);
    v3 = v1->allocator;
    if ( v3 )
    {
      sdDetails::sdPoolAlloc<rvParticleParms,128>::Shutdown(v1->allocator);
      operator delete(v3);
    }
    v1->allocator = 0;
  }
  v4 = sdDynamicBlockManagerBase::GetList();
  v5 = v4->num;
  v6 = 0;
  if ( v4->num > 0 )
  {
    v7 = v4->list;
    while ( *v7 != (sdDynamicBlockManagerBase *)v1 )
    {
      ++v6;
      ++v7;
      if ( v6 >= v5 )
        return;
    }
    if ( v6 >= 0 && v6 < v5 )
    {
      for ( v4->num = v5 - 1; v6 < v4->num; *v10 = v9 )
      {
        v8 = v4->list;
        v9 = v8[v6 + 1];
        v10 = &v8[v6++];
      }
    }
  }
}
sdDynamicBlockManager<rvEnvParms,&sdPoolAllocator_rvEnvParms,128> *__cdecl sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()
{
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v0; // eax

  if ( !(`sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager'::`2'::`local static guard' & 1) )
  {
    `sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager'::`2'::`local static guard' |= 1u;
    sdDynamicBlockManager<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128>::sdDynamicBlockManager<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128>(&`sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager'::`2'::manager);
    atexit(`sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager'::`2'::`dynamic atexit destructor for 'manager'');
  }
  if ( !`sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager'::`2'::manager.allocator )
  {
    v0 = (sdDetails::sdPoolAlloc<rvEnvParms,128> *)operator new(0x14u);
    if ( v0 )
    {
      v0->blocks = 0;
      v0->free = 0;
      v0->numFree = 0;
      v0->active = 0;
      v0->total = 0;
    }
    else
    {
      v0 = 0;
    }
    `sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager'::`2'::manager.allocator = v0;
  }
  return &`sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager'::`2'::manager;
}
sdDynamicBlockManager<rvParticleParms,&sdPoolAllocator_rvParticleParms,128> *__cdecl sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()
{
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v0; // eax

  if ( !(`sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager'::`2'::`local static guard' & 1) )
  {
    `sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager'::`2'::`local static guard' |= 1u;
    sdDynamicBlockManager<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128>::sdDynamicBlockManager<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128>(&`sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager'::`2'::manager);
    atexit(`sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager'::`2'::`dynamic atexit destructor for 'manager'');
  }
  if ( !`sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager'::`2'::manager.allocator )
  {
    v0 = (sdDetails::sdPoolAlloc<rvParticleParms,128> *)operator new(0x14u);
    if ( v0 )
    {
      v0->blocks = 0;
      v0->free = 0;
      v0->numFree = 0;
      v0->active = 0;
      v0->total = 0;
    }
    else
    {
      v0 = 0;
    }
    `sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager'::`2'::manager.allocator = v0;
  }
  return &`sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager'::`2'::manager;
}
rvEnvParms *__stdcall rvParticleTemplate::ParseMotionParms(idParser *src, int count, rvEnvParms *def)
{
  rvEnvParms *result; // eax
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v4; // ecx
  rvEnvParms *v5; // eax
  rvEnvParms *v6; // esi
  int v7; // ebp
  sdDeclTypeHolder *v8; // eax
  idLexer *v9; // eax
  int v10; // edx
  idBitMsg **v11; // ecx
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v12; // eax
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v13; // eax
  idToken token; // [esp+4h] [ebp-60h]
  __int16 v15; // [esp+54h] [ebp-10h]
  int v16; // [esp+60h] [ebp-4h]

  *(long double *)((char *)&token.floatvalue + 4) = 0.0;
  token.data = 0;
  *(_DWORD *)token.baseBuffer = -20;
  token.alloced = (int)&token.baseBuffer[4];
  token.baseBuffer[4] = 0;
  token.subtype = 0;
  token.line = 0;
  token.linesCrossed = 0;
  token.flags = 0;
  token.intvalue = 0;
  LODWORD(token.floatvalue) = 0;
  token.whiteSpaceEnd_p = 0;
  token.next = 0;
  *(_DWORD *)&token.binaryIndex = 0;
  v15 = 0;
  v16 = 0;
  if ( idParser::ExpectTokenString(src, "{", 0) )
  {
    v4 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v4 && (v5 = sdDetails::sdPoolAlloc<rvEnvParms,128>::Alloc(v4)) != 0 )
    {
      v5->mStatic = 0;
      v5->mFastLookUp = 0;
      v6 = v5;
    }
    else
    {
      v6 = 0;
    }
    rvEnvParms::Init(v6);
    if ( idParser::ReadToken(src, (idToken *)((char *)&token + 4)) )
    {
      while ( idStr::Cmp((const char *)token.alloced, "}") )
      {
        if ( idStr::Icmp((const char *)token.alloced, "envelope") )
        {
          if ( idStr::Icmp((const char *)token.alloced, "rate") )
          {
            if ( idStr::Icmp((const char *)token.alloced, "count") )
            {
              if ( idStr::Icmp((const char *)token.alloced, "offset") )
              {
                v9 = src->scriptstack;
                if ( v9 )
                  v10 = v9->line;
                else
                  v10 = 0;
                if ( v9 )
                  v11 = (idBitMsg **)v9->filename.data;
                else
                  v11 = &s2;
                (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
                  common.type,
                  "^4BSE:^1 Invalid motion parameter '%s' (file: %s, line: %d)",
                  token.alloced,
                  v11,
                  v10);
                idParser::SkipBracedSection(src, 1);
              }
              else
              {
                rvParticleTemplate::GetVector(src, count, &v6->mEnvOffset);
              }
            }
            else
            {
              rvParticleTemplate::GetVector(src, count, &v6->mRate);
              v6->mIsCount = 1;
            }
          }
          else
          {
            rvParticleTemplate::GetVector(src, count, &v6->mRate);
            v6->mIsCount = 0;
          }
        }
        else
        {
          idParser::ReadToken(src, (idToken *)((char *)&token + 4));
          v7 = token.alloced;
          v8 = sdSingleton<sdDeclTypeHolder>::GetInstance();
          v6->mTable = (idDeclTable *)((int (__stdcall *)(int, int, signed int))declManager->vfptr->FindType)(
                                        v8->declTableType.declTypeHandle,
                                        v7,
                                        1);
        }
        if ( !idParser::ReadToken(src, (idToken *)((char *)&token + 4)) )
          goto LABEL_25;
      }
      rvEnvParms::Finalize(v6);
      if ( rvEnvParms::Compare(v6, def) )
      {
        if ( v6 )
        {
          v13 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
          if ( v13 )
          {
            LODWORD(v6[-1].mRate.z) = v13->free;
            --v13->active;
            ++v13->numFree;
            v13->free = (sdDetails::sdPoolAlloc<rvEnvParms,128>::element_t *)&v6[-1].mRate.z;
          }
        }
        v16 = -1;
        idStr::FreeData((idStr *)&token.data);
        result = def;
      }
      else
      {
        v16 = -1;
        idStr::FreeData((idStr *)&token.data);
        result = v6;
      }
    }
    else
    {
LABEL_25:
      if ( v6 )
      {
        v12 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v12 )
        {
          LODWORD(v6[-1].mRate.z) = v12->free;
          --v12->active;
          ++v12->numFree;
          v12->free = (sdDetails::sdPoolAlloc<rvEnvParms,128>::element_t *)&v6[-1].mRate.z;
        }
      }
      v16 = -1;
      idStr::FreeData((idStr *)&token.data);
      result = def;
    }
  }
  else
  {
    v16 = -1;
    idStr::FreeData((idStr *)&token.data);
    result = def;
  }
  return result;
}
char __thiscall rvParticleTemplate::ParseMotionDomains(rvParticleTemplate *this, rvDeclEffect *effect, idParser *src)
{
  rvParticleTemplate *v3; // esi
  idParser *v4; // edi
  char result; // al
  idLexer *v6; // eax
  idBitMsg **v7; // ebp
  int v8; // eax
  idToken token; // [esp+0h] [ebp-60h]
  __int16 v10; // [esp+50h] [ebp-10h]
  int v11; // [esp+5Ch] [ebp-4h]
  idParser *srca; // [esp+68h] [ebp+8h]

  *(long double *)((char *)&token.floatvalue + 4) = 0.0;
  v3 = this;
  token.data = 0;
  *(_DWORD *)token.baseBuffer = -20;
  token.alloced = (int)&token.baseBuffer[4];
  token.baseBuffer[4] = 0;
  token.subtype = 0;
  token.line = 0;
  token.linesCrossed = 0;
  token.flags = 0;
  token.intvalue = 0;
  LODWORD(token.floatvalue) = 0;
  token.whiteSpaceEnd_p = 0;
  token.next = 0;
  *(_DWORD *)&token.binaryIndex = 0;
  v10 = 0;
  v4 = src;
  v11 = 0;
  if ( idParser::ExpectTokenString(src, "{", 0) )
  {
    if ( idParser::ReadToken(src, (idToken *)((char *)&token + 4)) )
    {
      while ( idStr::Cmp((const char *)token.alloced, "}") )
      {
        if ( idStr::Icmp((const char *)token.alloced, "tint") )
        {
          if ( idStr::Icmp((const char *)token.alloced, "fade") )
          {
            if ( idStr::Icmp((const char *)token.alloced, "size") )
            {
              if ( idStr::Icmp((const char *)token.alloced, "rotate") )
              {
                if ( idStr::Icmp((const char *)token.alloced, "angle") )
                {
                  if ( idStr::Icmp((const char *)token.alloced, "offset") )
                  {
                    if ( idStr::Icmp((const char *)token.alloced, "length") )
                    {
                      v6 = v4->scriptstack;
                      if ( v6 )
                        srca = (idParser *)v6->line;
                      else
                        srca = 0;
                      if ( v6 )
                        v7 = (idBitMsg **)v6->filename.data;
                      else
                        v7 = &s2;
                      v8 = (*(int (**)(void))effect->base->vfptr->gap4)();
                      (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
                        common.type,
                        "^4BSE:^1 Invalid motion domain '%s' in %s (file: %s, line: %d)",
                        token.alloced,
                        v8,
                        v7,
                        srca);
                      idParser::SkipBracedSection(v4, 1);
                    }
                    else
                    {
                      v3->mpLengthEnvelope = rvParticleTemplate::ParseMotionParms(
                                               v4,
                                               3,
                                               &rvParticleTemplate::sDefaultEnvelope);
                    }
                  }
                  else
                  {
                    v3->mpOffsetEnvelope = rvParticleTemplate::ParseMotionParms(
                                             v4,
                                             3,
                                             &rvParticleTemplate::sDefaultEnvelope);
                  }
                }
                else
                {
                  v3->mpAngleEnvelope = rvParticleTemplate::ParseMotionParms(
                                          v4,
                                          3,
                                          &rvParticleTemplate::sDefaultEnvelope);
                }
              }
              else
              {
                v3->mpRotateEnvelope = rvParticleTemplate::ParseMotionParms(
                                         v4,
                                         (unsigned __int8)v3->mNumRotateParms,
                                         &rvParticleTemplate::sDefaultEnvelope);
              }
            }
            else
            {
              v3->mpSizeEnvelope = rvParticleTemplate::ParseMotionParms(
                                     v4,
                                     (unsigned __int8)v3->mNumSizeParms,
                                     &rvParticleTemplate::sDefaultEnvelope);
            }
          }
          else
          {
            v3->mpFadeEnvelope = rvParticleTemplate::ParseMotionParms(v4, 1, &rvParticleTemplate::sDefaultEnvelope);
          }
        }
        else
        {
          v3->mpTintEnvelope = rvParticleTemplate::ParseMotionParms(v4, 3, &rvParticleTemplate::sDefaultEnvelope);
        }
        if ( !idParser::ReadToken(v4, (idToken *)((char *)&token + 4)) )
          goto LABEL_27;
      }
      v11 = -1;
      idStr::FreeData((idStr *)&token.data);
      result = 1;
    }
    else
    {
LABEL_27:
      v11 = -1;
      idStr::FreeData((idStr *)&token.data);
      result = 0;
    }
  }
  else
  {
    v11 = -1;
    idStr::FreeData((idStr *)&token.data);
    result = 0;
  }
  return result;
}
rvParticleParms *__stdcall rvParticleTemplate::ParseSpawnParms(rvDeclEffect *effect, idParser *src, int count, rvParticleParms *def)
{
  idParser *v4; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v6; // ecx
  rvParticleParms *v7; // eax
  rvParticleParms *v8; // edi
  idLexer *v9; // esi
  int v10; // ebx
  idBitMsg **v11; // esi
  int v12; // ST0C_4
  idLexer *v13; // esi
  int v14; // ebx
  idBitMsg **v15; // esi
  int v16; // ST0C_4
  char v17; // bl
  idLexer *v18; // esi
  idBitMsg **v19; // esi
  int v20; // eax
  char v21; // bl
  idLexer *v22; // esi
  idBitMsg **v23; // esi
  int v24; // eax
  char v25; // bl
  idLexer *v26; // esi
  idBitMsg **v27; // esi
  int v28; // eax
  idLexer *v29; // esi
  int v30; // ebx
  idBitMsg **v31; // esi
  int v32; // ST0C_4
  idLexer *v33; // eax
  int v34; // eax
  int *v35; // eax
  idLexer *v36; // esi
  int v37; // ebx
  idBitMsg **v38; // esi
  int v39; // eax
  idToken token; // [esp+8h] [ebp-60h]
  __int16 v41; // [esp+58h] [ebp-10h]
  int v42; // [esp+64h] [ebp-4h]
  idParser *srca; // [esp+70h] [ebp+8h]
  int counta; // [esp+74h] [ebp+Ch]
  int countb; // [esp+74h] [ebp+Ch]
  int countc; // [esp+74h] [ebp+Ch]
  rvParticleParms *defa; // [esp+78h] [ebp+10h]

  *(long double *)((char *)&token.floatvalue + 4) = 0.0;
  token.data = 0;
  *(_DWORD *)token.baseBuffer = -20;
  token.alloced = (int)&token.baseBuffer[4];
  token.baseBuffer[4] = 0;
  token.subtype = 0;
  token.line = 0;
  token.linesCrossed = 0;
  token.flags = 0;
  token.intvalue = 0;
  LODWORD(token.floatvalue) = 0;
  token.whiteSpaceEnd_p = 0;
  token.next = 0;
  *(_DWORD *)&token.binaryIndex = 0;
  v41 = 0;
  v4 = src;
  v42 = 0;
  if ( !idParser::ExpectTokenString(src, "{", 0) )
  {
    v42 = -1;
    idStr::FreeData((idStr *)&token.data);
    return def;
  }
  if ( !idParser::ReadToken(src, (idToken *)((char *)&token + 4)) || !idStr::Cmp((const char *)token.alloced, "}") )
  {
    v42 = -1;
    idStr::FreeData((idStr *)&token.data);
    return def;
  }
  v6 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v6 && (v7 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v6)) != 0 )
  {
    v7->mModelInfo = 0;
    v7->mStatic = 0;
    v8 = v7;
  }
  else
  {
    v8 = 0;
  }
  v8->mModelInfo = 0;
  v8->mRange = 0.0;
  v8->mSpawnType = 0;
  v8->mFlags = 0;
  v8->mMins.z = 0.0;
  v8->mMins.y = 0.0;
  v8->mMins.x = 0.0;
  v8->mMaxs.z = 0.0;
  v8->mMaxs.y = 0.0;
  v8->mMaxs.x = 0.0;
  if ( idStr::Icmp((const char *)token.alloced, "point") )
  {
    if ( idStr::Icmp((const char *)token.alloced, "line") )
    {
      if ( !idStr::Icmp((const char *)token.alloced, "box") )
      {
        v8->mSpawnType = count + 16;
        rvParticleTemplate::GetVector(src, count, &v8->mMins);
        idParser::ExpectTokenString(src, ",", 0);
        v17 = count;
        rvParticleTemplate::GetVector(src, count, &v8->mMaxs);
        if ( !rvParticleTemplate::CheckCommonParms(src, v8) )
        {
          v18 = src->scriptstack;
          if ( v18 )
            counta = v18->line;
          else
            counta = 0;
          if ( v18 )
            v19 = (idBitMsg **)v18->filename.data;
          else
            v19 = &s2;
          v20 = (*(int (**)(void))effect->base->vfptr->gap4)();
          (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
            common.type,
            "^4BSE:^1 Invalid box parameter '%s' in '%s' (file: %s, line: %d)",
            token.alloced,
            v20,
            v19,
            counta);
        }
        if ( v8->mFlags & 1 )
          v8->mSpawnType = v17 + 20;
        rvParticleTemplate::FixupParms(v8);
        goto LABEL_91;
      }
      if ( !idStr::Icmp((const char *)token.alloced, "sphere") )
      {
        v8->mSpawnType = count + 24;
        rvParticleTemplate::GetVector(src, count, &v8->mMins);
        idParser::ExpectTokenString(src, ",", 0);
        v21 = count;
        rvParticleTemplate::GetVector(src, count, &v8->mMaxs);
        if ( !rvParticleTemplate::CheckCommonParms(src, v8) )
        {
          v22 = src->scriptstack;
          if ( v22 )
            countb = v22->line;
          else
            countb = 0;
          if ( v22 )
            v23 = (idBitMsg **)v22->filename.data;
          else
            v23 = &s2;
          v24 = (*(int (**)(void))effect->base->vfptr->gap4)();
          (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
            common.type,
            "^4BSE:^1 Invalid sphere parameter '%s' in '%s' (file: %s, line: %d)",
            token.alloced,
            v24,
            v23,
            countb);
        }
        if ( v8->mFlags & 1 )
          v8->mSpawnType = v21 + 28;
        rvParticleTemplate::FixupParms(v8);
        goto LABEL_91;
      }
      if ( !idStr::Icmp((const char *)token.alloced, "cylinder") )
      {
        v8->mSpawnType = count + 32;
        rvParticleTemplate::GetVector(src, count, &v8->mMins);
        idParser::ExpectTokenString(src, ",", 0);
        v25 = count;
        rvParticleTemplate::GetVector(src, count, &v8->mMaxs);
        if ( !rvParticleTemplate::CheckCommonParms(src, v8) )
        {
          v26 = src->scriptstack;
          if ( v26 )
            countc = v26->line;
          else
            countc = 0;
          if ( v26 )
            v27 = (idBitMsg **)v26->filename.data;
          else
            v27 = &s2;
          v28 = (*(int (**)(void))effect->base->vfptr->gap4)();
          (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
            common.type,
            "^4BSE:^1 Invalid cylinder parameter '%s' in '%s' (file: %s, line: %d)",
            token.alloced,
            v28,
            v27,
            countc);
        }
        if ( v8->mFlags & 1 )
          v8->mSpawnType = v25 + 36;
        rvParticleTemplate::FixupParms(v8);
        goto LABEL_91;
      }
      if ( idStr::Icmp((const char *)token.alloced, "spiral") )
      {
        if ( !idStr::Icmp((const char *)token.alloced, "model") )
        {
          v8->mSpawnType = count + 44;
          idParser::ReadToken(src, (idToken *)((char *)&token + 4));
          token.len = ((int (__stdcall *)(int))renderModelManager->vfptr->FindModel)(token.alloced);
          if ( !(*(int (__thiscall **)(int))(*(_DWORD *)token.len + 100))(token.len) )
          {
            token.len = ((int (__stdcall *)(const char *))renderModelManager->vfptr->FindModel)("_default");
            v33 = src->scriptstack;
            if ( v33 )
              defa = (rvParticleParms *)v33->line;
            else
              defa = 0;
            if ( v33 )
              srca = (idParser *)v33->filename.data;
            else
              srca = (idParser *)&s2;
            v34 = (*(int (**)(void))effect->base->vfptr->gap4)();
            (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
              common.type,
              "^4BSE:^1 No surfaces defined in model '%s' in '%s' (file: %s, line: %d)",
              token.alloced,
              v34,
              srca,
              defa);
          }
          v35 = (int *)operator new(0x2Cu);
          v8->mModelInfo = (sdModelInfo *)v35;
          *v35 = token.len;
          sdModelInfo::CalculateSurfRemap(v8->mModelInfo);
          idParser::ExpectTokenString(v4, ",", 0);
          rvParticleTemplate::GetVector(v4, count, &v8->mMins);
          idParser::ExpectTokenString(v4, ",", 0);
          rvParticleTemplate::GetVector(v4, count, &v8->mMaxs);
          if ( !rvParticleTemplate::CheckCommonParms(v4, v8) )
          {
            v36 = v4->scriptstack;
            if ( v36 )
              v37 = v36->line;
            else
              v37 = 0;
            if ( v36 )
              v38 = (idBitMsg **)v36->filename.data;
            else
              v38 = &s2;
            v39 = (*(int (**)(void))effect->base->vfptr->gap4)();
            (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
              common.type,
              "^4BSE:^1 Invalid model parameter '%s' in '%s' (file: %s, line: %d)",
              token.alloced,
              v39,
              v38,
              v37);
          }
        }
        goto LABEL_91;
      }
      v8->mSpawnType = count + 40;
      rvParticleTemplate::GetVector(src, count, &v8->mMins);
      idParser::ExpectTokenString(src, ",", 0);
      rvParticleTemplate::GetVector(src, count, &v8->mMaxs);
      idParser::ExpectTokenString(src, ",", 0);
      v8->mRange = idParser::ParseFloat(src, 0);
      if ( !rvParticleTemplate::CheckCommonParms(src, v8) )
      {
        v29 = src->scriptstack;
        if ( v29 )
          v30 = v29->line;
        else
          v30 = 0;
        if ( v29 )
          v31 = (idBitMsg **)v29->filename.data;
        else
          v31 = &s2;
        v32 = (*(int (**)(void))effect->base->vfptr->gap4)();
        (*(void (__cdecl **)(netadrtype_t, const char *, int, int, idBitMsg **, int))(*(_DWORD *)common.type + 68))(
          common.type,
          "^4BSE:^1 Invalid spiral parameter '%s' in '%s' (file: %s, line: %d)",
          token.alloced,
          v32,
          v31,
          v30);
      }
    }
    else
    {
      v8->mSpawnType = count + 12;
      rvParticleTemplate::GetVector(src, count, &v8->mMins);
      idParser::ExpectTokenString(src, ",", 0);
      rvParticleTemplate::GetVector(src, count, &v8->mMaxs);
      if ( !rvParticleTemplate::CheckCommonParms(src, v8) )
      {
        v13 = src->scriptstack;
        if ( v13 )
          v14 = v13->line;
        else
          v14 = 0;
        if ( v13 )
          v15 = (idBitMsg **)v13->filename.data;
        else
          v15 = &s2;
        v16 = (*(int (**)(void))effect->base->vfptr->gap4)();
        (*(void (__cdecl **)(netadrtype_t, const char *, int, int, idBitMsg **, int))(*(_DWORD *)common.type + 68))(
          common.type,
          "^4BSE:^1 Invalid line parameter '%s' in '%s' (file: %s, line: %d)",
          token.alloced,
          v16,
          v15,
          v14);
      }
    }
  }
  else
  {
    v8->mSpawnType = count + 8;
    rvParticleTemplate::GetVector(src, count, &v8->mMins);
    if ( !rvParticleTemplate::CheckCommonParms(src, v8) )
    {
      v9 = src->scriptstack;
      if ( v9 )
        v10 = v9->line;
      else
        v10 = 0;
      if ( v9 )
        v11 = (idBitMsg **)v9->filename.data;
      else
        v11 = &s2;
      v12 = (*(int (**)(void))effect->base->vfptr->gap4)();
      (*(void (__cdecl **)(netadrtype_t, const char *, int, int, idBitMsg **, int))(*(_DWORD *)common.type + 68))(
        common.type,
        "^4BSE:^1 Invalid point parameter '%s' in '%s' (file: %s, line: %d)",
        token.alloced,
        v12,
        v11,
        v10);
    }
  }
  rvParticleTemplate::FixupParms(v8);
LABEL_91:
  v42 = -1;
  idStr::FreeData((idStr *)&token.data);
  return v8;
}
char __thiscall rvParticleTemplate::ParseSpawnDomains(rvParticleTemplate *this, rvDeclEffect *effect, idParser *src)
{
  int v3; // ebx
  rvParticleTemplate *v4; // esi
  idParser *v5; // ebp
  char result; // al
  rvParticleParms *v7; // eax
  idLexer *v8; // eax
  int v9; // eax
  idToken token; // [esp+0h] [ebp-60h]
  __int16 v11; // [esp+50h] [ebp-10h]
  int v12; // [esp+5Ch] [ebp-4h]
  idParser *srca; // [esp+68h] [ebp+8h]

  v3 = 0;
  *(long double *)((char *)&token.floatvalue + 4) = 0.0;
  v4 = this;
  token.data = 0;
  *(_DWORD *)token.baseBuffer = -20;
  token.alloced = (int)&token.baseBuffer[4];
  token.baseBuffer[4] = 0;
  token.subtype = 0;
  token.line = 0;
  token.linesCrossed = 0;
  token.flags = 0;
  token.intvalue = 0;
  LODWORD(token.floatvalue) = 0;
  token.whiteSpaceEnd_p = 0;
  token.next = 0;
  *(_DWORD *)&token.binaryIndex = 0;
  v11 = 0;
  v5 = src;
  v12 = 0;
  if ( idParser::ExpectTokenString(src, "{", 0) )
  {
    if ( idParser::ReadToken(src, (idToken *)((char *)&token + 4)) )
    {
      while ( idStr::Cmp((const char *)token.alloced, "}") )
      {
        if ( idStr::Icmp((const char *)token.alloced, "position") )
        {
          if ( idStr::Icmp((const char *)token.alloced, "direction") )
          {
            if ( idStr::Icmp((const char *)token.alloced, "velocity") )
            {
              if ( idStr::Icmp((const char *)token.alloced, "acceleration") )
              {
                if ( idStr::Icmp((const char *)token.alloced, "friction") )
                {
                  if ( idStr::Icmp((const char *)token.alloced, "tint") )
                  {
                    if ( idStr::Icmp((const char *)token.alloced, "fade") )
                    {
                      if ( idStr::Icmp((const char *)token.alloced, "size") )
                      {
                        if ( idStr::Icmp((const char *)token.alloced, "rotate") )
                        {
                          if ( idStr::Icmp((const char *)token.alloced, "angle") )
                          {
                            if ( idStr::Icmp((const char *)token.alloced, "offset") )
                            {
                              if ( idStr::Icmp((const char *)token.alloced, "length") )
                              {
                                if ( idStr::Icmp((const char *)token.alloced, "windStrength") )
                                {
                                  v8 = v5->scriptstack;
                                  if ( v8 )
                                    v3 = v8->line;
                                  if ( v8 )
                                    srca = (idParser *)v8->filename.data;
                                  else
                                    srca = (idParser *)&s2;
                                  v9 = (*(int (**)(void))effect->base->vfptr->gap4)();
                                  (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
                                    common.type,
                                    "^4BSE:^1 Invalid spawn type '%s' in '%s' (file: %s, line: %d)",
                                    token.alloced,
                                    v9,
                                    srca,
                                    v3);
                                  v3 = 0;
                                }
                                else
                                {
                                  v4->mpSpawnWindStrength = rvParticleTemplate::ParseSpawnParms(
                                                              effect,
                                                              v5,
                                                              1,
                                                              &rvParticleTemplate::sSPF_NONE_1);
                                }
                              }
                              else
                              {
                                v4->mpSpawnLength = rvParticleTemplate::ParseSpawnParms(
                                                      effect,
                                                      v5,
                                                      3,
                                                      &rvParticleTemplate::sSPF_NONE_3);
                              }
                            }
                            else
                            {
                              v4->mpSpawnOffset = rvParticleTemplate::ParseSpawnParms(
                                                    effect,
                                                    v5,
                                                    3,
                                                    &rvParticleTemplate::sSPF_NONE_3);
                            }
                          }
                          else
                          {
                            v4->mpSpawnAngle = rvParticleTemplate::ParseSpawnParms(
                                                 effect,
                                                 v5,
                                                 3,
                                                 &rvParticleTemplate::sSPF_NONE_3);
                          }
                        }
                        else
                        {
                          v4->mpSpawnRotate = rvParticleTemplate::ParseSpawnParms(
                                                effect,
                                                v5,
                                                (unsigned __int8)v4->mNumRotateParms,
                                                &rvParticleTemplate::sSPF_NONE_3);
                        }
                      }
                      else
                      {
                        v4->mpSpawnSize = rvParticleTemplate::ParseSpawnParms(
                                            effect,
                                            v5,
                                            (unsigned __int8)v4->mNumSizeParms,
                                            &rvParticleTemplate::sSPF_ONE_3);
                      }
                    }
                    else
                    {
                      v4->mpSpawnFade = rvParticleTemplate::ParseSpawnParms(
                                          effect,
                                          v5,
                                          1,
                                          &rvParticleTemplate::sSPF_ONE_1);
                    }
                  }
                  else
                  {
                    v4->mpSpawnTint = rvParticleTemplate::ParseSpawnParms(
                                        effect,
                                        v5,
                                        3,
                                        &rvParticleTemplate::sSPF_ONE_3);
                  }
                }
                else
                {
                  v4->mpSpawnFriction = rvParticleTemplate::ParseSpawnParms(
                                          effect,
                                          v5,
                                          3,
                                          &rvParticleTemplate::sSPF_NONE_3);
                }
              }
              else
              {
                v4->mpSpawnAcceleration = rvParticleTemplate::ParseSpawnParms(
                                            effect,
                                            v5,
                                            3,
                                            &rvParticleTemplate::sSPF_NONE_3);
              }
            }
            else
            {
              v4->mpSpawnVelocity = rvParticleTemplate::ParseSpawnParms(effect, v5, 3, &rvParticleTemplate::sSPF_NONE_3);
            }
          }
          else
          {
            v7 = rvParticleTemplate::ParseSpawnParms(effect, v5, 3, &rvParticleTemplate::sSPF_NONE_3);
            v4->mFlags |= 0x4000u;
            v4->mpSpawnDirection = v7;
          }
        }
        else
        {
          v4->mpSpawnPosition = rvParticleTemplate::ParseSpawnParms(effect, v5, 3, &rvParticleTemplate::sSPF_NONE_3);
        }
        if ( !idParser::ReadToken(v5, (idToken *)((char *)&token + 4)) )
          goto LABEL_38;
      }
      v12 = -1;
      idStr::FreeData((idStr *)&token.data);
      result = 1;
    }
    else
    {
LABEL_38:
      v12 = -1;
      idStr::FreeData((idStr *)&token.data);
      result = 0;
    }
  }
  else
  {
    v12 = -1;
    idStr::FreeData((idStr *)&token.data);
    result = 0;
  }
  return result;
}
char __thiscall rvParticleTemplate::ParseDeathDomains(rvParticleTemplate *this, rvDeclEffect *effect, idParser *src)
{
  rvParticleTemplate *v3; // esi
  idParser *v4; // ebp
  char result; // al
  rvParticleParms *v6; // eax
  bool v7; // zf
  rvParticleParms *v8; // eax
  rvParticleParms *v9; // eax
  rvParticleParms *v10; // eax
  rvParticleParms *v11; // eax
  rvParticleParms *v12; // eax
  rvParticleParms *v13; // eax
  idLexer *v14; // eax
  int v15; // ebx
  int v16; // eax
  idToken token; // [esp+0h] [ebp-60h]
  __int16 v18; // [esp+50h] [ebp-10h]
  int v19; // [esp+5Ch] [ebp-4h]
  idParser *srca; // [esp+68h] [ebp+8h]

  *(long double *)((char *)&token.floatvalue + 4) = 0.0;
  v3 = this;
  token.data = 0;
  *(_DWORD *)token.baseBuffer = -20;
  token.alloced = (int)&token.baseBuffer[4];
  token.baseBuffer[4] = 0;
  token.subtype = 0;
  token.line = 0;
  token.linesCrossed = 0;
  token.flags = 0;
  token.intvalue = 0;
  LODWORD(token.floatvalue) = 0;
  token.whiteSpaceEnd_p = 0;
  token.next = 0;
  *(_DWORD *)&token.binaryIndex = 0;
  v18 = 0;
  v4 = src;
  v19 = 0;
  if ( idParser::ExpectTokenString(src, "{", 0) )
  {
    if ( idParser::ReadToken(src, (idToken *)((char *)&token + 4)) )
    {
      while ( idStr::Cmp((const char *)token.alloced, "}") )
      {
        if ( idStr::Icmp((const char *)token.alloced, "tint") )
        {
          if ( idStr::Icmp((const char *)token.alloced, "fade") )
          {
            if ( idStr::Icmp((const char *)token.alloced, "size") )
            {
              if ( idStr::Icmp((const char *)token.alloced, "rotate") )
              {
                if ( idStr::Icmp((const char *)token.alloced, "angle") )
                {
                  if ( idStr::Icmp((const char *)token.alloced, "offset") )
                  {
                    if ( idStr::Icmp((const char *)token.alloced, "length") )
                    {
                      v14 = v4->scriptstack;
                      if ( v14 )
                        v15 = v14->line;
                      else
                        v15 = 0;
                      if ( v14 )
                        srca = (idParser *)v14->filename.data;
                      else
                        srca = (idParser *)&s2;
                      v16 = (*(int (**)(void))effect->base->vfptr->gap4)();
                      (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
                        common.type,
                        "^4BSE:^1 Invalid end type '%s' in '%s' (file: %s, line: %d)",
                        token.alloced,
                        v16,
                        srca,
                        v15);
                    }
                    else
                    {
                      v13 = rvParticleTemplate::ParseSpawnParms(effect, v4, 3, &rvParticleTemplate::sSPF_NONE_3);
                      v7 = v3->mpLengthEnvelope == &rvParticleTemplate::sEmptyEnvelope;
                      v3->mpDeathLength = v13;
                      if ( v7 )
                        v3->mpLengthEnvelope = &rvParticleTemplate::sDefaultEnvelope;
                    }
                  }
                  else
                  {
                    v12 = rvParticleTemplate::ParseSpawnParms(effect, v4, 3, &rvParticleTemplate::sSPF_NONE_3);
                    v7 = v3->mpOffsetEnvelope == &rvParticleTemplate::sEmptyEnvelope;
                    v3->mpDeathOffset = v12;
                    if ( v7 )
                      v3->mpOffsetEnvelope = &rvParticleTemplate::sDefaultEnvelope;
                  }
                }
                else
                {
                  v11 = rvParticleTemplate::ParseSpawnParms(effect, v4, 3, &rvParticleTemplate::sSPF_NONE_3);
                  v7 = v3->mpAngleEnvelope == &rvParticleTemplate::sEmptyEnvelope;
                  v3->mpDeathAngle = v11;
                  if ( v7 )
                    v3->mpAngleEnvelope = &rvParticleTemplate::sDefaultEnvelope;
                }
              }
              else
              {
                v10 = rvParticleTemplate::ParseSpawnParms(
                        effect,
                        v4,
                        (unsigned __int8)v3->mNumRotateParms,
                        &rvParticleTemplate::sSPF_NONE_3);
                v7 = v3->mpRotateEnvelope == &rvParticleTemplate::sEmptyEnvelope;
                v3->mpDeathRotate = v10;
                if ( v7 )
                  v3->mpRotateEnvelope = &rvParticleTemplate::sDefaultEnvelope;
              }
            }
            else
            {
              v9 = rvParticleTemplate::ParseSpawnParms(
                     effect,
                     v4,
                     (unsigned __int8)v3->mNumSizeParms,
                     &rvParticleTemplate::sSPF_ONE_3);
              v7 = v3->mpSizeEnvelope == &rvParticleTemplate::sEmptyEnvelope;
              v3->mpDeathSize = v9;
              if ( v7 )
                v3->mpSizeEnvelope = &rvParticleTemplate::sDefaultEnvelope;
            }
          }
          else
          {
            v8 = rvParticleTemplate::ParseSpawnParms(effect, v4, 1, &rvParticleTemplate::sSPF_NONE_1);
            v7 = v3->mpFadeEnvelope == &rvParticleTemplate::sEmptyEnvelope;
            v3->mpDeathFade = v8;
            if ( v7 )
              v3->mpFadeEnvelope = &rvParticleTemplate::sDefaultEnvelope;
          }
        }
        else
        {
          v6 = rvParticleTemplate::ParseSpawnParms(effect, v4, 3, &rvParticleTemplate::sSPF_NONE_3);
          v7 = v3->mpTintEnvelope == &rvParticleTemplate::sEmptyEnvelope;
          v3->mpDeathTint = v6;
          if ( v7 )
            v3->mpTintEnvelope = &rvParticleTemplate::sDefaultEnvelope;
        }
        if ( !idParser::ReadToken(v4, (idToken *)((char *)&token + 4)) )
          goto LABEL_34;
      }
      v19 = -1;
      idStr::FreeData((idStr *)&token.data);
      result = 1;
    }
    else
    {
LABEL_34:
      v19 = -1;
      idStr::FreeData((idStr *)&token.data);
      result = 0;
    }
  }
  else
  {
    v19 = -1;
    idStr::FreeData((idStr *)&token.data);
    result = 0;
  }
  return result;
}
char __thiscall rvParticleTemplate::Parse(rvParticleTemplate *this, rvDeclEffect *effect, idParser *src)
{
  rvParticleTemplate *v3; // esi
  idParser *v4; // edi
  char result; // al
  rvDeclEffect *v6; // ebx
  double v7; // st7
  double v8; // st7
  double v9; // st7
  rvTrailInfo *v10; // ebp
  int v11; // eax
  char *v12; // ebx
  idStr *v13; // ebp
  const char *v14; // ST10_4
  sdDeclTypeHolder *v15; // eax
  float *v16; // ebp
  float *v17; // ebp
  float *v18; // ebp
  float *v19; // ebp
  const char *v20; // ST10_4
  sdDeclTypeHolder *v21; // eax
  char *v22; // ebx
  const char *v23; // ST10_4
  sdDeclTypeHolder *v24; // eax
  rvElectricityInfo *v25; // eax
  const char *v26; // ST10_4
  sdDeclWrapperTemplate<idMaterial> *v27; // eax
  int v28; // eax
  idLexer *v29; // eax
  idBitMsg **v30; // ebp
  int v31; // ST0C_4
  idLexer *v32; // eax
  idBitMsg **v33; // ebp
  int v34; // ST0C_4
  idToken token; // [esp+0h] [ebp-60h]
  __int16 v36; // [esp+50h] [ebp-10h]
  int v37; // [esp+5Ch] [ebp-4h]
  float srca; // [esp+68h] [ebp+8h]
  float srcf; // [esp+68h] [ebp+8h]
  float srcb; // [esp+68h] [ebp+8h]
  float srcg; // [esp+68h] [ebp+8h]
  float srcc; // [esp+68h] [ebp+8h]
  float srch; // [esp+68h] [ebp+8h]
  idParser *srcd; // [esp+68h] [ebp+8h]
  idParser *srce; // [esp+68h] [ebp+8h]

  *(long double *)((char *)&token.floatvalue + 4) = 0.0;
  v3 = this;
  token.data = 0;
  *(_DWORD *)token.baseBuffer = -20;
  token.alloced = (int)&token.baseBuffer[4];
  token.baseBuffer[4] = 0;
  token.subtype = 0;
  token.line = 0;
  token.linesCrossed = 0;
  token.flags = 0;
  token.intvalue = 0;
  LODWORD(token.floatvalue) = 0;
  token.whiteSpaceEnd_p = 0;
  token.next = 0;
  *(_DWORD *)&token.binaryIndex = 0;
  v36 = 0;
  v4 = src;
  v37 = 0;
  if ( idParser::ExpectTokenString(src, "{", 0) )
  {
    if ( idParser::ReadToken(src, (idToken *)((char *)&token + 4)) )
    {
      v6 = effect;
      while ( idStr::Cmp((const char *)token.alloced, "}") )
      {
        if ( idStr::Icmp((const char *)token.alloced, "start") )
        {
          if ( idStr::Icmp((const char *)token.alloced, "end") )
          {
            if ( idStr::Icmp((const char *)token.alloced, "motion") )
            {
              if ( idStr::Icmp((const char *)token.alloced, "generatedNormal") )
              {
                if ( idStr::Icmp((const char *)token.alloced, "generatedOriginNormal") )
                {
                  if ( idStr::Icmp((const char *)token.alloced, "lineHit") )
                  {
                    if ( idStr::Icmp((const char *)token.alloced, "flipNormal") )
                    {
                      if ( idStr::Icmp((const char *)token.alloced, "generatedLine") )
                      {
                        if ( idStr::Icmp((const char *)token.alloced, "persist") )
                        {
                          if ( idStr::Icmp((const char *)token.alloced, "tiling") )
                          {
                            if ( idStr::Icmp((const char *)token.alloced, "parentvelocity") )
                            {
                              if ( idStr::Icmp((const char *)token.alloced, "duration") )
                              {
                                if ( idStr::Icmp((const char *)token.alloced, "gravity") )
                                {
                                  if ( idStr::Icmp((const char *)token.alloced, "trailType") )
                                  {
                                    if ( idStr::Icmp((const char *)token.alloced, "trailMaterial") )
                                    {
                                      if ( idStr::Icmp((const char *)token.alloced, "trailTime") )
                                      {
                                        if ( idStr::Icmp((const char *)token.alloced, "trailRepeat") )
                                        {
                                          if ( idStr::Icmp((const char *)token.alloced, "trailCount") )
                                          {
                                            if ( idStr::Icmp((const char *)token.alloced, "trailScale") )
                                            {
                                              if ( idStr::Icmp((const char *)token.alloced, "material") )
                                              {
                                                if ( idStr::Icmp((const char *)token.alloced, "entityDef") )
                                                {
                                                  if ( idStr::Icmp((const char *)token.alloced, "fork") )
                                                  {
                                                    if ( idStr::Icmp((const char *)token.alloced, "forkMins") )
                                                    {
                                                      if ( idStr::Icmp((const char *)token.alloced, "forkMaxs") )
                                                      {
                                                        if ( idStr::Icmp((const char *)token.alloced, "jitterSize") )
                                                        {
                                                          if ( idStr::Icmp((const char *)token.alloced, "jitterRate") )
                                                          {
                                                            if ( idStr::Icmp((const char *)token.alloced, "jitterTable") )
                                                            {
                                                              if ( idStr::Icmp((const char *)token.alloced, "blend") )
                                                              {
                                                                if ( idStr::Icmp((const char *)token.alloced, "shadows") )
                                                                {
                                                                  if ( idStr::Icmp(
                                                                         (const char *)token.alloced,
                                                                         "specular") )
                                                                  {
                                                                    if ( idStr::Icmp(
                                                                           (const char *)token.alloced,
                                                                           "useLightningAxis") )
                                                                    {
                                                                      if ( idStr::Icmp(
                                                                             (const char *)token.alloced,
                                                                             "fadeIn") )
                                                                      {
                                                                        if ( idStr::Icmp(
                                                                               (const char *)token.alloced,
                                                                               "numFrames") )
                                                                        {
                                                                          if ( idStr::Icmp(
                                                                                 (const char *)token.alloced,
                                                                                 "model") )
                                                                          {
                                                                            if ( idStr::Icmp(
                                                                                   (const char *)token.alloced,
                                                                                   "impact") )
                                                                            {
                                                                              if ( idStr::Icmp(
                                                                                     (const char *)token.alloced,
                                                                                     "timeout") )
                                                                              {
                                                                                if ( idStr::Icmp(
                                                                                       (const char *)token.alloced,
                                                                                       "windDeviationAngle") )
                                                                                {
                                                                                  v32 = v4->scriptstack;
                                                                                  if ( v32 )
                                                                                    srce = (idParser *)v32->line;
                                                                                  else
                                                                                    srce = 0;
                                                                                  if ( v32 )
                                                                                    v33 = (idBitMsg **)v32->filename.data;
                                                                                  else
                                                                                    v33 = &s2;
                                                                                  v34 = (*(int (**)(void))v6->base->vfptr->gap4)();
                                                                                  (*(void (__cdecl **)(netadrtype_t, const char *, int, int, idBitMsg **, idParser *))(*(_DWORD *)common.type + 68))(
                                                                                    common.type,
                                                                                    "^4BSE:^1 Invalid particle keyword '%"
                                                                                    "s' in '%s' (file: %s, line: %d)",
                                                                                    token.alloced,
                                                                                    v34,
                                                                                    v33,
                                                                                    srce);
                                                                                }
                                                                                else
                                                                                {
                                                                                  v3->mWindDeviationAngle = idParser::ParseFloat(v4, 0);
                                                                                }
                                                                              }
                                                                              else
                                                                              {
                                                                                rvParticleTemplate::ParseTimeout(
                                                                                  v3,
                                                                                  v6,
                                                                                  v4);
                                                                              }
                                                                            }
                                                                            else
                                                                            {
                                                                              rvParticleTemplate::ParseImpact(
                                                                                v3,
                                                                                v6,
                                                                                v4);
                                                                            }
                                                                          }
                                                                          else
                                                                          {
                                                                            idParser::ReadToken(
                                                                              v4,
                                                                              (idToken *)((char *)&token + 4));
                                                                            v28 = ((int (__stdcall *)(int))renderModelManager->vfptr->FindModel)(token.alloced);
                                                                            v3->mModel = (idRenderModel *)v28;
                                                                            if ( !(*(int (__thiscall **)(int))(*(_DWORD *)v28 + 100))(v28) )
                                                                            {
                                                                              v3->mModel = (idRenderModel *)((int (__stdcall *)(const char *))renderModelManager->vfptr->FindModel)("_default");
                                                                              v29 = v4->scriptstack;
                                                                              if ( v29 )
                                                                                srcd = (idParser *)v29->line;
                                                                              else
                                                                                srcd = 0;
                                                                              if ( v29 )
                                                                                v30 = (idBitMsg **)v29->filename.data;
                                                                              else
                                                                                v30 = &s2;
                                                                              v31 = (*(int (**)(void))v6->base->vfptr->gap4)();
                                                                              (*(void (__cdecl **)(netadrtype_t, const char *, int, int, idBitMsg **, idParser *))(*(_DWORD *)common.type + 68))(
                                                                                common.type,
                                                                                "^4BSE:^1 No surfaces defined in model '%"
                                                                                "s' in '%s' (file: %s, line: %d)",
                                                                                token.alloced,
                                                                                v31,
                                                                                v30,
                                                                                srcd);
                                                                            }
                                                                          }
                                                                        }
                                                                        else
                                                                        {
                                                                          v3->mNumFrames = idParser::ParseInt(v4);
                                                                        }
                                                                      }
                                                                      else
                                                                      {
                                                                        v3->mFlags |= (unsigned int)&vwin8192[2696];
                                                                      }
                                                                    }
                                                                    else
                                                                    {
                                                                      v3->mFlags |= 0x400000u;
                                                                    }
                                                                  }
                                                                  else
                                                                  {
                                                                    v3->mFlags |= 0x40000u;
                                                                  }
                                                                }
                                                                else
                                                                {
                                                                  v3->mFlags |= 0x20000u;
                                                                }
                                                              }
                                                              else
                                                              {
                                                                rvParticleTemplate::ParseBlendParms(v3, v6, v4);
                                                              }
                                                            }
                                                            else
                                                            {
                                                              rvParticleTemplate::AllocElectricityInfo(v3);
                                                              idParser::ReadToken(v4, (idToken *)((char *)&token + 4));
                                                              v26 = (const char *)token.alloced;
                                                              v27 = (sdDeclWrapperTemplate<idMaterial> *)sdSingleton<sdDeclTypeHolder>::GetInstance();
                                                              v3->mElecInfo->mJitterTable = (idDeclTable *)sdDeclWrapperTemplate<idSoundShader>::LocalFind(v27, v26, 1);
                                                            }
                                                          }
                                                          else
                                                          {
                                                            rvParticleTemplate::AllocElectricityInfo(v3);
                                                            v3->mElecInfo->mJitterRate = idParser::ParseFloat(v4, 0);
                                                          }
                                                        }
                                                        else
                                                        {
                                                          rvParticleTemplate::AllocElectricityInfo(v3);
                                                          rvParticleTemplate::GetVector(
                                                            v4,
                                                            3,
                                                            &v3->mElecInfo->mJitterSize);
                                                        }
                                                      }
                                                      else
                                                      {
                                                        rvParticleTemplate::AllocElectricityInfo(v3);
                                                        rvParticleTemplate::GetVector(
                                                          v4,
                                                          3,
                                                          &v3->mElecInfo->mForkSizeMaxs);
                                                      }
                                                    }
                                                    else
                                                    {
                                                      rvParticleTemplate::AllocElectricityInfo(v3);
                                                      rvParticleTemplate::GetVector(
                                                        v4,
                                                        3,
                                                        &v3->mElecInfo->mForkSizeMins);
                                                    }
                                                  }
                                                  else
                                                  {
                                                    rvParticleTemplate::AllocElectricityInfo(v3);
                                                    v3->mElecInfo->mNumForks = idParser::ParseInt(v4);
                                                    v25 = v3->mElecInfo;
                                                    if ( v25->mNumForks > 16 )
                                                      v25->mNumForks = 16;
                                                  }
                                                }
                                                else
                                                {
                                                  idParser::ReadToken(v4, (idToken *)((char *)&token + 4));
                                                  v22 = token.data;
                                                  if ( (_DWORD)(token.data + 1) > abs(v3->mEntityDefName.alloced) )
                                                    idStr::ReAllocate(&v3->mEntityDefName, (int)(token.data + 1), 0);
                                                  memcpy(
                                                    v3->mEntityDefName.data,
                                                    (const void *)token.alloced,
                                                    (size_t)v22);
                                                  v22[(unsigned int)v3->mEntityDefName.data] = 0;
                                                  v3->mEntityDefName.len = (int)v22;
                                                  v23 = v3->mEntityDefName.data;
                                                  v24 = sdSingleton<sdDeclTypeHolder>::GetInstance();
                                                  sdDeclTypeHolder::FindEntityDef(v24, v23, 0);
                                                  v6 = effect;
                                                }
                                              }
                                              else
                                              {
                                                idParser::ReadToken(v4, (idToken *)((char *)&token + 4));
                                                v20 = (const char *)token.alloced;
                                                v21 = sdSingleton<sdDeclTypeHolder>::GetInstance();
                                                v3->mMaterial = sdDeclWrapperTemplate<idSoundShader>::LocalFind(
                                                                  &v21->declMaterialType,
                                                                  v20,
                                                                  1);
                                              }
                                            }
                                            else
                                            {
                                              rvParticleTemplate::AllocTrail(v3);
                                              v3->mTrailInfo->mTrailScale = idParser::ParseFloat(v4, 0);
                                            }
                                          }
                                          else
                                          {
                                            rvParticleTemplate::AllocTrail(v3);
                                            v18 = &v3->mTrailInfo->mTrailCount.x;
                                            *v18 = idParser::ParseFloat(v4, 0);
                                            idParser::ExpectTokenString(v4, ",", 0);
                                            v19 = &v3->mTrailInfo->mTrailCount.y;
                                            *v19 = idParser::ParseFloat(v4, 0);
                                          }
                                        }
                                        else
                                        {
                                          rvParticleTemplate::AllocTrail(v3);
                                          v3->mTrailRepeat = idParser::ParseInt(v4);
                                        }
                                      }
                                      else
                                      {
                                        rvParticleTemplate::AllocTrail(v3);
                                        v16 = &v3->mTrailInfo->mTrailTime.x;
                                        *v16 = idParser::ParseFloat(v4, 0);
                                        idParser::ExpectTokenString(v4, ",", 0);
                                        v17 = &v3->mTrailInfo->mTrailTime.y;
                                        *v17 = idParser::ParseFloat(v4, 0);
                                      }
                                    }
                                    else
                                    {
                                      rvParticleTemplate::AllocTrail(v3);
                                      idParser::ReadToken(v4, (idToken *)((char *)&token + 4));
                                      v14 = (const char *)token.alloced;
                                      v15 = sdSingleton<sdDeclTypeHolder>::GetInstance();
                                      v3->mTrailInfo->mTrailMaterial = sdDeclWrapperTemplate<idSoundShader>::LocalFind(
                                                                         &v15->declMaterialType,
                                                                         v14,
                                                                         1);
                                    }
                                  }
                                  else
                                  {
                                    rvParticleTemplate::AllocTrail(v3);
                                    idParser::ReadToken(v4, (idToken *)((char *)&token + 4));
                                    if ( idStr::Icmp((const char *)token.alloced, "burn") )
                                    {
                                      if ( idStr::Icmp((const char *)token.alloced, "motion") )
                                      {
                                        v3->mTrailInfo->mTrailType = 3;
                                        v10 = v3->mTrailInfo;
                                        v11 = v10->mTrailTypeName.alloced;
                                        v12 = token.data;
                                        v13 = &v10->mTrailTypeName;
                                        if ( (_DWORD)(token.data + 1) > abs(v11) )
                                          idStr::ReAllocate(v13, (int)(token.data + 1), 0);
                                        memcpy(v13->data, (const void *)token.alloced, (size_t)v12);
                                        v12[(unsigned int)v13->data] = 0;
                                        v13->len = (int)v12;
                                        v6 = effect;
                                      }
                                      else
                                      {
                                        v3->mTrailInfo->mTrailType = 2;
                                      }
                                    }
                                    else
                                    {
                                      v3->mTrailInfo->mTrailType = 1;
                                    }
                                  }
                                }
                                else
                                {
                                  v3->mGravity.x = idParser::ParseFloat(v4, 0);
                                  idParser::ExpectTokenString(v4, ",", 0);
                                  v3->mGravity.y = idParser::ParseFloat(v4, 0);
                                }
                              }
                              else
                              {
                                srcb = idParser::ParseFloat(v4, 0);
                                v8 = 0.0020000001;
                                if ( srcb >= 0.0020000001 )
                                {
                                  v8 = srcb;
                                  if ( srcb > 300.0 )
                                    v8 = 300.0;
                                }
                                srcg = v8;
                                v3->mDuration.x = srcg;
                                idParser::ExpectTokenString(v4, ",", 0);
                                srcc = idParser::ParseFloat(v4, 0);
                                v9 = 0.0020000001;
                                if ( srcc < 0.0020000001 || (v9 = srcc, srcc <= 300.0) )
                                {
                                  srch = v9;
                                  v3->mDuration.y = srch;
                                }
                                else
                                {
                                  v3->mDuration.y = 300.0;
                                }
                              }
                            }
                            else
                            {
                              v3->mFlags |= 0x2000000u;
                            }
                          }
                          else
                          {
                            v3->mFlags |= 0x100000u;
                            srca = idParser::ParseFloat(v4, 0);
                            v7 = 0.0020000001;
                            if ( srca < 0.0020000001 || (v7 = srca, srca <= 1024.0) )
                            {
                              srcf = v7;
                              v3->mTiling = srcf;
                            }
                            else
                            {
                              v3->mTiling = 1024.0;
                            }
                          }
                        }
                        else
                        {
                          v3->mFlags |= 0x200000u;
                        }
                      }
                      else
                      {
                        v3->mFlags |= 0x10000u;
                      }
                    }
                    else
                    {
                      v3->mFlags |= 0x2000u;
                    }
                  }
                  else
                  {
                    v3->mFlags |= 0x4000000u;
                  }
                }
                else
                {
                  v3->mFlags |= 0x1000u;
                }
              }
              else
              {
                v3->mFlags |= 0x800u;
              }
            }
            else
            {
              rvParticleTemplate::ParseMotionDomains(v3, v6, v4);
            }
          }
          else
          {
            rvParticleTemplate::ParseDeathDomains(v3, v6, v4);
          }
        }
        else
        {
          rvParticleTemplate::ParseSpawnDomains(v3, v6, v4);
        }
        if ( !idParser::ReadToken(v4, (idToken *)((char *)&token + 4)) )
          goto LABEL_113;
      }
      rvParticleTemplate::Finish(v3, (int)v4);
      v37 = -1;
      idStr::FreeData((idStr *)&token.data);
      result = 1;
    }
    else
    {
LABEL_113:
      v37 = -1;
      idStr::FreeData((idStr *)&token.data);
      result = 0;
    }
  }
  else
  {
    v37 = -1;
    idStr::FreeData((idStr *)&token.data);
    result = 0;
  }
  return result;
}
void __thiscall rvParticleTemplate::MakeEditable(rvParticleTemplate *this)
{
  rvParticleTemplate *v1; // ebp
  rvTrailInfo *v2; // esi
  rvTrailInfo *v3; // eax
  rvElectricityInfo *v4; // esi
  rvElectricityInfo *v5; // eax
  rvElectricityInfo *v6; // ecx
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v7; // ecx
  rvParticleParms *v8; // eax
  rvParticleParms *v9; // eax
  sdModelInfo *v10; // eax
  sdModelInfo *v11; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v12; // ecx
  rvParticleParms *v13; // eax
  rvParticleParms *v14; // eax
  sdModelInfo *v15; // eax
  sdModelInfo *v16; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v17; // ecx
  rvParticleParms *v18; // eax
  rvParticleParms *v19; // eax
  sdModelInfo *v20; // eax
  sdModelInfo *v21; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v22; // ecx
  rvParticleParms *v23; // eax
  rvParticleParms *v24; // eax
  sdModelInfo *v25; // eax
  sdModelInfo *v26; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v27; // ecx
  rvParticleParms *v28; // eax
  rvParticleParms *v29; // eax
  sdModelInfo *v30; // eax
  sdModelInfo *v31; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v32; // ecx
  rvParticleParms *v33; // eax
  rvParticleParms *v34; // eax
  sdModelInfo *v35; // eax
  sdModelInfo *v36; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v37; // ecx
  rvParticleParms *v38; // eax
  rvParticleParms *v39; // eax
  sdModelInfo *v40; // eax
  sdModelInfo *v41; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v42; // ecx
  rvParticleParms *v43; // eax
  rvParticleParms *v44; // eax
  sdModelInfo *v45; // eax
  sdModelInfo *v46; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v47; // ecx
  rvParticleParms *v48; // eax
  rvParticleParms *v49; // eax
  sdModelInfo *v50; // eax
  sdModelInfo *v51; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v52; // ecx
  rvParticleParms *v53; // eax
  rvParticleParms *v54; // eax
  sdModelInfo *v55; // eax
  sdModelInfo *v56; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v57; // ecx
  rvParticleParms *v58; // eax
  rvParticleParms *v59; // eax
  sdModelInfo *v60; // eax
  sdModelInfo *v61; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v62; // ecx
  rvParticleParms *v63; // eax
  rvParticleParms *v64; // eax
  sdModelInfo *v65; // eax
  sdModelInfo *v66; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v67; // ecx
  rvParticleParms *v68; // eax
  rvParticleParms *v69; // eax
  sdModelInfo *v70; // eax
  sdModelInfo *v71; // edi
  rvEnvParms *v72; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v73; // ecx
  rvEnvParms *v74; // eax
  rvEnvParms *v75; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v76; // ecx
  rvEnvParms *v77; // eax
  rvEnvParms *v78; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v79; // ecx
  rvEnvParms *v80; // eax
  rvEnvParms *v81; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v82; // ecx
  rvEnvParms *v83; // eax
  rvEnvParms *v84; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v85; // ecx
  rvEnvParms *v86; // eax
  rvEnvParms *v87; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v88; // ecx
  rvEnvParms *v89; // eax
  rvEnvParms *v90; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v91; // ecx
  rvEnvParms *v92; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v93; // ecx
  rvParticleParms *v94; // eax
  rvParticleParms *v95; // eax
  sdModelInfo *v96; // eax
  sdModelInfo *v97; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v98; // ecx
  rvParticleParms *v99; // eax
  rvParticleParms *v100; // eax
  sdModelInfo *v101; // eax
  sdModelInfo *v102; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v103; // ecx
  rvParticleParms *v104; // eax
  rvParticleParms *v105; // eax
  sdModelInfo *v106; // eax
  sdModelInfo *v107; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v108; // ecx
  rvParticleParms *v109; // eax
  rvParticleParms *v110; // eax
  sdModelInfo *v111; // eax
  sdModelInfo *v112; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v113; // ecx
  rvParticleParms *v114; // eax
  rvParticleParms *v115; // eax
  sdModelInfo *v116; // eax
  sdModelInfo *v117; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v118; // ecx
  rvParticleParms *v119; // eax
  rvParticleParms *v120; // eax
  sdModelInfo *v121; // eax
  sdModelInfo *v122; // edi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v123; // ecx
  rvParticleParms *v124; // eax
  rvParticleParms *v125; // eax
  sdModelInfo *v126; // eax
  sdModelInfo *v127; // edi
  rvParticleParms *v128; // [esp+10h] [ebp-8h]
  rvParticleParms *v129; // [esp+10h] [ebp-8h]
  rvParticleParms *v130; // [esp+10h] [ebp-8h]
  rvParticleParms *v131; // [esp+10h] [ebp-8h]
  rvParticleParms *v132; // [esp+10h] [ebp-8h]
  rvParticleParms *v133; // [esp+10h] [ebp-8h]
  rvParticleParms *v134; // [esp+10h] [ebp-8h]
  rvParticleParms *v135; // [esp+10h] [ebp-8h]
  rvParticleParms *v136; // [esp+10h] [ebp-8h]
  rvParticleParms *v137; // [esp+10h] [ebp-8h]
  rvParticleParms *v138; // [esp+10h] [ebp-8h]
  rvParticleParms *v139; // [esp+10h] [ebp-8h]
  rvParticleParms *v140; // [esp+10h] [ebp-8h]
  rvParticleParms *v141; // [esp+10h] [ebp-8h]
  rvParticleParms *v142; // [esp+10h] [ebp-8h]
  rvParticleParms *v143; // [esp+10h] [ebp-8h]
  rvParticleParms *v144; // [esp+10h] [ebp-8h]
  rvParticleParms *v145; // [esp+10h] [ebp-8h]
  rvParticleParms *v146; // [esp+10h] [ebp-8h]
  rvParticleParms *v147; // [esp+10h] [ebp-8h]
  const void **v148; // [esp+14h] [ebp-4h]
  const void **v149; // [esp+14h] [ebp-4h]
  const void **v150; // [esp+14h] [ebp-4h]
  const void **v151; // [esp+14h] [ebp-4h]
  const void **v152; // [esp+14h] [ebp-4h]
  const void **v153; // [esp+14h] [ebp-4h]
  const void **v154; // [esp+14h] [ebp-4h]
  const void **v155; // [esp+14h] [ebp-4h]
  const void **v156; // [esp+14h] [ebp-4h]
  const void **v157; // [esp+14h] [ebp-4h]
  const void **v158; // [esp+14h] [ebp-4h]
  const void **v159; // [esp+14h] [ebp-4h]
  const void **v160; // [esp+14h] [ebp-4h]
  const void **v161; // [esp+14h] [ebp-4h]
  const void **v162; // [esp+14h] [ebp-4h]
  const void **v163; // [esp+14h] [ebp-4h]
  const void **v164; // [esp+14h] [ebp-4h]
  const void **v165; // [esp+14h] [ebp-4h]
  const void **v166; // [esp+14h] [ebp-4h]
  const void **v167; // [esp+14h] [ebp-4h]

  v1 = this;
  rvParticleTemplate::InitStatic();
  if ( !(v1->mFlags & 0x8000000) )
    rvParticleTemplate::Init(v1);
  v2 = v1->mTrailInfo;
  if ( v2->mStatic )
  {
    v3 = (rvTrailInfo *)operator new(0x3Cu);
    if ( v3 )
    {
      v3->mStatic = 0;
      v3->mTrailTypeName.len = 0;
      v3->mTrailTypeName.alloced = -20;
      v3->mTrailTypeName.data = v3->mTrailTypeName.baseBuffer;
      v3->mTrailTypeName.baseBuffer[0] = 0;
    }
    else
    {
      v3 = 0;
    }
    v1->mTrailInfo = v3;
    rvTrailInfo::operator=(v3, v2);
    v1->mTrailInfo->mStatic = 0;
  }
  v4 = v1->mElecInfo;
  if ( v4->mStatic )
  {
    v5 = (rvElectricityInfo *)operator new(0x34u);
    if ( v5 )
    {
      v5->mStatic = 0;
      v6 = v5;
    }
    else
    {
      v6 = 0;
    }
    v1->mElecInfo = v6;
    rvElectricityInfo::operator=(v6, v4);
    v1->mElecInfo->mStatic = 0;
  }
  v148 = (const void **)v1->mpSpawnPosition;
  if ( v1->mpSpawnPosition->mStatic )
  {
    v7 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v7 && (v8 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v7)) != 0 )
    {
      v8->mModelInfo = 0;
      v8->mStatic = 0;
      v128 = v8;
    }
    else
    {
      v128 = 0;
    }
    v9 = v128;
    v1->mpSpawnPosition = v128;
    qmemcpy(v128, v148, sizeof(rvParticleParms));
    if ( v148[2] )
    {
      v10 = (sdModelInfo *)operator new(0x2Cu);
      v128->mModelInfo = v10;
      v11 = v10;
      v9 = v128;
      qmemcpy(v11, v148[2], sizeof(sdModelInfo));
    }
    v9->mStatic = 0;
    v1->mpSpawnPosition->mStatic = 0;
  }
  v149 = (const void **)v1->mpSpawnDirection;
  if ( v1->mpSpawnDirection->mStatic )
  {
    v12 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v12 && (v13 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v12)) != 0 )
    {
      v13->mModelInfo = 0;
      v13->mStatic = 0;
      v129 = v13;
    }
    else
    {
      v129 = 0;
    }
    v14 = v129;
    v1->mpSpawnDirection = v129;
    qmemcpy(v129, v149, sizeof(rvParticleParms));
    if ( v149[2] )
    {
      v15 = (sdModelInfo *)operator new(0x2Cu);
      v129->mModelInfo = v15;
      v16 = v15;
      v14 = v129;
      qmemcpy(v16, v149[2], sizeof(sdModelInfo));
    }
    v14->mStatic = 0;
    v1->mpSpawnDirection->mStatic = 0;
  }
  v150 = (const void **)v1->mpSpawnVelocity;
  if ( v1->mpSpawnVelocity->mStatic )
  {
    v17 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v17 && (v18 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v17)) != 0 )
    {
      v18->mModelInfo = 0;
      v18->mStatic = 0;
      v130 = v18;
    }
    else
    {
      v130 = 0;
    }
    v19 = v130;
    v1->mpSpawnVelocity = v130;
    qmemcpy(v130, v150, sizeof(rvParticleParms));
    if ( v150[2] )
    {
      v20 = (sdModelInfo *)operator new(0x2Cu);
      v130->mModelInfo = v20;
      v21 = v20;
      v19 = v130;
      qmemcpy(v21, v150[2], sizeof(sdModelInfo));
    }
    v19->mStatic = 0;
    v1->mpSpawnVelocity->mStatic = 0;
  }
  v151 = (const void **)v1->mpSpawnAcceleration;
  if ( v1->mpSpawnAcceleration->mStatic )
  {
    v22 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v22 && (v23 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v22)) != 0 )
    {
      v23->mModelInfo = 0;
      v23->mStatic = 0;
      v131 = v23;
    }
    else
    {
      v131 = 0;
    }
    v24 = v131;
    v1->mpSpawnAcceleration = v131;
    qmemcpy(v131, v151, sizeof(rvParticleParms));
    if ( v151[2] )
    {
      v25 = (sdModelInfo *)operator new(0x2Cu);
      v131->mModelInfo = v25;
      v26 = v25;
      v24 = v131;
      qmemcpy(v26, v151[2], sizeof(sdModelInfo));
    }
    v24->mStatic = 0;
    v1->mpSpawnAcceleration->mStatic = 0;
  }
  v152 = (const void **)v1->mpSpawnFriction;
  if ( v1->mpSpawnFriction->mStatic )
  {
    v27 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v27 && (v28 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v27)) != 0 )
    {
      v28->mModelInfo = 0;
      v28->mStatic = 0;
      v132 = v28;
    }
    else
    {
      v132 = 0;
    }
    v29 = v132;
    v1->mpSpawnFriction = v132;
    qmemcpy(v132, v152, sizeof(rvParticleParms));
    if ( v152[2] )
    {
      v30 = (sdModelInfo *)operator new(0x2Cu);
      v132->mModelInfo = v30;
      v31 = v30;
      v29 = v132;
      qmemcpy(v31, v152[2], sizeof(sdModelInfo));
    }
    v29->mStatic = 0;
    v1->mpSpawnFriction->mStatic = 0;
  }
  v153 = (const void **)v1->mpSpawnTint;
  if ( v1->mpSpawnTint->mStatic )
  {
    v32 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v32 && (v33 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v32)) != 0 )
    {
      v33->mModelInfo = 0;
      v33->mStatic = 0;
      v133 = v33;
    }
    else
    {
      v133 = 0;
    }
    v34 = v133;
    v1->mpSpawnTint = v133;
    qmemcpy(v133, v153, sizeof(rvParticleParms));
    if ( v153[2] )
    {
      v35 = (sdModelInfo *)operator new(0x2Cu);
      v133->mModelInfo = v35;
      v36 = v35;
      v34 = v133;
      qmemcpy(v36, v153[2], sizeof(sdModelInfo));
    }
    v34->mStatic = 0;
    v1->mpSpawnTint->mStatic = 0;
  }
  v154 = (const void **)v1->mpSpawnFade;
  if ( v1->mpSpawnFade->mStatic )
  {
    v37 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v37 && (v38 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v37)) != 0 )
    {
      v38->mModelInfo = 0;
      v38->mStatic = 0;
      v134 = v38;
    }
    else
    {
      v134 = 0;
    }
    v39 = v134;
    v1->mpSpawnFade = v134;
    qmemcpy(v134, v154, sizeof(rvParticleParms));
    if ( v154[2] )
    {
      v40 = (sdModelInfo *)operator new(0x2Cu);
      v134->mModelInfo = v40;
      v41 = v40;
      v39 = v134;
      qmemcpy(v41, v154[2], sizeof(sdModelInfo));
    }
    v39->mStatic = 0;
    v1->mpSpawnFade->mStatic = 0;
  }
  v155 = (const void **)v1->mpSpawnSize;
  if ( v1->mpSpawnSize->mStatic )
  {
    v42 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v42 && (v43 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v42)) != 0 )
    {
      v43->mModelInfo = 0;
      v43->mStatic = 0;
      v135 = v43;
    }
    else
    {
      v135 = 0;
    }
    v44 = v135;
    v1->mpSpawnSize = v135;
    qmemcpy(v135, v155, sizeof(rvParticleParms));
    if ( v155[2] )
    {
      v45 = (sdModelInfo *)operator new(0x2Cu);
      v135->mModelInfo = v45;
      v46 = v45;
      v44 = v135;
      qmemcpy(v46, v155[2], sizeof(sdModelInfo));
    }
    v44->mStatic = 0;
    v1->mpSpawnSize->mStatic = 0;
  }
  v156 = (const void **)v1->mpSpawnRotate;
  if ( v1->mpSpawnRotate->mStatic )
  {
    v47 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v47 && (v48 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v47)) != 0 )
    {
      v48->mModelInfo = 0;
      v48->mStatic = 0;
      v136 = v48;
    }
    else
    {
      v136 = 0;
    }
    v49 = v136;
    v1->mpSpawnRotate = v136;
    qmemcpy(v136, v156, sizeof(rvParticleParms));
    if ( v156[2] )
    {
      v50 = (sdModelInfo *)operator new(0x2Cu);
      v136->mModelInfo = v50;
      v51 = v50;
      v49 = v136;
      qmemcpy(v51, v156[2], sizeof(sdModelInfo));
    }
    v49->mStatic = 0;
    v1->mpSpawnRotate->mStatic = 0;
  }
  v157 = (const void **)v1->mpSpawnAngle;
  if ( v1->mpSpawnAngle->mStatic )
  {
    v52 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v52 && (v53 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v52)) != 0 )
    {
      v53->mModelInfo = 0;
      v53->mStatic = 0;
      v137 = v53;
    }
    else
    {
      v137 = 0;
    }
    v54 = v137;
    v1->mpSpawnAngle = v137;
    qmemcpy(v137, v157, sizeof(rvParticleParms));
    if ( v157[2] )
    {
      v55 = (sdModelInfo *)operator new(0x2Cu);
      v137->mModelInfo = v55;
      v56 = v55;
      v54 = v137;
      qmemcpy(v56, v157[2], sizeof(sdModelInfo));
    }
    v54->mStatic = 0;
    v1->mpSpawnAngle->mStatic = 0;
  }
  v158 = (const void **)v1->mpSpawnOffset;
  if ( v1->mpSpawnOffset->mStatic )
  {
    v57 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v57 && (v58 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v57)) != 0 )
    {
      v58->mModelInfo = 0;
      v58->mStatic = 0;
      v138 = v58;
    }
    else
    {
      v138 = 0;
    }
    v59 = v138;
    v1->mpSpawnOffset = v138;
    qmemcpy(v138, v158, sizeof(rvParticleParms));
    if ( v158[2] )
    {
      v60 = (sdModelInfo *)operator new(0x2Cu);
      v138->mModelInfo = v60;
      v61 = v60;
      v59 = v138;
      qmemcpy(v61, v158[2], sizeof(sdModelInfo));
    }
    v59->mStatic = 0;
    v1->mpSpawnOffset->mStatic = 0;
  }
  v159 = (const void **)v1->mpSpawnLength;
  if ( v1->mpSpawnLength->mStatic )
  {
    v62 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v62 && (v63 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v62)) != 0 )
    {
      v63->mModelInfo = 0;
      v63->mStatic = 0;
      v139 = v63;
    }
    else
    {
      v139 = 0;
    }
    v64 = v139;
    v1->mpSpawnLength = v139;
    qmemcpy(v139, v159, sizeof(rvParticleParms));
    if ( v159[2] )
    {
      v65 = (sdModelInfo *)operator new(0x2Cu);
      v139->mModelInfo = v65;
      v66 = v65;
      v64 = v139;
      qmemcpy(v66, v159[2], sizeof(sdModelInfo));
    }
    v64->mStatic = 0;
    v1->mpSpawnLength->mStatic = 0;
  }
  v160 = (const void **)v1->mpSpawnWindStrength;
  if ( v1->mpSpawnWindStrength->mStatic )
  {
    v67 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v67 && (v68 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v67)) != 0 )
    {
      v68->mModelInfo = 0;
      v68->mStatic = 0;
      v140 = v68;
    }
    else
    {
      v140 = 0;
    }
    v69 = v140;
    v1->mpSpawnWindStrength = v140;
    qmemcpy(v140, v160, sizeof(rvParticleParms));
    if ( v160[2] )
    {
      v70 = (sdModelInfo *)operator new(0x2Cu);
      v140->mModelInfo = v70;
      v71 = v70;
      v69 = v140;
      qmemcpy(v71, v160[2], sizeof(sdModelInfo));
    }
    v69->mStatic = 0;
    v1->mpSpawnWindStrength->mStatic = 0;
  }
  v72 = v1->mpTintEnvelope;
  if ( v72->mStatic )
  {
    v73 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v73 && (v74 = sdDetails::sdPoolAlloc<rvEnvParms,128>::Alloc(v73)) != 0 )
    {
      v74->mStatic = 0;
      v74->mFastLookUp = 0;
    }
    else
    {
      v74 = 0;
    }
    v1->mpTintEnvelope = v74;
    rvEnvParms::operator=(v74, v72);
    v1->mpTintEnvelope->mStatic = 0;
  }
  v1->mpTintEnvelope->mFastLookUp = 0;
  v75 = v1->mpFadeEnvelope;
  if ( v75->mStatic )
  {
    v76 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v76 && (v77 = sdDetails::sdPoolAlloc<rvEnvParms,128>::Alloc(v76)) != 0 )
    {
      v77->mStatic = 0;
      v77->mFastLookUp = 0;
    }
    else
    {
      v77 = 0;
    }
    v1->mpFadeEnvelope = v77;
    rvEnvParms::operator=(v77, v75);
    v1->mpFadeEnvelope->mStatic = 0;
  }
  v1->mpFadeEnvelope->mFastLookUp = 0;
  v78 = v1->mpSizeEnvelope;
  if ( v78->mStatic )
  {
    v79 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v79 && (v80 = sdDetails::sdPoolAlloc<rvEnvParms,128>::Alloc(v79)) != 0 )
    {
      v80->mStatic = 0;
      v80->mFastLookUp = 0;
    }
    else
    {
      v80 = 0;
    }
    v1->mpSizeEnvelope = v80;
    rvEnvParms::operator=(v80, v78);
    v1->mpSizeEnvelope->mStatic = 0;
  }
  v1->mpSizeEnvelope->mFastLookUp = 0;
  v81 = v1->mpRotateEnvelope;
  if ( v81->mStatic )
  {
    v82 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v82 && (v83 = sdDetails::sdPoolAlloc<rvEnvParms,128>::Alloc(v82)) != 0 )
    {
      v83->mStatic = 0;
      v83->mFastLookUp = 0;
    }
    else
    {
      v83 = 0;
    }
    v1->mpRotateEnvelope = v83;
    rvEnvParms::operator=(v83, v81);
    v1->mpRotateEnvelope->mStatic = 0;
  }
  v1->mpRotateEnvelope->mFastLookUp = 0;
  v84 = v1->mpAngleEnvelope;
  if ( v84->mStatic )
  {
    v85 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v85 && (v86 = sdDetails::sdPoolAlloc<rvEnvParms,128>::Alloc(v85)) != 0 )
    {
      v86->mStatic = 0;
      v86->mFastLookUp = 0;
    }
    else
    {
      v86 = 0;
    }
    v1->mpAngleEnvelope = v86;
    rvEnvParms::operator=(v86, v84);
    v1->mpAngleEnvelope->mStatic = 0;
  }
  v1->mpAngleEnvelope->mFastLookUp = 0;
  v87 = v1->mpOffsetEnvelope;
  if ( v87->mStatic )
  {
    v88 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v88 && (v89 = sdDetails::sdPoolAlloc<rvEnvParms,128>::Alloc(v88)) != 0 )
    {
      v89->mStatic = 0;
      v89->mFastLookUp = 0;
    }
    else
    {
      v89 = 0;
    }
    v1->mpOffsetEnvelope = v89;
    rvEnvParms::operator=(v89, v87);
    v1->mpOffsetEnvelope->mStatic = 0;
  }
  v1->mpOffsetEnvelope->mFastLookUp = 0;
  v90 = v1->mpLengthEnvelope;
  if ( v90->mStatic )
  {
    v91 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v91 && (v92 = sdDetails::sdPoolAlloc<rvEnvParms,128>::Alloc(v91)) != 0 )
    {
      v92->mStatic = 0;
      v92->mFastLookUp = 0;
    }
    else
    {
      v92 = 0;
    }
    v1->mpLengthEnvelope = v92;
    rvEnvParms::operator=(v92, v90);
    v1->mpLengthEnvelope->mStatic = 0;
  }
  v1->mpLengthEnvelope->mFastLookUp = 0;
  v161 = (const void **)v1->mpDeathTint;
  if ( v1->mpDeathTint->mStatic )
  {
    v93 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v93 && (v94 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v93)) != 0 )
    {
      v94->mModelInfo = 0;
      v94->mStatic = 0;
      v141 = v94;
    }
    else
    {
      v141 = 0;
    }
    v95 = v141;
    v1->mpDeathTint = v141;
    qmemcpy(v141, v161, sizeof(rvParticleParms));
    if ( v161[2] )
    {
      v96 = (sdModelInfo *)operator new(0x2Cu);
      v141->mModelInfo = v96;
      v97 = v96;
      v95 = v141;
      qmemcpy(v97, v161[2], sizeof(sdModelInfo));
    }
    v95->mStatic = 0;
    v1->mpDeathTint->mStatic = 0;
  }
  v162 = (const void **)v1->mpDeathFade;
  if ( v1->mpDeathFade->mStatic )
  {
    v98 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v98 && (v99 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v98)) != 0 )
    {
      v99->mModelInfo = 0;
      v99->mStatic = 0;
      v142 = v99;
    }
    else
    {
      v142 = 0;
    }
    v100 = v142;
    v1->mpDeathFade = v142;
    qmemcpy(v142, v162, sizeof(rvParticleParms));
    if ( v162[2] )
    {
      v101 = (sdModelInfo *)operator new(0x2Cu);
      v142->mModelInfo = v101;
      v102 = v101;
      v100 = v142;
      qmemcpy(v102, v162[2], sizeof(sdModelInfo));
    }
    v100->mStatic = 0;
    v1->mpDeathFade->mStatic = 0;
  }
  v163 = (const void **)v1->mpDeathSize;
  if ( v1->mpDeathSize->mStatic )
  {
    v103 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v103 && (v104 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v103)) != 0 )
    {
      v104->mModelInfo = 0;
      v104->mStatic = 0;
      v143 = v104;
    }
    else
    {
      v143 = 0;
    }
    v105 = v143;
    v1->mpDeathSize = v143;
    qmemcpy(v143, v163, sizeof(rvParticleParms));
    if ( v163[2] )
    {
      v106 = (sdModelInfo *)operator new(0x2Cu);
      v143->mModelInfo = v106;
      v107 = v106;
      v105 = v143;
      qmemcpy(v107, v163[2], sizeof(sdModelInfo));
    }
    v105->mStatic = 0;
    v1->mpDeathSize->mStatic = 0;
  }
  v164 = (const void **)v1->mpDeathRotate;
  if ( v1->mpDeathRotate->mStatic )
  {
    v108 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v108 && (v109 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v108)) != 0 )
    {
      v109->mModelInfo = 0;
      v109->mStatic = 0;
      v144 = v109;
    }
    else
    {
      v144 = 0;
    }
    v110 = v144;
    v1->mpDeathRotate = v144;
    qmemcpy(v144, v164, sizeof(rvParticleParms));
    if ( v164[2] )
    {
      v111 = (sdModelInfo *)operator new(0x2Cu);
      v144->mModelInfo = v111;
      v112 = v111;
      v110 = v144;
      qmemcpy(v112, v164[2], sizeof(sdModelInfo));
    }
    v110->mStatic = 0;
    v1->mpDeathRotate->mStatic = 0;
  }
  v165 = (const void **)v1->mpDeathAngle;
  if ( v1->mpDeathAngle->mStatic )
  {
    v113 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v113 && (v114 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v113)) != 0 )
    {
      v114->mModelInfo = 0;
      v114->mStatic = 0;
      v145 = v114;
    }
    else
    {
      v145 = 0;
    }
    v115 = v145;
    v1->mpDeathAngle = v145;
    qmemcpy(v145, v165, sizeof(rvParticleParms));
    if ( v165[2] )
    {
      v116 = (sdModelInfo *)operator new(0x2Cu);
      v145->mModelInfo = v116;
      v117 = v116;
      v115 = v145;
      qmemcpy(v117, v165[2], sizeof(sdModelInfo));
    }
    v115->mStatic = 0;
    v1->mpDeathAngle->mStatic = 0;
  }
  v166 = (const void **)v1->mpDeathOffset;
  if ( v1->mpDeathOffset->mStatic )
  {
    v118 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v118 && (v119 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v118)) != 0 )
    {
      v119->mModelInfo = 0;
      v119->mStatic = 0;
      v146 = v119;
    }
    else
    {
      v146 = 0;
    }
    v120 = v146;
    v1->mpDeathOffset = v146;
    qmemcpy(v146, v166, sizeof(rvParticleParms));
    if ( v166[2] )
    {
      v121 = (sdModelInfo *)operator new(0x2Cu);
      v146->mModelInfo = v121;
      v122 = v121;
      v120 = v146;
      qmemcpy(v122, v166[2], sizeof(sdModelInfo));
    }
    v120->mStatic = 0;
    v1->mpDeathOffset->mStatic = 0;
  }
  v167 = (const void **)v1->mpDeathLength;
  if ( v1->mpDeathLength->mStatic )
  {
    v123 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
    if ( v123 && (v124 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v123)) != 0 )
    {
      v124->mModelInfo = 0;
      v124->mStatic = 0;
      v147 = v124;
    }
    else
    {
      v147 = 0;
    }
    v125 = v147;
    v1->mpDeathLength = v147;
    qmemcpy(v147, v167, sizeof(rvParticleParms));
    if ( v167[2] )
    {
      v126 = (sdModelInfo *)operator new(0x2Cu);
      v147->mModelInfo = v126;
      v127 = v126;
      v125 = v147;
      qmemcpy(v127, v167[2], sizeof(sdModelInfo));
    }
    v125->mStatic = 0;
    v1->mpDeathLength->mStatic = 0;
  }
}
void __thiscall rvParticleTemplate::Duplicate(rvParticleTemplate *this, rvParticleTemplate *copy)
{
  rvParticleTemplate *v2; // ebp
  rvTrailInfo *v3; // esi
  rvTrailInfo *v4; // eax
  rvElectricityInfo *v5; // esi
  rvElectricityInfo *v6; // eax
  rvElectricityInfo *v7; // ecx
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v8; // ecx
  rvParticleParms *v9; // eax
  sdModelInfo *v10; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v11; // ecx
  rvParticleParms *v12; // eax
  sdModelInfo *v13; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v14; // ecx
  rvParticleParms *v15; // eax
  sdModelInfo *v16; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v17; // ecx
  rvParticleParms *v18; // eax
  sdModelInfo *v19; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v20; // ecx
  rvParticleParms *v21; // eax
  sdModelInfo *v22; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v23; // ecx
  rvParticleParms *v24; // eax
  sdModelInfo *v25; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v26; // ecx
  rvParticleParms *v27; // eax
  sdModelInfo *v28; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v29; // ecx
  rvParticleParms *v30; // eax
  sdModelInfo *v31; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v32; // ecx
  rvParticleParms *v33; // eax
  sdModelInfo *v34; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v35; // ecx
  rvParticleParms *v36; // eax
  sdModelInfo *v37; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v38; // ecx
  rvParticleParms *v39; // eax
  sdModelInfo *v40; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v41; // ecx
  rvParticleParms *v42; // eax
  sdModelInfo *v43; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v44; // ecx
  rvParticleParms *v45; // eax
  sdModelInfo *v46; // eax
  rvEnvParms *v47; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v48; // ecx
  rvEnvParms *v49; // eax
  rvEnvParms *v50; // ecx
  rvEnvParms *v51; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v52; // ecx
  rvEnvParms *v53; // eax
  rvEnvParms *v54; // ecx
  rvEnvParms *v55; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v56; // ecx
  rvEnvParms *v57; // eax
  rvEnvParms *v58; // ecx
  rvEnvParms *v59; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v60; // ecx
  rvEnvParms *v61; // eax
  rvEnvParms *v62; // ecx
  rvEnvParms *v63; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v64; // ecx
  rvEnvParms *v65; // eax
  rvEnvParms *v66; // ecx
  rvEnvParms *v67; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v68; // ecx
  rvEnvParms *v69; // eax
  rvEnvParms *v70; // ecx
  rvEnvParms *v71; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v72; // ecx
  rvEnvParms *v73; // eax
  rvEnvParms *v74; // ecx
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v75; // ecx
  rvParticleParms *v76; // eax
  sdModelInfo *v77; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v78; // ecx
  rvParticleParms *v79; // eax
  sdModelInfo *v80; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v81; // ecx
  rvParticleParms *v82; // eax
  sdModelInfo *v83; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v84; // ecx
  rvParticleParms *v85; // eax
  sdModelInfo *v86; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v87; // ecx
  rvParticleParms *v88; // eax
  sdModelInfo *v89; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v90; // ecx
  rvParticleParms *v91; // eax
  sdModelInfo *v92; // eax
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v93; // ecx
  rvParticleParms *v94; // eax
  rvParticleParms *v95; // eax
  sdModelInfo *v96; // eax
  sdModelInfo *v97; // edi
  rvParticleParms *old; // [esp+10h] [ebp-8h]
  rvParticleParms *olda; // [esp+10h] [ebp-8h]
  rvParticleParms *oldb; // [esp+10h] [ebp-8h]
  rvParticleParms *oldc; // [esp+10h] [ebp-8h]
  rvParticleParms *oldd; // [esp+10h] [ebp-8h]
  rvParticleParms *olde; // [esp+10h] [ebp-8h]
  rvParticleParms *oldf; // [esp+10h] [ebp-8h]
  rvParticleParms *oldg; // [esp+10h] [ebp-8h]
  rvParticleParms *oldh; // [esp+10h] [ebp-8h]
  rvParticleParms *oldi; // [esp+10h] [ebp-8h]
  rvParticleParms *oldj; // [esp+10h] [ebp-8h]
  rvParticleParms *oldk; // [esp+10h] [ebp-8h]
  rvParticleParms *oldl; // [esp+10h] [ebp-8h]
  rvParticleParms *oldm; // [esp+10h] [ebp-8h]
  rvParticleParms *oldn; // [esp+10h] [ebp-8h]
  rvParticleParms *oldo; // [esp+10h] [ebp-8h]
  rvParticleParms *oldp; // [esp+10h] [ebp-8h]
  rvParticleParms *oldq; // [esp+10h] [ebp-8h]
  rvParticleParms *oldr; // [esp+10h] [ebp-8h]
  rvParticleParms *olds; // [esp+10h] [ebp-8h]
  const void **v118; // [esp+14h] [ebp-4h]
  const void **v119; // [esp+14h] [ebp-4h]
  const void **v120; // [esp+14h] [ebp-4h]
  const void **v121; // [esp+14h] [ebp-4h]
  const void **v122; // [esp+14h] [ebp-4h]
  const void **v123; // [esp+14h] [ebp-4h]
  const void **v124; // [esp+14h] [ebp-4h]
  const void **v125; // [esp+14h] [ebp-4h]
  const void **v126; // [esp+14h] [ebp-4h]
  const void **v127; // [esp+14h] [ebp-4h]
  const void **v128; // [esp+14h] [ebp-4h]
  const void **v129; // [esp+14h] [ebp-4h]
  const void **v130; // [esp+14h] [ebp-4h]
  const void **v131; // [esp+14h] [ebp-4h]
  const void **v132; // [esp+14h] [ebp-4h]
  const void **v133; // [esp+14h] [ebp-4h]
  const void **v134; // [esp+14h] [ebp-4h]
  const void **v135; // [esp+14h] [ebp-4h]
  const void **v136; // [esp+14h] [ebp-4h]
  const void **v137; // [esp+14h] [ebp-4h]

  v2 = this;
  rvParticleTemplate::InitStatic();
  if ( !(v2->mFlags & 0x8000000) )
    rvParticleTemplate::Init(v2);
  v3 = v2->mTrailInfo;
  v4 = (rvTrailInfo *)operator new(0x3Cu);
  if ( v4 )
  {
    v4->mStatic = 0;
    v4->mTrailTypeName.len = 0;
    v4->mTrailTypeName.alloced = -20;
    v4->mTrailTypeName.data = v4->mTrailTypeName.baseBuffer;
    v4->mTrailTypeName.baseBuffer[0] = 0;
  }
  else
  {
    v4 = 0;
  }
  v2->mTrailInfo = v4;
  rvTrailInfo::operator=(v4, v3);
  v2->mTrailInfo->mStatic = 0;
  v5 = v2->mElecInfo;
  v6 = (rvElectricityInfo *)operator new(0x34u);
  if ( v6 )
  {
    v6->mStatic = 0;
    v7 = v6;
  }
  else
  {
    v7 = 0;
  }
  v2->mElecInfo = v7;
  rvElectricityInfo::operator=(v7, v5);
  v2->mElecInfo->mStatic = 0;
  v118 = (const void **)v2->mpSpawnPosition;
  v8 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v8 && (v9 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v8)) != 0 )
  {
    v9->mModelInfo = 0;
    v9->mStatic = 0;
    old = v9;
  }
  else
  {
    old = 0;
  }
  v2->mpSpawnPosition = old;
  qmemcpy(old, v118, sizeof(rvParticleParms));
  if ( v118[2] )
  {
    v10 = (sdModelInfo *)operator new(0x2Cu);
    old->mModelInfo = v10;
    qmemcpy(v10, v118[2], sizeof(sdModelInfo));
  }
  old->mStatic = 0;
  v2->mpSpawnPosition->mStatic = 0;
  v119 = (const void **)v2->mpSpawnDirection;
  v11 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v11 && (v12 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v11)) != 0 )
  {
    v12->mModelInfo = 0;
    v12->mStatic = 0;
    olda = v12;
  }
  else
  {
    olda = 0;
  }
  v2->mpSpawnDirection = olda;
  qmemcpy(olda, v119, sizeof(rvParticleParms));
  if ( v119[2] )
  {
    v13 = (sdModelInfo *)operator new(0x2Cu);
    olda->mModelInfo = v13;
    qmemcpy(v13, v119[2], sizeof(sdModelInfo));
  }
  olda->mStatic = 0;
  v2->mpSpawnDirection->mStatic = 0;
  v120 = (const void **)v2->mpSpawnVelocity;
  v14 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v14 && (v15 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v14)) != 0 )
  {
    v15->mModelInfo = 0;
    v15->mStatic = 0;
    oldb = v15;
  }
  else
  {
    oldb = 0;
  }
  v2->mpSpawnVelocity = oldb;
  qmemcpy(oldb, v120, sizeof(rvParticleParms));
  if ( v120[2] )
  {
    v16 = (sdModelInfo *)operator new(0x2Cu);
    oldb->mModelInfo = v16;
    qmemcpy(v16, v120[2], sizeof(sdModelInfo));
  }
  oldb->mStatic = 0;
  v2->mpSpawnVelocity->mStatic = 0;
  v121 = (const void **)v2->mpSpawnAcceleration;
  v17 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v17 && (v18 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v17)) != 0 )
  {
    v18->mModelInfo = 0;
    v18->mStatic = 0;
    oldc = v18;
  }
  else
  {
    oldc = 0;
  }
  v2->mpSpawnAcceleration = oldc;
  qmemcpy(oldc, v121, sizeof(rvParticleParms));
  if ( v121[2] )
  {
    v19 = (sdModelInfo *)operator new(0x2Cu);
    oldc->mModelInfo = v19;
    qmemcpy(v19, v121[2], sizeof(sdModelInfo));
  }
  oldc->mStatic = 0;
  v2->mpSpawnAcceleration->mStatic = 0;
  v122 = (const void **)v2->mpSpawnFriction;
  v20 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v20 && (v21 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v20)) != 0 )
  {
    v21->mModelInfo = 0;
    v21->mStatic = 0;
    oldd = v21;
  }
  else
  {
    oldd = 0;
  }
  v2->mpSpawnFriction = oldd;
  qmemcpy(oldd, v122, sizeof(rvParticleParms));
  if ( v122[2] )
  {
    v22 = (sdModelInfo *)operator new(0x2Cu);
    oldd->mModelInfo = v22;
    qmemcpy(v22, v122[2], sizeof(sdModelInfo));
  }
  oldd->mStatic = 0;
  v2->mpSpawnFriction->mStatic = 0;
  v123 = (const void **)v2->mpSpawnTint;
  v23 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v23 && (v24 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v23)) != 0 )
  {
    v24->mModelInfo = 0;
    v24->mStatic = 0;
    olde = v24;
  }
  else
  {
    olde = 0;
  }
  v2->mpSpawnTint = olde;
  qmemcpy(olde, v123, sizeof(rvParticleParms));
  if ( v123[2] )
  {
    v25 = (sdModelInfo *)operator new(0x2Cu);
    olde->mModelInfo = v25;
    qmemcpy(v25, v123[2], sizeof(sdModelInfo));
  }
  olde->mStatic = 0;
  v2->mpSpawnTint->mStatic = 0;
  v124 = (const void **)v2->mpSpawnFade;
  v26 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v26 && (v27 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v26)) != 0 )
  {
    v27->mModelInfo = 0;
    v27->mStatic = 0;
    oldf = v27;
  }
  else
  {
    oldf = 0;
  }
  v2->mpSpawnFade = oldf;
  qmemcpy(oldf, v124, sizeof(rvParticleParms));
  if ( v124[2] )
  {
    v28 = (sdModelInfo *)operator new(0x2Cu);
    oldf->mModelInfo = v28;
    qmemcpy(v28, v124[2], sizeof(sdModelInfo));
  }
  oldf->mStatic = 0;
  v2->mpSpawnFade->mStatic = 0;
  v125 = (const void **)v2->mpSpawnSize;
  v29 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v29 && (v30 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v29)) != 0 )
  {
    v30->mModelInfo = 0;
    v30->mStatic = 0;
    oldg = v30;
  }
  else
  {
    oldg = 0;
  }
  v2->mpSpawnSize = oldg;
  qmemcpy(oldg, v125, sizeof(rvParticleParms));
  if ( v125[2] )
  {
    v31 = (sdModelInfo *)operator new(0x2Cu);
    oldg->mModelInfo = v31;
    qmemcpy(v31, v125[2], sizeof(sdModelInfo));
  }
  oldg->mStatic = 0;
  v2->mpSpawnSize->mStatic = 0;
  v126 = (const void **)v2->mpSpawnRotate;
  v32 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v32 && (v33 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v32)) != 0 )
  {
    v33->mModelInfo = 0;
    v33->mStatic = 0;
    oldh = v33;
  }
  else
  {
    oldh = 0;
  }
  v2->mpSpawnRotate = oldh;
  qmemcpy(oldh, v126, sizeof(rvParticleParms));
  if ( v126[2] )
  {
    v34 = (sdModelInfo *)operator new(0x2Cu);
    oldh->mModelInfo = v34;
    qmemcpy(v34, v126[2], sizeof(sdModelInfo));
  }
  oldh->mStatic = 0;
  v2->mpSpawnRotate->mStatic = 0;
  v127 = (const void **)v2->mpSpawnAngle;
  v35 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v35 && (v36 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v35)) != 0 )
  {
    v36->mModelInfo = 0;
    v36->mStatic = 0;
    oldi = v36;
  }
  else
  {
    oldi = 0;
  }
  v2->mpSpawnAngle = oldi;
  qmemcpy(oldi, v127, sizeof(rvParticleParms));
  if ( v127[2] )
  {
    v37 = (sdModelInfo *)operator new(0x2Cu);
    oldi->mModelInfo = v37;
    qmemcpy(v37, v127[2], sizeof(sdModelInfo));
  }
  oldi->mStatic = 0;
  v2->mpSpawnAngle->mStatic = 0;
  v128 = (const void **)v2->mpSpawnOffset;
  v38 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v38 && (v39 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v38)) != 0 )
  {
    v39->mModelInfo = 0;
    v39->mStatic = 0;
    oldj = v39;
  }
  else
  {
    oldj = 0;
  }
  v2->mpSpawnOffset = oldj;
  qmemcpy(oldj, v128, sizeof(rvParticleParms));
  if ( v128[2] )
  {
    v40 = (sdModelInfo *)operator new(0x2Cu);
    oldj->mModelInfo = v40;
    qmemcpy(v40, v128[2], sizeof(sdModelInfo));
  }
  oldj->mStatic = 0;
  v2->mpSpawnOffset->mStatic = 0;
  v129 = (const void **)v2->mpSpawnLength;
  v41 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v41 && (v42 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v41)) != 0 )
  {
    v42->mModelInfo = 0;
    v42->mStatic = 0;
    oldk = v42;
  }
  else
  {
    oldk = 0;
  }
  v2->mpSpawnLength = oldk;
  qmemcpy(oldk, v129, sizeof(rvParticleParms));
  if ( v129[2] )
  {
    v43 = (sdModelInfo *)operator new(0x2Cu);
    oldk->mModelInfo = v43;
    qmemcpy(v43, v129[2], sizeof(sdModelInfo));
  }
  oldk->mStatic = 0;
  v2->mpSpawnLength->mStatic = 0;
  v130 = (const void **)v2->mpSpawnWindStrength;
  v44 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v44 && (v45 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v44)) != 0 )
  {
    v45->mModelInfo = 0;
    v45->mStatic = 0;
    oldl = v45;
  }
  else
  {
    oldl = 0;
  }
  v2->mpSpawnWindStrength = oldl;
  qmemcpy(oldl, v130, sizeof(rvParticleParms));
  if ( v130[2] )
  {
    v46 = (sdModelInfo *)operator new(0x2Cu);
    oldl->mModelInfo = v46;
    qmemcpy(v46, v130[2], sizeof(sdModelInfo));
  }
  oldl->mStatic = 0;
  v2->mpSpawnWindStrength->mStatic = 0;
  v47 = v2->mpTintEnvelope;
  v48 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v48 && (v49 = sdDetails::sdPoolAlloc<rvEnvParms,128>::Alloc(v48)) != 0 )
  {
    v49->mStatic = 0;
    v49->mFastLookUp = 0;
    v50 = v49;
  }
  else
  {
    v50 = 0;
  }
  v2->mpTintEnvelope = v50;
  rvEnvParms::operator=(v50, v47);
  v2->mpTintEnvelope->mStatic = 0;
  v2->mpTintEnvelope->mFastLookUp = 0;
  v51 = v2->mpFadeEnvelope;
  v52 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v52 && (v53 = sdDetails::sdPoolAlloc<rvEnvParms,128>::Alloc(v52)) != 0 )
  {
    v53->mStatic = 0;
    v53->mFastLookUp = 0;
    v54 = v53;
  }
  else
  {
    v54 = 0;
  }
  v2->mpFadeEnvelope = v54;
  rvEnvParms::operator=(v54, v51);
  v2->mpFadeEnvelope->mStatic = 0;
  v2->mpFadeEnvelope->mFastLookUp = 0;
  v55 = v2->mpSizeEnvelope;
  v56 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v56 && (v57 = sdDetails::sdPoolAlloc<rvEnvParms,128>::Alloc(v56)) != 0 )
  {
    v57->mStatic = 0;
    v57->mFastLookUp = 0;
    v58 = v57;
  }
  else
  {
    v58 = 0;
  }
  v2->mpSizeEnvelope = v58;
  rvEnvParms::operator=(v58, v55);
  v2->mpSizeEnvelope->mStatic = 0;
  v2->mpSizeEnvelope->mFastLookUp = 0;
  v59 = v2->mpRotateEnvelope;
  v60 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v60 && (v61 = sdDetails::sdPoolAlloc<rvEnvParms,128>::Alloc(v60)) != 0 )
  {
    v61->mStatic = 0;
    v61->mFastLookUp = 0;
    v62 = v61;
  }
  else
  {
    v62 = 0;
  }
  v2->mpRotateEnvelope = v62;
  rvEnvParms::operator=(v62, v59);
  v2->mpRotateEnvelope->mStatic = 0;
  v2->mpRotateEnvelope->mFastLookUp = 0;
  v63 = v2->mpAngleEnvelope;
  v64 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v64 && (v65 = sdDetails::sdPoolAlloc<rvEnvParms,128>::Alloc(v64)) != 0 )
  {
    v65->mStatic = 0;
    v65->mFastLookUp = 0;
    v66 = v65;
  }
  else
  {
    v66 = 0;
  }
  v2->mpAngleEnvelope = v66;
  rvEnvParms::operator=(v66, v63);
  v2->mpAngleEnvelope->mStatic = 0;
  v2->mpAngleEnvelope->mFastLookUp = 0;
  v67 = v2->mpOffsetEnvelope;
  v68 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v68 && (v69 = sdDetails::sdPoolAlloc<rvEnvParms,128>::Alloc(v68)) != 0 )
  {
    v69->mStatic = 0;
    v69->mFastLookUp = 0;
    v70 = v69;
  }
  else
  {
    v70 = 0;
  }
  v2->mpOffsetEnvelope = v70;
  rvEnvParms::operator=(v70, v67);
  v2->mpOffsetEnvelope->mStatic = 0;
  v2->mpOffsetEnvelope->mFastLookUp = 0;
  v71 = v2->mpLengthEnvelope;
  v72 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v72 && (v73 = sdDetails::sdPoolAlloc<rvEnvParms,128>::Alloc(v72)) != 0 )
  {
    v73->mStatic = 0;
    v73->mFastLookUp = 0;
    v74 = v73;
  }
  else
  {
    v74 = 0;
  }
  v2->mpLengthEnvelope = v74;
  rvEnvParms::operator=(v74, v71);
  v2->mpLengthEnvelope->mStatic = 0;
  v2->mpLengthEnvelope->mFastLookUp = 0;
  v131 = (const void **)v2->mpDeathTint;
  v75 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v75 && (v76 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v75)) != 0 )
  {
    v76->mModelInfo = 0;
    v76->mStatic = 0;
    oldm = v76;
  }
  else
  {
    oldm = 0;
  }
  v2->mpDeathTint = oldm;
  qmemcpy(oldm, v131, sizeof(rvParticleParms));
  if ( v131[2] )
  {
    v77 = (sdModelInfo *)operator new(0x2Cu);
    oldm->mModelInfo = v77;
    qmemcpy(v77, v131[2], sizeof(sdModelInfo));
  }
  oldm->mStatic = 0;
  v2->mpDeathTint->mStatic = 0;
  v132 = (const void **)v2->mpDeathFade;
  v78 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v78 && (v79 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v78)) != 0 )
  {
    v79->mModelInfo = 0;
    v79->mStatic = 0;
    oldn = v79;
  }
  else
  {
    oldn = 0;
  }
  v2->mpDeathFade = oldn;
  qmemcpy(oldn, v132, sizeof(rvParticleParms));
  if ( v132[2] )
  {
    v80 = (sdModelInfo *)operator new(0x2Cu);
    oldn->mModelInfo = v80;
    qmemcpy(v80, v132[2], sizeof(sdModelInfo));
  }
  oldn->mStatic = 0;
  v2->mpDeathFade->mStatic = 0;
  v133 = (const void **)v2->mpDeathSize;
  v81 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v81 && (v82 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v81)) != 0 )
  {
    v82->mModelInfo = 0;
    v82->mStatic = 0;
    oldo = v82;
  }
  else
  {
    oldo = 0;
  }
  v2->mpDeathSize = oldo;
  qmemcpy(oldo, v133, sizeof(rvParticleParms));
  if ( v133[2] )
  {
    v83 = (sdModelInfo *)operator new(0x2Cu);
    oldo->mModelInfo = v83;
    qmemcpy(v83, v133[2], sizeof(sdModelInfo));
  }
  oldo->mStatic = 0;
  v2->mpDeathSize->mStatic = 0;
  v134 = (const void **)v2->mpDeathRotate;
  v84 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v84 && (v85 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v84)) != 0 )
  {
    v85->mModelInfo = 0;
    v85->mStatic = 0;
    oldp = v85;
  }
  else
  {
    oldp = 0;
  }
  v2->mpDeathRotate = oldp;
  qmemcpy(oldp, v134, sizeof(rvParticleParms));
  if ( v134[2] )
  {
    v86 = (sdModelInfo *)operator new(0x2Cu);
    oldp->mModelInfo = v86;
    qmemcpy(v86, v134[2], sizeof(sdModelInfo));
  }
  oldp->mStatic = 0;
  v2->mpDeathRotate->mStatic = 0;
  v135 = (const void **)v2->mpDeathAngle;
  v87 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v87 && (v88 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v87)) != 0 )
  {
    v88->mModelInfo = 0;
    v88->mStatic = 0;
    oldq = v88;
  }
  else
  {
    oldq = 0;
  }
  v2->mpDeathAngle = oldq;
  qmemcpy(oldq, v135, sizeof(rvParticleParms));
  if ( v135[2] )
  {
    v89 = (sdModelInfo *)operator new(0x2Cu);
    oldq->mModelInfo = v89;
    qmemcpy(v89, v135[2], sizeof(sdModelInfo));
  }
  oldq->mStatic = 0;
  v2->mpDeathAngle->mStatic = 0;
  v136 = (const void **)v2->mpDeathOffset;
  v90 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v90 && (v91 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v90)) != 0 )
  {
    v91->mModelInfo = 0;
    v91->mStatic = 0;
    oldr = v91;
  }
  else
  {
    oldr = 0;
  }
  v2->mpDeathOffset = oldr;
  qmemcpy(oldr, v136, sizeof(rvParticleParms));
  if ( v136[2] )
  {
    v92 = (sdModelInfo *)operator new(0x2Cu);
    oldr->mModelInfo = v92;
    qmemcpy(v92, v136[2], sizeof(sdModelInfo));
  }
  oldr->mStatic = 0;
  v2->mpDeathOffset->mStatic = 0;
  v137 = (const void **)v2->mpDeathLength;
  v93 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
  if ( v93 && (v94 = sdDetails::sdPoolAlloc<rvParticleParms,128>::Alloc(v93)) != 0 )
  {
    v94->mModelInfo = 0;
    v94->mStatic = 0;
    olds = v94;
  }
  else
  {
    olds = 0;
  }
  v95 = olds;
  v2->mpDeathLength = olds;
  qmemcpy(olds, v137, sizeof(rvParticleParms));
  if ( v137[2] )
  {
    v96 = (sdModelInfo *)operator new(0x2Cu);
    olds->mModelInfo = v96;
    v97 = v96;
    v95 = olds;
    qmemcpy(v97, v137[2], sizeof(sdModelInfo));
  }
  v95->mStatic = 0;
  v2->mpDeathLength->mStatic = 0;
}
void __thiscall rvParticleTemplate::Purge(rvParticleTemplate *this)
{
  rvParticleTemplate *v1; // ebx
  rvTrailInfo *v2; // edi
  rvParticleParms *v3; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v4; // eax
  rvParticleParms *v5; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v6; // eax
  rvParticleParms *v7; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v8; // eax
  rvParticleParms *v9; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v10; // eax
  rvParticleParms *v11; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v12; // eax
  rvParticleParms *v13; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v14; // eax
  rvParticleParms *v15; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v16; // eax
  rvParticleParms *v17; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v18; // eax
  rvParticleParms *v19; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v20; // eax
  rvParticleParms *v21; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v22; // eax
  rvParticleParms *v23; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v24; // eax
  rvParticleParms *v25; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v26; // eax
  rvParticleParms *v27; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v28; // eax
  rvEnvParms *v29; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v30; // eax
  rvEnvParms *v31; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v32; // eax
  rvEnvParms *v33; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v34; // eax
  rvEnvParms *v35; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v36; // eax
  rvEnvParms *v37; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v38; // eax
  rvEnvParms *v39; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v40; // eax
  rvEnvParms *v41; // esi
  sdDetails::sdPoolAlloc<rvEnvParms,128> *v42; // eax
  rvParticleParms *v43; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v44; // eax
  rvParticleParms *v45; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v46; // eax
  rvParticleParms *v47; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v48; // eax
  rvParticleParms *v49; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v50; // eax
  rvParticleParms *v51; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v52; // eax
  rvParticleParms *v53; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v54; // eax
  rvParticleParms *v55; // esi
  sdDetails::sdPoolAlloc<rvParticleParms,128> *v56; // eax

  v1 = this;
  if ( this->mFlags & 0x8000000 )
  {
    v2 = this->mTrailInfo;
    if ( !v2->mStatic )
    {
      if ( v2 )
      {
        idStr::FreeData(&v2->mTrailTypeName);
        v2->mTrailTypeName.len = 0;
        v2->mTrailTypeName.alloced = -20;
        v2->mTrailTypeName.data = v2->mTrailTypeName.baseBuffer;
        v2->mTrailTypeName.baseBuffer[0] = 0;
        operator delete(v2);
      }
      v1->mTrailInfo = &rvParticleTemplate::sTrailInfo;
    }
    if ( !v1->mElecInfo->mStatic )
    {
      operator delete(v1->mElecInfo);
      v1->mElecInfo = &rvParticleTemplate::sElectricityInfo;
    }
    v3 = v1->mpSpawnPosition;
    if ( !v3->mStatic )
    {
      if ( v3 )
      {
        operator delete(v3->mModelInfo);
        v4 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v4 )
        {
          LODWORD(v3[-1].mMaxs.z) = v4->free;
          --v4->active;
          ++v4->numFree;
          v4->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v3[-1].mMaxs.z;
        }
      }
      v1->mpSpawnPosition = &rvParticleTemplate::sSPF_NONE_3;
    }
    v5 = v1->mpSpawnDirection;
    if ( !v5->mStatic )
    {
      if ( v5 )
      {
        operator delete(v5->mModelInfo);
        v6 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v6 )
        {
          LODWORD(v5[-1].mMaxs.z) = v6->free;
          --v6->active;
          ++v6->numFree;
          v6->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v5[-1].mMaxs.z;
        }
      }
      v1->mpSpawnDirection = &rvParticleTemplate::sSPF_NONE_3;
    }
    v7 = v1->mpSpawnVelocity;
    if ( !v7->mStatic )
    {
      if ( v7 )
      {
        operator delete(v7->mModelInfo);
        v8 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v8 )
        {
          LODWORD(v7[-1].mMaxs.z) = v8->free;
          --v8->active;
          ++v8->numFree;
          v8->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v7[-1].mMaxs.z;
        }
      }
      v1->mpSpawnVelocity = &rvParticleTemplate::sSPF_NONE_3;
    }
    v9 = v1->mpSpawnAcceleration;
    if ( !v9->mStatic )
    {
      if ( v9 )
      {
        operator delete(v9->mModelInfo);
        v10 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v10 )
        {
          LODWORD(v9[-1].mMaxs.z) = v10->free;
          --v10->active;
          ++v10->numFree;
          v10->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v9[-1].mMaxs.z;
        }
      }
      v1->mpSpawnAcceleration = &rvParticleTemplate::sSPF_NONE_3;
    }
    v11 = v1->mpSpawnFriction;
    if ( !v11->mStatic )
    {
      if ( v11 )
      {
        operator delete(v11->mModelInfo);
        v12 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v12 )
        {
          LODWORD(v11[-1].mMaxs.z) = v12->free;
          --v12->active;
          ++v12->numFree;
          v12->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v11[-1].mMaxs.z;
        }
      }
      v1->mpSpawnFriction = &rvParticleTemplate::sSPF_NONE_3;
    }
    v13 = v1->mpSpawnTint;
    if ( !v13->mStatic )
    {
      if ( v13 )
      {
        operator delete(v13->mModelInfo);
        v14 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v14 )
        {
          LODWORD(v13[-1].mMaxs.z) = v14->free;
          --v14->active;
          ++v14->numFree;
          v14->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v13[-1].mMaxs.z;
        }
      }
      v1->mpSpawnTint = &rvParticleTemplate::sSPF_ONE_3;
    }
    v15 = v1->mpSpawnFade;
    if ( !v15->mStatic )
    {
      if ( v15 )
      {
        operator delete(v15->mModelInfo);
        v16 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v16 )
        {
          LODWORD(v15[-1].mMaxs.z) = v16->free;
          --v16->active;
          ++v16->numFree;
          v16->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v15[-1].mMaxs.z;
        }
      }
      v1->mpSpawnFade = &rvParticleTemplate::sSPF_ONE_1;
    }
    v17 = v1->mpSpawnSize;
    if ( !v17->mStatic )
    {
      if ( v17 )
      {
        operator delete(v17->mModelInfo);
        v18 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v18 )
        {
          LODWORD(v17[-1].mMaxs.z) = v18->free;
          --v18->active;
          ++v18->numFree;
          v18->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v17[-1].mMaxs.z;
        }
      }
      v1->mpSpawnSize = &rvParticleTemplate::sSPF_ONE_3;
    }
    v19 = v1->mpSpawnRotate;
    if ( !v19->mStatic )
    {
      if ( v19 )
      {
        operator delete(v19->mModelInfo);
        v20 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v20 )
        {
          LODWORD(v19[-1].mMaxs.z) = v20->free;
          --v20->active;
          ++v20->numFree;
          v20->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v19[-1].mMaxs.z;
        }
      }
      v1->mpSpawnRotate = &rvParticleTemplate::sSPF_NONE_3;
    }
    v21 = v1->mpSpawnAngle;
    if ( !v21->mStatic )
    {
      if ( v21 )
      {
        operator delete(v21->mModelInfo);
        v22 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v22 )
        {
          LODWORD(v21[-1].mMaxs.z) = v22->free;
          --v22->active;
          ++v22->numFree;
          v22->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v21[-1].mMaxs.z;
        }
      }
      v1->mpSpawnAngle = &rvParticleTemplate::sSPF_NONE_3;
    }
    v23 = v1->mpSpawnOffset;
    if ( !v23->mStatic )
    {
      if ( v23 )
      {
        operator delete(v23->mModelInfo);
        v24 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v24 )
        {
          LODWORD(v23[-1].mMaxs.z) = v24->free;
          --v24->active;
          ++v24->numFree;
          v24->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v23[-1].mMaxs.z;
        }
      }
      v1->mpSpawnOffset = &rvParticleTemplate::sSPF_NONE_3;
    }
    v25 = v1->mpSpawnLength;
    if ( !v25->mStatic )
    {
      if ( v25 )
      {
        operator delete(v25->mModelInfo);
        v26 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v26 )
        {
          LODWORD(v25[-1].mMaxs.z) = v26->free;
          --v26->active;
          ++v26->numFree;
          v26->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v25[-1].mMaxs.z;
        }
      }
      v1->mpSpawnLength = &rvParticleTemplate::sSPF_NONE_3;
    }
    v27 = v1->mpSpawnWindStrength;
    if ( !v27->mStatic )
    {
      if ( v27 )
      {
        operator delete(v27->mModelInfo);
        v28 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v28 )
        {
          LODWORD(v27[-1].mMaxs.z) = v28->free;
          --v28->active;
          ++v28->numFree;
          v28->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v27[-1].mMaxs.z;
        }
      }
      v1->mpSpawnWindStrength = &rvParticleTemplate::sSPF_NONE_1;
    }
    v29 = v1->mpTintEnvelope;
    if ( !v29->mStatic )
    {
      if ( v29 )
      {
        v30 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v30 )
        {
          LODWORD(v29[-1].mRate.z) = v30->free;
          --v30->active;
          ++v30->numFree;
          v30->free = (sdDetails::sdPoolAlloc<rvEnvParms,128>::element_t *)&v29[-1].mRate.z;
        }
      }
      v1->mpTintEnvelope = &rvParticleTemplate::sEmptyEnvelope;
    }
    v31 = v1->mpFadeEnvelope;
    if ( !v31->mStatic )
    {
      if ( v31 )
      {
        v32 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v32 )
        {
          LODWORD(v31[-1].mRate.z) = v32->free;
          --v32->active;
          ++v32->numFree;
          v32->free = (sdDetails::sdPoolAlloc<rvEnvParms,128>::element_t *)&v31[-1].mRate.z;
        }
      }
      v1->mpFadeEnvelope = &rvParticleTemplate::sEmptyEnvelope;
    }
    v33 = v1->mpSizeEnvelope;
    if ( !v33->mStatic )
    {
      if ( v33 )
      {
        v34 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v34 )
        {
          LODWORD(v33[-1].mRate.z) = v34->free;
          --v34->active;
          ++v34->numFree;
          v34->free = (sdDetails::sdPoolAlloc<rvEnvParms,128>::element_t *)&v33[-1].mRate.z;
        }
      }
      v1->mpSizeEnvelope = &rvParticleTemplate::sEmptyEnvelope;
    }
    v35 = v1->mpRotateEnvelope;
    if ( !v35->mStatic )
    {
      if ( v35 )
      {
        v36 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v36 )
        {
          LODWORD(v35[-1].mRate.z) = v36->free;
          --v36->active;
          ++v36->numFree;
          v36->free = (sdDetails::sdPoolAlloc<rvEnvParms,128>::element_t *)&v35[-1].mRate.z;
        }
      }
      v1->mpRotateEnvelope = &rvParticleTemplate::sEmptyEnvelope;
    }
    v37 = v1->mpAngleEnvelope;
    if ( !v37->mStatic )
    {
      if ( v37 )
      {
        v38 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v38 )
        {
          LODWORD(v37[-1].mRate.z) = v38->free;
          --v38->active;
          ++v38->numFree;
          v38->free = (sdDetails::sdPoolAlloc<rvEnvParms,128>::element_t *)&v37[-1].mRate.z;
        }
      }
      v1->mpAngleEnvelope = &rvParticleTemplate::sEmptyEnvelope;
    }
    v39 = v1->mpOffsetEnvelope;
    if ( !v39->mStatic )
    {
      if ( v39 )
      {
        v40 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v40 )
        {
          LODWORD(v39[-1].mRate.z) = v40->free;
          --v40->active;
          ++v40->numFree;
          v40->free = (sdDetails::sdPoolAlloc<rvEnvParms,128>::element_t *)&v39[-1].mRate.z;
        }
      }
      v1->mpOffsetEnvelope = &rvParticleTemplate::sEmptyEnvelope;
    }
    v41 = v1->mpLengthEnvelope;
    if ( !v41->mStatic )
    {
      if ( v41 )
      {
        v42 = sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v42 )
        {
          LODWORD(v41[-1].mRate.z) = v42->free;
          --v42->active;
          ++v42->numFree;
          v42->free = (sdDetails::sdPoolAlloc<rvEnvParms,128>::element_t *)&v41[-1].mRate.z;
        }
      }
      v1->mpLengthEnvelope = &rvParticleTemplate::sEmptyEnvelope;
    }
    v43 = v1->mpDeathTint;
    if ( !v43->mStatic )
    {
      if ( v43 )
      {
        operator delete(v43->mModelInfo);
        v44 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v44 )
        {
          LODWORD(v43[-1].mMaxs.z) = v44->free;
          --v44->active;
          ++v44->numFree;
          v44->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v43[-1].mMaxs.z;
        }
      }
      v1->mpDeathTint = &rvParticleTemplate::sSPF_NONE_3;
    }
    v45 = v1->mpDeathFade;
    if ( !v45->mStatic )
    {
      if ( v45 )
      {
        operator delete(v45->mModelInfo);
        v46 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v46 )
        {
          LODWORD(v45[-1].mMaxs.z) = v46->free;
          --v46->active;
          ++v46->numFree;
          v46->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v45[-1].mMaxs.z;
        }
      }
      v1->mpDeathFade = &rvParticleTemplate::sSPF_NONE_1;
    }
    v47 = v1->mpDeathSize;
    if ( !v47->mStatic )
    {
      if ( v47 )
      {
        operator delete(v47->mModelInfo);
        v48 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v48 )
        {
          LODWORD(v47[-1].mMaxs.z) = v48->free;
          --v48->active;
          ++v48->numFree;
          v48->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v47[-1].mMaxs.z;
        }
      }
      v1->mpDeathSize = &rvParticleTemplate::sSPF_ONE_3;
    }
    v49 = v1->mpDeathRotate;
    if ( !v49->mStatic )
    {
      if ( v49 )
      {
        operator delete(v49->mModelInfo);
        v50 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v50 )
        {
          LODWORD(v49[-1].mMaxs.z) = v50->free;
          --v50->active;
          ++v50->numFree;
          v50->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v49[-1].mMaxs.z;
        }
      }
      v1->mpDeathRotate = &rvParticleTemplate::sSPF_NONE_3;
    }
    v51 = v1->mpDeathAngle;
    if ( !v51->mStatic )
    {
      if ( v51 )
      {
        operator delete(v51->mModelInfo);
        v52 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v52 )
        {
          LODWORD(v51[-1].mMaxs.z) = v52->free;
          --v52->active;
          ++v52->numFree;
          v52->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v51[-1].mMaxs.z;
        }
      }
      v1->mpDeathAngle = &rvParticleTemplate::sSPF_NONE_3;
    }
    v53 = v1->mpDeathOffset;
    if ( !v53->mStatic )
    {
      if ( v53 )
      {
        operator delete(v53->mModelInfo);
        v54 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v54 )
        {
          LODWORD(v53[-1].mMaxs.z) = v54->free;
          --v54->active;
          ++v54->numFree;
          v54->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v53[-1].mMaxs.z;
        }
      }
      v1->mpDeathOffset = &rvParticleTemplate::sSPF_NONE_3;
    }
    v55 = v1->mpDeathLength;
    if ( !v55->mStatic )
    {
      if ( v55 )
      {
        operator delete(v55->mModelInfo);
        v56 = sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager()->allocator;
        if ( v56 )
        {
          LODWORD(v55[-1].mMaxs.z) = v56->free;
          --v56->active;
          ++v56->numFree;
          v56->free = (sdDetails::sdPoolAlloc<rvParticleParms,128>::element_t *)&v55[-1].mMaxs.z;
        }
      }
      v1->mpDeathLength = &rvParticleTemplate::sSPF_NONE_3;
    }
  }
}
int dynamic_initializer_for__singletonLock___107()
{
  sdLock::sdLock(&singletonLock_107);
  return atexit(dynamic_atexit_destructor_for__singletonLock___107);
}
int dynamic_initializer_for__rvParticleTemplate::sTrailInfo__()
{
  return atexit(dynamic_atexit_destructor_for__rvParticleTemplate::sTrailInfo__);
}
int dynamic_initializer_for__rvParticleTemplate::sDefaultEnvelope__()
{
  return atexit(dynamic_atexit_destructor_for__rvParticleTemplate::sDefaultEnvelope__);
}
int dynamic_initializer_for__rvParticleTemplate::sEmptyEnvelope__()
{
  return atexit(dynamic_atexit_destructor_for__rvParticleTemplate::sEmptyEnvelope__);
}
int dynamic_initializer_for__rvParticleTemplate::sSPF_ONE_1__()
{
  return atexit(dynamic_atexit_destructor_for__rvParticleTemplate::sSPF_ONE_1__);
}
int dynamic_initializer_for__rvParticleTemplate::sSPF_ONE_2__()
{
  return atexit(dynamic_atexit_destructor_for__rvParticleTemplate::sSPF_ONE_2__);
}
int dynamic_initializer_for__rvParticleTemplate::sSPF_ONE_3__()
{
  return atexit(dynamic_atexit_destructor_for__rvParticleTemplate::sSPF_ONE_3__);
}
int dynamic_initializer_for__rvParticleTemplate::sSPF_NONE_0__()
{
  return atexit(dynamic_atexit_destructor_for__rvParticleTemplate::sSPF_NONE_0__);
}
int dynamic_initializer_for__rvParticleTemplate::sSPF_NONE_1__()
{
  return atexit(dynamic_atexit_destructor_for__rvParticleTemplate::sSPF_NONE_1__);
}
int dynamic_initializer_for__rvParticleTemplate::sSPF_NONE_3__()
{
  return atexit(dynamic_atexit_destructor_for__rvParticleTemplate::sSPF_NONE_3__);
}
void __cdecl dynamic_atexit_destructor_for__singletonLock___106()
{
  sdLock::~sdLock(&singletonLock_106);
}
void __cdecl dynamic_atexit_destructor_for__singletonLock___106()
{
  sdLock::~sdLock(&singletonLock_106);
}
void __cdecl dynamic_atexit_destructor_for__rvParticleTemplate::sSPF_ONE_1__()
{
  operator delete(rvParticleTemplate::sSPF_ONE_1.mModelInfo);
}
void __cdecl dynamic_atexit_destructor_for__rvParticleTemplate::sSPF_ONE_2__()
{
  operator delete(rvParticleTemplate::sSPF_ONE_2.mModelInfo);
}
void __cdecl dynamic_atexit_destructor_for__rvParticleTemplate::sSPF_ONE_3__()
{
  operator delete(rvParticleTemplate::sSPF_ONE_3.mModelInfo);
}
void __cdecl dynamic_atexit_destructor_for__rvParticleTemplate::sSPF_NONE_0__()
{
  operator delete(rvParticleTemplate::sSPF_NONE_0.mModelInfo);
}
void __cdecl dynamic_atexit_destructor_for__rvParticleTemplate::sSPF_NONE_1__()
{
  operator delete(rvParticleTemplate::sSPF_NONE_1.mModelInfo);
}
void __cdecl dynamic_atexit_destructor_for__rvParticleTemplate::sSPF_NONE_3__()
{
  operator delete(rvParticleTemplate::sSPF_NONE_3.mModelInfo);
}
void __cdecl `sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager'::`2'::`dynamic atexit destructor for 'manager''()
{
  `sdPoolAllocator<rvParticleParms,&char const * const sdPoolAllocator_rvParticleParms,128,sdLockingPolicy_None>::GetMemoryManager'::`2'::manager.vfptr = (sdDynamicBlockManagerBaseVtbl *)&sdDynamicBlockManagerBase::`vftable';
}
void __cdecl `sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager'::`2'::`dynamic atexit destructor for 'manager''()
{
  `sdPoolAllocator<rvEnvParms,&char const * const sdPoolAllocator_rvEnvParms,128,sdLockingPolicy_None>::GetMemoryManager'::`2'::manager.vfptr = (sdDynamicBlockManagerBaseVtbl *)&sdDynamicBlockManagerBase::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__singletonLock___107()
{
  sdLock::~sdLock(&singletonLock_107);
}
void __cdecl dynamic_atexit_destructor_for__rvParticleTemplate::sTrailInfo__()
{
  idStr::FreeData(&stru_7E672C);
  stru_7E672C.len = 0;
  stru_7E672C.alloced = -20;
  stru_7E672C.data = stru_7E672C.baseBuffer;
  stru_7E672C.baseBuffer[0] = 0;
}
