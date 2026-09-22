
#ifndef _TTFONT_MANAGER_H
#define _TTFONT_MANAGER_H

struct ttFontConfig_t {
	int						version;
	idStr					file;
	unsigned int			checksum;
	idStr					lang;
	int						texelSize;

	idStr					name; // type
	int						fontId;

							ttFontConfig_t(void);
	void					MakeDefault(bool full);
	bool					Save(void) const;
	bool					Remove(void) const;
	int						Restore(bool checksum);
	void					ConvertFilePath(idStr &ret) const;
	void					ConfigFilePath(idStr &ret) const;
	bool					SupportLang(const char *lang) const;
};

class idTTFontManager
{
public:
							idTTFontManager(void);
							~idTTFontManager(void) {}

	void            		Init(void);
	void            		Shutdown(void);

	qhandle_t				RegisterFont(int fontId, const char *name, const char *lang, const char *fileName, fontInfoEx_t &font);
    qhandle_t				FindFont( const char* fontName );
    void					FreeFont( qhandle_t font );

private:
	bool					ReadConfig(ttFontConfig_t &config) const;
	ttFontConfig_t *		FindFontConfig(const char *name);
	bool					ConvertFont(const ttFontConfig_t &fc, const char *lang) const;
	bool					ReadConvertConfig(ttFontConfig_t &config) const;
	void					SaveConvertConfig(const ttFontConfig_t &fc) const;
	void					RemoveConvertConfig(const ttFontConfig_t &fc) const;

private:
	static idList<ttFontConfig_t> fontConfigs;
};

extern idTTFontManager* ttfontManager;

#endif /* !_TTFONT_MANAGER_H */
