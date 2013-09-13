#include "ActionMediate.h"
#include "math\kmathf.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
#include "GraphicsLib\Class\kPlane\kPlane.h"
#include "kaneta\ICharacter\Class\kPlayer\kPlayer.h"
#include "GraphicsLib\Class\r2DObj\r2DObj.h"
#include "Game\Gimmick\Gimmick.h"
#include "Game\Gimmick\G2D.h"
#include "input\Input.h"
#include "kaneta\Scene\MessageScene\MessageScene.h"
#include <stdlib.h>
using namespace rlib;

int TEST_POS_NUM = 0;

namespace klib
{
	using namespace math;
	using namespace ktl;

	ktl::kIAssociative<IGimmick*,TouchEvent> ActionMediate::m_Table;

	ActionMediate::PointerList ActionMediate::m_PointerList;


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
		m_Table.clear();
		m_PointerList.clear();

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

		std::list<IGimmick*> list=rlib::GimmickManager::getInst().getNearGimmick(m_Player->getObj()->getPosition(),3.0f);
		std::list<IGimmick*>::iterator it = list.begin();

		int i=0;
		while( it != list.end() )
		{
			i++;
			
			int index=-1;
			if(index=m_PointerList.find((*it))==-1)
			{
				m_PointerList.push_back((*it));
				index=m_PointerList.size()-1;
				m_Table[m_PointerList[index]].m_FontTime=0.0f;
				m_Table[m_PointerList[index]].m_RingTime=0.0f;
				m_Table[m_PointerList[index]].m_Pos=(*it)->getPos();
			}
			else
			{

			}
			it++;
		}
		for(int i=0;i<m_PointerList.size();i++)
		{
			f32 dist=m_Table[m_PointerList[i]].m_Pos.distance(m_Player->getObj()->getPosition());
			if(dist<3.0f)
			{
				m_Table[m_PointerList[i]].m_FontTime+=0.025f;
				m_Table[m_PointerList[i]].m_RingTime+=0.1f;

				if(1.0f<m_Table[m_PointerList[i]].m_FontTime)m_Table[m_PointerList[i]].m_FontTime=1.0f;
			}
			else
			{
				m_Table[m_PointerList[i]].m_FontTime-=0.025f;
				m_Table[m_PointerList[i]].m_RingTime-=0.1f;


				if(m_Table[m_PointerList[i]].m_RingTime<0.0f)m_Table[m_PointerList[i]].m_RingTime=0.0f;
				if(m_Table[m_PointerList[i]].m_FontTime<0.0f)
				{
					m_Table[m_PointerList[i]].m_FontTime=0.0f;
					m_PointerList.erase(i);
				}

			}
		}
	}

	void ActionMediate::render()
	{
			int touchIndex;
			int sx,sy;
			touchIndex=getTouch(&sx,&sy);

		for(int i=0;i<m_PointerList.size();i++)
		{
			m_AlphaBord->setTexture("maskTex",1,m_FireMask);
			m_AlphaBord->setShaderValue("alpha",1.0f-m_Table[m_PointerList[i]].m_FontTime);
			f32 ringScale=m_Table[m_PointerList[i]].m_FontTime*4.0f;
			kclampf(0,1,&ringScale);
			
			kPlane::render(m_AlphaBord,m_Font0,0.0f,1.0f,1.0f,math::Vector3(m_Table[m_PointerList[i]].m_Pos.x,m_Table[m_PointerList[i]].m_Pos.y+2.0f,m_Table[m_PointerList[i]].m_Pos.z),0,0,0,0);
			m_AddBord->setShaderValue("alpha",ringScale);
			kPlane::render(m_AddBord,m_Ring,m_Table[m_PointerList[i]].m_RingTime,(1.0f-ringScale)*4.0f+2.0f,(1.0f-ringScale)*4.0f+2.0f,m_Table[m_PointerList[i]].m_Pos,0,0,0,0);

			if(touchIndex!=-1)
			{
				int touchPosX=((sx+100.0f)/200.0f)*RenderLib::RenderState::getScreenWidth();
				int touchPosY=(1.0f-((sy+100.0f)/200.0f))*RenderLib::RenderState::getScreenHeight();
				float r=scaleParse((1.0f-ringScale)*4.0f+2.0f,m_Table[m_PointerList[i]].m_Pos);
				int gimicksx,gimicksy;
				worldToScreen(m_Table[m_PointerList[i]].m_Pos,&gimicksx,&gimicksy);
				// â~ì‡Ç©ÅH
				// åˆéÆÅ@(x -a)^2 + (y-b)^2 = r^2
				float point_val = (gimicksx - touchPosX) * (gimicksx - touchPosX) + (gimicksy - touchPosY) * (gimicksy - touchPosY);
				float r2 = r * r; 
				dprintf("(%d - %d)^2 + (%d - %d) ^2 < %f^2",gimicksx,touchPosX,gimicksy,touchPosY,r);
				//âÄì‡
				if (point_val < r2)
				{
					eprintf("touchGimick!!!!!!!!!!!!!!!!!!!!");
					GIMMICK_TYPE type=m_PointerList[i]->getType();
					if(type==eGIMMICK_2D)
					{
						G2D* gimick=(G2D*)m_PointerList[i];
						MessageScene::_getInstance().setTexture(gimick->getImage());
						framework.scenePush(MessageScene::_getInstancePtr());
					}
				}

			}


			

		}

	}
}