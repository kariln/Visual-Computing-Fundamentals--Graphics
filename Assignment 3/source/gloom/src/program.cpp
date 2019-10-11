// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "toolbox.hpp"
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

//when loading the mesh-assignment 3
#include "OBJLoader.hpp"
#include "sceneGraph.hpp"

float rotX = 0;
float rotY = 0;
float cordX = 0;
float cordY = 0;
float cordZ = -5;

float v_rotX = 0;
float v_rotY = 0;
float v_cordX = 0;
float v_cordY = 0;
float v_cordZ = -0;

float a_rot = 90;
float a_tra = 30;

Helicopter my_copter;
float my_roll = 0;
float my_pitch = 0;
float v = 0;

float fps = 40;
float dt = 0;
float t = 0;

float door_pos = 0;
unsigned int sceneSetup(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> colours, std::vector<float> normals);

void updateSceneNode(SceneNode* node, glm::mat4 transformationThusFar) {
	// Do transformations here
	// glm::mat4 transl = glm::translate(glm::vec3(-node->referencePoint.x, -node->referencePoint.y, -node->referencePoint.z));
	glm::mat4 transl    = glm::translate(-node->referencePoint);
	glm::mat4 translInv = glm::translate(+node->referencePoint);
	glm::mat4 rot = glm::rotate(node->rotation.y, glm::vec3(0, 1, 0)) * glm::rotate(node->rotation.x, glm::vec3(1, 0, 0)) * glm::rotate(node->rotation.z, glm::vec3(0, 0, 1)) ;

	glm::mat4 totalRot = translInv * rot * transl;
	glm::mat4 translation = glm::translate(node->position);
	glm::mat4 combinedTransformation =  transformationThusFar * translation * totalRot ;

	node->currentTransformationMatrix = combinedTransformation;

	for (SceneNode* child : node->children) {
		updateSceneNode(child, combinedTransformation);
	}
}

unsigned int create_VAO_Mesh(Mesh mesh)
{
			return sceneSetup(mesh.vertices, mesh.indices,mesh.colours, mesh.normals);
}

//creating the scene
unsigned int sceneSetup(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> colours, std::vector<float> normals){

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

			//create colour buffer
			unsigned int colours_buffer = 0;
			glGenBuffers(1, &colours_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, colours_buffer);
			glBufferData(GL_ARRAY_BUFFER, (colours.size())* sizeof(float), colours.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
			printGLError();

			//create normal buffer
			unsigned int normals_buffer = 0;
			glGenBuffers(1, &normals_buffer);
			glBindBuffer(GL_ARRAY_BUFFER,normals_buffer);
			glBufferData(GL_ARRAY_BUFFER, (normals.size())*sizeof(float), normals.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

      glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
      printGLError();

			unsigned int index_buffer = 0;
			glGenBuffers(1, &index_buffer);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),indices.data(), GL_STATIC_DRAW);

      return arrayID;
}

SceneNode* create_Node_Mesh(Mesh mesh){
	//set VAO and indexCount
	SceneNode* sceneNode = createSceneNode();

	//sets the VAO
	unsigned int arrayID = create_VAO_Mesh(mesh);
	sceneNode->vertexArrayObjectID = arrayID;

	//sets the index count
	unsigned int ind_count = 0;
	sceneNode->VAOIndexCount = mesh.indices.size();
	return sceneNode;
}

void drawSceneNode(SceneNode* root, glm::mat4 viewProjectionMatrix){
	if(root -> vertexArrayObjectID != -1){
		glBindVertexArray(root -> vertexArrayObjectID);
		glUniformMatrix4fv(0,1,GL_FALSE, &(root->currentTransformationMatrix)[0][0]);
		glUniformMatrix4fv(1,1,GL_FALSE, &((viewProjectionMatrix*root->currentTransformationMatrix)[0][0]));
		glDrawElements(GL_TRIANGLES, root->VAOIndexCount, GL_UNSIGNED_INT, 0);//is this right
	}
	for (SceneNode* child : root -> children){
		drawSceneNode(child,viewProjectionMatrix);
	}
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
    glEnable(GL_CULL_FACE);
    printGLError();

    // Set default colour after clearing the colour buffer
    glClearColor(0.6f, 0.3f, 0.3f, 1.0f);

    // Set up your scene here (create Vertex Array Objects, etc.)

		//load lunarTerrain
		std::string const model_terrain = "../gloom/resources/lunarsurface.obj";
		Mesh Terrain = loadTerrainMesh(model_terrain);
		unsigned int arrayID = create_VAO_Mesh(Terrain);

		//loadHelicopter
		std::string const model_helicopter = "../gloom/resources/helicopter.obj";
		Helicopter helicopter = loadHelicopterModel(model_helicopter);
		Mesh meshes[4] = {helicopter.body, helicopter.mainRotor, helicopter.tailRotor, helicopter.door};

		//creating a sceneGraph

		//create hierarchy and scenegraph
		SceneNode* rootNode = createSceneNode();
		SceneNode* terrainNode = create_Node_Mesh(Terrain);
		addChild(rootNode,terrainNode);

		//number of helicopters
		unsigned int num_heli = 5;
		std::vector<Helicopter> heli_list;

		for(int i = 1 ; i < num_heli +1; i++){

			SceneNode* bodyNode = create_Node_Mesh(meshes[0]);
			SceneNode* mainRotorNode = create_Node_Mesh(meshes[1]);
			SceneNode* tailRotorNode = create_Node_Mesh(meshes[2]);
			SceneNode* doorNode = create_Node_Mesh(meshes[3]);

			// assign reference points
			terrainNode->referencePoint = glm::vec3(0.0f, 0.0f, 0.0f);
			bodyNode->referencePoint = glm::vec3(0, 0, 0);
			mainRotorNode->referencePoint = glm::vec3(0, 0, 0);
			tailRotorNode->referencePoint = glm::vec3(0.35, 2.3, 10.4);
			doorNode->referencePoint = glm::vec3(0, 0, 0);

			//creates the hierarchy
			addChild(terrainNode,bodyNode);
			addChild(bodyNode,mainRotorNode);
			addChild(bodyNode,tailRotorNode);
			addChild(bodyNode,doorNode);

			//creates the helicopter
			Helicopter helicopter;
			helicopter.bodyNode = bodyNode;
			helicopter.mainRotorNode = mainRotorNode;
			helicopter.tailRotorNode = tailRotorNode;
			helicopter.doorNode = doorNode;

			//adds the helicopter to the list
			heli_list.push_back(helicopter);
		}

		SceneNode* bodyNode = create_Node_Mesh(meshes[0]);
		SceneNode* mainRotorNode = create_Node_Mesh(meshes[1]);
		SceneNode* tailRotorNode = create_Node_Mesh(meshes[2]);
		SceneNode* doorNode = create_Node_Mesh(meshes[3]);

			// assign reference points
			terrainNode->referencePoint = glm::vec3(0.0f, 0.0f, 0.0f);
			bodyNode->referencePoint = glm::vec3(0, 0, 0);
			mainRotorNode->referencePoint = glm::vec3(0, 0, 0);
			tailRotorNode->referencePoint = glm::vec3(0.35, 2.3, 10.4);
			doorNode->referencePoint = glm::vec3(0, 0, 0);

			//creates the hierarchy
			addChild(terrainNode,bodyNode);
			addChild(bodyNode,mainRotorNode);
			addChild(bodyNode,tailRotorNode);
			addChild(bodyNode,doorNode);

			//creates the helicopter
			my_copter.bodyNode = bodyNode;
			my_copter.mainRotorNode = mainRotorNode;
			my_copter.tailRotorNode = tailRotorNode;
			my_copter.doorNode = doorNode;

    //making a shader object
    Gloom::Shader shader ;
    shader.makeBasicShader("../gloom/shaders/simple.vert","../gloom/shaders/simple.frag");
    shader.activate();

		Heading path;
    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        printGLError();
				dt = getTimeDeltaSeconds();
				t += dt;
				// fps = 1/dt;



				//to create the offset between the helicopters
				int index = 0;

				// update animations
				for(Helicopter helicopter : heli_list){
					index += 1;
					helicopter.mainRotorNode->rotation.y += 30*dt;
					helicopter.tailRotorNode->rotation.x += 20*dt;

					path = simpleHeadingAnimation(t+index*3);
					helicopter.bodyNode->position.x = path.x;
					helicopter.bodyNode->position.z = path.z;
					helicopter.bodyNode->position.y = 20;

					helicopter.bodyNode->rotation.x = path.pitch;
					helicopter.bodyNode->rotation.y = path.yaw;
					helicopter.bodyNode->rotation.z = path.roll;

					helicopter.doorNode->position.z = door_pos;
				}

				my_copter.mainRotorNode->rotation.y += 30*dt;
				my_copter.tailRotorNode->rotation.x += 20*dt;

				my_copter.bodyNode->position.x = -cordX;
				my_copter.bodyNode->position.z = -cordZ;
				my_copter.bodyNode->position.y = -cordY;

				my_copter.bodyNode->rotation.x = -my_pitch;//path.pitch;
				my_copter.bodyNode->rotation.y = -glm::radians(rotY);//path.yaw;
				my_copter.bodyNode->rotation.z = -my_roll;//path.roll;

				my_copter.doorNode->position.z = door_pos;

        // Draw your scene here

				//drawing triangles
				 glm::mat4x4 matrix = glm::mat4(1);
				 glm::mat4x4 persp = glm::perspective(glm::radians(45.0f+v/2), 4.0f/3.0f, 1.0f, 2000.0f);
				 //translation
         glm::mat4x4 translate = glm::translate(glm::vec3(cordX, cordY, cordZ));
				 glm::mat4x4 third_person= glm::translate(glm::vec3(0, -5, -25));
         // rotation
         glm::mat4x4 rotXMatrix = glm::rotate(glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
         glm::mat4x4 rotYMartix = glm::rotate(glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));

				 matrix = persp*third_person*rotXMatrix*rotYMartix*translate; // this is for the camera
			   // glUniformMatrix4fv(0,1,GL_FALSE, &matrix[0][0]);

				 updateSceneNode(rootNode, glm::mat4(1));
				 drawSceneNode(rootNode, matrix);

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

void handleKeyboardInput(GLFWwindow* window) // how can this update variables outside its scope???
{
		// Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		{
			door_pos += 30*dt;
		}

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
			v_cordZ += (cos(glm::radians(rotY))) * a_tra*dt; // (cos(glm::radians(rotX)))* a_tra*dt;
			v_cordX -= (sin(glm::radians(rotY))) * a_tra*dt; // (cos(glm::radians(rotX)))* a_tra*dt;
			// v_cordY += (sin(glm::radians(rotX)))*a_tra*dt;
			my_pitch += 0.5*dt;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
			v_cordZ -= (cos(glm::radians(rotY)))* a_tra*dt; // (cos(glm::radians(rotX)))* a_tra*dt;
			v_cordX += (sin(glm::radians(rotY)))*  a_tra*dt;// (cos(glm::radians(rotX)))*a_tra*dt;
		//	v_cordY -= (sin(glm::radians(rotX)))*a_tra*dt;
			my_pitch -= 0.5*dt;

    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
			  v_cordZ += 1*(sin(glm::radians(rotY))) * (cos(glm::radians(rotX)))*a_tra*dt;
			  v_cordX += 1*(cos(glm::radians(rotY))) * (cos(glm::radians(rotX)))*a_tra*dt;
				my_roll -= 1*dt;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
			  v_cordZ -= (sin(glm::radians(rotY))) * (cos(glm::radians(rotX)))*a_tra*dt;
			  v_cordX -= (cos(glm::radians(rotY))) * (cos(glm::radians(rotX)))*a_tra*dt;
				my_roll += 1*dt;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
			v_cordY -= a_tra*dt;//(cos(glm::radians(rotX)))*a_tra*dt;
			// v_cordZ += (cos(glm::radians(rotY))) * (sin(glm::radians(rotX)))*a_tra*dt;
			// v_cordX -= (sin(glm::radians(rotY))) * (sin(glm::radians(rotX)))*a_tra*dt;


    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {

				v_cordY += a_tra*dt;//(cos(glm::radians(rotX)))*30/fps;
				// v_cordZ -= (cos(glm::radians(rotY))) * (sin(glm::radians(rotX)))*a_tra*dt;
				// v_cordX += (sin(glm::radians(rotY))) * (sin(glm::radians(rotX)))*a_tra*dt;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        rotX -= 30*dt;
				if(rotX < -90){rotX = -90;}
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        rotX += 30*dt;
				if(rotX > 90){rotX = 90;}
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        v_rotY -= a_rot*dt;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        v_rotY += a_rot*dt;
		}
		if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS))
		{
				my_roll += 1*dt;
		}
		if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS))
		{
				my_roll -= 1*dt;
		}


		rotX += v_rotX*dt;
		rotY += v_rotY*dt;

		v_rotX -= 0.7*v_rotX*dt;
		v_rotY -= 0.7*v_rotY*dt;


		cordX += v_cordX*dt;
		cordY += v_cordY*dt;
		cordZ += v_cordZ*dt;

		v_cordX -= 0.3*v_cordX*dt;//(1-resistance*dt);
		v_cordY -= 0.3*v_cordY*dt;//(1-resistance*dt);
		v_cordZ -= 0.3*v_cordZ*dt;//(1-resistance*dt);

		v = sqrt(pow(v_cordX,2)+pow(v_cordY,2)+pow(v_cordZ,2));
		my_pitch -= 0.8*my_pitch*dt;
		my_roll -= 0.8*my_roll*dt;


		if(my_roll > glm::radians(65.0f)){my_roll = glm::radians(65.0f);}
		if(my_roll < glm::radians(-65.0f)){my_roll = glm::radians(-65.0f);}
		if(my_pitch > glm::radians(65.0f)){my_pitch = glm::radians(65.0f);}
		if(my_pitch < glm::radians(-65.0f)){my_pitch = glm::radians(-65.0f);}
}
