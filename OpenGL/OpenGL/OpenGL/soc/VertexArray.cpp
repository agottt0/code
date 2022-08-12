#include"VertexArray.h"
#include"Renderer.h"
#include"VertexBufferLayout.h"

VertexArray::VertexArray()
{
	GLCALL(glGenVertexArrays(1, &m_RendererID));
	GLCALL(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCALL(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Addbuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);//启用由index指定的通用顶点属性数组
		glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout.GetStride(), (const void*)offset);//指定在index处的通用顶点属性数组的位置和数据格式
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	
}

void VertexArray::Bind() const
{
	GLCALL(glBindVertexArray(m_RendererID));
}
void VertexArray::Unbind() const
{
	GLCALL(glBindVertexArray(0));
}