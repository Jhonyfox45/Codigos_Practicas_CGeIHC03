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
const float toRadians = 3.14159265f/180.0; //grados a radianes (Porque los valores que maneja OpenGL son en radianes)
Window mainWindow; //La nueva ventana
std::vector<Mesh*> meshList; //Creo  listas (estructura de datos) uso lista porque con el indice ya puedo acceder al dato  
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
//Le pasamos el nombre de los archivos (Los 4 que tenemos en shaders, ruta del disco duro. Es la ruta de apartir del proyecto (relativa?))
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crear�an ac�

float angulo = 0.0f;

//color caf� en RGB : 0.478, 0.255, 0.067

//Pir�mide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2, //Triangulo con los vertices 0,1,2
		1,3,2, //Triangulo con los vertices 1,2,3
		3,0,2,
		1,0,3
	//La ventaja de usar indices es que en lugar de usar todas las coordenadas de tus vertices puedes indicar el dibujado entre vertices
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3
		// X,  y   z 
	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//V�rtices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
	//Al usar indices no tengo que repetir  las coordenadas de los vertices sino solamente indicar el  dibujado entre vertices
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
	//Tengo los 8 vertices del cubo, solo necesito eso
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
                                  //Cuantos vertices (ndat en el arreglo de vertices), cuantos indices
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	/*
	GLfloat vertices_letras[] = {
			//X			Y			Z			R		G		B
			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			/*1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			
	};*///No lo necesitamos de momento

	/*MeshColor* letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,18);
	meshColorList.push_back(letras);
	*///Para este ejerccio de clase no necesitamos las letras

	//Definimos "coordendadas" y color de nuestras figuras
	//Más adelante lo que haremos sera cambiar su escala (tamaño), posición (traslación) y rotación
	GLfloat vertices_trianguloamarillo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		
	};

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.5f,	0.0f,

	};

	GLfloat vertices_triangulomagenta[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	1.0f,

	};
	// Ahora definimos la info de cada figura y la metemos en la lista, checar el orden en lista

	MeshColor* trianguloamarillo = new MeshColor(); //                      Indice 0
	trianguloamarillo->CreateMeshColor(vertices_trianguloamarillo, 18);
	meshColorList.push_back(trianguloamarillo);

	MeshColor* triangulorojo = new MeshColor();    //                       Indice 1
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	MeshColor* trianguloverde = new MeshColor();   //                       Indice 2
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	MeshColor* triangulomagenta = new MeshColor();   //                     Indice 3
	triangulomagenta->CreateMeshColor(vertices_triangulomagenta, 18);
	meshColorList.push_back(triangulomagenta);

//Volvemos a definir "coordenadas" y color pero ahora de los cuadrados
	GLfloat vertices_cuadradocafe[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		0.5f,	-0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		0.5f,	0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		-0.5f,	-0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		0.5f,	0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		-0.5f,	0.5f,		0.5f,			0.478f, 0.255f, 0.067f,

	};

	GLfloat vertices_cuadradoazul[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	1.0f,

	};

	GLfloat vertices_cuadradonegro[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	0.0f,

	};


	//Y lo guardamos nuevamente en la lista        // INDICE         4
	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe); //Para guardarlo en la lista

	MeshColor* cuadradoazul = new MeshColor();     // INDICE         5
	cuadradoazul->CreateMeshColor(vertices_cuadradoazul, 36);
	meshColorList.push_back(cuadradoazul); //Para guardarlo en la lista

	MeshColor* cuadradonegro = new MeshColor();    // INDICE         6
	cuadradonegro->CreateMeshColor(vertices_cuadradonegro, 36);
	meshColorList.push_back(cuadradonegro); //Para guardarlo en la lista


}


void CreateShaders()
{
	//Los mando a compilar y guardo mis shaders (recordar indice 0 para las listas)
	Shader *shader1 = new Shader(); //shader para usar �ndices: objetos: cubo y  pir�mide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
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
	//Projection: Matriz de Dimensi�n 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//No toma en cuenta la profundidad,todo lo que muestra lo hace en  2D
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);// Ortogonal, sin perspectiva

	//Podemos visualizar más al modificar estos valores
    //glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f); //Con perspectiva
						 //Angulo de apertura de la "camara", proyección de aspecto 1/1 = 800/800 que esa cercano a 1, Znear , Zfar

//Model: Matriz de Dimensi�n 4x4 en la cual se almacena la multiplicaci�n de las transformaciones geom�tricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	//Afuera porque solo quiero crear mi variable una vez, si lo meto en el while se creara una variable cada ciclo  y desvordo la memoria.

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.95f, 0.52f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad


		//Para las letras hay que usar el segundo set de shaders con �ndice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//Inicializar matriz de dimensi�n 4x4 que servir� como matriz de modelo para almacenar las transformaciones geom�tricas

		//Iniciamos con el triangulo amarillo
		model = glm::mat4(1.0); //reseteo la matriz de modelo
		model = glm::translate(model, glm::vec3(-0.65f, 0.115f, -4.0f)); //Datos de una traslación 
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //rotación de 180 grados en el eje Z
		model = glm::scale(model, glm::vec3(0.08f, 0.06f, 0.08f)); //Escalamiento en 0.08

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se env�an al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); //Solo se define una sola vez
		meshColorList[0]->RenderMeshColor(); //Triangulo Amarillo

		//Triangulo rojo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.65f, 0.0f, -4.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.06f, 0.08f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		//Triangulo verde
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.65f, -0.115f, -4.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.06f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[2]->RenderMeshColor();

		//1er Cuadrado cafe escalado para parecer un rectangulo largo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.745f, 0.027f, -4.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.4f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[4]->RenderMeshColor();

		//2do Cuadrado cafe escalado para parecer un rectangulo largo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.555f, 0.027f, -4.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.4f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[4]->RenderMeshColor();

		//Cuadrado azul "grande" rotado 45 grados
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[5]->RenderMeshColor();

		//Cadrado cafe "pequeño" rotado 45 grados
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.125f, 0.125f, 0.125f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[4]->RenderMeshColor();

		//Triangulo amarillo "chato"
		//Nota parece ser que las modificaciones deben seguir el siguiente orden para evitar problemas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.13f, 0.13f, -4.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //rotación de 45 grados en el eje Z
		model = glm::scale(model, glm::vec3(0.128f, 0.065f, 0.08f)); //Escalamiento para formar un triangulo más "chato"
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[0]->RenderMeshColor();

		//Triangulo rojo "chato"
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.13f, 0.13f, -4.0f));
		model = glm::rotate(model, 315 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.128f, 0.065f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		//Triangulo magenta "chato"
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.13f, -0.13f, -4.0f));
		model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.128f, 0.065f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[3]->RenderMeshColor();

		//Triangulo verde "chato"
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.13f, -0.13f, -4.0f));
		model = glm::rotate(model, 225 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.128f, 0.065f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[2]->RenderMeshColor();

		//Triangulo equilatero amarillo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.65f, -0.091f, -4.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.08f, 0.09f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[0]->RenderMeshColor();

		//Triangulo equilatero rojo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.74f, -0.091f, -4.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.08f, 0.09f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		//Triangulo equilatero verde
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.56f, -0.091f, -4.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.08f, 0.09f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[2]->RenderMeshColor();

		//Triangulo equilatero magenta
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.65f, 0.074f, -4.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.08f, 0.09f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[3]->RenderMeshColor();

		//Por ultimo el cuadrado negro que parece un rectangulo muy largo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.1975f, -4.0f));
		model = glm::scale(model, glm::vec3(1.8f, 0.05f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[6]->RenderMeshColor();

		/*Para el cubo y la pir�mide se usa el primer set de shaders con �ndice 0 en ShaderList
		shaderList[0].useShader(); 
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.1;
		Inicializar matriz de dimensi�n 4x4 que servir� como matriz de modelo para almacenar las transformaciones geom�tricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f)); //Dibujo a partir de -3
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f)); //rotación para cubo

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshList[1]->RenderMesh(); //Cambio el indice para dibujar      
		*/

		/*Piramide
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -3.0f)); //Dibujo a partir de -3
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f)); //rotación para la piramide

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh(); //Cambio el indice para dibujar 
		*/
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
/*
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/