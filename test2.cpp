#include "classes/Shader.h"
#include "classes/Mesh.h"
#include "classes/Camera.h"
#include "classes/stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

// Orbit Camera
OrbitCamera camera(glm::vec3(0.0f), 5.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.processMouseMovement(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.processMouseScroll(yoffset);
}

// Simple texture loader
unsigned int loadTexture(const char* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 4) format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// Cube vertices: positions, normals, texcoords
std::vector<float> cubeVertices = {
    // positions          // normals       // texcoords
   -0.5f,-0.5f,-0.5f,    0.0f,0.0f,-1.0f, 0.0f,0.0f,
    0.5f,-0.5f,-0.5f,    0.0f,0.0f,-1.0f, 1.0f,0.0f,
    0.5f, 0.5f,-0.5f,    0.0f,0.0f,-1.0f, 1.0f,1.0f,
    0.5f, 0.5f,-0.5f,    0.0f,0.0f,-1.0f, 1.0f,1.0f,
   -0.5f, 0.5f,-0.5f,    0.0f,0.0f,-1.0f, 0.0f,1.0f,
   -0.5f,-0.5f,-0.5f,    0.0f,0.0f,-1.0f, 0.0f,0.0f,

   -0.5f,-0.5f, 0.5f,    0.0f,0.0f,1.0f,  0.0f,0.0f,
    0.5f,-0.5f, 0.5f,    0.0f,0.0f,1.0f,  1.0f,0.0f,
    0.5f, 0.5f, 0.5f,    0.0f,0.0f,1.0f,  1.0f,1.0f,
    0.5f, 0.5f, 0.5f,    0.0f,0.0f,1.0f,  1.0f,1.0f,
   -0.5f, 0.5f, 0.5f,    0.0f,0.0f,1.0f,  0.0f,1.0f,
   -0.5f,-0.5f, 0.5f,    0.0f,0.0f,1.0f,  0.0f,0.0f,

   -0.5f, 0.5f, 0.5f,   -1.0f,0.0f,0.0f,  1.0f,0.0f,
   -0.5f, 0.5f,-0.5f,   -1.0f,0.0f,0.0f,  1.0f,1.0f,
   -0.5f,-0.5f,-0.5f,   -1.0f,0.0f,0.0f,  0.0f,1.0f,
   -0.5f,-0.5f,-0.5f,   -1.0f,0.0f,0.0f,  0.0f,1.0f,
   -0.5f,-0.5f, 0.5f,   -1.0f,0.0f,0.0f,  0.0f,0.0f,
   -0.5f, 0.5f, 0.5f,   -1.0f,0.0f,0.0f,  1.0f,0.0f,

    0.5f, 0.5f, 0.5f,    1.0f,0.0f,0.0f,  1.0f,0.0f,
    0.5f, 0.5f,-0.5f,    1.0f,0.0f,0.0f,  1.0f,1.0f,
    0.5f,-0.5f,-0.5f,    1.0f,0.0f,0.0f,  0.0f,1.0f,
    0.5f,-0.5f,-0.5f,    1.0f,0.0f,0.0f,  0.0f,1.0f,
    0.5f,-0.5f, 0.5f,    1.0f,0.0f,0.0f,  0.0f,0.0f,
    0.5f, 0.5f, 0.5f,    1.0f,0.0f,0.0f,  1.0f,0.0f,

   -0.5f,-0.5f,-0.5f,    0.0f,-1.0f,0.0f, 0.0f,1.0f,
    0.5f,-0.5f,-0.5f,    0.0f,-1.0f,0.0f, 1.0f,1.0f,
    0.5f,-0.5f, 0.5f,    0.0f,-1.0f,0.0f, 1.0f,0.0f,
    0.5f,-0.5f, 0.5f,    0.0f,-1.0f,0.0f, 1.0f,0.0f,
   -0.5f,-0.5f, 0.5f,    0.0f,-1.0f,0.0f, 0.0f,0.0f,
   -0.5f,-0.5f,-0.5f,    0.0f,-1.0f,0.0f, 0.0f,1.0f,

   -0.5f, 0.5f,-0.5f,    0.0f,1.0f,0.0f,  0.0f,1.0f,
    0.5f, 0.5f,-0.5f,    0.0f,1.0f,0.0f,  1.0f,1.0f,
    0.5f, 0.5f, 0.5f,    0.0f,1.0f,0.0f,  1.0f,0.0f,
    0.5f, 0.5f, 0.5f,    0.0f,1.0f,0.0f,  1.0f,0.0f,
   -0.5f, 0.5f, 0.5f,    0.0f,1.0f,0.0f,  0.0f,0.0f,
   -0.5f, 0.5f,-0.5f,    0.0f,1.0f,0.0f,  0.0f,1.0f
};

// Vertex and fragment shader sources
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
        FragPos = vec3(model * vec4(aPos,1.0));
        Normal = mat3(transpose(inverse(model))) * aNormal;
        TexCoord = aTexCoord;
        gl_Position = projection * view * vec4(FragPos,1.0);
    }
)GLSL";

const char* fragmentShaderSource = R"GLSL(
    #version 330 core
    out vec4 FragColor;
    in vec3 FragPos;
    in vec3 Normal;
    in vec2 TexCoord;
    uniform vec3 lightColor;
    uniform vec3 lightPos;
    uniform vec3 viewPos;
    uniform sampler2D ourTexture;
    void main(){
        float ambientStrength = 0.3;
        vec3 ambient = ambientStrength * lightColor;
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;
        vec3 phong = ambient + diffuse + specular;
        vec3 texColor = texture(ourTexture, TexCoord).rgb;
        FragColor = vec4(texColor * phong, 1.0);
    }
)GLSL";

const char* lightVertexShaderSource = R"GLSL(
    #version 330 core
    layout (location=0) in vec3 aPos;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main(){
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)GLSL";

const char* lightFragmentShaderSource = R"GLSL(
    #version 330 core
    out vec4 FragColor;
    uniform vec3 lightColor;
    void main(){
        FragColor = vec4(lightColor, 1.0);
    }
)GLSL";

int main() {
    // GLFW init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800,600,"MiniEngine",NULL,NULL);
    if(!window){ std::cout<<"Failed to create window\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<<"Failed to init GLAD\n"; return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    Shader shader(vertexShaderSource, fragmentShaderSource);
    Shader lightShader(lightVertexShaderSource, lightFragmentShaderSource);

    Mesh cube(cubeVertices);
    cube.texture = loadTexture("container2.png");

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    float lastFrame = 0.0f;

    while(!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window,true);

        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);

        // Main cube
        shader.use();
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(50.0f) * currentFrame,
                                      glm::vec3(0.5f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("lightColor", glm::vec3(1.0f));
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("viewPos", camera.getPosition());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cube.texture);
        shader.setInt("ourTexture", 0);
        cube.Draw();

        // Light cube orbiting
        lightShader.use();
        float radius = 2.0f;
        float lightX = sin(currentFrame) * radius;
        float lightZ = cos(currentFrame) * radius;
        glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), glm::vec3(lightX, 1.0f, lightZ));
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        lightShader.setMat4("model", lightModel);
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);
        lightShader.setVec3("lightColor", glm::vec3(1.0f));
        cube.Draw(); // light uses same mesh

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

