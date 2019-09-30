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

//task 4c variables
float rotX = 0;
float rotY = 0;
float cordX = 0;
float cordY = 0;
float cordZ = -5;

//creating the scene
unsigned int sceneSetup(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> colors){

			//creating a VAO, vertex array object, and returning its ID.
			unsigned int arrayID = 0;
			glGenVertexArrays(1, &arrayID);
			glBindVertexArray(arrayID);
			printGLError();

			//creating a VBO, vertex buffer object, and returning its ID.
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

      glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
      printGLError();

			unsigned int index_buffer = 0;
			glGenBuffers(1, &index_buffer);

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
    // Enable depth (Z) buffer
    glEnable(GL_DEPTH_TEST);
    printGLError();
    glDepthFunc(GL_LESS);
    printGLError();

    // Configure miscellaneous OpenGL settings
    //glEnable(GL_CULL_FACE);
    printGLError();

    // Set default colour after clearing the colour buffer
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    printGLError();


    // Set up your scene here (create Vertex Array Objects, etc.)

    //Vertex coordinates
      std::vector<float> vertices = {
				-0.6f,-0.6f,0.5f,
        0.6f,-0.6f,0.5f,
        0.0f,0.6f, 1.5f,

				0.6f,0.6f,0.9f,
				-0.6f,0.6f,0.9f,
				0.0f,0.0f,0.9f,

				-1.0f,-0.3f,0.1f,
				0.7f,0.3f,0.1f,
				-0.6f,0.3f,0.1f,

				 3.0f,3.3f,10.1f,
				 2.7f,4.3f,10.1f,
				 3.6f,6.3f,10.1f
      };

			std::vector<float> colors = {
				1.0f, 0.0f,  0.0f, 0.5f,
				0.0f, 1.0f,  0.0f, 0.5f, // rainbow
				0.0f, 0.0f,  1.0f, 0.5f,

  			1.0f, 0.0f,  0.0f, 0.5f,
  			1.0f, 0.0f,  0.0f, 0.5f,  // red
  			1.0f, 0.0f,  0.0f, 0.5f,

				1.0f, 1.0f,  1.0f, 0.5f,
				1.0f, 1.0f,  1.0f, 0.5f, // white
				1.0f, 1.0f,  1.0f, 0.5f,

				1.0f, 1.0f,  0.0f, 1.0f,
				1.0f, 1.0f,  0.0f, 1.0f, // yellow
				1.0f, 1.0f,  0.0f, 1.0f
			};


    //Index buffer - indices
    std::vector<unsigned int> indices = {
		0,1,2,3,4,5,6,7,8,9,10,11
    };

    unsigned int arrayID = sceneSetup(vertices, indices, colors);

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
				 glm::mat4x4 matrix = glm::mat4(1);
				 glm::mat4x4 persp = glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 100.0f);

				 //translation
         glm::mat4x4 translate = glm::translate(glm::vec3(cordX, cordY, cordZ));

         // rotation
         glm::mat4x4 rotXMatrix = glm::rotate(glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
         glm::mat4x4 rotYMartix = glm::rotate(glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));

				 matrix = persp*rotXMatrix*rotYMartix*translate;
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
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        cordY -= 0.001*(cos(glm::radians(rotX)));
				cordZ += 0.001*(cos(glm::radians(rotY))) * (sin(glm::radians(rotX)));
				cordX -= 0.001*(sin(glm::radians(rotY))) * (sin(glm::radians(rotX)));
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        cordY += 0.001*(cos(glm::radians(rotX)));
				cordZ -= 0.001*(cos(glm::radians(rotY))) * (sin(glm::radians(rotX)));
				cordX += 0.001*(sin(glm::radians(rotY))) * (sin(glm::radians(rotX)));
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
			  cordZ += 0.001*(sin(glm::radians(rotY))) * (cos(glm::radians(rotX)));
			  cordX += 0.001*(cos(glm::radians(rotY))) * (cos(glm::radians(rotX)));
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
			  cordZ -= 0.001*(sin(glm::radians(rotY))) * (cos(glm::radians(rotX)));
			  cordX -= 0.001*(cos(glm::radians(rotY))) * (cos(glm::radians(rotX)));
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        cordZ += 0.001*(cos(glm::radians(rotY))) * (cos(glm::radians(rotX)));
				cordX -= 0.001*(sin(glm::radians(rotY))) * (cos(glm::radians(rotX)));
				cordY += 0.001*(sin(glm::radians(rotX)));
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
		  	cordZ -= 0.001*(cos(glm::radians(rotY)))* (cos(glm::radians(rotX)));
		   	cordX += 0.001*(sin(glm::radians(rotY)))* (cos(glm::radians(rotX)));
				cordY -= 0.001*(sin(glm::radians(rotX)));
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        rotX -= 0.03;
				if(rotX < -90){rotX = -90;}
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        rotX += 0.03;
				if(rotX > 90){rotX = 90;}
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        rotY -= 0.03;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        rotY += 0.03;
		}
}
