// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "DeviceContext_local.h"
#include "renderer/tr_local.h"

#if 0
#define DC_PLACEHOLDER(...) Sys_Printf(__VA_ARGS__)
#define DC_DRAW(...) Sys_Printf(__VA_ARGS__)
#else
#define DC_PLACEHOLDER(...)
#define DC_DRAW(...)
#endif
#define DC_UNUSED_ON_GAME

#define DEFAULT_FONT_TEXTURE_SIZE 1024

#define AsASCIICharLang(text_, len_) ( !_hasWideCharFont || idStr::IsPureASCII(text_, len_) )

extern idCVar harm_gui_useD3BFGFont;
extern idCVar gui_smallFontLimit;
extern idCVar gui_mediumFontLimit;
static bool _hasWideCharFont = false;

extern bool R_ExportTrueTypeFont(const char *fontPath, const char *fontType, const char *language, int width);

const int VIRTUAL_WIDTH = 640;
const int VIRTUAL_HEIGHT = 480;

idList<fontInfoEx_t> sdDeviceContextLocal::fonts;
idList<sdLocFont_t> sdDeviceContextLocal::fontConfigs;

sdDeviceContextLocal::sdDeviceContextLocal()
: whiteImage(NULL)
{
	xScale = 0.0;
	SetSize(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
	enableClipping = false;
	overStrikeMode = true;
	clipRects.Clear();
	tempColor = vec4_one;
	usingTempColor = false;
}

void sdDeviceContextLocal::Reset() {
	whiteImage = declManager->FindMaterial("guis/assets/white");
	whiteImage->SetSort(SS_GUI);
	SetupFonts();
}

void sdDeviceContextLocal::BeginEmitToCurrentView( const float modelMatrix[16], const int allowInViewID, const bool weaponDepthHack ) {
	tr.guiModel->BeginEmitToCurrentView(modelMatrix, allowInViewID, weaponDepthHack);
}

void sdDeviceContextLocal::BeginEmitFullScreen() {
	tr.guiModel->BeginEmitFullScreen();
}

void sdDeviceContextLocal::End() {
	tr.guiModel->End();
	tr.guiModel->SetRegisters(NULL);
	renderSystem->SetColor4(0.0f, 0.0f, 0.0f, 1.0f);
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
	tr.guiModel->SetRegister(index, value);
}

void sdDeviceContextLocal::SetRegisters( const float* values ) {
	tr.guiModel->SetRegisters(values);
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
	DC_DRAW("DCDraw:DrawRect|%s\n", material?material->GetName():NULL);

	if(!material)
		return;

	AdjustCoords(&x, &y, &w, &h);

	DrawStretchPicRotated(x, y, w, h, s1, t1, s2, t2, material, angle);
}

void sdDeviceContextLocal::DrawClippedRect( float x, float y, float w, float h, float s1, float t1, float s2, float t2, const idMaterial* material, float angle ) {
	DC_DRAW("DCDraw:DrawClippedRect|%s\n", material?material->GetName():NULL);

	if(!material)
		return;

	if (ClippedCoords(&x, &y, &w, &h, &s1, &t1, &s2, &t2)) {
		return;
	}

	DrawRect(x, y, w, h, s1, t1, s2, t2, material, angle);
}

void sdDeviceContextLocal::DrawMaskedClippedRect( float x, float y, float w, float h, float s01, float t01, float s02, float t02, float s11, float t11, float s12, float t12, const idMaterial* material, float angle ) {
	DC_UNUSED_ON_GAME
	DC_PLACEHOLDER("DC:DrawMaskedClippedRect|%s\n", material?material->GetName():NULL);

	if(!material)
		return;

	//printf("rrr|%f %f %f %f\n",s1,t1,s2,t2);
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
	verts[0].st[0] = s01;
	verts[0].st[1] = t01;
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
	verts[1].st[0] = s02;
	verts[1].st[1] = t02;
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
	verts[2].st[0] = s12;
	verts[2].st[1] = t12;
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
	verts[3].st[0] = s11;
	verts[3].st[1] = t11;
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

	renderSystem->DrawStretchPic(&verts[0], &indexes[0], 4, 6, material, (angle == 0.0) ? false : true);

	idVec4 c = tr.guiModel->CurrentColor();
	renderSystem->SetColor(colorRed);
	idStr str = "";
	str.Append(material->GetName());
	idWStr wstr = StrToWStr(str);
	sdBounds2D bb = sdBounds2D(x,y,w,h);
	bb.GetRight() = 640;
	DrawText(wstr.c_str(), bb, 0);

	renderSystem->SetColor(c);
}

void sdDeviceContextLocal::DrawCinematic( float x, float y, float w, float h, float s1, float t1, float s2, float t2, const idMaterial* material, idSoundEmitter* referenceSound, float angle ) {
	DC_PLACEHOLDER("DC:DrawCinematic|%s\n", material?material->GetName():NULL);

	if(!material)
		return;
}

void sdDeviceContextLocal::DrawClippedWinding( const idWinding2D& winding, const idMaterial* material ) {
	DC_PLACEHOLDER("DC:DrawClippedWinding|%s\n", material?material->GetName():NULL);

	if(!material)
		return;
}

void sdDeviceContextLocal::DrawClippedWindingMasked( const idWinding2D& winding, const idMaterial* material, float minx, float miny, float width, float height ) {
	DC_UNUSED_ON_GAME
	DC_PLACEHOLDER("DC:DrawClippedWindingMasked|%s\n", material?material->GetName():NULL);

	if(!material)
		return;
}

void sdDeviceContextLocal::DrawMaskedMaterial( float x, float y, float w, float h, float u0, float v0, float u1, float v1, const idMaterial* material, const idVec4 &color, float scaleX, float scaleY, float offsetX, float offsetY, float angle ) {
	DC_PLACEHOLDER("DC:DrawMaskedMaterial|%s\n", material?material->GetName():NULL);

	if(!material)
		return;

	if (color.w == 0.0f) {
		return;
	}

	//
	//  handle negative scales as well
	if (scaleX < 0) {
		w *= -1;
		scaleX *= -1;
	}

	if (scaleY < 0) {
		h *= -1;
		scaleY *= -1;
	}

	//
	if (w < 0) {	// flip about vertical
		w  = -w;
		idSwap(u0, u1);
		u0 = u0 * scaleX;
		u1 = u1 * scaleX;
	} else {
		u0 = u0 * scaleX;
		u1 = u1 * scaleX;
	}

	if (h < 0) {	// flip about horizontal
		h  = -h;
		idSwap(v0, v1);
		v0 = v0 * scaleY;
		v1 = v1 * scaleY;
	} else {
		v0 = v0 * scaleY;
		v1 = v1 * scaleY;
	}

	if (angle == 0.0f && ClippedCoords(&x, &y, &w, &h, &u0, &v0, &u1, &v1)) {
		return;
	}

	SetTempColor(color);

	AdjustCoords(&x, &y, &w, &h);

	DrawStretchPicRotated(x, y, w, h, u0, v0, u1, v1, material, angle);

	UnsetTempColor();
}

void sdDeviceContextLocal::DrawMaterial( float x, float y, float w, float h, const idMaterial* material, const idVec4 &color, float scaleX, float scaleY, float offsetX, float offsetY, float angle ) {
	DC_DRAW("DCDraw:DrawMaterial|%s\n", material?material->GetName():NULL);

	if(!material)
		return;

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
	DC_DRAW("DCDraw:DrawMaterial2|%s\n", material?material->GetName():NULL);
	DrawMaterial(rect.x, rect.y, rect.z, rect.w, material, color, scale.x, scale.y, offset.x, offset.y, angle);
}

void sdDeviceContextLocal::DrawMaterial( const sdBounds2D& rect, const idMaterial *material, const idVec4 &color, const idVec2& scale, const idVec2& offset, float angle ) {
	DC_DRAW("DCDraw:DrawMaterial3|%s\n", material?material->GetName():NULL);
	DrawMaterial(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), material, color, scale.x, scale.y, offset.x, offset.y, angle);
}

void sdDeviceContextLocal::DrawMaterial( float x, float y, float w, float h, const idMaterial* material, const idVec4 &color, const idVec2& st0, const idVec2& st1 ) {
	DC_DRAW("DCDraw:DrawMaterial5|%s\n", material?material->GetName():NULL);
	//printf("dddmmm|%f %f %f %f\n",st0.x, st0.y, st1.x, st1.y);

	if(!material || !material->GetStage(0) || !material->GetStage(0)->texture.image)
		return;

	DrawStretchPic(x, y, w, h, 
			st0.x / (float)material->GetImageWidth(), 
			st0.y / (float)material->GetImageHeight(), 
			(st0.x + st1.x) / (float)material->GetImageWidth(), 
			(st0.y + st1.y) / (float)material->GetImageHeight(), 
			material);
}

void sdDeviceContextLocal::DrawRotatedMaterial( float angle, idVec2 topLeft, idVec2 extents, const idMaterial* material, const idVec4& color ) {
	DC_UNUSED_ON_GAME
	DC_DRAW("DCDraw:DrawRotatedMaterial|%s\n", material?material->GetName():NULL);

	if(!material)
		return;

	if (color.w == 0.0f) {
		return;
	}

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

	SetTempColor(color);

	AdjustCoords(&x, &y, &w, &h);

	DrawStretchPicRotated(x, y, w, h, s0, t0, s1, t1, material, angle);

	UnsetTempColor();
}

void sdDeviceContextLocal::DrawWindingMaterial( const idWinding2D& winding, const idMaterial* material, const idVec4& color ) {
	DC_PLACEHOLDER("DC:DrawWindingMaterial|%s\n", material?material->GetName():NULL);

	if(!material)
		return;
}

void sdDeviceContextLocal::DrawRect( float x, float y, float w, float h, const idVec4 &color ) {
	DC_DRAW("DCDraw:DrawRect\n");

    if (color.w == 0.0f) {
        return;
    }

	SetTempColor(color);

	AdjustCoords(&x, &y, &w, &h);

	DrawStretchPic(x, y, w, h, 0, 0, 0, 0, whiteImage);

	UnsetTempColor();
}

void sdDeviceContextLocal::DrawClippedRect( float x, float y, float w, float h, const idVec4 &color ) {
	DC_DRAW("DCDraw:DrawClippedRect\n");

    if (color.w == 0.0f) {
        return;
    }

	if (ClippedCoords(&x, &y, &w, &h, NULL, NULL, NULL, NULL)) {
		return;
	}

	DrawRect(x, y, w, h, color);
}

void sdDeviceContextLocal::DrawBox( float x, float y, float w, float h, float size, const idVec4 &color ) {
	DC_DRAW("DCDraw:DrawBox\n");

    if (color.w == 0.0f) {
        return;
    }

	SetTempColor(color);

    AdjustCoords(&x, &y, &w, &h);
    DrawStretchPic(x, y, size, h, 0, 0, 0, 0, whiteImage);
    DrawStretchPic(x + w - size, y, size, h, 0, 0, 0, 0, whiteImage);
    DrawStretchPic(x, y, w, size, 0, 0, 0, 0, whiteImage);
    DrawStretchPic(x, y + h - size, w, size, 0, 0, 0, 0, whiteImage);

	UnsetTempColor();
}

void sdDeviceContextLocal::DrawClippedBox( float x, float y, float w, float h, float size, const idVec4 &color ) {
	DC_DRAW("DCDraw:DrawClippedBox\n");

    if (color.w == 0.0f) {
        return;
    }

    if (ClippedCoords(&x, &y, &w, &h, NULL, NULL, NULL, NULL)) {
        return;
    }

	DrawBox(x, y, w, h, size, color);
}

void sdDeviceContextLocal::DrawCircleMaterial( const float x, const float y, const idVec2& radius, const int numSides, const idVec4& tcInfo, const idMaterial* material, const idVec4& color, float rotation ) {
	DC_PLACEHOLDER("DC:DrawCircleMaterial|%s\n", material?material->GetName():NULL);

	if(!material)
		return;
}

void sdDeviceContextLocal::DrawCircleMaterialMasked( const float x, const float y, const idVec2& radius, const int numSides, const idVec4& tcInfo, const idMaterial* material, const idVec4& color, float rotation, float s11, float t11, float s12, float t12 ) {
	DC_PLACEHOLDER("DC:DrawCircleMaterialMasked|%s\n", material?material->GetName():NULL);

	if(!material)
		return;
}

void sdDeviceContextLocal::DrawCircle( const float x, const float y, const idVec2& radius, const float width, const int numSides, const idVec4& color ) {
	DC_PLACEHOLDER("DC:DrawCircle\n");
}

void sdDeviceContextLocal::DrawLineMaterial( const idVec2& start, const idVec2& end, const float width, const idMaterial* material, const idVec4& color ) {
	DC_UNUSED_ON_GAME
	DC_PLACEHOLDER("DC:DrawLineMaterial|%s\n", material?material->GetName():NULL);

	if(!material)
		return;
}

void sdDeviceContextLocal::DrawLine( const idVec2& start, const idVec2& end, const float width, const idVec4 &color ) {
	DC_PLACEHOLDER("DC:DrawLine\n");
}

void sdDeviceContextLocal::DrawFilledArc( const float x, const float y, const float radius, int numSides, float percent, const idVec4 &color, float startAngle, const idMaterial *material ) {
	DC_UNUSED_ON_GAME
	DC_PLACEHOLDER("DC:DrawFilledArc|%s\n", material?material->GetName():NULL);

	if(!material)
		return;
}

void sdDeviceContextLocal::DrawFilledArcMasked( const float x, const float y, const float radius, int numSides, float percent, const idVec4 &color, float s11, float t11, float s12, float t12, float startAngle, const idMaterial *material ) {
	DC_PLACEHOLDER("DC:DrawFilledArcMasked|%s\n", material?material->GetName():NULL);

	if(!material)
		return;
}

void sdDeviceContextLocal::DrawArc( const float x, const float y, const float radius, const float width, const int numSides, const float percent, const idVec4 &color, const float startAngle ) {
	DC_UNUSED_ON_GAME
	DC_PLACEHOLDER("DC:DrawArc\n");
}

void sdDeviceContextLocal::DrawTimer( const float x, const float y, const float w, const float h, float percent, const idVec4 &color, const idMaterial* material, bool invert, const idVec2& st0, const idVec2& st1 ) {
	DC_PLACEHOLDER("DC:DrawTimer|%s\n", material?material->GetName():NULL);

	if(!material)
		return;
}

qhandle_t sdDeviceContextLocal::FindFont( const char* name ) {
#if 0
	name = "fonts";
#endif
	int c = fonts.Num();

	for (int i = 0; i < c; i++) {
		if (idStr::Icmp(name, fonts[i].name) == 0) {
			return i;
		}
	}

	// If the font was not found, try to register it
	idStr fileName;
	if(!idStr::Icmpn(name, "fonts", 5))
		fileName = name;
	else
	{
		fileName = "fonts";
		fileName.AppendPath(name);
	}
	fileName.Replace("fonts", va("fonts/%s", fontLang.c_str()));

	sdLocFont_t *fc = FindFontConfig(name);

	fontInfoEx_t fontInfo;
    memset(&fontInfo, 0, sizeof(fontInfoEx_t));
	int index = fonts.Append(fontInfo);

	bool fontLoaded = false;
#ifdef _D3BFG_FONT
	const char *d3bfgFontName = harm_gui_useD3BFGFont.GetString();
	if(d3bfgFontName && d3bfgFontName[0] && idStr::Cmp(d3bfgFontName, "0") != 0)
	{
		if(idStr::Cmp(d3bfgFontName, "1") == 0)
		{
			idStr fname(name);
			fname.StripPath();
			if(!idStr::Icmp("an", fname))
				d3bfgFontName = "Arial_Narrow";
			else if(!idStr::Icmp("arial", fname))
				d3bfgFontName = "Arial_Narrow";
			else if(!idStr::Icmp("bank", fname))
				d3bfgFontName = "BankGothic_Md_BT";
			else if(!idStr::Icmp("micro", fname))
				d3bfgFontName = "microgrammadbolext";
			else
				d3bfgFontName = "Chainlink_Semi_Bold";
		}
        if(d3bfgFontName && d3bfgFontName[0])
        {
            idStr newFileName = fileName;
            newFileName.Replace(va("fonts/%s", fontLang.c_str()), "newfonts/");
            newFileName.StripFilename();
            newFileName.AppendPath(d3bfgFontName);
            if (renderSystem->RegisterFont(newFileName, fonts[index]))
            {
                common->Printf("Font '%s' using DOOM3-BFG new font '%s'.\n", name, newFileName.c_str());
                fontLoaded = true;
            }
            else // load default if fail
            {
                common->Printf("Font '%s' load DOOM3-BFG new font '%s' fail, try using default font.\n", name, newFileName.c_str());
                fontLoaded = renderSystem->RegisterFont(fileName, fonts[index]);
            }
        }
        else
        {
            common->Printf("Font '%s' not use DOOM3-BFG new font.\n", name);
            fontLoaded = renderSystem->RegisterFont(fileName, fonts[index]);
        }
	}
	else
#endif
	fontLoaded = renderSystem->RegisterFont(fileName, fonts[index]);
	if (fontLoaded) {
		idStr::Copynz(fonts[index].name, name, sizeof(fonts[index].name));
#ifdef _WCHAR_LANG
		if(!_hasWideCharFont)
		{
			const fontInfoEx_t *f = &fonts[index];
			if(f->fontInfoSmall.numIndexes > 0 || f->fontInfoMedium.numIndexes > 0 || f->fontInfoLarge.numIndexes > 0)
				_hasWideCharFont = true;
		}
#endif
		if(fc)
			fc->fontId = index;
		return index;
	} else {
		if(fc)
		{
			common->Printf("Converting and caching true type font '%s' to DOOM3 font......\n", name);
			if(R_ExportTrueTypeFont(fc->file.c_str(), name, fontLang.c_str(), DEFAULT_FONT_TEXTURE_SIZE))
			{
				common->Printf("Convert and cached true type font '%s' to DOOM3 font successful.\n", name);
				fontLoaded = renderSystem->RegisterFont(fileName, fonts[index]);
			}
			else	
				common->Warning("Couldn't convert and cache true type font '%s' to DOOM3 font.", name);
		}
		if (fontLoaded) {
			idStr::Copynz(fonts[index].name, name, sizeof(fonts[index].name));
			fc->fontId = index;
			return index;
		} else {
			common->Printf("Could not register font %s [%s]\n", name, fileName.c_str());
			return -1;
		}
	}
}

void sdDeviceContextLocal::FreeFont( const qhandle_t font ) {
}

const int sdDeviceContextLocal::GetFontHeight( const qhandle_t font, const int pointSize ) {
    return pointSize;
}

void sdDeviceContextLocal::SetFont( const qhandle_t num ) {
	if (num >= 0 && num < fonts.Num()) {
		activeFont = &fonts[num];
	} else {
		activeFont = &fonts[0];
	}
}

void sdDeviceContextLocal::SetFontSize( const int pointSize ) {
}

void sdDeviceContextLocal::DrawText( const wchar_t* text, const sdBounds2D& rect, unsigned int flags ) {
	idStr str = WStrToStr(text);

	bool wrap = (flags & DTF_WORDWRAP) && (flags & DTF_SINGLELINE) == 0;
	int textAlign;
	if (flags & DTF_CENTER)
		textAlign = ALIGN_CENTER;
	else if (flags & DTF_RIGHT)
		textAlign = ALIGN_RIGHT;
	else
		textAlign = ALIGN_LEFT;
	DrawText(str.c_str(), 0.3f, textAlign, tr.guiModel->CurrentColor(), rect, wrap, -1, false, NULL, 0);
}

void sdDeviceContextLocal::GetTextDimensions( const wchar_t* text, const sdBounds2D& rect, unsigned int flags, const qhandle_t font, const int pointSize, int& width, int& height, float* scale, int** charAdvances, idList< int >* lineBreaks ) {
	idStr str = WStrToStr(text);

	float fontScale = 0.3f;
	SetFont(font);
	bool wrap = (flags & DTF_WORDWRAP) && (flags & DTF_SINGLELINE) == 0;
	int textAlign;
	if (flags & DTF_CENTER)
		textAlign = ALIGN_CENTER;
	else if (flags & DTF_RIGHT)
		textAlign = ALIGN_RIGHT;
	else
		textAlign = ALIGN_LEFT;
	width = DrawText(str.c_str(), fontScale, textAlign, tr.guiModel->CurrentColor(), rect, wrap, -1, true, lineBreaks, 0) * MaxCharWidth(fontScale);
	height = MaxCharHeight(fontScale);

	if (scale)
		*scale = fontScale;
}

void sdDeviceContextLocal::OverrideAspectRationCorrection( bool setOverride ) {
}

float sdDeviceContextLocal::GetAspectRatioCorrection() const {
    return 1.0f;
}

bool sdDeviceContextLocal::ClippedCoords(float *x, float *y, float *w, float *h, float *s1, float *t1, float *s2, float *t2)
{

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
	//printf("ttt|%f %f %f %f\n",s1,t1,s2,t2);
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
	if (x) {
		*x *= xScale;
	}
	
	if (y) {
		*y *= yScale;
	}
	
	if (w) {
		*w *= xScale;
	}
	
	if (h) {
		*h *= yScale;
	}
}

void sdDeviceContextLocal::DrawStretchPicRotated(float x, float y, float w, float h, float s1, float t1, float s2, float t2, const idMaterial *shader, float angle)
{

	//printf("rrr|%f %f %f %f\n",s1,t1,s2,t2);
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

	if(shader)
	{

	idVec4 c = tr.guiModel->CurrentColor();
	renderSystem->SetColor(colorRed);
	idStr str = "";
	str.Append(shader->GetName());
	idWStr wstr = StrToWStr(str);
	sdBounds2D bb = sdBounds2D(x,y,w,h);
	bb.GetRight() = 640;
	DrawText(wstr.c_str(), bb, 0);

	renderSystem->SetColor(c);
	}
}

void sdDeviceContextLocal::SetupFonts() {
	fonts.SetGranularity(1);

	fontLang = cvarSystem->GetCVarString("sys_lang");

	// western european languages can use the english font
	if (fontLang == "french" || fontLang == "german" || fontLang == "spanish" || fontLang == "italian") {
		fontLang = "english";
	}

	// Default font has to be added first
	FindFont("fonts");

	common->Printf("Loading font configs......\n");
	LoadFontConfigs("english");
	if(idStr::Icmp(fontLang, "english"))
		LoadFontConfigs(fontLang);
	common->Printf("%d font configs found.\n", fontConfigs.Num());
}

void sdDeviceContextLocal::SetFontByScale(float scale)
{
	if (scale <= gui_smallFontLimit.GetFloat()) {
		useFont = &activeFont->fontInfoSmall;
		activeFont->maxHeight = activeFont->maxHeightSmall;
		activeFont->maxWidth = activeFont->maxWidthSmall;
	} else if (scale <= gui_mediumFontLimit.GetFloat()) {
		useFont = &activeFont->fontInfoMedium;
		activeFont->maxHeight = activeFont->maxHeightMedium;
		activeFont->maxWidth = activeFont->maxWidthMedium;
	} else {
		useFont = &activeFont->fontInfoLarge;
		activeFont->maxHeight = activeFont->maxHeightLarge;
		activeFont->maxWidth = activeFont->maxWidthLarge;
	}
}

void sdDeviceContextLocal::PaintChar(float x,float y,float width,float height,float scale,float	s,float	t,float	s2,float t2,const idMaterial *hShader)
{
	float	w, h;
	w = width * scale;
	h = height * scale;

	if (ClippedCoords(&x, &y, &w, &h, &s, &t, &s2, &t2)) {
		return;
	}

	AdjustCoords(&x, &y, &w, &h);
	DrawStretchPic(x, y, w, h, s, t, s2, t2, hShader);
}

void sdDeviceContextLocal::DrawEditCursor(float x, float y, float scale)
{
	if ((int)(com_ticNumber >> 4) & 1) {
		return;
	}

	SetFontByScale(scale);
	float useScale = scale * useFont->glyphScale;
	const glyphInfo_t *glyph2 = &useFont->glyphs[(overStrikeMode) ? '_' : '|'];
	float	yadj = useScale * glyph2->top;
	PaintChar(x, y - yadj,glyph2->imageWidth,glyph2->imageHeight,useScale,glyph2->s,glyph2->t,glyph2->s2,glyph2->t2,glyph2->glyph);
}

int sdDeviceContextLocal::DrawText(float x, float y, float scale, idVec4 color, const char *text, float adjust, int limit, int style, int cursor)
{
	int			len, count;
	idVec4		newColor;
	const glyphInfo_t *glyph;
	float		useScale;
	SetFontByScale(scale);
	useScale = scale * useFont->glyphScale;
	count = 0;

	if (text && color.w != 0.0f) {
		const unsigned char	*s = (const unsigned char *)text;
		renderSystem->SetColor(color);
		memcpy(&newColor[0], &color[0], sizeof(idVec4));
		len = strlen(text);

		if (limit > 0 && len > limit) {
			len = limit;
		}

#ifdef _WCHAR_LANG
        if(AsASCIICharLang(text, (int)len))
        {
#endif
		while (s && *s && count < len) {
			if (*s < GLYPH_START || *s > GLYPH_END) {
				s++;
				continue;
			}

			glyph = &useFont->glyphs[*s];

			//
			// int yadj = Assets.textFont.glyphs[text[i]].bottom +
			// Assets.textFont.glyphs[text[i]].top; float yadj = scale *
			// (Assets.textFont.glyphs[text[i]].imageHeight -
			// Assets.textFont.glyphs[text[i]].height);
			//
			if (idStr::IsColor((const char *)s)) {
				if (*(s+1) == C_COLOR_DEFAULT) {
					newColor = color;
				} else {
					newColor = idStr::ColorForIndex(*(s+1));
					newColor[3] = color[3];
				}

				if (cursor == count || cursor == count+1) {
					float partialSkip = ((glyph->xSkip * useScale) + adjust) / 5.0f;

					if (cursor == count) {
						partialSkip *= 2.0f;
					} else {
						renderSystem->SetColor(newColor);
					}

					DrawEditCursor(x - partialSkip, y, scale);
				}

				renderSystem->SetColor(newColor);
				s += 2;
				count += 2;
				continue;
			} else {
				float yadj = useScale * glyph->top;
				PaintChar(x,y - yadj,glyph->imageWidth,glyph->imageHeight,useScale,glyph->s,glyph->t,glyph->s2,glyph->t2,glyph->glyph);

				if (cursor == count) {
					DrawEditCursor(x, y, scale);
				}

				x += (glyph->xSkip * useScale) + adjust;
				s++;
				count++;
			}
		}
#ifdef _WCHAR_LANG
        }
        else
        {
            idStr drawText = text;
            int charIndex = 0;
            int lastCharIndex = 0;

            while( charIndex < len ) {
                lastCharIndex = charIndex;
                uint32_t textChar = drawText.UTF8Char( charIndex );

                glyph = R_Font_GetGlyphInfo(useFont, textChar);
                if (!glyph) {
                    continue;
                }

                //karin: charIndex will increment when read UTF8 character, so use last charIndex
                if( textChar == C_COLOR_ESCAPE && idStr::IsColor( drawText.c_str() + lastCharIndex ) ) {
                    // textChar == '^' and charIndex is color value current
                    if( drawText[ charIndex ] == C_COLOR_DEFAULT ) {
                        newColor = color;
                    } else {
                        newColor = idStr::ColorForIndex( drawText[ charIndex ] );
                        newColor[3] = color[3];
                    }
                    if( cursor == charIndex - 1 || cursor == charIndex ) {
                        float partialSkip = ((glyph->xSkip * useScale) + adjust) / 5.0f;

                        if (cursor == count) {
                            partialSkip *= 2.0f;
                        } else {
                            renderSystem->SetColor(newColor);
                        }

                        DrawEditCursor(x - partialSkip, y, scale);
                    }
                    renderSystem->SetColor( newColor );
                    charIndex++; //karin: skip color value character
                    continue;
                } else {
                    float yadj = useScale * glyph->top;
                    PaintChar(x,y - yadj,glyph->imageWidth,glyph->imageHeight,useScale,glyph->s,glyph->t,glyph->s2,glyph->t2,glyph->glyph);

                    if( cursor == charIndex - 1 ) {
                        DrawEditCursor( x, y, scale );
                    }

                    x += (glyph->xSkip * useScale) + adjust;
                }
            }
        }
#endif

		if (cursor == len) {
			DrawEditCursor(x, y, scale);
		}
	}

	return count;
}

int sdDeviceContextLocal::MaxCharWidth(float scale)
{
	SetFontByScale(scale);
	float useScale = scale * useFont->glyphScale;
	return idMath::FtoiFast(activeFont->maxWidth * useScale);
}

int sdDeviceContextLocal::MaxCharHeight(float scale)
{
	SetFontByScale(scale);
	float useScale = scale * useFont->glyphScale;
	return idMath::FtoiFast(activeFont->maxHeight * useScale);
}

int sdDeviceContextLocal::CharWidth(const char c, float scale)
{
	glyphInfo_t *glyph;
	float		useScale;
	SetFontByScale(scale);
	fontInfo_t	*font = useFont;
	useScale = scale * font->glyphScale;
	glyph = &font->glyphs[(const unsigned char)c];
	return idMath::FtoiFast(glyph->xSkip * useScale);
}

int sdDeviceContextLocal::DrawText(const char *text, float textScale, int textAlign, idVec4 color, const sdBounds2D &rectDraw, bool wrap, int cursor, bool calcOnly, idList<int> *breaks, int limit)
{
	const char	*p, *textPtr, *newLinePtr;
	char		buff[1024];
	int			len, newLine, newLineWidth, count;
	float		y;
	float		textWidth;

	float		charSkip = MaxCharWidth(textScale) + 1;
	float		lineSkip = MaxCharHeight(textScale);

	float		cursorSkip = (cursor >= 0 ? charSkip : 0);

	bool		lineBreak, wordBreak;

	SetFontByScale(textScale);

	textWidth = 0;
	newLinePtr = NULL;

	SetTempColor(color);
	if (!calcOnly && !(text && *text)) {
		if (cursor == 0) {
			renderSystem->SetColor(color);
			DrawEditCursor(rectDraw.GetLeft(), lineSkip + rectDraw.GetTop(), textScale);
		}

		UnsetTempColor();
		return idMath::FtoiFast(rectDraw.GetWidth() / charSkip);
	}

	textPtr = text;

	y = lineSkip + rectDraw.GetTop();
	len = 0;
	buff[0] = '\0';
	newLine = 0;
	newLineWidth = 0;
	p = textPtr;

	if (breaks) {
		breaks->Append(0);
	}

	count = 0;
	textWidth = 0;
	lineBreak = false;
	wordBreak = false;

#ifdef _WCHAR_LANG
    if(AsASCIICharLang(text, (int)strlen(text)))
    {
#endif
	while (p) {

		if (*p == '\n' || *p == '\r' || *p == '\0') {
			lineBreak = true;

			if ((*p == '\n' && *(p + 1) == '\r') || (*p == '\r' && *(p + 1) == '\n')) {
				p++;
			}
		}

		int nextCharWidth = (idStr::CharIsPrintable(*p) ? CharWidth(*p, textScale) : cursorSkip);
		// FIXME: this is a temp hack until the guis can be fixed not not overflow the bounding rectangles
		//		  the side-effect is that list boxes and edit boxes will draw over their scroll bars
		//	The following line and the !linebreak in the if statement below should be removed
		nextCharWidth = 0;

		if (!lineBreak && (textWidth + nextCharWidth) > rectDraw.GetWidth()) {
			// The next character will cause us to overflow, if we haven't yet found a suitable
			// break spot, set it to be this character
			if (len > 0 && newLine == 0) {
				newLine = len;
				newLinePtr = p;
				newLineWidth = textWidth;
			}

			wordBreak = true;
		} else if (lineBreak || (wrap && (*p == ' ' || *p == '\t'))) {
			// The next character is in view, so if we are a break character, store our position
			newLine = len;
			newLinePtr = p + 1;
			newLineWidth = textWidth;
		}

		if (lineBreak || wordBreak) {
			float x = rectDraw.GetLeft();

			if (textAlign == ALIGN_RIGHT) {
				x = rectDraw.GetLeft() + rectDraw.GetWidth() - newLineWidth;
			} else if (textAlign == ALIGN_CENTER) {
				x = rectDraw.GetLeft() + (rectDraw.GetWidth() - newLineWidth) / 2;
			}

			if (wrap || newLine > 0) {
				buff[newLine] = '\0';

				// This is a special case to handle breaking in the middle of a word.
				// if we didn't do this, the cursor would appear on the end of this line
				// and the beginning of the next.
				if (wordBreak && cursor >= newLine && newLine == len) {
					cursor++;
				}
			}

			if (!calcOnly) {
				count += DrawText(x, y, textScale, color, buff, 0, 0, 0, cursor);
			}

			if (cursor < newLine) {
				cursor = -1;
			} else if (cursor >= 0) {
				cursor -= (newLine + 1);
			}

			if (!wrap) {
				UnsetTempColor();
				return newLine;
			}

			if ((limit && count > limit) || *p == '\0') {
				break;
			}

			y += lineSkip + 5;

			if (!calcOnly && y > rectDraw.GetBottom()) {
				break;
			}

			p = newLinePtr;

			if (breaks) {
				breaks->Append(p - text);
			}

			len = 0;
			newLine = 0;
			newLineWidth = 0;
			textWidth = 0;
			lineBreak = false;
			wordBreak = false;
			continue;
		}

		buff[len++] = *p++;
		buff[len] = '\0';

		// update the width
		if (*(buff + len - 1) != C_COLOR_ESCAPE && (len <= 1 || *(buff + len - 2) != C_COLOR_ESCAPE)) {
			textWidth += textScale * useFont->glyphScale * useFont->glyphs[(const unsigned char)*(buff + len - 1)].xSkip;
		}
	}
#ifdef _WCHAR_LANG
    }
    else
    {
        idStr drawText = text;
        int			charIndex = 0;
        idStr textBuffer;
        int			lastBreak = 0;
        float		textWidthAtLastBreak = 0.0f;

        while( charIndex < drawText.Length() ) {
            uint32_t textChar = drawText.UTF8Char( charIndex );

            // See if we need to start a new line.
            if( textChar == '\n' || textChar == '\r' || charIndex == drawText.Length() ) {
                lineBreak = true;
                if( charIndex < drawText.Length() ) {
                    // New line character and we still have more text to read.
                    char nextChar = drawText[ charIndex + 1 ];
                    if( ( textChar == '\n' && nextChar == '\r' ) || ( textChar == '\r' && nextChar == '\n' ) ) {
                        // Just absorb extra newlines.
                        textChar = drawText.UTF8Char( charIndex );
                    }
                }
            }

            // Check for escape colors if not then simply get the glyph width.
            if( textChar == C_COLOR_ESCAPE && charIndex < drawText.Length() ) {
                textBuffer.AppendUTF8Char( textChar );
                textChar = drawText.UTF8Char( charIndex );
            }

            // If the character isn't a new line then add it to the text buffer.
            if( textChar != '\n' && textChar != '\r' ) {
                textWidth += R_Font_GetCharWidth( useFont, textChar, textScale );
                textBuffer.AppendUTF8Char( textChar );
            }

            if( !lineBreak && ( textWidth > rectDraw.GetWidth() ) ) {
                // The next character will cause us to overflow, if we haven't yet found a suitable
                // break spot, set it to be this character
                if( textBuffer.Length() > 0 && lastBreak == 0 ) {
                    lastBreak = textBuffer.Length();
                    textWidthAtLastBreak = textWidth;
                }
                wordBreak = true;
            } else if( lineBreak || ( wrap && ( textChar == ' ' || textChar == '\t' ) ) ) {
                // The next character is in view, so if we are a break character, store our position
                lastBreak = textBuffer.Length();
                textWidthAtLastBreak = textWidth;
            }

            // We need to go to a new line
            if( lineBreak || wordBreak ) {
                float x = rectDraw.GetLeft();

                if( textWidthAtLastBreak > 0 ) {
                    textWidth = textWidthAtLastBreak;
                }

                // Align text if needed
                if( textAlign == ALIGN_RIGHT ) {
                    x = rectDraw.GetLeft() + rectDraw.GetWidth() - textWidth;
                } else if( textAlign == ALIGN_CENTER ) {
                    x = rectDraw.GetRight() + ( rectDraw.GetWidth() - textWidth ) / 2;
                }

                if( wrap || lastBreak > 0 ) {
                    // This is a special case to handle breaking in the middle of a word.
                    // if we didn't do this, the cursor would appear on the end of this line
                    // and the beginning of the next.
                    if( wordBreak && cursor >= lastBreak && lastBreak == textBuffer.Length() ) {
                        cursor++;
                    }
                }

                // Draw what's in the current text buffer.
                if( !calcOnly ) {
                    if( lastBreak > 0 ) {
                        count += DrawText( x, y, textScale, color, textBuffer.Left( lastBreak ).c_str(), 0, 0, 0, cursor );
                        textBuffer = textBuffer.Right( textBuffer.Length() - lastBreak );
                    } else {
                        count += DrawText( x, y, textScale, color, textBuffer.c_str(), 0, 0, 0, cursor );
                        textBuffer.Clear();
                    }
                }

                if( cursor < lastBreak ) {
                    cursor = -1;
                } else if( cursor >= 0 ) {
                    cursor -= ( lastBreak + 1 );
                }

                // If wrap is disabled return at this point.
                if( !wrap ) {
					UnsetTempColor();
                    return lastBreak;
                }

                // If we've hit the allowed character limit then break.
                if( limit && count > limit ) {
                    break;
                }

                y += lineSkip + 5;

                if( !calcOnly && y > rectDraw.GetBottom() ) {
                    break;
                }

                // If breaks were requested then make a note of this one.
                if( breaks ) {
                    breaks->Append( drawText.Length() - charIndex );
                }

                // Reset necessary parms for next line.
                lastBreak = 0;
                textWidth = 0;
                textWidthAtLastBreak = 0;
                lineBreak = false;
                wordBreak = false;

                // Reassess the remaining width
                for( int i = 0; i < textBuffer.Length(); ) {
                    if( textChar != C_COLOR_ESCAPE ) {
                        textWidth += R_Font_GetCharWidth( useFont, textBuffer.UTF8Char( i ), textScale );
                    }
                }

                continue;
            }
        }
    }
#endif

	UnsetTempColor();

	return idMath::FtoiFast(rectDraw.GetWidth() / charSkip);
}

void sdDeviceContextLocal::SetTempColor(const idVec4 &c)
{
	tempColor = tr.guiModel->CurrentColor();
	renderSystem->SetColor(c);
	usingTempColor = true;
}

void sdDeviceContextLocal::UnsetTempColor()
{
	if(usingTempColor)
	{
		usingTempColor = false;
		renderSystem->SetColor(tempColor);
	}
}

bool sdDeviceContextLocal::ParseFontConfig(const char *path, sdLocFont_t &config) {
	idLexer src;
	src.SetFlags(LEXFL_ALLOWPATHNAMES);
	if(!src.LoadFile(path))
		return false;

	idToken token;
	while(true)
	{
		if(!src.ReadToken(&token))
			break;

		if(!idStr::Icmp(token, "file"))
		{
			if(!src.ReadToken(&token))
			{
				src.Error( "Parse font config: failed to parse file" );
				return false;
			}
			config.file = token.c_str();
			continue;
		}

		if(!idStr::Icmp(token, "faceIndex"))
		{
			config.faceIndex = src.ParseInt();
			continue;
		}

		src.Warning( "Parse font config: unexpected token '%s'.", token.c_str() );
	}
	return true;
}

sdLocFont_t * sdDeviceContextLocal::FindFontConfig(const char *name)
{
	for(int i = 0; i < fontConfigs.Num(); i++)
	{
		if(!idStr::Icmp(fontConfigs[i].name, name))
			return &fontConfigs[i];
	}
	return NULL;
}

void sdDeviceContextLocal::LoadFontConfigs(const char *lang) {
	idStr path("localization");
	if(!lang || !lang[0])
		lang = cvarSystem->GetCVarString("sys_lang");
	path.AppendPath(lang);
	path.AppendPath("fonts");

	idFileList* fileList = fileSystem->ListFiles(path.c_str(), ".font");

	common->Printf("Load font config on %s.....\n", path.c_str());

	for (int i = 0; i < fileList->GetNumFiles(); i++)
	{
		idLexer src;
		idToken	token;
		idStr fileName = fileList->GetList()[i];

		idStr str(path);
		str.AppendPath(fileName);

		sdLocFont_t config;
		if(!ParseFontConfig(str.c_str(), config))
			continue;

		fileName.StripFileExtension();
		sdLocFont_t *exists = FindFontConfig(fileName.c_str());
		if(exists)
		{
			exists->file = config.file;
			exists->faceIndex = config.faceIndex;
			common->Printf("Override %s font config '%s'.\n", lang, fileName.c_str());
		}
		else
		{
			config.name = fileName;
			config.fontId = -1;
			fontConfigs.Append(config);
			common->Printf("Add %s font config '%s'.\n", lang, fileName.c_str());
		}
	}

	fileSystem->FreeFileList(fileList);
}



static sdDeviceContextLocal deviceContextLocal;

sdDeviceContext* deviceContext = &deviceContextLocal;
