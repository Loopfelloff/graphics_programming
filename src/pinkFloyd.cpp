// We will make a dark side of the moon album cover
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include<cmath>

const char* vertexShaderSource = R"( 
#version 330 core
layout (location = 0) in vec2 aPos;
uniform vec2 offset;
uniform float scale;
void main(){
    gl_Position = vec4(aPos * scale + offset , 0.0 , 1.0);
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

float return_edge_x(float slope_edge , float y_val , float x_init , float y_init){
    return (y_val - y_init) / slope_edge + x_init; 
}

float return_edge_y(float slope_edge , float x_val , float x_init , float y_init){
    return slope_edge * (x_val - x_init) + y_init;
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

    float vertices_triangle[] = {
	-0.5f, -0.5f,
         0.5f, -0.5f,
         0.0f,  0.5f,
    };
    float vertices_left_line[] = {

	-1.0f,                                  0.05f,
        -1.0f,                                  0.00f,
        return_edge_x(2, 0.25f, -0.5f, -0.5f), 0.25f,

        -1.0f,                                  0.00f,
        return_edge_x(2, 0.25f, -0.5f, -0.5f), 0.25f,
        return_edge_x(2, 0.20f, -0.5f, -0.5f), 0.20f,
    };
    float vertices_color_line[] = {

	  1.0f,                               0.30f,
         1.0f,                               0.25f,
        return_edge_x(-2, 0.30f, 0.0f, 0.5f), 0.30f,

         1.0f,                               0.25f,
        return_edge_x(-2, 0.30f, 0.0f, 0.5f), 0.30f,
        return_edge_x(-2, 0.25f, 0.0f, 0.5f), 0.25f,
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle), vertices_triangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int VAO_left, VBO_left;
    glGenVertexArrays(1, &VAO_left);
    glGenBuffers(1, &VBO_left);

    glBindVertexArray(VAO_left);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_left);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_left_line), vertices_left_line, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int VAO_right, VBO_right;
    glGenVertexArrays(1, &VAO_right);
    glGenBuffers(1, &VBO_right);

    glBindVertexArray(VAO_right);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_right);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_color_line), vertices_color_line, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    unsigned int prog = createProgram();

    float red = 0.0f;
    float green = 0.0f;
    float blue = 0.0f; 

    float x_offset = 0.0f;
    float y_offset = 0.0f;

    float scale_factor = 1.0f;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(prog);
	
        glBindVertexArray(VAO_right);
	// violet	
	red = 0.5f;
	green = 0.0f;
	blue = 1.0f;
	scale_factor = 1.0f;
	int colorLocation = glGetUniformLocation(prog, "uColor");
	int offsetLocation = glGetUniformLocation(prog, "offset");
	int scale = glGetUniformLocation(prog, "scale");
	glUniform1f(scale, scale_factor);
	glUniform4f(colorLocation, red, green, blue, 1.0f);
	glUniform2f(offsetLocation, x_offset, y_offset);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// blue
	red = 0.0f;
	green = 0.0f;
	blue = 1.0f;
	scale_factor = 1.0f;
	x_offset = 0.0f;
	y_offset = -0.05f;
	colorLocation = glGetUniformLocation(prog, "uColor");
	offsetLocation = glGetUniformLocation(prog, "offset");
	scale = glGetUniformLocation(prog, "scale");
	glUniform1f(scale, scale_factor);
	glUniform4f(colorLocation, red, green, blue, 1.0f);
	glUniform2f(offsetLocation, x_offset, y_offset);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// green
	red = 0.0f;
	green = 1.0f;
	blue = 0.0f;
	scale_factor = 1.0f;
	x_offset = 0.0f;
	y_offset = -0.1f;
	colorLocation = glGetUniformLocation(prog, "uColor");
	offsetLocation = glGetUniformLocation(prog, "offset");
	scale = glGetUniformLocation(prog, "scale");
	glUniform1f(scale, scale_factor);
	glUniform4f(colorLocation, red, green, blue, 1.0f);
	glUniform2f(offsetLocation, x_offset, y_offset);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// yellow
	red = 1.0f;
	green = 1.0f;
	blue = 0.0f;
	scale_factor = 1.0f;
	x_offset = 0.0f;
	y_offset = -0.15f;
	colorLocation = glGetUniformLocation(prog, "uColor");
	offsetLocation = glGetUniformLocation(prog, "offset");
	scale = glGetUniformLocation(prog, "scale");
	glUniform1f(scale, scale_factor);
	glUniform4f(colorLocation, red, green, blue, 1.0f);
	glUniform2f(offsetLocation, x_offset, y_offset);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// orange
	red = 1.0f;
	green = 0.5f;
	blue = 0.0f;
	scale_factor = 1.0f;
	x_offset = 0.0f;
	y_offset = -0.2f;
	colorLocation = glGetUniformLocation(prog, "uColor");
	offsetLocation = glGetUniformLocation(prog, "offset");
	scale = glGetUniformLocation(prog, "scale");
	glUniform1f(scale, scale_factor);
	glUniform4f(colorLocation, red, green, blue, 1.0f);
	glUniform2f(offsetLocation, x_offset, y_offset);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// red
	red = 1.0f;
	green = 0.0f;
	blue = 0.0f;
	scale_factor = 1.0f;
	x_offset = 0.0f;
	y_offset = -0.25f;
	colorLocation = glGetUniformLocation(prog, "uColor");
	offsetLocation = glGetUniformLocation(prog, "offset");
	scale = glGetUniformLocation(prog, "scale");
	glUniform1f(scale, scale_factor);
	glUniform4f(colorLocation, red, green, blue, 1.0f);
	glUniform2f(offsetLocation, x_offset, y_offset);
	glDrawArrays(GL_TRIANGLES, 0, 6);


        glBindVertexArray(VAO);
	
	red = 1.0f;
	green = 1.0f;
	blue = 1.0f;
	scale_factor = 1.0f;
	x_offset = 0.0f;
	y_offset = 0.0f;
	colorLocation = glGetUniformLocation(prog, "uColor");
	offsetLocation = glGetUniformLocation(prog, "offset");
	scale = glGetUniformLocation(prog, "scale");
	glUniform1f(scale, scale_factor);
	glUniform4f(colorLocation, red, green, blue, 1.0f);
	glUniform2f(offsetLocation, x_offset, y_offset);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	red = 0.1f;
	green = 0.1f;
	blue = 0.15f;
	scale_factor = 0.9f;
	colorLocation = glGetUniformLocation(prog, "uColor");
	offsetLocation = glGetUniformLocation(prog, "offset");
	glUniform4f(colorLocation , red , green , blue , 1.0f);
	glUniform2f(offsetLocation ,x_offset , y_offset );
	glUniform1f(scale , scale_factor);
        glDrawArrays(GL_TRIANGLES, 0, 3);  

        glBindVertexArray(VAO_left);
	red = 1.0f;
	green = 1.0f;
	blue = 1.0f;
	scale_factor = 1.0f;
	colorLocation = glGetUniformLocation(prog, "uColor");
	offsetLocation = glGetUniformLocation(prog, "offset");
	glUniform4f(colorLocation , red , green , blue , 1.0f);
	glUniform2f(offsetLocation ,x_offset , y_offset );
	glUniform1f(scale , scale_factor);
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
