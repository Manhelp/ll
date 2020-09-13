--�»���
function I4800114_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	local LoverID = role.GetRoleLoverID(MapID, InstanceID,RoleID)
	if FreeSize < 1 then
		--��ʾ��ұ����ռ䲻��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100010)   		--Ŀ��������ҵĿռ䲻��
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		bRet = 32
	elseif LoverID ~= nil and LoverID == TargetID then
		bRet = 32
	end
	return bRet, bIgnore
end


function I4800114_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 4800115, 1, -1, 8, 420)
end


aux.RegisterItemEvent(4800114, 0, "I4800114_CanUse")
aux.RegisterItemEvent(4800114, 1, "I4800114_OnUse")
