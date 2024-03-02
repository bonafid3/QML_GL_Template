#version 330

layout (location=0) in vec3 qt_vertex;
layout (location=1) in vec3 qt_normal;
layout (location=2) in vec3 qt_color;

layout (location=3) in vec3 qt_position;
layout (location=4) in vec3 qt_colorka;

uniform mat3 qt_normalMatrix;

uniform mat4 qt_modelMatrix;
uniform mat4 qt_viewMatrix;
uniform mat4 qt_projectionMatrix;

uniform vec3 qt_lightPositionWorld;

out vec3 objectColor_FS;
out vec3 lightDirection_FS;
out vec3 vertexNormal_FS;

void main()
{
    vertexNormal_FS = qt_normalMatrix * qt_normal;
    //objectColor_FS = qt_color;
    objectColor_FS = qt_colorka;

    // apply local transformations to the model first
    // add displacement
    // transform to camera (world)
    vec4 worldPos = qt_viewMatrix * ((qt_modelMatrix * vec4(qt_vertex, 1.0)) + vec4(qt_position, 0.0));

    lightDirection_FS = normalize(worldPos - vec4(qt_lightPositionWorld, 1)).xyz;

    gl_Position = qt_projectionMatrix * worldPos;
}
