#include "stdafx.h"



// Include GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>
extern GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include "Control.h"

glm::mat4 ViewMatrix;
glm::mat4 ModelMatrixBall;
glm::mat4 ProjectionMatrix;
glm::mat4 ModelMatrixCube;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}
glm::mat4 getModelMatrixBall(){
	return ModelMatrixBall;
}

glm::mat4 getModelMatrixCube(){
	//ModelMatrixCube = glm::translate(ModelMatrixCube, vec3(2, 2, 2));
	return ModelMatrixCube;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3(0, 10, 10);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = -0.50f;
// Initial Field of View
float initialFoV = 90.0f;

float speed = 13.0f; // 3 units / second
float mouseSpeed = 0.005f;



void computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
	verticalAngle += mouseSpeed * float(768 / 2 - ypos);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
		);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Move forward
	ModelMatrixCube = glm::translate(mat4(1.0), position - vec3(0, 0, 5));


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		position += direction * deltaTime * speed;
		ModelMatrixCube = glm::translate(mat4(1.0), position-vec3(0,0,5));
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
		ModelMatrixCube = glm::translate(mat4(1.0), position - vec3(0, 0,5));

	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		position += right * deltaTime * speed;
		ModelMatrixCube = glm::translate(mat4(1.0), position - vec3(0, 0, 5));

	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		position -= right * deltaTime * speed;
		ModelMatrixCube = glm::translate(mat4(1.0), position - vec3(0, 0, 5));
	}


	float FoV = initialFoV;// -5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
		);
	//ModelMatrixBall
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		ModelMatrixBall = glm::scale(vec3(1, 0.2, 1));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		ModelMatrixBall = glm::scale(vec3(1, 1, 1));
	}


	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}
