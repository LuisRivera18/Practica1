/*
Semestre 2024-1
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transforomación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Adicional.- ,Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movAvion;
float movAvionX;
float movLetrero;
float movOffset;
float movOffset2;
float rothelices;
float rotletrero;
float rothelicesOffset;
float rotletreroOffset;
float rotAvion;
float rotDadoX;
float rotDadoY;
float rotDadoZ;
float movDado;
bool avanza;
bool letrr;
bool let;
bool rot2;
bool rot3;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

Model MentitaAT_M;
Model Arc_M;
Model Letrero_M;
Model Mapa_M;
Model Dado_M;
Model Avion_M;
Model Helices_M;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


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

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

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

	camera = Camera(glm::vec3(0.0f, 40.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.9f, 0.9f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/mapa.png");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	Dado_M = Model();
	Dado_M.LoadModel("Models/dadobest.obj");
	Avion_M = Model();
	Avion_M.LoadModel("Models/avion.obj");
	Helices_M = Model();
	Helices_M.LoadModel("Models/helices.obj");

	Arc_M = Model();
	Arc_M.LoadModel("Models/arc.obj");
	Arc_M = Model();
	Arc_M.LoadModel("Models/arc.obj");
	Letrero_M = Model();
	Letrero_M.LoadModel("Models/letrero.obj");
	Mapa_M.LoadModel("Models/mapa.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.8f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	movAvion = 3.0f;
	movAvionX = 3.0f;
	movDado = 1.0f;
	rotDadoX = 1.0f;
	rotDadoY = 1.0f;
	rotDadoZ = 1.0f;
	movOffset = 3.0f;
	movOffset2 = 0.2f;
	rothelices = 2.0f;
	rothelicesOffset = 9.0f;
	rotletreroOffset = 4.0f;
	movLetrero = 0.0f;
	rotletrero = 1.0f;
	avanza = true;
	letrr = true;
	rot2 = false;
	rot3 = false;
	let = true;
	int cont=2.0;
	static int numeroAleatorio=0;
	float prevTime = glfwGetTime();
	glfwSetTime(0);
	////Loop mientras no se cierra la ventana
	




	while (!mainWindow.getShouldClose())
	{
		if (mainWindow.getmuevex()) {
		srand(time(NULL));
		numeroAleatorio = (rand() % 10) + 1;
		//int numeroAleatorio = 1;	
		printf("Número aleatorio entre 1 y 10: %d\n", numeroAleatorio);
		rotDadoX = rotDadoY = rotDadoZ = movDado = 0.0f;
		}
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		
		if (glfwGetTime() > 5) {
			if (avanza) {
				if (movAvion > -240) {
					movAvion -= movOffset * deltaTime;
					rothelices += rothelicesOffset * deltaTime;
					
					if (movAvion < -239.0f)
					{
						rotAvion = -90;
						if (movAvionX > -239.0f) {
							movAvionX -= movOffset * deltaTime;
						}
						else {
							avanza = !avanza;
						}
						/*movAvion = 0;*/
					}
					else {
						avanza = !avanza;
					}
				}
			}
			//else if(movAvionX > 240) {
			//	movAvionX += movOffset * deltaTime;
			//	//printf("avanza%f \n ",movAvion);
			//	rothelices += rothelicesOffset * deltaTime;

			//	if (movAvionX < -239.0f)
			//	{
			//		rotAvion = -90;
			//		//movAvion = 0;

			//	}
			//	else {
			//		avanza = !avanza;
			//	}
			//}
			else
			{
				//if (movAvion < 260.0f)
				//{
				//	movAvion += movOffset * deltaTime;
				//	//printf("avanza%f \n ",movAvion);
				//	rothelices -= rothelicesOffset * deltaTime;
				//}
				//else {
				//	avanza = !avanza;
				//}
				avanza = !avanza;
			}
		}
		/*
		if (glfwGetTime() > 5) {
			if (letrr) {
				if (movLetrero > -90.0f)
				{
					rotletrero += 4.0 * deltaTime;
					movLetrero -= 1.0 * deltaTime;
				}
				else {
					letrr = !letrr;
				}
			}
			else
			{
				if (movLetrero < -0.1f)
				{
					rotletrero += 4.0 * deltaTime;
					movLetrero += 1.0 * deltaTime;
				}
				else {
					letrr = !letrr;
				}
			}*/

			/*if (glfwGetTime() > 5) {
				if (letrr) {
					movLetrero = movOffset2 * deltaTime;

					if (movLetrero < 0.0f && movLetrero > -1.0)
					{
						rotletrero += rotletreroOffset * deltaTime;
						if (glfwGetTime() > 2) {
							movLetrero = movOffset2 * deltaTime;
						}
					}
					else if (movLetrero < -1.1f && movLetrero > -89.0) {
						movLetrero=movOffset2*deltaTime;
					}
					else if (movLetrero <-89.1 && movLetrero > -90.0){
						rotletrero += rotletreroOffset * deltaTime;
						if (glfwGetTime() > 2) {
							movLetrero = movOffset2 * deltaTime;
						}
					}
					else {
						letrr = !letrr;
					}
				}
				else{
						letrr = !letrr;
				}

			}*/

			/*
			if (glfwGetTime() > 5) {
				//traslacion
				if (letrr) {
					if (glfwGetTime() == 5 + cont) {
						movLetrero -= movOffset2 * deltaTime;
						letrr = !letrr;
						cont += 2;
					}
				}

				else {
					if (movLetrero < 0.0f) {
						movLetrero += movOffset2 * deltaTime;
					}
					else letrr = !letrr;
				}


			}

			printf("%.3f, %d \n", glfwGetTime(), cont);
			if (glfwGetTime() > 5) {
				//movLetrero = movOffset2 * deltaTime;
				if (letrr) {
					//movLetrero = movOffset2 * deltaTime;
					if (movLetrero < 1.0f && movLetrero > -2.0) {
						movLetrero += movOffset2 * deltaTime;   // Rotar
						if (glfwGetTime() > 2 * cont) {
								rotletrero += rotletreroOffset * deltaTime;
								movLetrero -= movOffset2 * deltaTime;// Continuar movimiento
								cont++;
						}
					}
					else if (movLetrero <= -2.1f && movLetrero > -89.0) {
						if (glfwGetTime() > 2*cont) {
							movLetrero -= movOffset2 * deltaTime;// Continuar movimiento
							cont++;
						}
					}
					else if (movLetrero <= -89.1 && movLetrero > -91.0) {
						 movLetrero = movOffset2 * deltaTime;  // Rotar
						if (glfwGetTime() > 2*cont) {
							rotletrero += rotletreroOffset * deltaTime;
							movLetrero += movOffset2 * deltaTime;// Continuar movimiento
							cont++;
						}
					}
					else {
						letrr = !letrr;  // Invertir la variable
						  // Reiniciar el tiempo previo
					}
				}
				else {
					letrr = !letrr;  // Invertir la variable para repetir el proceso
					 // Reiniciar el tiempo previo
				}
			}
			*/
		
		switch (numeroAleatorio) {
		/*case 1:
			if (glfwGetTime() > 5) {
				if (letrr) {
					if (rotDadoY < 501) {
						rotDadoY += movOffset * deltaTime;
						if (glfwGetTime() > 9) {
							if (rotDadoX < 542) {
								rotDadoX += movOffset * deltaTime;
								if (glfwGetTime() > 11) {
									if (rotDadoZ < 15) {
										rotDadoZ += movOffset * deltaTime;
										if (movDado > -200.0) {
											movDado -= movOffset * deltaTime;
										}
									}
								}
							}
						}
					}
				}
			}*/
		case 1:
			if (glfwGetTime() > 5) {
				rotDadoX = 47.0;
				rotDadoZ = -20.0;
				if (movDado > -24) {
					movDado -= movOffset2 * deltaTime;
				}
			}		
			break;
		case 2:
			if (glfwGetTime() > 5) {
				rotDadoX = 137.0;
				rotDadoY = 17.0;
				rotDadoZ = 0.0;
				if (movDado > -24) {
					movDado -= movOffset2 * deltaTime;
				}
			}
			break;
		case 3:
			if (glfwGetTime() > 5) {
				rotDadoZ = -46.0;
				if (glfwGetTime() > 5) {
					if (movDado > -24) {
						movDado -= movOffset2 * deltaTime;
					}
				}
			}
			break;
		case 4:
			if (glfwGetTime() > 5) {
				rotDadoX = -135.0;
				rotDadoY = 55.0;
				rotDadoZ = 0.0;
				if (movDado > -24) {
					movDado -= movOffset2 * deltaTime;
				}
			}
			break;
		case 5:
			if (glfwGetTime() > 5) {
				rotDadoZ = 42.0;
				rotDadoX = -28.0;
				if (movDado > -24) {
					movDado -= movOffset2 * deltaTime;
				}
			}
			break;
		case 6:
			if (glfwGetTime() > 5) {
				rotDadoX = -135.0;
				rotDadoY = 125.0;
				rotDadoZ = 0.0;
				if (movDado > -24) {
					movDado -= movOffset2 * deltaTime;
				}
			}
			break;
		case 7:
			if (glfwGetTime() > 5) {
				rotDadoX = 28.0;
				rotDadoZ = 43.0;
				if (movDado > -24) {
					movDado -= movOffset2 * deltaTime;
				}
			}
			break;
		case 8:
			if (glfwGetTime() > 5) {
				rotDadoZ = 135.0;
				if (movDado > -24) {
					movDado -= movOffset2 * deltaTime;
				}
			}
			break;
		case 9:
			if (glfwGetTime() > 5) {
				rotDadoX = -42.0;
				rotDadoZ = -14.0;
				if (movDado > -24) {
					movDado -= movOffset2 * deltaTime;
				}
			}
			break;
		case 10:
			if (glfwGetTime() > 5) {
				rotDadoX = -135.0;
				rotDadoY = -18.0;
				rotDadoZ = 0.0;
				if (movDado > -24) {
					movDado -= movOffset2 * deltaTime;
				}
			}
			break;
		}
		

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
			glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		/*model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();*/
		
		//piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mapa_M.RenderModel();

		//Instancia del avion 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3( -130.0f , 5.0f, movAvionX +  movAvion + 120.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, rotAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Avion_M.RenderModel();	

		//helices
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.15f, 1.55f, -5.83f));
		model = glm::rotate(model, rothelices * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helices_M.RenderModel();

		//dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, movDado + 30.0f, 0.0f));
		model = glm::rotate(model, rotDadoX * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotDadoY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotDadoZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rothelices * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rothelices * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();
	

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-350.0f, 2.0f, 320.0));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//MentitaAT_M.RenderModel();

		//arco
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-387.0f, 0.2f, 330.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Arc_M.RenderModel();

		//Letrero
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, movLetrero + 110.0, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotletrero * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 50.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Letrero_M.RenderModel();

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
