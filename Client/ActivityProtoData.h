#pragma once

enum EActivityMode
{
	EActivityMode_Day,
	EActivityMode_Week,
	EActivityMode_Month,
	EActivityMode_Year
};

enum ENewActivityType
{
	ENewActivityType_Festival,  // ����
	ENewActivityType_Scene,		// ����
	ENewActivityType_Fallow,	// ����
	ENewActivityType_PVP,		// PVP
	ENewActivityType_Instance,	// ����
	ENewActivityType_Shuaguai,	// ˢ��
	ENewActivityType_Daily		// �ճ�
};

struct tagActivityProtoData 
{
	UINT16 id;				// �id
	EActivityMode mode;		// �����
	tstring name;			// �����
	tstring desc;			// �˵��
	//EActivityType type;	// �����
	BOOL acceptTimeLimit;	// ��ȡʱ������
	ENewActivityType activityType; // �����
	
	INT  prompt;			// �Ƿ���ʾ
	INT  startyear;        //��ʼ��
	INT  endyear;         //������
	INT  startmonth;		// ��ʼ�£����ջʹ��
	INT  endmonth;			// ������
	INT  startday;			// ��ʼ�գ����ջʹ��
	INT	 endday;			// ������

	EWeek week;				// ���ڼ�
	int startHour;			// ��ʼСʱ
	int startMinute;		// ��ʼ����
	int endHour;			// ����Сʱ
	int endMinute;			// ��������
	DWORD acceptNpcID;		// ��ȡNPC��ID
	INT32 minLevel;			// �ȼ���С����
	INT32 maxLevel;			// �ȼ��������
	INT acceptTimes;		// ��ȡ��������
	bool visible;			// �Ƿ���ʾ��ÿ�ջ�б���
};

struct tagDailyQuestProtoData 
{
	UINT16	id;					// �id
	tstring name;				// �����
	UINT16	preID;				// ǰ�ûid
	INT32	minLevel;			// �ȼ���С����
	INT32	maxLevel;			// �ȼ��������
	INT		acceptTimes;		// ��ȡ��������
	mutable INT		doneTimes;
	tstring desc;
	DWORD	acceptNpcID;
};

class ActivityProtoData
{
public:
	typedef std::map<UINT16, tagActivityProtoData> ActivityProtoMap;
	typedef std::map<UINT16, tagDailyQuestProtoData> DailyProtoMap;
public:

	BOOL LoadFromFile();
	const ActivityProtoMap& GetData() const;
	const tagActivityProtoData *GetQuestProto(UINT16 id) const;

	const DailyProtoMap& GetDailyData() const;
	const tagDailyQuestProtoData *GetDailyProto(UINT16 id) const;

	VOID ClearDailyQuestDoneTimes();
private:
	ActivityProtoMap m_mapActivityProtoData;	// �ID����Ӧ�����Ա�
	DailyProtoMap    m_mapDaily;
};


