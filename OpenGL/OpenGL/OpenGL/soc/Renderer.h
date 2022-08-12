#pragma once

#include<GL/glew.h>

#include"IndexBuffer.h"
#include"VertexArray.h"
#include"Shader.h"

#define ASSERT(x) if(!x) __debugbreak();
#define GLCALL(x) GLClearError(); x; \
    ASSERT(GLlogCall(#x, __FILE__, __LINE__))
//debug:在每行代码前加上GLCALL()

void GLClearError();

bool GLlogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};