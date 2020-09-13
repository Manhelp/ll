--̨����������
function I4700001_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070061, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070062, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070063, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4220101, 1, -1, 8, 420)
end

function I4700001_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700001, 1, "I4700001_GiftBag")
aux.RegisterItemEvent(4700001, 0, "I4700001_CanUseGiftBag")

