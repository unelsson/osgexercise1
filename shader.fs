#version 450
out vec4 FragmentColor;
//out uint Luminosity;

uniform int lightNum;
uniform vec4 lightColorAmbUni[2];
uniform vec4 lightColorDiffUni[2];
uniform vec4 lightPosUni[2];

in vec4 VertexToFragment;
in vec4 ColorToFragment;
in vec3 NormalToFragment;
in vec4 MultiTexCoordZeroToFragment;

//uniform sampler2d myTexture;

void main()
{
    float ambientStrength = 0.3;
    vec3 finalColor = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < lightNum; i++)
    {
        vec3 lightColorAmb = vec3(lightColorAmbUni[i][0], lightColorAmbUni[i][1], lightColorAmbUni[i][2]);
        vec3 lightColorDiff = vec3(lightColorDiffUni[i][0], lightColorDiffUni[i][1], lightColorDiffUni[i][2]);
        vec3 lightPos = vec3(lightPosUni[i][0], lightPosUni[i][1], lightPosUni[i][2]);
        vec3 ambient = ambientStrength * lightColorAmb;
        vec3 norm = normalize(NormalToFragment);
        vec3 lightDir = normalize(lightPos - vec3(VertexToFragment[0], VertexToFragment[1], VertexToFragment[2]));
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColorDiff;
        vec3 suggestedColor = vec3(0.0, 0.0, 0.0);
        suggestedColor[0] = finalColor[0] + (ambient[0] + diffuse[0]) * ColorToFragment[0];
        suggestedColor[1] = finalColor[1] + (ambient[1] + diffuse[1]) * ColorToFragment[1];
        suggestedColor[2] = finalColor[2] + (ambient[2] + diffuse[2]) * ColorToFragment[2];
        finalColor += suggestedColor;
        if (finalColor[0] > 1.0) finalColor[0] = 1.0;
        if (finalColor[1] > 1.0) finalColor[1] = 1.0;
        if (finalColor[2] > 1.0) finalColor[2] = 1.0;
    }

    FragmentColor = vec4(finalColor, 1.0);
    //Luminosity = 1;
}
