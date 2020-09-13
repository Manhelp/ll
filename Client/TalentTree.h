#pragma once

#include "..\WorldDefine\skill_define.h"

class TalentButton;
struct tagSkillData;
class FlashStatic;
/** \struct	tagTalentLeaf
	\brief	��¼������Ҷ�ڵ��������
*/
struct tagTalentLeaf
{
	DWORD				dwTalentLeaf;				// ����ID
	ESkillUseType		eUseType;					// ����ʹ�÷�ʽ
	ETalentType			eTalentType;				// ������������
	ESkillLevelUpType	eLevelUpType;				// ������������
	BOOL				bActive;					// �Ƿ��Ͷ��
	INT					nCurLevel;					// ��ǰ�ȼ�
	INT					nLearnLevel;				// ϰ�õĵȼ�
	INT					nMaxLevel;					// ����Ͷ�ȼ�
	tagPoint			Pos;						// ����λ��
	DWORD				dwChildLeaf;				// ��������ID
	DWORD				dwParentLeaf;				// ���ߵ�����ID
	TalentButton*		pBtnTalentLeaf;				// ���ܰ�ť
	FlashStatic*		pStcMaxPoint;				// ����Ͷ������ʾ
	FlashStatic*		pStcBidden;					// ��Ͷ������ʾ
	GUIStatic*			pStcTalentBranch;			// ������֦
	TCHAR				szIconPath[X_SHORT_NAME];	// ����ͼ��
	
	DWORD				dwIconCtrlId;				// ����ͼ����ʾ�������ID��ʶ�ļ���ûѧ���ھ���ģʽ�»���ʾͼ��Ϊ�ʺ�

	tagTalentLeaf(){ZeroMemory(this, sizeof(tagTalentLeaf)); dwTalentLeaf = dwChildLeaf = NULL; }
};

/** \class	 TalentTree
	\brief	 ������
	\remarks �洢�����������ڵ㼰��Ӧ��ϵ
*/
class TalentTree
{
public:
	TalentTree(void);
	~TalentTree(void);
	
	/** \����������
	*/
	VOID Create(GUIWnd* pWnd, GUISystem* pGUI);
	/** \����������
	*/
	VOID Destroy();
	
	/** \��ȡҶ�ڵ㼼��ID
	*/
	DWORD GetCurLeafID(DWORD dwWndID){return m_mapIndex.Peek(dwWndID);}
	/** \��ȡҶ�ڵ����ݽṹ
	*/
	tagTalentLeaf* GetCurLeaf(DWORD dwWndID);

	/** \���õ�ǰҳ
	*/
	VOID SetCurPage(INT nPage);

	/** \���ص�ǰҳ
	*/
	VOID HideCurPage();

	/** \����������
	*/
	VOID UpdateTalentLeaf(DWORD dwID, BOOL bAdd = FALSE);
	/** \�����������������Ӱ��
	*/
	VOID OnRoleLevelUp(INT nLevel, INT nInc);
	/** \ְҵ�仯����������Ӱ��
	*/
	VOID OnRoleClassChange(EClassType eCur, EClassType ePast);
	/** \���ʵ�仯����������Ӱ��
	*/
	VOID OnTalentPointChange(ETalentType eTalent, INT nCurPoint);
	
	/** \������������������Ӱ��
	*/
	VOID OnTalentLevelUp(DWORD dwPreTalentID);
	/** \û�����ʵ�
	*/
	VOID OnNoTalentPoint();
	/** \�����ʵ��Ͷ
	*/
	VOID OnHaveTalentPoint();
	/** \��Ͷ�������ʹﵽ����
	*/
	VOID OnNoTalentTypeCanChoose();
	/** \�������
	*/
	VOID OnClearAllTalent();
	
	/** �����Ƿ��Ǿ�����ʾģʽ
	*/
	VOID SetSimpleDisplayMode( BOOL bDisplayMode )
	{ 
		if( bDisplayMode != m_bSimpleDisplayMode )
		{
			m_bSimpleDisplayMode = bDisplayMode; 
			Refresh4DisplayMode();
		}
	}

private:
	/** \��ȡ�����������ļ�
	*/
	VOID LoadTalentConfig(LPCTSTR szFile);
	/** \��ȡ������Ҷ�ڵ�
	*/
	VOID LoadTalentLeaf(XmlElement* pElement);
	
	/** \����һ������������
	*/
	VOID CreateWnd(ETalentType eType, GUIWnd* pFather, GUISystem* pGUI);
	/** \����һ��Ҷ�ڵ�
	*/
	VOID CreateLeaf(tagTalentLeaf* pLeaf, GUIWnd* pFather, GUISystem* pGUI);
	/** \����һ��Ҷ�ڵ����֦
	*/
	VOID CreateBranch(tagTalentLeaf* pLeaf, tagPoint ChildPos, GUIWnd* pFather, GUISystem* pGUI);
	
	/** \������Ͷ����
	*/
	VOID SetBidden(tagTalentLeaf* pLeaf);

	/** �Ƿ��Ͷ��
	*/
	BOOL IfSkillCanLevelUp(tagTalentLeaf* pLeaf);
	

	/** \��ȡ��ǰҶ�ڵ������
	*/
	VOID GetCurLeafPos(SIZE& size, tagPoint pos);
	/** \��ȡ��ǰҶ�ڵ���֦�Ĵ�С
	*/
	VOID GetCurBranchSize(SIZE& size, tagPoint pos, tagPoint child);

	/** \�ͷ�TMap������
	*/
	template<typename K, typename V>
	VOID FreeMap(TMap<K, V*> &mapList);

	/** \���ӿ���Ӱ���
	*/
	VOID AddTalentActiveEffect(DWORD dwTypeID);
	/** \����һ����Ԫ��
	*/
	template<typename A, typename B>
	VOID AddNode(TMap<A, TList<B>*> &mapList, A key, B value);

	/** \ˢ�¼��ܿ���״̬
	*/
	BOOL IsTalentLeafActive(tagTalentLeaf* pLeaf);
	/** \ˢ�¿�Ͷ�����ʼ����б�
	*/
	VOID ReflashActiveTalentList(tagTalentLeaf* pLeaf);

	/** \��ȡ��һ����TypeID
	*/
	DWORD GetNextLevelTalentTypeID(tagTalentLeaf* pLeaf, DWORD dwSkillType);
	
	/** \��������Tips
	*/
	VOID ReflashTalentTips(tagTalentLeaf* pLeaf);

	/** ������ʾģʽˢ���������ʵļ��ܵ�ͼ��
	*/
	VOID Refresh4DisplayMode();

	/** ѧ��һ�����ܺ�ˢ��������������ܵļ���ͼ��
		\param �Ѿ�ѧ��ļ���
	*/
	VOID RefreshTalentIcon( DWORD dwLearnedSkillTypeID );

private:
	TObjRef<Util>	m_pUtil;

	TMap<DWORD, tagTalentLeaf*>		m_mapTalent;						//--���������ݴ洢�������飬���м�ֵΪ������Ҷ�ڵ�����ID
	TMap<DWORD, tagTalentLeaf*>		m_mapBoxFirst2Talent;				//--�����ڵ�һ�����ܶ�Ӧ����Ҷ�ӽڵ��ӳ��
	TMap<DWORD, DWORD>				m_mapIndex;							//--������������ͨ����Ӧ�ؼ�ID�ҵ���Ӧ�ļ���ID
	GUIWnd*							m_pWndTalent[ETT_End];				//--�����������ı����ؼ�
	TList<DWORD>					m_listActiveTalent[ETT_End];		//--��Ͷ�����ʼ����б�
	TMap<INT, TList<DWORD>*>		m_mapTalentPointEffect[ETT_End];	//--���ʵ����Լ��ܵĿ���Ӱ��
	TMap<DWORD, TList<DWORD>*>		m_mapTalentSkillEffect;				//--���ܵȼ��Ժ������ܵĿ���Ӱ��
	TMap<INT, TList<DWORD>*>		m_mapRoleLevelEffect;				//--��ҵȼ��Լ��ܵĿ���Ӱ��
	TMap<EClassType, TList<DWORD>*>	m_mapRoleClassEffect;				//--���ְҵ�Լ��ܵĿ���Ӱ��
	INT								m_nCurPage;							//��ǰҳ

	BOOL							m_bSimpleDisplayMode;				//�Ƿ��Ǿ������ʾģʽ���������Щͼ��Ҫ��ʾ���ʺŲ�����tips
	
};


