#pragma once

class Role;

class RoleShake
{
public:
	enum EDir
	{
		ED_None = -1,
		ED_Axis_X,
		ED_Axis_Y,
		ED_Axis_Z
	};

public:
	RoleShake(void);
	~RoleShake(void);

	//! �󶨽�ɫ
	void BindRole( Role* pRole, EDir type = ED_Axis_X );

	//! ��ʼ��
	void Start( DWORD dwTimeLen, int nShakeD );

	//! ��ת����
	void Update();

	//! ���ص�ǰ�Ƿ�������ת��
	bool IsShaking();

private:
	//! �õ��񶯲�����ƫ��
	Vector3 GetShakeOffset( int nIndex );

private:
	Role* m_pRole;

	bool  m_bStarted;		// �Ѿ���ʼ�񶯷�?
	EDir  m_type;			// �񶯲������˵ķ���
	float m_startTime;		// �񶯵Ŀ�ʼʱ��
	DWORD m_totalTime;		// �񶯵���ʱ��
	int	  m_nShakeD;		// �񶯵ķ���

	int	  m_nFrames;		// m_totalTime ʱ�����е�֡��
};
