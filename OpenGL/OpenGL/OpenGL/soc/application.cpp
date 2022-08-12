#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

#include"Renderer.h"
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"VertexArray.h"
#include"Shader.h"
#include"VertexBufferLayout.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //glewInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(10);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "error" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = 
    {
        -0.5f, -0.5f,// 0
         0.5f, -0.5f,// 1
         0.5f,  0.5f,// 2
        -0.5f,  0.5f // 3
    };

    unsigned int indicies[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int vao;
    GLCALL(glGenVertexArrays(1, &vao));
    GLCALL(glBindVertexArray(vao));

    VertexArray va;
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.Addbuffer(vb, layout);

    IndexBuffer ib(indicies, 6);

    Shader shader("res/shaders/basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

    va.Unbind();  
    vb.unbind();
    ib.unbind();
    shader.Unbind();

    Renderer renderer;

    float r=0.0f, increment = 0.05f;

    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        shader.Bind();
        shader.SetUniform4f("u_color", r, 0.3f, 0.8f, 1.0f);

        renderer.Draw(va, ib, shader);
        
        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;
        r += increment;
        /*glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.0f,  0.5f);
        glVertex2f( 0.5f, -0.5f);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.4f,  0.4f);
        glVertex2f( 0.4f,  0.4f);
        glVertex2f(-0.4f, -0.4f);
        glEnd();*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}