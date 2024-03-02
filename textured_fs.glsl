#version 330

uniform sampler2D qt_texture;

uniform float qt_imageOpacity;

in vec2 textCoord_FS;

out vec4 fragColor;

void main()
{
    fragColor = texture(qt_texture, textCoord_FS) * qt_imageOpacity;
}
