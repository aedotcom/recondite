#ifndef R_MATH_UTILS_HPP
#define R_MATH_UTILS_HPP

#include <algorithm>

#ifndef M_PI
	#define M_PI 3.141592653589793238462643f
#endif

#define rMATH_ZERO_TOLERANCE 1e-06f

#include "rTypes.hpp"
#include "rRectangle2.hpp"
#include "rCircle2.hpp"
#include "rMatrix3.hpp"
#include "rLine2.hpp"

#include "rVector3.hpp"

namespace rMath{
	
	float DegreeToRad(float deg);
	
	float Max3(float n1 , float n2, float n3);

	void Matrix3TransformRectangle(const rMatrix3& m , rRectangle2& r);
	void Matrix3TransformCircle(const rMatrix3& m , rCircle2& c);

	rVector2 ClosestPointOnSegment(const rLineSegment2& ls , const rCircle2& c);

	bool PointInBoundedXYPlane(const rVector3& corner1 , const rVector3& corner2 , const rVector3& point);
	bool PointInBoundedXZPlane(const rVector3& corner1 , const rVector3& corner2 , const rVector3& point);
	bool PointInBoundedYZPlane(const rVector3& corner1 , const rVector3& corner2 , const rVector3& point);
};

#endif