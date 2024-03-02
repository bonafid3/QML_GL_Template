#version 330

layout (location=0) in vec3 qt_vertex;
layout (location=1) in vec2 qt_dimensions;

uniform mat4 qt_modelViewMatrix;
uniform mat4 qt_projectionMatrix;

out vec2 dimensions;

void main()
{
    dimensions = qt_dimensions;

    //this is done in the geometry shader after triangle strips made
    //gl_Position = qt_projectionMatrix * qt_modelViewMatrix * vec4(qt_vertex, 1.0);

    gl_Position = vec4(qt_vertex, 1.0);
}
