#version 330 core

layout (location=0) out vec4 vFragColor; //wyjście shadera fragmentów

void main()
{
	//wyjściowy stały kolor (biały)
	vFragColor = vec4(1,1,1,1);
}