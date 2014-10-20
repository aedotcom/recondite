#ifndef RUI_WIDGETBASE_HPP
#define RUI_WIDGETBASE_HPP

#include "rBuild.hpp"

#include "rString.hpp"
#include "rEngine.hpp"
#include "rObject.hpp"

#include "ui/ruiStyle.hpp"

class RECONDITE_API ruiWidgetBase : public rObject{
public:
	ruiWidgetBase(const rString& id, rEngine* engine);

public:
	virtual rRect BoundingBox() = 0;
	virtual rSize Size();
	virtual rPoint Position() const = 0;

	virtual void Update(rEngine& engine);
	virtual void Draw(rEngine& engine);

public:
	virtual void AddClass(const rString& className);
	virtual void RemoveClass(const rString& className);
	bool HasClass(const rString& className) const;
	void GetClasses(rArrayString& classlist);

	ruiStyle* Style();
	ruiStyle* ComputedStyle();

	virtual rString GetWidgetType() const = 0;

protected: //style related utility methods
	rFont* DetermineFont();

protected:
	virtual rSize ComputeSize() = 0;
	void InvalidateSize();

private:
	int GetClassIndex(const rString& className) const;
	void RecomputeStyle();
	
private:
	ruiStyle m_style;
	ruiStyle m_computedStyle;
	rArrayString m_classList;

	rSize m_size;

};

#endif