#include "MenuFrameListener.h"
#include <sstream>
#include <iomanip>


bool MenuFrameListener::frameRenderingQueued(const FrameEvent& evt)
{
	//ExampleFrameListener::frameRenderingQueued(evt);		// odkomentuj ¿eby odblokowaæ kamerê

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
	
	if(!processUnbufferedInput(evt)) return false;
	
	return true;
}

bool MenuFrameListener::processUnbufferedInput(const FrameEvent& evt)	// reakcja na klawisze
{
	if (mKeyboard->isKeyDown(OIS::KC_RETURN))
		return false;

	if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
	{
		*exitgame = true;
		return false;
	}
		
	return true;
}