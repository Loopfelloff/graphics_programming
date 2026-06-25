#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// this is saying the input is coming through slot 0 , and each input is two floats
// aPos (x,y) we are 2D so depth is 0.0 , 1.0 it's for math projections of some sorts.
const char* vertexShaderSource = R"( 
#version 330 core
layout (location = 0) in vec2 aPos;
void main(){
    gl_Position = vec4(aPos , 0.0 , 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main(){
    FragColor = vec4(1.0 , 0.4, 0.1, 1.0);
}
)";

// compilerShader creates an empty slot in the GPU and and returns an ID. OpenGL uses ID not pointers for GPU realted stuff.

unsigned int compileShader(unsigned int type , const char* src){
    unsigned int id = glCreateShader(type);
    glShaderSource(id , 1 , &src, nullptr); // 1 means we are passing only one string 
    glCompileShader(id); // compiles the glsl into the shader source into the GPU
    int ok;
    glGetShaderiv(id , GL_COMPILE_STATUS , &ok);
    if (!ok) {char log[512];
	glGetShaderInfoLog(id , 512 , nullptr , log);
	std::cerr << log;
    }
    return id;
}

unsigned int createProgram() {
    // program is the vertex shader and program shader compiled to one pipeline.
    unsigned int vs = compileShader(GL_VERTEX_SHADER,   vertexShaderSource);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    unsigned int prog = glCreateProgram();
    glAttachShader(prog, vs); glAttachShader(prog, fs);
    glLinkProgram(prog); // once both the vertex shader and fragment shaders are linked delete the indiviual shaders because we combined both into one 
    glDeleteShader(vs); glDeleteShader(fs);
    return prog;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1920 , 1080, "My Rectangle", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0,0,1920 , 1080);

    float vertices[] = {
	-0.5f, -0.5f,  // bottom left
         0.5f, -0.5f,  // bottom right
         0.5f,  0.5f,  // top right

        -0.5f, -0.5f,  // bottom left
         0.5f,  0.5f,  // top right
        -0.5f,  0.5f,  // top left
    };

    // Create VAO and VBO
    unsigned int VAO, VBO; //vertex buffer object and vertex array object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO first, then VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send the vertex data to the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Tell OpenGL how to read the data
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int prog = createProgram();

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(prog);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);  

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(prog);
    glfwTerminate();


    return 0;
}
