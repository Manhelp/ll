-- 39200 4245 6800
--����������ͼˢ��
function NPC6040043_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --��ȡ��ҵȼ�
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 207101)		--ѡ��������ͼˢ��
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			
		msg.AddMsgEvent(MsgID, 1, 207105)		--ˢ��
		msg.AddMsgEvent(MsgID, 21, 5)			
		msg.AddMsgEvent(MsgID, 1, 207102)		--��ʼ
		msg.AddMsgEvent(MsgID, 21, 6)			
		msg.AddMsgEvent(MsgID, 1, 207103)		--�ȴ�
		msg.AddMsgEvent(MsgID, 21, 7)			
		msg.AddMsgEvent(MsgID, 1, 207104)		--ֹͣ
		msg.AddMsgEvent(MsgID, 21, 8)			
		msg.AddMsgEvent(MsgID, 1, 207106)		--����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		map.SetMonsterGenState(MapID, InstanceID, 0)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,207110)      --������ͼ������ˢ��
		msg.DispatchWorldMsgEvent(MsgID)
	elseif TalkIndex == 5 then
		map.SetMonsterGenState(MapID, InstanceID, 1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,207107)      --������ͼ�����Կ�ʼ
		msg.DispatchWorldMsgEvent(MsgID)
	elseif TalkIndex == 6 then
		map.SetMonsterGenState(MapID, InstanceID, 2)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,207108)      --������ͼ�����Եȴ�
		msg.DispatchWorldMsgEvent(MsgID)
	elseif TalkIndex == 7 then
		map.SetMonsterGenState(MapID, InstanceID, 3)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,207109)      --������ͼ������ֹͣ
		msg.DispatchWorldMsgEvent(MsgID)
	elseif TalkIndex == 8 then
		map.SetMonsterGenState(MapID, InstanceID, 4)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,207111)      --������ͼ�����Լ���
		msg.DispatchWorldMsgEvent(MsgID)
	end
end
aux.RegisterCreatureEvent(6040043, 7, "NPC6040043_OnTalk")

