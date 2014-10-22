#include "ui/ruiOverlayLoader.hpp"

ruiOverlayLoader::ruiParseItemMap ruiOverlayLoader::s_parseItemMap;

ruiOverlayLoader::ruiOverlayLoader(rEngine* engine){
	m_engine = engine;

	InitParseItemMap();
}

void ruiOverlayLoader::InitParseItemMap(){
	if (s_parseItemMap.size() > 0) return;

	s_parseItemMap["stylesheet"] = &ruiOverlayLoader::ParseStylesheetItem;
	s_parseItemMap["absolutelayout"] = &ruiOverlayLoader::ParseAbsoluteLayoutItem;
	s_parseItemMap["text"] = &ruiOverlayLoader::ParseTextItem;
	s_parseItemMap["picker"] = &ruiOverlayLoader::ParsePickerItem;
}

void ruiOverlayLoader::Reset(){
	m_currentOverlay = NULL;

	m_path.clear();
	m_error.clear();
	m_layoutStack.clear();
}

ruiOverlayLoader::ruiParseItemMethod ruiOverlayLoader::GetParseItemMethod (const rString& itemName){
	rString item = itemName;
	
	for (size_t i =0; i < itemName.size(); i++)
		item[i] = tolower(itemName[i]);

	if (s_parseItemMap.count(item))
		return s_parseItemMap[item];
	else
		return NULL;
}

ruiOverlay* ruiOverlayLoader::ParseOverlay(const rString& path, rViewport* viewport){
	Reset();

	rXMLDocument document;
	rXMLReaderError error = document.LoadFromFile(path);

	rXMLElement* root = document.GetRoot();
	if (!root) return NULL;

	m_currentOverlay = new ruiOverlay(viewport);
	m_path = path;

	ParseChildItems(root);

	return m_currentOverlay;
}

void ruiOverlayLoader::ParseChildItems(rXMLElement* parent){
	for (size_t i = 0; i < parent->NumChildren(); i++){
		rXMLElement* element = parent->GetChild(i);
		ruiParseItemMethod method = GetParseItemMethod(element->Name());

		if (method){ 
			(*this.*method)(element);
		}

	}
}

void ruiOverlayLoader::ParseStylesheetItem(rXMLElement* element){
	rString pathName = element->Text();
	
	rString dir = rPath::Directory(m_path);
	rString stylesheetPath = rPath::Combine(dir, pathName);

	rAssetStream cssFile = m_engine->content->LoadTextFromPath(stylesheetPath);
	
	if (cssFile)
		m_engine->ui->Styles()->ParseStylesheet(*cssFile);
}

void ruiOverlayLoader::ParseAbsoluteLayoutItem(rXMLElement* element){
	int top = 0;
	int left = 0;

	element->GetAttribute<int>("top", top);
	element->GetAttribute<int>("left", left);

	ruiAbsoluteLayout* absoluteLayout = new ruiAbsoluteLayout();
	absoluteLayout->SetTop(top);
	absoluteLayout->SetLeft(left);

	if (m_layoutStack.size() == 0)
		m_currentOverlay->SetLayout(absoluteLayout);

	m_layoutStack.push_back(absoluteLayout);

	ParseChildItems(element);

	m_layoutStack.pop_back();
}

void ruiOverlayLoader::ParseTextItem(rXMLElement* element){
	rString id = m_currentOverlay->GetDefaultId();
	element->GetAttribute<rString>("id", id);

	ruiText* text = new ruiText(element->Text(), id, m_engine);
	m_layoutStack.back()->AddItem(text);
	m_currentOverlay->AddWidget(text);
}

void ruiOverlayLoader::ParsePickerItem(rXMLElement* element){
	rString id = m_currentOverlay->GetDefaultId();
	element->GetAttribute<rString>("id", id);

	rArrayString options;

	size_t optionCount = element->NumChildren();
	for (size_t i =0; i < optionCount; i++){
		options.push_back(element->GetChild(i)->Text());
	}

	ruiPicker* picker = new ruiPicker(options, id, m_engine);
	m_layoutStack.back()->AddItem(picker);
	m_currentOverlay->AddWidget(picker);
}