#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLSLShader.h"

#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SOIL.lib")

using namespace std;

//wymiary okna
const int WIDTH  = 512;
const int HEIGHT = 512;

//shader 
GLSLShader shader;

//tablica wierzcho�k�w i obiekt bufora wierzcho�k�w dla pe�noekranowego czworok�ta
GLuint vaoID;
GLuint vboVerticesID;
GLuint vboIndicesID;

//ID tekstury
GLuint textureID;

//wierzcho�ki i indeksy czworok�ta
glm::vec2 vertices[4];
GLushort indices[6];

//nazwa pliku z obrazem tekstury
const string filename = "media/Lenna.png";

//Inicjalizacja OpenGL
void OnInit() {
	GL_CHECK_ERRORS

	//wczytanie shader�w
	shader.LoadFromFile(GL_VERTEX_SHADER, "shadery/shader.vert");
	shader.LoadFromFile(GL_FRAGMENT_SHADER, "shadery/shader.frag");
	//compile and link shader
	shader.CreateAndLinkProgram();
	shader.Use();
		//add attributes and uniforms
		shader.AddAttribute("vVertex");
		shader.AddUniform("textureMap");
		//pass values of constant uniforms at initialization
		glUniform1i(shader("textureMap"), 0);
	shader.UnUse();

	GL_CHECK_ERRORS

	//Ustalanie geometrii czworok�ta
	//ustalanie wierzcho�k�w czworok�ta
	vertices[0] = glm::vec2(0.0,0.0);
	vertices[1] = glm::vec2(1.0,0.0);
	vertices[2] = glm::vec2(1.0,1.0);
	vertices[3] = glm::vec2(0.0,1.0);

	//wype�nianie tablicy indeks�w czworok�ta
	GLushort* id=&indices[0];
	*id++ =0;
	*id++ =1;
	*id++ =2;
	*id++ =0;
	*id++ =2;
	*id++ =3;

	GL_CHECK_ERRORS

	//przygotowanie vao i vbo dla czworok�ta
	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboVerticesID);
	glGenBuffers(1, &vboIndicesID);

	glBindVertexArray(vaoID);

		glBindBuffer (GL_ARRAY_BUFFER, vboVerticesID);
		//przekazanie wierzcho�k�w czworok�ta do obiektu bufora
		glBufferData (GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
		GL_CHECK_ERRORS
		//w��czenie tablicy atrybut�w wierzho�ka dla po�o�enia
		glEnableVertexAttribArray(shader["vVertex"]);
		glVertexAttribPointer(shader["vVertex"], 2, GL_FLOAT, GL_FALSE,0,0);
		GL_CHECK_ERRORS
		//przekazanie indeks�w czworok�ta do bufora tablicy element�w
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
		GL_CHECK_ERRORS


    //wczytanie obrazu
    int texture_width = 0, texture_height = 0, channels=0;
    GLubyte* pData = SOIL_load_image(filename.c_str(), &texture_width, &texture_height, &channels, SOIL_LOAD_AUTO);
    if(pData == NULL) {
        cerr<<"Nie mozna wczytac obrazu: "<<filename.c_str()<<endl;
        exit(EXIT_FAILURE);
    }
    //odwracanie obrazu wzgl�dem osi Y
    int i,j;
    for( j = 0; j*2 < texture_height; ++j )
    {
        int index1 = j * texture_width * channels;
        int index2 = (texture_height - 1 - j) * texture_width * channels;
        for( i = texture_width * channels; i > 0; --i )
        {
            GLubyte temp = pData[index1];
            pData[index1] = pData[index2];
            pData[index2] = temp;
            ++index1;
            ++index2;
        }
    }
    //przygotowanie tekstury i zwi�zanie jej jednostk� teksturuj�c� o numerze 0
    glGenTextures(1, &textureID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        //ustawienie parametr�w tekstury
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        //alokowanie tekstury
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);

    //zwalnianie zaj�tych wcze�niej zasob�w
    SOIL_free_image_data(pData);


    //GL_CHECK_ERRORS

	cout<<"Inicjalizacja powiodla sie"<<endl;
}


//zwalnianie wszystkich alokowanych zasob�w
void OnShutdown() {

	//likwidacja shadera
	shader.DeleteShaderProgram();

	//Likwidacja vao i vbo
	glDeleteBuffers(1, &vboVerticesID);
	glDeleteBuffers(1, &vboIndicesID);
	glDeleteVertexArrays(1, &vaoID);

	//usuwanie tekstur
	glDeleteTextures(1, &textureID);
	cout<<"Zamkniecie powiodlo sie"<<endl;
}

//obs�uga zdarzenia zmiany wymiar�w okna
void OnResize(int w, int h) {
	//ustalanie wymiar�w okna widokowego
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
}

//funkcja wy�wietlania
void OnRender() {
	//czyszczenie bufor�w koloru i g��bi
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//wi�zanie shadera
	shader.Use();
		//rysowanie pe�noekranowego czworok�ta
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	//odwi�zywanie shadera
	shader.UnUse();

	//zamiana bufor�w w celu wy�wietlenia obrazu
	glutSwapBuffers();
}

int main(int argc, char** argv) {
	//inicjalizacja freeglut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitContextVersion (3, 3);
	glutInitContextFlags (GLUT_CORE_PROFILE | GLUT_DEBUG);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Rysowanie obrazu - OpenGL 3.3");

	//inicjalizacja glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)	{
		cerr<<"Blad: "<<glewGetErrorString(err)<<endl;
	} else {
		if (GLEW_VERSION_3_3)
		{
			cout<<"Sterownik obsluguje OpenGL 3.3\nDetails:"<<endl;
		}
	}
	err = glGetError(); //w celu ignorowania b��du 1282 INVALID ENUM
	GL_CHECK_ERRORS

	//wyprowadzanie informacji na ekran
	cout<<"\tWersja GLEW "<<glewGetString(GLEW_VERSION)<<endl;
	cout<<"\tProducent: "<<glGetString (GL_VENDOR)<<endl;
	cout<<"\tRenderer: "<<glGetString (GL_RENDERER)<<endl;
	cout<<"\tWersja OpenGL: "<<glGetString (GL_VERSION)<<endl;
	cout<<"\tGLSL: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<endl;

	GL_CHECK_ERRORS

	//inicjalizacja OpenGL
	OnInit();

	//rejestracja funkcji zwrotnych
	glutCloseFunc(OnShutdown);
	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnResize);

	//wywo�anie p�tli g��wnej
	glutMainLoop();

	return 0;
}
