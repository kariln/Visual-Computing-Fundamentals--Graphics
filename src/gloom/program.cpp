// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"

//constructing vectors
#include <iostream>
#include <vector>

//for use in circle drawing, sine, cosine and pi
#include <math.h>
unsigned int sceneSetup(std::vector<float> vertices, std::vector<unsigned int> indices);

//drawing a circle by adding several triangles - the triangle fan method
//input:coorinates of centre, radius and number of sides in the triangles
unsigned int drawCircle(float x, float y, float z, float radius, int numberOfSides){
		//finding the number og vertices, which is the number of sides + 1 plus the central vertex
		int numberOfVertices = numberOfSides + 2;

		//saving the 2*pi
		float double_pi = 2.0f*M_PI;

		//vertices array
		float *circleVerticesX = new float [numberOfVertices];
		float *circleVerticesY = new float [numberOfVertices];
		float *circleVerticesZ = new float [numberOfVertices];

		//defining the central point
		circleVerticesX[0] = x;
		circleVerticesY[0] = y;

		circleVerticesZ[0] = z;

		//defining the other vertices from 1 to numberOfVertices
		for (int i=1; i<numberOfVertices;i++){
				circleVerticesX[i] = x + (radius *cos(i*double_pi/numberOfSides ));
				circleVerticesY[i] = y + (radius *sin(i*double_pi/numberOfSides ));
				circleVerticesZ[i] = z; //the circle is going to be in 2D
		}

		std::vector<float> allCircleVertices;
		allCircleVertices.resize(numberOfVertices * 3); //needs to have space for all three arrays made earlier

		std::vector<unsigned int> index_buffer_circle;
		index_buffer_circle.resize(numberOfVertices);
		//making the vertex buffer
		for (int i = 0; i<numberOfVertices; i++){
			allCircleVertices[i*3] = circleVerticesX[i];
			allCircleVertices[(i*3) +1] = circleVerticesY[i];
			allCircleVertices[(i*3)+2] = circleVerticesZ[i];

			index_buffer_circle[i] = i+1;

		}



		return sceneSetup(allCircleVertices,index_buffer_circle);


}


//creating the scene
// vertices.data(), burde denne brukes?
unsigned int sceneSetup(std::vector<float> vertices, std::vector<unsigned int> indices){

      //setting up a VAO and binding the object.What happens when binding?
			//glBindvertexArray() doesn't return anything

			//creating a VAO, vertex array object, and returning its ID.

				unsigned int arrayID = 0;
				glGenVertexArrays(1, &arrayID);
			  printGLError();
				glBindVertexArray(arrayID);
				printGLError();

			//creating a VBO, vertex buffer object, and returning its ID.

				unsigned int buffer = 0;
				glGenBuffers(1, &buffer);
			  printGLError();

      //setting up a VBO witch is a space in the memory for holding the values for the vertices
			//glBindBuffer() doesn't return anything
      glBindBuffer(GL_ARRAY_BUFFER, buffer);
      printGLError();

      //sending the vertices data to the GPU by telling OpenGl that the space must be sufficient for
			//n(triangles*3) floatingpoint numbers corresponding to the vertices
      // ,giving it the vertices and telling it that their values are going to be static since its
			//not changing much

			glBufferData(GL_ARRAY_BUFFER, (vertices.size())* sizeof(float), vertices.data(), GL_STATIC_DRAW);
      printGLError();

      //stride is 0 because we only have one entry type in the vertex array, so we dont have to
			// tell the number of bytes between each entry
      //Pointer is 0 because it the entries starts at 0 in the buffer
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(0);
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
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    printGLError();
    glDepthFunc(GL_LESS);
    printGLError();

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);
    printGLError();

    // Set default colour after clearing the colour buffer
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);
    printGLError();


    // Set up your scene here (create Vertex Array Objects, etc.)

    //Vertex coordinates
      std::vector<float> vertices = {
        -0.6f,-0.6f,0.0f,
        0.6f,-0.6f,0.0f,
        0.0f,0.6f,0.0f,
				0.6f,0.6f,0.0f,
				-0.6f,0.6f,0.0f,
				0.0f,0.0f,0.0f
      };


    //Index buffer - indices
    std::vector<unsigned int> indices = {
      0,1,5
    };

    unsigned int arrayID = sceneSetup(vertices, indices);
		unsigned int arrayID_circle = drawCircle(0.0,0.0,0.0,0.5,360);


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

				printGLError();
				//drawing triangles
				//glBindVertexArray(arrayID);
				//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


				//drawing circle
				glBindVertexArray(arrayID_circle);
				glDrawElements(GL_TRIANGLE_FAN, 360 ,GL_UNSIGNED_INT, 0);
				printGLError();


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
