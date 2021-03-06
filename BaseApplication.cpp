/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.cpp
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

//-------------------------------------------------------------------------------------
cadogApplication::cadogApplication()
	: mRoot(0),
	mCamera(0),
	mSceneMgr(0),
	mWindow(0),
	mResourcesCfg(Ogre::BLANKSTRING),
	mPluginsCfg(Ogre::BLANKSTRING),
	mTrayMgr(0),
	mCameraMan(0),
	mDetailsPanel(0),
	mCursorWasVisible(false),
	mShutDown(false),
	mInputManager(0),
	mMouse(0),
	mKeyboard(0)
{
    nc=0;
    cc=-1;
    mrat=0;
    mcat=0;
    stat=0;
    Lkey=-1;
    movCatCmp=movCmp=true;
    showDistOgre=false;
    statusGame=CADOG_INIT;
    mCadogLocation=CADOG_MAIN_MENU;

    yesNoCaptions.push_back("You Win!");
    yesNoQuestions.push_back("Do you want to continue?");

    yesNoCaptions.push_back("You Lose!");
    yesNoQuestions.push_back("Do you want to retry?");

    yesNoCaptions.push_back("Congratulations");
    yesNoQuestions.push_back("You won the game.\nDo you want to play again?");


    loadScenarios();
}

void cadogApplication::reset(){
    stat=0;
    Lkey=-1;
    movCatCmp=movCmp=true;
    statusGame=CADOG_INIT;
}
//-------------------------------------------------------------------------------------
cadogApplication::~cadogApplication(void)
{
    mSoundMgr->stopAllSounds();

	if (mTrayMgr) delete mTrayMgr;
	if (mCameraMan) delete mCameraMan;

	//Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);

	delete mRoot;
	delete mSoundMgr;
}

//-------------------------------------------------------------------------------------
bool cadogApplication::configure(void)
{
	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	if(mRoot->showConfigDialog())
	{
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		mWindow = mRoot->initialise(true, "Cadog");

		return true;
	}
	else
	{
		return false;
	}
}
//-------------------------------------------------------------------------------------
void cadogApplication::chooseSceneManager(void)
{
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_INTERIOR);
}
//-------------------------------------------------------------------------------------
void cadogApplication::createCamera(void)
{
	// Create the camera
	mCamera = mSceneMgr->createCamera("c0");

	// Position it at 500 in Z direction
	mCamera->setPosition(Ogre::Vector3(0,0,80));
	// Look back along -Z
	mCamera->lookAt(Ogre::Vector3(0,0,-300));
	mCamera->setNearClipDistance(5);

	mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}
//-------------------------------------------------------------------------------------
void cadogApplication::createFrameListener(void)
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	//Set initial mouse clipping size
	windowResized(mWindow);

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mInputContext.mKeyboard = mKeyboard;
	mInputContext.mMouse = mMouse;

	mFontManager->getSingleton().getByName("SdkTrays/Caption")->load();
	mFontManager->getSingleton().getByName("SdkTrays/Value")->load();

	mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mInputContext, this);
	mTrayMgr->hideCursor();

	//Cria painel com parametros do jogo
	Ogre::StringVector items;

	items.push_back("Number of moves");
	items.push_back("Distance to Ogre");

	mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "PainelDistNumMoves", 180, items);

    mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(0));
	mDetailsPanel->setParamValue(1, "inf");

    //mCadogMainMenu = new cadogMainMenu(mTrayMgr);

	mRoot->addFrameListener(this);
	mSoundMgr=irrklang::createIrrKlangDevice();

}
//-------------------------------------------------------------------------------------
void cadogApplication::destroyScene(void)
{
    mDetailsPanel->hide();
    mSoundMgr->stopAllSounds();
    mSceneMgr->clearScene();
}
//-------------------------------------------------------------------------------------
void cadogApplication::createViewports(void)
{
	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------
void cadogApplication::setupResources(void)
{
	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}

	
}
//-------------------------------------------------------------------------------------
void cadogApplication::createResourceListener(void)
{
	mOverlaySystem = new Ogre::OverlaySystem();

	mFontManager = Ogre::FontManager::getSingletonPtr();
	mOverlayManager = Ogre::OverlayManager::getSingletonPtr();
	
	mSceneMgr->addRenderQueueListener(mOverlaySystem);
}
//-------------------------------------------------------------------------------------
void cadogApplication::loadResources(void)
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Essential");
}
//-------------------------------------------------------------------------------------
void cadogApplication::go(void)
{
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	if (!setup())
		return;

    mRoot->startRendering();

    destroyScene();
}
//-------------------------------------------------------------------------------------
bool cadogApplication::setup(void)
{
	mRoot = new Ogre::Root(mPluginsCfg);

	
	setupResources();

	bool carryOn = configure();
	if (!carryOn) return false;

	chooseSceneManager();
	createCamera();
	createViewports();

	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Create any resource listeners (for loading screens)
	//Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	//Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Essential");

	createResourceListener();

	// Load resources
	
	loadResources();

	
    createFrameListener();

	// Create the scene
	createScene();

	return true;
};
//-------------------------------------------------------------------------------------

bool cadogApplication::mouseMoved( const OIS::MouseEvent &arg )
{
    if (mTrayMgr->injectMouseMove(arg)) return true;
	mCameraMan->injectMouseMove(arg);
	return true;
}

bool cadogApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseDown(arg, id)) return true;
	mCameraMan->injectMouseDown(arg, id);
	return true;
}

bool cadogApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
	mCameraMan->injectMouseUp(arg, id);
	return true;
}

//Adjust mouse clipping area
void cadogApplication::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void cadogApplication::windowClosed(Ogre::RenderWindow* rw)
{
	//Only close for window that created OIS (the main window in these demos)
	if( rw == mWindow )
	{
		if( mInputManager )
		{
			mInputManager->destroyInputObject( mMouse );
			mInputManager->destroyInputObject( mKeyboard );

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}
