function LVjingti_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	local FreeSize = role.GetBagFreeSize(RoleID)

	local a = role.GetRoleItemNum(RoleID, 4800087)--��ɫ����
	local b = role.GetRoleItemNum(RoleID, 4800086)--����ܰ
	
	if a < 5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206474)--��ɫ����ᾧ����5��
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 32

	else--�������������귶Χ������ʹ��
		if b < 5 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 206475)--��ɫ����ܰ����5��
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			bRet = 32
		else
		--����С��1ʱ����ʹ��
			local FreeSize = role.GetBagFreeSize(RoleID)
			if (FreeSize < 1) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)--����С��xʱ����ʹ��
				msg.AddMsgEvent(MsgID, 9, 1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				bRet = 40
				return bRet, bIgnore
			end
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4800087, 0, "LVjingti_CanUse")

function LVjingti_OnUse(MapID, InstanceID, TypeID, RoleID)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4800089, 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 4800087, 5, 101)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 4800086, 5, 101)
	return 1
end
aux.RegisterItemEvent(4800087, 1, "LVjingti_OnUse")

function LAjingti_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	local FreeSize = role.GetBagFreeSize(RoleID)

	local a = role.GetRoleItemNum(RoleID, 4800088)--��ɫ����
	local b = role.GetRoleItemNum(RoleID, 4800086)--����ܰ
	
	if a < 5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206473)--��ɫ����ᾧ����5��
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 32

	else--�������������귶Χ������ʹ��
		if b < 5 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 206475)--��ɫ����ܰ����5��
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			bRet = 32
		else
		--����С��1ʱ����ʹ��
			local FreeSize = role.GetBagFreeSize(RoleID)
			if (FreeSize < 1) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)--����С��xʱ����ʹ��
				msg.AddMsgEvent(MsgID, 9, 1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				bRet = 40
				return bRet, bIgnore
			end
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4800088, 0, "LAjingti_CanUse")

function LAjingti_OnUse(MapID, InstanceID, TypeID, RoleID)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4800090, 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 4800088, 5, 101)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 4800086, 5, 101)
	return 1
end
aux.RegisterItemEvent(4800088, 1, "LAjingti_OnUse")