//pr�ctica 3: Modelado Geom�trico y C�mara Sint�tica.
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
//clases para dar orden y limpieza al c�digo
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
const float toRadians = 3.14159265f / 180.0; //grados a radianes
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
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks


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

// Pir�mide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2, 
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		0.0f,0.0f,2.0f,	//0
		1.73f,0.0f,1.0f,	//1
		0.0f,0.0f,0.0f,	//2
		0.58,1.63,1.0f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}

// Pir�mide triangular regular Alrevez
void CrearPiramideTriangularAlrevez()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		0.58f,1.63f,0.0f,	//0
		0.58f,1.63f,2.0f,	//1
		0.0f,0.0f,1.0f,	//2
		1.73,0.0,1.0f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}

/*
Crear cilindro, cono y esferas con arreglos din�micos vector creados en el Semestre 2023 - 1 : por S�nchez P�rez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los v�rtices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el c�rculo
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
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

	//Se generan los indices de los v�rtices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//funci�n para crear un cono
void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	//ciclo for para crear los v�rtices de la circunferencia del cono
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


	for (i = 0; i < res + 2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//funci�n para crear pir�mide cuadrangular unitaria
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
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	//Cilindro y cono reciben resoluci�n (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//�ndice 0 en MeshList
	CrearPiramideTriangular();//�ndice 1 en MeshList
	CrearPiramideTriangularAlrevez();//�ndice 2 en MeshList
	CrearCilindro(50, 1.0f);//�ndice 3 en MeshList
	CrearCono(3, 1.0f);//�ndice 4 en MeshList
	CrearPiramideCuadrangular();//�ndice 5 en MeshList
	CreateShaders();



	/*C�mara se usa el comando: glm::lookAt(vector de posici�n, vector de orientaci�n, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posici�n,
	glm::vec3 vector up,
	GlFloat yaw rotaci�n para girar hacia la derecha e izquierda
	GlFloat pitch rotaci�n para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posici�n inicial est� en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.15f, 0.4f);


	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);

	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//C�mara
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

		model = glm::mat4(1.0);
		//Traslaci�n inicial para posicionar en -Z a los objetos
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//otras transformaciones para el objeto
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la l�nea de proyecci�n solo se manda una vez a menos que en tiempo de ejecuci�n
		//se programe cambio entre proyecci�n ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh(); //dibuja cubo y pir�mide triangular
		meshList[1]->RenderMesh(); //dibuja las figuras geom�tricas cilindro, cono, pir�mide base cuadrangular
		//sp.render(); //dibuja esfera


			//ejercicio: Instanciar primitivas geom�tricas para recrear el dibujo de la pr�ctica pasada en 3D,
		//se requiere que exista piso y la casa tiene una ventana azul circular justo en medio de la pared trasera y solo 1 puerta frontal.

		//TR1
		{
			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 0.0f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(0.9f, 2.15f, -2.45f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TR2
		{
			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 0.0f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(0.56f, 1.11f, -1.8f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TR3
		{
			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 0.0f, 0.0f);
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(2.05f, 1.16f, -1.56f));
			model = glm::scale(model, glm::vec3(0.57, 0.57f, -0.57f));
			model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[2]->RenderMesh();
		}

		//TR4
		{
			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 0.0f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(1.6f, 1.11f, -2.4f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TR5
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 0.0f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(0.2f, 0.1f, -1.2f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TR6
		{
			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 0.0f, 0.0f);
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(1.78f, 0.14f, -0.96f));
			model = glm::scale(model, glm::vec3(0.57, 0.57f, 0.57f));
			model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[2]->RenderMesh();
		}

		//TR7
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 0.0f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(1.25f, 0.1f, -1.8f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TR8
		{
			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 0.0f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(0.73f, 0.13f, -0.38f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[2]->RenderMesh();
		}

		//TR9
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 0.0f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(2.3f, 0.1f, -2.4f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}


		//TV1
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(0.0f, 0.5f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(0.7f, 2.15f, -2.56f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TV2
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(0.0f, 0.5f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(0.33f, 1.11f, -3.2f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TV3
		{
			model = glm::mat4(1.0f);
			color = glm::vec3(0.0f, 0.5f, 0.0f);
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(0.3f, 1.18f, -2.58f));
			model = glm::scale(model, glm::vec3(0.65f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[2]->RenderMesh();
		}

		//TV4
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(0.0f, 0.5f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(0.33f, 1.11f, -1.95f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TV5
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(0.0f, 0.5f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(-0.05f, 0.1f, -3.85f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TV6
		{
			model = glm::mat4(1.0f);
			color = glm::vec3(0.0f, 0.5f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(-0.05f, 0.13f, -3.2f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[2]->RenderMesh();
		}

		//TV7
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(0.0f, 0.5f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(-0.05f, 0.1f, -2.56f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TV8
		{
			model = glm::mat4(1.0f);
			color = glm::vec3(0.0f, 0.5f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(-0.05f, 0.13f, -1.93f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[2]->RenderMesh();
		}

		//TV9
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(0.0f, 0.5f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(-0.05f, 0.1f, -1.3f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}


		//TAZ1
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(0.0f, 0.0f, 1.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(0.1f, -0.05f, -3.85f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TAZ2
		{
			model = glm::mat4(1.0f);
			color = glm::vec3(0.0f, 0.0f, 1.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(0.1f, -0.05f, -2.56f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TAZ3
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(0.0f, 0.0f, 1.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(1.12f, -0.05f, -2.065f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TAZ4
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(0.0f, 0.0f, 1.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(1.2f, -0.05f, -3.2f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TAZ5
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(0.0f, 0.0f, 1.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(0.1f, -0.05f, -1.3f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TAZ6
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(0.0f, 0.0f, 1.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(1.12f, -0.05f, -0.795f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TAZ7
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(0.0f, 0.0f, 1.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(1.18f, -0.05f, -1.92f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TAZ8
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(0.0f, 0.0f, 1.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(2.2f, -0.05f, -1.41f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TAZ9
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(0.0f, 0.0f, 1.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(2.28f, -0.05f, -2.57f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}


		//TAM1
		{
			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 1.0f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(0.9f, 2.15f, -2.68f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TAM2
		{
			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 1.0f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(0.56f, 1.11f, -3.3f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TAM3
		{
			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 1.0f, 0.0f);
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(1.08f, 1.18f, -3.0f));
			model = glm::scale(model, glm::vec3(0.57, 0.57f, -0.57f));
			model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[2]->RenderMesh();
		}

		//TAM4
		{
			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 1.0f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(1.6f, 1.11f, -2.71f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TAM5
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 1.0f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(0.2f, 0.1f, -3.93f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TAM6
		{
			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 1.0f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(1.7f, 0.13f, -3.05f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[2]->RenderMesh();
		}

		//TAM7
		{
			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 1.0f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(1.25f, 0.1f, -3.32f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}

		//TAM8
		{
			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 1.0f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(2.77f, 0.13f, -2.44f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[2]->RenderMesh();
		}

		//TAM9
		{

			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 1.0f, 0.0f);
			//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::translate(model, glm::vec3(2.33f, 0.1f, -2.71f));
			model = glm::scale(model, glm::vec3(0.57f, 0.57f, 0.57f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
			meshList[1]->RenderMesh();
		}


		////Y
		//model = glm::mat4(1.0f);
		//color = glm::vec3(0.0f, 1.5f, 0.0f);
		////Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
		////model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//model = glm::scale(model, glm::vec3(10.0f, 0.01f, 0.01f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh();

		////Z
		//model = glm::mat4(1.0f);
		//color = glm::vec3(0.0f, 0.0f, 1.0f);
		////Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
		////model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//model = glm::scale(model, glm::vec3(0.01f, 0.01f, 10.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}


