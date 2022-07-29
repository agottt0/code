#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

#define ASSERT(x) if(!x) __debugbreak();
#define GLCALL(x) GLClearError(); x; \
    ASSERT(GLlogCall(#x, __FILE__, __LINE__))
//debug:在每行代码前加上GLCALL()

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLlogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL_Error] (" << error << ")" << std::endl <<
            function << std::endl <<
            file << std::endl << 
            "line:" << line << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource
{
    std::string VertexShader;
    std::string FragmentShader;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);//read file

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];//字符串数组
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;//set mod in vertex shader
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;// set mod in fragment shader
        }
        else
        {
            ss[(int)type] << line << '\n';//在当前模式下执行
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << " Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);

        return 0;
    }
    return id;
}

static int CreateShader(const std::string& vertexShader, const std::string fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;

}
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

    unsigned int buffer;
    glGenBuffers(1, &buffer);//返回缓冲区中的 n 个缓冲区对象名称
    glBindBuffer(GL_ARRAY_BUFFER, buffer);//将缓冲区对象绑定到指定的缓冲区绑定点
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);//为当前绑定到目标的缓冲区对象创建新的数据存储区

    glEnableVertexAttribArray(0);//启用由index指定的通用顶点属性数组
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);//指定在index处的通用顶点属性数组的位置和数据格式

    unsigned int ibo;// index buffer object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies) * sizeof(float), indicies, GL_STATIC_DRAW);

    ShaderProgramSource source = ParseShader("res/shaders/basic.shader");
    unsigned int shader = CreateShader(source.VertexShader, source.FragmentShader);
    glUseProgram(shader);

    GLCALL(int location = glGetUniformLocation(shader, "u_Color"));
    //ASSERT(location != -1);
    GLCALL(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

    GLCALL(glBindVertexArray(0));
    GLCALL(glUseProgram(0));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    float r=0.0f, increment = 0.05f;

    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */

        glClear(GL_COLOR_BUFFER_BIT);

        GLCALL(glUseProgram(shader));
        GLCALL(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

        GLCALL(glBindVertexArray(vao));
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
  
        
        GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}