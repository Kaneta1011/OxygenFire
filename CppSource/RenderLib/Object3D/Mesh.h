#ifndef __teMesh_H__
#define __teMesh_H__
#include	"RenderLib\\Object3D\\Object3D.h"
#include	"RenderLib\\Shader\\Shader.h"

namespace RenderLib{

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
	Vector3*	posData;
};

}//RenderLib


#endif