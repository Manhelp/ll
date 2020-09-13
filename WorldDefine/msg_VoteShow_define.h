//-----------------------------------------------------------------------------
//!\file msg_VoteShow_define.h
//!
//!\date 2010-4-1
//!
//!\brief  ѡ�� �ͻ����������֮�����Ϣ
//-----------------------------------------------------------------------------

#pragma once
#pragma pack(push, 1)

enum EVoteShowError
{
	E_VoteShow_Success      =  0 ,
	E_VoteShow_NoNeckLace   =  1 ,
	E_VoteShow_SignIdInvaild=  2 ,
	E_VoteShow_UnOpen       =  3 ,
	E_VoteShow_End,
};
//------------------------------------------------------------------
// �������Ϳͻ��� ��Ů�������� 
//------------------------------------------------------------------
struct  tagVoteShowRankingsData
{
     TCHAR   tcRoleName[X_SHORT_NAME];
	 DWORD   dwPoint;
};
//------------------------------------------------------------------
// �������Ϳͻ��� ��Ů��ѯ��Ϣ
//------------------------------------------------------------------
CMD_START(NC_QueryDragonGirlRankings)
CMD_END


CMD_START(NS_QueryDragonGirlRankings)
INT16          n16Count;
tagVoteShowRankingsData  data[1];
CMD_END

//------------------------------------------------------------------
// �������Ϳͻ��� ����ʿ��ѯ��Ϣ
//------------------------------------------------------------------

CMD_START(NC_QueryDragonDefenderRankings)
CMD_END

CMD_START(NS_QueryDragonDefenderRankings)
INT16          n16Count;
tagVoteShowRankingsData  data[1];
CMD_END


#pragma pack(pop)