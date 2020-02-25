// License PD
// OSG: LGPL

#include <chrono>

#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/Array>
#include <osg/Group>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/Node>
#include <osg/NodeCallback>
#include <osg/NodeVisitor>
#include <osg/Uniform>
#include <osg/StateSet>

#include "mycustomcameramanipulator.hpp"
#include "main.h"

std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

// Not used
class rootNodeCallBack : public osg::NodeCallback
{
    public:

    virtual void operator() (osg::Node* node, osg::NodeVisitor* nv)
    {
    }

    private:
};

class lightColorCallBack : public osg::Uniform::Callback
{
    public:

    virtual void operator() (osg::Uniform* uniform, osg::NodeVisitor* nv)
    {
        std::chrono::high_resolution_clock::time_point thisTime = std::chrono::high_resolution_clock::now();
        float milliselapsed = std::chrono::duration_cast<std::chrono::milliseconds>(thisTime - startTime).count();

        osg::Vec4 colorDiffuse(sin(milliselapsed / 100), cos(milliselapsed / 300), 0.3f, 1.0f);

        // Change light colors based on time
        //uniform->setElement(0, colorDiffuse);
        //uniform->setElement(1, colorDiffuse);
    }

    private:
};

class lightPosCallBack : public osg::Uniform::Callback
{
    public:

    virtual void operator() (osg::Uniform* uniform, osg::NodeVisitor* nv)
    {
        std::chrono::high_resolution_clock::time_point thisTime = std::chrono::high_resolution_clock::now();
        float milliselapsed = std::chrono::duration_cast<std::chrono::milliseconds>(thisTime - startTime).count();

        // Make lights make circular motions
        osg::Vec4 location(-190.0f + 500 * sin(milliselapsed/700), 100.0f + 500 * cos(milliselapsed/600), 1000.0f, 1.0f);
        uniform->setElement(0, location);

        location = osg::Vec4f (-190.0f + 1000 * sin(milliselapsed /1000), -150.0f + 1000 * cos(milliselapsed/1000), 1000.0f, 1.0f);
        uniform->setElement(1, location);
    }

    private:
};

void createLight(int& lightNum, osg::ref_ptr<osg::Uniform> lightPosUniform, osg::ref_ptr<osg::Uniform> lightColorAmbUniform, osg::ref_ptr<osg::Uniform> lightColorDiffUniform,
    const osg::Vec4& position, const osg::Vec4& colorAmbient, const osg::Vec4& colorDiffuse)
{
    lightColorAmbUniform->setElement(lightNum, colorAmbient);
    lightColorDiffUniform->setElement(lightNum, colorDiffuse);
    lightPosUniform->setElement(lightNum, position);
    lightNum++;
}

osg::ref_ptr<osg::Node> readNode(std::string fileName)
{
    osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(fileName);
    if (!node)
    {
        std::cout << "Failed to read nodefile " << fileName << std::endl;
        return nullptr;
    }
    return node;
}

void configureCameraSettings(osg::ref_ptr<osg::Camera> camera)
{
    //gluPerspective
    double fov = 45.0; // field of view for camera
    double aspect = 1.0; //aspect ratio (x and y)
    double near = 1.0; //near clipping plane
    double far = 5000.0; //far clipping plane
    camera->setProjectionMatrixAsPerspective(fov, aspect, near, far);

    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setClearColor(osg::Vec4f(0.01f,0.01f,0.1f,1.0f));
    camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

std::string loadFile(std::string fileName)
{
    std::ifstream fileStream;
    std::string fileData = "";
    fileStream.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        fileStream.open(fileName);
        std::stringstream stream;
        stream << fileStream.rdbuf();
        fileStream.close();
        fileData = stream.str();
    }
    catch (std::ifstream::failure error)
    {
        std::cout << "File reading problem" << std::endl;
        return "";
    }
    return fileData;
}

osg::ref_ptr<osg::Geometry> createTestNode()
{
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

    vertices->push_back(osg::Vec3f(-10,-100,0));
    vertices->push_back(osg::Vec3f(100,0,0));
    vertices->push_back(osg::Vec3f(100,100,100));

    colors->push_back(osg::Vec4f(1,0,0,1));
    colors->push_back(osg::Vec4f(0,1,0,1));
    colors->push_back(osg::Vec4f(1,0,1,1));

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, vertices->size()));
    geom->setUseDisplayList(false);

    geom->setVertexArray(vertices);
    geom->setColorArray(colors, osg::Array::BIND_PER_VERTEX);

    geom->setVertexAttribArray(0, vertices, osg::Array::BIND_PER_VERTEX);
    geom->setVertexAttribArray(2, colors, osg::Array::BIND_PER_VERTEX);
    return geom;
}

void addShaders(osg::ref_ptr<osg::Node> node)
{
    //Add test shader
    std::string fragmentShader = loadFile("../shader.fs");
    std::string vertexShader = loadFile("../shader.vs");
    osg::ref_ptr<osg::Program> testShaderProgram = new osg::Program;
    testShaderProgram->setName( "Cool light shader" );
    testShaderProgram->addShader( new osg::Shader( osg::Shader::VERTEX, vertexShader ) );
    testShaderProgram->addShader( new osg::Shader( osg::Shader::FRAGMENT, fragmentShader ) );
    node->getOrCreateStateSet()->setAttributeAndModes(testShaderProgram, osg::StateAttribute::ON);
}

int main(int argc, char** argv )
{
    osgViewer::Viewer viewer;

    osg::ref_ptr<osg::Camera> mainCamera;
    mainCamera = viewer.getCamera();
    configureCameraSettings(mainCamera);

    // Create root Node, for attaching
    osg::ref_ptr<osg::Group> rootNode = new osg::Group;

    // Add object(s) (osg::Node) to base Node
    osg::ref_ptr<osg::Node> rock = readNode("../graniterock_shiny.osgt");
    addShaders(rock);
    rootNode->addChild(rock);

    // Add a non-textured triangle to base Node
    osg::ref_ptr<osg::Node> testNode = createTestNode();
    addShaders(testNode);
    rootNode->addChild(testNode);

    // Settings for camera coordinates
    float cameraX = 0.0f;
    float cameraY = 0.0f;
    float cameraZ = 600.0f;
    osg::Vec3 eye = osg::Vec3(cameraX, cameraY, cameraZ); // Camera position
    osg::Vec3 center = osg::Vec3(0.0f, 0.0f, 0.0f); // Target of camera
    osg::Vec3 up = osg::Y_AXIS;

    osg::ref_ptr<osg::Uniform> lightNumberUniform = new osg::Uniform(osg::Uniform::INT, "lightNum");
    osg::ref_ptr<osg::Uniform> lightPosUniform = new osg::Uniform(osg::Uniform::FLOAT_VEC4, "lightPosUni", 2);
    osg::ref_ptr<osg::Uniform> lightColorAmbUniform = new osg::Uniform(osg::Uniform::FLOAT_VEC4, "lightColorAmbUni", 2);
    osg::ref_ptr<osg::Uniform> lightColorDiffUniform = new osg::Uniform(osg::Uniform::FLOAT_VEC4, "lightColorDiffUni", 2);

    int lightNum = 0;
    createLight(lightNum, lightPosUniform, lightColorAmbUniform, lightColorDiffUniform,
        osg::Vec4 (-550.0f, 550.0f, 550.0f, 1.0f), //lightPos
        osg::Vec4 (0.3f, 0.3f, 0.3f, 1.0f),        //ambient
        osg::Vec4 (0.7f, 0.3f, 0.3f, 1.0f));        // diffuse

    createLight(lightNum, lightPosUniform, lightColorAmbUniform, lightColorDiffUniform,
        osg::Vec4 (-200.0f, 550.0f, -1000.0f, 1.0f),
        osg::Vec4 (0.7f, 0.1f, 0.1f, 1.0f),
        osg::Vec4 (0.0f, 0.0f, 0.7f, 1.0f));

    // Set lights as Uniforms, so they can be readed by both fragment and vertex shader
    lightNumberUniform->set(lightNum);
    lightColorDiffUniform->setUpdateCallback(new lightColorCallBack);
    lightPosUniform->setUpdateCallback(new lightPosCallBack);
    rootNode->getOrCreateStateSet()->addUniform(lightNumberUniform);
    rootNode->getOrCreateStateSet()->addUniform(lightColorAmbUniform);
    rootNode->getOrCreateStateSet()->addUniform(lightColorDiffUniform);
    rootNode->getOrCreateStateSet()->addUniform(lightPosUniform);

    // Add a camera manipulator, responsible also for GUI
    // Camera manipulator is based on osgGA::FirstPersonManipulator
    osg::ref_ptr<myCustomCameraManipulator> cameraManipulator = new myCustomCameraManipulator();
    viewer.setCameraManipulator(cameraManipulator);
    cameraManipulator->setTransformation(eye, center, up);
    cameraManipulator->setHomePosition(eye, center, up);
    cameraManipulator->setVerticalAxisFixed(true);

    viewer.setSceneData(rootNode);
    viewer.setUpViewInWindow(0,0, 1024, 768);

    //These are needed for OSG's in-built global variables used in shaders
    mainCamera->getGraphicsContext()->getState()->setUseModelViewAndProjectionUniforms(true);
    mainCamera->getGraphicsContext()->getState()->setUseVertexAttributeAliasing(true);

    while (!viewer.done())
    {
        viewer.frame();
    }
    return 0;
}
