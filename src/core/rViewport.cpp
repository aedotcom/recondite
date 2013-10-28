#include "rViewport.hpp"

rViewport::rViewport(rViewportType type){
	m_type = type;
	m_camera = NULL;
	
	m_rect.Set(0,0,0,0);
	SetClipping(1.0f, 100.0f);
}

void rViewport::SetNearClip(float nearClip){
	m_nearClip = nearClip;
}

float rViewport::NearClip() const{
	return m_nearClip;
}

void rViewport::SetFarClip(float farClip){
	m_farClip = farClip;
}

float rViewport::FarClip() const{
	return m_farClip;
}

void rViewport::SetClipping(float near, float far){
	m_farClip = far;
	m_nearClip = near;
}

int rViewport::GetSelectionRay(const rPoint& pos , rRay3& selectionRay) const{
    rMatrix4 projection, model;
    GetProjectionMatrix(projection);
    GetViewMatrix(model);
    
    rVector3 nearPoint , farPoint;
    int result;
    

    rVector3 source((float)pos.x , (float)pos.y , 0.0f);
    rMatrixUtil::Unproject(source, model, projection, m_rect, nearPoint);
	
	if (result == false)
		return false;

    source.z = 1.0;
    rMatrixUtil::Unproject(source, model, projection, m_rect, farPoint);
	
	if (result == false)
		return false;

    rVector3 direction = farPoint - nearPoint;
	direction.Normalize();

	selectionRay.Set(nearPoint, direction);
	return true;
}

void rViewport::GetProjectionMatrix(rMatrix4& matrix) const{
	switch (m_type){
	case rVIEWPORT_PERSP:
		rMatrixUtil::Perspective(45.0f, (float)m_rect.width / m_rect.height, m_nearClip, m_farClip, matrix);
		break;
		
	case rVIEWPORT_ORTHO:
		rMatrixUtil::Ortho(m_rect.Left(), m_rect.Right(), m_rect.Bottom(), m_rect.Top(), m_nearClip, m_farClip, matrix);
		break;
		
	case rVIEWPORT_2D:
		rMatrixUtil::Ortho2D(m_rect.Left(), m_rect.Right(), m_rect.Bottom(), m_rect.Top(), matrix);
		break;
	};
}

void rViewport::GetViewMatrix(rMatrix4& matrix) const{
	if (m_camera)
		rMatrixUtil::LookAt(m_camera->Position(), m_camera->Target(), m_camera->Up(), matrix);
}

rCamera* rViewport::Camera() const{
	return m_camera;
}

void rViewport::SetCamera(rCamera* camera){
	m_camera = camera;
}

void rViewport::SetViewportType(rViewportType type){
	m_type = type;
}

rViewportType rViewport::ViewportType() const{
	return m_type;
}


void rViewport::SetSize(int width , int height){
	m_rect.SetSize(width, height);
}

void rViewport::SetSize(const rSize& size){
	m_rect.SetSize(size);
}

void rViewport::SetPosition(int x, int y){
	m_rect.SetPosition(x,y);
}

void rViewport::SetPosition(const rPoint& point){
	m_rect.SetPosition(point);
}

rRect rViewport::GetScreenRect() const{
	return m_rect;
}

void rViewport::SetScreenRect(const rRect& rect){
	m_rect = rect;
}

void rViewport::SetScreenRect(int x, int y, int width, int height){
	m_rect.Set(x, y, width, height);
}
