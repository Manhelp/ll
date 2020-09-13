function I4830119_GiftBag(MapID, InstanceID, TypeID, TargetID)
	
	role.AddRoleItem(MapID, InstanceID, TargetID, 8220524, 1, 6, 8, 420)
	
end
function I4830119_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830119, 1, "I4830119_GiftBag")
aux.RegisterItemEvent(4830119, 0, "I4830119_CanUseGiftBag")
