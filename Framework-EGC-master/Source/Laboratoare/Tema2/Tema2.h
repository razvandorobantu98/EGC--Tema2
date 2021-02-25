#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>

#include "CameraTema2.h"

class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;

		Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1));

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		void DrawPlane(float deltaTimeSeconds);
		void ManageObstacles(float deltaTimeSeconds);
		void ManageFuels(float deltaTimeSeconds);
		void ManageClouds(float deltaTimeSeconds);
		void DrawLives();
		void DrawFuel();
		void DrawCylinder(float deltaTimeSeconds);
		void StartGame();
		void EndGame();

		float angularStepRotor;
		float angularStepPlane;
		float transY;	//retine cu cat se deplaseaza avionul pe OY
		float speed;	//retine viteza avionului (X > 0 => avionul urca, X < 0 => avionul coboara)
		float rotationCylinder;

		float rotationObstacle;
		//coordonatele initiale ale obstacolelor
		float initObstaclesX[4] = { 0, -2, 0, 2 };
		float initObstaclesY[4] = { -2.5, 0, 1.5, 0 };
		//coordonatele obstacolelor
		float obstaclesX[4] = { 0, 0, 0, 0 };
		float obstaclesY[4] = { 0, 0, 0, 0 };
		//vector care retine daca un obiect e vizibil sau nu
		bool showObstacles[4] = { false, false, false, false };

		float rotationClouds;
		//coordonatele initiale ale norilor
		float initCloudsX[4] = { 0, -2, 0, 2 };
		float initCloudsY[4] = { -2.5, 0, 1.5, 0 };
		//coordonatele norilor
		float cloudsX[4] = { 0, 0, 0, 0 };
		float cloudsY[4] = { 0, 0, 0, 0 };

		float rotationFuel;
		//coordonatele initiale ale bucatilor de combustibil
		float initFuelX[4] = { 0, 0, 0, 0 };
		float initFuelY[4] = { -2.5, -2.5, -2.5, -2.5 };
		//coordonatele bucatilor de combustibil
		float fuelX[4] = { 0, 0, 0, 0 };
		float fuelY[4] = { 0, 0, 0, 0 };
		//vector care retine daca o bucata e vizibila sau nu
		bool showFuel[4] = { false, false, false, false };

		bool isCylinder = false;

		int lives;
		float planeFuel;
		bool gameRunning;
		bool firstStart;

	protected:
		Tema2Camera::Camera* camera;
		glm::mat4 projectionMatrix;
		bool renderCameraTarget;
};
