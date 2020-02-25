#include <osgGA/FirstPersonManipulator>

class myCustomCameraManipulator : public osgGA::FirstPersonManipulator
{
    public:
        myCustomCameraManipulator();

        bool handleMouseMove(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
        bool handleKeyDown(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
        bool handleKeyUp(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
        bool performMouseDeltaMovement( const float& dx, const float& dy );
        bool handleFrame(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
        void moveEye();

    protected:
        ~myCustomCameraManipulator();
        myCustomCameraManipulator(const myCustomCameraManipulator&,const osg::CopyOp&) {}

    private:

        bool mMoveForwards = false;
        bool mMoveBackwards = false;
        bool mMoveRight = false;
        bool mMoveLeft = false;
        bool mRunning = false;
};
