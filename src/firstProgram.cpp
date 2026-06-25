#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

int main(){
    // start off the glfw window.
    glfwInit();
    // tell glfw which verion of glfw to open up. 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window by specifying the width and height
    GLFWwindow* window = glfwCreateWindow(800, 600, "My First Window", nullptr, nullptr);
    
    // now everyitme glfw draw  something make the glfw draw into this window
    glfwMakeContextCurrent(window);
    
    // load all opengl functions via glad
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

     // Step 5: Tell OpenGL the size of the drawing area
    glViewport(0, 0, 800, 600);
    
    while (!glfwWindowShouldClose(window)) {
        // Paint the screen dark blue-grey 
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f); // basically like picking up color in a bucker in ms-paint
        glClear(GL_COLOR_BUFFER_BIT);  // using that bucker color to pour onto the screen in ms-paint
        glfwSwapBuffers(window);  // show what we drew
        glfwPollEvents(); // checks for input via keyboard mouse or any input device specified
    }
    glfwTerminate();

    return 0;

}
