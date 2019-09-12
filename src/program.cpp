// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
//constructing vectors
#include <iostream>
#include <vector>
//for use in circle drawing, sine, cosine and pi
#include <math.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

unsigned int sceneSetup(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> colors);


//creating the scene
// vertices.data(), burde denne brukes?
unsigned int sceneSetup(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> colors){

      //setting up a VAO and binding the object.What happens when binding?
			//glBindvertexArray() doesn't return anything

			//creating a VAO, vertex array object, and returning its ID.
			unsigned int arrayID = 0;
			glGenVertexArrays(1, &arrayID);
			glBindVertexArray(arrayID);
			printGLError();

			//creating a VBO, vertex buffer object, and returning its ID.

      //setting up a VBO witch is a space in the memory for holding the values for the vertices
			//glBindBuffer() doesn't return anything

      //sending the vertices data to the GPU by telling OpenGl that the space must be sufficient for
			//n(triangles*3) floatingpoint numbers corresponding to the vertices
      // ,giving it the vertices and telling it that their values are going to be static since its
			//not changing much
			unsigned int buffer = 0;
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, (vertices.size())* sizeof(float), vertices.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			printGLError();

			unsigned int colors_buffer = 0;
			glGenBuffers(1, &colors_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, colors_buffer);
			glBufferData(GL_ARRAY_BUFFER, (colors.size())* sizeof(float), colors.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
			printGLError();

      //stride is 0 because we only have one entry type in the vertex array, so we dont have to
			// tell the number of bytes between each entry
      //Pointer is 0 because it the entries starts at 0 in the buffer

      glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
      printGLError();

			unsigned int index_buffer = 0;
			glGenBuffers(1, &index_buffer);

      //setting up a buffer for the indeces -0.25f, -0.5f, 0.0f, so we can tell opengl
			// which vertices who overlap and therefore can be drawn only one time
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),indices.data(), GL_STATIC_DRAW);
      printGLError();
      return arrayID;



}

void runProgram(GLFWwindow* window)
{
		glEnable(GL_BLEND);
		printGLError();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		printGLError();
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    printGLError();
    glDepthFunc(GL_LESS);
    printGLError();

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);
    printGLError();

    // Set default colour after clearing the colour buffer
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    printGLError();


    // Set up your scene here (create Vertex Array Objects, etc.)

		//Vertex coordinates
      std::vector<float> vertices = {
				-0.6f,-0.6f,0.5f,
        0.6f,-0.6f,0.5f,
        0.0f,0.6f,0.5f,

				0.6f,0.6f,0.9f,
				-0.6f,0.6f,0.9f,
				0.0f,0.0f,0.9f,

				-1.0f,-0.3f,0.1f,
				0.7f,0.3f,0.1f,
				-0.6f,0.3f,0.1f

      };

			std::vector<float> colors = {
				0.0f, 1.0f,  1.0f, 0.5f,
				0.0f, 1.0f,  1.0f, 0.5f,
				0.0f, 1.0f,  1.0f, 0.5f,

  			1.0f, 0.0f,  0.0f, 0.5f,
  			1.0f, 0.0f,  0.0f, 0.5f,
  			1.0f, 0.0f,  0.0f, 0.5f,

				1.0f, 1.0f,  1.0f, 0.5f,
				1.0f, 1.0f,  1.0f, 0.5f,
				1.0f, 1.0f,  1.0f, 0.5f
			};


    //Index buffer - indices
    std::vector<unsigned int> indices = {
      0,1,2,3,4,5,6,7,8
    };
    unsigned int arrayID = sceneSetup(vertices, indices, colors);
		//unsigned int arrayID_circle = drawCircle(0.0,0.0,0.0,0.5,360);


    //making a shader object
    Gloom::Shader shader ;
    shader.makeBasicShader("../gloom/shaders/simple.vert","../gloom/shaders/simple.frag");
    printGLError();
    shader.activate();
    printGLError();


    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        printGLError();


        // Draw your scene here

				//drawing triangles
				//make matrix
				glm::mat4x4 matrix = glm::mat4(1);
				//uniform position 0, 1 matrix, no transpose
				glUniformMatrix4fv(0,1,GL_FALSE, &matrix[0][0]);

				glBindVertexArray(arrayID);
				glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


        // Handle other events
        glfwPollEvents();
        printGLError();
        handleKeyboardInput(window);
        printGLError();

        // Flip buffers
        glfwSwapBuffers(window);
        printGLError();

    }

    shader.deactivate();
    printGLError();
}


void handleKeyboardInput(GLFWwindow* window)
{
    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
        printGLError();
    }
}
