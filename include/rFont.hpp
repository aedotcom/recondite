#ifndef R_FONT_HPP
#define R_FONT_HPP

#include <map>
#include <algorithm>
#include <climits>

#include "rBuild.hpp"
#include "rString.hpp"

#include "rFontGlyph.hpp"
#include "rTexture2D.hpp"
#include "rAsset.hpp"

#include "rSize.hpp"

typedef std::map<int, rFontGlyph*> rFontGlyphMap;

class RECONDITE_API rFont : public rAsset{
public:
	rFont(rTexture2D* texture, size_t size, size_t lineHeight, size_t ascender, size_t descender, int assetid, const rString& name, const rString& path);
	~rFont();
	
	rFontGlyph* GetGlyph(int scancode) const;
	rFontGlyph* AddGlyph(const rFontGlyph& g);
	size_t NumGlyphs() const;
	rTexture2D* Texture() const;
	
	rSize MeasureString(const rString& str, int lineWidth = INT_MAX) const;

	void RemoveGlyph(int scancode);
	
	void Clear();
	
	virtual rAssetType Type() const;

	size_t Size() const;
	size_t LineHeight() const;
	size_t Ascender() const;
	size_t Descender() const;

private:

	rFontGlyphMap m_glyphs;
	rTexture2D* m_texture;
	size_t m_size;
	size_t m_lineHeight;
	size_t m_ascender;
	size_t m_descender;
};

typedef std::map<rString, rFont*> rFontMap;

#endif
