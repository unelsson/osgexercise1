#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include "mycustomcameramanipulator.hpp"

myCustomCameraManipulator::myCustomCameraManipulator()
{
}

myCustomCameraManipulator::~myCustomCameraManipulator()
{
}

bool myCustomCameraManipulator::handleKeyDown(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
    switch(ea.getEventType())
    {
        case(osgGA::GUIEventAdapter::KEYDOWN):
        {
            if (ea.getUnmodifiedKey()==osgGA::GUIEventAdapter::KEY_W)
            {
                mMoveForwards = true;
                return true;
            }
            else if (ea.getUnmodifiedKey()==osgGA::GUIEventAdapter::KEY_A)
            {
                mMoveLeft = true;
                return true;
            }
            else if (ea.getUnmodifiedKey()==osgGA::GUIEventAdapter::KEY_S)
            {
                mMoveBackwards = true;
                return true;
            }
            else if (ea.getUnmodifiedKey()==osgGA::GUIEventAdapter::KEY_D)
            {
                mMoveRight = true;
                return true;
            }
            else if (ea.getUnmodifiedKey()==osgGA::GUIEventAdapter::KEY_Shift_L || ea.getKey()==osgGA::GUIEventAdapter::KEY_Shift_R)
            {
                mRunning = true;
                return true;
            }
        }
        default:
            return false;
    }
}

bool myCustomCameraManipulator::handleKeyUp(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
    switch(ea.getEventType())
    {
        case(osgGA::GUIEventAdapter::KEYUP):
        {
            if (ea.getUnmodifiedKey()==osgGA::GUIEventAdapter::KEY_W)
            {
                mMoveForwards = false;
                return true;
            }
            else if (ea.getUnmodifiedKey()==osgGA::GUIEventAdapter::KEY_A)
            {
                mMoveLeft = false;
                return true;
            }
            else if (ea.getUnmodifiedKey()==osgGA::GUIEventAdapter::KEY_S)
            {
                mMoveBackwards = false;
                return true;
            }
            else if (ea.getUnmodifiedKey()==osgGA::GUIEventAdapter::KEY_D)
            {
                mMoveRight = false;
                return true;
            }
            else if (ea.getUnmodifiedKey()==osgGA::GUIEventAdapter::KEY_Shift_L || ea.getKey()==osgGA::GUIEventAdapter::KEY_Shift_R)
            {
                mRunning = false;
                return true;
            }
            return false;
        }
        default:
            return false;
    }
}

bool myCustomCameraManipulator::handleMouseMove(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
    addMouseEvent(ea);

    // return if less then two events have been added.
    if (_ga_t0.get()==NULL || _ga_t1.get()==NULL) return false;

    float dx = _ga_t0->getXnormalized();
    float dy = _ga_t0->getYnormalized();
    if (performMouseDeltaMovement(dx, dy)) us.requestRedraw();
    return false;
}


bool myCustomCameraManipulator::performMouseDeltaMovement(const float& dx, const float& dy)
{
    // world up vector
    osg::CoordinateFrame coordinateFrame = getCoordinateFrame( _eye );
    osg::Vec3d localUp = getUpVector( coordinateFrame );
    if (dx == 0. && dy == 0.) return false;

    rotateYawPitch( _rotation, dx, dy, localUp );

    return true;
}

bool myCustomCameraManipulator::handleFrame(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
    moveEye();
    centerMousePointer(ea, us);
    return false;
}

void myCustomCameraManipulator::moveEye()
{
    auto forward = _rotation * osg::Vec3d(0.0, 0.0, -1.0);
    auto right = _rotation* osg::Vec3d(1.0, 0.0, 0.0);

    osg::Vec3d movement;
    float movementSpeed = 15.0f;

    if (mMoveForwards) movement += forward;
    if (mMoveBackwards) movement += -forward;
    if (mMoveRight) movement += right;
    if (mMoveLeft) movement += -right;
    if (mRunning) movementSpeed = 30.0f;

    _eye += movement * movementSpeed;
}
