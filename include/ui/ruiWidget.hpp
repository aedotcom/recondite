#ifndef RUI_WIDGET_HPP
#define RUI_WIDGET_HPP

#include "rBuild.hpp"

#include "rRenderer.hpp"
#include "rSize.hpp"
#include "rPoint.hpp"

#include "ruiStyle.hpp"

#include "rEngine.hpp"

#include "ui/ruiInterface.hpp"
#include "ui/ruiStyleManager.hpp"
#include "ui/ruiWidgetBase.hpp"

#include "ui/ruiDefaultEventHandler.hpp"


class RECONDITE_API ruiWidget : public ruiWidgetBase, public ruiDefaultEventHandler {
public:
	ruiWidget(const rString& id, rEngine* engine);
	ruiWidget(const rString& id, rEngine* engine, const rPoint& position);

	virtual rRect BoundingBox() const;

	virtual void Draw(rEngine& engine) = 0;

	virtual rPoint Position() const;
	void SetPosition(int x, int y);

protected:
	rPoint m_position;
};

#endif
