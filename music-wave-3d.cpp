#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "common/shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

GLFWwindow *window;

// main

int main(int argc, char *argv[])
{
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 打开窗口并创建opengl上下文
    window = glfwCreateWindow(1024, 768, "3D音乐波形图", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // glfwSetKeyCallback(window, key_callback);

    // 初始化 GLEW
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "GLEW 初始化失败\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // 允许按键输入，后面按 Esc 键退出
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // 设置背景 rgba
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, -1.0f, // triangle 1 : begin
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f, -1.0f, // triangle 2 : begin
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f, // triangle 2 : end
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f};

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    // Or, for an ortho camera :
    // glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

    // 创建一个VAO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // This will identify our vertex buffer
    GLuint vertexbuffer;

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);

    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // One color for each vertex. They were generated randomly.
    static const GLfloat g_color_buffer_data[] = {
        0.583f, 0.771f, 0.014f,
        0.609f, 0.115f, 0.436f,
        0.327f, 0.483f, 0.844f,
        0.822f, 0.569f, 0.201f,
        0.435f, 0.602f, 0.223f,
        0.310f, 0.747f, 0.185f,
        0.597f, 0.770f, 0.761f,
        0.559f, 0.436f, 0.730f,
        0.359f, 0.583f, 0.152f,
        0.483f, 0.596f, 0.789f,
        0.559f, 0.861f, 0.639f,
        0.195f, 0.548f, 0.859f,
        0.014f, 0.184f, 0.576f,
        0.771f, 0.328f, 0.970f,
        0.406f, 0.615f, 0.116f,
        0.676f, 0.977f, 0.133f,
        0.971f, 0.572f, 0.833f,
        0.140f, 0.616f, 0.489f,
        0.997f, 0.513f, 0.064f,
        0.945f, 0.719f, 0.592f,
        0.543f, 0.021f, 0.978f,
        0.279f, 0.317f, 0.505f,
        0.167f, 0.620f, 0.077f,
        0.347f, 0.857f, 0.137f,
        0.055f, 0.953f, 0.042f,
        0.714f, 0.505f, 0.345f,
        0.783f, 0.290f, 0.734f,
        0.722f, 0.645f, 0.174f,
        0.302f, 0.455f, 0.848f,
        0.225f, 0.587f, 0.040f,
        0.517f, 0.713f, 0.338f,
        0.053f, 0.959f, 0.120f,
        0.393f, 0.621f, 0.362f,
        0.673f, 0.211f, 0.457f,
        0.820f, 0.883f, 0.371f,
        0.982f, 0.099f, 0.879f};

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(
        1,        // attribute. No particular reason for 1, but must match the layout in the shader.
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );
    float z = 4;
    float x = 0;

    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(5, 10, 10), // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0),   // and looks at the origin
        glm::vec3(0, 1, 0)    // Head is up (set to 0,-1,0 to look upside-down)
    );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

    do
    {
        // 清除屏幕
        glClear(GL_COLOR_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        // {
        //     z += 0.01f;
        // }
        // else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        // {
        //     z -= 0.01f;
        // }

        // if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        // {
        //     x -= 0.01f;
        // }
        // else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        // {
        //     x += 0.01f;
        // }

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        // 在这里可以画东西
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);

        // 交换缓冲区
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // 按下 ESC 键退出
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Cleanup VBO
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);

    // 关闭 OpenGL 窗口，并结束 GLFW
    glfwTerminate();

    return 0;
}
