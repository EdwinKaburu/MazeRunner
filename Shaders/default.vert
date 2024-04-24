#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// output colors from vertex shader to fragment shader
out vec3 color;

// Uniform can be accessed with the program and the shaders, akin to a global variable
// uniform float scale;

void main()
{
    gl_Position = vec4(aPos, 1.0);

    // Set the output color
    color = aColor;
}