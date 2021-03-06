#include "tools/reTranslateTool.hpp"

reTranslateTool::reTranslateTool(reComponent* component, wxFrame* owner)
	: reToolBase(component, owner)
{
	m_gizmo.reset(new reTranslateGizmo(component));
	m_command = nullptr;
	m_selectedAxis = reGizmoAxis::NONE;
}

reTranslateTool::~reTranslateTool(){
	if (m_command) delete m_command;
}

bool reTranslateTool::OnMouseDown(wxMouseEvent& event, rwxGLCanvas* canvas){
	reToolBase::OnMouseDown(event, canvas);

	rActor3* actor = PickActor(event, canvas);

	if (actor){
		m_selectedAxis = m_gizmo->GetGizmoAxis(actor);

		if (m_selectedAxis == reGizmoAxis::NONE){
			bool result = DoActorSelection(actor, event);
			m_gizmo->Update();
		}
		else{
			m_gizmo->Update();
			SetDragPlaneFromSelectedAxis();
			GetWorldSpaceDragPosition(canvas, m_previousWorldPosition);
			m_command = new reTranslateCommand(m_component->SelectionManager()->GetSelection(), m_component);
		}

		return true;
	}
	else{
		m_gizmo->SetVisibility(false);
		return DoClearSelection();
	}
}

bool reTranslateTool::OnMouseUp(wxMouseEvent& event, rwxGLCanvas* canvas){
	reToolBase::OnMouseUp(event, canvas);

	if (m_command){
		m_component->SubmitCommand(m_command);

		m_selectedAxis = reGizmoAxis::NONE;
		m_command = nullptr;
		return true;
	}

	return false;
}

bool reTranslateTool::OnMouseMotion(wxMouseEvent& event, rwxGLCanvas* canvas){
	reToolBase::OnMouseMotion(event, canvas);

	if (m_selectedAxis == reGizmoAxis::NONE){
		return false;
	}
	else{
		rVector3 currentWorldSpacePosition;
		GetWorldSpaceDragPosition(canvas, currentWorldSpacePosition);
		rVector3 delta = currentWorldSpacePosition - m_previousWorldPosition;

		switch (m_selectedAxis){
		case reGizmoAxis::X:
			delta.Set(delta.x, 0.0f, 0.0f);
			break;

		case reGizmoAxis::Y:
			delta.Set(0.0f, delta.y, 0.0f);
			break;

		case reGizmoAxis::Z:
			delta.Set(0.0f, 0.0f, delta.z);
			break;
		}

		m_command->Update(delta);
		m_gizmo->Update();
		
		rEvent event;
		m_component->Trigger(reExternalPropertyChange, event);

		m_previousWorldPosition = currentWorldSpacePosition;

		return true;
	}
}

void reTranslateTool::OnActivate(){
	m_gizmo->Update();
}

void reTranslateTool::OnUpdate(){
	m_gizmo->Update();
}

wxString reTranslateTool::GetToolName() const{
	return "Translate Tool";
}

void reTranslateTool::SetDragPlaneFromSelectedAxis(){
	switch (m_selectedAxis){
	case reGizmoAxis::X:
	case reGizmoAxis::Y:
		m_dragPlane.SetFromPointAndNormal(m_gizmo->GetPosition(), rVector3::BackwardVector);
		break;

	case reGizmoAxis::Z:
		m_dragPlane.SetFromPointAndNormal(m_gizmo->GetPosition(), rVector3::RightVector);
		break;
	}
}

bool reTranslateTool::GetWorldSpaceDragPosition(rwxGLCanvas* canvas, rVector3& result){
	rCamera* camera = canvas->GetCamera();
	rViewport* viewport = canvas->GetViewport();

	rPoint pt(m_currentPoint.x, m_currentPoint.y);
	rRay3 selectionRay;
	viewport->GetSelectionRay(pt, selectionRay);
	
	return rIntersection::RayIntersectsPlane(selectionRay, m_dragPlane, &result);
}

