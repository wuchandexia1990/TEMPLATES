#version 330 core
  
layout(location=0) in vec3 vVertex;		 //położenie wierzchołka w przestrzeni obiektu
 
void main()
{    
	//ustawienie położenia w przestrzeni obiektu
	gl_Position =  vec4(vVertex, 1);			
}