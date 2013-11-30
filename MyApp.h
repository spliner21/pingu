#define _USE_MATH_DEFINES

#include <ExampleApplication.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>
#include <OgreViewport.h>
#include <cmath>
#include <string>
#include <time.h>
#include "MyFrameListener.h"

using std::string;


class MyApp : public ExampleApplication
{
public:
	Ogre::Entity* traps[32];
	Ogre::SceneNode* trapNode[32];
	bool inmenu;
	MyApp();
	~MyApp();
	
	void createScene(void);
	void createCamera(void);
	void createFrameListener(void);
	bool frameRenderingQueued(const FrameEvent& evt);
};