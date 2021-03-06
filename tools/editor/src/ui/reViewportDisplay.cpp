#include "reViewportDisplay.hpp"

reViewportDisplay::reViewportDisplay(reComponent* component, reToolManager* toolManager, wxWindow* parent, wxWindowID id)
	:wxPanel(parent, id)
{
	m_component = component;
	m_toolManager = toolManager;

	m_activeViewport = nullptr;

	CreateViewportDisplay();

	m_component->Bind(rEVT_COMPONENT_INITIALIZED, this, &reViewportDisplay::OnComponentInitialized);
}

reViewport* reViewportDisplay::GetActiveViewport(){
	return m_activeViewport;
}

void reViewportDisplay::OnViewportActivate(wxMouseEvent& event){
	int id = event.GetId();

	if (id == m_topLeftViewport->GetCanvas()->GetId())
		m_activeViewport = m_topLeftViewport;

	else if (id == m_topRightViewport->GetCanvas()->GetId())
		m_activeViewport = m_topRightViewport;

	else if (id == m_bottomLeftViewport->GetCanvas()->GetId())
		m_activeViewport = m_bottomLeftViewport;

	else if (id == m_bottomRightViewport->GetCanvas()->GetId())
		m_activeViewport = m_bottomRightViewport;

	else
		m_activeViewport = nullptr;

	event.Skip();
}

void reViewportDisplay::BindCanvasEvents(rwxGLCanvas* canvas){
	canvas->Bind(wxEVT_LEFT_DOWN, &reViewportDisplay::OnViewportActivate, this);
	canvas->Bind(wxEVT_MIDDLE_DOWN, &reViewportDisplay::OnViewportActivate, this);
	canvas->Bind(wxEVT_RIGHT_DOWN, &reViewportDisplay::OnViewportActivate, this);

	canvas->SetDropTarget(new rePaletteDropTarget(m_component, canvas));
}

void reViewportDisplay::CreateViewportDisplay(){
	m_mainSplitter = new wxSplitterWindow(this, wxID_ANY);
	m_mainSplitter->SetSashGravity(0.5);

	m_leftSplitter = new wxSplitterWindow(m_mainSplitter, wxID_ANY);
	m_leftSplitter->SetSashGravity(0.5);

	m_topLeftViewport = new reViewport(m_component, m_toolManager, this, "Viewport 1", m_leftSplitter);
	m_bottomLeftViewport = new reViewport(m_component, m_toolManager, this, "Viewport 2", m_leftSplitter);
	m_leftSplitter->SplitHorizontally(m_topLeftViewport, m_bottomLeftViewport);

	m_rightSplitter = new wxSplitterWindow(m_mainSplitter, wxID_ANY);
	m_rightSplitter->SetSashGravity(0.5);

	m_topRightViewport = new reViewport(m_component, m_toolManager, this, "Viewport 3", m_rightSplitter);
	m_bottomRightViewport = new reViewport(m_component, m_toolManager, this, "Viewport 4", m_rightSplitter);
	m_rightSplitter->SplitHorizontally(m_topRightViewport, m_bottomRightViewport);

	m_mainSplitter->SplitVertically(m_leftSplitter, m_rightSplitter);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(m_mainSplitter, 1, wxEXPAND | wxALL, 2);
	SetSizer(mainSizer);
}

reViewport* reViewportDisplay::GetViewport(const wxString& name){
	if (name == m_topLeftViewport->GetViewportName())
		return m_topLeftViewport;
	else if (name == m_topRightViewport->GetViewportName())
		return m_topRightViewport;
	else if (name == m_bottomLeftViewport->GetViewportName())
		return m_bottomLeftViewport;
	else if (name == m_bottomRightViewport->GetViewportName())
		return m_bottomRightViewport;
	else
		return nullptr;
}

void reViewportDisplay::OnComponentInitialized(rEvent& event){
	rScene* scene = m_component->GetScene();

	scene->Bind(rEVT_SCENE_ACTOR_ADDED, this, &reViewportDisplay::OnDisplayShouldUpdate);
	scene->Bind(rEVT_SCENE_ACTOR_REMOVED, this, &reViewportDisplay::OnDisplayShouldUpdate);
	scene->Bind(rEVT_SCENE_LOAD_END, this, &reViewportDisplay::OnDisplayShouldUpdate);

	m_component->Bind(reCommandProcessed, this, &reViewportDisplay::OnCommandProcessed);

	BindCanvasEvents(m_topLeftViewport->GetCanvas());
	BindCanvasEvents(m_topRightViewport->GetCanvas());
	BindCanvasEvents(m_bottomLeftViewport->GetCanvas());
	BindCanvasEvents(m_bottomRightViewport->GetCanvas());
}

void reViewportDisplay::UpdateAllViewports(){
	m_topLeftViewport->GetCanvas()->Refresh();
	m_topRightViewport->GetCanvas()->Refresh();
	m_bottomLeftViewport->GetCanvas()->Refresh();
	m_bottomRightViewport->GetCanvas()->Refresh();
}

void reViewportDisplay::OnDisplayShouldUpdate(rEvent& event){
	UpdateAllViewports();
}

void reViewportDisplay::OnCommandProcessed(rEvent& event){
	UpdateAllViewports();
}