#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "FreeImage.h"
#include <vector>
#include "../header/SceneObj.h"
#include "../header/fpsController.h"
#include "../header/material.h"
#include "../header/texture.h"
#include <iostream>

// rozmiar okna i poożenie myszy
glm::vec2 viewportDimensions = glm::vec2(800, 600);
glm::vec2 mousePosition = glm::vec2();

// zmiana rozmiaru okna
void resizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
    viewportDimensions = glm::vec2(width, height);
}

// zapis pozycji kursora.
void mouseMoveCallback(GLFWwindow *window, GLdouble mouseX, GLdouble mouseY)
{
    mousePosition = glm::vec2(mouseX, mouseY);
}


int main(int argc, char **argv)
{
	// Inicjalizacja GLFW
	glfwInit();

	// Inicjalizacja okna
	GLFWwindow* window = glfwCreateWindow(viewportDimensions.x, viewportDimensions.y, "Wirtualna szkola", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// odwołania do funkcji podczas zdarzeń
	glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);

	// Inicjalizacja glew
	glewInit();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Ładowanie modeli;
    SceneObj trawa;
    trawa.load("./Assets/kitten.obj");

    SceneObj budynek;
    budynek.load("./Assets/kitten.obj");

    SceneObj nauczyciel;
    nauczyciel.load("./Assets/kitten.obj");

    SceneObj uczen;
    uczen.load("./Assets/kitten.obj");

    // Położenie obiektów na scenie
    trawa.ustaw(0, 0, 0);
    budynek.ustaw(0, 1, 0);
    nauczyciel.ustaw(0, 0, -2);
    uczen.ustaw(0, -1, -2);

    // Stworzneie "postaci" do poruszania kamerą
    FPSController controller = FPSController();


    //Shader * fragmentShader = new Shader("/home/szymon/Pulpit/testpath/Assets/fragment.gsls", GL_FRAGMENT_SHADER);
    Shader * fragmentShader = new Shader("./Assets/f_lamberttextured.glsl", GL_FRAGMENT_SHADER);

    //Shader* vertexShader = new Shader("./Assets/DVertex", GL_VERTEX_SHADER);
    Shader* vertexShader = new Shader("./Assets/v_lamberttextured.glsl", GL_VERTEX_SHADER);



	// pola używane w shaderach
	char cameraViewVS[] = "cameraView";
	char worldMatrixVS[] = "worldMatrix";

    // Create A Shader Program
    // The class wraps all of the functionality of a gl shader program.
    ShaderProgram* shaderProgram = new ShaderProgram();
    shaderProgram->AttachShader(vertexShader);
    shaderProgram->AttachShader(fragmentShader);


    // pliki grafczne (tekstury)
    char textureFile1[] = "./Assets/BrickColor.png";
    // nazwa pola użyta w shderze
    char textureFS[] = "tex";

    // Materiał na nasz model
    Material* material = new Material(shaderProgram);
    material->SetTexture(textureFS, new Texture(textureFile1));

	// program działa dopoki okno jest otwarte
	while (!glfwWindowShouldClose(window))
	{
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // Exit when escape is pressed.
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;

        // roznica w czasie dla porusznia się.
        float dt = glfwGetTime();
        glfwSetTime(0);

        // odswizenie polozenia i kierunku patrzenia
        controller.Update(window, viewportDimensions, mousePosition, dt);

        // macierz widoku.
        glm::mat4 view = controller.GetTransform().GetInverseMatrix();
        // macierz projekcji.
        glm::mat4 projection = glm::perspective(.75f, viewportDimensions.x / viewportDimensions.y, .1f, 100.f);
        // kombinacja.
        glm::mat4 viewProjection = projection * view;



        // Set the camera and world matrices to the shader
        // The string names correspond directly to the uniform names within the shader.
        material->SetMatrix(cameraViewVS, viewProjection);
        material->SetMatrix(worldMatrixVS, trawa.zwroc().GetMatrix());
        // Bind the material
        material->Bind();

        //Rysowanie modeli
        trawa.draw();

		// Stop using the shader program.

        material->Unbind();

		// Swap the backbuffer to the front.
		glfwSwapBuffers(window);

		// Poll input and window events.
		glfwPollEvents();
	}

    // Free material should free all objects used by material
    delete material;

	// Free GLFW memory.
	glfwTerminate();

	// End of Program.
	return 0;
}
