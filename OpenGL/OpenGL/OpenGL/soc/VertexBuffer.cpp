#include"Renderer.h"
#include"VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCALL(glGenBuffers(1, &m_RenderID));//返回缓冲区中的 n 个缓冲区对象名称
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));//将缓冲区对象绑定到指定的缓冲区绑定点
    GLCALL(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), data, GL_STATIC_DRAW));//为当前绑定到目标的缓冲区对象创建新的数据存储区
}

VertexBuffer::~VertexBuffer()
{
    GLCALL(glDeleteBuffers(1, &m_RenderID));
}

void VertexBuffer::bind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
}

void VertexBuffer::unbind() const 
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}