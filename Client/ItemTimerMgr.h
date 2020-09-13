#pragma once

class ItemTimerMgr
{
	static ItemTimerMgr m_SItemTimeMgr;
	ItemTimerMgr(void);
	~ItemTimerMgr(void);
	ItemTimerMgr(const ItemTimerMgr&){}
	const ItemTimerMgr& operator =(const ItemTimerMgr&){}
public:
	static ItemTimerMgr* Inst();
	VOID Init();
	VOID Destroy();
	VOID Update();
	
	DWORD GetItemRemainTime(INT64 n64ID);		// ����ʣ�������
	VOID	SetItemRemainTime(INT64 N64ID,DWORD dwTick);	// ʱ��Ϊʣ���tick����
private:
	std::map<INT64,DWORD>	m_mapItemTimeLimit;	// ʱ��������Ʒ��Ŀǰ��Ҫ��ս�����ģ�
};
