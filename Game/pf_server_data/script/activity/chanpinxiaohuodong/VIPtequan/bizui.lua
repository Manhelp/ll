function anjing_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)--
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	if role.GetRoleItemNum(TargetID, 4810033) >= 1  or role.GetRoleItemNum(TargetID, 4810034) >= 1 then
		local MsgID_1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 605205)--XXX����ʹ�ý�����Ʒʧ��
		msg.AddMsgEvent(MsgID_1, 2, RoleID)
		msg.DispatchRoleMsgEvent(TargetID, MsgID_1)
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_2, 13, 0)
		msg.AddMsgEvent(MsgID_2, 1, 605204)--���XXXʹ�ý�����Ʒʧ��
		msg.AddMsgEvent(MsgID_2, 2, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_2)
	else
		
		if TypeID == 4810030 then
			local MsgID_1 = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID_1, 13, 0)
			msg.AddMsgEvent(MsgID_1, 1, 605203)--XXX����ʹ�ý�����Ʒ
			msg.AddMsgEvent(MsgID_1, 2, RoleID)
			msg.DispatchRoleMsgEvent(TargetID, MsgID_1)
			unit.AddBuff(Target_MapID, Target_InstanceID, TargetID, 9100601, TargetID)--���һ��BUFF���簲����
			local MsgID = msg.BeginMsgEvent();	--ע��㲥���
			msg.AddMsgEvent(MsgID,102,100187)--XXX�� XXX����
			msg.AddMsgEvent(MsgID, 2, RoleID)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.DispatchBroadcast(MsgID, -1, -1, -1)
		else
			local MsgID_1 = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID_1, 13, 0)
			msg.AddMsgEvent(MsgID_1, 1, 605201)--һ����������������
			msg.DispatchRoleMsgEvent(TargetID, MsgID_1)
			unit.AddBuff(Target_MapID, Target_InstanceID, TargetID, 9100601, TargetID)--���һ��BUFFȫ���簲����
			local MsgID = msg.BeginMsgEvent();	--ע��㲥���
			msg.AddMsgEvent(MsgID,102,100185)--һ����������� XXX����7Сʱ
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.DispatchBroadcast(MsgID, -1, -1, -1)
		end
	end
end

aux.RegisterItemEvent(4810030, 1, "anjing_OnUse")
aux.RegisterItemEvent(4810031, 1, "anjing_OnUse")



