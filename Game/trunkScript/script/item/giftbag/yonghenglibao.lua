function I4700277_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local gold = 1314

	role.AddRoleSilver(MapID, InstanceID, TargetID, gold*10000, 1, -1, 8, 420)



end

function I4700277_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local freegold=role.GetRoleSilver(MapID,InstanceID,TargetID)


		--�жϽ�Ǯ�Ƿ񳬳�����
    if(freegold > 9986859999) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 306002)--��Ҫ����x�񱳰��ռ�ſɼ�������
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 999
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700277, 1, "I4700277_GiftBag")
aux.RegisterItemEvent(4700277, 0, "I4700277_CanUseGiftBag")
