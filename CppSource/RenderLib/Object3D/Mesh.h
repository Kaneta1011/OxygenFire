#ifndef __teMesh_H__
#define __teMesh_H__
#include	"RenderLib\\Object3D\\Object3D.h"
#include	"RenderLib\\Shader\\Shader.h"

namespace RenderLib{

enum eVERTEX_INDEX_TYPE
{
	VERTEX_INDEX_POS,
	VERTEX_INDEX_COLOR,
	VERTEX_INDEX_MAX,
};
enum eVERTEX_SIZE_TYPE
{
	VERTEX_SIZE_POS = 3,
	VERTEX_SIZE_COLOR = 4,
	VERTEX_SIZE_ATTRIB = VERTEX_SIZE_POS+VERTEX_SIZE_COLOR,
};
enum eVERTEX_OFFSET_TYPE
{
	VERTEX_OFFSET_POS = 0,
	VERTEX_OFFSET_COLOR = VERTEX_SIZE_POS,
};

//----------------------------------------------------------------------
//	Mesh
//----------------------------------------------------------------------
class Mesh : public IObject3D
{
public:
	void Init();
	void Update();
	void Render(Shader* spShader);
	void Create_Box(float Width,float Height,float Depth);
	virtual ~Mesh(){void Destroy();}void Destroy();
protected:
	int				m_VertexMax;
	Vector3*	m_VertexPos;
	Vector4*	m_VertexColor;

	GLuint		m_VBO[VERTEX_INDEX_MAX];
};

}//RenderLib


#endif