function I4700276_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local gold = math.random(1,99)

	role.AddRoleSilver(MapID, InstanceID, TargetID, gold*10000, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700278, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 10, -1, 8, 420)


end

function I4700276_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	local freegold=role.GetRoleSilver(MapID,InstanceID,TargetID)
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40

		--�жϽ�Ǯ�Ƿ񳬳�����
    elseif(freegold > 9999009999) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 306002)--��Ҫ����x�񱳰��ռ�ſɼ�������
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 999
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700276, 1, "I4700276_GiftBag")
aux.RegisterItemEvent(4700276, 0, "I4700276_CanUseGiftBag")
