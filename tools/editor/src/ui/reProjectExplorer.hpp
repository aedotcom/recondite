#ifndef RE_PROJECTEXPLORER_HPP
#define RE_PROJECTEXPLORER_HPP

#include "rwxComponent.hpp"

#include <wx/wx.h>
#include <wx/dataview.h>

#include "project/reProject.hpp"

class reProjectExplorer : public wxDataViewTreeCtrl{
public:
	reProjectExplorer(rwxComponent* component, reProject* project, wxWindow* parent, wxWindowID id = wxID_ANY);

	void ShowProject();
	void AddLevel(const wxString& name);

private:
	void OnItemActivated(wxDataViewEvent& event);
	void OnContext(wxDataViewEvent& event);

private:
	enum reProjectExplorerId{
		reProjectExplorerRenameLevel = 6000,
		reProjectExplorerDeleteLevel
	};

	void LevelContextMenu(wxDataViewItem target);
	void RenameLevel(wxDataViewItem target);

private:
	rwxComponent* m_component;
	reProject* m_project;

	wxDataViewItem m_projectRoot;
	wxDataViewItem m_levelsRoot;
};

#endif