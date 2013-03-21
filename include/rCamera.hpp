#ifndef R_CAMERA_HPP
#define R_CAMERA_HPP

#include "rTypes.hpp"
#include "rActor3.hpp"

class rCamera : public rActor3{
public:
	rCamera(const rString& name , const rVector3& position);

	virtual rVector3 Target() const = 0;
protected:

	rVector3 m_target;
};

class rViewCamera : public rCamera{
public:
	rViewCamera(const rString& name , const rVector3& position);
	virtual int Update(const rGameTime& gameTime);

	virtual rVector3 Target() const;
};

#endif