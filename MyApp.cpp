#include "MyApp.h"


double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

// konstruktor
MyApp::MyApp(void)
{
	inmenu = true;
}
 
// destruktor
MyApp::~MyApp(void)
{
}

// funkcja tworz¹ca scenê
void MyApp::createScene(void)
{
	/* initialize random seed: */
	srand ( time(NULL) );
	// Set the scene's ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
	mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

	Ogre::Entity* terrain = mSceneMgr->createEntity("terrain", "teren.002.mesh");
	Ogre::SceneNode* terrainNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("TerrainNode");
	terrainNode->attachObject(terrain);
	terrainNode->yaw(Ogre::Radian(0.5*M_PI));
	terrainNode->scale(8,2,4);
	terrainNode->translate(0,-3,0);

	Ogre::ColourValue fadeColour(0.9, 0.9, 0.9);	
	mSceneMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 50, 500);
	
	Ogre::Entity* pingu = mSceneMgr->createEntity("pingu", "pingu.mesh");
	Ogre::SceneNode* pinguNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("PinguNode");
	pinguNode->attachObject(pingu);
	pinguNode->scale(0.5,0.5,0.5);
	pinguNode->setPosition(0.0, 2.6, 0.0);
	pinguNode->yaw(Ogre::Radian(M_PI));
	
 
	Ogre::SceneNode* trapsNode =  mSceneMgr->getRootSceneNode()->createChildSceneNode("trapsNode");

	double dist = -20.0;
	string name;
	for(int i=0; i<32;i+=4)
	{
		name = "snowman";
		name += (i+48);
		traps[i] = mSceneMgr->createEntity(name, "snowman.mesh");
		name = "TrapNode";
		name += (i+48);
		trapNode[i] = trapsNode->createChildSceneNode(name);
		trapNode[i]->attachObject(traps[i]);
		trapNode[i]->setPosition((fRand(0.0,100.0)) - 50, 4.0, dist);
		dist -= 20;
		name = "tree";
		name += (i+48);
		traps[i+1] = mSceneMgr->createEntity(name, "choinka.002.mesh");
		name = "TrapNode";
		name += (i+49);
		trapNode[i+1] = trapsNode->createChildSceneNode(name);
		trapNode[i+1]->attachObject(traps[i+1]);
		trapNode[i+1]->setPosition((fRand(0.0,100.0)) - 50, 2.0, dist);
		dist -= 20;
		name = "fence";
		name += (i+48);
		traps[i+2] = mSceneMgr->createEntity(name, "plotek.mesh");
		name = "TrapNode";
		name += (i+50);
		trapNode[i+2] = trapsNode->createChildSceneNode(name);
		trapNode[i+2]->attachObject(traps[i+2]);
		trapNode[i+2]->setPosition((fRand(0.0,100.0)) - 50, 2.0, dist);
		dist -= 20;
		name = "stone";
		name += (i+48);
		traps[i+3] = mSceneMgr->createEntity(name, "stone.mesh");
		name = "TrapNode";
		name += (i+51);
		trapNode[i+3] = trapsNode->createChildSceneNode(name);
		trapNode[i+3]->attachObject(traps[i+3]);
		trapNode[i+3]->setPosition((fRand(0.0,100.0)) - 50, 3.0, dist);
		dist -= 20;
	}
	// Create a Light and set its position
	Ogre::Light* light = mSceneMgr->createLight("MainLight");
	light->setPosition(20.0f, 200.0f, 0.0f);

	Ogre::Plane plane;
	plane.d = 1000;
	plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;	

	mSceneMgr->setSkyPlane(true, plane, "mySky", 1500, 40, true, 1.5f, 150, 150);
	
	OverlayManager& overlayManager = OverlayManager::getSingleton();
	unsigned int screenX = mWindow->getWidth();
	unsigned int screenY = mWindow->getHeight();
	// Panel
	OverlayContainer* panel = static_cast<OverlayContainer*>(
		overlayManager.createOverlayElement("Panel", "GUI"));
	panel->setMetricsMode(Ogre::GMM_PIXELS);
	panel->setPosition(0, 0);
	panel->setDimensions(screenX, screenY);
 
	// Pole tekstowe z dystansem w metrach
	TextAreaOverlayElement* distText = static_cast<TextAreaOverlayElement*>(
		overlayManager.createOverlayElement("TextArea", "distance"));
	distText->setMetricsMode(Ogre::GMM_PIXELS);
	distText->setPosition(screenX/2-50, 10);
	distText->setDimensions(200, 200);
	distText->setCaption("0m");
	distText->setCharHeight(48);
	distText->setFontName("PaintedFont");
	distText->setColourBottom(ColourValue(0.0, 0.0, 0.0));
	distText->setColourTop(ColourValue(0.2, 0.2, 0.2));
	distText->hide();

	// Pole tekstowe z czasem
	TextAreaOverlayElement* timerText = static_cast<TextAreaOverlayElement*>(
		overlayManager.createOverlayElement("TextArea", "time"));
	timerText->setMetricsMode(Ogre::GMM_PIXELS);
	timerText->setPosition(10, 10);
	timerText->setDimensions(200, 200);
	timerText->setCaption("0:00.000");
	timerText->setCharHeight(32);
	timerText->setFontName("PaintedFont");
	timerText->setColourBottom(ColourValue(0.0, 0.0, 0.0));
	timerText->setColourTop(ColourValue(0.2, 0.2, 0.2));
	timerText->hide();
	
	OverlayElement* menuBg = overlayManager.createOverlayElement("BorderPanel", "menu");
	
	menuBg->setMetricsMode(Ogre::GMM_PIXELS);
	menuBg->setPosition(0,0);
	menuBg->setDimensions(screenX, screenY);
	menuBg->setMaterialName("Menu");
	
 
	// Create an overlay, and add the panel
	Overlay* overlay = overlayManager.create("panel");
	overlay->add2D(panel);
 
	// Add the text area to the panel
	panel->addChild(distText);
	panel->addChild(timerText);
 
	panel->addChild(menuBg);

	// Show the overlay
	overlay->show();
}

void MyApp::createCamera(void)
{
	// Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // Position it at 500 in Z direction
    mCamera->setPosition(Vector3(0,10,20));
    // Look back along -Z
    mCamera->lookAt(Vector3(0,0,-300));
    mCamera->setNearClipDistance(1);
}

void MyApp::createFrameListener(void)
{
	mFrameListener = new MyFrameListener(mWindow, mCamera, mSceneMgr,(bool*)&inmenu);
	mFrameListener->showDebugOverlay(true);
    mRoot->addFrameListener(mFrameListener);
}
