#include "ActionMediate.h"
#include "math\kmathf.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
#include "GraphicsLib\Class\kPlane\kPlane.h"
#include "kaneta\ICharacter\Class\kPlayer\kPlayer.h"
#include "GraphicsLib\Class\r2DObj\r2DObj.h"
#include <stdlib.h>
using namespace rlib;

int TEST_POS_NUM;

namespace klib
{
	using namespace math;
	using namespace ktl;

	ActionMediate::IndexList ActionMediate::m_IndexList;
	ActionMediate::TimeTable ActionMediate::m_TimeTable;

	r2DObj* ActionMediate::m_Font0;
	r2DObj* ActionMediate::m_FireMask;
	r2DObj* ActionMediate::m_Ring;

	kGraphicsPipline* ActionMediate::m_AlphaBord;
	kGraphicsPipline* ActionMediate::m_AddBord;

	Vector3 testpos[TEST_ACTION_POS];

	static kInputElementDesc billBordDesc[]=
	{
		{"POSITION",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
		{"TEXCOORD",0,k_VF_R32G32_FLOAT,0,eVertex,0}
	};
	static u32 billBordDescNum=sizeof(billBordDesc)/sizeof(kInputElementDesc);

	void ActionMediate::init()
	{

		m_Font0=new r2DObj;
		m_Font0->load("Font/font1.png");

		m_FireMask=new r2DObj;
		m_FireMask->load("kanetaPlace/mask.png");

		m_Ring=new r2DObj;
		m_Ring->load("kanetaPlace/ring.png");

		m_AlphaBord=new kGraphicsPipline();
		//pipline->createVertexShader("a");
		//m_AlphaBord->createVertexShader("kanetaPlace/shader/testbordvs.txt");
		//m_AlphaBord->createPixelShader("kanetaPlace/shader/testbordps.txt");
		m_AlphaBord->createShader("kanetaPlace/shader/testbordvs.txt","kanetaPlace/shader/testbordps.txt");
		m_AlphaBord->createBlendState(k_BLEND_ALPHA);
		m_AlphaBord->createDepthStencilState(true,true,eLESS_EQUAL);
		m_AlphaBord->createRasterizerState(eSOLID,eNONE,false);
		m_AlphaBord->complete(billBordDesc,billBordDescNum);

		m_AddBord=new kGraphicsPipline();
		//pipline->createVertexShader("a");
		//m_AddBord->createVertexShader("kanetaPlace/shader/touchbordvs.txt");
		//m_AddBord->createPixelShader("kanetaPlace/shader/touchbordps.txt");
		m_AddBord->createShader("kanetaPlace/shader/touchbordvs.txt","kanetaPlace/shader/touchbordps.txt");
		m_AddBord->createBlendState(k_BLEND_ADD);
		m_AddBord->createDepthStencilState(true,false,eLESS_EQUAL);
		m_AddBord->createRasterizerState(eSOLID,eNONE,false);
		m_AddBord->complete(billBordDesc,billBordDescNum);

		//m_Player=player;
		m_IndexList.clear();
		m_TimeTable.clear();

		for(int i=0;i<TEST_ACTION_POS;i++)
		{
			testpos[i].x=drand48()*20.0f-10.0f;
			testpos[i].y=drand48()*2.0f-1.0f;
			testpos[i].z=drand48()*20.0f-10.0f;
		}
	}

	bool ActionMediate::release()
	{
		delete m_Font0;
		delete m_FireMask;
		delete m_Ring;

		delete m_AlphaBord;
		delete m_AddBord;
	}


	bool ActionMediate::update(kPlayer* m_Player)
	{
		//前回のプレイヤー周囲のイベントに登録されていないイベントのタイマーを０にする
		//for(int i=0;i<TEST_ACTION_POS;i++)
		//{
		//	if(m_IndexList.find(i)!=-1)continue;
		//	m_TimeTable(i)=0.0f;
		//}

		//

		for(int i=0;i<TEST_ACTION_POS;i++)
		{
			f32 dist=testpos[i].distance(m_Player->getObj()->getPosition());
			if(m_IndexList.find(i)==-1)
			{
				m_TimeTable(i).m_FontTime=0.0f;
				m_TimeTable(i).m_RingTime=0.0f;
			}
			if(dist<3.0f)
			{
				m_TimeTable(i).m_FontTime+=0.025f;
				m_TimeTable(i).m_RingTime+=0.1f;

				if(1.0f<m_TimeTable(i).m_FontTime)m_TimeTable(i).m_FontTime=1.0f;
			}
			else
			{


				m_TimeTable(i).m_FontTime-=0.025f;
				m_TimeTable(i).m_RingTime-=0.1f;

				if(m_TimeTable(i).m_FontTime<0.0f)m_TimeTable(i).m_FontTime=0.0f;
				if(m_TimeTable(i).m_RingTime<0.0f)m_TimeTable(i).m_RingTime=0.0f;

			}
		}

		m_IndexList.clear();
		for(int i=0;i<TEST_ACTION_POS;i++)
		{
			if(0.0f<m_TimeTable(i).m_FontTime)
			{
				m_IndexList.push_back(i);
			}
		}
	}

	void ActionMediate::render()
	{
		for(int i=0;i<m_IndexList.size();i++)
		{
			m_AlphaBord->setTexture("maskTex",1,m_FireMask);
			m_AlphaBord->setShaderValue("alpha",1.0f-m_TimeTable[m_IndexList[i]].m_FontTime);
			f32 ringScale=m_TimeTable[m_IndexList[i]].m_FontTime*4.0f;
			kclampf(0,1,&ringScale);
			
			kPlane::render(m_AlphaBord,m_Font0,0.0f,1.0f,1.0f,math::Vector3(testpos[m_IndexList[i]].x,testpos[m_IndexList[i]].y+2.0f,testpos[m_IndexList[i]].z),0,0,0,0);
			m_AddBord->setShaderValue("alpha",ringScale);
			kPlane::render(m_AddBord,m_Ring,m_TimeTable[m_IndexList[i]].m_RingTime,(1.0f-ringScale)*4.0f+2.0f,(1.0f-ringScale)*4.0f+2.0f,testpos[m_IndexList[i]],0,0,0,0);
		}
	}
}