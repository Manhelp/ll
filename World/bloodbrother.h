#pragma once
//-----------------------------------------------------------------------------
// ���ϵͳ���������ݽṹ
//-----------------------------------------------------------------------------
struct tagBrotherData
{
	DWORD	dwTeatherID;	// ��ݴ���ID��һ����ֻ����һ����磬һ�������Դ�5��С�ܣ�
	tagDWORDTime m_dwBrotherEndTime;	// ��ݽ���ʱ�䣬0Ϊû�н��

	tagBrotherData():dwTeatherID(0)
	{
		m_dwBrotherEndTime.Clear();
	};

	void SetEndTime(tagDWORDTime& time ){m_dwBrotherEndTime = time;}	
	void SetTeatherID(DWORD& dwID){	dwTeatherID = dwID;}	

	void init(DWORD dwID,tagDWORDTime time)
	{
		dwTeatherID = dwID;
		m_dwBrotherEndTime = time;
	}

	void clearTeatherInfo()
	{
		dwTeatherID = 0;
		m_dwBrotherEndTime.Clear();
	}

};



//��ݹ�����
class BloodbrotherMgr
{
public:
	BloodbrotherMgr(void);
	~BloodbrotherMgr(void);

	//INT GetBortherNum(){return m_data.nBrotherNum;}	// ���ͽ�ܵ�������ֻ��ʦ�����õ��˷�������Ϊʦ��ֻ����һ��
	DWORD GetTeatherID(){return m_data.dwTeatherID;}
	tagDWORDTime& GetEndTime(){return m_data.m_dwBrotherEndTime;}
	tagBrotherData& GetData(){return m_data;}
	void InitData(DWORD dwID,tagDWORDTime time);

	void SetEndTime(tagDWORDTime& time ){m_data.SetEndTime(time);}
	void SetTeatherID(DWORD& dwID){	m_data.SetTeatherID(dwID);}	
	void ClearTeatherData(){m_data.clearTeatherInfo();}

	bool CanMakeBrother();
private:
	tagBrotherData m_data;

};