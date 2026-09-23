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
	//Necesitamos 8 gets para el holocron
	GLfloat getarticulacion1H() { return articulacion1H; }
	GLfloat getarticulacion2H() { return articulacion2H; }
	GLfloat getarticulacion3H() { return articulacion3H; }
	GLfloat getarticulacion4H() { return articulacion4H; }
	GLfloat getarticulacion5H() { return articulacion5H; }
	GLfloat getarticulacion6H() { return articulacion6H; }
	GLfloat getarticulacion7H() { return articulacion7H; }
	GLfloat getarticulacion8H() { return articulacion8H; }
	//Los gets para nuestro satelite
	GLfloat getarticulacionsonda1() { return articulacionsonda1; }
	GLfloat getarticulacionsonda2() { return articulacionsonda2; }
	GLfloat getarticulacionsonda3() { return articulacionsonda3; }
	GLfloat getarticulacionsonda4() { return articulacionsonda4; }
	GLfloat getvarMovX() { return varMovX; }
	GLfloat getvarMovY() { return varMovY; }
	GLfloat getvarMovZ() { return varMovZ; }
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	//Añadimos más articulaciones
	GLfloat rotax,rotay,rotaz, articulacion1, articulacion2, articulacion3, articulacion4, articulacion5, articulacion6
		, articulacion7, articulacion8, articulacion9, articulacion10     //Tenemos todas las articulaciones necesarias para el rover
		, articulacion1H, articulacion2H, articulacion3H, articulacion4H
		, articulacion5H, articulacion6H, articulacion7H, articulacion8H  //Tenemos todas las articulaciones para el holocron
		, varMovX, varMovY, varMovZ //Variables de movimiento para el satelite
		, articulacionsonda1, articulacionsonda2, articulacionsonda3, articulacionsonda4; //Tenemos las articulaciones para el satelite
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

