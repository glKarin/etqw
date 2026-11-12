
#include "../../idlib/precompiled.h"

#include "TTFontManager.h"

#include "ui/DeviceContext.h"

#define DEFAULT_FONT_TEXTURE_SIZE 1024
#define FONT_CONFIG_FILE_EXT ".ttfc"
#define FONT_CONVERT_FILE_EXT ".ttfc_checksum"
#define FONT_CONVERT_VERSION 1
#define FONT_CONVERT_MAGIC "fontDef"

extern bool R_ExportTrueTypeFont(const char *fontPath, const char *fontType, const char *language, int width);



static unsigned int TrueTypeFontFileChecksum(const char *file)
{
	unsigned int checksum;
	void *data = NULL;
	int length = fileSystem->ReadFile(file, &data, NULL);
	if(length > 0)
		checksum = MD5_BlockChecksum(data, length);
	else
		checksum = 0;
	Mem_Free(data);
	return checksum;
}



ttFontConfig_t::ttFontConfig_t(void)
	: version(0),
	checksum(0),
	texelSize(0),
	fontId(-1)
{
}

void ttFontConfig_t::MakeDefault(bool full)
{
	if (full)
	{
		version = FONT_CONVERT_VERSION;
		texelSize = DEFAULT_FONT_TEXTURE_SIZE;
	}
	checksum = 0;
	fontId = -1;
}

void ttFontConfig_t::ConfigFilePath(idStr &ret) const
{
	if (!name.IcmpPrefix("fonts/"))
		ret = name;
	else
	{
		ret = "fonts";
		ret.AppendPath(name);
	}
	ret.SetFileExtension(FONT_CONFIG_FILE_EXT);
}

void ttFontConfig_t::ConvertFilePath(idStr &ret) const
{
	if (!name.IcmpPrefix("fonts/"))
		ret = name;
	else
	{
		ret = "fonts";
		ret.AppendPath(name);
	}
	ret.SetFileExtension(FONT_CONVERT_FILE_EXT);
}

bool ttFontConfig_t::Save(void) const
{
	idStr convertFile;
	ConvertFilePath(convertFile);

	idFile *f = fileSystem->OpenFileWrite(convertFile);
	if (!f)
		return false;

	f->Printf("%s %s {\n", FONT_CONVERT_MAGIC, name.c_str());
#define FC_WRITEF(name, format, value) f->Printf("    \"%s\" " format "\n", name, value);
	FC_WRITEF("version", "%d", version);
	FC_WRITEF("file", "\"%s\"", file.c_str());
	FC_WRITEF("lang", "\"%s\"", lang.c_str());
	FC_WRITEF("texelSize", "%d", texelSize);
	FC_WRITEF("checksum", "%u", checksum);
#undef FC_WRITEF
	f->Printf("}\n");

	fileSystem->CloseFile(f);

	return true;
}

int ttFontConfig_t::Restore(bool loadChecksum) {
	idStr configFile;
	if (loadChecksum)
		ConfigFilePath(configFile);
	else
		ConvertFilePath(configFile);

	idLexer src;
	src.SetFlags(DECL_LEXER_FLAGS | LEXFL_NOFATALERRORS);
	if(!src.LoadFile(configFile.c_str()))
		return -1;

	idToken token;
	if (!src.ExpectTokenType(TT_NAME, 0, &token))
	{
		src.Error("Font config magic not match: '%s' != '%s'", FONT_CONVERT_MAGIC, token.c_str());
		return 0;
	}

	if (!src.ExpectTokenType(TT_NAME, 0, &token)) {
		src.Warning("Font config name not found");
		src.UnreadToken(&token);
	}
	else
	{
		if (token.Icmp(name))
		{
			src.Error("Font config name not match: '%s' != '%s'", name.c_str(), token.c_str() );
			return 0;
		}
	}

	src.ExpectTokenString("{");

	while(true)
	{
		if(!src.ReadToken(&token))
		{
			src.Error( "Parse font config: missing end }" );
			return 0;
		}

		if (!token.Cmp("}"))
		{
			src.UnreadToken(&token);
			break;
		}

		if(!idStr::Icmp(token, "file"))
		{
			if(!src.ReadToken(&token))
			{
				src.Error( "Parse font config: failed to parse file" );
				return 0;
			}
			file = token.c_str();
			if (loadChecksum)
				checksum = TrueTypeFontFileChecksum(file.c_str());
			continue;
		}

		if(!idStr::Icmp(token, "lang"))
		{
			if(!src.ReadToken(&token))
			{
				src.Error( "Parse font config: failed to parse lang" );
				return 0;
			}
			lang = token.c_str();
			continue;
		}

		if(!idStr::Icmp(token, "texelSize"))
		{
			texelSize = src.ParseInt();
			continue;
		}

		if(!idStr::Icmp(token, "checksum"))
		{
			checksum = src.ParseInt();
			continue;
		}

		if(!idStr::Icmp(token, "version"))
		{
			version = src.ParseInt();
			continue;
		}

		src.Warning( "Parse font config: unexpected token '%s'.", token.c_str() );
	}

	src.ExpectTokenString("}");

	return checksum;
}

bool ttFontConfig_t::Remove(void) const
{
	idStr convertFile;
	ConvertFilePath(convertFile);

	if (fileSystem->ReadFile(convertFile.c_str(), NULL) > 0)
	{
		fileSystem->RemoveFile(convertFile);
		return true;
	}
	else
		return false;
}

bool ttFontConfig_t::SupportLang(const char *target) const
{
	if (lang.IsEmpty())
		return true;
	if (!lang.Icmp(target))
		return true;
	idList<idStr> list;
	idStr::SplitUnique(list, lang, ',');
	for (int i = 0; i < list.Num(); i++)
	{
		if (!list[i].Icmp(target))
			return true;
	}
	return false;
}



idList<ttFontConfig_t> idTTFontManager::fontConfigs;


idTTFontManager::idTTFontManager(void)
{
}

void idTTFontManager::Init(void) {
}

void idTTFontManager::Shutdown(void) {
	fontConfigs.Clear();
}

qhandle_t idTTFontManager::FindFont( const char* name ) {
	ttFontConfig_t *fc = FindFontConfig(name);
	return fc ? fc->fontId : -1;
}

qhandle_t idTTFontManager::RegisterFont(int fontId, const char *name, const char *lang, const char *fileName, fontInfoEx_t &font)
{
	int fid = FindFont(name);
	if (fid != -1) {
		return fid;
	}

	ttFontConfig_t config;
	config.MakeDefault(true);
	config.name = name;
	if(!ReadConfig(config))
		return -1;

	if (!config.SupportLang(lang))
	{
		common->Warning("Font config '%s' not support language '%s'", config.lang.c_str(), lang);
		return -1;
	}

	bool needConvert = false;
	ttFontConfig_t convertConfig;
	convertConfig.name = name;
	if (ReadConvertConfig(convertConfig))
	{
		if (convertConfig.version != config.version)
		{
			common->Printf("Font '%s' version changed!\n", name);
			needConvert = true;
		}
		else if (convertConfig.file != config.file)
		{
			common->Printf("Font '%s' path changed!\n", name);
			needConvert = true;
		}
		else if (convertConfig.checksum != config.checksum)
		{
			common->Printf("Font '%s' is outdate!\n", name);
			needConvert = true;
		}
		else if (convertConfig.lang != config.lang)
		{
			common->Printf("Font '%s' language changed!\n", name);
			needConvert = true;
		}
		else if (convertConfig.texelSize != config.texelSize)
		{
			common->Printf("Font '%s' texel size changed!\n", name);
			needConvert = true;
		}
		else
			common->DPrintf("Font '%s' don't need convert.\n", name);
	}
	else
	{
		common->DPrintf("Font '%s' need convert!\n", name);
		needConvert = true;
	}

	if (needConvert)
	{
		if(ConvertFont(config, lang))
		{
			config.fontId = fontId;
			fontConfigs.Append(config);
		}
	}
	else
	{
		config.fontId = fontId;
		fontConfigs.Append(config);
	}

	if (config.fontId == fontId)
	{
		bool loaded = renderSystem->RegisterFont(fileName, font);
		if(loaded)
			common->Printf("TTF Font '%s' is loaded.\n", fileName);
		else
			common->Printf("TTF Font '%s' load fail.\n", fileName);
		return loaded ? config.fontId : -1;
	}
	else
		return -1;
}

void idTTFontManager::FreeFont( qhandle_t font ) {
	for(int i = 0; i < fontConfigs.Num(); i++)
	{
		if(fontConfigs[i].fontId == font)
		{
			fontConfigs.RemoveIndex(i);
			return;
		}
	}
}

bool idTTFontManager::ReadConfig(ttFontConfig_t &config) const {
	idStr configFile;
	config.ConfigFilePath(configFile);
	int res = config.Restore(true);
	if (res > 0)
		common->Printf("Read font config in file %s: file=%s, lang=%s, size=%d, checksum=%u\n", configFile.c_str(), config.file.c_str(), config.lang.c_str(), config.texelSize, config.checksum);
	else if (res < 0)
		common->DPrintf("Font config file %s not exists\n", configFile.c_str());
	else
		common->Warning("Read font config file %s fail\n", configFile.c_str());
	return res > 0;
}

ttFontConfig_t * idTTFontManager::FindFontConfig(const char *name)
{
	for(int i = 0; i < fontConfigs.Num(); i++)
	{
		if(!idStr::Icmp(fontConfigs[i].name, name))
			return &fontConfigs[i];
	}
	return NULL;
}

bool idTTFontManager::ReadConvertConfig(ttFontConfig_t &convert) const
{
	idStr convertFile;
	convert.ConvertFilePath(convertFile);
	int res = convert.Restore(false);
	if (res > 0)
		common->Printf("Read font convert config in file %s: file=%s, lang=%s, size=%d, checksum=%u\n", convertFile.c_str(), convert.file.c_str(), convert.lang.c_str(), convert.texelSize, convert.checksum);
	else if (res < 0)
		common->DPrintf("Font convert config file %s not exists\n", convertFile.c_str());
	else
		common->Warning("Read font convert config file %s fail\n", convertFile.c_str());
	return res > 0;
}

void idTTFontManager::SaveConvertConfig(const ttFontConfig_t &config) const
{
	ttFontConfig_t convertConfig = config;
	idStr convertFile;
	convertConfig.ConvertFilePath(convertFile);
	if (convertConfig.Save())
		common->Printf("Save font convert config to %s.\n", convertFile.c_str());
	else
		common->Warning("Save font convert config to %s fail.", convertFile.c_str());
}

void idTTFontManager::RemoveConvertConfig(const ttFontConfig_t &fc) const
{
	idStr convertFile;
	fc.ConvertFilePath(convertFile);
	if (fc.Remove())
		common->Printf("Remove old font convert config file '%s'.\n", convertFile.c_str());
	// else common->Printf("Remove old font convert config file '%s' fail.\n", convertFile.c_str());
}

bool idTTFontManager::ConvertFont(const ttFontConfig_t &fc, const char *lang) const
{
	common->Printf("Converting and caching true type font '%s' to DOOM3 font......\n", fc.name.c_str());
	RemoveConvertConfig(fc);
	if(R_ExportTrueTypeFont(fc.file.c_str(), fc.name.c_str(), lang, fc.texelSize))
	{
		common->Printf("Convert and cached true type font '%s' to DOOM3 font successful.\n", fc.name.c_str());
		SaveConvertConfig(fc);
		return true;
	}
	else	
	{
		common->Warning("Couldn't convert and cache true type font '%s' to DOOM3 font.", fc.name.c_str());
		return false;
	}
}



static idTTFontManager ttfontManagerLocal;

idTTFontManager* ttfontManager = &ttfontManagerLocal;
