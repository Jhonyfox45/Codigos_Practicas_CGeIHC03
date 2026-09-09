//práctica 3: Modelado Geométrico y Cámara Sintética.
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
#include"Sphere.h" //la esfera existe como una clase por separado
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
std::vector<MeshColor*> meshColorList;
vector<Shader>shaderList;
//Vertex Shader
//Cambiamos el shadercolor por el nuestro, shaderColor3D
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shaderColor3D.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks
// Aca se le pasan los datos, R=1, Resolución vertical = 20 y horizontal = 20)

//DEFINO UN PAR DE CONSTANTES PARA DEZPLAZAR MIS MODELOS
const float movX_Cohete = -0.2f;
const float movY_Cohete = 0.0f;
const float movZ_Cohete = 0.0f;

const float movX_Pir8 = 0.2f;
const float movY_Pir8 = 0.0f;
const float movZ_Pir8 = 0.0f;
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
	Mesh* piramidet = new Mesh();
	piramidet->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(piramidet);

}


//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	unsigned int piramidecuadrangular_indices[] = {
		0,1,2, //Cara cuadrada 1er Triangulo
		0,3,2, //Cara cuadrada 2do Triangulo
		0,1,4,   //Lateral Izquierda
		1,2,4,   //Lateral Frente
		2,3,4,   //Lateral Derecha
		3,0,4,  //Lateral Trasera

	};
	GLfloat piramidecuadrangular_vertices[] = {
	   -0.5f, 0.0f, -0.5f,	//0
	   -0.5f, 0.0f,  0.5f,	//1
		0.5f, 0.0f,  0.5f,	//2
		0.5f, 0.0f, -0.5f,	//3
		0.0f, 1.0f,  0.0f,  //4
		//   X     Y      Z
	};
	Mesh* piramidec = new Mesh();
	piramidec->CreateMesh(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramidec);
}



/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
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

	//ciclo for para crear la circunferencia inferior //Cilindro
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

	//ciclo for para crear la circunferencia superior //Cilindro
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
		x = R * cos((n)*dt); //Estas eran o son las ecuaciones
		z = R * sin((n)*dt); //Estas son las ecuaciones 
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

//AÑADIMOS UNA FUNCIÓN PARA CREAR UNA PIRAMIDE DE BASE CUADRADA CON CADA CARA DE UN COLOR DIFERENTE
void CrearPiramideCuadrangularColorida()
{
// Definimos cada triángulo por separado sin compartir vértices
	GLfloat piramide_vertices[] = {
	    //X      Y      Z          R     G     B
		//Lateral Frente VERDE
		-0.5f, -0.5f,  0.5f,      0.0f, 1.0f, 0.0f, 
		 0.5f, -0.5f,  0.5f,      0.0f, 1.0f, 0.0f, 
		 0.0f,  1.0f,  0.0f,      0.0f, 1.0f, 0.0f, 

		//Lateral Derecha AMARILLA
		 0.5f, -0.5f,  0.5f,      1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,      1.0f, 1.0f, 0.0f, 
		 0.0f,  1.0f,  0.0f,      1.0f, 1.0f, 0.0f, 

		//Lateral Trasera MAGENTA
		 0.5f, -0.5f, -0.5f,      1.0f, 0.0f, 1.0f, 
		-0.5f, -0.5f, -0.5f,      1.0f, 0.0f, 1.0f, 
		 0.0f,  1.0f,  0.0f,      1.0f, 0.0f, 1.0f, 

		//Lateral Izquierda ROJA
		-0.5f, -0.5f, -0.5f,      1.0f, 0.0f, 0.0f, 
		-0.5f, -0.5f,  0.5f,      1.0f, 0.0f, 0.0f, 
		 0.0f,  1.0f,  0.0f,      1.0f, 0.0f, 0.0f, 

		//Base Cuadrada AZUL
		-0.5f, -0.5f,  0.5f,      0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,      0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,      0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,      0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,      0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,      0.0f, 0.0f, 1.0f
	};

	MeshColor* piramide = new MeshColor();
	// Son 18 vértices * 6 atributos por vértice = 108 datos
	piramide->CreateMeshColor(piramide_vertices, 108); 
	meshColorList.push_back(piramide); //meshColorList Indice = 0
}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(25, 1.0f);//índice 2 en MeshList
	            //Resolución y radio 
	//Para hacerlos más altos tenemos que forzosamente escalar, se podria modificar la altura en la función pero no es recomendable
	CrearCono(25, 1.0f);//índice 3 en MeshList
	         //Resolución y radio 
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CrearPiramideCuadrangularColorida(); //índice 0 en MeshListColor
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
	                                                                                       //PODEMOS MODIFICAR ESTOS ULTIMOS DOS
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.025f, 0.2f);
	//Los ultimos 3.0 son que tan rapido me muevo y el otro que tan rapido giro (sensibilidad)
	
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
		glClearColor(0.95f, 0.52f, 0.0f, 1.0f);//Fondo naranja
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		//model = glm::mat4(1.0);
		//Traslación inicial para posicionar en -Z a los objetos
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//otras transformaciones para el objeto
		//model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(0.95f, 0.52f, 0.0f); //Cambia el color
		//uniform color hace que todo lo que se dibuje despues de el sea del color previamente definido
		//Toda la geometria la pinta del mismo color
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos

		//meshList[0]->RenderMesh(); //dibuja cubo, pirámide triangular y pirámide base cuadrangular
		//Cambiar el indice de meshlist para cambiar las figuras

		//meshList[3]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro, cono
		//sp.render(); //dibuja esfera

		//A PARTIR DE ACA HACEMOS LAS FIGURAS PARA EL COHETE Y LAS 8 PIRAMIDES
		
		//PLATAFORMA PARA EL COHETE
		//Hacemos la piramide Amarilla
		model = glm::mat4(1.0); 
		model = glm::translate(model, glm::vec3(0.0f + movX_Cohete, 0.06f + movY_Cohete, -1.5f + movZ_Cohete)); 
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); 
		model = glm::scale(model, glm::vec3(0.08f, 0.06f, 0.08f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); //DEFINIR UNA SOLA VEZ 
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 1.0f, 0.0f);  //YA NO USAMOS SHADERS AHORA USAMOS ESTAS DOS LINEAS
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMesh();
		//Hacemos la piramide roja
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + movX_Cohete, 0.0f + movY_Cohete, -1.5f + movZ_Cohete));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.06f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);  
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMesh();
		//Hacemos la piramide verde
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + movX_Cohete, -0.06f + movY_Cohete, -1.5f + movZ_Cohete));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.06f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMesh();
		//Hacemos el 1er cubo cafe que será como un rectangulo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.055f + movX_Cohete, -0.009f + movY_Cohete, -1.5f + movZ_Cohete));
		model = glm::scale(model, glm::vec3(0.03f, 0.215f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.478f, 0.255f, 0.067f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		//Hacemos el 2o cubo cafe que será como un rectangulo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.055f + movX_Cohete, -0.009f + movY_Cohete, -1.5f + movZ_Cohete));
		model = glm::scale(model, glm::vec3(0.03f, 0.215f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.478f, 0.255f, 0.067f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//AHORA HACEMOS NUESTRAS FIGURAS CON AYUDA DE LAS NUEVAS FIGURAS
		//TANQUE 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + movX_Cohete, -0.01f + movY_Cohete, -1.428f + movZ_Cohete));
		model = glm::scale(model, glm::vec3(0.025f, 0.125f, 0.025f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.62f, 0.65f, 0.68f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		//PROPULSOR DEL TANQUE 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + movX_Cohete, -0.07f + movY_Cohete, -1.428f + movZ_Cohete));
		model = glm::scale(model, glm::vec3(0.02f, 0.08f, 0.02f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.15f, 0.18f, 0.22f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMeshGeometry();
		//TANQUE 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.045f + movX_Cohete, -0.03f + movY_Cohete, -1.428f + movZ_Cohete));
		model = glm::scale(model, glm::vec3(0.025f, 0.125f, 0.025f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.62f, 0.65f, 0.68f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		//PUNTA EL TANQUE 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.045f + movX_Cohete, 0.04f + movY_Cohete, -1.415f + movZ_Cohete));
		model = glm::scale(model, glm::vec3(0.04f, 0.03f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.94f, 0.94f, 0.94f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();
		//PROPULSOR DEL TANQUE 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.045f + movX_Cohete, -0.085f + movY_Cohete, -1.428f + movZ_Cohete));
		model = glm::scale(model, glm::vec3(0.02f, 0.06f, 0.02f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.15f, 0.18f, 0.22f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMeshGeometry();
		//TANQUE 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.045f + movX_Cohete, -0.03f + movY_Cohete, -1.428f + movZ_Cohete));
		model = glm::scale(model, glm::vec3(0.025f, 0.115f, 0.025f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.62f, 0.65f, 0.68f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		//PUNTA EL TANQUE 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.045f + movX_Cohete, 0.04f + movY_Cohete, -1.415f + movZ_Cohete));
		model = glm::scale(model, glm::vec3(0.04f, 0.03f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.94f, 0.94f, 0.94f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();
		//PROPULSOR DEL TANQUE 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.045f + movX_Cohete, -0.085f + movY_Cohete, -1.428f + movZ_Cohete));
		model = glm::scale(model, glm::vec3(0.02f, 0.06f, 0.02f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.15f, 0.18f, 0.22f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMeshGeometry();
		//CAPSULA CUADRADA
		model = glm::mat4(1.0); // Reseteamos la matriz
		model = glm::translate(model, glm::vec3(0.0f + movX_Cohete, 0.07f + movY_Cohete, -1.428f + movZ_Cohete));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.54f, 0.22f, 0.89f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		//PUNTA PIRAMIDE TRIANGULAR DE LA CAPSULA
		model = glm::mat4(1.0); // Reseteamos la matriz
		model = glm::translate(model, glm::vec3(0.0f + movX_Cohete, 0.095f + movY_Cohete, -1.428f + movZ_Cohete));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.92f, 0.12f, 0.15f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMesh();
		// CUPULA DE "VIDRIO"
		model = glm::mat4(1.0); // Reseteamos la matriz
		model = glm::translate(model, glm::vec3(0.0f + movX_Cohete, 0.07f + movY_Cohete, -1.428f + movZ_Cohete));
		model = glm::scale(model, glm::vec3(0.033f, 0.033f, 0.033f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.6f, 0.8f, 0.9f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		//Piso de color negro (FIJO)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.134f, -1.4f));
		model = glm::scale(model, glm::vec3(1.7f, 0.03f, 1.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();        
	
		//ACA HACEMOS NUESTRA UNION DE 8 PIRAMIDES 
		// NOTA: SE COLOCA AL ULTIMO PARA EVITAR PROBLEMAS CON LOS SHADERS
		shaderList[1].useShader(); // Usamos el shaderColor
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//Primer piramide del 1er par de piramides
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + movX_Pir8, -0.034f + movY_Pir8, -1.4f + movZ_Pir8));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.035f, 0.09f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();
		//Segunda Piramide del 1er par de piramides
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + movX_Pir8, 0.0f + movY_Pir8, -1.4f + movZ_Pir8));
		model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.035f, 0.09f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();
		//Primer piramide del 2do par de piramides
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + movX_Pir8, 0.071f + movY_Pir8, -1.4f + movZ_Pir8));
		model = glm::rotate(model, 225 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.035f, 0.09f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();
		//Segunda Piramide del 2do par de piramides
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + movX_Pir8, 0.105f + movY_Pir8, -1.4f + movZ_Pir8));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.035f, 0.09f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();
		//Primer piramide del 3er par de piramides
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.036f + movX_Pir8, 0.036f + movY_Pir8, -1.4f + movZ_Pir8));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.035f, 0.09f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();
		//Segunda Piramide del 3er par de piramides
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.072f + movX_Pir8, 0.036f + movY_Pir8, -1.4f + movZ_Pir8));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.035f, 0.09f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();
		//Primer piramide del 4to par de piramides
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.072f + movX_Pir8, 0.036f + movY_Pir8, -1.4f + movZ_Pir8));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.035f, 0.09f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();
		//Segunda Piramide del 4to par de piramides
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.036f + movX_Pir8, 0.036f + movY_Pir8, -1.4f + movZ_Pir8));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.035f, 0.09f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();

	
		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		