#pragma once

enum EGuildPos
{
	EGP_HuiYuan = 0,		// ��Ա
	EGP_JingYing,				// ��Ӣ
	EGP_HuiZhang,			// �᳤
	EGP_FuHuiZhang,		// ���᳤
	EGP_GuGan,					// �Ǹ�
};

struct GuildRewardData
{
	EGuildPos	ePos;
	INT				nOccupyNum;
	DWORD		dwItem;
	DWORD		dwBuff;
};

class GuildWarRewardDate
{
public:
	typedef std::map<pair<EGuildPos,INT>,GuildRewardData> RewardMap;

	GuildWarRewardDate(void);
	~GuildWarRewardDate(void);

	BOOL	LoadFormFile();
	const GuildRewardData* FindData(const EGuildPos& pos,const INT& num) const;

public:
	RewardMap m_mapGuildWarReward;
};
