#include "rIntersection.hpp"

bool rIntersection::AlignedBoxIntersectsSphere(const rAlignedBox3& box , const rSphere& sphere){
	rVector3 r;

	if (sphere.center.x < box.min.x) {
		r.x =  box.min.x;
	} else if (sphere.center.x >  box.max.x) {
		r.x =  box.max.x;
	} else {
		r.x = sphere.center.x;
	}

	if (sphere.center.y < box.min.y) {
		r.y =  box.min.y;
	} else if (sphere.center.y >  box.max.y) {
		r.y =  box.max.y;
	} else {
		r.y = sphere.center.y;
	}

	if (sphere.center.z <  box.min.z) {
		r.z =  box.min.z;
	} else if (sphere.center.z >  box.max.z) {
		r.z =  box.max.z;
	} else {
		r.z = sphere.center.z;
	}

	return r.Distance(sphere.center) <= sphere.radius;
}

bool rIntersection::SphereContainsAlignedBox(const rSphere& sphere, const rAlignedBox3& box){
	return sphere.ContainsPoint(box.max) && sphere.ContainsPoint(box.min);
}

bool rIntersection::AlignedBoxContainsSphere(const rAlignedBox3& box , const rSphere& sphere){
	return	sphere.center.x + sphere.radius <= box.max.x &&
			sphere.center.y + sphere.radius <= box.max.y &&
			sphere.center.z + sphere.radius <= box.max.z &&

			sphere.center.x - sphere.radius >= box.min.x &&
			sphere.center.y - sphere.radius >= box.min.y &&
			sphere.center.z - sphere.radius >= box.min.z;
}

bool rIntersection::RayIntersectsPlane(const rRay3& ray , const rPlane& plane, rVector3* point){
	float dist_dot_normal = plane.normal.Dot(ray.direction);
	float signedDistance =  plane.DistanceToPoint(ray.origin);
	float rayParam = -signedDistance / dist_dot_normal;

	if (rayParam >= 0 && std::fabs(dist_dot_normal) > 0.0f){
		if (point){
			rVector3 tv = ray.direction * rayParam;
			point->Set(ray.origin.x + tv.x , ray.origin.y + tv.y, ray.origin.z + tv.z);
		}

		return true;
	}

	if (rayParam >= 0 && std::fabs(signedDistance) <= 0.0f){
		if (point)
			*point = ray.origin;
		
		return true;
	}
	

	return false;
}

bool rIntersection::RayIntersectsSphere(const rRay3& ray , const rSphere& sphere, rVector3* intersectionPoint ){
        // Adjust ray origin relative to sphere center
        const rVector3& rayorig = ray.origin - sphere.center;

		if (rayorig.LengthSquared() <= sphere.radius* sphere.radius) {
            return false;
        }

        // Build coeffs which can be used with std quadratic solver
        // ie t = (-b +/- sqrt(b*b + 4ac)) / 2a
		float a = ray.direction.Dot(ray.direction);
		float b = 2 * rayorig.Dot(ray.direction);
        float c = rayorig.Dot(rayorig) - sphere.radius * sphere.radius;

        // Calc determinant
        float d = (b*b) - (4 * a * c);

        if (d < 0.0f) {// No intersection
            return false;
        }
        else
        {
			float t = ( -b - std::sqrt(d) ) / (2 * a);

			if (t < 0){// We only want the intersection in the direction of the ray : else t = ( -b + std::sqrt(d) ) / (2 * a);
				return false;
			}
			else{
				if (intersectionPoint){
					*intersectionPoint = ray.origin + (ray.direction * t);					
				}

				return true;
			}
        }
}

bool FrustrumPlaneSphereCheck(const rPlane& plane, const rSphere& sphere, int& planeCount){


	return true;
}

int FrustrumSphereTest(const rFrustrum& frustrum, const rSphere& sphere){
	int planeCount = 0;
	float distance;

	distance = frustrum.pNear.DistanceToPoint(sphere.center);
	if (distance <= -sphere.radius) 
		return 0;
	if (distance > sphere.radius)
		planeCount++;

	distance = frustrum.pFar.DistanceToPoint(sphere.center);
	if (distance <= -sphere.radius) 
		return 0;
	if (distance > sphere.radius)
		planeCount++;

	distance = frustrum.pLeft.DistanceToPoint(sphere.center);
	if (distance <= -sphere.radius) 
		return 0;
	if (distance > sphere.radius)
		planeCount++;

	distance = frustrum.pRight.DistanceToPoint(sphere.center);
	if (distance <= -sphere.radius) 
		return 0;
	if (distance > sphere.radius)
		planeCount++;

	distance = frustrum.pTop.DistanceToPoint(sphere.center);
	if (distance <= -sphere.radius) 
		return 0;
	if (distance > sphere.radius)
		planeCount++;

	distance = frustrum.pBottom.DistanceToPoint(sphere.center);
	if (distance <= -sphere.radius) 
		return 0;
	if (distance > sphere.radius)
		planeCount++;

	return planeCount;
}

bool rIntersection::FrustrumContainsSphere(const rFrustrum& frustrum, const rSphere& sphere){
	return FrustrumSphereTest(frustrum, sphere) == 6;
}

bool rIntersection::FrustrumIntersectsSphere(const rFrustrum& frustrum, const rSphere& sphere){
	return FrustrumSphereTest(frustrum, sphere) > 0;
}

bool rIntersection::RayIntersectsAlignedBox(const rRay3& ray, const rAlignedBox3& box, rVector3* intersectionPoint){
	float t0 = 0.0f;
	float t1 = 10000.0f;

	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	float divx = 1 / ray.direction.x;
	if (divx >= 0) {
		tmin = (box.min.x - ray.origin.x) * divx;
		tmax = (box.max.x - ray.origin.x) * divx;
	}
	else {
		tmin = (box.max.x - ray.origin.x) * divx;
		tmax = (box.min.x - ray.origin.x) * divx;
	}

	float divy = 1 / ray.direction.y;
	if (divy >= 0) {
		tymin = (box.min.y - ray.origin.y) * divy;
		tymax = (box.max.y - ray.origin.y) * divy;
	}
	else {
		tymin = (box.max.y - ray.origin.y) * divy;
		tymax = (box.min.y - ray.origin.y) * divy;
	}
	if ( (tmin > tymax) || (tymin > tmax) )
		return false;

	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	float divz = 1 / ray.direction.z;
	if (divz >= 0) {
		tzmin = (box.min.z - ray.origin.z) * divz;
		tzmax = (box.max.z - ray.origin.z) * divz;
	}
	else {
		tzmin = (box.max.z - ray.origin.z) * divz;
		tzmax = (box.min.z - ray.origin.z) * divz;
	}
	if ( (tmin > tzmax) || (tzmin > tmax) )
		return false;

	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	if ( (tmin < t1) && (tmax > t0) ){		if (intersectionPoint) *intersectionPoint = ray.origin + (ray.direction * tmin);		return true;	}
	return false;
}