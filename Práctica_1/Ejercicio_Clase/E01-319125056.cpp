#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>

//Dado que con un contador la velocidad del cambio depende del los ciclos de reloj del CPU usaremos Chrono para medir el tiempo
#include <chrono>

//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader; //Son el indice  de cada uno de estos elemenotos, en el previo poner para que sirven
//Variables para cambiar los colores del fondo
float colorR = 0.0f;
float colorG = 0.0f;
float colorB = 0.0f;

//LENGUAJE DE SHADER (SOMBRAS) GLSL
//Vertex Shader (Por hoy esta aquí pero despues estara aparte en un doc txt) Dibuja los  vertices
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}"; //El cuarto flotante sera siempre 1.0
//Esos datos que recibe los convierte en coordenadas espaciales de vertices (x,y,z,1.0)

//Fragment Shader
//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(0.95f,0.52f,0.0f,1.0f);	 			\n\
}";
//Aqui el valor de alpha, el ultimo sera 1.0 hasta texturizado, le  puedo dar valor plano , variable uniform, color de textura, color por iluminación



void CrearTriangulo()
{
	//Siempre tener este arreglo de datos de tipo GLfloat, son flotantes y se les llama vertices. No necesariamente datos de coordenadas (x,y,z)
	//Por   hoy si son datos (x,y,z) pero más adelante seran datos de color,texturizado etc. El nombre de vertex no es necesario datos de vertices
	//Distintos valores

	//Datos de las esquintas para todos mis triangulos
	GLfloat vertices[] = { //Conjunto de datos, jugar con estos en casa

		//Necesitamos 5 triangulos para el ejercicio
		//Triangulo 1
		-0.90f, 0.00f, 0.0f, 
		-0.50f, 0.90f, 0.0f, 
		-0.10f, 0.00f, 0.0f,

		//Triangulo 2
		-0.90f, 0.00f, 0.0f, 
		-0.50f,-0.90f, 0.0f, 
		-0.10f, 0.00f, 0.0f,

		//Triangulo 3
		-0.10f,-0.90f, 0.0f,
		 0.15f, 0.00f, 0.0f,
		 0.40f,-0.90f, 0.0f,

		//Triangulo 4
		 0.65f, 0.00f, 0.0f,
		 0.15f, 0.00f, 0.0f,
		 0.40f,-0.90f, 0.0f,

		//Triangulo 5
		 0.65f, 0.00f,0.0f,
		 0.40f,-0.90f, 0.0f, 
		 0.90f,-0.90f,0.0f 
		//(x,y,z)
	};
	// Los datos anteriores se deben de guardar en un VAO, Vertes Array Objet
	glGenVertexArrays(1, &VAO); //generar 1 VAO //Memoria de video
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO); //Memoria de video
	glBindBuffer(GL_ARRAY_BUFFER, VBO);                          //AQUI SIEMPRE SERAN STATIC pero más adelante podran ser variables
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores
		               //indice,Ndatos, Tipo, No normalizados, 3 datos de un cierto Ndatos,
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
		glEnableVertexAttribArray(0); //Ya que guardo los datos, los envia al VertexShader
		//Los 9 flotantes se convierten en 3  vec tres , luego ahí se convierten en 3 puntos en la ventana
		//agregar valores a vèrtices y luego declarar un nuevo vertexAttribPointer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

}
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica

//the Program recibe los datos de theShader

{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader,1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n",shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}
//Al Add y Compile no hay que moverle nada, solo tenerlo ahí, exepto más adelante al Compiler se le pasa cierta info
void CompileShaders() {
	shader= glCreateProgram(); //se crea un programa 1
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica 2
	 //verificaciones y prevención de errores 3
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader); //Procesa el paquete 4
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}


// Estos pasos de arriba siempre se dejan, es la documentación


}
int main()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	//****  LAS SIGUIENTES 4 LÍNEAS SE COMENTAN EN DADO CASO DE QUE AL USUARIO NO LE FUNCIONE LA VENTANA Y PUEDA CONOCER LA VERSIÓN DE OPENGL QUE TIENE ****/

	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

 //Llamada a las funciones creadas antes del main
	CrearTriangulo();
	CompileShaders();


	//Tiempo 0 antes del ciclo
	auto inicio = std::chrono::high_resolution_clock::now();
	

	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{

		//Recibir eventos del usuario
		glfwPollEvents();

		//Limpiar la ventana

		//Proceso para que cada 3 segundos cambie de color el fondo de pantalla
		auto actual = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> difTiempo = actual - inicio;
		double tiempoSeg = difTiempo.count();
		int cambColor = (int)(tiempoSeg / 3.0) % 3; // El 3.0 indica 3 segundos, se puede cambiar

		//Cambiar el color del fondo de acuerdo al tiempo 
		if (cambColor == 0) {
			colorR = 1.0f; colorG = 0.0f, colorB = 0.0f;
		}
		else if (cambColor == 1) {
			colorR = 0.0f; colorG = 1.0f, colorB = 0.0f;
		}
		else{
			colorR = 0.0f; colorG = 0.0f, colorB = 1.0f;
		}

		glClearColor(colorR, colorG, colorB, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glUseProgram(shader); //podemos ircambiandode shader

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES,0,15); //Podemos dibujar lineas, puntos, triangulos
		//Para el ejercicio son 5 triangulos por lo tanto son 3 datos por traingulo 5*3 = 15
		             // punto 1, lineas 2, triangulos 3, ...
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
		 
		//NO ESCRIBIR NINGUNA LÍNEA DESPUÉS DE glfwSwapBuffers(mainWindow); 
	}


	return 0;
}