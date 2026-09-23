#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	//Articulaciones para nuestro rover
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	articulacion6 = 0.0f;
	articulacion7 = 0.0f;
	articulacion8 = 0.0f;
	articulacion9 = 0.0f;
	articulacion10 = 0.0f;
	//Instanciamos las articuaciones para el holocron
	articulacion1H = 0.0f;
	articulacion2H = 0.0f;
	articulacion3H = 0.0f;
	articulacion4H = 0.0f;
	articulacion5H = 0.0f;
	articulacion6H = 0.0f;
	articulacion7H = 0.0f;
	articulacion8H = 0.0f;
	//Las articulaciones y variables de movimiento de nuestra sonda 
	articulacionsonda1 = 0.0f;
	articulacionsonda2 = 0.0f;
	articulacionsonda3 = 0.0f;
	articulacionsonda4 = 0.0f;
	varMovZ = 0.0f;
	varMovY = 0.0f;
	varMovX = 0.0f;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica XX: Nombre de la práctica", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	
	if (key == GLFW_KEY_E)
	{
		theWindow->rotax += 10.0;
	}
	if (key == GLFW_KEY_R)
	{
		theWindow->rotay += 10.0; //rotar sobre el eje y 10 grados
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->rotaz += 10.0;
	}
	//Modificamos la distribución de las teclas
	if (key == GLFW_KEY_Y)
	{
		theWindow->articulacion1 += 10.0;
	}

	if (key == GLFW_KEY_U)
	{
		theWindow->articulacion2 += 10.0;
	}
	if (key == GLFW_KEY_I)
	{
		theWindow->articulacion3 += 10.0;
	}
	if (key == GLFW_KEY_O)
	{
		theWindow->articulacion4 += 10.0;
	}
	//Logica del tope en grados usando dos teclas para aumentar o disminuir el angulo
	if (key == GLFW_KEY_F)
	{
		if (theWindow->articulacion5 < 45.0f) {
			theWindow->articulacion5 += 1.0;
		}
	}
	if (key == GLFW_KEY_G)
	{
		if (theWindow->articulacion5 > -45) {
			theWindow->articulacion5 -= 1.0;
		}
	}
	if (key == GLFW_KEY_H)
	{
		if (theWindow->articulacion6 < 45.0f) {
			theWindow->articulacion6 += 1.0;
		}
	}
	if (key == GLFW_KEY_J)
	{
		if (theWindow->articulacion6 > -45) {
			theWindow->articulacion6 -= 1.0;
		}
	}
	if (key == GLFW_KEY_K)
	{
		if (theWindow->articulacion7 < 45.0f) {
			theWindow->articulacion7 += 1.0;
		}
	}
	if (key == GLFW_KEY_L)
	{
		if (theWindow->articulacion7 > -45) {
			theWindow->articulacion7 -= 1.0;
		}
	}
	if (key == GLFW_KEY_Z)
	{
		if (theWindow->articulacion8 < 45.0f) {
			theWindow->articulacion8 += 1.0;
		}
	}
	if (key == GLFW_KEY_X)
	{
		if (theWindow->articulacion8 > -45) {
			theWindow->articulacion8 -= 1.0;
		}
	}
	if (key == GLFW_KEY_C)
	{
		if (theWindow->articulacion9 < 45.0f) {
			theWindow->articulacion9 += 1.0;
		}
	}
	if (key == GLFW_KEY_V)
	{
		if (theWindow->articulacion9 > -45) {
			theWindow->articulacion9 -= 1.0;
		}
	}
	if (key == GLFW_KEY_B)
	{
		if (theWindow->articulacion10 < 45.0f) {
			theWindow->articulacion10 += 1.0;
		}
	}
	if (key == GLFW_KEY_N)
	{
		if (theWindow->articulacion10 > -45) {
			theWindow->articulacion10 -= 1.0;
		}
	}
	//Los 8 if para las articulaciones del holocron
	if (key == GLFW_KEY_1)
	{
		theWindow->articulacion1H += 10.0;
	}
	if (key == GLFW_KEY_2)
	{
		theWindow->articulacion2H += 10.0;
	}
	if (key == GLFW_KEY_3)
	{
		theWindow->articulacion3H += 10.0;
	}
	if (key == GLFW_KEY_4)
	{
		theWindow->articulacion4H += 10.0;
	}
	if (key == GLFW_KEY_5)
	{
		theWindow->articulacion5H += 10.0;
	}
	if (key == GLFW_KEY_6)
	{
		theWindow->articulacion6H += 10.0;
	}
	if (key == GLFW_KEY_7)
	{
		theWindow->articulacion7H += 10.0;
	}
	if (key == GLFW_KEY_8)
	{
		theWindow->articulacion8H += 10.0;
	}
	//Las articulaciones y variables de movimiento para el satelite
	//Primero las variables de movimiento
	if (key == GLFW_KEY_UP)
	{
		theWindow->varMovZ += 0.75;
	}
	if (key == GLFW_KEY_DOWN)
	{
		theWindow->varMovZ -= 0.75;
	}
	if (key == GLFW_KEY_LEFT)
	{
		theWindow->varMovX += 0.75;
	}
	if (key == GLFW_KEY_RIGHT)
	{
		theWindow->varMovX -= 0.75;
	}
	if (key == GLFW_KEY_M)
	{
		theWindow->varMovY += 0.75;
	}
	if (key == GLFW_KEY_COMMA)
	{
		theWindow->varMovY -= 0.75;
	}
	//Ahora nuestras articulaciones
	if (key == GLFW_KEY_9)
	{
		theWindow->articulacionsonda1 += 10.0;
	}
	if (key == GLFW_KEY_0)
	{
		theWindow->articulacionsonda2 += 10.0;
	}
	if (key == GLFW_KEY_P)
	{
		theWindow->articulacionsonda3 += 10.0;
	}
	if (key == GLFW_KEY_LEFT_BRACKET)
	{
		theWindow->articulacionsonda4 += 10.0;
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		//printf("se presiono la tecla: %s\n",key_name);
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		//printf("se presiono la tecla: %s\n",key_name);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
