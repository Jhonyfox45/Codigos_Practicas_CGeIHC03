//Pr�ctica 2: �ndices, mesh, proyecciones, transformaciones geom�tricas
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
//clases para dar orden y limpieza al c�digo
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;

//Vertex Shader
//Modificamos solamente el vShader para cada color
static const char* fShader = "shaders/shader.frag";
static const char* vShaderAmarillo = "shaders/shaderamarillo.vert"; //Amarillo
static const char* vShaderRojo = "shaders/shaderrojo.vert";         //Rojo
static const char* vShaderVerde = "shaders/shaderverde.vert";       //Verde
static const char* vShaderMagenta = "shaders/shadermagenta.vert";   //Magenta
static const char* vShaderCafe = "shaders/shadercafe.vert";         //Cafe
static const char* vShaderAzul = "shaders/shaderazul.vert";         //Azul
static const char* vShaderNegro = "shaders/shadernegro.vert";       //Negro

//Dejamos estos dos tal cual porque los necesitamos para las letras
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crear�an ac�

float angulo = 0.0f;

//color caf� en RGB : 0.478, 0.255, 0.067

//Modificamos la piramide para que tenga base cuadrada
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,3,
		0,1,4,
		1,2,4,
		2,3,4,
		3,0,4,
		
	};
	GLfloat vertices[] = {
	   -0.5f, 0.0f, -0.5f,	//0
	   -0.5f, 0.0f,  0.5f,	//1
		0.5f, 0.0f,  0.5f,	//2
		0.5f, 0.0f, -0.5f,	//3
		0.0f, 1.0f,  0.0f,  //4
	//   X     Y      Z
	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 15, 16);
	meshList.push_back(obj1);
}

//V�rtices de un cubo
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
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	//Metemos los vertices de nuestras letras de la práctica anterior
	//Aquí mismo cambiamos el color de cada letra
	GLfloat vertices_letras[] = {	
		// X     Y      Z                R     G     B
		//Triangulo 1
		-0.95f, 0.90f, 0.0f,           0.7f, 1.0f, 0.2f, //Verde distinto al que usaremos en las piramides (verde limón) 
		-0.35f, 0.90f, 0.0f,           0.7f, 1.0f, 0.2f,
		-0.95f, 0.80f, 0.0f,           0.7f, 1.0f, 0.2f,
		//Triangulo 2
		-0.95f, 0.80f, 0.0f,           0.7f, 1.0f, 0.2f,
		-0.35f, 0.90f, 0.0f,           0.7f, 1.0f, 0.2f,
		-0.35f, 0.80f, 0.0f,           0.7f, 1.0f, 0.2f,

		//Triangulo 3
		-0.30f, 0.90f, 0.0f,           0.15f, 0.45f, 0.85f, //Azul distinto al que usaremos en el cubo (azul cielo)
		 0.30f, 0.90f, 0.0f,           0.15f, 0.45f, 0.85f,
		-0.30f, 0.80f, 0.0f,           0.15f, 0.45f, 0.85f,
		//Triangulo 4
		-0.30f, 0.80f, 0.0f,           0.15f, 0.45f, 0.85f,
		 0.30f, 0.90f, 0.0f,           0.15f, 0.45f, 0.85f,
		 0.30f, 0.80f, 0.0f,           0.15f, 0.45f, 0.85f,

		 //Triangulo 5
		 0.52f, 0.90f, 0.0f,           0.5f, 0.0f, 0.8f,  //Morado distinto al magenta que usaremos en las piramides
		 0.78f, 0.90f, 0.0f,           0.5f, 0.0f, 0.8f,
		 0.60f, 0.80f, 0.0f,           0.5f, 0.0f, 0.8f,
		 //Triangulo 6
		 0.60f, 0.80f, 0.0f,           0.5f, 0.0f, 0.8f,
		 0.78f, 0.90f, 0.0f,           0.5f, 0.0f, 0.8f,
		 0.70f, 0.80f, 0.0f,           0.5f, 0.0f, 0.8f,

		 //Triangulo 7
		-0.60f, 0.80f, 0.0f,           0.7f, 1.0f, 0.2f,
		-0.70f, 0.80f, 0.0f,           0.7f, 1.0f, 0.2f,
		-0.60f,-0.90f, 0.0f,           0.7f, 1.0f, 0.2f,
		 //Triangulo 8
		-0.60f,-0.90f, 0.0f,           0.7f, 1.0f, 0.2f,
		-0.70f, 0.80f, 0.0f,           0.7f, 1.0f, 0.2f,
		-0.70f,-0.90f, 0.0f,           0.7f, 1.0f, 0.2f,

		 //Triangulo 9
		-0.30f, 0.80f, 0.0f,           0.15f, 0.45f, 0.85f,
		-0.20f, 0.80f, 0.0f,           0.15f, 0.45f, 0.85f,
		-0.30f,-0.90f, 0.0f,           0.15f, 0.45f, 0.85f,
		 //Triangulo 10
		-0.30f,-0.90f, 0.0f,           0.15f, 0.45f, 0.85f,
		-0.20f, 0.80f, 0.0f,           0.15f, 0.45f, 0.85f,
		-0.20f,-0.90f, 0.0f,           0.15f, 0.45f, 0.85f,

		 //Triangulo 11
		 0.52f, 0.90f, 0.0f,           0.5f, 0.0f, 0.8f,
		 0.35f,-0.90f, 0.0f,           0.5f, 0.0f, 0.8f,
		 0.60f, 0.80f, 0.0f,           0.5f, 0.0f, 0.8f,
		 //Triangulo 12
		 0.35f,-0.90f, 0.0f,           0.5f, 0.0f, 0.8f,
		 0.45f,-0.90f, 0.0f,           0.5f, 0.0f, 0.8f,
		 0.60f, 0.80f, 0.0f,           0.5f, 0.0f, 0.8f,

		 //Triangulo 13
		-0.70f,-0.90f, 0.0f,           0.7f, 1.0f, 0.2f,
		-0.70f,-0.80f, 0.0f,           0.7f, 1.0f, 0.2f,
		-0.95f,-0.80f, 0.0f,           0.7f, 1.0f, 0.2f,
		 //Triangulo 14
		-0.95f,-0.90f, 0.0f,           0.7f, 1.0f, 0.2f,
		-0.70f,-0.90f, 0.0f,           0.7f, 1.0f, 0.2f,
		-0.95f,-0.80f, 0.0f,           0.7f, 1.0f, 0.2f,

		 //Triangulo 15
		-0.30f,-0.90f, 0.0f,           0.15f, 0.45f, 0.85f,
		 0.30f,-0.90f, 0.0f,           0.15f, 0.45f, 0.85f,
		-0.30f,-0.80f, 0.0f,           0.15f, 0.45f, 0.85f,
		 //Triangulo 16
		-0.30f,-0.80f, 0.0f,           0.15f, 0.45f, 0.85f,
		 0.30f,-0.90f, 0.0f,           0.15f, 0.45f, 0.85f,
		 0.30f,-0.80f, 0.0f,           0.15f, 0.45f, 0.85f,

		 //Triangulo 17
		 0.78f, 0.90f, 0.0f,           0.5f, 0.0f, 0.8f,
		 0.95f,-0.90f, 0.0f,           0.5f, 0.0f, 0.8f,
		 0.70f, 0.80f, 0.0f,           0.5f, 0.0f, 0.8f,
		 //Triangulo 18
		 0.95f,-0.90f, 0.0f,           0.5f, 0.0f, 0.8f,
		 0.85f,-0.90f, 0.0f,           0.5f, 0.0f, 0.8f,
		 0.70f, 0.80f, 0.0f,           0.5f, 0.0f, 0.8f,

		 //Triangulo 19
		-0.95f, 0.00f, 0.0f,           0.7f, 1.0f, 0.2f,
		-0.85f,-0.80f, 0.0f,           0.7f, 1.0f, 0.2f,
		-0.95f,-0.80f, 0.0f,           0.7f, 1.0f, 0.2f,
		 //Triangulo 20
		-0.85f, 0.00f, 0.0f,           0.7f, 1.0f, 0.2f,
		-0.95f, 0.00f, 0.0f,           0.7f, 1.0f, 0.2f,
		-0.85f,-0.80f, 0.0f,           0.7f, 1.0f, 0.2f,

		 //Triangulo 21
		-0.20f, 0.05f, 0.0f,           0.15f, 0.45f, 0.85f,
		-0.20f,-0.05f, 0.0f,           0.15f, 0.45f, 0.85f,
		 0.20f,-0.05f, 0.0f,           0.15f, 0.45f, 0.85f,
		 //Triangulo 22
		 0.20f,-0.05f, 0.0f,           0.15f, 0.45f, 0.85f,
		-0.20f, 0.05f, 0.0f,           0.15f, 0.45f, 0.85f,
		 0.20f, 0.05f, 0.0f,           0.15f, 0.45f, 0.85f,

		 //Triangulo 23
		 0.52f, 0.05f, 0.0f,           0.5f, 0.0f, 0.8f,
		 0.52f,-0.05f, 0.0f,           0.5f, 0.0f, 0.8f,
		 0.78f,-0.05f, 0.0f,           0.5f, 0.0f, 0.8f,
		 //Triangulo 24
		 0.78f,-0.05f, 0.0f,           0.5f, 0.0f, 0.8f, 
		 0.52f, 0.05f, 0.0f,           0.5f, 0.0f, 0.8f,
		 0.78f, 0.05f, 0.0f,           0.5f, 0.0f, 0.8f, 
			
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,432); //Mofificamos el número de variables de vertices 
	//Un total de 144 pq -> 24 triangulos * 18 atributos de vertices cada uno = 432
	meshColorList.push_back(letras);
	//Ya no necesitamos las otras figuras, los triangulos y cuadrados
}


void CreateShaders()
{
	//Indice 0
	Shader *shaderColor = new Shader();//shader para usar color como parte del VAO: letras 
	shaderColor->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shaderColor);

	//Indice 1
	Shader *shaderAmarillo = new Shader(); //shader Amarillo
	shaderAmarillo->CreateFromFiles(vShaderAmarillo, fShader);
	shaderList.push_back(*shaderAmarillo);

	//Indice 2
	Shader* shaderRojo = new Shader(); //shader Rojo
	shaderRojo->CreateFromFiles(vShaderRojo, fShader);
	shaderList.push_back(*shaderRojo);

	//Indice 3
	Shader* shaderVerde = new Shader(); //shader Verde 
	shaderVerde->CreateFromFiles(vShaderVerde, fShader);
	shaderList.push_back(*shaderVerde);

	//Indice 4
	Shader* shaderMagenta = new Shader(); //shader Magenta 
	shaderMagenta->CreateFromFiles(vShaderMagenta, fShader);
	shaderList.push_back(*shaderMagenta);

	//Indice 5
	Shader* shaderCafe = new Shader(); //shader Cafe 
	shaderCafe->CreateFromFiles(vShaderCafe, fShader);
	shaderList.push_back(*shaderCafe);

	//Indice 6
	Shader* shaderAzul = new Shader(); //shader Azul 
	shaderAzul->CreateFromFiles(vShaderAzul, fShader);
	shaderList.push_back(*shaderAzul);

	//Indice 7
	Shader* shaderNegro = new Shader(); //shader Negro 
	shaderNegro->CreateFromFiles(vShaderNegro, fShader);
	shaderList.push_back(*shaderNegro);

}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	CreaPiramide(); //�ndice 0 en MeshList
	CrearCubo();//�ndice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, �ndices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Para la practica dejamos la proyección en perspectiva al usar cubos y piramides
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	glm::mat4 model(1.0); 
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.95f, 0.52f, 0.0f, 1.0f);//Color del fondo de la ventana en naranja
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
													
		//Para las letras hay que usar el PRIMER set de shaders con �ndice 0 en ShaderList 
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		
		//Mandamos a modificar la escala y traslación de nuestras letras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f)); //Lo dejamos en el origen pero en el fondo 
		model = glm::scale(model, glm::vec3(2.4f, 2.4f, 2.4f)); //Escalamiento para que nuestras letras ocupen más pantalla

		//Renderizamos nuestras Letras
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se env�an al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor(); //Mandamos a renderizar nuestras letras

		//Para el cubo y la pirámide se usa DEL SEGUNDO SET AL OCTAVO de shaders con índice 1 a 7 en ShaderList
		shaderList[1].useShader(); //CAMBIAR INDICE
		uniformModel = shaderList[1].getModelLocation(); //CAMBIAR INDICE
		uniformProjection = shaderList[1].getProjectLocation(); //CAMBIAMOS EL SHADER PARA EL COLOR INDICES DEL 1 A 7
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas

		//NOTA:Tuvimos que modificar la variable Z de la traslación ya que ahora si tenemos profundidad TODAS LAS FIGURAS tendran Z= -1.4
		//Hacemos la piramide Amarilla
		model = glm::mat4(1.0); //reseteo la matriz de modelo
		model = glm::translate(model, glm::vec3(-0.65f, 0.06f, -1.4f)); //Datos de una traslación 
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //rotación de 180 grados en el eje Z
		model = glm::scale(model, glm::vec3(0.08f, 0.06f, 0.08f)); //Escalamiento en 0.08
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh(); //Indice 0 ->Piramide e Indice 1 ->Cubo

		//Hacemos la piramide roja
		shaderList[2].useShader(); //CAMBIAR INDICE
		uniformModel = shaderList[2].getModelLocation(); 
		uniformProjection = shaderList[2].getProjectLocation(); 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.65f, 0.0f, -1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.06f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Hacemos la piramide verde
		shaderList[3].useShader(); 
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.65f, -0.06f, -1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.06f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		
		//Hacemos el 1er cubo cafe que será como un rectangulo
		shaderList[5].useShader(); 
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.73f,-0.009f, -1.4f));
		model = glm::scale(model, glm::vec3(0.03f, 0.215f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Hacemos el 2o cubo cafe que será como un rectangulo
		shaderList[5].useShader(); 
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.57f,-0.009f, -1.4f));
		model = glm::scale(model, glm::vec3(0.03f, 0.215f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		
		//Hacemos el cubo azul "Grande" Rotado 45 grados en Z
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.02f, -1.4f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.12f, 0.12f, 0.12f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Hacemos el cubo cafe "pequeño" Rotado 45 grados en Z
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.02f, -1.35f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.06f, 0.06f, 0.06f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Hacemos la piramide amarilla "chata"
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.04f, 0.028f, -1.4f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //rotación de 45 grados en el eje Z
		model = glm::scale(model, glm::vec3(0.128f, 0.065f, 0.128f)); //Escalamiento para formar un triangulo más "chato"
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Hacemos la piramide roja "chata"
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.04f, 0.028f, -1.4f));
		model = glm::rotate(model, 315 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.128f, 0.065f, 0.128f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Hacemos la piramide Magenta "chata"
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.04f, -0.067f, -1.4f));
		model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.128f, 0.065f, 0.128f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Hacemos la piramide Verde "chata"
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.04f, -0.067f, -1.4f));
		model = glm::rotate(model, 225 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.128f, 0.065f, 0.128f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Piramide equilatera amarilla
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.65f, -0.035f, -1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.08f, 0.09f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Piramide equilatera roja
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.71f, -0.11f, -1.4f));
		model = glm::scale(model, glm::vec3(0.09f, 0.08f, 0.09f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Piramide equilatera verde
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.59f, -0.11f, -1.4f));
		model = glm::scale(model, glm::vec3(0.09f, 0.08f, 0.09f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Piramide equilatera magenta
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.65f, -0.035f, -1.4f));
		model = glm::scale(model, glm::vec3(0.09f, 0.08f, 0.09f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Por ultimo el cuadrado negro que parece un rectangulo muy largo
		shaderList[7].useShader();
		uniformModel = shaderList[7].getModelLocation();
		uniformProjection = shaderList[7].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.128f, -1.4f));
		model = glm::scale(model, glm::vec3(1.7f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		
		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslaci�n
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACI�N //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/