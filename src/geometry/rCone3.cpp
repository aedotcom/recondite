#include "rCone3.hpp"

rCone3::rCone3(const rVector3& p , const rVector3& d , float h , float a){
	Set(p , d , h, a);
}

float rCone3::CapRadius() const{
	return std::tan(angle) * height;
}

rVector3 rCone3::Base() const{
	return point + (direction * height);
}

void rCone3::Set(const rVector3& p , const rVector3& d , float h , float a){
	point = p;
	direction = d;
	height = h;
	angle = a;
}