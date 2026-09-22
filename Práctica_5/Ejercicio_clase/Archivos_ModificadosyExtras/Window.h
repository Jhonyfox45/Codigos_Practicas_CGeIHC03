#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }
	//Los gets para el rover
	GLfloat getarticulacion1() { return articulacion1; }
	GLfloat getarticulacion2() { return articulacion2; }
	GLfloat getarticulacion3() { return articulacion3; }
	GLfloat getarticulacion4() { return articulacion4; }
	GLfloat getarticulacion5() { return articulacion5; }
	GLfloat getarticulacion6() { return articulacion6; }
	GLfloat getarticulacion7() { return articulacion7; }
	GLfloat getarticulacion8() { return articulacion8; }
	GLfloat getarticulacion9() { return articulacion9; }
	GLfloat getarticulacion10() { return articulacion10; }
	//Gets que usaremos más adelante
	GLfloat getarticulacion11() { return articulacion11; }
	GLfloat getarticulacion12() { return articulacion12; }
	GLfloat getarticulacion13() { return articulacion13; }
	GLfloat getarticulacion14() { return articulacion14; }
	GLfloat getarticulacion15() { return articulacion15; }
	GLfloat getarticulacion16() { return articulacion16; }
	GLfloat getarticulacion17() { return articulacion17; }
	GLfloat getarticulacion18() { return articulacion18; }
	GLfloat getarticulacionsonda1() { return articulacionsonda1; }
	GLfloat getarticulacionsonda2() { return articulacionsonda2; }
	GLfloat getarticulacionsonda3() { return articulacionsonda3; }
	GLfloat getarticulacionsonda4() { return articulacionsonda4; }

	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	//Añadimos más articulaciones
	GLfloat rotax,rotay,rotaz, articulacion1, articulacion2, articulacion3, articulacion4, articulacion5, articulacion6
		, articulacion7, articulacion8, articulacion9, articulacion10, articulacion11, articulacion12, articulacion13, articulacion14
		, articulacion15, articulacion16, articulacion17, articulacion18 //Tenemos todas las articulaciones necesarias para el rover
		, articulacionsonda1, articulacionsonda2, articulacionsonda3, articulacionsonda4; //Tenemos las 4 articulaciones para la sonda
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};

