#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "stb_image.h"

// -------------------- Shader Class --------------------
class Shader {
public:
    unsigned int ID;
    Shader(const char* vertexSrc, const char* fragmentSrc) {
        // Compile vertex shader
        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexSrc, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        // Compile fragment shader
        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentSrc, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        // Link shaders
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    void use() { glUseProgram(ID); }
    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID.c_str(), name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
    void setVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(ID.c_str(), name.c_str()), 1, &value[0]);
    }
    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID.c_str(), name.c_str()), value);
    }
private:
    void checkCompileErrors(unsigned int shader, std::string type){
        int success;
        char infoLog[1024];
        if(type != "PROGRAM"){
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success){
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success){
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
            }
        }
    }
};

// -------------------- Camera Class --------------------
class Camera {
public:
    glm::vec3 Position, Front, Up;
    float Yaw, Pitch;
    float Speed = 2.5f;
    float Sensitivity = 0.1f;

    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Position(position), Up(up), Yaw(yaw), Pitch(pitch)
    { updateCameraVectors(); }

    glm::mat4 getViewMatrix() { return glm::lookAt(Position, Position + Front, Up); }

    void processKeyboard(int key, float deltaTime){
        float velocity = Speed * deltaTime;
        if(key == GLFW_KEY_W) Position += Front * velocity;
        if(key == GLFW_KEY_S) Position -= Front * velocity;
        if(key == GLFW_KEY_A) Position -= glm::normalize(glm::cross(Front, Up)) * velocity;
        if(key == GLFW_KEY_D) Position += glm::normalize(glm::cross(Front, Up)) * velocity;
    }

    void processMouseMovement(float xoffset, float yoffset){
        xoffset *= Sensitivity;
        yoffset *= Sensitivity;
        Yaw += xoffset;
        Pitch += yoffset;
        if(Pitch > 89.0f) Pitch = 89.0f;
        if(Pitch < -89.0f) Pitch = -89.0f;
        updateCameraVectors();
    }

private:
    void updateCameraVectors(){
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
    }
};

// -------------------- Mesh Class --------------------
class Mesh {
public:
    unsigned int VAO, VBO;
    std::vector<float> vertices;

    Mesh(const std::vector<float>& verts) : vertices(verts) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);
    }

    void Draw() { glBindVertexArray(VAO); glDrawArrays(GL_TRIANGLES, 0, vertices.size()/8); glBindVertexArray(0); }
};

// -------------------- Light Class --------------------
struct Light {
    glm::vec3 Position;
    glm::vec3 Color;
};

// -------------------- Material Struct --------------------
struct Material {
    glm::vec3 Color;
    float SpecularStrength;
    float Shininess;
};

// -------------------- Cube Vertices --------------------
std::vector<float> cubeVertices = {
    // positions         // normals          // texcoords
   -0.5f,-0.5f,-0.5f,   0.0f,0.0f,-1.0f,   0.0f,0.0f,
    0.5f,-0.5f,-0.5f,   0.0f,0.0f,-1.0f,   1.0f,0.0f,
    0.5f, 0.5f,-0.5f,   0.0f,0.0f,-1.0f,   1.0f,1.0f,
    0.5f, 0.5f,-0.5f,   0.0f,0.0f,-1.0f,   1.0f,1.0f,
   -0.5f, 0.5f,-0.5f,   0.0f,0.0f,-1.0f,   0.0f,1.0f,
   -0.5f,-0.5f,-0.5f,   0.0f,0.0f,-1.0f,   0.0f,0.0f,
   // ... (fill the rest of cube faces similarly)
};

// -------------------- Main --------------------
int main() {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800,600,"MiniEngine",NULL,NULL);
    if(!window){ std::cout<<"Failed to create window"<<std::endl; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);

    // Shaders
    const char* vertexShaderSource = R"GLSL(
        #version 330 core
        layout (location=0) in vec3 aPos;
        layout (location=1) in vec3 aNormal;
        layout (location=2) in vec2 aTexCoord;
        out vec3 FragPos;
        out vec3 Normal;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        void main(){ FragPos = vec3(model*vec4(aPos,1.0)); Normal = mat3(transpose(inverse(model)))*aNormal; gl_Position = projection*view*vec4(aPos,1.0); }
    )GLSL";

    const char* fragmentShaderSource = R"GLSL(
        #version 330 core
        out vec4 FragColor;
        in vec3 FragPos;
        in vec3 Normal;
        uniform vec3 objectColor;
        uniform vec3 lightColor;
        uniform vec3 lightPos;
        uniform vec3 viewPos;
        void main(){
            float ambientStrength=0.1;
            vec3 ambient = ambientStrength * lightColor;
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir),0.0);
            vec3 diffuse = diff * lightColor;
            float specularStrength=0.5;
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir,norm);
            float spec = pow(max(dot(viewDir,reflectDir),0.0),32.0);
            vec3 specular = specularStrength * spec * lightColor;
            vec3 result = (ambient+diffuse+specular)*objectColor;
            FragColor = vec4(result,1.0);
        }
    )GLSL";

    Shader shader(vertexShaderSource, fragmentShaderSource);
    Mesh cube(cubeVertices);

    Camera camera(glm::vec3(0,0,5), glm::vec3(0,1,0), -90.0f,0.0f);
    Light light{ glm::vec3(1.2f,1.0f,2.0f), glm::vec3(1.0f,1.0f,1.0f) };
    Material mat{ glm::vec3(1.0f,0.5f,0.31f), 0.5f, 32.0f };

    float lastFrame = 0.0f;

    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS) camera.processKeyboard(GLFW_KEY_W,deltaTime);
        if(glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS) camera.processKeyboard(GLFW_KEY_S,deltaTime);
        if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS) camera.processKeyboard(GLFW_KEY_A,deltaTime);
        if(glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS) camera.processKeyboard(GLFW_KEY_D,deltaTime);

        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Transformations
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(20.0f*(float)glfwGetTime()), glm::vec3(1.0f,0.3f,0.5f));
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.1f,100.0f);

        // Draw cube
        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("objectColor", mat.Color);
        shader.setVec3("lightColor", light.Color);
        shader.setVec3("lightPos", light.Position);
        shader.setVec3("viewPos", camera.Position);
        cube.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

