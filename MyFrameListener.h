#include <ExampleFrameListener.h>

double fRand(double fMin, double fMax);

class MyFrameListener : public ExampleFrameListener
{
public:
	bool* inmenu;

	MyFrameListener(RenderWindow* win, Camera* cam, SceneManager *SceneMgr, bool* menu, bool bufferedKeys = false, bool bufferedMouse = false,
			     bool bufferedJoy = false );

	bool frameRenderingQueued(const FrameEvent& evt);
	bool processUnbufferedInput(const FrameEvent& evt);

protected:
	SceneManager *mSceneMgr;
	AnimationState* animationState;
};