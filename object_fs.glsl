#version 330

in vec3 vertexNormal_FS;
in vec3 objectColor_FS;
in vec3 lightDirection_FS;

out vec4 fragColor;

void main()
{
   float diff = max(dot(-vertexNormal_FS, lightDirection_FS), 0.0);
   vec3 diffuse = diff * objectColor_FS;
   //vec3 diffuse = diff * vec3(0.2, 0.2, 1.0);
   fragColor = vec4(diffuse, 1.0);
}
