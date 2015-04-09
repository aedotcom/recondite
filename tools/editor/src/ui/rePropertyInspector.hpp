#ifndef RE_PROPERTYINSPECTOR_HPP
#define RE_PROPERTYINSPECTOR_HPP

#include "project/reComponent.hpp"

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/clntdata.h>
#include <wx/tokenzr.h>

#include "reViewportDisplay.hpp"
#include "rePropertyInterface.hpp"

class rePropertyInspector : public wxPropertyGrid{
public:
	rePropertyInspector(reComponent* component, reViewportDisplay* display, wxWindow* parent, wxWindowID id = wxID_ANY);

public:
	void Inspect(const wxString& actorName);
	void StopInspecting();

private:
	void OnPropertyValueChanged(wxPropertyGridEvent& event);

	void OnLevelBeginLoad(rEvent& event);
	void OnSelection(rEvent& event);
	void OnSelectNone(rEvent& event);

private:
	rwxComponent* m_component;
	reViewportDisplay* m_display;

	rActor3* m_actor;
};

#endif