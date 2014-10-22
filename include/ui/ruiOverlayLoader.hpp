#ifndef RUI_OVERLAYLOADER_HPP
#define RUI_OVERLAYLOADER_HPP

#include <map>
#include <vector>

#include "rBuild.hpp"
#include "rEngine.hpp"
#include "rPath.hpp"

#include "xml/rXMLDocument.hpp"

#include "ui/ruiOverlay.hpp"
#include "ui/ruiAbsoluteLayout.hpp"
#include "ui/ruiText.hpp"
#include "ui/ruiPicker.hpp"

class RECONDITE_API ruiOverlayLoader{
public:
	ruiOverlayLoader(rEngine* engine);
	
public:
	ruiOverlay* ParseOverlay(const rString& path, rViewport* viewport);
	
private:
	typedef void (ruiOverlayLoader::*ruiParseItemMethod)(rXMLElement* element);
	typedef std::map<rString, ruiParseItemMethod> ruiParseItemMap;
	typedef std::vector<ruiLayout*> ruiLayoutVector;

private:
	void Reset();
	ruiParseItemMethod GetParseItemMethod(const rString& itemName);

	void ParseChildItems(rXMLElement* parent);
	void ParseStylesheetItem(rXMLElement* element);
	void ParseAbsoluteLayoutItem(rXMLElement* element);
	void ParseTextItem(rXMLElement* element);
	void ParsePickerItem(rXMLElement* element);


	
private:
	rEngine* m_engine;
	
	ruiOverlay* m_currentOverlay;
	ruiLayoutVector m_layoutStack;

	rString m_path;
	rString m_error;

private:
	static void InitParseItemMap();
	static ruiParseItemMap s_parseItemMap;
};

#endif