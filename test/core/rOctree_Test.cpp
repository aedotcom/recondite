#include "gtest/gtest.h"
#include "container/rOctree.hpp"
#include "rActor3.hpp"

class Core_rOctree : public ::testing::Test{
};

class rTestActor3 : public rActor3{
public:

	rTestActor3(const rString& name , const rVector3& position);
	virtual int Update(rEngine& engine);
};

rTestActor3::rTestActor3(const rString& name , const rVector3& position):rActor3(name, position){}
int rTestActor3::Update(rEngine& engine){return 0;}

TEST(Core_rOctree, Clear){
	rOctree<rTestActor3*> octree;
	octree.Clear();
}