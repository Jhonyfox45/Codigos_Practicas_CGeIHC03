/*Práctica 4: Modelado Jerárquico.
Se implementa el uso de matrices adicionales para almacenar información de transformaciones geométricas que se quiere
heredar entre diversas instancias para que estén unidas
Teclas de la F a la K para rotaciones de articulaciones
*/
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

//USAMOS CONSTANTES PARA HACER MÁS FACIL EL DESPLAZAMIENTO
const float movX_Rover = 0.0f;
const float movY_Rover = 0.0f;
const float movZ_Rover = 0.0f;

const float movX_Sonda =  10.0f;
const float movY_Sonda =  30.0f;
const float movZ_Sonda = -20.0f;



void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro y cono con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(25, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CrearCilindro(8, 1.0f); //índice 5 en MeshList (Es un octagono para la sonda)
	CreateShaders();
	

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux2(1.0);

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		//La linea 356 es como el origen de mi modelo jerarquico
		//articulacion1 hasta articulación5 sólo son puntos de rotación o articulación, en este caso no dibujaremos esferas que los representen
		model = glm::mat4(1.0);
		//Este es nuestro traslate para movernos del origen al centro de la cabina
		model = glm::translate(model, glm::vec3(0.0f + movX_Rover, 4.0f + movY_Rover, -4.0f + movZ_Rover)); //NOS POSICIONAMOS EN EL CENTRO DEL OBJETO X AMARILLA 

		// Creando la cabina del rover ¿CUÁNTAS UNIDADES MEDIRÁ EN Z?
		model = glm::translate(model, glm::vec3(1.0f, 2.0f, 0.0f));//PARA LLEGAR AL CENTRO DE LA CABINA a PARTIR DEL ORIGEN
		modelaux = model; //guardamos la matriz de modelo para que la base se mueva con el objeto
		model = glm::scale(model, glm::vec3(8.0f, 4.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); //RECORDAD ESTE UNA SOLA VEZ
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular
		//meshList[3]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro, cono, pirámide base cuadrangular
		//sp.render(); //dibuja esfera


		// BASE
		// Quitamos la siguiente linea o si no sera como derfinir otro objeto modelo
		//model = glm::mat4(1.0); //Si dejamos está linea, la base no estará unida al centro del objeto. se debe comentar para que la base se mueva con el objeto.
		/*En su lugar usamos la matriz auxiliar modelaux para que la base se mueva con el objeto.
		Lo que debemos de saber es: de las transformaciones geométricas que se aplican a la cabina y
		cuales queremos que se apliquen a la base. En este caso, sólo queremos que se aplique la traslación del origen, no la rotación ni el escalado de la cabina.
		Por lo tanto, debemos de guardar en modelaux sólo la traslación del origen y luego aplicarla a la base.
		*/
		model = modelaux;
		//NOS POSICIONAMOS EN EL CENTRO DE LA BASE
		model = glm::translate(model, glm::vec3(-1.0f, -2.75f, 0.0f));
		modelaux = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base
		model = glm::scale(model, glm::vec3(10.0f, 1.5f, 8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		model = modelaux;
		modelaux2 = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base
		/*
		A PARTIR DE ESTE PUNTO HAY QUE ACOMODAR VARIOS ELEMENTOS:
		LA BASE PARA LA LLANTA FRONTAL (CERCA)
		LA BASE PARA EL BRAZO
		LA BASE PARA LA LLANTA DE EN MEDIO (CERCA)
		LA BASE PARA LA LLANTA DE ATRÁS (CERCA)
		.........¿FALTA ALGO MÁS? Sí, x2 todas las llantas
		LA BASE PARA LA LLANTA FRONTAL (LEJOS)
		LA BASE PARA LA LLANTA DE EN MEDIO (LEJOS)
		LA BASE PARA LA LLANTA DE ATRÁS (LEJOS)
		CÓMO ESTAREMOS REGRESANDO A ESTE PUNTO EN ESPECÍFICO VARIAS VECES, DEBEMOS DE USAR UNA NUEVA MATRIZ AUXILIAR PARA CADA UNO DE LOS ELEMENTOS
		QUE SE QUIERAN CONECTAR A LA BASE, YA QUE SI USAMOS LA MISMA MATRIZ AUXILIAR, SE SOBRESCRIBIRÁN LAS TRANSFORMACIONES GEOMÉTRICAS DE CADA ELEMENTO
		Y NO SE PODRÁN CONECTAR CORRECTAMENTE.

		*/
		//EJE PARA LA BASE DEL BRAZO
		model = modelaux2;
		//Nos posicionamos al centro del brazo 
		model = glm::translate(model, glm::vec3(-3.5f, 0.75f, 2.5f)); //Nos movemos del origen de la base a donde ira nuestro brazo
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escalamos la esfera 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//PASAMOS MATRIZ MODELO
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		//BASE PARA EL BRAZO
		model = modelaux;
		model = glm::translate(model, glm::vec3( 0.0f, 0.5f, 0.0f));
		//modelaux = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base
		//model = glm::scale(model, glm::vec3(10.0f, 1.5f, 8.0f)); SE COMENTA PORQUE NO ES NECESARIO APLICAR LA ESCALA
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//PASAMOS MATRIZ MODELO
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();
		//Ahora necesitamos colocar el EJE DE ROTACIÓN
		model = glm::translate(model, glm::vec3(0.0f, 0.75f, 0.0f)); //Nos movemos del origen de la base a donde ira nuestro brazo
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escalamos la esfera 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//PASAMOS MATRIZ MODELO
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		//1er BRAZO
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.25f, 1.25f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; // GUARDAMOS LA ROTACIÓN, EL EJE DEL MUNDO HA ROTADO
		model = glm::scale(model, glm::vec3(0.5f, 3.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();
		//SEGUNDO EJE
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f); //NOTA: Todos los ejes los haremos del mismo color
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		//BRAZO 2 recordar que los ejes están rotados
		model = modelaux;
		model = glm::translate(model, glm::vec3( 1.5f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 3.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();
		//TERCER EJE
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f); //NOTA: Todos los ejes los haremos del mismo color
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		//PINZA (Sera un cilindro más pequeño y delgado)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.25f, 1.0f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();
		
		//EJE  PARA BASE DE LA LLANTA FRONTAL (CERCA)
		model = modelaux2;
		//Nos posicionamos al centro del brazo 
		model = glm::translate(model, glm::vec3(-4.0f, 0.75f, 4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); 
		sp.render();
		//BARRA PARA LA LLANTA FRONTAL PT1 (CERCA)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.75f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 
		model = glm::scale(model, glm::vec3(0.35f, 1.5f, 0.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.62f, 0.65f, 0.68f); //Usamos el color gris de la otra vez
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();
		//BARRA PARA LA LLANTA FRONTAL PT2 (CERCA)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.5f, -0.75f, 0.0f));
		model = glm::rotate(model, glm::radians(112.5f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; //Guardamos la rotación
		model = glm::scale(model, glm::vec3(0.35f, 4.0f, 0.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.62f, 0.65f, 0.68f); //Usamos el color gris de la otra vez
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();
		//EJE ENTRE LA BARRA Y PALTA L DE LA LLANTA FRONTAL (CERCA)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f)); //Recordemos que tenemos el eje rotado
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f); //NOTA: Todos los ejes los haremos del mismo color
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		//PATA L PARA LA LLANTA FRONTAL (CERCA) PT1
		model = modelaux;
		model = glm::translate(model, glm::vec3( 0.0f, 0.75f, 0.0f));
		model = glm::rotate(model, glm::radians(10.f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; //Guardamos la rotación
		model = glm::scale(model, glm::vec3(0.6f, 1.5f, 0.3f)); //Hacemos "Placas"
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.15f, 0.18f, 0.22f); //Usamos el color gris oscurlo 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); 
		meshList[0]->RenderMeshGeometry();
		//PATA L PARA LA LLANTA FRONTAL (CERCA) PT2
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.75f, 0.45f, 0.0f));
		modelaux = model; //Guardamos la rotación
		model = glm::scale(model, glm::vec3(1.5f, 0.6f, 0.3f)); //Hacemos "Placas"
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.15f, 0.18f, 0.22f); //Usamos el color gris oscurlo 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMeshGeometry();
		//EJE PARA LA LLANTA FRONTAL (CERCA)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.75f, 0.0f, 0.0f)); //Recordemos que tenemos el eje rotado
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f); //NOTA: Todos los ejes los haremos del mismo color
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		//LLANTA FRONTAL (CERCA)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.5f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 0.3f)); //Primero escalo antes de rotar
		model = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f); //Las llantas seran de color blanco
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();
		
		//EJE PARA LA LLANTA FRONTAL (LEJOS) 
		//LO MISMO QUE EL ANTERIOR SOLO QUE CAMBIAMOS DE Z A -Z EN LAS TRASLACIONES 
		model = modelaux2;
		//Nos posicionamos al centro del brazo 
		model = glm::translate(model, glm::vec3(-4.0f, 0.75f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		//BARRA PARA LA LLANTA FRONTAL PT1 (LEJOS)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.75f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.35f, 1.5f, 0.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.62f, 0.65f, 0.68f); //Usamos el color gris de la otra vez
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();
		//BARRA PARA LA LLANTA FRONTAL PT2 (LEJOS)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.5f, -0.75f, 0.0f));
		model = glm::rotate(model, glm::radians(112.5f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; //Guardamos la rotación
		model = glm::scale(model, glm::vec3(0.35f, 4.0f, 0.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.62f, 0.65f, 0.68f); //Usamos el color gris de la otra vez
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();
		//EJE ENTRE LA BARRA Y PALTA L DE LA LLANTA FRONTAL (LEJOS)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f)); //Recordemos que tenemos el eje rotado
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f); //NOTA: Todos los ejes los haremos del mismo color
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		//PATA L PARA LA LLANTA FRONTAL (LEJOS) PT1
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.75f, 0.0f));
		model = glm::rotate(model, glm::radians(10.f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; //Guardamos la rotación
		model = glm::scale(model, glm::vec3(0.6f, 1.5f, 0.3f)); //Hacemos "Placas"
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.15f, 0.18f, 0.22f); //Usamos el color gris oscurlo 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMeshGeometry();
		//PATA L PARA LA LLANTA FRONTAL (LEJOS) PT2
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.75f, 0.45f, 0.0f));
		modelaux = model; //Guardamos la rotación
		model = glm::scale(model, glm::vec3(1.5f, 0.6f, 0.3f)); //Hacemos "Placas"
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.15f, 0.18f, 0.22f); //Usamos el color gris oscurlo 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMeshGeometry();
		//EJE PARA LA LLANTA FRONTAL (LEJOS)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.75f, 0.0f, 0.0f)); //Recordemos que tenemos el eje rotado
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f); //NOTA: Todos los ejes los haremos del mismo color
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		//LLANTA FRONTAL (LEJOS)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.5f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 0.3f)); //Primero escalo antes de rotar
		model = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f); //Las llantas seran de color blanco
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();
		

		//EJE PARA LA LLANTA DE EN MEDIO (CERCA)
		model = modelaux2;
		model = glm::translate(model, glm::vec3( 0.33f, -0.25f, 4.3f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		//PATA L PARA LA LLANTA DE EN MEDIO (CERCA) PT1
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.33f, 0.0f, -0.3f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; //Guardamos la rotación
		model = glm::scale(model, glm::vec3(0.6f, 1.66f, 0.6f)); //Más grueso que los brazos del rover
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.62f, 0.65f, 0.68f); //Otra vez usamos el color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();
		//PATA L PARA LA LLANTA DE EN MEDIO (CERCA) PT2
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.0f, 1.75f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; //Guardamos la rotación
		model = glm::scale(model, glm::vec3(0.6f, 3.25f, 0.6f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.62f, 0.65f, 0.68f); 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); 
		meshList[2]->RenderMeshGeometry();
		//EJE PARA LA LLANTA DE EN MEDIO (CERCA)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.875f, 0.0f)); //Recordemos que tenemos el eje rotado
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion12()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f); 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); 
		sp.render();
		//LLANTA DE EN MEDIO (CERCA)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.6f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 0.3f)); //Primero escalo antes de rotar
		model = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f); //Las llantas seran de color blanco
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); 
		meshList[2]->RenderMeshGeometry();

		//EJE PARA LA LLANTA DE EN MEDIO (LEJOS)
		//LO MISMO QUE EL ANTERIOR SOLO QUE CAMBIAMOS DE Z A -Z EN LAS TRASLACIONES 
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.33f, -0.25f, -4.3f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion13()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		//PATA L PARA LA LLANTA DE EN MEDIO (LEJOS) PT1
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.33f, 0.0f, 0.3f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; //Guardamos la rotación
		model = glm::scale(model, glm::vec3(0.6f, 1.66f, 0.6f)); //Más grueso que los brazos del rover
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.62f, 0.65f, 0.68f); //Otra vez usamos el color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();
		//PATA L PARA LA LLANTA DE EN MEDIO (LEJOS) PT2
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.0f, 1.75f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; //Guardamos la rotación
		model = glm::scale(model, glm::vec3(0.6f, 3.25f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.62f, 0.65f, 0.68f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		//EJE PARA LA LLANTA DE EN MEDIO (LEJOS)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.875f, 0.0f)); //Recordemos que tenemos el eje rotado
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion14()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		//LLANTA DE EN MEDIO (LEJOS)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.6f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 0.3f)); //Primero escalo antes de rotar
		model = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f); //Las llantas seran de color blanco
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		//EJE PARA LA LLANTA DE ATRÁS (CERCA)
		//AL SER IGUALES QUE LAS LLANTAS DE EN MEDIO SOLO INVERTIMOS EL EJE X EN TRASLACIÓN Y Z EN ROTACIÓN
		// DESPUES DE UBICAR EL PUNTO INICIAL DEL EJE
		// ES IGUAL A LA LLANDA DE EN MEDIO PERO ESPEJEADA
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.66f, -0.25f, 4.3f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion15()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		//PATA L PARA LA LLANTA DE ATRAS (CERCA) PT1
		model = modelaux;
		model = glm::translate(model, glm::vec3( 0.33f, 0.0f, -0.3f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; //Guardamos la rotación
		model = glm::scale(model, glm::vec3(0.6f, 1.66f, 0.6f)); //Más grueso que los brazos del rover
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.62f, 0.65f, 0.68f); //Otra vez usamos el color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();
		//PATA L PARA LA LLANTA ATRAS (CERCA) PT2
		model = modelaux;
		model = glm::translate(model, glm::vec3( 1.0f, 1.75f, 0.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; //Guardamos la rotación
		model = glm::scale(model, glm::vec3(0.6f, 3.25f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.62f, 0.65f, 0.68f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		//EJE PARA LA LLANTA DE ATRASO (CERCA)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.875f, 0.0f)); //Recordemos que tenemos el eje rotado
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion16()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		//LLANTA DE ATRAS (CERCA)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.6f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 0.3f)); //Primero escalo antes de rotar
		model = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f); //Las llantas seran de color blanco
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		//EJE PARA LA LLANTA DE ATRÁS (LEJOS)
		//IGUALES A LA ANTERIOR SOLO QUE EN LA PARTE DE ATRAS, POR LO TANTO Z = -Z
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.66f, -0.25f, -4.3f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion17()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		//PATA L PARA LA LLANTA DE EN MEDIO (LEJOS) PT1
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.33f, 0.0f, 0.3f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; //Guardamos la rotación
		model = glm::scale(model, glm::vec3(0.6f, 1.66f, 0.6f)); //Más grueso que los brazos del rover
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.62f, 0.65f, 0.68f); //Otra vez usamos el color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();
		//PATA L PARA LA LLANTA DE EN MEDIO (LEJOS) PT2
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.0f, 1.75f, 0.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; //Guardamos la rotación
		model = glm::scale(model, glm::vec3(0.6f, 3.25f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.62f, 0.65f, 0.68f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		//EJE PARA LA LLANTA DE EN MEDIO (LEJOS)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.875f, 0.0f)); //Recordemos que tenemos el eje rotado
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion18()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		//LLANTA DE EN MEDIO (LEJOS)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.6f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 0.3f)); //Primero escalo antes de rotar
		model = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f); //Las llantas seran de color blanco
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();



		//AQUÍ HACEMOS NUESTRA SONDA POR LO QUE TENEMOS QUE REINICIAR LA MATRIZ MODEL
		model = glm::mat4(1.0); //Reiniciamos nuestra matriz modelo
		modelaux2 = model; //Reiniciamos la matriz auxiliar
		//Para la sonda nos basaremos en el origen y luego la moveremos con las constantes de desplazamiento
		model = glm::translate(model, glm::vec3(0.0f + movX_Sonda, 0.0f + movY_Sonda, 0.0f + movZ_Sonda));  
		// Creando el "OJO" de la sonda
		model = glm::translate(model, glm::vec3(- 1.0f, 0.0f, 0.0f));// Posicionamos el "OJO"
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; //Guardamos los valores a heredar y la rotación
		model = glm::scale(model, glm::vec3(2.5f, 0.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución se programe cambio entre proyección ortogonal y perspectiva
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); //RECORDAD ESTE UNA SOLA VEZ
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.95f, 0.52f, 0.0f); //Color naranja :D 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry(); //dibuja cubo y pirámide triangular
		//meshList[3]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro, cono, pirámide base cuadrangular
		//sp.render(); //dibuja esfera

		// BASE
		model = modelaux;
		//NOS POSICIONAMOS EN EL CENTRO DE LA BASE
		model = glm::translate(model, glm::vec3( 0.0f, -2.25f, 0.0f));
		modelaux = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base
		model = glm::rotate(model, glm::radians(22.5f), glm::vec3(0.0f, 1.0f, 0.0f)); //No guardamos la rotación
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.15f, 0.18f, 0.22f); //Usamos el color gris oscurlo 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[5]->RenderMeshGeometry();
		model = modelaux;
		modelaux2 = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base

		//EJE PARA LA BISAGRA 1
		model = modelaux2;
		//Nos posicionamos a donde debe ir nuestra bisagra 
		model = glm::translate(model, glm::vec3( 0.0f, -2.0f, 3.7f)); //Nos movemos a la orilla , lejos del ojo, de la sonda
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacionsonda1()), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; //Los ejes vuelven a su posición original
		model = glm::scale(model, glm::vec3(0.2f, 3.06f, 0.2f)); //Escalamos el cilindro 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//PASAMOS MATRIZ MODELO
		color = glm::vec3( 0.0f, 0.0f, 1.0f); //Usamos el color azul de siempre
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();
		//PANEL DE LA BISAGRA 1
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		modelaux = model; 
		model = glm::scale(model, glm::vec3(0.2f, 5.0f, 6.0f)); //Escalamos el cubo para que sea un panel
		//modelaux = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base
		//model = glm::scale(model, glm::vec3(10.0f, 1.5f, 8.0f)); SE COMENTA PORQUE NO ES NECESARIO APLICAR LA ESCALA
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//PASAMOS MATRIZ MODELO
		color = glm::vec3(0.6f, 0.8f, 0.9f); //Usamos el color vidrio
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		//EJE PARA LA BISAGRA 2
		model = modelaux2;
		//Nos posicionamos a donde debe ir nuestra bisagra 
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -3.7f)); //Nos movemos a la orilla , lejos del ojo, de la sonda
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacionsonda2()), glm::vec3( 1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; //Los ejes vuelven a su posición original
		model = glm::scale(model, glm::vec3(0.2f, 3.06f, 0.2f)); //Escalamos el cilindro 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//PASAMOS MATRIZ MODELO
		color = glm::vec3(0.0f, 0.0f, 1.0f); //Usamos el color azul de siempre
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();
		//PANEL DE LA BISAGRA 2
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.2f, 5.0f, 6.0f)); //Escalamos el cubo para que sea un panel
		//modelaux = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base
		//model = glm::scale(model, glm::vec3(10.0f, 1.5f, 8.0f)); SE COMENTA PORQUE NO ES NECESARIO APLICAR LA ESCALA
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//PASAMOS MATRIZ MODELO
		color = glm::vec3(0.6f, 0.8f, 0.9f); //Usamos el color vidrio
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		//EJE PARA LA BISAGRA 3
		model = modelaux2;
		//Nos posicionamos a donde debe ir nuestra bisagra 
		model = glm::translate(model, glm::vec3(3.7f, -2.0f, 0.0f)); //Nos movemos a la orilla , lejos del ojo, de la sonda
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacionsonda3()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians( 90.f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model; //LOS EJES ESTAN ROTADOS
		model = glm::scale(model, glm::vec3(0.2f, 3.06f, 0.2f)); //Escalamos el cilindro 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//PASAMOS MATRIZ MODELO
		color = glm::vec3(0.0f, 0.0f, 1.0f); //Usamos el color azul de siempre
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();
		//PANEL DE LA BISAGRA 3
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.2f, 5.0f, 6.0f)); //Escalamos el cubo para que sea un panel
		//modelaux = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base
		//model = glm::scale(model, glm::vec3(10.0f, 1.5f, 8.0f)); SE COMENTA PORQUE NO ES NECESARIO APLICAR LA ESCALA
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//PASAMOS MATRIZ MODELO
		color = glm::vec3(0.6f, 0.8f, 0.9f); //Usamos el color vidrio
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		//EJE PARA LA BISAGRA 4
		model = modelaux2;
		//Nos posicionamos a donde debe ir nuestra bisagra 
		model = glm::translate(model, glm::vec3(-3.7f, -2.0f, 0.0f)); //Nos movemos a la orilla , lejos del ojo, de la sonda
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacionsonda4()), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(-90.f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model; //LOS EJES ESTAN ROTADOS
		model = glm::scale(model, glm::vec3(0.2f, 3.06f, 0.2f)); //Escalamos el cilindro 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//PASAMOS MATRIZ MODELO
		color = glm::vec3(0.0f, 0.0f, 1.0f); //Usamos el color azul de siempre
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry();
		//PANEL DE LA BISAGRA 4
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.2f, 5.0f, 6.0f)); //Escalamos el cubo para que sea un panel
		//modelaux = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base
		//model = glm::scale(model, glm::vec3(10.0f, 1.5f, 8.0f)); SE COMENTA PORQUE NO ES NECESARIO APLICAR LA ESCALA
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//PASAMOS MATRIZ MODELO
		color = glm::vec3(0.6f, 0.8f, 0.9f); //Usamos el color vidrio
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		