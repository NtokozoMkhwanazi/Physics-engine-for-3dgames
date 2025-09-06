#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include "classes/stb_image.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height){

    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){
    
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(){

    unsigned int EBO;
    unsigned int VBO;
    unsigned int VAO;
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;
    
    int success;
    char infoLog[512];
    
    float vertices[] = {
	-0.5f,-0.5f,-0.5f,  0.0f,0.0f,-1.0f,  0.0f,0.0f,
    0.5f,-0.5f,-0.5f,  0.0f,0.0f,-1.0f,  1.0f,0.0f,
    0.5f, 0.5f,-0.5f,  0.0f,0.0f,-1.0f,  1.0f,1.0f,
    0.5f, 0.5f,-0.5f,  0.0f,0.0f,-1.0f,  1.0f,1.0f,
   -0.5f, 0.5f,-0.5f,  0.0f,0.0f,-1.0f,  0.0f,1.0f,
   -0.5f,-0.5f,-0.5f,  0.0f,0.0f,-1.0f,  0.0f,0.0f,
   
   -0.5f,-0.5f, 0.5f,  0.0f,0.0f,1.0f,   0.0f,0.0f,
    0.5f,-0.5f, 0.5f,  0.0f,0.0f,1.0f,   1.0f,0.0f,
    0.5f, 0.5f, 0.5f,  0.0f,0.0f,1.0f,   1.0f,1.0f,
    0.5f, 0.5f, 0.5f,  0.0f,0.0f,1.0f,   1.0f,1.0f,
   -0.5f, 0.5f, 0.5f,  0.0f,0.0f,1.0f,   0.0f,1.0f,
   -0.5f,-0.5f, 0.5f,  0.0f,0.0f,1.0f,   0.0f,0.0f,

   -0.5f, 0.5f, 0.5f, -1.0f,0.0f,0.0f,   1.0f,0.0f,
   -0.5f, 0.5f,-0.5f, -1.0f,0.0f,0.0f,   1.0f,1.0f,
   -0.5f,-0.5f,-0.5f, -1.0f,0.0f,0.0f,   0.0f,1.0f,
   -0.5f,-0.5f,-0.5f, -1.0f,0.0f,0.0f,   0.0f,1.0f,
   -0.5f,-0.5f, 0.5f, -1.0f,0.0f,0.0f,   0.0f,0.0f,
   -0.5f, 0.5f, 0.5f, -1.0f,0.0f,0.0f,   1.0f,0.0f,

    0.5f, 0.5f, 0.5f,  1.0f,0.0f,0.0f,   1.0f,0.0f,
    0.5f, 0.5f,-0.5f,  1.0f,0.0f,0.0f,   1.0f,1.0f,
    0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f,   0.0f,1.0f,
    0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f,   0.0f,1.0f,
    0.5f,-0.5f, 0.5f,  1.0f,0.0f,0.0f,   1.0f,0.0f,
    0.5f, 0.5f, 0.5f,  1.0f,0.0f,0.0f,   0.0f,1.0f,

   -0.5f,-0.5f,-0.5f,  0.0f,-1.0f,0.0f,  0.0f,1.0f,
    0.5f,-0.5f,-0.5f,  0.0f,-1.0f,0.0f,  1.0f,1.0f,
    0.5f,-0.5f, 0.5f,  0.0f,-1.0f,0.0f,  1.0f,0.0f,
    0.5f,-0.5f, 0.5f,  0.0f,-1.0f,0.0f,  1.0f,0.0f,
   -0.5f,-0.5f, 0.5f,  0.0f,-1.0f,0.0f,  0.0f,0.0f,
   -0.5f,-0.5f,-0.5f,  0.0f,-1.0f,0.0f,  0.0f,1.0f,

   -0.5f, 0.5f,-0.5f,  0.0f,1.0f,0.0f,   0.0f,1.0f, 
    0.5f, 0.5f,-0.5f,  0.0f,1.0f,0.0f,   1.0f,1.0f,
    0.5f, 0.5f, 0.5f,  0.0f,1.0f,0.0f,   1.0f,0.0f,
    0.5f, 0.5f, 0.5f,  0.0f,1.0f,0.0f,   1.0f,0.0f,
   -0.5f, 0.5f, 0.5f,  0.0f,1.0f,0.0f,   0.0f,0.0f,
   -0.5f, 0.5f,-0.5f,  0.0f,1.0f,0.0f,   0.0f,1.0f
    };

    unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3 };

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    
    framebuffer_size_callback(window,800,600);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1,&VAO); 
    glGenBuffers(1, &EBO); 
          
    
    glBindVertexArray(VAO);
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);     
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);    

glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
 
        
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture coordinate attribute
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    	glEnableVertexAttribArray(2);
  	
    
    
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "layout (location = 2) in vec2 aTexCoord;\n"
        "out vec3 ourColor;\n"
        "out vec2 TexCoord;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        
        "void main() {\n"
        "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "   ourColor = aColor;\n"
        "   TexCoord = aTexCoord;\n"
        "}\n";

    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 ourColor;\n"
        "in vec2 TexCoord;\n"
        "uniform sampler2D texture1;\n"
        "uniform sampler2D texture2;\n"
        "void main() {\n"
        "   FragColor = mix(texture(texture1, TexCoord),texture(texture2,TexCoord),0.2);\n"
        "}\n";

  
        vertexShader = glCreateShader(GL_VERTEX_SHADER);    

        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

      
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if(!success){
 
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog <<std::endl;
        
        }else{std::cout<<"SUCCESS::SHADER::VERTEX::COMPILATION_SUCCEEDED\n"<<infoLog<<std::endl;}
        

       
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        
        glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
        
        if(!success){
        	glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
        	std::cout<<"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"<< infoLog <<std::endl;
        }else{std::cout<<"SUCCESS::SHADER::FRAGMENT::COMPILATION_SUCCEEDED\n"<<infoLog<<std::endl;}
        
            
        shaderProgram = glCreateProgram();    
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);    

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::LINKING::SHADER::COMPILATION_FAILED\n" << infoLog <<std::endl;
         
         }else{std::cout<<"SUCCESS::LINKING::SHADER::COMPILATION_SUCCEEDED\n"<<infoLog<<std::endl;}   
         
         
           // Load and create a texture
   
   
    	unsigned int texture,texture2;
    	glGenTextures(1, &texture);
    	glGenTextures(1,&texture2);
   
   	stbi_set_flip_vertically_on_load(true);  
    //first texture 
    
    	glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, texture);
    
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    	
    
    	int width, height, nrchannels;
    	unsigned char* data = stbi_load("container2.png", &width, &height, &nrchannels, 0);

	if(data){
     
        	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        	glGenerateMipmap(GL_TEXTURE_2D);
  		
  		std::cout<<"SUCCESS::STBI_LOAD::TEXTURE\n"<<infoLog <<std::endl;
 	
     		
    	}else{std::cout<<"ERROR::STBI_LOAD::TEXTURE\n"<<infoLog<<std::endl;}
        
    	//second texture
    	glActiveTexture(GL_TEXTURE1);
    	glBindTexture(GL_TEXTURE_2D,texture2);
   
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
              
       data = stbi_load("tm3n3p02.png", &width, &height, &nrchannels, 0);

	if(data){
        	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        	glGenerateMipmap(GL_TEXTURE_2D);
  	 
 	std::cout<<"SUCCESS::STBI_LOAD::TEXTURE\n"<<infoLog<<std::endl;
     		
    	}else{std::cout<<"ERROR::STBI_LOAD::TEXTURE\n"<<infoLog<<std::endl;}

	stbi_image_free(data);
	
	
	
	glm::mat4 view = glm::mat4(1.0f);
	//translating the scene in the reverse direction
	view = glm::translate(view,glm::vec3(0.0f,0.0f,-3.0f));
	
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f),800.0f / 600.0f,0.1f,100.0f);
	
	glEnable(GL_DEPTH_TEST);
	
    	while(!glfwWindowShouldClose(window)){

        	processInput(window);

        	//rendering commands
        	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glUniform1i(glGetUniformLocation(shaderProgram, "texture"), 0);
		glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 0);
		
		glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model,(float)glfwGetTime() * glm::radians(50.0f),glm::vec3(0.5f, 1.0f, 0.0f));
		int modelLoc = glGetUniformLocation(shaderProgram,"model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE,glm::value_ptr(model));
		
		int viewLoc = glGetUniformLocation(shaderProgram,"view");
		glUniformMatrix4fv(viewLoc,1,GL_FALSE,glm::value_ptr(view));
			
	int projectionLoc = glGetUniformLocation(shaderProgram,"projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
        	glUseProgram(shaderProgram);
        		
        	glBindVertexArray(VAO);
        	
	
        	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        	glDrawArrays(GL_TRIANGLES,0, 36);
        	glBindVertexArray(0); 
		
        	glfwSwapBuffers(window);
        	glfwPollEvents();
        	
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture);
    glDeleteTextures(1,&texture2);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}
