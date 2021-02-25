#include "Tema2.h"
#include "Transformari3D.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	camera = new Tema2Camera::Camera();
	camera->Set(glm::vec3(0, 1.2f, 2.5f), glm::vec3(0, 1.5f, 0), glm::vec3(0, 1, 0));
	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	// Creaza un cub cu centrul in (0,0)
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(1)),
			VertexFormat(glm::vec3(1, -1,  1), glm::vec3(1)),
			VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1)),
			VertexFormat(glm::vec3(1,  1,  1), glm::vec3(1)),
			VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1)),
			VertexFormat(glm::vec3(1, -1, -1), glm::vec3(1)),
			VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1)),
			VertexFormat(glm::vec3(1,  1, -1), glm::vec3(1)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
			2, 3, 7,		2, 7, 6,
			1, 7, 3,		1, 5, 7,
			6, 7, 4,		7, 5, 4,
			0, 4, 1,		1, 4, 5,
			2, 6, 4,		0, 2, 4,
		};

		CreateMesh("cube", vertices, indices);
	}

	// Creaza un cub cu centrul in (1,1)
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(0, 0, 2), glm::vec3(1)),
			VertexFormat(glm::vec3(2, 0, 2), glm::vec3(1)),
			VertexFormat(glm::vec3(0, 2, 2), glm::vec3(1)),
			VertexFormat(glm::vec3(2, 2, 2), glm::vec3(1)),
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1)),
			VertexFormat(glm::vec3(2, 0, 0), glm::vec3(1)),
			VertexFormat(glm::vec3(0, 2, 0), glm::vec3(1)),
			VertexFormat(glm::vec3(2, 2, 0), glm::vec3(1)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
			2, 3, 7,		2, 7, 6,
			1, 7, 3,		1, 5, 7,
			6, 7, 4,		7, 5, 4,
			0, 4, 1,		1, 4, 5,
			2, 6, 4,		0, 2, 4,
		};

		CreateMesh("cube2", vertices, indices);
	}

	// Creaza trunchi de piramida
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, 0,  1), glm::vec3(1)),
			VertexFormat(glm::vec3(1, -1,  2), glm::vec3(1)),
			VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1)),
			VertexFormat(glm::vec3(1,  1,  2), glm::vec3(1)),
			VertexFormat(glm::vec3(-1, 0, -1), glm::vec3(1)),
			VertexFormat(glm::vec3(1, -1, -2), glm::vec3(1)),
			VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1)),
			VertexFormat(glm::vec3(1,  1, -2), glm::vec3(1)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
			2, 3, 7,		2, 7, 6,
			1, 7, 3,		1, 5, 7,
			6, 7, 4,		7, 5, 4,
			0, 4, 1,		1, 4, 5,
			2, 6, 4,		0, 2, 4,
		};

		CreateMesh("trunk", vertices, indices);
	}

	// Creaza un tetraedru
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1)),
			VertexFormat(glm::vec3(1, 0, 0), glm::vec3(1)),
			VertexFormat(glm::vec3(0, 1, 0), glm::vec3(1)),
			VertexFormat(glm::vec3(0, 0, 1), glm::vec3(1))
		};

		vector<unsigned short> indices =
		{
			1, 3, 2,
			1, 4, 3,
			1, 2, 4,
			2, 3, 4
		};

		CreateMesh("tetrahedron", vertices, indices);
	}

	{
		Mesh* mesh = new Mesh("cylinder");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "cylinder2.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("heart");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "heart.fbx");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShaderTema2");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	//valori initiale
	{
		rotationCylinder = 0;
		rotationClouds = 0;
		isCylinder = false;
	
		for (int i = 1; i < 4; i++) {
			initFuelX[i] = initFuelX[0] * cos(RADIANS(4 * i)) - initFuelY[0] * sin(RADIANS(4 * i));
			initFuelY[i] = initFuelX[0] * sin(RADIANS(4 * i)) + initFuelY[0] * cos(RADIANS(4 * i));
		}
		
		gameRunning = false;
		firstStart = true;
		lives = 3;
		planeFuel = 100;

		printf("Press SPACE to start the game!\n");
	}
}

Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
	return meshes[name];
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.55f, 0.8f, 1.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);	
}

void Tema2::StartGame()
{
	//seteaza valorile initiale si porneste jocul
	gameRunning = true;
	printf("Game has started!\n\n");

	angularStepRotor = 0;
	angularStepPlane = 0;
	transY = 0;
	speed = 0;

	rotationObstacle = 0;
	rotationFuel = 0;

	lives = 3;
	planeFuel = 100;

	for (int i = 0; i < 4; i++) {
		showObstacles[i] = false;
		showFuel[i] = false;
	}
}

void Tema2::EndGame()
{
	//opreste jocul
	gameRunning = false;
	printf("Game Over!\n");
	printf("Press Space to try again.\n\n");
}

void Tema2::DrawPlane(float deltaTimeSeconds)
{
	glm::mat4 rotationMatrix = glm::mat4(1);

	//daca jocul ruleaza
	if (gameRunning) {

		//calculeaza cu cat se deplaseaza avonul pe axa OY
		transY += speed * deltaTimeSeconds;

		//nu lasa avionul sa se duca prea sus sau prea jos (limite imaginare)
		if (transY < -0.8f) {
			transY = -0.8f;
		}
		else if (transY > 0.8f) {
			transY = 0.8f;
		}

		//calculeaza unghiul avionului
		angularStepPlane = 0.5 * speed;
		if (angularStepPlane > RADIANS(60)) {
			angularStepPlane = RADIANS(60);
		}
		if (angularStepPlane < RADIANS(-60)) {
			angularStepPlane = RADIANS(-60);
		}
		rotationMatrix = Transformari3D::Translate(0, 2, 0) * Transformari3D::RotateOZ(angularStepPlane) * Transformari3D::Translate(0, -2, 0);
	}
	//daca jocul se termina, avionul face animatia de prabusire
	else if (gameRunning == false && firstStart == false) {
		if (2 + transY > 0) {
			transY -= 1 * deltaTimeSeconds;
		}
		rotationMatrix = Transformari3D::Translate(0, 2, 0) * Transformari3D::RotateOZ(RADIANS(-60)) * Transformari3D::Translate(0, -2, 0);
	}

	//rotor elice
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(0, transY, 0);
		modelMatrix *= rotationMatrix;
		modelMatrix *= Transformari3D::Translate(0.16f, 2.03f, 0);
		modelMatrix *= Transformari3D::RotateOZ(RADIANS(5.0f));
		modelMatrix *= Transformari3D::Scale(0.015f, 0.015f, 0.015f);
		RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0, 0, 0));
	}

	angularStepRotor += 1000 * deltaTimeSeconds;
	if (angularStepRotor > 360) {
		angularStepRotor = (int)angularStepRotor % 360;
	}

	//elice
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(0, transY, 0);
		modelMatrix *= rotationMatrix;
		modelMatrix *= Transformari3D::Translate(0.17f, 2.03f, 0);
		modelMatrix *= Transformari3D::RotateOZ(RADIANS(5.0f));
		modelMatrix *= Transformari3D::RotateOX(RADIANS(angularStepRotor));
		modelMatrix *= Transformari3D::Scale(0.01f, 0.021f, 0.14f);
		RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.2, 0.2, 0.2));
	}

	//parte alba fata avion
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(0, transY, 0);
		modelMatrix *= rotationMatrix;
		modelMatrix *= Transformari3D::Translate(0.12f, 2.03f, 0);
		modelMatrix *= Transformari3D::RotateOZ(RADIANS(5.0f));
		modelMatrix *= Transformari3D::Scale(0.025f, 0.072f, 0.072f);
		RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 1, 1));
	}

	//corp avion
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(0, transY, 0);
		modelMatrix *= rotationMatrix;
		modelMatrix *= Transformari3D::Translate(-0.1f, 2.01f, 0);
		modelMatrix *= Transformari3D::RotateOZ(RADIANS(5.0f));
		modelMatrix *= Transformari3D::Scale(0.2f, 0.07f, 0.035f);
		RenderSimpleMesh(meshes["trunk"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0));
	}

	//aripa dreapta
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(0, transY, 0);
		modelMatrix *= rotationMatrix;
		modelMatrix *= Transformari3D::Translate(0, 2.02f, 0.14f);
		modelMatrix *= Transformari3D::RotateOZ(RADIANS(5.0f));
		modelMatrix *= Transformari3D::Scale(0.05f, 0.02f, 0.09f);
		RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.8, 0, 0));
	}

	//aripa stanga
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(0, transY, 0);
		modelMatrix *= rotationMatrix;
		modelMatrix *= Transformari3D::Translate(0, 2.02f, -0.14f);
		modelMatrix *= Transformari3D::RotateOZ(RADIANS(5.0f));
		modelMatrix *= Transformari3D::Scale(0.05f, 0.02f, 0.09f);
		RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.8, 0, 0));
	}

	//capat coada
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(0, transY, 0);
		modelMatrix *= rotationMatrix;
		modelMatrix *= Transformari3D::Translate(-0.27f, 2.08f, 0);
		modelMatrix *= Transformari3D::RotateOZ(RADIANS(5.0f));
		modelMatrix *= Transformari3D::Scale(0.03f, 0.02f, 0.02f);
		RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.2, 0.2, 0.2));
	}

	//capat coada dreapta
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(0, transY, 0);
		modelMatrix *= rotationMatrix;
		modelMatrix *= Transformari3D::Translate(-0.25f, 2.04f, 0.05f);
		modelMatrix *= Transformari3D::RotateOZ(RADIANS(5.0f));
		modelMatrix *= Transformari3D::Scale(0.025f, 0.018f, 0.018f);
		RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.2, 0.2, 0.2));
	}

	//capat coada stanga
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(0, transY, 0);
		modelMatrix *= rotationMatrix;
		modelMatrix *= Transformari3D::Translate(-0.25f, 2.04f, -0.05f);
		modelMatrix *= Transformari3D::RotateOZ(RADIANS(5.0f));
		modelMatrix *= Transformari3D::Scale(0.025f, 0.018f, 0.018f);
		RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.2, 0.2, 0.2));
	}

	//roata dreapta
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(0, transY, 0);
		modelMatrix *= rotationMatrix;
		modelMatrix *= Transformari3D::Translate(0, 1.95f, 0.05);
		modelMatrix *= Transformari3D::RotateOZ(RADIANS(50.0f));
		modelMatrix *= Transformari3D::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.2, 0.2, 0.2));
	}

	//roata stanga
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(0, transY, 0);
		modelMatrix *= Transformari3D::Translate(0, 1.95f, -0.05);
		modelMatrix *= Transformari3D::RotateOZ(RADIANS(50.0f));
		modelMatrix *= Transformari3D::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.2, 0.2, 0.2));
	}

	//roata spate
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(0, transY, 0);
		modelMatrix *= rotationMatrix;
		modelMatrix *= Transformari3D::Translate(-0.25f, 1.98f, 0);
		modelMatrix *= Transformari3D::RotateOZ(RADIANS(-50.0f));
		modelMatrix *= Transformari3D::Scale(0.04f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.2, 0.2, 0.2));
	}

	//om
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(0, transY, 0);
		modelMatrix *= rotationMatrix;
		modelMatrix *= Transformari3D::Translate(0, 2.09f, 0);
		modelMatrix *= Transformari3D::RotateOZ(RADIANS(5.0f));
		modelMatrix *= Transformari3D::Scale(0.02f, 0.02f, 0.03f);
		RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0, 0.75, 0));
	}

	//cap om
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(0, transY, 0);
		modelMatrix *= rotationMatrix;
		modelMatrix *= Transformari3D::Translate(-0.002f, 2.12f, 0);
		modelMatrix *= Transformari3D::RotateOZ(RADIANS(5.0f));
		modelMatrix *= Transformari3D::Scale(0.015f, 0.015f, 0.015f);
		RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0.9, 0.8));
	}
}

void Tema2::ManageObstacles(float deltaTimeSeconds)
{
	rotationObstacle += 50 * deltaTimeSeconds;
	if (rotationObstacle > 360) {
		rotationObstacle = (int)rotationObstacle % 360;
	}

	//calculeaza coordonatele X si Y pentru fiecare obstacol
	for (int i = 0; i < 4; i++) {
		obstaclesX[i] = initObstaclesX[i] * cos(RADIANS(rotationObstacle)) - initObstaclesY[i] * sin(RADIANS(rotationObstacle));
		obstaclesY[i] = initObstaclesX[i] * sin(RADIANS(rotationObstacle)) + initObstaclesY[i] * cos(RADIANS(rotationObstacle));
	}

	if(gameRunning == true) {
		
		for (int i = 0; i < 4; i++) {
			//deseneaza obstacolele
			if (showObstacles[i]) {
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix *= Transformari3D::Translate(obstaclesX[i], obstaclesY[i], 0);
				modelMatrix *= Transformari3D::Scale(0.1f, 0.1f, 0.1f);
				RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0));

				//verifica coliziunea doar cu obstacolele vizibile
				if (obstaclesX[i] < 0.2 && obstaclesX[i] > -0.3) {

					if ((transY + 2.0 < obstaclesY[i] + 0.15) && (transY + 2.0 > obstaclesY[i] - 0.15)) {
						//daca avionul loveste un obstacol, obstacolul devine invizibil
						showObstacles[i] = false;
						lives--;
						
						//jocul se termina daca nu mai am nicio viata ramasa
						if (lives <= 0) {
							EndGame();
						}
					}
				}
			}

			//daca un obstacol invizibil ajunge in punctul initial (sub cilindru), devine din nou vizibil
			if (!showObstacles[i]) {

				if (obstaclesX[i] > 0 && obstaclesY[i] < 0) {
					showObstacles[i] = true;
				}
			}
		}
	}
}

void Tema2::ManageFuels(float deltaTimeSeconds)
{
	rotationFuel += 25 * deltaTimeSeconds;
	if (rotationFuel > 360) {
		rotationFuel = (int)rotationFuel % 360;
	}

	//calculeaza coordonatele X si Y pentru fiecare bucata de combustibil
	for (int i = 0; i < 4; i++) {
		fuelX[i] = initFuelX[i] * cos(RADIANS(rotationFuel)) - initFuelY[i] * sin(RADIANS(rotationFuel));
		fuelY[i] = initFuelX[i] * sin(RADIANS(rotationFuel)) + initFuelY[i] * cos(RADIANS(rotationFuel));
	}

	if(gameRunning == true) {
		//jocul se termina cand combustibilul ajunge la 0
		if (planeFuel <= 0.0f) {
			EndGame();
		}
		planeFuel -= 4 * deltaTimeSeconds;

		for (int i = 0; i < 4; i++) {
			//deseneaza bucatile de combustibil
			if (showFuel[i]) {
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix *= Transformari3D::Translate(fuelX[i], fuelY[i], 0);
				modelMatrix *= Transformari3D::RotateOX(rotationFuel / 10);
				modelMatrix *= Transformari3D::Scale(0.065f, 0.065f, 0.065f);
				RenderSimpleMesh(meshes["tetrahedron"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 1, 0));

				//verifica coliziunea cu bucatile de combustibil vizibile
				if (fuelX[i] < 0.2 && fuelX[i] > -0.3) {
					if ((transY + 2.0 < fuelY[i] + 0.1) && (transY + 2.0 > fuelY[i] - 0.1)) {
						//daca iau o bucata de combustibil, bucata respectiva devine invizibila
						showFuel[i] = false;
						planeFuel += 20;

						if (planeFuel > 100) {
							planeFuel = 100;
						}
					}
				}
			}

			//daca o bucata de combustibil invizibila ajunge sub cilindru, aceasta devine din nou vizibila
			if (!showFuel[i]) {
				if (fuelX[i] > 0 && fuelY[i] < 0) {
					showFuel[i] = true;
				}
			}
		}
	}
}

void Tema2::ManageClouds(float deltaTimeSeconds)
{
	rotationClouds += 10 * deltaTimeSeconds;
	if (rotationClouds > 360) {
		rotationClouds = (int)rotationClouds % 360;
	}

	//calculeaza pozitiile X si Y pentru nori
	for (int i = 0; i < 4; i++) {
		cloudsX[i] = initCloudsX[i] * cos(RADIANS(rotationClouds)) - initCloudsY[i] * sin(RADIANS(rotationClouds));
		cloudsY[i] = initCloudsX[i] * sin(RADIANS(rotationClouds)) + initCloudsY[i] * cos(RADIANS(rotationClouds));
	}

	//deseneaza norii
	for (int i = 0; i < 4; i++) {

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(0, 0, -1);
		modelMatrix *= Transformari3D::Translate(cloudsX[i], cloudsY[i], 0) * Transformari3D::RotateOZ(RADIANS(rotationClouds)) * Transformari3D::Translate(-cloudsX[i], -cloudsY[i], 0);
		modelMatrix *= Transformari3D::Translate(cloudsX[i], cloudsY[i], 0);
		modelMatrix *= Transformari3D::RotateOY(RADIANS(-15));
		modelMatrix *= Transformari3D::RotateOX(RADIANS(15));
		modelMatrix *= Transformari3D::Scale(0.08f, 0.08f, 0.08f);
		RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 1, 1));

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(0.1f, 0, -0.95f);
		modelMatrix *= Transformari3D::RotateOZ(RADIANS(0.5));
		modelMatrix *= Transformari3D::Translate(cloudsX[i], cloudsY[i], 0) * Transformari3D::RotateOZ(RADIANS(rotationClouds)) * Transformari3D::Translate(-cloudsX[i], -cloudsY[i], 0);
		modelMatrix *= Transformari3D::Translate(cloudsX[i], cloudsY[i], 0);
		modelMatrix *= Transformari3D::RotateOY(RADIANS(-15));
		modelMatrix *= Transformari3D::RotateOX(RADIANS(15));
		modelMatrix *= Transformari3D::Scale(0.08f, 0.08f, 0.08f);
		RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 1, 1));

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(-0.13f, -0.032f, -1);
		modelMatrix *= Transformari3D::RotateOZ(RADIANS(-0.5));
		modelMatrix *= Transformari3D::Translate(cloudsX[i], cloudsY[i], 0) * Transformari3D::RotateOZ(RADIANS(rotationClouds)) * Transformari3D::Translate(-cloudsX[i], -cloudsY[i], 0);
		modelMatrix *= Transformari3D::Translate(cloudsX[i], cloudsY[i], 0);
		modelMatrix *= Transformari3D::RotateOY(RADIANS(15));
		modelMatrix *= Transformari3D::RotateOZ(RADIANS(25));
		modelMatrix *= Transformari3D::Scale(0.06f, 0.06f, 0.06f);
		RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 1, 1));

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(-0.08f, 0.07f, -1);
		modelMatrix *= Transformari3D::Translate(cloudsX[i], cloudsY[i], 0) * Transformari3D::RotateOZ(RADIANS(rotationClouds)) * Transformari3D::Translate(-cloudsX[i], -cloudsY[i], 0);
		modelMatrix *= Transformari3D::Translate(cloudsX[i], cloudsY[i], 0);
		modelMatrix *= Transformari3D::RotateOY(RADIANS(-45));
		modelMatrix *= Transformari3D::RotateOX(RADIANS(45));
		modelMatrix *= Transformari3D::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 1, 1));
	}
}

void Tema2::DrawLives()
{
	//afiseaza vietile ramase
	if (lives >= 1) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(-2.4f, 2.8f, 0);
		modelMatrix *= Transformari3D::RotateOX(RADIANS(-90));
		modelMatrix *= Transformari3D::Scale(0.002f, 0.002f, 0.002f);
		RenderSimpleMesh(meshes["heart"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0));
	}
	if (lives >= 2) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(-2.15f, 2.8f, 0);
		modelMatrix *= Transformari3D::RotateOX(RADIANS(-90));
		modelMatrix *= Transformari3D::Scale(0.002f, 0.002f, 0.002f);
		RenderSimpleMesh(meshes["heart"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0));
	}
	if (lives == 3) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(-1.9f, 2.8f, 0);
		modelMatrix *= Transformari3D::RotateOX(RADIANS(-90));
		modelMatrix *= Transformari3D::Scale(0.002f, 0.002f, 0.002f);
		RenderSimpleMesh(meshes["heart"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0));
	}
}

void Tema2::DrawFuel()
{
	//adiseaza combustibilul ramas
	if (planeFuel > 0) {
		float fuelLeft = 0.41f * planeFuel / 100;

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transformari3D::Translate(1.59f, 2.8f, 0);
		modelMatrix *= Transformari3D::RotateOX(RADIANS(90));
		modelMatrix *= Transformari3D::Scale(fuelLeft, 0.031f, 0.061f);
		RenderSimpleMesh(meshes["cube2"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 1, 0));
	}

	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transformari3D::Translate(1.6f, 2.8f, 0);
	modelMatrix *= Transformari3D::RotateOX(RADIANS(90));
	modelMatrix *= Transformari3D::Scale(0.4f, 0.03f, 0.06f);
	RenderSimpleMesh(meshes["cube2"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0, 0, 0));
}

void Tema2::DrawCylinder(float deltaTimeSeconds)
{
	rotationCylinder += 0.5 * deltaTimeSeconds;
	if (rotationCylinder > 360) {
		rotationCylinder = (int)rotationCylinder % 360;
	}

	//deseneaza cilindrul
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = Transformari3D::Translate(0, 0, 0);
		modelMatrix *= Transformari3D::Scale(1, 1, 1);
		modelMatrix *= Transformari3D::RotateOX(RADIANS(90.0f));
		modelMatrix *= Transformari3D::RotateOY(rotationCylinder);
		isCylinder = true;
		RenderSimpleMesh(meshes["cylinder"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0, 0, 1));
		isCylinder = false;
	}
}

void Tema2::Update(float deltaTimeSeconds)
{
	//ShowCursor(FALSE);

	DrawCylinder(deltaTimeSeconds);
	DrawPlane(deltaTimeSeconds);
	ManageObstacles(deltaTimeSeconds);
	ManageFuels(deltaTimeSeconds);
	ManageClouds(deltaTimeSeconds);
	DrawLives();
	DrawFuel();
}

void Tema2::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color)
{
	if (!mesh || !shader || !shader->GetProgramID()) {
		return;
	}

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	int object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(object_color, color.r, color.g, color.b);

	float timeElapsed = Engine::GetElapsedTime();
	int loc = glGetUniformLocation(shader->program, "timeElapsed");
	glUniform1f(loc, timeElapsed);

	loc = glGetUniformLocation(shader->program, "isCylinder");
	glUniform1f(loc, isCylinder);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
}

void Tema2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE) {

		//daca abia am deschis jocul, acesta incepe la apasarea tastei space
		if (firstStart == true) {
			StartGame();
			firstStart = false;
		}
		//daca jocul s-a terminat, incepe din nou la apasarea tastei space
		else if(gameRunning == false) {
			StartGame();
		}
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	//if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
	float sensivityOX = 0.01;
	float sensivityOY = 0.01;
	
	//calculeaza viteza in functie de miscarea mouse-ului
	speed -= deltaY * sensivityOY;
	if (speed > 2) {
		speed = 2;
	}
	if (speed < -2) {
		speed = -2;
	}
	//}
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
