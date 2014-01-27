#ifndef RUI_OVERLAY_HPP
#define RUI_OVERLAY_HPP

#include <vector>

#include "rTypes.hpp"
#include "rDefs.hpp"

#include "rEngine.hpp"

#include "ruiBase.hpp"
#include "ruiWidget.hpp"


typedef std::vector<ruiWidget*> rWidgetVector;

class ruiOverlay : public ruiIOverlay{
public:
	ruiOverlay();

public:
	virtual void AddWidget(ruiWidget* widget);
	ruiWidget* GetWidget(int id);

	virtual void ShowModal(ruiWidget* widget);
	virtual void EndModal(ruiWidget* widget);

	void Update(rEngine& engine);
	void Draw(rEngine& engine);

	void Clear();

	virtual bool InjectTouchDown(const rTouch& touch);
	virtual bool InjectTouchMove(const rTouch& touch);
	virtual bool InjectTouchUp(const rTouch& touch);

	virtual bool InjectMouseDownEvent(rMouseButton button, const rMouseState& mouse);
	virtual bool InjectMouseUpEvent(rMouseButton button, const rMouseState& mouse);
	virtual bool InjectMouseMotionEvent(const rMouseState& mouse);

private:

	ruiWidget* SelectWidget(const rPoint& position);

private:

	rWidgetVector m_widgets;

	ruiWidget* m_activeWidget;
	ruiWidget* m_modalWidget;
};

#endif