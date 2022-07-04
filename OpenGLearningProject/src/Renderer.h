#pragma once
#include <GL/glew.h>
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

#define ASSERT(x) if(!(x)) __debugbreak()

#define GlCall(x) GlClearError();\
    x;\
    ASSERT(GlLogCall(#x, __FILE__, __LINE__))

#define GlCallReturns(x, y) GlClearError();\
	y = x;\
	ASSERT(GlLogCall(#x, __FILE__, __LINE__))

void GlClearError();
bool GlLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	void Clean() const;
	void Draw(const VertexArray& va, const Shader& shader) const;
};

