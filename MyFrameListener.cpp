#include "MyFrameListener.h"
#include <sstream>
#include <iomanip>

MyFrameListener::MyFrameListener(RenderWindow* win, Camera* cam, SceneManager *SceneMgr, bool* menu, bool bufferedKeys, bool bufferedMouse,
			     bool bufferedJoy) 
			: ExampleFrameListener(win, cam)
	{
		mSceneMgr = SceneMgr;
		inmenu = menu;
		
		// ustawienie animacji
		animationState = mSceneMgr->getEntity("pingu")->getAnimationState("wave");
		animationState->setLoop(true);
		animationState->setEnabled(true);
	}

std::string int2strInM(int in)
{
	std::string s;
	std::stringstream out;
	out << in;
	s = out.str();
	s += "m";
	return s;
}

std::string timer2str(clock_t begin)
{
	int diffms = (clock()-begin)*1000/CLOCKS_PER_SEC;
	std::string s;
	std::stringstream out;
	out << diffms/60000; // minuty
	out << ":";
	if((diffms%60000)/1000 < 10)
		out << "0";
	out << (diffms%60000)/1000; // sekundy
	out << ".";
	if(diffms%1000 < 10)
		out << "0";
	if(diffms%1000 < 100)
		out << "0";
	out << diffms%1000; // milisekundy
	s = out.str();
	return s;

}

bool ColisionDetector(Ogre::Vector3 obj1, Ogre::Vector3 obj2, int trapCount)
{
	switch(trapCount%4)
	{
	case 0:	// ba³wanek
		if ( (obj1 - obj2).length() < 3.5 )
			return false;
		break;
	case 1:	// choinka
		if ( (obj1 - obj2).length() < 4 )
			return false;
		break;
	case 2:	// p³otek
		if (( abs(obj1.x - obj2.x) < 1 ) && ( abs(obj1.z - obj2.z) < 3 ))
			return false;
		break;
	case 3:	// kamieñ
		if ( (obj1 - obj2).length() < 3 )
			return false;
		break;
	}
	return true;
}

bool MyFrameListener::frameRenderingQueued(const FrameEvent& evt)
{
	//ExampleFrameListener::frameRenderingQueued(evt);		// odkomentuj ¿eby odblokowaæ kamerê
	static const Ogre::Real step = 20;
	static int trapCount = 0;
	static int dist = 0;
	static clock_t begin = clock();
	int trapCount2;
	bool everythingOk = true;
	std::string trapName,trapName2;

	animationState->addTime(evt.timeSinceLastFrame);

	if(mWindow->isClosed())	return false;
	
	mKeyboard->capture();
	//mMouse->capture();
	//Check if one of the devices is not buffered
	if( !mMouse->buffered() || !mKeyboard->buffered() )
	{
		// one of the input modes is immediate, so setup what is needed for immediate movement
		if (mTimeUntilNextToggle >= 0)
			mTimeUntilNextToggle -= evt.timeSinceLastFrame;

		// Move about 100 units per second
		mMoveScale = mMoveSpeed * evt.timeSinceLastFrame;
		// Take about 10 seconds for full rotation
		mRotScale = mRotateSpeed * evt.timeSinceLastFrame;

		mRotX = 0;
		mRotY = 0;
		mTranslateVector = Ogre::Vector3::ZERO;

	}
	if( !mMouse->buffered() )
		if( processUnbufferedMouseInput(evt) == false )
			return false;
	if( !mKeyboard->buffered() )
		if( processUnbufferedInput(evt) == false )
			return false;
	if( !mMouse->buffered() || !mKeyboard->buffered() )
		moveCamera();
	if(!(*inmenu))
	{
		Ogre::Node* pinguNode = mSceneMgr->getRootSceneNode()->getChild("PinguNode");
		//pinguNode->yaw(Ogre::Radian(2.f*evt.timeSinceLastFrame));
		mSceneMgr->getSceneNode("trapsNode")->translate(0,0,step*evt.timeSinceLastFrame, Ogre::Node::TS_PARENT);

		trapName = "TrapNode";
		trapName2 = trapName;
		trapName += (trapCount+48);
		trapCount2 = (trapCount + 1)%32;
		trapName2 += (trapCount2+48);
		// kolizje
		Ogre::Vector3 tmp = mSceneMgr->getSceneNode("trapsNode")->getChild(trapName)->_getDerivedPosition();

		everythingOk = ColisionDetector(tmp,pinguNode->_getDerivedPosition(),trapCount);
		if(!everythingOk)
			return false;
		tmp = mSceneMgr->getSceneNode("trapsNode")->getChild(trapName2)->_getDerivedPosition();
		everythingOk = ColisionDetector(tmp,pinguNode->_getDerivedPosition(),trapCount2);
		if(!everythingOk)
			return false;
	
		// "regeneracja" przeszkód
		if(mSceneMgr->getSceneNode("trapsNode")->getChild(trapName)->_getDerivedPosition().z >20.0)
		{
			switch(trapCount%4)
			{
			case 0:
				mSceneMgr->getSceneNode("trapsNode")->getChild(trapName)->_setDerivedPosition(Ogre::Vector3((fRand(0.0,100.0)) - 50, 4.0, 31*(-20)));
				break;
			case 3:
				mSceneMgr->getSceneNode("trapsNode")->getChild(trapName)->_setDerivedPosition(Ogre::Vector3((fRand(0.0,100.0)) - 50, 3.0, 31*(-20)));
				break;
			default:
				mSceneMgr->getSceneNode("trapsNode")->getChild(trapName)->_setDerivedPosition(Ogre::Vector3((fRand(0.0,100.0)) - 50, 2.0, 31*(-20)));
				break;
			}
			trapCount = (trapCount + 1)%32;
		}


		OverlayManager& overlayManager = OverlayManager::getSingleton();
		overlayManager.getByName("panel")->getChild("GUI")->getChild("distance")->setCaption(int2strInM(dist/35));
		overlayManager.getByName("panel")->getChild("GUI")->getChild("time")->setCaption(timer2str(begin));
		dist++;
		if(!processUnbufferedInput(evt)) return false;
	}
	return true;
}

bool MyFrameListener::processUnbufferedInput(const FrameEvent& evt)	// reakcja na klawisze
{
	static const Ogre::Real step = 5;	// d³ugoœæ kroku
	static bool isleftpressed = false, isrightpressed = false;
	switch(*inmenu)
	{
	case false:
		if (mKeyboard->isKeyDown(OIS::KC_LEFT) && mSceneMgr->getSceneNode("PinguNode")->getPosition().x > (-50.0))
		{
			animationState = mSceneMgr->getEntity("pingu")->getAnimationState("left");
			animationState->setLoop(false);
			animationState->setEnabled(true);
			if (animationState->hasEnded())
			{
				animationState = mSceneMgr->getEntity("pingu")->getAnimationState("wave");
				animationState->setLoop(true);
				animationState->setEnabled(true);
			}
			mSceneMgr->getSceneNode("PinguNode")->translate(-step*evt.timeSinceLastFrame,0,0, Ogre::Node::TS_PARENT);
			mSceneMgr->getCamera("PlayerCam")->move(Ogre::Vector3(-step*evt.timeSinceLastFrame,0,0));
			isleftpressed = true;
		}
		else if(isleftpressed)
		{
			animationState = mSceneMgr->getEntity("pingu")->getAnimationState("right");
			animationState->setLoop(false);
			animationState->setEnabled(true);
			if (animationState->hasEnded())
			{
				animationState->setTimePosition(0);
				animationState = mSceneMgr->getEntity("pingu")->getAnimationState("left");
				animationState->setTimePosition(0);
				animationState = mSceneMgr->getEntity("pingu")->getAnimationState("wave");
				animationState->setLoop(true);
				animationState->setEnabled(true);
				isleftpressed = false;
			}
		}

		if (mKeyboard->isKeyDown(OIS::KC_RIGHT) && mSceneMgr->getSceneNode("PinguNode")->getPosition().x < (50.0))
		{
			animationState = mSceneMgr->getEntity("pingu")->getAnimationState("right");
			animationState->setLoop(false);
			animationState->setEnabled(true);
			if (animationState->hasEnded())
			{
				animationState = mSceneMgr->getEntity("pingu")->getAnimationState("wave");
				animationState->setLoop(true);
				animationState->setEnabled(true);
			}
			mSceneMgr->getSceneNode("PinguNode")->translate(step*evt.timeSinceLastFrame,0,0, Ogre::Node::TS_PARENT);
			mSceneMgr->getCamera("PlayerCam")->move(Ogre::Vector3(step*evt.timeSinceLastFrame,0,0));
			isrightpressed = true;
		}
		else if(isrightpressed)
		{
			animationState = mSceneMgr->getEntity("pingu")->getAnimationState("left");
			animationState->setLoop(false);
			animationState->setEnabled(true);
			if (animationState->hasEnded())
			{
				animationState->setTimePosition(0);
				animationState = mSceneMgr->getEntity("pingu")->getAnimationState("right");
				animationState->setTimePosition(0);
				animationState = mSceneMgr->getEntity("pingu")->getAnimationState("wave");
				animationState->setLoop(true);
				animationState->setEnabled(true);
				isrightpressed = false;
			}
		}

		if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		{
			return false;
		}
		break;
	case true:
		if (mKeyboard->isKeyDown(OIS::KC_RETURN))
		{
			OverlayManager& overlayManager = OverlayManager::getSingleton();
			overlayManager.getByName("panel")->getChild("GUI")->getChild("menu")->hide();
			overlayManager.getByName("panel")->getChild("GUI")->getChild("distance")->show();
			overlayManager.getByName("panel")->getChild("GUI")->getChild("time")->show();
			*inmenu = false;
		}
		if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		{
			return false;
		}
		
		break;
	}

		
	return true;
}