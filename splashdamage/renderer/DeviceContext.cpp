// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "DeviceContext_local.h"

const int VIRTUAL_WIDTH = 640;
const int VIRTUAL_HEIGHT = 480;

sdDeviceContextLocal::sdDeviceContextLocal()
: whiteImage(NULL)
{
	xScale = 0.0;
	SetSize(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
	enableClipping = true;
	clipRects.Clear();
}

void sdDeviceContextLocal::Reset() {
	whiteImage = declManager->FindMaterial("guis/assets/white");
	whiteImage->SetSort(SS_GUI);
}

void sdDeviceContextLocal::BeginEmitToCurrentView( const float modelMatrix[16], const int allowInViewID, const bool weaponDepthHack ) {
}

void sdDeviceContextLocal::BeginEmitFullScreen() {
}

void sdDeviceContextLocal::End() {
}

void sdDeviceContextLocal::SetColor( const idVec4& color ) {
    renderSystem->SetColor(color);
}

void sdDeviceContextLocal::SetColor( const float r, const float g, const float b, const float a ) {
    renderSystem->SetColor4(r, g, b, a);
}

idVec4 sdDeviceContextLocal::SetColorMultiplier( const idVec4& c ) {
    return vec4_one;
}

void sdDeviceContextLocal::SetRegister( const int index, const float value ) {
}

void sdDeviceContextLocal::SetRegisters( const float* values ) {
}

void sdDeviceContextLocal::EnableClipping( bool enable ) {
    enableClipping = enable;
}

void sdDeviceContextLocal::PushClipRect( const sdBounds2D& bounds ) {
	clipRects.Append(bounds);
}

void sdDeviceContextLocal::PopClipRect() {
    if (clipRects.Num()) {
        clipRects.RemoveIndex(clipRects.Num()-1);
    }
}

void sdDeviceContextLocal::DrawRect( float x, float y, float w, float h, float s1, float t1, float s2, float t2, const idMaterial* material, float angle ) {

	AdjustCoords(&x, &y, &w, &h);

	DrawStretchPicRotated(x, y, w, h, s1, t1, s2, t2, material, angle);
}

void sdDeviceContextLocal::DrawClippedRect( float x, float y, float w, float h, float s1, float t1, float s2, float t2, const idMaterial* material, float angle ) {

	if (ClippedCoords(&x, &y, &w, &h, &s1, &t1, &s2, &t2)) {
		return;
	}

	DrawRect(x, y, w, h, s1, t1, s2, t2, material, angle);
}

void sdDeviceContextLocal::DrawMaskedClippedRect( float x, float y, float w, float h, float s01, float t01, float s02, float t02, float s11, float t11, float s12, float t12, const idMaterial* material, float angle ) {
	assert(0); // unused
}

void sdDeviceContextLocal::DrawCinematic( float x, float y, float w, float h, float s1, float t1, float s2, float t2, const idMaterial* material, idSoundEmitter* referenceSound, float angle ) {
}

void sdDeviceContextLocal::DrawClippedWinding( const idWinding2D& winding, const idMaterial* material ) {
}

void sdDeviceContextLocal::DrawClippedWindingMasked( const idWinding2D& winding, const idMaterial* material, float minx, float miny, float width, float height ) {
}

void sdDeviceContextLocal::DrawMaskedMaterial( float x, float y, float w, float h, float u0, float v0, float u1, float v1, const idMaterial* material, const idVec4 &color, float scaleX, float scaleY, float offsetX, float offsetY, float angle ) {
}

void sdDeviceContextLocal::DrawMaterial( float x, float y, float w, float h, const idMaterial* material, const idVec4 &color, float scaleX, float scaleY, float offsetX, float offsetY, float angle ) {

	if (color.w == 0.0f) {
		return;
	}

	float nw = w * scaleX;
	float nh = h * scaleY;

	float cx = x - (nw - w) * 0.5f;
	float cy = y - (nh - h) * 0.5f;

	cx += offsetX;
	cy += offsetY;

	DrawRotatedMaterial( angle, idVec2(cx, cy), idVec2(nw, nh), material, color );
}

void sdDeviceContextLocal::DrawMaterial( const idVec4& rect, const idMaterial *material, const idVec4 &color, const idVec2& scale, const idVec2& offset, float angle ) {
	DrawMaterial(rect.x, rect.y, rect.z, rect.w, material, color, scale.x, scale.y, offset.x, offset.y, angle);
}

void sdDeviceContextLocal::DrawMaterial( const sdBounds2D& rect, const idMaterial *material, const idVec4 &color, const idVec2& scale, const idVec2& offset, float angle ) {
	DrawMaterial(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), material, color, scale.x, scale.y, offset.x, offset.y, angle);
}

void sdDeviceContextLocal::DrawMaterial( float x, float y, float w, float h, const idMaterial* material, const idVec4 &color, const idVec2& st0, const idVec2& st1 ) {
	DrawStretchPic(x, y, w, h, st0.x, st0.y, st1.x, st1.y, material);
}

void sdDeviceContextLocal::DrawRotatedMaterial( float angle, idVec2 topLeft, idVec2 extents, const idMaterial* material, const idVec4& color ) {

	if (color.w == 0.0f) {
		return;
	}

	renderSystem->SetColor(color);

	float	s0, s1, t0, t1;
	float scalex = 1.0f;
	float scaley = 1.0f;
	float x = topLeft.x;
	float y = topLeft.y;
	float w = extents.x;
	float h = extents.y;

	//
	//  handle negative scales as well
	if (scalex < 0) {
		w *= -1;
		scalex *= -1;
	}

	if (scaley < 0) {
		h *= -1;
		scaley *= -1;
	}

	//
	if (w < 0) {	// flip about vertical
		w  = -w;
		s0 = 1 * scalex;
		s1 = 0;
	} else {
		s0 = 0;
		s1 = 1 * scalex;
	}

	if (h < 0) {	// flip about horizontal
		h  = -h;
		t0 = 1 * scaley;
		t1 = 0;
	} else {
		t0 = 0;
		t1 = 1 * scaley;
	}

	if (angle == 0.0f && ClippedCoords(&x, &y, &w, &h, &s0, &t0, &s1, &t1)) {
		return;
	}

	AdjustCoords(&x, &y, &w, &h);

	DrawStretchPicRotated(x, y, w, h, s0, t0, s1, t1, material, angle);
}

void sdDeviceContextLocal::DrawWindingMaterial( const idWinding2D& winding, const idMaterial* material, const idVec4& color ) {
}

void sdDeviceContextLocal::DrawRect( float x, float y, float w, float h, const idVec4 &color ) {

    if (color.w == 0.0f) {
        return;
    }

	renderSystem->SetColor(color);

	AdjustCoords(&x, &y, &w, &h);

	DrawStretchPic(x, y, w, h, 0, 0, 0, 0, whiteImage);
}

void sdDeviceContextLocal::DrawClippedRect( float x, float y, float w, float h, const idVec4 &color ) {

    if (color.w == 0.0f) {
        return;
    }

	if (ClippedCoords(&x, &y, &w, &h, NULL, NULL, NULL, NULL)) {
		return;
	}

	DrawRect(x, y, w, h, color);
}

void sdDeviceContextLocal::DrawBox( float x, float y, float w, float h, float size, const idVec4 &color ) {

    if (color.w == 0.0f) {
        return;
    }

    renderSystem->SetColor(color);

    AdjustCoords(&x, &y, &w, &h);
    DrawStretchPic(x, y, size, h, 0, 0, 0, 0, whiteImage);
    DrawStretchPic(x + w - size, y, size, h, 0, 0, 0, 0, whiteImage);
    DrawStretchPic(x, y, w, size, 0, 0, 0, 0, whiteImage);
    DrawStretchPic(x, y + h - size, w, size, 0, 0, 0, 0, whiteImage);
}

void sdDeviceContextLocal::DrawClippedBox( float x, float y, float w, float h, float size, const idVec4 &color ) {

    if (color.w == 0.0f) {
        return;
    }

    if (ClippedCoords(&x, &y, &w, &h, NULL, NULL, NULL, NULL)) {
        return;
    }

	DrawBox(x, y, w, h, size, color);
}

void sdDeviceContextLocal::DrawCircleMaterial( const float x, const float y, const idVec2& radius, const int numSides, const idVec4& tcInfo, const idMaterial* material, const idVec4& color, float rotation ) {
}

void sdDeviceContextLocal::DrawCircleMaterialMasked( const float x, const float y, const idVec2& radius, const int numSides, const idVec4& tcInfo, const idMaterial* material, const idVec4& color, float rotation, float s11, float t11, float s12, float t12 ) {
}

void sdDeviceContextLocal::DrawCircle( const float x, const float y, const idVec2& radius, const float width, const int numSides, const idVec4& color ) {
}

void sdDeviceContextLocal::DrawLineMaterial( const idVec2& start, const idVec2& end, const float width, const idMaterial* material, const idVec4& color ) {
}

void sdDeviceContextLocal::DrawLine( const idVec2& start, const idVec2& end, const float width, const idVec4 &color ) {
}

void sdDeviceContextLocal::DrawFilledArc( const float x, const float y, const float radius, int numSides, float percent, const idVec4 &color, float startAngle, const idMaterial *material ) {
}

void sdDeviceContextLocal::DrawFilledArcMasked( const float x, const float y, const float radius, int numSides, float percent, const idVec4 &color, float s11, float t11, float s12, float t12, float startAngle, const idMaterial *material ) {
}

void sdDeviceContextLocal::DrawArc( const float x, const float y, const float radius, const float width, const int numSides, const float percent, const idVec4 &color, const float startAngle ) {
}

void sdDeviceContextLocal::DrawTimer( const float x, const float y, const float w, const float h, float percent, const idVec4 &color, const idMaterial* material, bool invert, const idVec2& st0, const idVec2& st1 ) {
}

qhandle_t sdDeviceContextLocal::FindFont( const char* fontName ) {
    return 0;
}

void sdDeviceContextLocal::FreeFont( const qhandle_t font ) {
}

const int sdDeviceContextLocal::GetFontHeight( const qhandle_t font, const int pointSize ) {
    return pointSize;
}

void sdDeviceContextLocal::SetFont( const qhandle_t font ) {
}

void sdDeviceContextLocal::SetFontSize( const int pointSize ) {
}

void sdDeviceContextLocal::DrawText( const wchar_t* text, const sdBounds2D& rect, unsigned int flags ) {
}

void sdDeviceContextLocal::GetTextDimensions( const wchar_t* text, const sdBounds2D& rect, unsigned int flags, const qhandle_t font, const int pointSize, int& width, int& height, float* scale, int** charAdvances, idList< int >* lineBreaks ) {
}

void sdDeviceContextLocal::OverrideAspectRationCorrection( bool setOverride ) {
}

float sdDeviceContextLocal::GetAspectRatioCorrection() const {
    return 1.0f;
}

bool sdDeviceContextLocal::ClippedCoords(float *x, float *y, float *w, float *h, float *s1, float *t1, float *s2, float *t2)
{
	return false;

    if (enableClipping == false || clipRects.Num() == 0) {
        return false;
    }

    int c = clipRects.Num();

    while (--c > 0) {
        sdBounds2D *clipRect = &clipRects[c];

        float ox = *x;
        float oy = *y;
        float ow = *w;
        float oh = *h;

        if (ow <= 0.0f || oh <= 0.0f) {
            break;
        }

        if (*x < clipRect->GetLeft()) {
            *w -= clipRect->GetLeft() - *x;
            *x = clipRect->GetLeft();
        } else if (*x > clipRect->GetLeft() + clipRect->GetWidth()) {
            *x = *w = *y = *h = 0;
        }

        if (*y < clipRect->GetTop()) {
            *h -= clipRect->GetTop() - *y;
            *y = clipRect->GetTop();
        } else if (*y > clipRect->GetTop() + clipRect->GetHeight()) {
            *x = *w = *y = *h = 0;
        }

        if (*w > clipRect->GetWidth()) {
            *w = clipRect->GetWidth() - *x + clipRect->GetLeft();
        } else if (*x + *w > clipRect->GetLeft() + clipRect->GetWidth()) {
            *w = clipRect->GetRight() - *x;
        }

        if (*h > clipRect->GetHeight()) {
            *h = clipRect->GetHeight() - *y + clipRect->GetTop();
        } else if (*y + *h > clipRect->GetTop() + clipRect->GetHeight()) {
            *h = clipRect->GetBottom() - *y;
        }

        if (s1 && s2 && t1 && t2 && ow > 0.0f) {
            float ns1, ns2, nt1, nt2;
            // upper left
            float u = (*x - ox) / ow;
            ns1 = *s1 * (1.0f - u) + *s2 * (u);

            // upper right
            u = (*x + *w - ox) / ow;
            ns2 = *s1 * (1.0f - u) + *s2 * (u);

            // lower left
            u = (*y - oy) / oh;
            nt1 = *t1 * (1.0f - u) + *t2 * (u);

            // lower right
            u = (*y + *h - oy) / oh;
            nt2 = *t1 * (1.0f - u) + *t2 * (u);

            // set values
            *s1 = ns1;
            *s2 = ns2;
            *t1 = nt1;
            *t2 = nt2;
        }
    }

    return (*w == 0 || *h == 0) ? true : false;
}

void sdDeviceContextLocal::DrawStretchPic(float x, float y, float w, float h, float s1, float t1, float s2, float t2, const idMaterial *shader)
{
	idDrawVert verts[4];
	glIndex_t indexes[6];
	indexes[0] = 3;
	indexes[1] = 0;
	indexes[2] = 2;
	indexes[3] = 2;
	indexes[4] = 0;
	indexes[5] = 1;
	verts[0].xyz[0] = x;
	verts[0].xyz[1] = y;
	verts[0].xyz[2] = 0;
	verts[0].st[0] = s1;
	verts[0].st[1] = t1;
	verts[0].normal[0] = 0;
	verts[0].normal[1] = 0;
	verts[0].normal[2] = 1;
	verts[0].tangents[0][0] = 1;
	verts[0].tangents[0][1] = 0;
	verts[0].tangents[0][2] = 0;
	verts[0].tangents[1][0] = 0;
	verts[0].tangents[1][1] = 1;
	verts[0].tangents[1][2] = 0;
	verts[1].xyz[0] = x + w;
	verts[1].xyz[1] = y;
	verts[1].xyz[2] = 0;
	verts[1].st[0] = s2;
	verts[1].st[1] = t1;
	verts[1].normal[0] = 0;
	verts[1].normal[1] = 0;
	verts[1].normal[2] = 1;
	verts[1].tangents[0][0] = 1;
	verts[1].tangents[0][1] = 0;
	verts[1].tangents[0][2] = 0;
	verts[1].tangents[1][0] = 0;
	verts[1].tangents[1][1] = 1;
	verts[1].tangents[1][2] = 0;
	verts[2].xyz[0] = x + w;
	verts[2].xyz[1] = y + h;
	verts[2].xyz[2] = 0;
	verts[2].st[0] = s2;
	verts[2].st[1] = t2;
	verts[2].normal[0] = 0;
	verts[2].normal[1] = 0;
	verts[2].normal[2] = 1;
	verts[2].tangents[0][0] = 1;
	verts[2].tangents[0][1] = 0;
	verts[2].tangents[0][2] = 0;
	verts[2].tangents[1][0] = 0;
	verts[2].tangents[1][1] = 1;
	verts[2].tangents[1][2] = 0;
	verts[3].xyz[0] = x;
	verts[3].xyz[1] = y + h;
	verts[3].xyz[2] = 0;
	verts[3].st[0] = s1;
	verts[3].st[1] = t2;
	verts[3].normal[0] = 0;
	verts[3].normal[1] = 0;
	verts[3].normal[2] = 1;
	verts[3].tangents[0][0] = 1;
	verts[3].tangents[0][1] = 0;
	verts[3].tangents[0][2] = 0;
	verts[3].tangents[1][0] = 0;
	verts[3].tangents[1][1] = 1;
	verts[3].tangents[1][2] = 0;

	renderSystem->DrawStretchPic(&verts[0], &indexes[0], 4, 6, shader, false);

}

void sdDeviceContextLocal::SetSize(float width, float height)
{
	vidWidth = VIRTUAL_WIDTH;
	vidHeight = VIRTUAL_HEIGHT;
	xScale = yScale = 0.0f;

	if (width != 0.0f && height != 0.0f) {
		xScale = vidWidth * (1.0f / width);
		yScale = vidHeight * (1.0f / height);
	}
}

void sdDeviceContextLocal::AdjustCoords(float *x, float *y, float *w, float *h)
{
	// if (x) {
	// 	*x *= xScale;
	// }
	//
	// if (y) {
	// 	*y *= yScale;
	// }
	//
	// if (w) {
	// 	*w *= xScale;
	// }
	//
	// if (h) {
	// 	*h *= yScale;
	// }
}

void sdDeviceContextLocal::DrawStretchPicRotated(float x, float y, float w, float h, float s1, float t1, float s2, float t2, const idMaterial *shader, float angle)
{

	idDrawVert verts[4];
	glIndex_t indexes[6];
	indexes[0] = 3;
	indexes[1] = 0;
	indexes[2] = 2;
	indexes[3] = 2;
	indexes[4] = 0;
	indexes[5] = 1;
	verts[0].xyz[0] = x;
	verts[0].xyz[1] = y;
	verts[0].xyz[2] = 0;
	verts[0].st[0] = s1;
	verts[0].st[1] = t1;
	verts[0].normal[0] = 0;
	verts[0].normal[1] = 0;
	verts[0].normal[2] = 1;
	verts[0].tangents[0][0] = 1;
	verts[0].tangents[0][1] = 0;
	verts[0].tangents[0][2] = 0;
	verts[0].tangents[1][0] = 0;
	verts[0].tangents[1][1] = 1;
	verts[0].tangents[1][2] = 0;
	verts[1].xyz[0] = x + w;
	verts[1].xyz[1] = y;
	verts[1].xyz[2] = 0;
	verts[1].st[0] = s2;
	verts[1].st[1] = t1;
	verts[1].normal[0] = 0;
	verts[1].normal[1] = 0;
	verts[1].normal[2] = 1;
	verts[1].tangents[0][0] = 1;
	verts[1].tangents[0][1] = 0;
	verts[1].tangents[0][2] = 0;
	verts[1].tangents[1][0] = 0;
	verts[1].tangents[1][1] = 1;
	verts[1].tangents[1][2] = 0;
	verts[2].xyz[0] = x + w;
	verts[2].xyz[1] = y + h;
	verts[2].xyz[2] = 0;
	verts[2].st[0] = s2;
	verts[2].st[1] = t2;
	verts[2].normal[0] = 0;
	verts[2].normal[1] = 0;
	verts[2].normal[2] = 1;
	verts[2].tangents[0][0] = 1;
	verts[2].tangents[0][1] = 0;
	verts[2].tangents[0][2] = 0;
	verts[2].tangents[1][0] = 0;
	verts[2].tangents[1][1] = 1;
	verts[2].tangents[1][2] = 0;
	verts[3].xyz[0] = x;
	verts[3].xyz[1] = y + h;
	verts[3].xyz[2] = 0;
	verts[3].st[0] = s1;
	verts[3].st[1] = t2;
	verts[3].normal[0] = 0;
	verts[3].normal[1] = 0;
	verts[3].normal[2] = 1;
	verts[3].tangents[0][0] = 1;
	verts[3].tangents[0][1] = 0;
	verts[3].tangents[0][2] = 0;
	verts[3].tangents[1][0] = 0;
	verts[3].tangents[1][1] = 1;
	verts[3].tangents[1][2] = 0;

	//Generate a translation so we can translate to the center of the image rotate and draw
	idVec3 origTrans;
	origTrans.x = x+(w/2);
	origTrans.y = y+(h/2);
	origTrans.z = 0;


	//Rotate the verts about the z axis before drawing them
	idMat4 rotz;
	rotz.Identity();
	float sinAng = idMath::Sin(angle);
	float cosAng = idMath::Cos(angle);
	rotz[0][0] = cosAng;
	rotz[0][1] = sinAng;
	rotz[1][0] = -sinAng;
	rotz[1][1] = cosAng;

	for (int i = 0; i < 4; i++) {
		//Translate to origin
		verts[i].xyz -= origTrans;

		//Rotate
		verts[i].xyz = rotz * verts[i].xyz;

		//Translate back
		verts[i].xyz += origTrans;
	}


	renderSystem->DrawStretchPic(&verts[0], &indexes[0], 4, 6, shader, (angle == 0.0) ? false : true);
}



static sdDeviceContextLocal deviceContextLocal;

sdDeviceContext* deviceContext = &deviceContextLocal;
