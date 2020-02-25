#version 450

uniform mat4 osg_ModelViewProjectionMatrix;
uniform int lightNum;

layout(location = 0) in vec4 osg_Vertex;
layout(location = 1) in vec3 osg_Normal;
layout(location = 2) in vec4 osg_Color;
layout(location = 3) in vec4 osg_MultiTexCoord0;
layout(location = 4) in vec4 osg_MultiTexCoord1;
layout(location = 5) in vec4 osg_MultiTexCoord2;
layout(location = 6) in vec4 osg_MultiTexCoord3;
layout(location = 7) in vec4 osg_MultiTexCoord4;
layout(location = 8) in vec4 osg_MultiTexCoord5;
layout(location = 9) in vec4 osg_MultiTexCoord6;
layout(location = 10)in vec4 osg_MultiTexCoord7;

out vec4 VertexToFragment;
out vec4 ColorToFragment;
out vec3 NormalToFragment;
out vec4 MultiTexCoordZeroToFragment;

void main()
{
    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;
    VertexToFragment = osg_Vertex;
    ColorToFragment = osg_Color;
    NormalToFragment = osg_Normal;
}
