#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void animacion();
void animacionPtero();
void animacionBronto();
void animacionTrex();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 2.0f, 45.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active = true;
bool luces = false;

// Variables de las animaciones
// Animacion Pterodactilo
glm::vec3 PosIniPtero(-35.0f, 11.0f, -40.0f);
float movKitX = 0.0;
float movKitZ = 0.0;
float rotKit = 0.0;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;
bool alas = false;
float rotAlaD = 0.0f;
float rotAlaI = 0.0f;

// Animacion Brontosaurio
float movBronto = 40.0f;
float rotCuello = 0.0f;
bool cuello = false;

// Animacion T-Rex
glm::vec3 PosIniTrex(-43.0f, 0.0f, -40.0f);
glm::vec3 correcionD(-0.5f, 5.2f, 1.1f);
glm::vec3 correcionI(-0.5f, 5.2f, -1.1f);
float movKitX2 = 0.0;
float movKitZ2 = 0.0;
float rotKit2 = 90.0;
bool camino1 = true;
bool camino2 = false;
bool camino3 = false;
bool patas = false;
float rotPataD = 0.0f;
float rotPataI = 0.0f;

// Positions of the point lights
// Indicar en el lighting.frag el num de point ligths y agregar sus posiciones aqui
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,0.0f,0.0f)
};

glm::vec3 Light1 = glm::vec3(0);





// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Fossil Land", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	// Setup and compile our shaders
	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");

	/* Parque */
	Model Piso ((char*)"Models/Piso/Piso.obj");
	
	Model fuente ((char*)"Models/Sodas/Fuente/fuenteC.obj");
	Model fuenteAgua ((char*)"Models/Sodas/Fuente/fuenteA.obj");
	Model mesa ((char*)"Models/Sodas/Mesa/mesa.obj");
	Model maquinas ((char*)"Models/Sodas/Maquina/maquinas.obj");

	Model cartel ((char*)"Models/Piso/cartel.obj");

	/* Dinosaurios */
	Model brontoCa ((char*)"Models/Dinosaurios/Bronto/brontoCabeza.obj");
	Model brontoCu ((char*)"Models/Dinosaurios/Bronto/brontoCuerpo.obj");

	Model pteroCu ((char*)"Models/Dinosaurios/Ptero/pteroCu.obj");
	Model pteroAlaD ((char*)"Models/Dinosaurios/Ptero/pteroAlaD.obj");
	Model pteroAlaI ((char*)"Models/Dinosaurios/Ptero/pteroAlaI.obj");

	Model veloCu ((char*)"Models/Dinosaurios/Velo/veloCu.obj");
	Model veloHomD ((char*)"Models/Dinosaurios/Velo/veloHombroDer.obj");
	Model veloBraD ((char*)"Models/Dinosaurios/Velo/veloBrazoDer.obj");
	Model veloHomI ((char*)"Models/Dinosaurios/Velo/veloHombroIzq.obj");
	Model veloBraI ((char*)"Models/Dinosaurios/Velo/veloBrazoIzq.obj");

	Model tRexCu ((char*)"Models/Dinosaurios/Trex/T_rexCu.obj");
	Model tRexPD ((char*)"Models/Dinosaurios/Trex/T_rexPiernaD.obj");
	Model tRexPI ((char*)"Models/Dinosaurios/Trex/T_rexPiernaI.obj");


	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);



		//Load Model
	    // Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.0f, 1.0f, 1.0f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

		
		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.32f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.44f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);

		//Carga de modelo 
		view = camera.GetViewMatrix();

		/* PARQUE */
		/* Piso */
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Piso.Draw(lightingShader);
					
		/* Fuente */
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);
		fuenteAgua.Draw(lightingShader);
		glDisable(GL_BLEND);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		fuente.Draw(lightingShader);

		/* Mesas */
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-39.0f, 0.0f, 20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		mesa.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-34.0f, 0.0f, 20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		mesa.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-39.0f, 0.0f, 27.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		mesa.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-34.0f, 0.0f, 27.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		mesa.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-39.0f, 0.0f, 35.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		mesa.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-34.0f, 0.0f, 35.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		mesa.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-39.0f, 0.0f, 42.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		mesa.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-34.0f, 0.0f, 42.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		mesa.Draw(lightingShader);

		/* Maquinas */
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-36.0f, 0.0f, 47.9f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		maquinas.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 43.f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		cartel.Draw(lightingShader);

		/* DINOSAURIOS */

		/* Brontosaurio */
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(36.0f, 0.0f, movBronto));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		brontoCu.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(36.0f, 0.0f, 40.0f));
		model = glm::rotate(model, glm::radians(rotCuello), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		brontoCa.Draw(lightingShader);
		

		/* Pterodactilo */
		model = glm::mat4(1);
		model = glm::translate(model, PosIniPtero + glm::vec3(movKitX, 0, movKitZ));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		pteroCu.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, PosIniPtero + glm::vec3(movKitX, 0, movKitZ));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotAlaD), glm::vec3(0.0f, 0.0f, 1.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		pteroAlaD.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, PosIniPtero + glm::vec3(movKitX, 0, movKitZ));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotAlaI), glm::vec3(0.0f, 0.0f, 1.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		pteroAlaI.Draw(lightingShader);


		/* Velociraptor */
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(36.0f, 0.0f, -40.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		veloCu.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(36.0f, 0.0f, -40.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		veloHomD.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(36.0f, 0.0f, -40.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		veloBraD.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(36.0f, 0.0f, -40.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		veloHomI.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(36.0f, 0.0f, -40.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		veloBraI.Draw(lightingShader);

		///* T-Rex */
		model = glm::mat4(1);
		model = glm::translate(model, PosIniTrex + glm::vec3(movKitX2, 0, movKitZ2));
		model = glm::rotate(model, glm::radians(rotKit2), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		tRexCu.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, PosIniTrex + correcionD + glm::vec3(movKitX2, 0, movKitZ2));
		model = glm::rotate(model, glm::radians(rotKit2), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotPataD), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		tRexPD.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, PosIniTrex + correcionI + glm::vec3(movKitX2, 0, movKitZ2));
		model = glm::rotate(model, glm::radians(rotKit2), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotPataI), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		tRexPI.Draw(lightingShader);

		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
	}

	if (keys[GLFW_KEY_L])
	{
		luces = !luces;
		// Luz
		if (luces)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 1.0f);

		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void animacion()
{
	if (active)
	{
		animacionPtero();
		animacionBronto();
		animacionTrex();
	}
}

void animacionPtero()
{
	//Movimiento del Ptero
	if (recorrido1)
	{
		movKitZ += 0.1f;
		if (movKitZ > 70)
		{
			recorrido1 = false;
			recorrido2 = true;
		}
	}
	if (recorrido2)
	{
		rotKit = 90;
		movKitX += 0.1f;
		if (movKitX > 70)
		{
			recorrido2 = false;
			recorrido3 = true;

		}
	}

	if (recorrido3)
	{
		rotKit = 180;
		movKitZ -= 0.1f;
		if (movKitZ < 0)
		{
			recorrido3 = false;
			recorrido4 = true;
		}
	}

	if (recorrido4)
	{
		rotKit = 270;
		movKitX -= 0.1f;
		if (movKitX < 0)
		{
			recorrido4 = false;
			recorrido5 = true;
		}
	}
	if (recorrido5)
	{
		rotKit = 0;
		movKitZ += 0.1f;
		if (movKitZ > 0)
		{
			recorrido5 = false;
			recorrido1 = true;
		}
	}
	// Alas del Ptero
	if (rotAlaI < 20 && !alas)
	{
		rotAlaI += 0.1f;
		rotAlaD -= 0.1f;
	}
	else
		alas = true;
	if (rotAlaI > -20 && alas)
	{
		rotAlaI -= 0.1f;
		rotAlaD += 0.1f;
	}
	else
		alas = false;
}

void animacionBronto()
{
	if (rotCuello < 5 && !cuello)
	{
		rotCuello += 0.1f;
		movBronto += 0.006f;
	}
	else
		cuello = true;
	if (rotCuello > 0 && cuello)
	{
		rotCuello -= 0.1f;
		movBronto -= 0.006f;
	}	
	else
		cuello = false;
}

void animacionTrex()
{
	// Recorrido del T-Rex
	if (camino1)
	{
		movKitX2 += 0.1f;
		if (movKitX2 > 10)
		{
			camino1 = false;
			camino2 = true;
			correcionD = glm::vec3(-0.54f, 5.2f, -1.22f);
			correcionI = glm::vec3(1.22f, 5.2f, 0.54f);
		}
	}
	if (camino2)
	{
		rotKit2 = -45;
		movKitZ2 += 0.2f;
		movKitX2 -= 0.1f;
		if (movKitZ2 > 25)
		{
			camino2 = false;
			camino3 = true;
			correcionD = glm::vec3(1.0f, 5.2f, 0.5f);
			correcionI = glm::vec3(-1.0f, 5.2f, 0.5f);
		}
	}
	if (camino3)
	{
		rotKit2 = 180;
		movKitZ2 -= 0.1f;
		if (movKitZ2 < 0)
		{
			camino3 = false;
			camino1 = true;
			rotKit2 = 90;
			correcionD = glm::vec3(-0.5f, 5.2f, 1.1f);
			correcionI = glm::vec3(-0.5f, 5.2f, -1.1f);
		}
	}
	// Patas del T-Rex
	if (rotPataI < 20 && !patas)
	{
		rotPataI += 0.5f;
		rotPataD -= 0.5f;
	}
	else
		patas = true;
	if (rotPataI > -20 && patas)
	{
		rotPataI -= 0.5f;
		rotPataD += 0.5f;
	}
	else
		patas = false;
}