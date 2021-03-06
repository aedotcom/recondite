#include "reProjectExplorer.hpp"

reProjectExplorer::reProjectExplorer(reComponent* component, wxWindow* parent, wxWindowID id)
	:wxDataViewTreeCtrl(parent, id)
{
	m_component = component;

	Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &reProjectExplorer::OnItemActivated, this);
	Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &reProjectExplorer::OnContext, this);
}

void reProjectExplorer::ShowProject(){
	DeleteAllItems();

	m_projectRoot = AppendContainer(wxDataViewItem(0), m_component->GetProject()->Name());
	m_levelsRoot = AppendContainer(m_projectRoot, "Levels");
	Expand(m_projectRoot);

	const wxArrayString& levels = m_component->GetProject()->Levels();

	for (auto& level : levels){
		AppendItem(m_levelsRoot, level);
	}
}

void reProjectExplorer::AddLevel(const wxString& name){
	wxDataViewItem item = AppendItem(m_levelsRoot, name);
	Select(item);
	
}

void reProjectExplorer::OnItemActivated(wxDataViewEvent& event){
	wxDataViewItem target = event.GetItem();
	wxDataViewItem parent = GetModel()->GetParent(target);

	if (parent == m_levelsRoot){
		m_component->GetProject()->ActivateLevel(GetItemText(target));
	}
}

void reProjectExplorer::OnContext(wxDataViewEvent& event){
	wxDataViewItem target = event.GetItem();
	wxDataViewItem parent = GetModel()->GetParent(target);

	if (parent == m_levelsRoot){
		LevelContextMenu(target);
	}
}

void reProjectExplorer::LevelContextMenu(wxDataViewItem target){
	wxMenu levelContextMenu;
	levelContextMenu.Append(reProjectExplorerRenameLevel, "Rename level...");
	levelContextMenu.Append(reProjectExplorerDeleteLevel, "Delete level...");

	int selection = GetPopupMenuSelectionFromUser(levelContextMenu);

	switch (selection){
		case reProjectExplorerRenameLevel:
			RenameLevel(target);
			break;

		case reProjectExplorerDeleteLevel:
			DeleteLevel(target);
			break;
	}
}

void reProjectExplorer::RenameLevel(wxDataViewItem target){
	wxString oldName = GetItemText(target);
	wxString newName = wxGetTextFromUser("Enter new name:", "Rename " + oldName);

	if (m_component->GetProject()->RenameLevel(oldName, newName)){
		SetItemText(target, newName);
	}
}

void reProjectExplorer::DeleteLevel(wxDataViewItem target){
	wxString levelName = GetItemText(target);

	int choice = wxMessageBox("Really delete " + levelName + "?", "Delete " + levelName, wxOK | wxCANCEL | wxICON_QUESTION | wxCENTRE);
	if (choice != wxOK) return;

	bool result = m_component->GetProject()->DeleteLevel(levelName);
	if (result) DeleteItem(target);
	
}