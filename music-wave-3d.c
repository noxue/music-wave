#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow *window;

int main(void)
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

    do
    {
        // 清除屏幕
        glClear(GL_COLOR_BUFFER_BIT);

        // 在这里可以画东西

        // 交换缓冲区
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // 按下 ESC 键退出
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // 关闭 OpenGL 窗口，并结束 GLFW
    glfwTerminate();

    return 0;
}
