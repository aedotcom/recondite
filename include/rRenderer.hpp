#ifndef R_RENDERER_HPP
#define R_RENDERER_HPP

#include "rDefs.hpp"
#include "rTypes.hpp"

#include "rViewport.hpp"
#include "rGraphicsDevice.hpp"
#include "rContentManager.hpp"

#include "rGeometryUtil.hpp"
#include "data/rGeometryData.hpp"
#include "rMatrixUtil.hpp"

class rRenderer {
public:
	rRenderer(rGraphicsDevice* graphicsDevice, rContentManager* contentManager);
	
public:
	void Render (rViewport& viewport);
	
	void RenderGeometry(rGeometry* geometry, const rMatrix4& transform, const rString& elementBufferName, rMaterial* material);
	
	void RenderRect(const rRect& rect, const rColor& color);
	void RenderRect(const rRect& rect, rTexture2D* texture);

private:
	void ComputeWorldSpaceTransformForObject(const rMatrix4& object, rMatrix4& world);
	
	void CreateRequiredMaterials();

private:
		rGraphicsDevice* m_graphicsDevice;
		rContentManager* m_contentManager;
		rViewport* m_activeViewport;
};

#endif