#include <ExampleFrameListener.h>

class MenuFrameListener : public ExampleFrameListener
{
public:
	bool *exitgame;

	MenuFrameListener(RenderWindow* win, Camera* cam, SceneManager *SceneMgr, bool * exit, bool bufferedKeys = false, bool bufferedMouse = false,
			     bool bufferedJoy = false ) 
			: ExampleFrameListener(win, cam)
	{
		mSceneMgr = SceneMgr;
		exitgame = exit;
	}

	bool frameRenderingQueued(const FrameEvent& evt);
	bool processUnbufferedInput(const FrameEvent& evt);

protected:
	SceneManager *mSceneMgr;
};