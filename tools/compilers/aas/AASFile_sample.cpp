/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "../../../idlib/precompiled.h"
#pragma hdrstop

#include "AASFile.h"
#include "AASFile_local.h"


//===============================================================
//
//	Environment Sampling
//
//===============================================================

/*
================
idAASFileLocal::EdgeCenter
================
*/
idVec3 idAASFileLocal::EdgeCenter(int edgeNum) const
{
	const aasEdge_t *edge;
	edge = &edges[edgeNum];
	return (vertices[edge->vertexNum[0]] + vertices[edge->vertexNum[1]]) * 0.5f;
}

/*
================
idAASFileLocal::FaceCenter
================
*/
idVec3 idAASFileLocal::FaceCenter(int faceNum) const
{
	int i, edgeNum;
	const aasFace_t *face;
	const aasEdge_t *edge;
	idVec3 center;

	center = vec3_origin;

#ifdef _SPLASHDAMAGE
	edgeNum = faceNum;
	edge = &edges[ abs(edgeNum)];
	center += vertices[ edge->vertexNum[ INTSIGNBITSET(edgeNum)] ];
#else
	face = &faces[faceNum];

	if (face->numEdges > 0) {
		for (i = 0; i < face->numEdges; i++) {
			edgeNum = edgeIndex[ face->firstEdge + i ];
			edge = &edges[ abs(edgeNum)];
			center += vertices[ edge->vertexNum[ INTSIGNBITSET(edgeNum)] ];
		}

		center /= face->numEdges;
	}
#endif

	return center;
}

/*
================
idAASFileLocal::AreaCenter
================
*/
idVec3 idAASFileLocal::AreaCenter(int areaNum) const
{
	int i, faceNum;
	const aasArea_t *area;
	idVec3 center;

	center = vec3_origin;

	area = &areas[areaNum];

#ifdef _SPLASHDAMAGE
	if (area->numEdges > 0) {
		for (i = 0; i < area->numEdges; i++) {
			int edgeNum = edgeIndex[area->firstEdge + i];
			center += EdgeCenter(abs(edgeNum));
		}

		center /= area->numEdges;
	}
#else
	if (area->numFaces > 0) {
		for (i = 0; i < area->numFaces; i++) {
			faceNum = faceIndex[area->firstFace + i];
			center += FaceCenter(abs(faceNum));
		}

		center /= area->numFaces;
	}
#endif

	return center;
}

/*
============
idAASFileLocal::AreaReachableGoal
============
*/
idVec3 idAASFileLocal::AreaReachableGoal(int areaNum) const
{
	int i, faceNum, numFaces;
	const aasArea_t *area;
	idVec3 center;
	idVec3 start, end;
	aasTrace_t trace;

	area = &areas[areaNum];

	if (!(area->flags & (AREA_REACHABLE_WALK|AREA_REACHABLE_FLY)) || (area->flags & AREA_LIQUID)) {
		return AreaCenter(areaNum);
	}

	center = vec3_origin;

#ifdef _SPLASHDAMAGE
	int _numEdges = 0;

	for (i = 0; i < area->numEdges; i++) {
		int edgeNum = edgeIndex[area->firstEdge + i];

		if (!(edges[abs(edgeNum)].flags & FACE_FLOOR)) {
			continue;
		}

		center += FaceCenter(abs(edgeNum));
		_numEdges++;
	}

	if (_numEdges > 0) {
		center /= _numEdges;
	}
#else
	numFaces = 0;

	for (i = 0; i < area->numFaces; i++) {
		faceNum = faceIndex[area->firstFace + i];

		if (!(faces[abs(faceNum)].flags & FACE_FLOOR)) {
			continue;
		}

		center += FaceCenter(abs(faceNum));
		numFaces++;
	}

	if (numFaces > 0) {
		center /= numFaces;
	}
#endif

	center[2] += 1.0f;
	end = center;
	end[2] -= 1024;
	Trace(trace, center, end);

	return trace.endpos;
}

/*
================
idAASFileLocal::EdgeBounds
================
*/
idBounds idAASFileLocal::EdgeBounds(int edgeNum) const
{
	const aasEdge_t *edge;
	idBounds bounds;

	edge = &edges[ abs(edgeNum)];
	bounds[0] = bounds[1] = vertices[ edge->vertexNum[0] ];
	bounds += vertices[ edge->vertexNum[1] ];
	return bounds;
}

/*
================
idAASFileLocal::FaceBounds
================
*/
idBounds idAASFileLocal::FaceBounds(int faceNum) const
{
	int i, edgeNum;
	const aasFace_t *face;
	const aasEdge_t *edge;
	idBounds bounds;

#ifdef _SPLASHDAMAGE
	edgeNum = faceNum;
	bounds.Clear();

	edge = &edges[ abs(edgeNum)];
	bounds.AddPoint(vertices[ edge->vertexNum[ INTSIGNBITSET(edgeNum)] ]);
#else
	face = &faces[faceNum];
	bounds.Clear();

	for (i = 0; i < face->numEdges; i++) {
		edgeNum = edgeIndex[ face->firstEdge + i ];
		edge = &edges[ abs(edgeNum)];
		bounds.AddPoint(vertices[ edge->vertexNum[ INTSIGNBITSET(edgeNum)] ]);
	}
#endif

	return bounds;
}

/*
================
idAASFileLocal::AreaBounds
================
*/
idBounds idAASFileLocal::AreaBounds(int areaNum) const
{
	int i, faceNum;
	const aasArea_t *area;
	idBounds bounds;

	area = &areas[areaNum];
	bounds.Clear();

#ifdef _SPLASHDAMAGE
	for (i = 0; i < area->numEdges; i++) {
		int edgeNum = edgeIndex[area->firstEdge + i];
		bounds += EdgeBounds(abs(edgeNum));
	}
#else
	for (i = 0; i < area->numFaces; i++) {
		faceNum = faceIndex[area->firstFace + i];
		bounds += FaceBounds(abs(faceNum));
	}
#endif

	return bounds;
}

/*
============
idAASFileLocal::PointAreaNum
============
*/
int idAASFileLocal::PointAreaNum(const idVec3 &origin) const
{
	int nodeNum;
	const aasNode_t *node;

	nodeNum = 1;

	do {
		node = &nodes[nodeNum];

		if (planeList[node->planeNum].Side(origin) == PLANESIDE_BACK) {
			nodeNum = node->children[1];
		} else {
			nodeNum = node->children[0];
		}

		if (nodeNum < 0) {
			return -nodeNum;
		}
	} while (nodeNum);

	return 0;
}

/*
============
idAASFileLocal::PointReachableAreaNum
============
*/
int idAASFileLocal::PointReachableAreaNum(const idVec3 &origin, const idBounds &searchBounds, const int areaFlags, const int excludeTravelFlags) const
{
#ifdef _SPLASHDAMAGE
	int result; // eax
	int v7; // ebx
	idVec3 v8;
	//float v8; // [esp+8h] [ebp-50h]
	//float v9; // [esp+Ch] [ebp-4Ch]
	//float v10; // [esp+10h] [ebp-48h]
	idVec3 v11; // [esp+14h] [ebp-44h] BYREF
	idBounds v12; // [esp+20h] [ebp-38h] BYREF
	idAASFileLocal::bestReachableArea_t v13; // [esp+38h] [ebp-20h] BYREF
	float maxStepHeight; // [esp+64h] [ebp+Ch]

	v13.v0 = this->settings.boundingBox[1].z - this->settings.boundingBox[0].z;
	v13.excludeTravelFlags = excludeTravelFlags;
	v13.v1 = 24.0;
	v13.distance1 = 1.0e30;
	v13.areaFlags = areaFlags;
	v13.distance2 = 1.0e30;
	v13.areaNum1 = 0;
	v13.areaNum2 = 0;
	idAASFileLocal::PointBestReachableAreaNum(&origin, &v13);
	if ( !v13.areaNum1 )
	{
		maxStepHeight = this->settings.maxStepHeight;
		v8.x = maxStepHeight * this->settings.invGravityDir.x;
		v8.y = this->settings.invGravityDir.y * maxStepHeight;
		v8.z = maxStepHeight * this->settings.invGravityDir.z;
		v11 = origin + v8;
		idAASFileLocal::PointBestReachableAreaNum(&v11, &v13);
	}
	if ( v13.v0 > (double)v13.distance1 )
		return v13.areaNum1;
	v7 = 0;
	v12[0].x = origin.x - 4.0;
	v12[0].y = origin.y - 4.0;
	v12[1].x = origin.x + 4.0;
	v12[1].y = origin.y + 4.0;
	v12[1].z = origin.z;
	v12[0].z = v12[1].z;
	while ( 1 )
	{
		result = v13.areaNum2;
		if ( v13.areaNum2 )
			break;
		idAASFileLocal::BoundsBestReachableAreaNum(&v12, &origin, 1, 0, &v13);
		++v7;
		v12.ExpandSelf(4.0f);
		if ( v7 >= 4 )
		{
			result = v13.areaNum2;
			if ( !v13.areaNum2 )
				return v13.areaNum1;
			return result;
		}
	}
	return result;
#else
	int areaList[32], areaNum, i;
	idVec3 start, end, pointList[32];
	aasTrace_t trace;
	idBounds bounds;
	float frac;

	start = origin;

	trace.areas = areaList;
	trace.points = pointList;
	trace.maxAreas = sizeof(areaList) / sizeof(int);
	trace.getOutOfSolid = true;

	areaNum = PointAreaNum(start);

	if (areaNum) {
		if ((areas[areaNum].flags & areaFlags) && ((areas[areaNum].travelFlags & excludeTravelFlags) == 0)) {
			return areaNum;
		}
	} else {
		// trace up
		end = start;
		end[2] += 32.0f;
		Trace(trace, start, end);

		if (trace.numAreas >= 1) {
			if ((areas[0].flags & areaFlags) && ((areas[0].travelFlags & excludeTravelFlags) == 0)) {
				return areaList[0];
			}

			start = pointList[0];
			start[2] += 1.0f;
		}
	}

	// trace down
	end = start;
	end[2] -= 32.0f;
	Trace(trace, start, end);

	if (trace.lastAreaNum) {
		if ((areas[trace.lastAreaNum].flags & areaFlags) && ((areas[trace.lastAreaNum].travelFlags & excludeTravelFlags) == 0)) {
			return trace.lastAreaNum;
		}

		start = trace.endpos;
	}

	// expand bounds until an area is found
	for (i = 1; i <= 12; i++) {
		frac = i * (1.0f / 12.0f);
		bounds[0] = origin + searchBounds[0] * frac;
		bounds[1] = origin + searchBounds[1] * frac;
		areaNum = BoundsReachableAreaNum(bounds, areaFlags, excludeTravelFlags);

		if (areaNum && (areas[areaNum].flags & areaFlags) && ((areas[areaNum].travelFlags & excludeTravelFlags) == 0)) {
			return areaNum;
		}
	}

	return 0;
#endif
}

/*
============
idAASFileLocal::BoundsReachableAreaNum_r
============
*/
int idAASFileLocal::BoundsReachableAreaNum_r(int nodeNum, const idBounds &bounds, const int areaFlags, const int excludeTravelFlags) const
{
	int res;
	const aasNode_t *node;

	while (nodeNum) {
		if (nodeNum < 0) {
			if ((areas[-nodeNum].flags & areaFlags) && ((areas[-nodeNum].travelFlags & excludeTravelFlags) == 0)) {
				return -nodeNum;
			}

			return 0;
		}

		node = &nodes[nodeNum];
		res = bounds.PlaneSide(planeList[node->planeNum]);

		if (res == PLANESIDE_BACK) {
			nodeNum = node->children[1];
		} else if (res == PLANESIDE_FRONT) {
			nodeNum = node->children[0];
		} else {
			nodeNum = BoundsReachableAreaNum_r(node->children[1], bounds, areaFlags, excludeTravelFlags);

			if (nodeNum) {
				return nodeNum;
			}

			nodeNum = node->children[0];
		}
	}

	return 0;
}

/*
============
idAASFileLocal::BoundsReachableAreaNum
============
*/
int idAASFileLocal::BoundsReachableAreaNum(const idBounds &bounds, const int areaFlags, const int excludeTravelFlags) const
{

	return BoundsReachableAreaNum_r(1, bounds, areaFlags, excludeTravelFlags);
}

/*
============
idAASFileLocal::PushPointIntoAreaNum
============
*/
void idAASFileLocal::PushPointIntoAreaNum(int areaNum, idVec3 &point) const
{
#ifdef _SPLASHDAMAGE //karin: removed in ETQW game
	// TODO
	common->Error("Disable idAASFileLocal::PushPointIntoAreaNum(%d)", areaNum);
#else
	int i, faceNum;
	const aasArea_t *area;
	const aasFace_t *face;

	area = &areas[areaNum];

	// push the point to the right side of all area face planes
	for (i = 0; i < area->numFaces; i++) {
		faceNum = faceIndex[area->firstFace + i];
		face = &faces[abs(faceNum)];

		const idPlane &plane = planeList[face->planeNum ^ INTSIGNBITSET(faceNum)];
		float dist = plane.Distance(point);

		// project the point onto the face plane if it is on the wrong side
		if (dist < 0.0f) {
			point -= dist * plane.Normal();
		}
	}
#endif
}

/*
============
idAASFileLocal::Trace
============
*/
#define TRACEPLANE_EPSILON		0.125f

typedef struct aasTraceStack_s {
	idVec3			start;
	idVec3			end;
	int				planeNum;
	int				nodeNum;
} aasTraceStack_t;

bool idAASFileLocal::Trace(aasTrace_t &trace, const idVec3 &start, const idVec3 &end) const
{
#ifdef _SPLASHDAMAGE
	return Trace(&trace, &start, &end);
#else
	int side, nodeNum, tmpPlaneNum;
	double front, back, frac;
	idVec3 cur_start, cur_end, cur_mid, v1, v2;
	aasTraceStack_t tracestack[MAX_AAS_TREE_DEPTH];
	aasTraceStack_t *tstack_p;
	const aasNode_t *node;
	const idPlane *plane;

	trace.numAreas = 0;
	trace.lastAreaNum = 0;
	trace.blockingAreaNum = 0;

	tstack_p = tracestack;
	tstack_p->start = start;
	tstack_p->end = end;
	tstack_p->planeNum = 0;
	tstack_p->nodeNum = 1;		//start with the root of the tree
	tstack_p++;

	while (1) {

		tstack_p--;

		// if the trace stack is empty
		if (tstack_p < tracestack) {
			if (!trace.lastAreaNum) {
				// completely in solid
				trace.fraction = 0.0f;
				trace.endpos = start;
			} else {
				// nothing was hit
				trace.fraction = 1.0f;
				trace.endpos = end;
			}

			trace.planeNum = 0;
			return false;
		}

		// number of the current node to test the line against
		nodeNum = tstack_p->nodeNum;

		// if it is an area
		if (nodeNum < 0) {
			// if can't enter the area
			if ((areas[-nodeNum].flags & trace.flags) || (areas[-nodeNum].travelFlags & trace.travelFlags)) {
				if (!trace.lastAreaNum) {
					trace.fraction = 0.0f;
					v1 = vec3_origin;
				} else {
					v1 = end - start;
					v2 = tstack_p->start - start;
					trace.fraction = v2.Length() / v1.Length();
				}

				trace.endpos = tstack_p->start;
				trace.blockingAreaNum = -nodeNum;
				trace.planeNum = tstack_p->planeNum;
				// always take the plane with normal facing towards the trace start
				plane = &planeList[trace.planeNum];

				if (v1 * plane->Normal() > 0.0f) {
					trace.planeNum ^= 1;
				}

				return true;
			}

			trace.lastAreaNum = -nodeNum;

			if (trace.numAreas < trace.maxAreas) {
				if (trace.areas) {
					trace.areas[trace.numAreas] = -nodeNum;
				}

				if (trace.points) {
					trace.points[trace.numAreas] = tstack_p->start;
				}

				trace.numAreas++;
			}

			continue;
		}

		// if it is a solid leaf
		if (!nodeNum) {
			if (!trace.lastAreaNum) {
				trace.fraction = 0.0f;
				v1 = vec3_origin;
			} else {
				v1 = end - start;
				v2 = tstack_p->start - start;
				trace.fraction = v2.Length() / v1.Length();
			}

			trace.endpos = tstack_p->start;
			trace.blockingAreaNum = 0;	// hit solid leaf
			trace.planeNum = tstack_p->planeNum;
			// always take the plane with normal facing towards the trace start
			plane = &planeList[trace.planeNum];

			if (v1 * plane->Normal() > 0.0f) {
				trace.planeNum ^= 1;
			}

			if (!trace.lastAreaNum && trace.getOutOfSolid) {
				continue;
			} else {
				return true;
			}
		}

		// the node to test against
		node = &nodes[nodeNum];
		// start point of current line to test against node
		cur_start = tstack_p->start;
		// end point of the current line to test against node
		cur_end = tstack_p->end;
		// the current node plane
		plane = &planeList[node->planeNum];

		front = plane->Distance(cur_start);
		back = plane->Distance(cur_end);

		// if the whole to be traced line is totally at the front of this node
		// only go down the tree with the front child
		if (front >= -ON_EPSILON && back >= -ON_EPSILON) {
			// keep the current start and end point on the stack and go down the tree with the front child
			tstack_p->nodeNum = node->children[0];
			tstack_p++;

			if (tstack_p >= &tracestack[MAX_AAS_TREE_DEPTH]) {
				common->Error("idAASFileLocal::Trace: stack overflow\n");
				return false;
			}
		}
		// if the whole to be traced line is totally at the back of this node
		// only go down the tree with the back child
		else if (front < ON_EPSILON && back < ON_EPSILON) {
			// keep the current start and end point on the stack and go down the tree with the back child
			tstack_p->nodeNum = node->children[1];
			tstack_p++;

			if (tstack_p >= &tracestack[MAX_AAS_TREE_DEPTH]) {
				common->Error("idAASFileLocal::Trace: stack overflow\n");
				return false;
			}
		}
		// go down the tree both at the front and back of the node
		else {
			tmpPlaneNum = tstack_p->planeNum;

			// calculate the hit point with the node plane
			// put the cross point TRACEPLANE_EPSILON on the near side
			if (front < 0) {
				frac = (front + TRACEPLANE_EPSILON) / (front - back);
			} else {
				frac = (front - TRACEPLANE_EPSILON) / (front - back);
			}

			if (frac < 0) {
				frac = 0.001f; //0
			} else if (frac > 1) {
				frac = 0.999f; //1
			}

			cur_mid = cur_start + (cur_end - cur_start) * frac;

			// side the front part of the line is on
			side = front < 0;

			// first put the end part of the line on the stack (back side)
			tstack_p->start = cur_mid;
			tstack_p->planeNum = node->planeNum;
			tstack_p->nodeNum = node->children[!side];
			tstack_p++;

			if (tstack_p >= &tracestack[MAX_AAS_TREE_DEPTH]) {
				common->Error("idAASFileLocal::Trace: stack overflow\n");
				return false;
			}

			// now put the part near the start of the line on the stack so we will
			// continue with that part first.
			tstack_p->start = cur_start;
			tstack_p->end = cur_mid;
			tstack_p->planeNum = tmpPlaneNum;
			tstack_p->nodeNum = node->children[side];
			tstack_p++;

			if (tstack_p >= &tracestack[MAX_AAS_TREE_DEPTH]) {
				common->Error("idAASFileLocal::Trace: stack overflow\n");
				return false;
			}
		}
	}

	return false;
#endif
}

/*
============
idAASLocal::AreaContentsTravelFlags
============
*/
int idAASFileLocal::AreaContentsTravelFlags(int areaNum) const
{
	if (areas[areaNum].contents & AREACONTENTS_WATER) {
		return TFL_WATER;
	}

	return TFL_AIR;
}

/*
============
idAASFileLocal::MaxTreeDepth_r
============
*/
void idAASFileLocal::MaxTreeDepth_r(int nodeNum, int &depth, int &maxDepth) const
{
	const aasNode_t *node;

	if (nodeNum <= 0) {
		return;
	}

	depth++;

	if (depth > maxDepth) {
		maxDepth = depth;
	}

	node = &nodes[nodeNum];
	MaxTreeDepth_r(node->children[0], depth, maxDepth);
	MaxTreeDepth_r(node->children[1], depth, maxDepth);

	depth--;
}

/*
============
idAASFileLocal::MaxTreeDepth
============
*/
int idAASFileLocal::MaxTreeDepth(void) const
{
	int depth, maxDepth;

	depth = maxDepth = 0;
	MaxTreeDepth_r(1, depth, maxDepth);
	return maxDepth;
}

#ifdef _SPLASHDAMAGE

typedef unsigned int _DWORD;
#define __int16 short
typedef unsigned short _WORD;
typedef unsigned char _BYTE;
#define LODWORD(x) ((DWORD)(*(uint64_t *)&(x)))

bool idAASFileLocal::PushPointIntoArea( int areaNum, idVec3 &point ) const {
  bool v4; // bl
  const aasArea_t *v5; // esi
  int v6; // ecx
  unsigned int v7; // edx
  const aasEdge_t *list; // eax
  bool v9; // zf
  const aasEdge_t *v10; // eax
  const idVec3 *v11; // edx
  const idVec3 *p_x; // esi
  const idVec3 *v13; // ebx
  double v14; // st7
  double v15; // st7
  long double v16; // st6
  long double v17; // st5
  long double v18; // st4
  long double v19; // st5
  long double v20; // st4
  double v21; // st3
  double v22; // st3
  long double v23; // st4
  long double v24; // st6
  long double v25; // rt2
  long double v26; // rtt
  double v27; // st3
  bool result; // al
  float v29; // [esp+8h] [ebp-98h]
  float v30; // [esp+8h] [ebp-98h]
  float v31; // [esp+8h] [ebp-98h]
  float v32; // [esp+8h] [ebp-98h]
  float v33; // [esp+8h] [ebp-98h]
  float v34; // [esp+8h] [ebp-98h]
  char v35; // [esp+Fh] [ebp-91h]
  float v36; // [esp+10h] [ebp-90h]
  float v37; // [esp+10h] [ebp-90h]
  float v38; // [esp+10h] [ebp-90h]
  float v39; // [esp+10h] [ebp-90h]
  float v40; // [esp+10h] [ebp-90h]
  float v41; // [esp+10h] [ebp-90h]
  float v42; // [esp+10h] [ebp-90h]
  char v43; // [esp+17h] [ebp-89h]
	idVec3 v44; // v44 v45 v48
  //float v44; // [esp+18h] [ebp-88h]
  //float v45; // [esp+1Ch] [ebp-84h]
  float v46; // [esp+1Ch] [ebp-84h]
  float v47; // [esp+1Ch] [ebp-84h]
  //float v48; // [esp+20h] [ebp-80h]
  float v49; // [esp+20h] [ebp-80h]
  float v50; // [esp+20h] [ebp-80h]
  float v51; // [esp+20h] [ebp-80h]
  float v52; // [esp+24h] [ebp-7Ch]
  int v53; // [esp+28h] [ebp-78h]
  const aasArea_t *v56; // [esp+38h] [ebp-68h]
  float v57; // [esp+3Ch] [ebp-64h]
	idVec3 v58; // v58 v59 v60
  //float v58; // [esp+40h] [ebp-60h]
  //float v59; // [esp+44h] [ebp-5Ch]
  //float v60; // [esp+48h] [ebp-58h]
	idVec3 v61; // v61 v62 v63
  //float v61; // [esp+4Ch] [ebp-54h]
  //float v62; // [esp+50h] [ebp-50h]
  //float v63; // [esp+54h] [ebp-4Ch]
	idVec3 v64; // v64 v65 v66
  //float v64; // [esp+58h] [ebp-48h]
  //float v65; // [esp+5Ch] [ebp-44h]
  //float v66; // [esp+60h] [ebp-40h]
	idVec3 v67; // v67 v68 v69
  //float v67; // [esp+64h] [ebp-3Ch]
  //float v68; // [esp+68h] [ebp-38h]
  //float v69; // [esp+6Ch] [ebp-34h]
	idVec3 v70; // v70 v71 v72
  //float v70; // [esp+70h] [ebp-30h]
  //float v71; // [esp+74h] [ebp-2Ch]
  //float v72; // [esp+78h] [ebp-28h]
	idVec3 v73; // v73 v74 v75
  //float v73; // [esp+7Ch] [ebp-24h]
  //float v74; // [esp+80h] [ebp-20h]
  //float v75; // [esp+84h] [ebp-1Ch]
	idVec3 v78; // v78 v79 v80
  //float v78; // [esp+94h] [ebp-Ch]
  //float v79; // [esp+98h] [ebp-8h]
  //float v80; // [esp+9Ch] [ebp-4h]
  idVec3 v81; // 0:^34.12
  idVec3 v82; // 0:^90.12

  v57 = 1.0e30;
  v82 = point;
  v81 = point;
  v4 = false;
  v5 = &this->areas[areaNum];
  v35 = 0;
  v43 = 0;
  v56 = v5;
  v53 = 0;
  if ( v5->numEdges <= 0 )
    return v4;
  do
  {
    v6 = this->edgeIndex[v53 + v5->firstEdge];
    v7 = abs(v6);
    list = this->edges.Ptr();
    v9 = (list[v7].flags & AAS_EDGE_VERTICAL/* 0x40 */) == 0;
    v10 = &list[v7];
    if ( !v9 )
      goto LABEL_53;
    v11 = this->vertices.Ptr();
    p_x = &v11[v10->vertexNum[(unsigned int)v6 >> 31]];
    v78 = *p_x - v82;
    v13 = &v11[v10->vertexNum[v6 >= 0]];
    v29 = v78.LengthSqr();
    if ( v57 > (double)v29 )
    {
      v57 = v78.LengthSqr();
      v81 = *p_x;
    }
    v61 = *v13 - *p_x;
  	v58 = v61.Cross(this->settings.invGravityDir);
    v44 = v58;
    v30 = v58.LengthSqr();
    v31 = sqrt(v30);
    v14 = v31;
    if ( v31 >= 0.00000011920929 )
    {
      v33 = 1.0 / v14;
      v44 = v33 * v58;
      v32 = v14;
      v15 = 0.0;
    }
    else
    {
      v15 = 0.0;
      v32 = 0.0;
    }
    v16 = v44.x;
    v17 = v44.z;
    v18 = v44.y;
    if ( v44.x == v15 )
    {
      if ( v44.y == v15 )
      {
        v19 = v44.y;
        v20 = v44.z;
        if ( v44.z <= v15 )
          v21 = -1.0;
        else
          v21 = 1.0;
        if ( v21 != v20 )
        {
          v49 = v21;
          v20 = v49;
        }
        goto LABEL_44;
      }
      if ( v15 == v17 )
      {
        if ( v18 <= v15 )
          v22 = -1.0;
        else
          v22 = 1.0;
        if ( v22 != v18 )
        {
          v46 = v22;
          v18 = v46;
        }
        goto LABEL_43;
      }
    }
    else if ( v44.y == v15 && v15 == v17 )
    {
      v23 = v44.x;
      v24 = v44.y;
      if ( v44.x <= v15 )
      {
        if ( -1.0 != v23 )
          v23 = (float)-1.0;
      }
      else if ( 1.0 != v23 )
      {
        v23 = (float)1.0;
      }
      goto LABEL_42;
    }
    v36 = fabs(v16);
    if ( 1.0 == v36 )
    {
      if ( v15 == v18 && v15 == v17 )
        goto LABEL_43;
      v50 = v15;
      v17 = v50;
      v24 = v50;
      v23 = v44.x;
LABEL_42:
      v25 = v23;
      v18 = v24;
      v16 = v25;
LABEL_43:
      v26 = v18;
      v20 = v17;
      v19 = v26;
      goto LABEL_44;
    }
    v37 = fabs(v18);
    if ( v37 == 1.0 )
    {
      if ( v15 != v16 || v15 != v17 )
      {
        v51 = v15;
        v17 = v51;
        v16 = v51;
        v18 = v44.y;
      }
      goto LABEL_43;
    }
    v38 = fabs(v17);
    if ( v38 != 1.0 || v15 == v16 && v15 == v18 )
      goto LABEL_43;
    v47 = v15;
    v19 = v47;
    v16 = v47;
    v20 = v44.z;
LABEL_44:
    if ( v15 != v32 )
    {
      v39 = v19 * v13->y + v13->x * v16 + v13->z * v20;
      v52 = -v39;
    }
    v34 = point.x * v16 + point.y * v19 + v20 * point.z + v52;
    v27 = v34;
    if ( v34 < v15 )
    {
      v35 = 1;
      v70.x = v16 * v27;
      v70.y = v19 * v27;
      v70.z = v27 * v20;
      point = point - v70;
      v34 = v15;
    }
    v40 = fabs(v34);
    if ( v40 < 0.1000000014901161 )
    {
      v67 = *v13 - *p_x;
      v64 = point - *p_x;
      v41 = v64 * v67;
      if ( v41 >= v15 )
      {
        v73 = point - *v13;
        v42 = v67 * v73;
        if ( v15 >= v42 )
          v43 = 1;
      }
    }
    v4 = v35;
    v5 = v56;
LABEL_53:
    ++v53;
  }
  while ( v53 < v5->numEdges );
  if ( !v4 )
    return v4;
  result = v4;
  if ( !v43 )
    point = v81;
  return result;
}

bool idAASFileLocal::TraceHeight( aasTraceHeight_t &trace, const idVec3 &start, const idVec3 &end ) const {
  aasTraceStack_t *v4; // edx
  double y; // st7
  int v6; // eax
  const aasNode_t *list; // esi
  int v8; // eax
  bool v9; // zf
  const aasNode_t *v10; // esi
  int numPoints; // eax
  idVec3 *points; // ebx
  int v13; // eax
  idVec3 *v14; // eax
  int planeNum; // ebx
  const idPlane *v16; // eax
  double v17; // st6
  double v18; // st7
  double v19; // st5
  double v20; // st4
  double v21; // st2
  int v22; // eax
  int v23; // ebp
  double v24; // st1
  double v25; // st2
  double v26; // st2
  double v27; // st1
  float v29; // [esp+10h] [ebp-1040h]
  float v30; // [esp+10h] [ebp-1040h]
  float v31; // [esp+10h] [ebp-1040h]
  idVec3 v32; // v32 v33 v34
  //float v32; // [esp+14h] [ebp-103Ch]
  //float v33; // [esp+18h] [ebp-1038h]
  //float v34; // [esp+1Ch] [ebp-1034h]
  idVec3 v35; // v35 v36 v37
  //float v35; // [esp+20h] [ebp-1030h]
  //float v36; // [esp+24h] [ebp-102Ch]
  //float v37; // [esp+28h] [ebp-1028h]
  idVec3 v38; // v38 v39 v40
  //float v38; // [esp+2Ch] [ebp-1024h]
  //float v39; // [esp+30h] [ebp-1020h]
  //float v40; // [esp+34h] [ebp-101Ch]
  idVec3 v41; // v41 v42 v43
  //float v41; // [esp+38h] [ebp-1018h]
  //float v42; // [esp+3Ch] [ebp-1014h]
  //float v43; // [esp+40h] [ebp-1010h]
  idVec3 v44; // v44 v45 v46
  //float v44; // [esp+44h] [ebp-100Ch]
  //float v45; // [esp+48h] [ebp-1008h]
  //float v46; // [esp+4Ch] [ebp-1004h]
  aasTraceStack_t v47[MAX_AAS_TREE_DEPTH];
  // float v47[1024]; // [esp+50h] [ebp-1000h] BYREF
  aasTraceStack_t *retaddr = &v47[MAX_AAS_TREE_DEPTH]; // (v47 + 1024) - 32; // [esp+1050h] [ebp+0h] BYREF // aasTraceStack_t

	idVec3 _v17; // v17 v18 v19
	v4 = v47;
  trace.numPoints = 0;
  v4->start = start;
  y = start.y;
  v4->planeNum = 0;
  v4->nodeNum = 1; // LODWORD(v47[7]) = 1;
	v4->end = end;
  while ( 1 )
  {
    v6 = v4->nodeNum;
    if ( !v6 )
      goto LABEL_23;
    list = this->nodes.Ptr();
    v8 = v6;
    v9 = (list[v8].flags & AAS_NODE_FLAG_COLUMN_HEIGHT) == 0; // 2
    v10 = &list[v8];
    if ( !v9 )
    {
      numPoints = trace.numPoints;
      if ( numPoints < trace.maxPoints )
      {
        points = trace.points;
        v13 = numPoints;
        points[v13] = v4->start;
        v14 = &points[v13];
        trace.points[trace.numPoints++].z = (float)((v10->flags >> 2) - 0x2000);
      }
      goto LABEL_23;
    }
    planeNum = v10->planeNum;
    v44 = v4->start;
    v16 = &this->planeList[planeNum];
    v32 = v4->end;
    _v17 = v44;
    v29 = v16->Distance(v44);
    v20 = v29;
    v30 = v16->Distance(v32);
    v21 = v30;
    if ( v20 >= -ON_EPSILON && v21 >= -ON_EPSILON )
    {
      v22 = v10->children[0];
      goto LABEL_22;
    }
    if ( v20 < ON_EPSILON && v21 < ON_EPSILON )
    {
      v22 = v10->children[1];
      goto LABEL_22;
    }
    v23 = v4->planeNum;
    v24 = v20 >= 0.0 ? v20 - TRACEPLANE_EPSILON : v20 + TRACEPLANE_EPSILON;
    v25 = v24 / (v20 - v21);
    if ( v25 >= 0.0 )
    {
      if ( v25 <= 1.0 )
      {
        v27 = v25;
        v26 = 0.0;
      }
      else
      {
        v26 = 0.0;
        v27 = 0.999f;
      }
    }
    else
    {
      v26 = 0.0;
      v27 = 0.001f;
    }
    v35 = v32 - _v17;
    v31 = v27;
    v38 = v35 * v31;
    v41 = v38 + _v17;
    v4->planeNum = planeNum;
    v4->start = v41;
    v4->nodeNum = v10->children[v26 <= v20];
    v4 += 1;
    if ( v4 >= retaddr )
      break;
    v22 = v10->children[v26 > v20];
    v4->start = v44;
    v4->end = v41;
    v4->planeNum = v23;
LABEL_22:
    v4->nodeNum = v22;
    v4 += 1;
    if ( v4 >= retaddr )
      break;
LABEL_23:
    v4 -= 1;
    if ( v4 < v47 )
      return true;
  }
  common->Warning("idAASFileLocal::Trace: stack overflow");
  return false;
}

bool idAASFileLocal::TraceFloor( aasTraceFloor_t &trace, const idVec3 &start, int startAreaNum, const idVec3 &end, int endAreaNum, int travelFlags ) const
{
  struct aasTraceFloor_t *v7; // ebp
  long double a; // st6
  double v10; // st5
  long double c; // st3
  double b; // st2
  long double v13; // st7
  long double v14; // st5
  long double v15; // st7
  long double v16; // st5
  long double v17; // st4
  long double v18; // st6
  long double v19; // rt0
  long double v20; // st4
  long double v21; // st6
  long double v22; // st5
  long double v23; // st4
  long double v24; // st5
  long double v25; // st7
  double v26; // st4
  long double v27; // st7
  double v28; // st4
  double v29; // st4
  long double v30; // st6
  long double v31; // rtt
  long double v32; // st4
  long double v33; // st3
  double x; // st7
  int i; // eax
  int v36; // edx
  int granularity; // eax
  bool v38; // cc
  int num; // ebx
  int size; // eax
  int v41; // eax
  int *list; // edi
  int j; // eax
  double v44; // st7
  int v45; // edi
  int v46; // eax
  double v47; // st4
	const aasReachability_t *v48;
  //float v48; // ecx
  double v49; // st5
  double v50; // st7
  int v51; // ebx
  const aasArea_t *v52; // eax
  int v53; // eax
  int v54; // ebp
  int v55; // eax
  int v56; // eax
  int *v57; // edi
  int k; // eax
  double v59; // st7
  double v60; // st5
  int v61; // ebp
  int m; // eax
  int v63; // edx
  double v64; // st7
  double v65; // st7
  int *v66; // edi
  int n; // eax
  //float v68; // eax
  const aasReachability_t *v68; // eax
  int ii; // eax
  int v70; // edx
  float v72; // [esp+8h] [ebp-84h]
  float v73; // [esp+8h] [ebp-84h]
  float v74; // [esp+8h] [ebp-84h]
  float v75; // [esp+8h] [ebp-84h]
  float v76; // [esp+8h] [ebp-84h]
  float v77; // [esp+8h] [ebp-84h]
  float v78; // [esp+8h] [ebp-84h]
  float v79; // [esp+8h] [ebp-84h]
  float v80; // [esp+8h] [ebp-84h]
  float v81; // [esp+8h] [ebp-84h]
  float v82; // [esp+8h] [ebp-84h]
  float v83; // [esp+8h] [ebp-84h]
  float v84; // [esp+8h] [ebp-84h]
  float v85; // [esp+8h] [ebp-84h]
  float v86; // [esp+8h] [ebp-84h]
  float v87; // [esp+Ch] [ebp-80h]
  float v88; // [esp+Ch] [ebp-80h]
  float v89; // [esp+Ch] [ebp-80h]
  float v90; // [esp+Ch] [ebp-80h]
  float v91; // [esp+Ch] [ebp-80h]
  float v92; // [esp+Ch] [ebp-80h]
  float v93; // [esp+Ch] [ebp-80h]
  float v94; // [esp+10h] [ebp-7Ch]
	idVec3 v95; // v95 v100 v105
  //float v95; // [esp+10h] [ebp-7Ch]
	idVec3 v96; // v96 v101 v106
  //float v96; // [esp+10h] [ebp-7Ch]
	idVec3 v97; // v97 v102 v107
  //float v97; // [esp+10h] [ebp-7Ch]
  float v98; // [esp+10h] [ebp-7Ch]
  float v99; // [esp+14h] [ebp-78h]
  //float v100; // [esp+14h] [ebp-78h]
  //float v101; // [esp+14h] [ebp-78h]
  //float v102; // [esp+14h] [ebp-78h]
  float v103; // [esp+14h] [ebp-78h]
  float v104; // [esp+18h] [ebp-74h]
  //float v105; // [esp+18h] [ebp-74h]
  //float v106; // [esp+18h] [ebp-74h]
  //float v107; // [esp+18h] [ebp-74h]
  float v108; // [esp+18h] [ebp-74h]
  idPlane v109; // [esp+1Ch] [ebp-70h] BYREF
  int v110; // [esp+2Ch] [ebp-60h]
  idPlane v111; // [esp+30h] [ebp-5Ch] BYREF
  float v112; // [esp+40h] [ebp-4Ch]
	idVec3 v113; // v113 v114 v115
  //float v113; // [esp+44h] [ebp-48h]
  //float v114; // [esp+48h] [ebp-44h]
  //float v115; // [esp+4Ch] [ebp-40h]
  int v116; // [esp+50h] [ebp-3Ch]
	floorEdgeSplitPoint_t v117; // v117 y z v120 v121
  //float v117; // [esp+54h] [ebp-38h] BYREF
  //float y; // [esp+58h] [ebp-34h]
  //float z; // [esp+5Ch] [ebp-30h]
  //float v120; // [esp+60h] [ebp-2Ch]
  //int v121; // [esp+64h] [ebp-28h]
	floorEdgeSplitPoint_t v122; // v122 v123 v124 v125 v126
  //float v122; // [esp+68h] [ebp-24h] BYREF
  //float v123; // [esp+6Ch] [ebp-20h]
  //float v124; // [esp+70h] [ebp-1Ch]
  //float v125; // [esp+74h] [ebp-18h]
  //int v126; // [esp+78h] [ebp-14h]
  float v127; // [esp+88h] [ebp-4h]

	idList<aasArea_t> &areas = (idList<aasArea_t> &)this->areas;
	idList<int> &floorIndex = (idList<int> &)this->floorIndex;

  v7 = &trace;
  trace.fraction = 0.0;
  trace.endpos = start;
  trace.lastAreaNum = startAreaNum;
  trace.lastEdgeNum = 0;
  v113 = end - start;
	idVec3 normal = v113.Cross(this->settings.gravityDir);
	v111.SetNormal(normal);
	v111.Normalize();
  a = v111[0];
  v10 = -1.0;
  c = v111[2];
  b = v111[1];
  if ( v111[0] != 0.0 )
  {
    if ( v111[1] == 0.0 && 0.0 == c )
    {
      v15 = v111[1];
      if ( a <= 0.0 )
      {
        v17 = v111[2];
        if ( -1.0 == a )
        {
LABEL_43:
          v16 = v17;
          goto LABEL_23;
        }
        v18 = v111[2];
      }
      else
      {
        v16 = v111[2];
        if ( 1.0 == a )
          goto LABEL_23;
        v10 = 1.0;
        v18 = v111[2];
      }
      v111[0] = v10;
      goto LABEL_22;
    }
LABEL_28:
    v14 = v111[1];
    v79 = fabs(a);
    if ( v79 == 1.0 )
    {
      v20 = v111[2];
      if ( 0.0 != v14 || 0.0 != v20 )
      {
        v111[2] = 0.0;
        v111[1] = 0.0;
        v13 = (float)0.0;
        v14 = v13;
        goto LABEL_24;
      }
      goto LABEL_44;
    }
    v80 = fabs(v14);
    if ( v80 != 1.0 )
    {
      v81 = fabs(c);
      v20 = v111[2];
      if ( v81 == 1.0 && (0.0 != a || 0.0 != v14) )
      {
        v13 = v111[2];
        v111[1] = 0.0;
        v111[0] = 0.0;
        v14 = (float)0.0;
        a = v14;
        goto LABEL_24;
      }
      goto LABEL_44;
    }
    v20 = v111[2];
    if ( 0.0 == a && 0.0 == v20 )
    {
LABEL_44:
      v13 = v20;
      goto LABEL_24;
    }
    v15 = v111[1];
    v111[2] = 0.0;
    v111[0] = 0.0;
    v18 = (float)0.0;
LABEL_22:
    v16 = v18;
    a = v111[0];
    goto LABEL_23;
  }
  if ( v111[1] != 0.0 )
  {
    if ( 0.0 == c )
    {
      v15 = v111[1];
      if ( b > 0.0 )
      {
        v16 = v111[2];
        if ( 1.0 != b )
        {
          v13 = v111[2];
          v111[1] = 1.0;
          v14 = (float)1.0;
          goto LABEL_24;
        }
        goto LABEL_23;
      }
      v17 = v111[2];
      if ( -1.0 != b )
      {
        v13 = v111[2];
        v111[1] = -1.0;
        v14 = (float)-1.0;
        goto LABEL_24;
      }
      goto LABEL_43;
    }
    goto LABEL_28;
  }
  v13 = v111[2];
  if ( v111[2] <= 0.0 )
  {
    if ( -1.0 != v13 )
    {
      v15 = v111[1];
      v111[2] = -1.0;
      v16 = (float)-1.0;
      goto LABEL_23;
    }
    v14 = v111[1];
  }
  else
  {
    v14 = v111[1];
    if ( 1.0 != v13 )
    {
      v15 = v111[1];
      v111[2] = 1.0;
      v16 = (float)1.0;
LABEL_23:
      v19 = v16;
      v14 = v15;
      v13 = v19;
    }
  }
LABEL_24:
  v75 = v14 * start.y + start.x * a + v13 * start.z;
  v111[3] = -v75;
  v95.x = v13 * this->settings.gravityDir.y - this->settings.gravityDir.z * v14;
  v95.y = this->settings.gravityDir.z * a - v13 * this->settings.gravityDir.x;
  v95.z = v14 * this->settings.gravityDir.x - a * this->settings.gravityDir.y;
  v109.SetNormal(v95);
	v109.Normalize();
  v21 = v109[0];
  v22 = v109[2];
  v23 = v109[1];
  if ( v109[0] != 0.0 )
  {
    if ( v109[1] == 0.0 && 0.0 == v22 )
    {
      v27 = v109[1];
      if ( v21 <= 0.0 )
        v29 = -1.0;
      else
        v29 = 1.0;
      if ( v29 == v21 )
        goto LABEL_66;
      v30 = v109[2];
      v109[0] = v29;
      goto LABEL_65;
    }
LABEL_77:
    v84 = fabs(v21);
    if ( 1.0 == v84 )
    {
      if ( 0.0 != v23 || 0.0 != v22 )
      {
        v109[2] = 0.0;
        v109[1] = 0.0;
        v25 = (float)0.0;
        v24 = v25;
        goto LABEL_67;
      }
      goto LABEL_91;
    }
    v85 = fabs(v23);
    if ( v85 == 1.0 )
    {
      if ( 0.0 != v21 || 0.0 != v22 )
      {
        v27 = v109[1];
        v109[2] = 0.0;
        v109[0] = 0.0;
        v30 = (float)0.0;
LABEL_65:
        v22 = v30;
        v21 = v109[0];
LABEL_66:
        v31 = v22;
        v24 = v27;
        v25 = v31;
        goto LABEL_67;
      }
    }
    else
    {
      v86 = fabs(v22);
      if ( v86 == 1.0 && (0.0 != v21 || 0.0 != v23) )
      {
        v25 = v109[2];
        v109[1] = 0.0;
        v109[0] = 0.0;
        v24 = (float)0.0;
        v21 = v24;
        goto LABEL_67;
      }
    }
LABEL_91:
    v27 = v109[1];
    goto LABEL_66;
  }
  if ( v109[1] == 0.0 )
  {
    v24 = v109[1];
    v25 = v109[2];
    if ( v109[2] <= 0.0 )
      v26 = -1.0;
    else
      v26 = 1.0;
    if ( v26 == v25 )
      goto LABEL_67;
    v27 = v109[1];
    v109[2] = v26;
    v22 = v109[2];
    goto LABEL_66;
  }
  if ( 0.0 != v22 )
    goto LABEL_77;
  v27 = v109[1];
  if ( v23 <= 0.0 )
    v28 = -1.0;
  else
    v28 = 1.0;
  if ( v28 == v27 )
    goto LABEL_66;
  v25 = v109[2];
  v109[1] = v28;
  v24 = v109[1];
LABEL_67:
  v32 = v24 * start.y;
  v33 = start.x * v21;
  v116 = startAreaNum;
  v82 = v32 + v33 + v25 * start.z;
  v109[3] = -v82;
  v83 = v25 * end.z + v21 * end.x + v24 * end.y;
  v127 = -v83;
  if ( !idAASFileLocal::GetFloorEdgeSplitPoints(
          (struct idAASFileLocal::floorEdgeSplitPoint_t *)&v122,
          (struct idAASFileLocal::floorEdgeSplitPoint_t *)&v117,
          startAreaNum,
          &v111,
          &v109) )
  {
    x = start.x;
    v117.edgeIndex = 0;
    v117.point = start;
    v122.edgeIndex = 0;
    v122.point = v117.point;
    v117.distance = 0.0;
    v122.distance = 0.0;
  }
  for ( i = 0; i < this->floorIndex.Num(); ++i )
  {
    v36 = this->floorIndex[i];
    areas[v36].flags &= ~AAS_AREA_FLOOD_VISITED /* 0x8000u */;
  }
  floorIndex.SetNum(0);
  while ( 1 )
  {
    v44 = v117.point[0];
    v45 = v116;
    floorIndex.Append(v116);
    areas[v45].flags |= AAS_AREA_FLOOD_VISITED /* 0x8000u */;
    v7->endpos = v117.point;
    v46 = v117.edgeIndex;
    v7->lastEdgeNum = v46;
    if ( v45 == endAreaNum )
      break;
    v112 = v109.Normal() * v7->endpos + v127;
    if ( v112 > 0.1000000014901161 )
      break;
    v47 = v7->endpos.y;
    v48 = this->areas[v45].reach;
    v49 = v109[0] * v7->endpos.x;
    v110 = this->floorIndex.Num();
  	v112 = v109.Normal() * v7->endpos;
    v50 = v112;
    const aasReachability_t *_v112 = v48;
    v109[3] = -v50;
    if ( v48 == NULL )
    {
LABEL_150:
    	v113 = v7->endpos - start;
    	idVec3 _v98 = end - start;
      float _v110 = v113.LengthSqr();
      v64 = _v110;
      _v110 = _v98.LengthSqr();
      _v110 = v64 / _v110;
      _v110 = sqrt(_v110);
      v65 = _v110;
      goto LABEL_159;
    }
    while ( 1 )
    {
      if ( ( v48->travelFlags & (unsigned __int16)~travelFlags) == 0 )
      {
        v51 = v48->travelFlags;
        v52 = &this->areas[v51];
        if ( (v52->travelFlags & (unsigned __int16)~travelFlags) == 0 && (v52->flags & AAS_AREA_FLOOD_VISITED /* 0x8000u */) == 0 )
        {
          floorIndex.Append(v51);
          areas[v51].flags |= AAS_AREA_FLOOD_VISITED/* 0x8000u */;
          idAASFileLocal::GetFloorEdgeSplitPoints(
            (struct idAASFileLocal::floorEdgeSplitPoint_t *)&v122,
            (struct idAASFileLocal::floorEdgeSplitPoint_t *)&v117,
            _v112->travelFlags,
            &v111,
            &v109);
          if ( v122.distance < 1.0e30 && v117.distance >= 0.1000000014901161 )
          {
            v96 = trace.endpos - v122.point;
            v87 = fabs(v96.x);
            if ( v87 < 0.00000011920929 )
              v96 = 0.0;
            v88 = fabs(v96.y);
            if ( v88 < 0.00000011920929 )
              v96.y = 0.0;
            v89 = fabs(v96.z);
            v59 = v96.z;
            if ( v89 < 0.00000011920929 ) {
            	v59 = (float)0.0;
            	v96.z = v59;
            }
            v90 = this->settings.gravityDir * v96;
            v113 = v90 * this->settings.gravityDir;
            v91 = v113.LengthSqr();
            v60 = v91;
            v92 = this->settings.maxStepHeight * this->settings.maxStepHeight;
            if ( v92 >= v60 )
            {
              v97 = v96 - v113;
              v93 = v97.LengthSqr();
              if ( v93 <= 0.040000003 )
                break;
            }
          }
        }
      }
      _v112 = _v112->next;
      v48 = _v112;
      if ( _v112 == NULL )
      {
        v45 = v116;
        v7 = &trace;
        goto LABEL_150;
      }
    }
    v61 = v110;
    for ( m = v110; m < this->floorIndex.Num(); ++m )
    {
      v63 = this->floorIndex[m];
      areas[v63].flags &= ~AAS_AREA_FLOOD_VISITED/* 0x8000u */;
    }
    v68 = _v112;
    floorIndex.SetNum(v61);
    v7 = &trace;
    v116 = v68->toAreaNum;
  }
  v7->endpos = end;
  v65 = 1.0;
LABEL_159:
  v7->lastAreaNum = v45;
  v7->fraction = v65;
  for ( ii = 0; ii < this->floorIndex.Num(); ++ii )
  {
    v70 = this->floorIndex[ii];
    areas[v70].flags &= ~AAS_AREA_FLOOD_VISITED/* 0x8000u */;
  }
  floorIndex.Clear();
  return true;
}

bool idAASFileLocal::SplitFloorWinding(
		int a2, // areaNum
		const idPlane *a3,
		float *a4,
		int *a5) const
{
	const aasArea_t *list; // eax
	int v6; // ebp
	bool v7; // cc
	const aasArea_t *v8; // eax
	float *v9; // edi
	int v10; // esi
	const idVec3 *v11; // eax
	int v12; // eax
	int v14; // [esp+4h] [ebp-8h]
	int v15; // [esp+8h] [ebp-4h]
	const aasArea_t *v16; // [esp+10h] [ebp+4h]

	list = this->areas.Ptr();
	v6 = 0;
	v7 = list[a2].numEdges <= 0;
	v8 = &list[a2];
	v15 = 1;
	v14 = 0;
	v16 = v8;
	if ( !v7 )
	{
		v9 = a4;
		do
		{
			v10 = this->edgeIndex[v6 + v8->firstEdge];
			v11 = &this->vertices[this->edges[abs(v10)].vertexNum[v10 >> 31]];
			++v6;
			float d = a3->Distance(*v11);
			*v9++ = d; // a4 += 1
			v12 = FLOATSIGNBITSET(d); // check distance less than 0; v12 = 1 if distance < 0 // *((_DWORD *)v9 - 1) >> 31;
			v15 &= v12;
			v14 |= v12;
			*a5++ = v12; // a5 += 1 // *(_DWORD *)((char *)v9 + ((char *)a5 - (char *)a4) - 4) = v12; // a5 - a4 == align16( (1+numEdges) * sizeof(int/float) )
			v8 = v16;
			//Sys_Printf("EEE %d/%d: %f %d | %d %d\n", v6-1,v8->numEdges, d,v12,v15, v14);
		}
		while ( v6 < v16->numEdges );
	}
	a4[v8->numEdges] = *a4;
	a5[v8->numEdges] = *a5;
	return v15 != v14; // all distance >= 0; v14 == v15 if has distance < 0
}

//----- (005EC1D0) --------------------------------------------------------
bool idAASFileLocal::GetFloorEdgeSplitPoints(
        idAASFileLocal::floorEdgeSplitPoint_t *a2,
        idAASFileLocal::floorEdgeSplitPoint_t *a3,
        int a4, // areaNum
        const idPlane *a5,
        const idPlane *a6) const
{
  float *v8; // esp
  int *v9; // esp
  bool result; // al
  int v11; // eax
  float *v12; // ebx
  //idPlane *v12; // ebx
  int v13; // ecx
  int v14; // ecx // edgeIndex
  const aasEdge_t *v15; // edx
  const idVec3 *v16; // eax
  const idVec3 *v17; // edx
  double v18; // st6
  double v19; // st7
  double v20; // st5
  int v21; // [esp+0h] [ebp-3Ch] BYREF
  _BYTE v22[8]; // [esp+4h] [ebp-38h] BYREF
  idVec3 v23; // v23 v24 v25
  //float v23; // [esp+Ch] [ebp-30h]
  //float v24; // [esp+10h] [ebp-2Ch]
  //float v25; // [esp+14h] [ebp-28h]
  idVec3 v26; // v26 v27 v28
  //float v26; // [esp+18h] [ebp-24h]
  //float v27; // [esp+1Ch] [ebp-20h]
  //float v28; // [esp+20h] [ebp-1Ch]
  idVec3 v29; // edgeVec // v29 v30 v31
  //float v29; // [esp+24h] [ebp-18h]
  //float v30; // [esp+28h] [ebp-14h]
  //float v31; // [esp+2Ch] [ebp-10h]
  int i; // [esp+30h] [ebp-Ch]
  int *v33; // [esp+34h] [ebp-8h]
  const aasArea_t *v34; // [esp+38h] [ebp-4h]
  int v35; // [esp+4Ch] [ebp+10h]
  int v36; // [esp+4Ch] [ebp+10h]
  float *v37; // [esp+50h] [ebp+14h]
  //idPlane *v37; // [esp+50h] [ebp+14h]
  float v38; // [esp+50h] [ebp+14h]
  float v39; // [esp+50h] [ebp+14h]

	idVec3 _v19;
  a2->point = vec3_origin;
  a2->edgeIndex = 0;
  a2->distance = 1.0e30;
  a3->point = vec3_origin;
  a3->edgeIndex = 0;
  a3->distance = -1.0e30;
  v34 = &this->areas[a4];
  v35 = 4 * (v34->numEdges + 1) + 15;
  v8 = (float *)alloca(v35);
  v9 = (int *)alloca(v35);
  v33 = &v21;
  result = idAASFileLocal::SplitFloorWinding(a4, a5, v8, v9); // (this, a4, a5, (float *)&v21, &v21);
  if ( result ) // all distance >= 0
  {
    v11 = 0;
    v36 = 0;
    if ( v34->numEdges > 0 )
    {
      // v12 = (idPlane *)v22;
      // v13 = (char *)v33 - (char *)&v21; // == sizeof(v8/v9)
      // v37 = (idPlane *)v22;
v33 = v9;
v12 = v8;
int *_v12 = v9;
v37 = v8;
int lastSign = v9[0]; // first sign
      for ( ; ; /*i = (char *)v33 - (char *)&v21; ; v13 = i*/ )
      {
        //if ( v33[v11] != *_v12 /* *(_DWORD *)((char *)&v12->ToFloatPtr()[0] + v13)*/ )
		if(lastSign != v33[v11]) // cross this edge
        {
          v14 = this->edgeIndex[v11 + v34->firstEdge];
          v15 = &this->edges[abs(v14)];
          v16 = &this->vertices[v15->vertexNum[(unsigned int)v14 >> 31]];
          v17 = &this->vertices[v15->vertexNum[v14 >= 0]];
          v12 = v37;
          v29 = *v17 - *v16;
		  float _v37 = *(v37 - 1);
          v38 = _v37 / (_v37 - *v37);
          v26 = v29 * v38;
          v23 = *v16 + v26;
          _v19 = v23;
          v39 = a6->Distance(v23);
          if ( a2->distance > v39 )
          {
            a2->distance = v39;
            a2->edgeIndex = v14;
            a2->point = _v19;
          }
          v11 = v36;
          if ( a3->distance < v39 )
          {
            a3->distance = v39;
            a3->edgeIndex = v14;
            a3->point = _v19;
          }
		  lastSign = v33[v11];
        }
        ++v11;
        v12++;
      	_v12++;
        v36 = v11;
        v37 = v12;
        if ( v11 >= v34->numEdges )
          break;
      }
    }
    return true;
  }
  return result;
}

float idAASFileLocal::GetFloorDistance(
        int a2,
        const idPlane *a3,
        const idVec3 *a4,
        float a5,
        float a6) const
{
  const aasArea_t *v6; // edx
  double v8; // st7
  double result; // st7
  const aasEdge_t *list; // ebx
  const idVec3 *v11; // esi
  const int *v12; // edi
  unsigned int v13; // eax
  int v14; // edx
  const idVec3 *p_x; // eax
  const idVec3 *v16; // edx
  double v17; // st7
  double v18; // st5
  double v19; // st6
  double v20; // st2
  float v21; // [esp+4h] [ebp-40h]
	idVec3 v22; // v22 v23 v24
  //float v22; // [esp+8h] [ebp-3Ch]
  //float v23; // [esp+Ch] [ebp-38h]
  //float v24; // [esp+10h] [ebp-34h]
  idVec3 v25; // v25 v26 v27
  //float v25; // [esp+14h] [ebp-30h]
  //float v26; // [esp+18h] [ebp-2Ch]
  //float v27; // [esp+1Ch] [ebp-28h]
  idVec3 v28; // v28 v29 v30
  //float v28; // [esp+20h] [ebp-24h]
  //float v29; // [esp+24h] [ebp-20h]
  //float v30; // [esp+28h] [ebp-1Ch]
  idVec3 v31; // v31 v32 v33
  //float v31; // [esp+2Ch] [ebp-18h]
  //float v32; // [esp+30h] [ebp-14h]
  //float v33; // [esp+34h] [ebp-10h]
  idVec3 v34; // v34 v35 v36
  //float v34; // [esp+38h] [ebp-Ch]
  //float v35; // [esp+3Ch] [ebp-8h]
  //float v36; // [esp+40h] [ebp-4h]
  float v37; // [esp+48h] [ebp+4h]
  float v38; // [esp+48h] [ebp+4h]
  float v39; // [esp+48h] [ebp+4h]
  float v40; // [esp+48h] [ebp+4h]
  float v41; // [esp+48h] [ebp+4h]
  float v42; // [esp+48h] [ebp+4h]
  float v43; // [esp+48h] [ebp+4h]
  float v44; // [esp+48h] [ebp+4h]
  float v45; // [esp+48h] [ebp+4h]
  float v46; // [esp+4Ch] [ebp+8h]
  float v47; // [esp+50h] [ebp+Ch]
  int numEdges; // [esp+54h] [ebp+10h]

	idVec3 _v18; // v18 v17 v19

  v6 = &this->areas[a2];
  v37 = a3->Distance(*a4);
  v38 = fabs(v37);
  v8 = v38;
  v39 = this->settings.invGravityDir * a3->Normal();
  v46 = v8 / v39;
  result = v46;
  if ( a5 <= (double)v46 )
  {
    v47 = 1.0e30;
    if ( v6->numEdges > 0 )
    {
      list = this->edges.Ptr();
      v11 = this->vertices.Ptr();
      v12 = &this->edgeIndex[v6->firstEdge];
      numEdges = v6->numEdges;
      do
      {
        v13 = abs(*v12);
        v14 = list[v13].vertexNum[0];
        p_x = &v11[list[v13].vertexNum[1]];
        v16 = &v11[v14];
        v25 = *p_x - *v16;
        _v18 = v25;
        v40 = v25.LengthSqr();
        if ( v40 >= 0.1000000014901161 )
        {
          v22 = *a4 - *v16;
          v21 = v22 * _v18;
          v41 = v21 / v40;
          if ( v41 >= 0.0 )
          {
            v20 = v41;
            if ( v41 > 1.0 )
              v20 = (float)1.0;
          }
          else
          {
            v20 = (float)0.0;
          }
          v28 = _v18 * v20;
          v31 = v22 - v28;
          v42 = v31.LengthSqr();
          if ( v47 > (double)v42 )
          {
            v47 = v31.LengthSqr();
            v34 = v22 - v28;
          }
        }
        ++v12;
        --numEdges;
      }
      while ( numEdges );
      result = v46;
    }
    v43 = a6 * a6;
    if ( v43 <= (double)v47 )
      return v46;
    v44 = this->settings.invGravityDir * v34;
    v45 = fabs(v44);
    if ( v45 >= result )
      return v46;
    else
      return v45;
  }
  return result;
}

void idAASFileLocal::BoundsBestReachableAreaNum(
		idBounds *a2,
		const idVec3 *a3,
		int a4,
		const idPlane *a5,
		idAASFileLocal::bestReachableArea_t *a6) const
{
	int v6; // esi
	bool v7; // sf
	const aasNode_t *v10; // esi
	int v11; // eax
	const aasArea_t *v12; // eax
	int v13; // esi
	float FloorDistance; // [esp+1Ch] [ebp+4h]

	v6 = a4;
	v7 = a4 < 0;
	if ( !a4 )
		return;
	while ( !v7 )
	{
		v10 = &this->nodes[v6];
		v11 = a2->PlaneSide(this->planeList[v10->planeNum], 0.1);
		if ( !v11 )
			goto LABEL_7;
		if ( v11 != 1 )
		{
			idAASFileLocal::BoundsBestReachableAreaNum(a2, a3, v10->children[1], a5, a6);
			LABEL_7:
				  if ( (v10->flags & 1) != 0 )
				  	a5 = &this->planeList[v10->planeNum];
			v6 = v10->children[0];
			goto LABEL_10;
		}
		v6 = v10->children[1];
		LABEL_10:
			v7 = v6 < 0;
		if ( !v6 )
			return;
	}
	v12 = &this->areas[-v6];
	if ( (v12->flags & a6->areaFlags) != 0 && (v12->travelFlags & a6->excludeTravelFlags) == 0 )
	{
		v13 = -v6;
		FloorDistance = idAASFileLocal::GetFloorDistance(v13, a5, a3, a6->v0, a6->v1);
		if ( a6->distance1 - a6->v0 > FloorDistance && a6->distance2 > (double)FloorDistance )
		{
			a6->distance2 = FloorDistance;
			a6->areaNum2 = v13;
		}
	}
}

void idAASFileLocal::PointBestReachableAreaNum(
		const idVec3 *a2,
		idAASFileLocal::bestReachableArea_t *a3) const
{
	const idPlane *list; // ebp
	int v5; // eax
	const aasNode_t *v6; // esi
	const idPlane *p_a; // edx
	const aasArea_t *v8; // edx
	int v9; // ebx
	const idPlane *v10; // [esp+18h] [ebp-4h]
	float v11; // [esp+20h] [ebp+4h]

	list = this->planeList.Ptr();
	v10 = 0;
	v5 = 1;
	while ( 1 )
	{
		v6 = &this->nodes[v5];
		p_a = &list[v6->planeNum];
		v11 = p_a->Distance(*a2);
		if ( v11 <= 0.0 )
		{
			v5 = v6->children[1];
		}
		else
		{
			v5 = v6->children[0];
			if ( (v6->flags & 1) != 0 )
				v10 = &list[v6->planeNum];
		}
		if ( v5 < 0 )
			break;
		if ( !v5 )
			return;
	}
	v8 = &this->areas[-v5];
	if ( (v8->flags & a3->areaFlags) != 0 && (v8->travelFlags & a3->excludeTravelFlags) == 0 )
	{
		v9 = -v5;
		a3->distance1 = idAASFileLocal::GetFloorDistance(-v5, v10, a2, a3->v0, a3->v1);
		a3->areaNum1 = v9;
	}
}


#if 0
int idAASFileLocal::BoundsReachableAreaNum_r(
		idBounds *a2,
		int a3,
		int a4,
		int a5) const
{
	int v5; // eax
	bool v6; // sf
	const aasNode_t *v8; // esi
	int v9; // eax
	int result; // eax
	const aasArea_t *v11; // edi

	v5 = a3;
	v6 = a3 < 0;
	if ( !a3 )
		return 0;
	while ( !v6 )
	{
		v8 = &this->nodes[v5];
		v9 = a2->PlaneSide(this->planeList[v8->planeNum], 0.1);
		if ( v9 == 1 )
		{
			v5 = v8->children[1];
		}
		else
		{
			if ( v9 )
			{
				result = idAASFileLocal::BoundsReachableAreaNum_r(a2, v8->children[1], a4, a5);
				if ( result )
					return result;
			}
			v5 = v8->children[0];
		}
		v6 = v5 < 0;
		if ( !v5 )
			return 0;
	}
	v11 = &this->areas[-v5];
	if ( (v11->flags & (unsigned __int16)a4) != 0 && (v11->travelFlags & (unsigned __int16)a5) == 0 )
		return -v5;
	else
		return 0;
}
#endif

#if 1
bool idAASFileLocal::Trace(
        aasTrace_t *a2,
        const idVec3 *a3,
        const idVec3 *a4) const
{
  const idVec3 *v4; // edx
  const idVec3 *v5; // ecx
	aasTraceStack_t * v6;
  //float *v6; // esi
  double z; // st7
  int v8; // ebx
  const aasArea_t *v9; // eax
  int numAreas; // eax
  int v11; // ebx
  bool v12; // cc
  int *areas; // ecx
  idVec3 *points; // ecx
  idVec3 p_x; // eax
  int v16; // ebx
  const aasNode_t *v17; // ebx
  const idPlane *v18; // eax
  double v19; // st6
  double v20; // st5
  double v21; // st7
  double v22; // st4
  double v23; // st3
  double v24; // st2
  bool v25; // cf
  double v26; // st1
  double v27; // st2
  double v28; // st2
  double v29; // st1
  aasTraceStack_t *v30; // esi
  int v31; // eax
  bool result; // al
  double v33; // st7
  double v34; // st6
  double v35; // st5
  double v36; // st7
  int v37; // esi
  float v38; // [esp+Ch] [ebp-108Ch]
  float v39; // [esp+Ch] [ebp-108Ch]
  float v40; // [esp+Ch] [ebp-108Ch]
  float v41; // [esp+Ch] [ebp-108Ch]
  float v42; // [esp+Ch] [ebp-108Ch]
  float v43; // [esp+Ch] [ebp-108Ch]
  float v44; // [esp+Ch] [ebp-108Ch]
  float v45; // [esp+Ch] [ebp-108Ch]
  float v46; // [esp+Ch] [ebp-108Ch]
  float v47; // [esp+Ch] [ebp-108Ch]
  float v48; // [esp+10h] [ebp-1088h]
  float v49; // [esp+10h] [ebp-1088h]
  float v50; // [esp+10h] [ebp-1088h]
  float v51; // [esp+10h] [ebp-1088h]
  int v52; // [esp+10h] [ebp-1088h]
  float v53; // [esp+10h] [ebp-1088h]
  idVec3 v54; // v54 v57 v60
  //float v54; // [esp+14h] [ebp-1084h]
  idVec3 v55; // v55 v58 v61
  //float v55; // [esp+14h] [ebp-1084h]
  idVec3 v56; // v56 v59 v62
  //float v56; // [esp+14h] [ebp-1084h]
  //float v57; // [esp+18h] [ebp-1080h]
  //float v58; // [esp+18h] [ebp-1080h]
  //float v59; // [esp+18h] [ebp-1080h]
  //float v60; // [esp+1Ch] [ebp-107Ch]
  //float v61; // [esp+1Ch] [ebp-107Ch]
  //float v62; // [esp+1Ch] [ebp-107Ch]
  idVec3 x; // [esp+20h] [ebp-1078h] // x y 67
  //float x; // [esp+20h] [ebp-1078h]
  idVec3 v64; // [esp+20h] [ebp-1078h] // 64 66 68
  //float v64; // [esp+20h] [ebp-1078h]
  //float y; // [esp+24h] [ebp-1074h]
  //float v66; // [esp+24h] [ebp-1074h]
  //float v67; // [esp+28h] [ebp-1070h]
  //float v68; // [esp+28h] [ebp-1070h]
  float v70; // [esp+34h] [ebp-1064h]
  float v71; // [esp+38h] [ebp-1060h]
  idVec3 v72; // v72 v73 v74
  //float v72; // [esp+54h] [ebp-1044h]
  //float v73; // [esp+58h] [ebp-1040h]
  //float v74; // [esp+5Ch] [ebp-103Ch]
  idVec3 v75; // v75 v76 v77
  //float v75; // [esp+60h] [ebp-1038h]
  //float v76; // [esp+64h] [ebp-1034h]
  //float v77; // [esp+68h] [ebp-1030h]
  idVec3 v78; // v78 v79 v80
  //float v78; // [esp+6Ch] [ebp-102Ch]
  //float v79; // [esp+70h] [ebp-1028h]
  //float v80; // [esp+74h] [ebp-1024h]
  double v81; // [esp+78h] [ebp-1020h]
  double v82; // [esp+80h] [ebp-1018h]
  idVec3 v83; // v83 v84 v85
  //float v83; // [esp+8Ch] [ebp-100Ch]
  //float v84; // [esp+90h] [ebp-1008h]
  //float v85; // [esp+94h] [ebp-1004h]
  aasTraceStack_t v86[MAX_AAS_TREE_DEPTH]; // [esp+98h] [ebp-1000h] BYREF
  //float v86[1024]; // [esp+98h] [ebp-1000h] BYREF
  aasTraceStack_t *vars0 = &v86[MAX_AAS_TREE_DEPTH]; // [esp+1098h] [ebp+0h] BYREF

	v6 = &v86[0];
	idVec3 _v23; // v23 v70 v71
	idVec3 _v19; // v19 v20 v21

  v4 = a3;
  a2->numAreas = 0;
  a2->lastAreaNum = 0;
  a2->blockingAreaNum = 0;
	v6->start = *a3;
	v6->end = *a4;
	v6->planeNum = 0;
	v6->nodeNum = 1;
  v5 = a4;
  v6 = v86;
  while ( 1 )
  {
    v8 = v6->nodeNum;
    if ( v8 >= 0 )
    {
      if ( v8 )
      {
        v17 = &this->nodes[v8];
        v18 = &this->planeList[v17->planeNum];
        _v19 = v6->start;
        v40 = v18->Distance(_v19);
        v22 = v40;
        _v23 = v6->end;
        v41 = v18->Distance(v6->end);
        v24 = v41;
        if ( v22 < -ON_EPSILON || v24 < -ON_EPSILON )
        {
          if ( v22 >= ON_EPSILON || v24 >= ON_EPSILON )
          {
            v52 = v6->planeNum;
            if ( v22 >= 0.0 )
              v26 = v22 - TRACEPLANE_EPSILON;
            else
              v26 = v22 + TRACEPLANE_EPSILON;
            v27 = v26 / (v22 - v24);
            if ( v27 >= 0.0 )
            {
              if ( v27 <= 1.0 )
              {
                v29 = v27;
                v28 = 0.0;
              }
              else
              {
                v28 = 0.0;
                v29 = 0.999f;
              }
            }
            else
            {
              v28 = 0.0;
              v29 = 0.001f;
            }
            v72 = _v23 - _v19;
            v42 = v29;
            v78 = v72 * v42;
            v54 = v78 + _v19;
            v6->planeNum = v17->planeNum;
            v6->start = v54;
          	v30 = v6 + 1;
            v6->nodeNum = v17->children[v28 <= v22];
            if ( v30 >= vars0 )
            {
LABEL_50:
			  common->Warning("idAASFileLocal::Trace: stack overflow");
              return false;
            }
            v31 = v17->children[v28 > v22];
          	v6->start = _v19;
          	v6->end = v54;
            v6->planeNum = v52;
            v6->nodeNum = v31;
            v6 = v6 + 1;
            v25 = v6 < vars0;
          }
          else
          {
            v6->nodeNum = v17->children[1];
            v6 = v6 + 1;
            v25 = v6 < vars0;
          }
          if ( !v25 )
            goto LABEL_50;
        }
        else
        {
          v6->nodeNum = v17->children[0];
          v6 = v6 + 1;
          if ( v6 >= vars0 )
            goto LABEL_50;
        }
      }
      else
      {
        if ( a2->lastAreaNum )
        {
          v75 = *v5 - *v4;
          x = v75;
          v83 = v6->start - *v4;
          v49 = v83.Length();
          v38 = v49;
          v51 = v75.Length();
          v4 = a3;
          a2->fraction = v38 / v51;
        }
        else
        {
          a2->fraction = 0.0;
          x = vec3_origin;
        }
        v16 = v6->planeNum;
        a2->endpos = v6->start;
        a2->blockingAreaNum = 0;
        a2->planeNum = v16;
        v39 = x * this->planeList[v16].Normal();
        if ( v39 > 0.0 )
          a2->planeNum = v16 ^ 1;
        if ( a2->lastAreaNum || !a2->getOutOfSolid )
          return true;
      }
    }
    else
    {
      v9 = &this->areas[-v8];
      if ( (v9->flags & a2->flags) != 0 || (v9->travelFlags & a2->travelFlags) != 0 )
      {
        if ( a2->lastAreaNum )
        {
          v55 = *a4 - *v4;
        	v64 = v55;
          v56 = v6->start - *v4;
          v44 = v6->start.Length();
          v53 = v44;
          v46 = v55.Length();
          a2->fraction = v53 / v46;
        }
        else
        {
          a2->fraction = 0.0;
          v64 = vec3_origin;
        }
        a2->endpos = v6->start;
        v37 = v6->planeNum;
        a2->blockingAreaNum = -v8;
        a2->planeNum = v37;
        v47 = v64 * this->planeList[v37].Normal();
        if ( v47 > 0.0 )
          a2->planeNum = v37 ^ 1;
        return true;
      }
      numAreas = a2->numAreas;
      v11 = -v8;
      v12 = numAreas < a2->maxAreas;
      a2->lastAreaNum = v11;
      if ( v12 )
      {
        areas = a2->areas;
        if ( areas )
          areas[numAreas] = v11;
        points = a2->points;
        if ( points )
        {
          p_x = v6->start;
			points[numAreas] = p_x;
        }
        ++a2->numAreas;
      }
    }
    v6 = v6 - 1;
    if ( v6 < v86 )
      break;
    v5 = a4;
  }
  if ( a2->lastAreaNum )
  {
    a2->fraction = 1.0;
    a2->endpos = *a4;
    result = false;
  }
  else
  {
    result = false;
    a2->fraction = 0.0;
    a2->endpos = *v4;
  }
  a2->planeNum = 0;
  return result;
}
#endif

#endif
