#include "cadogApplication.h"
#include "cadogFrameListener.h"
#include "cadog.h"

cadogFrameListener::cadogFrameListener(
            RenderWindow* win, Camera* cam,
            SceneNode* controlNode,
            SceneNode* ratNode,
            SceneNode* cameraNode,Cadog* cmgr, double Dx, double Dy, double Dz)
:ExampleFrameListener(win, cam),Dx(Dx),Dy(Dy),Dz(Dz)
{
    fprintf(stderr,"AQUI\n");
    mControlNode=controlNode;
    mRatNode=ratNode;
    mCameraNode=cameraNode;
    mCmgr=cmgr;
}
/*
bool cadogFrameListener::frameStarted(const FrameEvent& evt)
{
fprintf(stderr,"trantando teclado..\n");
    mKeyboard->capture();
fprintf(stderr,"trantando teclado..\n");
    //Fecha programa
   if (mKeyboard->isKeyDown(OIS::KC_F1)) {
        while (mKeyboard->isKeyDown(OIS::KC_F1))
            mKeyboard->capture();
        mCameraNode->setPosition(Vector3(0, 50, -200));
    }
fprintf(stderr,"aqui oh...\n\n");
    return true;
}
*/
