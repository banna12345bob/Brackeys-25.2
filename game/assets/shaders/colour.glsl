// Basic Colour Shader

#type vertex
#version 450 core
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Colour;
out vec4 v_Colour;

void main()
{
	gl_Position = a_Position;
	v_Colour = a_Colour;
}

#type fragment
#version 450 core
in vec4 v_Colour;
out vec4 FragColor;

void main()
{
	FragColor = v_Colour;
}