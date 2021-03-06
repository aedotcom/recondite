#include "rGeometry_Test_Common.hpp"

void InitBasicFrustrum(rFrustrum& f, float scale){
	f.pNear.SetFromPointAndNormal(rVector3::BackwardVector * scale, rVector3::ForwardVector);
	f.pFar.SetFromPointAndNormal(rVector3::ForwardVector * scale, rVector3::BackwardVector);
	f.pLeft.SetFromPointAndNormal(rVector3::LeftVector * scale, rVector3::RightVector);
	f.pRight.SetFromPointAndNormal(rVector3::RightVector * scale, rVector3::LeftVector);
	f.pTop.SetFromPointAndNormal(rVector3::UpVector * scale, rVector3::DownVector);
	f.pBottom.SetFromPointAndNormal(rVector3::DownVector * scale, rVector3::UpVector);

}