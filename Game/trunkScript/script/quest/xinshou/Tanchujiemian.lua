
--����������ѡ��ID��20002����ɺ��Զ�������ɫ���Խ���[C]
--�������ս����ID��20003����ɺ��Զ�������ɫ���ܽ���[K]
--�������������ID��20005����ɺ��Զ�������������
--�����ַ�ħ�飨ID��20006�����ܺ��Զ������������
--�������������ID��20009����ɺ��Զ�������������ͽ�ɫ���Խ���
--�����µ���;��ID��20017����ɺ��Զ�������������
--������в��֣�ID��20021����ɺ��Զ����������ͼ����


function q20002_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.OpenFramePage(ownerID, 1)
end

function q20003_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.OpenFramePage(ownerID, 3)
end

function q20005_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.OpenFramePage(ownerID, 2)
end

function q20006_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.OpenFramePage(ownerID, 4)
end

function q20009_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.OpenFramePage(ownerID, 1)
	role.OpenFramePage(ownerID, 2)
end

function q20017_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.OpenFramePage(ownerID, 2)
end




--ע��
aux.RegisterQuestEvent(20002, 1, "q20002_OnComplete")
aux.RegisterQuestEvent(20003, 1, "q20003_OnComplete")
aux.RegisterQuestEvent(20005, 1, "q20005_OnComplete")
aux.RegisterQuestEvent(20006, 1, "q20006_OnComplete")
aux.RegisterQuestEvent(20009, 1, "q20009_OnComplete")
aux.RegisterQuestEvent(20017, 1, "q20017_OnComplete")







