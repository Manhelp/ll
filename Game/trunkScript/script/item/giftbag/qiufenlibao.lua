--������
function I4700186_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800131, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820040, 10, -1, 8, 420)
end

function I4700186_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700186, 1, "I4700186_GiftBag")
aux.RegisterItemEvent(4700186, 0, "I4700186_CanUseGiftBag")
