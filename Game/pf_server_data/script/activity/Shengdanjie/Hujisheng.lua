
function I4800008_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	local FreeSize1 = role.GetBagFreeSize(RoleID)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)

	if (level < 20) then
		--��ʾ��ҵȼ�����
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100008)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		bRet = 32
		return bRet, bIgnore
	elseif (FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100010)   --Ŀ��������ҵĿռ䲻��
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		bRet = 32
		return bRet, bIgnore
	elseif (FreeSize1 < 1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
		return bRet, bIgnore
	end
end


function I4800008_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 4800009, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4800003, 1, -1, 8, 420)


end


aux.RegisterItemEvent(4800008, 0, "I4800008_CanUse")
aux.RegisterItemEvent(4800008, 1, "I4800008_OnUse")
	