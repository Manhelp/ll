--����ҩ��ʹ���ж�
function I3010042_CanUseneishangyao(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�ж���ҵ������Ƿ����0
	local neishang = role.GetRoleAttValue(MapID, InstanceID , TargetID , 64 )
	if(neishang == 0) then
		--��ʾ����Ϊ0����ʹ��
		bRet = 31
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3010042, 0, "I3010042_CanUseneishangyao")
