#pragma once

enum EBidType
{
	EBT_Normal = 0,
	EBT_BlackGold,
	EBT_BlackShengBi
};
struct tagBidDataLocal
{
	DWORD	dwID;						// id
	DWORD  dwItemID;				// ��ƷID
	int			nNum;						//	����
	EBidType	eType;						// ����
	DWORD	dwStartPrice;			// ��ʼ��
	DWORD	dwMinBidPrice;		// ���۵���
};

class BidData
{
	typedef map<DWORD,tagBidDataLocal> BidMap;
public:
	BidData(void);
	~BidData(void);

	void LoadFormFile();
	const tagBidDataLocal* GetBidData(const DWORD& id) const; 
private:
	BidMap m_mapBidMap;
};
