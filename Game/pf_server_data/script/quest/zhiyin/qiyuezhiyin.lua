--�����ȡʱ����
function Q40043_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	--���������
	map.CreateInstance(1826991655, RoleID)
	role.RoleGotoNewMap(MapID, InstanceID, RoleID, 1826991655, 6236/50, 1, 6970/50)
end

--ע��
aux.RegisterQuestEvent(40043, 0, "Q40043_OnAccept")

function q05_PlayerEnter(MapID, InstanceID, RoleID)
	map.MapCreateColCreature(MapID, InstanceID, 6010061, 6372/50, 32, 8191/50, 1)
end

--ע�������ͼ�¼�
aux.RegisterMapEvent("q05", 2, "q05_PlayerEnter")

--�ҵ�����ʵĸ���ʵ��
function q05_GetOnePerfectMap(MapID, InstanceID, RoleID)

	return RoleID

end

aux.RegisterMapEvent("q05",  14, "q05_GetOnePerfectMap")

function N5600001_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	400002)		--�������ݿɣ�\n    ��Ҫ�����ǵ޽���Լ��Ҫ�������ǵĹ�ذ죡��
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	400003)			--����β�׽������
		msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	400004)			--���뿪����֮�ҡ�
		msg.AddMsgEvent(MsgID, 21,	6)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	400005)			--�������Ի���
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	400006)		--�������ݿɣ�\n    �������������Ǽ��ɿ�ʼ�������޽���Լ���޽���Լʱ������Ҫ�ڽ����·���5��������ѡ����������ṩ�ĵ������Ǻϵ�һ�������ѡ����ȷ��ͬʱ�����о��иõ��ߣ���������������Լֵ�������ӡ�����Լֵ�ﵽ�����Ϲ涨������֮������ܵõ������е������ˡ���
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	100001)			--��ȷ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 5 then
		--ɾ�������
		map.DeleteInstance(1826991655, InstanceID)
	end
end

aux.RegisterCreatureEvent(5600001, 7, "N5600001_OnTalk")

--��30��������ʱ�䣬����ʱ��ʹ��
function c6010061_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
    --ɾ�������
	map.DeleteInstance(1826991655, InstanceID)
end

aux.RegisterCreatureEvent(6010061, 13, "c6010061_OnDisappear")

--����ڻ�е��ߺ�����
function q05_CanTakeOverWhenOnline(MapID, RoleID)
	--������ҵ����ߵ�ͼ������todo
	return 3424071976, 762,  14174, 969
end

aux.RegisterMapEvent("q05",  12, "q05_CanTakeOverWhenOnline")

-------------------------------------------------------------------------------------------------------
--�ҵ���ͼ�ĳ��ں�����
function q05_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3424071976, 762,  14174, 969
end

aux.RegisterMapEvent("q05",  13, "q05_GetExporMapAndCoord")

--�������Ƿ��ܹ���������
function q05_CanEnter(MapID, InstanceID, RoleID)

	return 1

end

aux.RegisterMapEvent("q05",  15, "q05_CanEnter")
