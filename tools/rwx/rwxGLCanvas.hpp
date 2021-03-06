#ifndef RWX_GLCANVAS_HPP
#define RWX_GLCANVAS_HPP

//note this must be included before GL canvas because glew.h must be included before gl.h
#include "rwxComponent.hpp"

#include "wx/wx.h"
#include "wx/glcanvas.h"

#include "recondite.hpp"

class RECONDITE_RWX_CLASS rwxGLCanvas : public wxGLCanvas{
public:
	rwxGLCanvas(rwxComponent* component, rCamera* camera, const wxString& name, wxWindow *parent, wxWindowID id = wxID_ANY);
	void OnPaint(wxPaintEvent& event);

	rViewport* GetViewport();
	rCamera* GetCamera();
	void SetCamera(rCamera* camera);

private:
	void CreateViewport();

private:
	rwxComponent* m_component;

	wxString m_name;
	rViewport* m_viewport;
	rCamera* m_camera;
};

#endif