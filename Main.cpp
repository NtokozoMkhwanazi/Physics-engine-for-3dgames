#include"classes/Shader.h"
#include"classes/Mesh.h"
#include"classes/Camera.h"
#include"classes/stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

// globals for camera + mouse
Camera camera(glm::vec3(0,0,5), glm::vec3(0,1,0), -90.0f, 0.0f);
float lastX = 400.0f, lastY = 300.0f;
bool firstMouse = true;

struct Light {
    glm::vec3 Position;
    glm::vec3 Color;
};

struct Material {
    glm::vec3 Color;
    float SpecularStrength;
    float Shininess;
};

// framebuffer resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

// mouse movement
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if(firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed
    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(window,xoffset, yoffset); // <-- use Camera function
}

// cube vertices
std::vector<float> cubeVertices = {
   // positions        //normals         // texcoords
   -0.5f,-0.5f,-0.5f,    0.0f,0.0f,-1.0f,  0.0f,0.0f,
    0.5f,-0.5f,-0.5f,    0.0f,0.0f,-1.0f,  1.0f,0.0f,
    0.5f, 0.5f,-0.5f,    0.0f,0.0f,-1.0f,  1.0f,1.0f,
    0.5f, 0.5f,-0.5f,    0.0f,0.0f,-1.0f,  1.0f,1.0f,
   -0.5f, 0.5f,-0.5f,    0.0f,0.0f,-1.0f,  0.0f,1.0f,
   -0.5f,-0.5f,-0.5f,    0.0f,0.0f,-1.0f,  0.0f,0.0f,
   
   -0.5f,-0.5f, 0.5f,    0.0f,0.0f,1.0f,   0.0f,0.0f,
     0.5f,-0.5f, 0.5f,   0.0f,0.0f,1.0f,   1.0f,0.0f,
     0.5f, 0.5f, 0.5f,   0.0f,0.0f,1.0f,   1.0f,1.0f,
     0.5f, 0.5f, 0.5f,   0.0f,0.0f,1.0f,   1.0f,1.0f,
    -0.5f, 0.5f, 0.5f,   0.0f,0.0f,1.0f,   0.0f,1.0f,
    -0.5f,-0.5f, 0.5f,   0.0f,0.0f,1.0f,   0.0f,0.0f,

    -0.5f, 0.5f, 0.5f,  -1.0f,0.0f,0.0f,   1.0f,0.0f,
    -0.5f, 0.5f,-0.5f,  -1.0f,0.0f,0.0f,   1.0f,1.0f,
    -0.5f,-0.5f,-0.5f,  -1.0f,0.0f,0.0f,   0.0f,1.0f,
    -0.5f,-0.5f,-0.5f,  -1.0f,0.0f,0.0f,   0.0f,1.0f,
    -0.5f,-0.5f, 0.5f,  -1.0f,0.0f,0.0f,   0.0f,0.0f,
    -0.5f, 0.5f, 0.5f,  -1.0f,0.0f,0.0f,   1.0f,0.0f,

     0.5f, 0.5f, 0.5f,   1.0f,0.0f,0.0f,   1.0f,0.0f,
     0.5f, 0.5f,-0.5f,   1.0f,0.0f,0.0f,   1.0f,1.0f,
     0.5f,-0.5f,-0.5f,   1.0f,0.0f,0.0f,   0.0f,1.0f,
     0.5f,-0.5f,-0.5f,   1.0f,0.0f,0.0f,   0.0f,1.0f,
     0.5f,-0.5f, 0.5f,   1.0f,0.0f,0.0f,   1.0f,0.0f,
     0.5f, 0.5f, 0.5f,   1.0f,0.0f,0.0f,   0.0f,1.0f,

    -0.5f,-0.5f,-0.5f,   0.0f,-1.0f,0.0f,  0.0f,1.0f,
     0.5f,-0.5f,-0.5f,   0.0f,-1.0f,0.0f,  1.0f,1.0f,
     0.5f,-0.5f, 0.5f,   0.0f,-1.0f,0.0f,  1.0f,0.0f,
     0.5f,-0.5f, 0.5f,   0.0f,-1.0f,0.0f,  1.0f,0.0f,
    -0.5f,-0.5f, 0.5f,   0.0f,-1.0f,0.0f,  0.0f,0.0f,
    -0.5f,-0.5f,-0.5f,   0.0f,-1.0f,0.0f,  0.0f,1.0f,

    -0.5f, 0.5f,-0.5f,   0.0f,1.0f,0.0f,   0.0f,1.0f, 
     0.5f, 0.5f,-0.5f,   0.0f,1.0f,0.0f,   1.0f,1.0f,
     0.5f, 0.5f, 0.5f,   0.0f,1.0f,0.0f,   1.0f,0.0f,
     0.5f, 0.5f, 0.5f,   0.0f,1.0f,0.0f,   1.0f,0.0f,
    -0.5f, 0.5f, 0.5f,   0.0f,1.0f,0.0f,   0.0f,0.0f,
    -0.5f, 0.5f,-0.5f,   0.0f,1.0f,0.0f,    0.0f,1.0f
};

int main() {
    // init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800,600,"MiniEngine",NULL,NULL);
    if(!window){ std::cout<<"Failed to create window"<<std::endl; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<<"Failed to init GLAD\n"; return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    // Shaders
    const char* vertexShaderSource = R"GLSL(
        #version 330 core
        layout (location=0) in vec3 aPos;
        layout (location=1) in vec3 aNormal;
        layout (location=2) in vec2 aTexCoord;

        out vec3 FragPos;
        out vec3 Normal;
        out vec2 TexCoord;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main(){
            FragPos = vec3(model*vec4(aPos,1.0));
            Normal = mat3(transpose(inverse(model)))*aNormal;
            gl_Position = projection*view*vec4(aPos,1.0);
            TexCoord = aTexCoord;
        }
    )GLSL";

    const char* fragmentShaderSource = R"GLSL(
        #version 330 core
        out vec4 FragColor;
        in vec3 FragPos;
        in vec3 Normal;
        in vec2 TexCoord;
        
        uniform sampler2D texture;
        uniform vec3 objectColor;
        uniform vec3 lightColor;
        uniform vec3 lightPos;
        uniform vec3 viewPos;
        void main(){
            float ambientStrength = 0.1;
            vec3 ambient = ambientStrength*lightColor;

            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos-FragPos);
            float diff = max(dot(norm,lightDir),0.0);
            vec3 diffuse = diff*lightColor;

            float specularStrength = 0.5;
            vec3 viewDir = normalize(viewPos-FragPos);
            vec3 reflectDir = reflect(-lightDir,norm);
            float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
            vec3 specular = specularStrength*spec*lightColor;

            vec3 result = (ambient+diffuse+specular)*objectColor;
            FragColor = texture(texture,TexCoord)* vec4(result,1.0);
        }
    )GLSL";


    //initialize the shader
    Shader shader(vertexShaderSource, fragmentShaderSource);
 
    //initialize the buffers
    Mesh cube(cubeVertices);

    //Load the texture image
    
    unsigned int texture;
    int width, height, nrChannels;

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
   // set the texture wrapping/filtering options (on currently bound texture)
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   
   unsigned char* data = stbi_load("wood.png", &width, &height, &nrChannels, 0);
 
    if (data){
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
	GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
		
   }else{std::cout << "Failed to load texture" << std::endl;}
    
    stbi_image_free(data);
    
    //initialize the lighting and the material
    Light light{ glm::vec3(1.2f,1.0f,2.0f), glm::vec3(1.0f,1.0f,1.0f) };
    Material mat{ glm::vec3(1.0f,0.5f,0.31f), 0.5f, 32.0f };

    float lastFrame=0.0f;
    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame-lastFrame;
        lastFrame = currentFrame;

        // input
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window,true);
        if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS) camera.processKeyboard(GLFW_KEY_W,deltaTime);
        if(glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS) camera.processKeyboard(GLFW_KEY_S,deltaTime);
        if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS) camera.processKeyboard(GLFW_KEY_A,deltaTime);
        if(glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS) camera.processKeyboard(GLFW_KEY_D,deltaTime);

        // render
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::rotate(glm::mat4(1.0f),
                                      glm::radians(20.0f*(float)glfwGetTime()),
                                      glm::vec3(1.0f,0.3f,0.5f));
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                               800.0f/600.0f,0.1f,100.0f);
        glUniform1i(glGetUniformLocation(shader.programID, "texture"), 0);

        shader.use();
        shader.setMat4("model",model);
        shader.setMat4("view",view);
        shader.setMat4("projection",projection);
        shader.setVec3("objectColor",mat.Color);
        shader.setVec3("lightColor",light.Color);
        shader.setVec3("lightPos",light.Position);
        shader.setVec3("viewPos",camera.Position);
        cube.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    cube.cleanUp(texture,shader.programID);
    
    glfwTerminate();
    return 0;
}

