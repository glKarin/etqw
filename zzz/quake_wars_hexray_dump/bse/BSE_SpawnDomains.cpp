void __cdecl SpawnGetNormal(idVec3 *normal, idVec3 *result, idVec3 *centre)
{
  idVec3 *v3; // esi
  float v4; // [esp+4h] [ebp-Ch]
  float v5; // [esp+8h] [ebp-8h]
  float v6; // [esp+Ch] [ebp-4h]
  float normalc; // [esp+14h] [ebp+4h]
  float normala; // [esp+14h] [ebp+4h]
  float normald; // [esp+14h] [ebp+4h]
  float normale; // [esp+14h] [ebp+4h]
  float normalf; // [esp+14h] [ebp+4h]
  float normalb; // [esp+14h] [ebp+4h]
  float normalg; // [esp+14h] [ebp+4h]

  v3 = normal;
  if ( normal )
  {
    if ( centre )
    {
      v4 = result->x - centre->x;
      v5 = result->y - centre->y;
      v6 = result->z - centre->z;
      normal->x = v4;
      normal->y = v5;
      normal->z = v6;
      normalc = v6 * v6 + v5 * v5 + v4 * v4;
      normala = sqrt(normalc);
      if ( normala >= 0.00000011920929 )
      {
        normald = 1.0 / normala;
        v3->x = v4 * normald;
        v3->y = v5 * normald;
        v3->z = normald * v6;
      }
    }
    else
    {
      normal->x = result->x;
      normal->y = result->y;
      normale = result->z;
      v3->z = normale;
      normalf = normale * normale + v3->y * v3->y + v3->x * v3->x;
      normalb = sqrt(normalf);
      if ( normalb >= 0.00000011920929 )
      {
        normalg = 1.0 / normalb;
        v3->x = v3->x * normalg;
        v3->y = v3->y * normalg;
        v3->z = normalg * v3->z;
      }
    }
  }
}
void __cdecl SpawnNone1(float *result)
{
  *result = 0.0;
}
void __cdecl SpawnNone2(float *result)
{
  *result = 0.0;
  result[1] = 0.0;
}
void __cdecl SpawnNone3(float *result, rvParticleParms *parms, idVec3 *normal, idVec3 *centre)
{
  *result = 0.0;
  result[1] = 0.0;
  result[2] = 0.0;
  SpawnGetNormal(normal, (idVec3 *)result, centre);
}
void __cdecl SpawnOne1(float *result)
{
  *result = 1.0;
}
void __cdecl SpawnOne2(float *result)
{
  *result = 1.0;
  result[1] = 1.0;
}
void __cdecl SpawnOne3(float *result, rvParticleParms *parms, idVec3 *normal, idVec3 *centre)
{
  *result = 1.0;
  result[1] = 1.0;
  result[2] = 1.0;
  SpawnGetNormal(normal, (idVec3 *)result, centre);
}
void __cdecl SpawnPoint1(float *result, rvParticleParms *parms)
{
  *result = parms->mMins.x;
}
void __cdecl SpawnPoint2(float *result, rvParticleParms *parms)
{
  *result = parms->mMins.x;
  result[1] = parms->mMins.y;
}
void __cdecl SpawnPoint3(float *result, rvParticleParms *parms, idVec3 *normal, idVec3 *centre)
{
  *result = parms->mMins.x;
  result[1] = parms->mMins.y;
  result[2] = parms->mMins.z;
  SpawnGetNormal(normal, (idVec3 *)result, centre);
}
void __cdecl SpawnLinear1(float *result, rvParticleParms *parms)
{
  float v2; // ST08_4

  v2 = rvRandom::flrand(0.0, 1.0);
  *result = (parms->mMaxs.x - parms->mMins.x) * v2 + parms->mMins.x;
}
void __cdecl SpawnLinear2(float *result, rvParticleParms *parms)
{
  rvParticleParms *v2; // esi
  double v3; // st7
  float rand; // [esp+18h] [ebp+8h]

  v2 = parms;
  if ( parms->mFlags & 0x10 )
    v3 = *result;
  else
    v3 = rvRandom::flrand(0.0, 1.0);
  rand = v3;
  *result = (v2->mMaxs.x - v2->mMins.x) * rand + v2->mMins.x;
  result[1] = rand * (v2->mMaxs.y - v2->mMins.y) + v2->mMins.y;
}
void __cdecl SpawnLinear3(float *result, rvParticleParms *parms, idVec3 *normal, idVec3 *centre)
{
  rvParticleParms *v4; // esi
  double v5; // st7
  float rand; // [esp+18h] [ebp+8h]

  v4 = parms;
  if ( parms->mFlags & 0x10 )
    v5 = *result;
  else
    v5 = rvRandom::flrand(0.0, 1.0);
  rand = v5;
  *result = (v4->mMaxs.x - v4->mMins.x) * rand + v4->mMins.x;
  result[1] = (v4->mMaxs.y - v4->mMins.y) * rand + v4->mMins.y;
  result[2] = rand * (v4->mMaxs.z - v4->mMins.z) + v4->mMins.z;
  SpawnGetNormal(normal, (idVec3 *)result, centre);
}
void __cdecl SpawnBox1(float *result, rvParticleParms *parms)
{
  *result = rvRandom::flrand(parms->mMins.x, parms->mMaxs.x);
}
void __cdecl SpawnBox2(float *result, rvParticleParms *parms)
{
  *result = rvRandom::flrand(parms->mMins.x, parms->mMaxs.x);
  result[1] = rvRandom::flrand(parms->mMins.y, parms->mMaxs.y);
}
void __cdecl SpawnBox3(float *result, rvParticleParms *parms, idVec3 *normal, idVec3 *centre)
{
  rvParticleParms *v4; // esi

  v4 = parms;
  *result = rvRandom::flrand(parms->mMins.x, parms->mMaxs.x);
  result[1] = rvRandom::flrand(v4->mMins.y, v4->mMaxs.y);
  result[2] = rvRandom::flrand(v4->mMins.z, v4->mMaxs.z);
  SpawnGetNormal(normal, (idVec3 *)result, centre);
}
void __cdecl SpawnSurfaceBox2(float *result, rvParticleParms *parms)
{
  switch ( rvRandom::irand(0, 3) )
  {
    case 0:
      *result = parms->mMins.x;
      result[1] = rvRandom::flrand(parms->mMins.y, parms->mMaxs.y);
      break;
    case 1:
      *result = rvRandom::flrand(parms->mMins.x, parms->mMaxs.x);
      result[1] = parms->mMins.y;
      break;
    case 2:
      *result = parms->mMaxs.x;
      result[1] = rvRandom::flrand(parms->mMins.y, parms->mMaxs.y);
      break;
    case 3:
      *result = rvRandom::flrand(parms->mMins.x, parms->mMaxs.x);
      result[1] = parms->mMaxs.y;
      break;
    default:
      return;
  }
}
void __cdecl SpawnSurfaceBox3(float *result, rvParticleParms *parms, idVec3 *normal, idVec3 *centre)
{
  int v4; // ebx
  rvParticleParms *v5; // edi
  double v6; // st7
  float min; // ST00_4
  double v8; // st7
  double v9; // st7
  idVec3 *v10; // edi
  float *v11; // eax
  float max; // [esp+4h] [ebp-18h]
  float normalb; // [esp+28h] [ebp+Ch]
  float normalc; // [esp+28h] [ebp+Ch]
  float normala; // [esp+28h] [ebp+Ch]
  float normald; // [esp+28h] [ebp+Ch]

  v4 = rvRandom::irand(0, 5);
  switch ( v4 )
  {
    case 0:
      v5 = parms;
      *result = parms->mMins.x;
      max = parms->mMaxs.y;
      v6 = parms->mMins.y;
      goto LABEL_3;
    case 1:
      v5 = parms;
      *result = rvRandom::flrand(parms->mMins.x, parms->mMaxs.x);
      v8 = parms->mMins.y;
      goto LABEL_4;
    case 2:
      *result = rvRandom::flrand(parms->mMins.x, parms->mMaxs.x);
      result[1] = rvRandom::flrand(parms->mMins.y, parms->mMaxs.y);
      v9 = parms->mMins.z;
      goto LABEL_10;
    case 3:
      v5 = parms;
      *result = parms->mMaxs.x;
      max = parms->mMaxs.y;
      v6 = parms->mMins.y;
LABEL_3:
      min = v6;
      v8 = rvRandom::flrand(min, max);
      goto LABEL_4;
    case 4:
      v5 = parms;
      *result = rvRandom::flrand(parms->mMins.x, parms->mMaxs.x);
      v8 = parms->mMaxs.y;
LABEL_4:
      result[1] = v8;
      v9 = rvRandom::flrand(v5->mMins.z, v5->mMaxs.z);
      goto LABEL_10;
    case 5:
      *result = rvRandom::flrand(parms->mMins.x, parms->mMaxs.x);
      result[1] = rvRandom::flrand(parms->mMins.y, parms->mMaxs.y);
      v9 = parms->mMaxs.z;
LABEL_10:
      result[2] = v9;
      break;
    default:
      break;
  }
  v10 = normal;
  if ( normal )
  {
    if ( centre )
    {
      v11 = (float *)((int (__stdcall *)(int))bse->vfptr->GetCubeNormals)(v4);
      normal->x = *v11;
      normal->y = v11[1];
      normal->z = v11[2];
    }
    else
    {
      normal->x = *result;
      normal->y = result[1];
      normalb = result[2];
      v10->z = normalb;
      normalc = normalb * normalb + v10->y * v10->y + v10->x * v10->x;
      normala = sqrt(normalc);
      if ( normala >= 0.00000011920929 )
      {
        normald = 1.0 / normala;
        v10->x = v10->x * normald;
        v10->y = v10->y * normald;
        v10->z = normald * v10->z;
      }
    }
  }
}
void __cdecl SpawnSphere2(float *result, rvParticleParms *parms)
{
  float direction_4; // [esp+Ch] [ebp-18h]
  float radius; // [esp+10h] [ebp-14h]
  float radius_4; // [esp+14h] [ebp-10h]
  float origin; // [esp+18h] [ebp-Ch]
  float origin_4; // [esp+1Ch] [ebp-8h]
  float v7; // [esp+20h] [ebp-4h]
  float parmsb; // [esp+2Ch] [ebp+8h]
  float parmsa; // [esp+2Ch] [ebp+8h]
  float parmsc; // [esp+2Ch] [ebp+8h]

  origin_4 = (parms->mMins.x + parms->mMaxs.x) * 0.5;
  v7 = (parms->mMins.y + parms->mMaxs.y) * 0.5;
  radius_4 = (parms->mMaxs.x - parms->mMins.x) * 0.5;
  origin = 0.5 * (parms->mMaxs.y - parms->mMins.y);
  direction_4 = rvRandom::flrand(-1.0, 1.0);
  radius = rvRandom::flrand(-1.0, 1.0);
  parmsb = direction_4 * direction_4 + radius * radius;
  parmsa = sqrt(parmsb);
  if ( parmsa >= 0.00000011920929 )
  {
    parmsc = 1.0 / parmsa;
    direction_4 = direction_4 * parmsc;
    radius = parmsc * radius;
  }
  *result = rvRandom::flrand(0.0, radius_4) * direction_4 + origin_4;
  result[1] = rvRandom::flrand(0.0, origin) * radius + v7;
}
void __cdecl SpawnSphere3(float *result, rvParticleParms *parms, idVec3 *normal, idVec3 *centre)
{
  float direction_4; // [esp+Ch] [ebp-24h]
  float direction_8; // [esp+10h] [ebp-20h]
  float radius; // [esp+14h] [ebp-1Ch]
  float radius_4; // [esp+18h] [ebp-18h]
  float radius_8; // [esp+1Ch] [ebp-14h]
  float origin; // [esp+20h] [ebp-10h]
  float origin_4; // [esp+24h] [ebp-Ch]
  float origin_8; // [esp+28h] [ebp-8h]
  float v12; // [esp+2Ch] [ebp-4h]
  float parmsb; // [esp+38h] [ebp+8h]
  float parmsa; // [esp+38h] [ebp+8h]
  float parmsc; // [esp+38h] [ebp+8h]

  origin_4 = (parms->mMins.x + parms->mMaxs.x) * 0.5;
  origin_8 = (parms->mMins.y + parms->mMaxs.y) * 0.5;
  v12 = (parms->mMins.z + parms->mMaxs.z) * 0.5;
  radius_4 = (parms->mMaxs.x - parms->mMins.x) * 0.5;
  radius_8 = (parms->mMaxs.y - parms->mMins.y) * 0.5;
  origin = 0.5 * (parms->mMaxs.z - parms->mMins.z);
  direction_4 = rvRandom::flrand(-1.0, 1.0);
  direction_8 = rvRandom::flrand(-1.0, 1.0);
  radius = rvRandom::flrand(-1.0, 1.0);
  parmsb = direction_8 * direction_8 + direction_4 * direction_4 + radius * radius;
  parmsa = sqrt(parmsb);
  if ( parmsa >= 0.00000011920929 )
  {
    parmsc = 1.0 / parmsa;
    direction_4 = direction_4 * parmsc;
    direction_8 = direction_8 * parmsc;
    radius = parmsc * radius;
  }
  *result = rvRandom::flrand(0.0, radius_4) * direction_4 + origin_4;
  result[1] = rvRandom::flrand(0.0, radius_8) * direction_8 + origin_8;
  result[2] = rvRandom::flrand(0.0, origin) * radius + v12;
  SpawnGetNormal(normal, (idVec3 *)result, centre);
}
void __cdecl SpawnSurfaceBox1(float *result, rvParticleParms *parms)
{
  *result = *(&parms->mMins.x + rvRandom::irand(0, 1));
}
void __cdecl SpawnSurfaceSphere2(float *result, rvParticleParms *parms)
{
  float direction_4; // [esp+8h] [ebp-18h]
  float radius; // [esp+Ch] [ebp-14h]
  float radius_4; // [esp+10h] [ebp-10h]
  float origin; // [esp+14h] [ebp-Ch]
  float origin_4; // [esp+18h] [ebp-8h]
  float v7; // [esp+1Ch] [ebp-4h]
  float parmsb; // [esp+28h] [ebp+8h]
  float parmsa; // [esp+28h] [ebp+8h]
  float parmsc; // [esp+28h] [ebp+8h]

  origin_4 = (parms->mMins.x + parms->mMaxs.x) * 0.5;
  v7 = (parms->mMins.y + parms->mMaxs.y) * 0.5;
  radius_4 = (parms->mMaxs.x - parms->mMins.x) * 0.5;
  origin = 0.5 * (parms->mMaxs.y - parms->mMins.y);
  direction_4 = rvRandom::flrand(-1.0, 1.0);
  radius = rvRandom::flrand(-1.0, 1.0);
  parmsb = direction_4 * direction_4 + radius * radius;
  parmsa = sqrt(parmsb);
  if ( parmsa >= 0.00000011920929 )
  {
    parmsc = 1.0 / parmsa;
    direction_4 = direction_4 * parmsc;
    radius = parmsc * radius;
  }
  *result = radius_4 * direction_4 + origin_4;
  result[1] = origin * radius + v7;
}
void __cdecl SpawnSurfaceSphere3(float *result, rvParticleParms *parms, idVec3 *normal, idVec3 *centre)
{
  float direction_4; // [esp+8h] [ebp-24h]
  float direction_8; // [esp+Ch] [ebp-20h]
  float radius; // [esp+10h] [ebp-1Ch]
  float radius_4; // [esp+14h] [ebp-18h]
  float radius_8; // [esp+18h] [ebp-14h]
  float origin; // [esp+1Ch] [ebp-10h]
  float origin_4; // [esp+20h] [ebp-Ch]
  float origin_8; // [esp+24h] [ebp-8h]
  float v12; // [esp+28h] [ebp-4h]
  float parmsb; // [esp+34h] [ebp+8h]
  float parmsa; // [esp+34h] [ebp+8h]
  float parmsc; // [esp+34h] [ebp+8h]

  origin_4 = (parms->mMins.x + parms->mMaxs.x) * 0.5;
  origin_8 = (parms->mMins.y + parms->mMaxs.y) * 0.5;
  v12 = (parms->mMins.z + parms->mMaxs.z) * 0.5;
  radius_4 = (parms->mMaxs.x - parms->mMins.x) * 0.5;
  radius_8 = (parms->mMaxs.y - parms->mMins.y) * 0.5;
  origin = 0.5 * (parms->mMaxs.z - parms->mMins.z);
  direction_4 = rvRandom::flrand(-1.0, 1.0);
  direction_8 = rvRandom::flrand(-1.0, 1.0);
  radius = rvRandom::flrand(-1.0, 1.0);
  parmsb = direction_8 * direction_8 + direction_4 * direction_4 + radius * radius;
  parmsa = sqrt(parmsb);
  if ( parmsa >= 0.00000011920929 )
  {
    parmsc = 1.0 / parmsa;
    direction_4 = direction_4 * parmsc;
    direction_8 = direction_8 * parmsc;
    radius = parmsc * radius;
  }
  *result = radius_4 * direction_4 + origin_4;
  result[1] = radius_8 * direction_8 + origin_8;
  result[2] = origin * radius + v12;
  SpawnGetNormal(normal, (idVec3 *)result, centre);
}
void __cdecl SpawnCylinder3(float *result, rvParticleParms *parms, idVec3 *normal, idVec3 *centre)
{
  rvParticleParms *v4; // esi
  double v5; // st6
  double v6; // st6
  idVec3 *v7; // esi
  float z; // [esp+Ch] [ebp-20h]
  float direction; // [esp+10h] [ebp-1Ch]
  float direction_4; // [esp+14h] [ebp-18h]
  float radius; // [esp+18h] [ebp-14h]
  float radius_4; // [esp+1Ch] [ebp-10h]
  float origin; // [esp+20h] [ebp-Ch]
  float origin_4; // [esp+24h] [ebp-8h]
  float v15; // [esp+28h] [ebp-4h]
  float resulta; // [esp+30h] [ebp+4h]
  float taperb; // [esp+34h] [ebp+8h]
  float taper; // [esp+34h] [ebp+8h]
  float taperc; // [esp+34h] [ebp+8h]
  float tapera; // [esp+34h] [ebp+8h]
  float taperd; // [esp+34h] [ebp+8h]

  v4 = parms;
  origin_4 = (parms->mMins.y + parms->mMaxs.y) * 0.5;
  v15 = (parms->mMins.z + parms->mMaxs.z) * 0.5;
  radius_4 = (parms->mMaxs.y - parms->mMins.y) * 0.5;
  origin = 0.5 * (parms->mMaxs.z - parms->mMins.z);
  direction_4 = rvRandom::flrand(-1.0, 1.0);
  radius = rvRandom::flrand(-1.0, 1.0);
  taperb = direction_4 * direction_4 + radius * radius;
  taper = sqrt(taperb);
  if ( taper >= 0.00000011920929 )
  {
    taperc = 1.0 / taper;
    direction_4 = direction_4 * taperc;
    radius = taperc * radius;
  }
  z = v4->mMaxs.x - v4->mMins.x;
  direction = rvRandom::flrand(0.0, z);
  tapera = 1.0;
  if ( z != 0.0 && v4->mFlags & 4 )
  {
    v5 = direction;
    tapera = direction / z;
  }
  else
  {
    v5 = direction;
  }
  v6 = v5 + v4->mMins.x;
  v7 = (idVec3 *)result;
  *result = v6;
  resulta = radius_4 * tapera;
  v7->y = rvRandom::flrand(0.0, resulta) * direction_4 + origin_4;
  taperd = origin * tapera;
  v7->z = rvRandom::flrand(0.0, taperd) * radius + v15;
  SpawnGetNormal(normal, v7, centre);
}
void __cdecl SpawnSurfaceCylinder3(float *result, rvParticleParms *parms, idVec3 *normal, idVec3 *centre)
{
  rvParticleParms *v4; // esi
  float direction; // ST10_4
  double v6; // st7
  double v7; // st6
  double v8; // st6
  double v9; // st3
  float v10; // ST20_4
  float v11; // ST30_4
  double v12; // st5
  float v13; // ST1C_4
  float v14; // ST20_4
  float top; // ST24_4
  float z; // [esp+Ch] [ebp-28h]
  float direction_4; // [esp+14h] [ebp-20h]
  float side; // [esp+18h] [ebp-1Ch]
  float side_4; // [esp+1Ch] [ebp-18h]
  float side_8; // [esp+20h] [ebp-14h]
  float top_4; // [esp+28h] [ebp-Ch]
  float top_4a; // [esp+28h] [ebp-Ch]
  float top_8; // [esp+2Ch] [ebp-8h]
  float top_8a; // [esp+2Ch] [ebp-8h]
  float v25; // [esp+30h] [ebp-4h]
  float heightc; // [esp+3Ch] [ebp+8h]
  float height; // [esp+3Ch] [ebp+8h]
  float heightd; // [esp+3Ch] [ebp+8h]
  float heighte; // [esp+3Ch] [ebp+8h]
  float heightf; // [esp+3Ch] [ebp+8h]
  float heightg; // [esp+3Ch] [ebp+8h]
  float heighta; // [esp+3Ch] [ebp+8h]
  float heighth; // [esp+3Ch] [ebp+8h]
  float heighti; // [esp+3Ch] [ebp+8h]
  float heightj; // [esp+3Ch] [ebp+8h]
  float heightb; // [esp+3Ch] [ebp+8h]
  float heightk; // [esp+3Ch] [ebp+8h]

  v4 = parms;
  top_4 = (parms->mMins.y + parms->mMaxs.y) * 0.5;
  top_8 = (parms->mMins.z + parms->mMaxs.z) * 0.5;
  side_4 = (parms->mMaxs.y - parms->mMins.y) * 0.5;
  side_8 = 0.5 * (parms->mMaxs.z - parms->mMins.z);
  direction_4 = rvRandom::flrand(-1.0, 1.0);
  side = rvRandom::flrand(-1.0, 1.0);
  heightc = side * side + direction_4 * direction_4;
  height = sqrt(heightc);
  if ( height >= 0.00000011920929 )
  {
    heightd = 1.0 / height;
    direction_4 = direction_4 * heightd;
    side = heightd * side;
  }
  heighte = v4->mMaxs.x - v4->mMins.x;
  direction = rvRandom::flrand(0.0, heighte);
  z = 1.0;
  v6 = heighte;
  v7 = direction;
  if ( heighte != 0.0 && v4->mFlags & 4 )
    z = v7 / v6;
  *result = v7 + v4->mMins.x;
  v8 = side_4 * direction_4;
  result[1] = z * v8 + top_4;
  v9 = side_8 * side;
  result[2] = z * v9 + top_8;
  if ( normal )
  {
    if ( centre )
    {
      if ( 1.0 == z )
      {
        normal->x = 0.0;
        normal->y = direction_4;
        normal->z = side;
      }
      else if ( 0.0 == v6 )
      {
        normal->x = 1.0;
        normal->y = 0.0;
        normal->z = 0.0;
      }
      else
      {
        v10 = side * -side_8;
        heightf = v8;
        v11 = v9;
        v12 = v10;
        v13 = heightf * heightf - v11 * v10;
        v14 = v11 * 0.0 - heightf * v6;
        top = v6 * v12 - heightf * 0.0;
        top_4a = -v13;
        top_8a = -v14;
        v25 = -top;
        normal->x = top_4a;
        normal->y = top_8a;
        normal->z = v25;
        heightg = v25 * v25 + top_8a * top_8a + top_4a * top_4a;
        heighta = sqrt(heightg);
        if ( heighta >= 0.00000011920929 )
        {
          heighth = 1.0 / heighta;
          normal->x = top_4a * heighth;
          normal->y = top_8a * heighth;
          normal->z = heighth * v25;
        }
      }
    }
    else
    {
      normal->x = *result;
      normal->y = result[1];
      heighti = result[2];
      normal->z = heighti;
      heightj = heighti * heighti + normal->y * normal->y + normal->x * normal->x;
      heightb = sqrt(heightj);
      if ( heightb >= 0.00000011920929 )
      {
        heightk = 1.0 / heightb;
        normal->x = normal->x * heightk;
        normal->y = normal->y * heightk;
        normal->z = heightk * normal->z;
      }
    }
  }
}
void __cdecl SpawnSpiral2(float *result, rvParticleParms *parms)
{
  rvParticleParms *v2; // esi
  float *v3; // edi
  double v4; // st7
  float resulta; // [esp+18h] [ebp+4h]
  float resultb; // [esp+18h] [ebp+4h]
  float left; // [esp+1Ch] [ebp+8h]

  v2 = parms;
  if ( parms->mFlags & 0x10 )
  {
    v3 = result;
    v4 = (parms->mMaxs.x - parms->mMins.x) * *result + parms->mMins.x;
  }
  else
  {
    v4 = rvRandom::flrand(parms->mMins.x, parms->mMaxs.x);
    v3 = result;
  }
  *v3 = v4;
  left = rvRandom::flrand(parms->mMins.y, parms->mMaxs.y);
  resulta = *v3 * idMath::TWO_PI / v2->mRange;
  resultb = cos(resulta);
  v3[1] = resultb * left;
}
void __cdecl SpawnSpiral3(float *result, rvParticleParms *parms, idVec3 *normal, idVec3 *centre)
{
  rvParticleParms *v4; // esi
  float *v5; // edi
  double v6; // st7
  double v7; // st7
  double v8; // st7
  idVec3 *v9; // esi
  bool v10; // zf
  double v11; // st5
  double v12; // st4
  double v13; // st6
  double v14; // st7
  float min; // [esp+10h] [ebp-14h]
  float fraction; // [esp+14h] [ebp-10h]
  float left; // [esp+18h] [ebp-Ch]
  float up; // [esp+1Ch] [ebp-8h]
  float v19; // [esp+20h] [ebp-4h]
  float resultc; // [esp+28h] [ebp+4h]
  float resultd; // [esp+28h] [ebp+4h]
  float resulta; // [esp+28h] [ebp+4h]
  float resulte; // [esp+28h] [ebp+4h]
  float resultb; // [esp+28h] [ebp+4h]
  float resultf; // [esp+28h] [ebp+4h]

  v4 = parms;
  if ( parms->mFlags & 0x10 )
  {
    v5 = result;
    v6 = (parms->mMaxs.x - parms->mMins.x) * *result + parms->mMins.x;
  }
  else
  {
    parms = (rvParticleParms *)LODWORD(parms->mMaxs.x);
    min = v4->mMins.x;
    v6 = rvRandom::flrand(min, *(float *)&parms);
    v5 = result;
  }
  *v5 = v6;
  resultc = v4->mMaxs.y;
  fraction = v4->mMins.y;
  up = rvRandom::flrand(fraction, resultc);
  resultd = v4->mMaxs.z;
  fraction = v4->mMins.z;
  v19 = rvRandom::flrand(fraction, resultd);
  v7 = *v5 * idMath::TWO_PI;
  fraction = COERCE_FLOAT(&parms);
  left = v7 / v4->mRange;
  *(float *)&parms = cos(left);
  min = sin(left);
  v8 = *(float *)&parms;
  v9 = normal;
  v10 = normal == 0;
  v11 = up;
  v12 = min;
  v13 = v19;
  resulta = *(float *)&parms * up - min * v19;
  v5[1] = resulta;
  v5[2] = v13 * v8 + v11 * v12;
  if ( !v10 )
  {
    v10 = centre == 0;
    v9->y = resulta;
    v9->z = v5[2];
    if ( v10 )
      v14 = *v5;
    else
      v14 = 0.0;
    v9->x = v14;
    resulte = v9->y * v9->y + v9->x * v9->x + v9->z * v9->z;
    resultb = sqrt(resulte);
    if ( resultb >= 0.00000011920929 )
    {
      resultf = 1.0 / resultb;
      v9->x = v9->x * resultf;
      v9->y = v9->y * resultf;
      v9->z = resultf * v9->z;
    }
  }
}
void __thiscall sdModelInfo::CalculateSurfRemap(sdModelInfo *this)
{
  sdModelInfo *v1; // esi
  signed int v2; // ebx
  int v3; // edi
  signed int v4; // ebp
  int v5; // ebx
  float v6; // ST14_4
  signed int v7; // edx
  signed int v8; // [esp+10h] [ebp-8h]
  float v9; // [esp+14h] [ebp-4h]

  v1 = this;
  v2 = 0;
  v8 = 0;
  v3 = 0;
  if ( ((int (*)(void))this->model->vfptr->NumSurfaces)() > 0 )
  {
    do
      v2 += *(_DWORD *)(*(_DWORD *)(((int (__stdcall *)(int))v1->model->vfptr->Surface)(v3++) + 8) + 56) / 3;
    while ( v3 < ((int (*)(void))v1->model->vfptr->NumSurfaces)() );
    v8 = v2;
  }
  v4 = 0;
  v5 = 0;
  if ( ((int (*)(void))v1->model->vfptr->NumSurfaces)() <= 0 )
    goto LABEL_13;
  v9 = (double)v8;
  do
  {
    v6 = (double)(*(_DWORD *)(*(_DWORD *)(((int (__stdcall *)(int))v1->model->vfptr->Surface)(v5) + 8) + 56) / 3) / v9;
    v7 = (signed int)(v6 * 10.0);
    if ( v7 > 0 )
    {
      memset32(&v1->surfRemap[v4], v5, v7);
      v4 += v7;
    }
    ++v5;
  }
  while ( v5 < ((int (*)(void))v1->model->vfptr->NumSurfaces)() );
  if ( v4 < 10 )
LABEL_13:
    memset(&v1->surfRemap[v4], 0, 4 * (10 - v4));
}
void __thiscall rvParticleParms::HandleRelativeParms(rvParticleParms *this, float *death, float *init, int count)
{
  int v4; // ebx
  float *v5; // ecx
  unsigned int v6; // edx
  float *v7; // eax
  double v8; // st7
  float *v9; // eax
  int v10; // esi
  double v11; // st7

  if ( this->mFlags & 8 )
  {
    v4 = 0;
    if ( count >= 4 )
    {
      v5 = init + 3;
      v6 = ((unsigned int)(count - 4) >> 2) + 1;
      v7 = death + 1;
      v4 = 4 * v6;
      do
      {
        v7 += 4;
        v8 = *(v5 - 3) + *(v7 - 5);
        v5 += 4;
        --v6;
        *(v7 - 5) = v8;
        *(v7 - 4) = *(float *)((char *)v7 + (char *)init - (char *)death - 16) + *(v7 - 4);
        *(v7 - 3) = *(v5 - 5) + *(v7 - 3);
        *(v7 - 2) = *(v5 - 4) + *(v7 - 2);
      }
      while ( v6 );
    }
    if ( v4 < count )
    {
      v9 = &death[v4];
      v10 = count - v4;
      do
      {
        v11 = *(float *)((char *)v9 + (char *)init - (char *)death);
        ++v9;
        --v10;
        *(v9 - 1) = v11 + *(v9 - 1);
      }
      while ( v10 );
    }
  }
}
void __thiscall rvParticleParms::GetMinsMaxs(rvParticleParms *this, idVec3 *mins, idVec3 *maxs)
{
  mins->z = 0.0;
  mins->y = 0.0;
  mins->x = 0.0;
  maxs->z = 0.0;
  maxs->y = 0.0;
  maxs->x = 0.0;
  switch ( this->mSpawnType )
  {
    case 5:
      goto $LN8_27;
    case 6:
      goto $LN9_24;
    case 7:
      mins->z = 1.0;
      maxs->z = 1.0;
$LN9_24:
      mins->y = 1.0;
      maxs->y = 1.0;
$LN8_27:
      mins->x = 1.0;
      maxs->x = 1.0;
      break;
    case 9:
      mins->x = this->mMins.x;
      maxs->x = this->mMins.x;
      break;
    case 0xA:
      mins->y = this->mMins.y;
      maxs->y = this->mMins.y;
      mins->x = this->mMins.x;
      maxs->x = this->mMins.x;
      break;
    case 0xB:
      mins->z = this->mMins.z;
      maxs->z = this->mMins.z;
      mins->y = this->mMins.y;
      maxs->y = this->mMins.y;
      mins->x = this->mMins.x;
      maxs->x = this->mMins.x;
      break;
    case 0xD:
    case 0x11:
    case 0x15:
    case 0x19:
    case 0x1D:
    case 0x21:
    case 0x25:
    case 0x29:
    case 0x2D:
      mins->x = this->mMins.x;
      maxs->x = this->mMaxs.x;
      break;
    case 0xE:
    case 0x12:
    case 0x16:
    case 0x1A:
    case 0x1E:
    case 0x22:
    case 0x26:
    case 0x2A:
    case 0x2E:
      mins->y = this->mMins.y;
      maxs->y = this->mMaxs.y;
      mins->x = this->mMins.x;
      maxs->x = this->mMaxs.x;
      break;
    case 0xF:
    case 0x13:
    case 0x17:
    case 0x1B:
    case 0x1F:
    case 0x23:
    case 0x27:
    case 0x2B:
    case 0x2F:
      mins->z = this->mMins.z;
      maxs->z = this->mMaxs.z;
      mins->y = this->mMins.y;
      maxs->y = this->mMaxs.y;
      mins->x = this->mMins.x;
      maxs->x = this->mMaxs.x;
      break;
    default:
      return;
  }
}
void __cdecl SpawnModel3(float *result, rvParticleParms *parms, idVec3 *normal, idVec3 *centre)
{
  rvParticleParms *v4; // ebx
  sdModelInfo *v5; // eax
  idRenderModel *v6; // esi
  idRenderModelVtbl *v7; // edi
  int v8; // eax
  _DWORD *v9; // esi
  int v10; // eax
  int v11; // edx
  int v12; // edi
  unsigned __int16 *v13; // ecx
  float *v14; // eax
  float p1_4; // ST38_4
  float p1_8; // ST3C_4
  float scale; // ST40_4
  float *v18; // eax
  float p2_4; // ST2C_4
  float p2_8; // ST30_4
  float p1; // ST34_4
  float *v22; // eax
  float v23; // ST20_4
  float v24; // ST24_4
  float v25; // ST28_4
  float p3; // ST1C_4
  float random_4; // ST50_4
  float random_8; // ST54_4
  float scale_4; // ST44_4
  float scale_8; // ST48_4
  float random; // ST4C_4
  float v32; // ST20_4
  float v33; // ST24_4
  float v34; // ST28_4
  float v35; // ST2C_4
  float v36; // ST30_4
  float v37; // ST34_4
  float v38; // ST38_4
  float v39; // ST3C_4
  float v40; // ST40_4
  float *v41; // edi
  float v42; // ST44_4
  float v43; // ST48_4
  float v44; // ST4C_4
  float p3_4; // [esp+20h] [ebp-3Ch]
  float p3_8; // [esp+24h] [ebp-38h]
  float p2; // [esp+28h] [ebp-34h]
  float ta; // [esp+64h] [ebp+8h]
  float tb; // [esp+64h] [ebp+8h]
  float tc; // [esp+64h] [ebp+8h]
  float t; // [esp+64h] [ebp+8h]
  float td; // [esp+64h] [ebp+8h]

  v4 = parms;
  v5 = parms->mModelInfo;
  v6 = v5->model;
  v7 = v5->model->vfptr;
  v8 = rvRandom::irand(0, 9);
  v9 = *(_DWORD **)((int)v7->Surface(v6, parms->mModelInfo->surfRemap[v8]) + 8);
  v10 = rvRandom::irand(0, v9[14] / 3 - 1);
  v11 = v9[13];
  v12 = v10;
  v13 = (unsigned __int16 *)(v9[15] + 6 * v10);
  v14 = (float *)(v11 + 32 * *v13);
  p1_4 = *v14;
  p1_8 = v14[1];
  scale = v14[2];
  v18 = (float *)(v11 + 32 * v13[1]);
  p2_4 = *v18;
  p2_8 = v18[1];
  p1 = v18[2];
  v22 = (float *)(v11 + 32 * v13[2]);
  v23 = *v22;
  v24 = v22[1];
  v25 = v22[2];
  p3 = rvRandom::flrand(0.0, 1.0);
  ta = sqrt(rvRandom::flrand(0.0, 1.0));
  random_4 = 1.0 - ta;
  random_8 = (1.0 - p3) * ta;
  tb = ta * p3;
  scale_4 = tb * v23;
  scale_8 = v24 * tb;
  random = tb * v25;
  v32 = random_8 * p2_4;
  v33 = random_8 * p2_8;
  v34 = random_8 * p1;
  v35 = random_4 * p1_4;
  v36 = random_4 * p1_8;
  v37 = random_4 * scale;
  v38 = v35 + v32;
  v39 = v36 + v33;
  v40 = v37 + v34;
  p3_4 = v38 + scale_4;
  p3_8 = v39 + scale_8;
  p2 = v40 + random;
  if ( normal )
  {
    if ( centre )
    {
      if ( !v9[22] )
        ((void (__stdcall *)(_DWORD *))renderUtilities->vfptr->DeriveFacePlanes)(v9);
      v41 = (float *)(v9[22] + 16 * v12);
      normal->x = *v41;
      normal->y = v41[1];
      normal->z = v41[2];
    }
    else
    {
      normal->x = p3_4;
      normal->y = p3_8;
      normal->z = p2;
      tc = p3_8 * p3_8 + p3_4 * p3_4 + p2 * p2;
      t = sqrt(tc);
      if ( t >= 0.00000011920929 )
      {
        td = 1.0 / t;
        normal->x = td * p3_4;
        normal->y = p3_8 * td;
        normal->z = td * p2;
      }
    }
  }
  v42 = v4->mMaxs.x - v4->mMins.x;
  v43 = v4->mMaxs.y - v4->mMins.y;
  v44 = v4->mMaxs.z - v4->mMins.z;
  *result = v42 * p3_4 + v4->mMins.x;
  result[1] = v43 * p3_8 + v4->mMins.y;
  result[2] = v44 * p2 + v4->mMins.z;
}
int dynamic_initializer_for__singletonLock___114()
{
  sdLock::sdLock(&singletonLock_114);
  return atexit(dynamic_atexit_destructor_for__singletonLock___114);
}
void __cdecl dynamic_atexit_destructor_for__singletonLock___114()
{
  sdLock::~sdLock(&singletonLock_114);
}
