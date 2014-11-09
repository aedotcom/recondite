#ifndef R_PRIMITIVEBOX_HPP
#define R_PRIMITIVEBOX_HPP

#include "rBuild.hpp"
#include "rAlignedBox3.hpp"

#include "primitive/rPrimitive.hpp"

class RECONDITE_API rPrimitiveBox : public rPrimitive{
public:
	rPrimitiveBox(const rString& id, rEngine* engine);

public:

	virtual rString ClassName() const;

	float Width() const;
	void SetWidth(float width);

	float Height() const;
	void SetHeight(float height);

	float Depth() const;
	void SetDepth(float depth);

protected:
	virtual void CreateGeometry();

private:

	float m_width;
	float m_height;
	float m_depth;
};

#endif