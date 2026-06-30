#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
uniform vec2 offset;
void main(){
    gl_Position = vec4(aPos + offset, 0.0, 1.0);
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
int drawLineDDA(std::vector<int>& vertices, int x0, int y0, int x1, int y1) {
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;   // step direction on x
    int sy = (y0 < y1) ? 1 : -1;   // step direction on y

    int x = x0, y = y0;
    int count = 0;

    if (dx >= dy) {
        // x is the driving axis (shallow or horizontal line / vertical too if dx==dy==0)
        int decision_param = 2 * dy - dx;
        while (true) {
            vertices.push_back(x);
            vertices.push_back(y);
            count++;
            if (x == x1) break;
            if (decision_param < 0) {
                decision_param += 2 * dy;
            } else {
                y += sy;
                decision_param += 2 * dy - 2 * dx;
            }
            x += sx;
        }
    } else {
        // y is the driving axis (steep line, including perfectly vertical bars)
        int decision_param = 2 * dx - dy;
        while (true) {
            vertices.push_back(x);
            vertices.push_back(y);
            count++;
            if (y == y1) break;
            if (decision_param < 0) {
                decision_param += 2 * dx;
            } else {
                x += sx;
                decision_param += 2 * dx - 2 * dy;
            }
            y += sy;
        }
    }
    return count;
}

void drawFilledBar(std::vector<int>& vertices, int xLeft, int xRight, int yBase, int yTop) {
    for (int x = xLeft; x <= xRight; x++) {
        drawLineDDA(vertices, x, yBase, x, yTop);
    }
}

unsigned int compileShader(unsigned int type, const char* src) {
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    int ok;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(id, 512, nullptr, log);
        std::cerr << log;
    }
    return id;
}

unsigned int createProgram() {
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    unsigned int prog = glCreateProgram();
    glAttachShader(prog, vs); glAttachShader(prog, fs);
    glLinkProgram(prog);
    glDeleteShader(vs); glDeleteShader(fs);
    return prog;
}

int main() {
    std::vector<int> data = { 50, 120, 80, 200, 150, 90, 30 };

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int WIN_W = 1200, WIN_H = 800;
    GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, "Histogram - DDA/Bresenham Line Algorithm", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, WIN_W, WIN_H);

    std::vector<int> vertices;

    int marginLeft = 80, marginBottom = 80, marginTop = 40, marginRight = 40;
    int plotW = WIN_W - marginLeft - marginRight;
    int plotH = WIN_H - marginBottom - marginTop;

    int maxVal = *std::max_element(data.begin(), data.end());
    int n = (int)data.size();
    int gap = 20;
    int barWidth = (plotW - gap * (n + 1)) / n;

    // X axis (baseline) and Y axis, drawn with the same line function as the bars
    drawLineDDA(vertices, marginLeft, marginBottom, WIN_W - marginRight, marginBottom);
    drawLineDDA(vertices, marginLeft, marginBottom, marginLeft, WIN_H - marginTop);

    int x = marginLeft + gap;
    for (int i = 0; i < n; i++) {
        int barHeight = (int)((double)data[i] / maxVal * (plotH - 20));
        int yBase = marginBottom;
        int yTop  = marginBottom + barHeight;
        drawFilledBar(vertices, x, x + barWidth, yBase, yTop);
        x += barWidth + gap;
    }

    int totalPoints = (int)(vertices.size() / 2);

    // Normalize pixel coordinates into OpenGL clip space [-1, 1]
    std::vector<float> norm_vertices;
    for (size_t i = 0; i < vertices.size(); i++) {
        float val_to_push;
        if (i % 2 == 0) {
            val_to_push = float(vertices[i]) / float(WIN_W) * 2.0f - 1.0f;
        } else {
            val_to_push = float(vertices[i]) / float(WIN_H) * 2.0f - 1.0f;
        }
        norm_vertices.push_back(val_to_push);
    }

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, norm_vertices.size() * sizeof(float), &norm_vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int prog = createProgram();

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(prog);
        glBindVertexArray(VAO);
        int colorLocation = glGetUniformLocation(prog, "uColor");
        glUniform4f(colorLocation, 0.3f, 0.7f, 1.0f, 1.0f);
        int offsetLocation = glGetUniformLocation(prog, "offset");
        glUniform2f(offsetLocation, 0.0f, 0.0f);
        glDrawArrays(GL_POINTS, 0, totalPoints);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(prog);
    glfwTerminate();
    return 0;
}
