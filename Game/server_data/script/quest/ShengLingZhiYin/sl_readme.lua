function c6040033_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 650002)--����
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)
		msg.AddMsgEvent(MsgID, 1, 650003)--��������
		msg.AddMsgEvent(MsgID, 21, 5)
		msg.AddMsgEvent(MsgID, 1, 650004)--��ӡ/ϴӡʥ��
		msg.AddMsgEvent(MsgID, 21, 6)
		msg.AddMsgEvent(MsgID, 1, 650005)--ʥ�Ƴ���
		msg.AddMsgEvent(MsgID, 21, 7)
		msg.AddMsgEvent(MsgID, 1, 650006)--ʥ�ƻ���/��ȡ
		msg.AddMsgEvent(MsgID, 21, 10)
		msg.AddMsgEvent(MsgID, 1, 650009)--�ر�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 650011)--��������˵��
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, -1)
		msg.AddMsgEvent(MsgID, 1, 650010)--����
		msg.AddMsgEvent(MsgID, 21, 10)
		msg.AddMsgEvent(MsgID, 1, 650009)--�ر�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 650012)--��ӡ/ϴӡʥ��˵��
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, -1)
		msg.AddMsgEvent(MsgID, 1, 650010)--����
		msg.AddMsgEvent(MsgID, 21, 10)
		msg.AddMsgEvent(MsgID, 1, 650009)--�ر�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 6 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 650013)--ʥ�Ƴ���˵��
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, -1)
		msg.AddMsgEvent(MsgID, 1, 650010)--����
		msg.AddMsgEvent(MsgID, 21, 10)
		msg.AddMsgEvent(MsgID, 1, 650009)--�ر�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 7 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 650014)--ʥ�ƻ���/��ȡ˵��
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, -1)
		msg.AddMsgEvent(MsgID, 1, 650010)--����
		msg.AddMsgEvent(MsgID, 21, 10)
		msg.AddMsgEvent(MsgID, 1, 650009)--�ر�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end

aux.RegisterCreatureEvent(6040033, 7, "c6040033_On_Talk")
