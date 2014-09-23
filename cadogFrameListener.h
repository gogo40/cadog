#ifndef CADOGFRAMELISTENER_H_INCLUDED
#define CADOGFRAMELISTENER_H_INCLUDED


#include <Ogre.h>
#include "ExampleApplication.h"
#include "ExampleFrameListener.h"
#include "cadog.h"

class cadogFrameListener
: public ExampleFrameListener
{
    protected:
        SceneNode* mControlNode;
        SceneNode* mRatNode;
        SceneNode* mCameraNode;
        Cadog* mCmgr;

        double Dx, Dy, Dz;
    public:
        cadogFrameListener(
            RenderWindow* win, Camera* cam,
            SceneNode* controlNode,
            SceneNode* ratNode,
            SceneNode* cameraNode,Cadog* cmgr, double Dx, double Dy, double Dz);
       // bool frameStarted(const FrameEvent& evt);

};

#endif // CADOGFRAMELISTENER_H_INCLUDED
