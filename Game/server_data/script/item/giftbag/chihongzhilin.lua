function I4830069_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local num = 0 
	if TypeID == 4830069 then
		num = 3000
	elseif TypeID == 4830070 then
		num = 5000
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820262, num, -1, 8, 420)

end

function I4830069_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 1 then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4830069, 1, "I4830069_GiftBag")
aux.RegisterItemEvent(4830070, 1, "I4830069_GiftBag")
aux.RegisterItemEvent(4830069, 0, "I4830069_CanUseGiftBag")
aux.RegisterItemEvent(4830070, 0, "I4830069_CanUseGiftBag")
