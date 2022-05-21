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
#include "Texture.h" /*SKYBOX*/

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
void animacionPer();
void animKeyFrame();

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
glm::vec3 PosIniHI(34.89f, 1.85f, -39.76);
glm::vec3 PosIniBI(34.89f, 1.85f, -39.76);
glm::vec3 PosIniHD(34.89f, 1.85f, -40.21f);
glm::vec3 PosIniBD(34.89f, 1.85f, -40.21f);
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

// Animacion Persona
bool brazo = false;
float rotBrazo = 0.0f;


// Positions of the point lights
// Indicar en el lighting.frag el num de point ligths y agregar sus posiciones aqui
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f, 7.5f, 40.7f)
};

glm::vec3 Light1 = glm::vec3(0);


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Keyframes
float posX = PosIniHI.x, posY = PosIniHI.y, posZ = PosIniHI.z, rotHI = 0;
float posXBI = PosIniBI.x, posYBI = PosIniBI.y, posZBI = PosIniBI.z, rotBI = 0, rotBIX = 0;
float posXHD = PosIniHD.x, posYHD = PosIniHD.y, posZHD = PosIniHD.z, rotHD = 0; 
float posXBD = PosIniBD.x, posYBD = PosIniBD.y, posZBD = PosIniBD.z, rotBD = 0, rotBDX = 0;

#define MAX_FRAMES 9
int i_max_steps = 120;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotHI;
	float rotIncHI;

	float posXBI;		//Variable para PosicionX
	float posYBI;		//Variable para PosicionY
	float posZBI;		//Variable para PosicionZ
	float incXBI;		//Variable para IncrementoX
	float incYBI;		//Variable para IncrementoY
	float incZBI;		//Variable para IncrementoZ
	float rotBI;
	float rotIncBI;
	float rotBIX;
	float rotIncBIX;

	float posXHD;		//Variable para PosicionX
	float posYHD;		//Variable para PosicionY
	float posZHD;		//Variable para PosicionZ
	float incXHD;		//Variable para IncrementoX
	float incYHD;		//Variable para IncrementoY
	float incZHD;		//Variable para IncrementoZ
	float rotHD;
	float rotIncHD;

	float posXBD;		//Variable para PosicionX
	float posYBD;		//Variable para PosicionY
	float posZBD;		//Variable para PosicionZ
	float incXBD;		//Variable para IncrementoX
	float incYBD;		//Variable para IncrementoY
	float incZBD;		//Variable para IncrementoZ
	float rotBD;
	float rotIncBD;
	float rotBDX;
	float rotIncBDX;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 8;			//introducir datos
bool play = false;
int playIndex = 0;


void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotHI = KeyFrame[0].rotHI;

	posXBI = KeyFrame[0].posXBI;
	posYBI = KeyFrame[0].posYBI;
	posZBI = KeyFrame[0].posZBI;

	rotBI = KeyFrame[0].rotBI;
	rotBIX = KeyFrame[0].rotBIX;

	posXHD = KeyFrame[0].posXHD;
	posYHD = KeyFrame[0].posYHD;
	posZHD = KeyFrame[0].posZHD;

	rotHD = KeyFrame[0].rotHD;

	posXBD = KeyFrame[0].posXBD;
	posYBD = KeyFrame[0].posYBD;
	posZBD = KeyFrame[0].posZBD;

	rotBD = KeyFrame[0].rotBD;
	rotBDX = KeyFrame[0].rotBDX;


}

void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	KeyFrame[playIndex].rotIncHI = (KeyFrame[playIndex + 1].rotHI - KeyFrame[playIndex].rotHI) / i_max_steps;

	KeyFrame[playIndex].incXBI = (KeyFrame[playIndex + 1].posXBI - KeyFrame[playIndex].posXBI) / i_max_steps;
	KeyFrame[playIndex].incYBI = (KeyFrame[playIndex + 1].posYBI - KeyFrame[playIndex].posYBI) / i_max_steps;
	KeyFrame[playIndex].incZBI = (KeyFrame[playIndex + 1].posZBI - KeyFrame[playIndex].posZBI) / i_max_steps;

	KeyFrame[playIndex].rotIncBI = (KeyFrame[playIndex + 1].rotBI - KeyFrame[playIndex].rotBI) / i_max_steps;
	KeyFrame[playIndex].rotIncBIX = (KeyFrame[playIndex + 1].rotBIX - KeyFrame[playIndex].rotBIX) / i_max_steps;

	KeyFrame[playIndex].incXHD = (KeyFrame[playIndex + 1].posXHD - KeyFrame[playIndex].posXHD) / i_max_steps;
	KeyFrame[playIndex].incYHD = (KeyFrame[playIndex + 1].posYHD - KeyFrame[playIndex].posYHD) / i_max_steps;
	KeyFrame[playIndex].incZHD = (KeyFrame[playIndex + 1].posZHD - KeyFrame[playIndex].posZHD) / i_max_steps;

	KeyFrame[playIndex].rotIncHD = (KeyFrame[playIndex + 1].rotHD - KeyFrame[playIndex].rotHD) / i_max_steps;

	KeyFrame[playIndex].incXBD = (KeyFrame[playIndex + 1].posXBD - KeyFrame[playIndex].posXBD) / i_max_steps;
	KeyFrame[playIndex].incYBD = (KeyFrame[playIndex + 1].posYBD - KeyFrame[playIndex].posYBD) / i_max_steps;
	KeyFrame[playIndex].incZBD = (KeyFrame[playIndex + 1].posZBD - KeyFrame[playIndex].posZBD) / i_max_steps;

	KeyFrame[playIndex].rotIncBD = (KeyFrame[playIndex + 1].rotBD - KeyFrame[playIndex].rotBD) / i_max_steps;
	KeyFrame[playIndex].rotIncBDX = (KeyFrame[playIndex + 1].rotBDX - KeyFrame[playIndex].rotBDX) / i_max_steps;

}

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag"); /* SKYBOX */

	/* Parque */
	Model piso ((char*)"Models/Piso/Piso.obj");
	Model habitats ((char*)"Models/Piso/habitats2.obj");
	Model caja ((char*)"Models/Piso/Caja.obj");
	Model barrotes ((char*)"Models/Piso/barrotes.obj");
	Model lampara ((char*)"Models/Piso/lampara.obj");
	Model lamparas ((char*)"Models/Piso/streetlamp.obj");
	Model mateo ((char*)"Models/Piso/mateo.obj");
	
	Model fuente ((char*)"Models/Sodas/Fuente/fuenteC.obj");
	Model fuenteAgua ((char*)"Models/Sodas/Fuente/fuenteA.obj");
	Model mesa ((char*)"Models/Sodas/Mesa/mesa.obj");
	Model maquinas ((char*)"Models/Sodas/Maquina/maquinas.obj");

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

	Model personaCu ((char*)"Models/Persona/personaCu.obj");
	Model personaBra ((char*)"Models/Persona/personaBraT.obj");

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotHI = 0;
		KeyFrame[i].rotIncHI = 0;
	}

	// Guargando animacion KEYFRAMES 0
	KeyFrame[0].posXHD = posXHD;
	KeyFrame[0].posYHD = posYHD;
	KeyFrame[0].posZHD = posZHD;
	KeyFrame[0].rotHD = rotHD;

	KeyFrame[0].posXBD = posXBD;
	KeyFrame[0].posYBD = posYBD;
	KeyFrame[0].posZBD = posZBD;
	KeyFrame[0].rotBD = rotBD;
	KeyFrame[0].rotBDX = rotBDX;

	KeyFrame[0].posX = posX;
	KeyFrame[0].posY = posY;
	KeyFrame[0].posZ = posZ;
	KeyFrame[0].rotHI = rotHI;

	KeyFrame[0].posXBI = posXBI;
	KeyFrame[0].posYBI = posYBI;
	KeyFrame[0].posZBI = posZBI;
	KeyFrame[0].rotBI = rotBI;
	KeyFrame[0].rotBIX = rotBIX;


	// Guargando animacion KEYFRAMES 1
	KeyFrame[1].posXHD = posXHD;
	KeyFrame[1].posYHD = posYHD;
	KeyFrame[1].posZHD = posZHD;
	KeyFrame[1].rotHD = -90.0f;

	KeyFrame[1].posXBD = posXBD;
	KeyFrame[1].posYBD = posYBD;
	KeyFrame[1].posZBD = posZBD;
	KeyFrame[1].rotBD = -90.0f;
	KeyFrame[1].rotBDX = 0.0f;
	
	KeyFrame[1].posX = posX;
	KeyFrame[1].posY = posY;
	KeyFrame[1].posZ = posZ;
	KeyFrame[1].rotHI = 0.0f;

	KeyFrame[1].posXBI = posXBI;
	KeyFrame[1].posYBI = posYBI;
	KeyFrame[1].posZBI = posZBI;
	KeyFrame[1].rotBI = 0.0f;
	KeyFrame[1].rotBIX = 0.0f;

	// Guargando animacion KEYFRAMES 2
	KeyFrame[2].posXHD = posXHD;
	KeyFrame[2].posYHD = posYHD;
	KeyFrame[2].posZHD = posZHD;
	KeyFrame[2].rotHD = -90.0f;

	KeyFrame[2].posXBD = posXBD;
	KeyFrame[2].posYBD = posYBD;
	KeyFrame[2].posZBD = posZBD;
	KeyFrame[2].rotBD = -90.0f;
	KeyFrame[2].rotBDX = 0.0f;
	
	KeyFrame[2].posX = posX;
	KeyFrame[2].posY = posY;
	KeyFrame[2].posZ = posZ;
	KeyFrame[2].rotHI = -90.0f;

	KeyFrame[2].posXBI = posXBI;
	KeyFrame[2].posYBI = posYBI;
	KeyFrame[2].posZBI = posZBI;
	KeyFrame[2].rotBI = -90.0f;
	KeyFrame[2].rotBIX = 0.0f;

	// Guargando animacion KEYFRAMES 3
	KeyFrame[3].posXHD = posXHD;
	KeyFrame[3].posYHD = posYHD;
	KeyFrame[3].posZHD = posZHD;
	KeyFrame[3].rotHD = -90.0f;

	KeyFrame[3].posXBD = posXBD;
	KeyFrame[3].posYBD = posYBD;
	KeyFrame[3].posZBD = posZBD - 0.1;
	KeyFrame[3].rotBD = -90.0f;
	KeyFrame[3].rotBDX = -15.0f;

	KeyFrame[3].posX = posX;
	KeyFrame[3].posY = posY;
	KeyFrame[3].posZ = posZ;
	KeyFrame[3].rotHI = -90.0f;

	KeyFrame[3].posXBI = posXBI;
	KeyFrame[3].posYBI = posYBI;
	KeyFrame[3].posZBI = posZBI;
	KeyFrame[3].rotBI = -90.0f;
	KeyFrame[3].rotBIX = 0.0f;

	// Guargando animacion KEYFRAMES 4
	KeyFrame[4].posXHD = posXHD;
	KeyFrame[4].posYHD = posYHD;
	KeyFrame[4].posZHD = posZHD;
	KeyFrame[4].rotHD = -90.0f;

	KeyFrame[4].posXBD = posXBD;
	KeyFrame[4].posYBD = posYBD;
	KeyFrame[4].posZBD = posZBD - 0.1;
	KeyFrame[4].rotBD = -90.0f;
	KeyFrame[4].rotBDX = -15.0f;

	KeyFrame[4].posX = posX;
	KeyFrame[4].posY = posY;
	KeyFrame[4].posZ = posZ;
	KeyFrame[4].rotHI = -90.0f;

	KeyFrame[4].posXBI = posXBI;
	KeyFrame[4].posYBI = posYBI;
	KeyFrame[4].posZBI = posZBI + 0.1;
	KeyFrame[4].rotBI = -90.0f;
	KeyFrame[4].rotBIX = 15.0f;

	// Guargando animacion KEYFRAMES 5
	KeyFrame[5].posXHD = posXHD;
	KeyFrame[5].posYHD = posYHD;
	KeyFrame[5].posZHD = posZHD;
	KeyFrame[5].rotHD = -120.0f;

	KeyFrame[5].posXBD = posXBD;
	KeyFrame[5].posYBD = posYBD;
	KeyFrame[5].posZBD = posZBD;
	KeyFrame[5].rotBD = -120.0f;
	KeyFrame[5].rotBDX = 0.0f;

	KeyFrame[5].posX = posX;
	KeyFrame[5].posY = posY;
	KeyFrame[5].posZ = posZ;
	KeyFrame[5].rotHI = -90.0f;

	KeyFrame[5].posXBI = posXBI;
	KeyFrame[5].posYBI = posYBI;
	KeyFrame[5].posZBI = posZBI + 0.1;
	KeyFrame[5].rotBI = -90.0f;
	KeyFrame[5].rotBIX = 15.0f;

	// Guargando animacion KEYFRAMES 6
	KeyFrame[6].posXHD = posXHD;
	KeyFrame[6].posYHD = posYHD;
	KeyFrame[6].posZHD = posZHD;
	KeyFrame[6].rotHD = -120.0f;

	KeyFrame[6].posXBD = posXBD;
	KeyFrame[6].posYBD = posYBD;
	KeyFrame[6].posZBD = posZBD;
	KeyFrame[6].rotBD = -120.0f;
	KeyFrame[6].rotBDX = 0.0f;

	KeyFrame[6].posX = posX;
	KeyFrame[6].posY = posY;
	KeyFrame[6].posZ = posZ;
	KeyFrame[6].rotHI = -120.0f;

	KeyFrame[6].posXBI = posXBI;
	KeyFrame[6].posYBI = posYBI;
	KeyFrame[6].posZBI = posZBI;
	KeyFrame[6].rotBI = -120.0f;
	KeyFrame[6].rotBIX = 0.0f;

	// Guargando animacion KEYFRAMES 7
	KeyFrame[7].posXHD = posXHD;
	KeyFrame[7].posYHD = posYHD;
	KeyFrame[7].posZHD = posZHD;
	KeyFrame[7].rotHD = 0.0f;

	KeyFrame[7].posXBD = posXBD;
	KeyFrame[7].posYBD = posYBD;
	KeyFrame[7].posZBD = posZBD;
	KeyFrame[7].rotBD = -.0f;
	KeyFrame[7].rotBDX = 0.0f;

	KeyFrame[7].posX = posX;
	KeyFrame[7].posY = posY;
	KeyFrame[7].posZ = posZ;
	KeyFrame[7].rotHI = 0.0f;

	KeyFrame[7].posXBI = posXBI;
	KeyFrame[7].posYBI = posYBI;
	KeyFrame[7].posZBI = posZBI;
	KeyFrame[7].rotBI = 0.0f;
	KeyFrame[7].rotBIX = 0.0f;

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	/*SKYBOX*/
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	/*glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);*/

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.jpg");
	faces.push_back("SkyBox/left.jpg");
	faces.push_back("SkyBox/top.jpg");
	faces.push_back("SkyBox/bottom.jpg");
	faces.push_back("SkyBox/back.jpg");
	faces.push_back("SkyBox/front.jpg");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);
	/*SKYBOX*/
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

		/*SKYBOX*/
		// OpenGL options
		glEnable(GL_DEPTH_TEST);
		/*SKYBOX*/

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
		lightColor.x = Light1.x;
		lightColor.y = Light1.y;
		lightColor.z = Light1.z;

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
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
		piso.Draw(lightingShader);

		/* Lampara */
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		lampara.Draw(lightingShader);

		/* Lamparas */
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		lamparas.Draw(lightingShader);

		/* Mateo */
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		mateo.Draw(lightingShader);

		/* Habitats */
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		habitats.Draw(lightingShader);

		/* Caja */
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		caja.Draw(lightingShader);

		/* Barrotes */
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		barrotes.Draw(lightingShader);

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
		model = glm::translate(model, glm::vec3(posXHD, posYHD, posZHD));
		model = glm::rotate(model, glm::radians(rotHD), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		veloHomD.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posXBD, posYBD, posZBD));
		model = glm::rotate(model, glm::radians(rotBD), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(rotBDX), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		veloBraD.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rotHI), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		veloHomI.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posXBI, posYBI, posZBI));
		model = glm::rotate(model, glm::radians(rotBI), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(rotBIX), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		veloBraI.Draw(lightingShader);

		/* T-Rex */
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

		/* Persona */
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 33.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		personaCu.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.51f, 2.6f, 33.3f));
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, glm::radians(rotBrazo), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		personaBra.Draw(lightingShader);

		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		/*SKYBOX*/
		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default
		/*SKYBOX*/

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	/*SKYBOX*/
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	/*SKYBOX*/
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
		animacionPer();
		animKeyFrame();
	}
}

void animacionPtero()
{
	//Movimiento del Ptero
	if (recorrido1)
	{
		movKitZ += 0.5f;
		if (movKitZ > 70)
		{
			recorrido1 = false;
			recorrido2 = true;
		}
	}
	if (recorrido2)
	{
		rotKit = 90;
		movKitX += 0.5f;
		if (movKitX > 70)
		{
			recorrido2 = false;
			recorrido3 = true;

		}
	}

	if (recorrido3)
	{
		rotKit = 180;
		movKitZ -= 0.5f;
		if (movKitZ < 0)
		{
			recorrido3 = false;
			recorrido4 = true;
		}
	}

	if (recorrido4)
	{
		rotKit = 270;
		movKitX -= 0.5f;
		if (movKitX < 0)
		{
			recorrido4 = false;
			recorrido5 = true;
		}
	}
	if (recorrido5)
	{
		rotKit = 0;
		movKitZ += 0.5f;
		if (movKitZ > 0)
		{
			recorrido5 = false;
			recorrido1 = true;
		}
	}
	// Alas del Ptero
	if (rotAlaI < 20 && !alas)
	{
		rotAlaI += 0.5f;
		rotAlaD -= 0.5f;
	}
	else
		alas = true;
	if (rotAlaI > -20 && alas)
	{
		rotAlaI -= 0.5f;
		rotAlaD += 0.5f;
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

void animacionPer()
{
	// Brazo de la persona persona
	if (rotBrazo < 30 && !brazo)
		rotBrazo += 0.5f;
	else
		brazo = true;
	if (rotBrazo > 0 && brazo)
		rotBrazo -= 0.5f;
	else
		brazo = false;
}

void animKeyFrame()
{
	if (play == false && (FrameIndex > 1))
	{

		resetElements();
		//First Interpolation				
		interpolation();

		play = true;
		playIndex = 0;
		i_curr_steps = 0;
	}
	if (play)
	{

		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				/*printf("Animaciones KeyFrames\n");*/
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += KeyFrame[playIndex].incX;
			posY += KeyFrame[playIndex].incY;
			posZ += KeyFrame[playIndex].incZ;

			rotHI += KeyFrame[playIndex].rotIncHI;

			posXBI += KeyFrame[playIndex].incXBI;
			posYBI += KeyFrame[playIndex].incYBI;
			posZBI += KeyFrame[playIndex].incZBI;

			rotBI += KeyFrame[playIndex].rotIncBI;
			rotBIX += KeyFrame[playIndex].rotIncBIX;

			posXHD += KeyFrame[playIndex].incXHD;
			posYHD += KeyFrame[playIndex].incYHD;
			posZHD += KeyFrame[playIndex].incZHD;

			rotHD += KeyFrame[playIndex].rotIncHD;

			posXBD += KeyFrame[playIndex].incXBD;
			posYBD += KeyFrame[playIndex].incYBD;
			posZBD += KeyFrame[playIndex].incZBD;

			rotBD += KeyFrame[playIndex].rotIncBD;
			rotBDX += KeyFrame[playIndex].rotIncBDX;

			i_curr_steps++;
		}

	}
}