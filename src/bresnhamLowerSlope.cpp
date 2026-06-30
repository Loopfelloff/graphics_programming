//
// this contains the dda line drawing algorithm written here
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include<cmath>
#include<vector>

// this is saying the input is coming through slot 0 , and each input is two floats
// aPos (x,y) we are 2D so depth is 0.0 , 1.0 it's for math projections of some sorts.
const char* vertexShaderSource = R"( 
#version 330 core
layout (location = 0) in vec2 aPos;
uniform vec2 offset;
void main(){
    gl_Position = vec4(aPos + offset , 0.0 , 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec4 uColor;
void main(){
    FragColor = uColor;
}
)";

int return_something(std::vector<int>& vertices , int start_x , int finish_x , int start_y , int finish_y){
    
    int dx = finish_x - start_x;
    int dy = finish_y - start_y;
    int decision_param = 2*dy - dx;
    int x_init = start_x;
    int y_init = start_y;
    int i =0;
    while(x_init <= finish_x){
	i++;
	vertices.push_back(x_init);
	vertices.push_back(y_init);
	if (decision_param  < 0){
	    x_init += 1;
	    decision_param  = decision_param + 2 * dy;
	}
	else {
	    x_init += 1;
	    y_init += 1;
	    decision_param = decision_param + 2*dy - 2*dx;
	}
    }
    return i;
}

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
    int width, height;
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0,0,1920 , 1080);
    std::vector<int> vertices;
    int steps = return_something(vertices , 0,1920, 0 , 1080);
    std::vector<float> norm_vertices;
    for(int i=0 ; i< vertices.size(); i++){
	float val_to_push;
	if(i % 2 == 0){
	val_to_push = float(vertices[i])/1920.0f * 2 -1 ;
	}
	else{
	val_to_push = float(vertices[i])/1080.0f * 2 -1 ;
	}
	norm_vertices.push_back(val_to_push);
    }
    // Create VAO and VBO
    unsigned int VAO, VBO; //vertex buffer object and vertex array object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO first, then VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send the vertex data to the GPU
    glBufferData(GL_ARRAY_BUFFER, norm_vertices.size()*sizeof(float), &norm_vertices[0], GL_STATIC_DRAW);

    // Tell OpenGL how to read the data
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int prog = createProgram();

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


	float time = glfwGetTime();
	float red   = (sin(time * 1.0f) + 1.0f) / 2.0f;
	float green = (sin(time * 0.5f) + 1.0f) / 2.0f;
	float blue  = (sin(time * 2.0f) + 1.0f) / 2.0f;

        glUseProgram(prog);
        glBindVertexArray(VAO);
	int colorLocation = glGetUniformLocation(prog, "uColor");
	glUniform4f(colorLocation , red , green , blue , 1.0f);
	glDrawArrays(GL_POINTS, 0, steps);
	int offsetLocation = glGetUniformLocation(prog, "offset");

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(prog);
    glfwTerminate();


    return 0;
}


