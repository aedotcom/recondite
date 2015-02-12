#include "reProject.hpp"

void reProject::Create(const wxString& directory, const wxString& name){
	Close();

	m_name = name;

	m_projectDir.AssignDir(directory);
	m_projectDir.AppendDir(name);

	wxMkDir(m_projectDir.GetPath());

	SaveProjectFile();
}

bool reProject::Open(const wxString& path){
	Close();

	wxFileName projPath(path);
	wxString projectDir = projPath.GetPath();

	wxXmlDocument doc(path);

	wxXmlNode* child = doc.GetRoot()->GetChildren();

	while (child){
		if (child->GetName() == "name") {
			m_name = child->GetNodeContent();
		}

		child = child->GetNext();
	}

	return true;
}

void reProject::Close(){
	if (!IsOpen())
		return;

	SaveProjectFile();

	m_projectDir.Clear();
	m_name.Clear();
}

void reProject::SaveProjectFile(){
	wxXmlNode* root = new wxXmlNode(wxXML_ELEMENT_NODE, "project");
	wxXmlNode* nameElement = new wxXmlNode(root, wxXML_ELEMENT_NODE, "name");
	wxXmlNode* nameNode = new wxXmlNode(nameElement, wxXML_TEXT_NODE, "name", m_name);

	wxXmlDocument document;
	document.SetRoot(root);

	wxString projFile = ProjectFilePath();
	document.Save(projFile);
}

wxString reProject::Directory() const{
	return m_projectDir.GetPath();
}

wxString reProject::Name() const{
	return m_name;
}

wxString reProject::ProjectFilePath() const{
	return m_projectDir.GetPathWithSep() + m_name + ".rprj";
}

bool reProject::IsOpen() const{
	return !m_name.IsEmpty();
}