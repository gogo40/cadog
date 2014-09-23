/*
-----------------------------------------------------------------------------
Filename:    cadogFrameRenderingQueued.cpp
-----------------------------------------------------------------------------


This source file is generated by the
   ___                   _              __    __ _                  _
  /___\__ _ _ __ ___    /_\  _ __  _ __/ / /\ \ (_)______ _ _ __ __| |
 //  // _` | '__/ _ \  //_\\| '_ \| '_ \ \/  \/ / |_  / _` | '__/ _` |
/ \_// (_| | | |  __/ /  _  \ |_) | |_) \  /\  /| |/ / (_| | | | (_| |
\___/ \__, |_|  \___| \_/ \_/ .__/| .__/ \/  \/ |_/___\__,_|_|  \__,_|
      |___/                 |_|   |_|
	  Ogre 1.7.x Application Wizard for Code::Blocks (May 2010)
	  http://code.google.com/p/ogreappwizards/
-----------------------------------------------------------------------------
*/

#include "cadogApplication.h"
#include "cadog.h"

#ifdef WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

void cadogApplication::yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit)
{
    switch(statusGame){
        case CADOG_LOSE:case CADOG_WIN:case CADOG_GAMEOVER:
            mShutDown=!yesHit;
            if(yesHit){
                mSoundMgr->stopAllSounds();
                statusGame=CADOG_RUNNING;
                createScene();
            }
        default:
        break;

    }
}

void cadogApplication::buttonHit(OgreBites::Button* button)
{
    if(button->getName()=="NewGame"){
        mCadogLocation=CADOG_GAME;
        createScene();
    }
}

bool cadogApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{

	if(mWindow->isClosed()){
	    mSoundMgr->stopAllSounds();
		return false;
	}

	if(mShutDown){
	    mSoundMgr->stopAllSounds();
		return false;
	}


#ifdef  __linux

    usleep(CADOG_SLEEP_TIME * 1000);

#elif WIN32 || _WIN32

    Sleep(CADOG_SLEEP_TIME);
#else
    usleep(CADOG_SLEEP_TIME * 1000);
#endif

    if(status()){
        destroyScene();
        if(cc<nc){
            if(status()<0){
                statusGame=CADOG_LOSE;
                mSoundMgr->play2D(mlose.c_str(), true);
                mTrayMgr->showYesNoDialog(yesNoCaptions[CADOG_LOSE],yesNoQuestions[CADOG_LOSE]);
                cc--;
            }else if(cc==nc-1){
                statusGame=CADOG_GAMEOVER;
                mSoundMgr->play2D(mgover.c_str(), true);
                mTrayMgr->showYesNoDialog(yesNoCaptions[CADOG_GAMEOVER],yesNoQuestions[CADOG_GAMEOVER]);
            }else{
                statusGame=CADOG_WIN;
                mSoundMgr->play2D(mwin.c_str(), true);
                mTrayMgr->showYesNoDialog(yesNoCaptions[CADOG_WIN],yesNoQuestions[CADOG_WIN]);
            }
            stat=0;
            return true;
        }
    }

    if(statusGame!=CADOG_RUNNING && statusGame!=CADOG_INIT){
        //Need to capture/update each device
        mKeyboard->capture();
        mMouse->capture();
        return true;
    }


    mratAc.showMove(evt,movCmp,"Walk","Idle1","Idle2");
    mcatAc.showMoveSin(evt,movCatCmp,0,0,0);

    mKeyboard->capture();
    mMouse->capture();

    if(mKeyboard->isKeyDown(OIS::KC_W) ||
       mKeyboard->isKeyDown(OIS::KC_NUMPAD8) ||
       mKeyboard->isKeyDown(OIS::KC_UP)){
           if(movCmp&&movCatCmp){

                Lkey=0;
                movCatCmp=movCmp=false;

                makeTurn();

                #ifdef _DEBUG
                //Imprime na saida padrao o estado do jogo
                fprintf(stderr,"Key down\n");
                print();
                #endif
           }
	}

    if(distOgro>0){
        if(distOgro<4 && !mSoundMgr->isCurrentlyPlaying(mosound.c_str())){
            mSoundMgr->play2D(mosound.c_str(),false);
        }
    }

    mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(nmoves));
    if(distOgro>-1&&showDistOgre) mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(distOgro));
	else mDetailsPanel->setParamValue(1, "???");

    mTrayMgr->frameRenderingQueued(evt);


	if (!mTrayMgr->isDialogVisible())
	{
		mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
	}

	return true;
}
