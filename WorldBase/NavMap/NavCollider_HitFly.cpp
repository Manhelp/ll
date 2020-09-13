#include "StdAfx.h"
#include "NavCollider_HitFly.h"
#include "NavColliderPrivateDef.h"
#include "NavCollider.h"
#include "NavCarrierNode.h"


namespace WorldBase
{
	const FLOAT Y_SPEED_CAN_JUMP = -800.f;
	const FLOAT XY_SPEED_CAN_SLIDE = 300.f;
	const FLOAT XY_G = 3300.f;

	//--class NavCollider_Jump::Collider------------------------------------------------------------
	class NavCollider_HitFly::Collider
	{
	public:
		struct tagCDResult						//��ײ���
		{
			ENavResult	result;					//��ײ����ֵ

			struct tagLanded
			{
				float		landTime;			//��½ʱ�䣨���������ʱ�䣩

				void Fill(float _landTime)
				{
					landTime			= _landTime;
				}
			};

			struct tagSwim
			{
				float		swimTime;			//��Ӿʱ�䣨���������ʱ�䣩

				void Fill(float _swimTime)
				{
					swimTime			= _swimTime;
				}
			};

			struct tagOnWater
			{
				float		onWaterTime;		//��ˮʱ�䣨���������ʱ�䣩

				void Fill(float _onWaterTime)
				{
					onWaterTime			= _onWaterTime;
				}
			};

			struct tagSlide
			{
				float		slideTime;			//����ʱ�䣨���������ʱ�䣩

				void Fill(float _slideTime)
				{
					slideTime			= _slideTime;
				}
			};

			struct tagToBeContinue
			{
				float		lastCDTime;			//���һ����ײʱ�䣨���������ʱ�䣩

				void Fill(float _lastCDTime)
				{
					lastCDTime			= _lastCDTime;
				}
			};

			struct tagCarry
			{
				float		cdTime;				//��ײʱ�䣨���������ʱ�䣩
				D3DVECTOR	localLandPos;		//�ֲ��ռ���½λ��
				DWORD		dwCarrierObjID;		//�ؾ�ObjID

				void Fill(float _cdTime,const Vector3& _localLandPos,DWORD _dwCarrierObjID)
				{
					cdTime				= _cdTime;
					localLandPos		= _localLandPos;
					dwCarrierObjID		= _dwCarrierObjID;
				}
			};

			struct tagVDrop
			{
				float		vdropTime;			//��ֱ����ʱ�䣨���������ʱ�䣩
				float		cdCarrierTime;		//��ײ���ؾߵ�ʱ�䣨����ʱ�䣩
				DWORD		dwCarrierObjID;		//��ײ�����ؾ�ObjID��-1��ʾδ��ײ���ؾ�

				void Fill(float _vdropTime,const float _cdCarrierTime,DWORD _dwCarrierObjID)
				{
					vdropTime			= _vdropTime;
					cdCarrierTime		= _cdCarrierTime;
					dwCarrierObjID		= _dwCarrierObjID;
				}			
			};

			union 
			{
				tagLanded		landed;
				tagSwim			swim;
				tagOnWater		onWater;
				tagSlide		slide;
				tagToBeContinue	toBeContinue;
				tagCarry		carry;
				tagVDrop		vdrop;
			};
		};

	public:
		NavCollider_HitFly*	m_p;
		Vector3				m_lastPos;			//�ϴ���ײλ��
		float				m_lastTime;			//�ϴ���ײʱ�䣨���������ʱ�䣩
		float				m_cdInterval;		//��ײʱ����


	public:
		Collider(NavCollider_HitFly* pParent)
			:m_p(pParent)
		{}
		~Collider()
		{}

		void Init()
		{
			//--
			m_lastTime=0.0f;

			//--
			m_lastPos=m_p->m_CurPos;

			//--
			if(m_p->m_input.fXZSpeed>0.0f
				||m_p->m_input.fYSpeed>0.0f)
			{
				m_cdInterval=g_collideStep/max(m_p->m_input.fXZSpeed,m_p->m_input.fYSpeed);
				if(m_cdInterval>0.1f)
					m_cdInterval=0.1f;
			}
			else
				m_cdInterval=0.1f;
		}

		bool Collide( NavCollider* pCD,float startTime,float deltaTime,BitMap* pSpecCangoMap,bool bOnWaterCheck,tagCDResult& out )
		{
			const Vector3& startPos	= m_p->m_CurPos;
			const float xzSpeed		= m_p->m_fXZSpeed;
			const float ySpeed		= m_p->m_fYSpeed;
			const Vector3& dir		= m_p->m_input.xzDir;
			const float g			= m_p->m_input.g;
			const float timeOut		= m_p->m_input.cdTimeOut;

			for(float t=m_cdInterval;;t+=m_cdInterval)
			{
				//--
				float curTime=m_lastTime+m_cdInterval;

				//--
				
				Vector3 curPos=m_p->m_CurPos;

				//--������������
				if(!pCD->IfCanGo(curPos.x,curPos.z,pSpecCangoMap))
				{
					out.result=ENR_Arrive;
					out.vdrop.Fill(m_lastTime,0.0f,-1);
					return true;
				}

				const Vector3 halfSize		= m_p->m_input.roleSize*0.5f;
				const Vector3& roleSize		= m_p->m_input.roleSize;
				const DWORD cdBoxFlags		= m_p->m_input.cdBoxFlag;

				//--���ɽ�ɫAABB
				AABBox box;
				box.min.x=curPos.x-halfSize.x;
				box.max.x=curPos.x+halfSize.x;
				box.min.z=curPos.z-halfSize.z;
				box.max.z=curPos.z+halfSize.z;
				box.min.y=curPos.y;
				box.max.y=curPos.y+roleSize.y;

				//--�����Ƿ�����ײ
				DWORD dwCarrierObjID=-1;
				if(pCD->AABBoxCollideBox(box,startTime+t,cdBoxFlags,&dwCarrierObjID))
				{
					out.result=ENR_Arrive;
					out.vdrop.Fill(m_lastTime,startTime+t,dwCarrierObjID);
					return true;
				}

				//--����cdInterval
				float jumpSpeed=Vec3Dist(m_lastPos,curPos)/m_cdInterval;
				m_cdInterval=g_collideStep/jumpSpeed;
				if(m_cdInterval>0.1f)
					m_cdInterval=0.1f;

				//--�ж�ʱ���Ƿ��ˣ�
				if(t>=deltaTime
					||t>=timeOut)
				{
					out.result=ENR_ToBeContinued;
					out.toBeContinue.Fill(curTime);

					//--
					m_lastPos=curPos;
					m_lastTime=curTime;
					return false;
				}

				//--
				m_lastPos=curPos;
				m_lastTime=curTime;
			}

			//������ִ�е����
			ASSERT(0);
			return false;
		}	
	};

	NavCollider_HitFly::NavCollider_HitFly(void):m_state(EState_None)
	{
		m_pCollider=new Collider(this);
	}

	NavCollider_HitFly::~NavCollider_HitFly(void)
	{
		SAFE_DELETE(m_pCollider);
	}

	void NavCollider_HitFly::Init( const Vector3& startPos,const Vector3& xzDir,const Vector3& roleSize,float xzSpeed,float ySpeed,float g,float cdTimeOut,bool bTrnOnly,bool bIgnoreCarrier )
	{
		//--
		m_input.roleSize=roleSize;
		m_input.g=g;
		m_input.cdTimeOut=cdTimeOut;
		m_input.bTrnOnly=bTrnOnly;
		m_input.bIgnoreCarrier=bIgnoreCarrier;
		m_input.fXZSpeed = xzSpeed;
		m_input.fYSpeed = ySpeed;
		m_input.cdBoxFlag |= NavSceneNode::EFlag_Carrier;

		m_fStartTime = Kernel::Inst()->GetAccumTime();
		m_input.xzDir = xzDir;
		Vec3Normalize(m_input.xzDir);
		m_fXZSpeed = xzSpeed;
		m_fYSpeed = ySpeed;
		m_fOriXZSpeed = xzSpeed;
		m_CurPos = startPos;
		m_SlideStartPos = startPos;
		//--
		m_pCollider->Init();

		m_jumpCollider.Init(startPos,xzDir,roleSize,xzSpeed,ySpeed,g,cdTimeOut,bTrnOnly,bIgnoreCarrier);
		m_state=EState_Jump;
	}


	ENavResult NavCollider_HitFly::Update( NavCollider* pCollider,const Vector3& startPos,float startTime,float deltaTime,BitMap* pSpecCangoMap,Vector3& outPos,float& endTime,DWORD& dwCarrierObjID,bool bOnWaterCheck/*=false*/ )
	{
		switch(m_state)
		{
		case EState_Jump:
			{
				float cdCarrierTime=0.0f;
				ENavResult result=m_jumpCollider.Update(pCollider,startTime,deltaTime,pSpecCangoMap,outPos,endTime,dwCarrierObjID,cdCarrierTime,bOnWaterCheck);
				// ���㵱ǰy�ٶ�
				m_fYSpeed = m_fYSpeed - m_input.g * deltaTime;
				if(result==ENR_WillVDrop)
				{
					//--
					m_state=EState_VDrop;

					//--
					Vector3 xzDir(0.0f,0.0f,0.0f);
					float xzSpeed=0.0f;
					float ySpeed=0.0f;
					m_fYSpeed = 0.0f;
					m_fXZSpeed = 0.0f;
					if(dwCarrierObjID!=-1)
					{
						NavCarrierNode* pCarrier=pCollider->FindCarrier(dwCarrierObjID);
						ASSERT(pCarrier!=NULL&&pCarrier->IsRunning());

						pCarrier->AdjustJumpDirAndSpeed(cdCarrierTime,xzDir,xzSpeed,ySpeed);
					}

					//--
					m_jumpCollider.Init(outPos,xzDir,m_input.roleSize,xzSpeed,ySpeed,m_input.g,m_input.cdTimeOut,m_input.bTrnOnly,m_input.bIgnoreCarrier);

					//--
					deltaTime-=(endTime-startTime);
					if(deltaTime<0)
						deltaTime=0.0f;
					return m_jumpCollider.Update(pCollider,endTime,deltaTime,pSpecCangoMap,outPos,endTime,dwCarrierObjID,cdCarrierTime,bOnWaterCheck);
				}
				else
				{
					//����output
					if (ENR_Landed == result)
					{
						// ��һ�εأ��ٶ�˥��
						if(m_fYSpeed <= Y_SPEED_CAN_JUMP)
						{
							// ����
							m_fYSpeed = max(Y_SPEED_CAN_JUMP - 200 ,m_fYSpeed) * -1 - 600;
							m_fXZSpeed = max(0, m_fXZSpeed - 700);
							m_jumpCollider.Init(outPos,m_input.xzDir,m_input.roleSize,m_fXZSpeed,m_fYSpeed,m_input.g,m_input.cdTimeOut,m_input.bTrnOnly,m_input.bIgnoreCarrier);
							m_fStartTime = Kernel::Inst()->GetAccumTime();
							m_state = EState_Jump;
							return ENR_ToBeContinued;
						}
						else if (m_fYSpeed > Y_SPEED_CAN_JUMP && m_fXZSpeed >= XY_SPEED_CAN_SLIDE)
						{
							// ����
							m_fYSpeed = 0.0f;
							m_fOriXZSpeed = m_fXZSpeed;
							m_SlideStartPos = startPos;
							m_fStartTime = Kernel::Inst()->GetAccumTime(); 
							m_state = EState_SlideXY;
							return ENR_ToBeContinued;
						}
						else
						{
							float fHeight = pCollider->SafeGetGroundHeightSlerp(outPos.x,outPos.z);
							outPos.y = fHeight + min(g_tileSize / 2 ,m_input.roleSize.z/2);
							m_CurPos = outPos;
							m_state = EState_HitFlyStop;
						}
					}
					return result;
				}
			}
			break;
		case EState_VDrop:
			{
				float cdCarrierTime=0.0f;
				ENavResult result=m_jumpCollider.Update(pCollider,startTime,deltaTime,pSpecCangoMap,outPos,endTime,dwCarrierObjID,cdCarrierTime,bOnWaterCheck);
				m_fYSpeed = m_fYSpeed - m_input.g * deltaTime;
				//����output
				if (ENR_Landed == result)
				{
					// û��һ�εأ��ٶ�˥��
					// ���㵱ǰy�ٶ�
					//m_fYSpeed = m_fYSpeed - m_input.g * (Kernel::Inst()->GetAccumTime() - m_fStartTime);
					if(m_fYSpeed <= Y_SPEED_CAN_JUMP)
					{
						// ����
						m_fYSpeed = max(Y_SPEED_CAN_JUMP - 200 ,m_fYSpeed) * -1 - 600;
						m_fXZSpeed = 0.0f;
						m_jumpCollider.Init(outPos,m_input.xzDir,m_input.roleSize,m_fXZSpeed,m_fYSpeed,m_input.g,m_input.cdTimeOut,m_input.bTrnOnly,m_input.bIgnoreCarrier);
						m_fStartTime = Kernel::Inst()->GetAccumTime();
						m_state = EState_Jump;
						return ENR_ToBeContinued;
					}
					else
					{
						float fHeight = pCollider->SafeGetGroundHeightSlerp(outPos.x,outPos.z);
						outPos.y = fHeight + min(g_tileSize / 2 ,m_input.roleSize.z/2);
						m_CurPos = outPos;
						m_state = EState_HitFlyStop;
					}
				}
				return result;
			}
			break;
		case EState_SlideXY:
			{
				if (m_fXZSpeed > 0)
				{
					Collider::tagCDResult cdResult;
					bool bCDOk=m_pCollider->Collide(pCollider,startTime,deltaTime,pSpecCangoMap,bOnWaterCheck,cdResult);
					endTime = Kernel::Inst()->GetAccumTime();
					if (bCDOk)
					{
						m_state = EState_HitFlyStop;
						return ENR_ToBeContinued;
					}

					float curTimePass = Kernel::Inst()->GetAccumTime() - m_fStartTime;
					m_fXZSpeed = m_fXZSpeed - XY_G * deltaTime;
					outPos.x = m_SlideStartPos.x + m_fOriXZSpeed * m_input.xzDir.x * curTimePass - 0.5f * XY_G * m_input.xzDir.x * curTimePass * curTimePass;
					outPos.z = m_SlideStartPos.z + m_fOriXZSpeed * m_input.xzDir.z * curTimePass - 0.5f * XY_G * m_input.xzDir.z * curTimePass * curTimePass;
					float fHeight = pCollider->SafeGetGroundHeightSlerp(outPos.x,outPos.z);
					outPos.y = fHeight + min(g_tileSize / 2 ,m_input.roleSize.z/2);
					m_CurPos = outPos;
					return ENR_ToBeContinued;
				}
				else
				{
					endTime = Kernel::Inst()->GetAccumTime();
					outPos = m_CurPos;
					m_state = EState_HitFlyStop;
					return ENR_Arrive;
				}
			}
			break;
		case EState_HitFlyStop:
			{
				endTime = Kernel::Inst()->GetAccumTime();
				outPos = m_CurPos;
				return ENR_Arrive;
			}
			break;
		}

		ASSERT(0);
		return ENR_Failed;
	}

}//namespace WorldBase