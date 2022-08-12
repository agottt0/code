#include"Renderer.h"
#include"IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    GLCALL(glGenBuffers(1, &m_RenderID));//返回缓冲区中的 n 个缓冲区对象名称
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));//将缓冲区对象绑定到指定的缓冲区绑定点
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));//为当前绑定到目标的缓冲区对象创建新的数据存储区
}

IndexBuffer::~IndexBuffer()
{
    GLCALL(glDeleteBuffers(1, &m_RenderID));
}

void IndexBuffer::bind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
}

void IndexBuffer::unbind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}