#include "MyMenu.h"

// konstruktor
MyMenu::MyMenu(void)
{
}
MyMenu::MyMenu(bool* end)
{
	exiter=end;
}
 
// destruktor
MyMenu::~MyMenu(void)
{
}

// funkcja tworz¹ca menu
void MyMenu::createScene(void)
{	
	OverlayManager& overlayManager = OverlayManager::getSingleton();
	unsigned int screenX = mWindow->getWidth();
	unsigned int screenY = mWindow->getHeight();
	// Panel
	OverlayContainer* panel = static_cast<OverlayContainer*>(
		overlayManager.createOverlayElement("Panel", "Menu"));
	panel->setMetricsMode(Ogre::GMM_PIXELS);
	panel->setPosition(0, 0);
	panel->setDimensions(screenX, screenY);

	OverlayElement* menuBg = overlayManager.createOverlayElement("BorderPanel", "menu");
	
	menuBg->setMetricsMode(Ogre::GMM_PIXELS);
	menuBg->setPosition(0,0);
	menuBg->setDimensions(screenX, screenY);
	menuBg->setMaterialName("Menu");

	
	Overlay* overlay = overlayManager.create("panel");
	overlay->add2D(panel);
 
	// Add the text area to the panel
	panel->addChild(menuBg);
 
	// Show the overlay
	overlay->show();
}

void MyMenu::createCamera(void)
{
	// Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // Position it at 500 in Z direction
    mCamera->setPosition(Vector3(0,10,20));
    // Look back along -Z
    mCamera->lookAt(Vector3(0,0,-300));
    mCamera->setNearClipDistance(1);
}

void MyMenu::createFrameListener(void)
{
	mFrameListener = new MenuFrameListener(mWindow, mCamera, mSceneMgr,exiter);
	mFrameListener->showDebugOverlay(true);
    mRoot->addFrameListener(mFrameListener);
}
