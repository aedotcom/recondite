#ifndef RUI_CHECKBOX_HPP
#define RUI_CHECKBOX_HPP

#include "rTypes.hpp"
#include "rDefs.hpp"

#include "ruiWidget.hpp"

class ruiCheckbox : public ruiWidget{
public:
	ruiCheckbox(int id, const rPoint& position, const rSize& size);
	
public:
	
	virtual void OnTouchDown(const rTouch& touch);

	bool IsChecked () const;
	void SetChecked(bool checked);
	
	virtual void Draw(rEngine& engine);
	
private:
	bool m_isChecked;
};


#endif