#ifndef R_APPLICATION_HPP
#define R_APPLICATION_HPP

#include "rBuild.hpp"

#include "rComponent.hpp"

#include "rEngine.hpp"
#include "rModule.hpp"
#include "rGraphicsDevice.hpp"
#include "rInputManager.hpp"
#include "rScene.hpp"

class RECONDITE_API rApplicationBase : public rComponent{
public:
	virtual void Update();
	virtual void Draw();

public:
	rApplicationBase();
	virtual ~rApplicationBase();

	virtual void Uninit();

	virtual void Tick();

	virtual size_t TargetFPS() const;
	void SetTargetFPS(unsigned int targetFPS);

	void SetDisplaySize(int width, int height);
	virtual rSize DisplaySize() const;

	virtual size_t FrameCount() const;

	

public:
	virtual rModule* CreateModule() = 0;

protected:
	virtual void InitEngine(rGraphicsDevice* graphics, rContentManager* content, rInputManager* input);
	void InitModule();

protected:
	unsigned int m_targetFPS;
	size_t m_frameCount;

	
	rModule* m_module;

	rSize m_displaySize;

	
};

#endif
