/*
-----------------------------------------------------------------------------
Filename:    cadogApplication.h
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
#ifndef __cadogApplication_h_
#define __cadogApplication_h_

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <ois/OISEvents.h>
#include <ois/OISInputManager.h>
#include <ois/OISKeyboard.h>
#include <ois/OISMouse.h>

#include <SdkTrays.h>
#include <SdkCameraMan.h>

#include <irrKlang.h>

#include "cadogMainMenu.h"

#pragma comment(lib, "irrKlang.lib")

#include "cadog.h"
#include "cadogAnimationControl.h"

#define CADOG_SLEEP_TIME 50

class cadogApplication : public Cadog, public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{

public:

	cadogApplication();
	~cadogApplication(void);

    void go(void);


protected:

    void reset();
    bool setup();
	bool configure(void);
	void chooseSceneManager(void);
	void createCamera(void);
	void createFrameListener(void);
	void createScene(void);
	void destroyScene(void);
	void createViewports(void);
	void setupResources(void);
	void createResourceListener(void);
	void loadResources(void);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );
	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	//Adjust mouse clipping area
	void windowResized(Ogre::RenderWindow* rw);
	//Unattach OIS before window shutdown (very important under Linux)
	void windowClosed(Ogre::RenderWindow* rw);

    void loadScenarios();

	void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);
	void buttonHit(OgreBites::Button* button);

	void makeTurn();

	//========================================================//
    Ogre::Root *mRoot;
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;

    irrklang::ISoundEngine* mSoundMgr;

	// OgreBites
	OgreBites::SdkTrayManager*	mTrayMgr;
	OgreBites::SdkCameraMan* mCameraMan;     	// basic camera controller
	OgreBites::ParamsPanel* mDetailsPanel;   	// sample details panel
	bool mCursorWasVisible;						// was cursor visible before dialog appeared
	bool mShutDown;

	//OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;

	cadogMainMenu* mCadogMainMenu;

	//========================================================//

	int Lkey;
    bool movCmp;//movimento completado;
    bool movCatCmp;//movimento gato completado

    AnimationControl mratAc;
    AnimationControl mcatAc;

    Ogre::SceneNode* mrat;
	Ogre::SceneNode* mcat;
	Ogre::SceneNode* mcamn;

    enum yesNoAnswer{
        CADOG_WIN=0, CADOG_LOSE=1,
        CADOG_GAMEOVER=2, CADOG_INIT=3, CADOG_RUNNING=4,
        CADOG_NEWGAME=5
    };

    enum cadogLocations{
        CADOG_MAIN_MENU=0, CADOG_GAME=1
    };

    vector<Ogre::DisplayString> yesNoQuestions;
    vector<Ogre::DisplayString> yesNoCaptions;

    yesNoAnswer statusGame;

	vector<string> cenario;
	vector<string> mcenario;//Musica do cenario

	string mwin;//musica vitoria
	string mlose;//musica derrota
	string mgover;//musica fim de jogo

	string mosound;//Som do ogro
    string mosound2;//som do ogro proximo

	int cc;//conta cenarios
	int nc;//numero de cenarios

	bool showDistOgre;

	cadogLocations mCadogLocation;

};

#endif // #ifndef __cadogApplication_h_
