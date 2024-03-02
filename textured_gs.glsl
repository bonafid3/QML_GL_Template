#version 330

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

// input stuff from Qt
uniform mat4 qt_projectionMatrix;
uniform mat4 qt_modelViewMatrix;

in vec2 dimensions[];

out vec2 textCoord_FS;

// executes for each primitive (point)
void main()
{
    float halfW = dimensions[0].x / 2; // half width
    float halfH = dimensions[0].y / 2; // half height

    vec4 vertexPos = gl_in[0].gl_Position + vec4(-halfW, -halfH, 0.0, 0.0); // top left

    gl_Position = qt_projectionMatrix * qt_modelViewMatrix * vertexPos;

    textCoord_FS = vec2(0.0, 0.0);
    EmitVertex();

    vertexPos = gl_in[0].gl_Position + vec4(-halfW, halfH, 0.0, 0.0); // bottom left

    gl_Position = qt_projectionMatrix * qt_modelViewMatrix * vertexPos;

    textCoord_FS = vec2(0.0, 1.0);
    EmitVertex();

    vertexPos = gl_in[0].gl_Position + vec4(halfW, -halfH, 0.0, 0.0); // bottom right

    gl_Position = qt_projectionMatrix * qt_modelViewMatrix * vertexPos;

    textCoord_FS = vec2(1.0, 0.0);
    EmitVertex();

    vertexPos = gl_in[0].gl_Position + vec4(halfW, halfH, 0.0, 0.0); // top right

    gl_Position = qt_projectionMatrix * qt_modelViewMatrix * vertexPos;

    textCoord_FS = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}
