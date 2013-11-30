#define _USE_MATH_DEFINES

#include <ExampleApplication.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>
#include <OgreViewport.h>
#include <cmath>
#include <string>
#include <time.h>
#include "MenuFrameListener.h"

using std::string;


class MyMenu : public ExampleApplication
{
public:
	Ogre::Entity* traps[32];
	Ogre::SceneNode* trapNode[32];
	bool *exiter;
	MyMenu();
	MyMenu(bool* end);
	~MyMenu();
	
	void createScene(void);
	void createCamera(void);
	void createFrameListener(void);
	bool frameRenderingQueued(const FrameEvent& evt);
};