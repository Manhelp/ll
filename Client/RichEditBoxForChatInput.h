#pragma once
enum REBForChatInput
{
	EXUIE_ContextChange = EGUIE_User + 1024,
	EXUIE_ShowItemOrEquip,
};
class RichEditBoxForChatInput :
	public GUIRichEditBox
{
public:
	RichEditBoxForChatInput(void);
	~RichEditBoxForChatInput(void);
	//�����ͷŵ���Ʒ
	virtual VOID OnEvent(tagGUIEvent* pEvent); 

	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);

	/*
	�����Ƿ�չʾ�ϷŽ��ռ����Ʒ����װ��
	*/
	void SetShowItemOrEquip(bool show){m_bShowItemOrEquip = show ;};
private:
	bool m_bShowItemOrEquip;	//�Ƿ�չʾ��ק��������Ʒ����װ��
};
