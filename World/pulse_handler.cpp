#include "StdAfx.h"
#include "player_session.h"
#include "role.h"
#include "..\WorldDefine\msg_pulse.h"

DWORD PlayerSession::HandleActivePulse(tagNetCmd * pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_ActivePulse);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
		return GT_INVALID;

	tagNS_ActivePulse send;
	send.dwid = pRecv->dwPulseID;
	send.dwErrorCode = pRole->ActivePulse(pRecv->dwPulseID);
	send.nLearnTime = pRole->GetPulseLearnTime();
	pRole->SendMessage(&send, send.dwSize);
	return 0;
}
