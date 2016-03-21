// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Inne biblioteki
#include <SOIL.h>

// Wymiar ekranu
GLuint screenWidth = 1280, screenHeight = 720;

// Prototypy funkcji
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void do_movement();

// Kamera
glm::vec3 cameraPos = glm::vec3(0.8f, 1.5f, 4.0f);
glm::vec3 cameraFront = glm::vec3(-0.2f, -0.3f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -100.0f;
GLfloat pitch = -15.0f;
GLfloat lastX = screenWidth / 2.0;
GLfloat lastY = screenHeight / 2.0;
bool keys[1024];

// Swiatlo
glm::vec3 lightPos = glm::vec3(0.9f, 0.3f, 0.0f); // Pozycja lampy
GLfloat light = 1.0f; // natezenie swiatla

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	// Inicjalizacja GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Blokada okna

	// Tworzenie okna
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Scenography", nullptr, nullptr); // Parametry okna
	glfwMakeContextCurrent(window); // Generowanie okna

	// Ustawienie funkcji callback
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// Opcje
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Wylaczenie kursora myszy

	// Inicjalizacja GLEW do ustawienia funkcji OpenGl'a
	glewExperimental = GL_TRUE;
	glewInit();

	// Rozmiar viewportu dla OpenGL
	glViewport(0, 0, screenWidth, screenHeight);

	// Ustawenia OpenGL
	glEnable(GL_DEPTH_TEST);

	// Kompilacja shaderow
	Shader shader("./res/model_shader.vs", "./res/model_shader.frag");
	Shader lampShader("./res/lamp.vs", "./res/lamp.frag");

	// Ladowanie modeli
	Model islandModel("./res/island/island.obj");
	Model lightBulb("./res/lamp/lamp.obj");
	Model tentModel("./res/tent/tent.obj");
	Model fireModel("./res/fire/fire.obj");
	Model treeModel("./res/tree/tree.obj");
	Model liscModel("./res/lisc/lisc.obj");
	Model bedModel("./res/bed/bed.obj");
	Model palkaModel("./res/palka/palka.obj");
	Model stoneModel("./res/stone/stone.obj");
	Model cloudModel("./res/clouds/clouds.obj");
	Model flowerModel("./res/flower/flower.obj");
	Model bushModel("./res/bush/bush.obj");
	Model sunModel("./res/sun/sun.obj");


	// Rysowanie siatki modeli
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Pozycja swiatla
	glm::vec3 LightPositions[] = {
		glm::vec3() = lightPos,
		glm::vec3(-2.5f, 3.5f, 2.0f)
	};

	// Petla generowania obrazu
	while (!glfwWindowShouldClose(window))
	{
		// Czas generowania obrazu
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Sprawdzanie akcji klawiatury i myszki
		glfwPollEvents();
		do_movement();

		// Czyszczenie bufora koloru oraz glebi
		glClearColor(0.05f, 0.05 + light/2, 0.05 + light, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();   // Aktywacja shadera
		// Transformacje
		glm::mat4  projection = glm::perspective(45.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); // Kierunek kamery
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Ustawienia oswietlenia
		glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), cameraPos.x, cameraPos.y, cameraPos.z);
		// Lampa
		glUniform3f(glGetUniformLocation(shader.Program, "Lights[0].position"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(shader.Program, "Lights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(shader.Program, "Lights[0].diffuse"), 1.0f - light / 1.3, 1.0f - light / 1.3, 1.0f - light / 1.3);
		glUniform3f(glGetUniformLocation(shader.Program, "Lights[0].specular"), 1.0f - light / 1.3, 1.0f - light / 1.3, 1.0f - light / 1.3);
		// Slonce
		glUniform3f(glGetUniformLocation(shader.Program, "Lights[1].position"), LightPositions[1].x, LightPositions[1].y, LightPositions[1].z);
		glUniform3f(glGetUniformLocation(shader.Program, "Lights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(shader.Program, "Lights[1].diffuse"), light, light, light);
		glUniform3f(glGetUniformLocation(shader.Program, "Lights[1].specular"), light, light, light);

		// Rysowanie modeli
		glm::mat4 model;
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		islandModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-0.9f, 0.0f, 0.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.01f, 0.6f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		tentModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.1f, -0.02f, 0.4f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		fireModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-0.3f, -0.02f, -0.6f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.00f, -1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		treeModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(1.5f, -0.02f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, glm::radians(23.0f), glm::vec3(0.00f, -1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		treeModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.4f, -0.02f, -1.2f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.00f, -1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		treeModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.6f, -0.02f, -0.3f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		treeModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(1.3f, -0.02f, -0.6f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		bushModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(1.0f, -0.02f, -0.9f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		bushModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.16f, 0.3f, -0.45f));
		model = glm::scale(model, glm::vec3(0.22f, 0.22f, 0.22f));
		model = glm::rotate(model, glm::radians(110.0f), glm::vec3(0.00f, -1.0f, 0.0f));
		model = glm::rotate(model, (GLfloat)cos(glfwGetTime() * 0.4f)/3, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		bedModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-0.1f, -0.032f, 1.1f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		liscModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, -0.032f, 1.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.00f, -1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		liscModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.2f, -0.05f, 1.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		palkaModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-0.8f, -0.07f, 1.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		palkaModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-0.8f, -0.04f, 1.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		palkaModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, -0.09f, 0.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		palkaModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, 0.05f, -0.9f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		flowerModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.3f, 0.00f, 1.9f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		flowerModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.3f, 0.00f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		flowerModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.9f, 0.00f, -0.3f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		flowerModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(1.5f, 0.00f, 0.6f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		flowerModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.8f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		stoneModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.5f, -0.05f, 1.1f));
		model = glm::scale(model, glm::vec3(0.12f, 0.13f, 0.1f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		stoneModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.4f, -0.05f, 1.4f));
		model = glm::scale(model, glm::vec3(0.13f, 0.11f, 0.16f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		stoneModel.Draw(shader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-0.2f, 1.3f, 0.2f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		cloudModel.Draw(shader);

		// Rysowanie lamp
		lampShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

			model = glm::mat4();
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
			model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.00f, -1.0f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			lightBulb.Draw(lampShader);

			//model = glm::mat4();
			//model = glm::translate(model, LightPositions[1]);
			//model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
			//glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			//sunModel.Draw(lampShader);


		// Zamiana bufora
		glfwSwapBuffers(window);
	}

	// Zamykanie okna
	glfwTerminate();
	return 0;
}

// Wywolana kiedy klawisz jest wcisniety poprzez GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement()
{
	// Kontrola kamery i oswietlenia
	GLfloat cameraSpeed = 3.0f * deltaTime;
	GLfloat lightSpeed = 1.0f * deltaTime;
	if (keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_UP])
		lightPos.z -= lightSpeed;
	if (keys[GLFW_KEY_DOWN])
		lightPos.z += lightSpeed;
	if (keys[GLFW_KEY_LEFT])
		lightPos.x -= lightSpeed;
	if (keys[GLFW_KEY_RIGHT])
		lightPos.x += lightSpeed;
	if (keys[GLFW_KEY_MINUS]){
		light -= 0.5f * deltaTime;
	}
	if (keys[GLFW_KEY_EQUAL]){
		light += 0.5f * deltaTime;
	}
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Odwrocone poniewaz y idzie od dolu do gory
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.2;	// Czulosc zalezna od myszki
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Zapobieganie odwroceniu ekranu
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}