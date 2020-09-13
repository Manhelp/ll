#include "StdAfx.h"
#include "gm_policy.h"
#include "world.h"
#include "role_mgr.h"

#include "../ServerDefine/gm_tool_define.h"
#include "../ServerDefine/msg_rt_s.h"
#include "../ServerDefine/base_define.h"

#include "../WorldDefine/chat.h"
#include "../worlddefine/msg_gm_cmd.h"
#include "role.h"

GMPolicy g_GMPolicy;

//-----------------------------------------------------------------------------
// ˫������
//-----------------------------------------------------------------------------
GMDoublePolicy::GMDoublePolicy()
{
	for (INT i=0; i<NUM_EDT; ++i)
	{
		m_fCurDouble[i] = 1.0f;
		m_bDouble[i]	= FALSE;
	}
}

VOID GMDoublePolicy::SetRate( EDoubleType eDoubleType, DWORD dwRate, DWORD dwStart, DWORD dwLast )
{
	if (!EDT_VALID(eDoubleType))
		return;
	m_bDouble[eDoubleType] = TRUE;

	// ���ñ���
	m_fCurDouble[eDoubleType] = (FLOAT)dwRate / 100.0f;

	// ��¼�ر�ʱ��
	m_dwEndTime[eDoubleType] = IncreaseTime(dwStart, dwLast);

	tagNS_DoubleDropProbabilityChange msg;
	msg.eType = (EDDPType)eDoubleType;
	msg.nProbability = m_fCurDouble[eDoubleType];
	msg.bOpen = dwRate > 100;
	msg.dwTimeLeft = dwLast;
	g_roleMgr.SendWorldMsg(&msg,msg.dwSize);
}

VOID GMDoublePolicy::Update()
{
	for (INT i=0; i<NUM_EDT; ++i)
	{
		if (m_bDouble[i] == TRUE)
		{
			// ���Ŀǰ�����˶౶�ʣ���ô����Ƿ�ʱ
			tagDWORDTime dwNow = GetCurrentDWORDTime();

			if(dwNow >= m_dwEndTime[i])
			{
				// �౶��ʱ���ѵ�
				m_fCurDouble[i] = 1.0f;
				m_bDouble[i] = FALSE;

				tagNS_DoubleDropProbabilityChange msg;
				msg.eType = (EDDPType)i;
				msg.nProbability = m_fCurDouble[i];
				msg.bOpen = FALSE;
				msg.dwTimeLeft = 0;
				g_roleMgr.SendWorldMsg(&msg,msg.dwSize);
			}
		}
	}
}

VOID GMDoublePolicy::NotifyToPlayer(Role * p)
{
	if( !P_VALID(p) )
		return;
	// ���Ŀǰ�����˶౶�ʣ���ô����Ƿ�ʱ
	tagDWORDTime dwNow = GetCurrentDWORDTime();
	
	for (INT i=0; i<NUM_EDT; ++i)
	{
		if (m_bDouble[i] == TRUE)
		{
			if(dwNow < m_dwEndTime[i])
			{
				tagNS_DoubleDropProbabilityChange msg;
				msg.eType = (EDDPType)i;
				msg.nProbability = m_fCurDouble[i];
				msg.bOpen = TRUE;
				msg.dwTimeLeft = CalcTimeDiff(m_dwEndTime[i],dwNow);
				p->SendMessage(&msg,msg.dwSize);
			}
		}
	}
}

VOID	GMDoublePolicy::DoubleSwitch(EDoubleType eType, BOOL bFlag) 
//{ m_bDouble[eType] = bFlag; }
{
	m_bDouble[eType] = bFlag;
	if( !bFlag )
	{
		tagNS_DoubleDropProbabilityChange msg;
		msg.eType = (EDDPType)eType;
		msg.nProbability = 0;
		msg.bOpen = FALSE;
		msg.dwTimeLeft = 0;
		g_roleMgr.SendWorldMsg(&msg,msg.dwSize);
	}
}