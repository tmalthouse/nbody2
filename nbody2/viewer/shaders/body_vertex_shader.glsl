#version 150

in vec3 position;

uniform mat4 MVPmatrix;

void main()
{
    gl_Position = MVPmatrix * vec4(position, 1.0);
}
