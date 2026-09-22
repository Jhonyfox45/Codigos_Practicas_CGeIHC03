/*
Práctica 5: Optimización y Carga de Modelos
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList; //solo recibe xyz
std::vector<MeshColor*> meshListColor; // recibe xyz rgb
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;
//LA LISTA DE TODOS LOS MODELOS A IMPORTAR
Model Rover_Base_M;
Model Base_brazo_M;
Model Brazo1_M;
Model Brazo2_M;
Model Pinza_brazo_M;
Model Llanta_delantera_derecha_M;
Model Llanta_delantera_izquierda_M;
Model Llanta_media_derecha_M;
Model Llanta_media_izquierda_M;
Model Llanta_trasera_derecha_M;
Model Llanta_trasera_izquierda_M;
//Lista de Skybox a crear
Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";

//USAMOS CONSTANTES PARA HACER MÁS FACIL EL DESPLAZAMIENTO
const float movX_Rover = 0.0f;
const float movY_Rover = 1.5f;
const float movZ_Rover = 0.0f;

//const float movX_Holocron = 0.0f;
//const float movY_Holocron = 0.0f;
//const float movZ_Holocron = 0.0f;

//const float movX_Satelite = 0.0f;
//const float movY_Satelite = 0.0f;
//const float movZ_Satelite = 0.0f;

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	
	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);


}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);
	//TODOS LOS MODELOS QUE DEBIMOS CARGAR
	Rover_Base_M = Model();
	Rover_Base_M.LoadModel("Models/Base_Rover.obj");
	Base_brazo_M = Model();
	Base_brazo_M.LoadModel("Models/Base_brazo_obj.obj");
	Brazo1_M = Model();
	Brazo1_M.LoadModel("Models/Brazo1.obj");
	Brazo2_M = Model();
	Brazo2_M.LoadModel("Models/Brazo2.obj");
	Pinza_brazo_M = Model();
	Pinza_brazo_M.LoadModel("Models/Pinza_brazo.obj");
	Llanta_delantera_derecha_M = Model();
	Llanta_delantera_derecha_M.LoadModel("Models/Llanta_delantera_derecha.obj");
	Llanta_delantera_izquierda_M = Model();
	Llanta_delantera_izquierda_M.LoadModel("Models/Llanta_delantera_izquierda.obj");
	Llanta_media_derecha_M = Model();
	Llanta_media_derecha_M.LoadModel("Models/Llanta_media_derecha.obj");
	Llanta_media_izquierda_M = Model();
	Llanta_media_izquierda_M.LoadModel("Models/Llanta_media_izquierda.obj");
	Llanta_trasera_derecha_M = Model();
	Llanta_trasera_derecha_M.LoadModel("Models/Llanta_trasera_derecha.obj");
	Llanta_trasera_izquierda_M = Model();
	Llanta_trasera_izquierda_M.LoadModel("Models/Llanta_trasera_izquierda.obj");
	//Crear Skybox con sus 6 texturas
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshListModel[2]->RenderMeshModel();

		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
		//Modelo Inicial
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + movX_Rover, 1.5f + movY_Rover, 0.0f + movZ_Rover));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.1f, 0.1f); //Rojo brillante
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Rover_Base_M.RenderModel();
		modelaux = model;
		modelaux2 = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base

		//Brazo
		//Renderizando la base del brazo
		modelaux2 = model;
		model = glm::translate(model, glm::vec3( 3.0f, 1.5f, -1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.1f, 0.8f, 0.1f); //Verde brillante
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Base_brazo_M.RenderModel();
		//Renderizamos el primer brazo
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.05f, 1.05f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.1f, 0.3f, 0.9f);// Azul vibrante
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Brazo1_M.RenderModel();
		//Rotamos el eje para facilitar la colocación del otro brazo
		model = modelaux;
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		//Renderizamos el segundo brazo
		model = modelaux;
		model = glm::translate(model, glm::vec3( 3.65f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelaux = model;
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f,-1.0f));// Anula la rotación de la pieza
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.9f, 0.1f);// Amarillo puro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Brazo2_M.RenderModel();
		//Renderizamos las pinzas
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 4.65f, 0.1f)); //Pequeña correción del eje Z
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelaux = model;
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, -1.0f));// Anula la rotación de la pieza
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.9f, 0.1f, 0.6f);// Magenta
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Pinza_brazo_M.RenderModel();
		//TERMINAMOS DE RENDERIZAR EL BRAZO CON SUS RESPECTIVOS EJES

		//	Llanta delantera derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(3.0f, 0.40f, -3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.8f, 0.8f); //Cian
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Llanta_delantera_derecha_M.RenderModel();
		//	Llanta delantera izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(3.0f, 0.40f, 2.86f)); //Pequeña correción en Z
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.5f, 0.0f); //Naranja
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Llanta_delantera_izquierda_M.RenderModel();

		//Llanta media derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-2.3f, -0.1f, -3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.0f, 0.8f); //Morado
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Llanta_media_derecha_M.RenderModel();
		// Llanta media izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-2.3f, -0.1f, 2.86f)); //Pequeña correción en Z
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.6f, 1.0f, 0.1f); //Verde lima
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Llanta_media_izquierda_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.8f, -0.1f, -3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.6f, 0.3f, 0.1f); //Marrón
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Llanta_trasera_derecha_M.RenderModel();
		//	Llanta trasera izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.8f, -0.1f, 2.86f)); //Pequeña correción en Z
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.4f, 0.6f); //Rosa coral
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Llanta_trasera_izquierda_M.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
