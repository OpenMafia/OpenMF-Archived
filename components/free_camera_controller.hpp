#ifndef FREE_CAMERA_CONTROLLER
#define FREE_CAMERA_CONTROLLER

#include <camera_controller.hpp>
#include <math.hpp>

namespace MFGame
{

class FreeCameraController: public CameraController
{
public:
    FreeCameraController(MFRender::Renderer *renderer, MFInput::InputManager *inputManager);
    virtual void update(double dt) override;

protected:
    double mSpeed;
    double mRotationSpeed;
    double mPreviousMouseButton;
};

FreeCameraController::FreeCameraController(MFRender::Renderer *renderer, MFInput::InputManager *inputManager):
    CameraController(renderer,inputManager)
{
    mSpeed = 1.0;
    mRotationSpeed = 0.005;
    mPreviousMouseButton = false;
}

void FreeCameraController::update(double dt)
{
    MFMath::Vec3 pos, rot;
    double distance = dt * mSpeed;
    MFMath::Vec3 direction = MFMath::Vec3(0,0,0);

    // TODO: un-hardcode keys

    if (mInputManager->keyPressed(225))
        distance *= 5.0;

    if (mInputManager->keyPressed(26))
        direction.y -= 1.0;

    if (mInputManager->keyPressed(22))
        direction.y += 1.0;

    if (mInputManager->keyPressed(4))
        direction.x -= 1.0;

    if (mInputManager->keyPressed(7))
        direction.x += 1.0;

    if (mInputManager->keyPressed(8))
        direction.z += 1.0;

    if (mInputManager->keyPressed(20))
        direction.z -= 1.0;

    direction = direction * ((float) distance);

    MFMath::Vec3 f,r,u;
    mRenderer->getCameraVectors(f,r,u);

    MFMath::Vec3 offset, rotOffset;

    offset = direction.x * r + direction.z * f + direction.y * u;
    rotOffset = MFMath::Vec3(0,0,0);

    if (mInputManager->mouseButtonPressed(1))   // rotation
    {
        unsigned int windowW, windowH;
        mInputManager->getWindowSize(windowW,windowH);

        if (mPreviousMouseButton)      // don't rotate if mouse wasn't centered previously
        {
            int dx, dy;
            unsigned int x, y;
            mInputManager->getMousePosition(x,y);
 
            dx = x - windowW / 2;
            dy = y - windowH / 2;

            rotOffset = MFMath::Vec3(-1 * mRotationSpeed * dx, -1 * mRotationSpeed * dy, 0);
        }

        mPreviousMouseButton = true;
        mInputManager->setMousePosition(windowW / 2, windowH / 2);   // center the mouse
    }
    else
        mPreviousMouseButton = false;

    mRenderer->getCameraPositionRotation(pos,rot);
    mRenderer->setCameraPositionRotation(pos + offset,rot + rotOffset);
}

}

#endif