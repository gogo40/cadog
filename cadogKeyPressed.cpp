/*
-----------------------------------------------------------------------------
Filename:    cadogKeyPressed.cpp
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

inline int convert_( const OIS::KeyEvent &arg){
    int Lkey=-1;
    if(arg.key==OIS::KC_W || arg.key==OIS::KC_NUMPAD8 || arg.key==OIS::KC_UP) Lkey=0;
    else if(arg.key==OIS::KC_D || arg.key==OIS::KC_NUMPAD6 || arg.key==OIS::KC_RIGHT) Lkey=1;
    else if(arg.key==OIS::KC_S || arg.key==OIS::KC_NUMPAD5 || arg.key==OIS::KC_DOWN) Lkey=2;
    else if(arg.key==OIS::KC_A || arg.key==OIS::KC_NUMPAD4 || arg.key==OIS::KC_LEFT) Lkey=3;
    else if(arg.key==OIS::KC_Q) Lkey=4;
    else if(arg.key==OIS::KC_E) Lkey=5;
    else if(arg.key==OIS::KC_Z) Lkey=6;
    else if(arg.key==OIS::KC_C) Lkey=7;
    else if(arg.key==OIS::KC_X) Lkey=8;
    return Lkey;
}


void cadogApplication::makeTurn(){
    if(Lkey<0) return;

    if(mrat!=0){
        point u=posPlayer;
        bool tel,ok; tel=false;
        Ogre::Vector3 s(Dx*u.x,Dz,Dy*u.y);
        tel=ok=false;
        switch(Lkey){

            case 0:
                ok=move(u,direcao,u,tel);
                moveAI();
            break;

            case 1:
                direcao=(direcao+1)%8;
                mrat->yaw(Ogre::Degree(-45));
                movCmp=true;
            break;

            case 2:
                direcao=(direcao+4)%8;
                mrat->yaw(Ogre::Degree(180));
                movCmp=true;
            break;

            case 3:
                direcao=(direcao-1+8)%8;
                mrat->yaw(Ogre::Degree(45));
                movCmp=true;
            break;

            case 4:
                mCamera->yaw(Ogre::Degree(180));
                movCmp=true;
            break;

            case 5:
                mCamera->yaw(Ogre::Degree(-180));
                movCmp=true;
            break;

            case 6:
                mCamera->yaw(Ogre::Degree(45));
                movCmp=true;
            break;

            case 7:
                mCamera->yaw(Ogre::Degree(-45));
                movCmp=true;
            break;

            case 8:
                showDistOgre=!showDistOgre;
                movCmp=true;
            break;

            default:
            break;
        }
        if(Lkey==0){
            Ogre::Vector3 t(Dx*u.x,Dz,Dy*u.y);
            if(!tel&&ok) mratAc.move(t);
            else if(status()==0){
                mrat->setPosition(t);
                mratAc.move(t);
                movCmp=true;
            }
        }
    }
    if(mcat!=0){

        point u=posPlayer;
        point v=posEnemy;

        Ogre::Vector3 t(Dx*v.x,Dz+80,Dy*v.y);

        if(status()==0) mcatAc.move(t);

        int sx=(v.x-u.x)<=0?-1:1;
        int sy=(v.y-u.y)<=0?-1:1;

        if(u.x==v.x) sx=0;
        if(u.y==v.y) sy=0;

        mcat->lookAt(Ogre::Vector3(10*N*Dx*sx,Dz,10*M*Dy*sy),Ogre::Node::TS_WORLD);
    }
}

bool cadogApplication::keyPressed( const OIS::KeyEvent &arg ){
    Lkey=-1;

    if (mTrayMgr->isDialogVisible()) return true;

    if(arg.key==OIS::KC_SPACE){
        mratAc.acelerate();
        mcatAc.acelerate();
    }else if (arg.key == OIS::KC_SYSRQ){
		mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
	}
	else if (arg.key == OIS::KC_ESCAPE){
		mShutDown = true;
	}else if(movCmp){
         Lkey=convert_(arg);
         if(Lkey>=0){

             movCatCmp=movCmp=false;

             makeTurn();

             #ifdef _DEBUG
             //Imprime na saida padrao o estado do jogo
             fprintf(stderr,"Key pressed\n");
             fprintf(stderr,"dist Ogro:%d\n",distOgro);
             print();
             #endif

         }
    }
    return true;
}

bool cadogApplication::keyReleased(const OIS::KeyEvent& arg)
{
    if(arg.key==OIS::KC_SPACE){
        mratAc.reduce();
        mcatAc.reduce();
    }
    return true;
}

