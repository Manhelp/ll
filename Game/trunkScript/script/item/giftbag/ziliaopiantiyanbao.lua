
--����Ƭ�����
function I4700213_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4820047, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820045, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820048, 5, -1, 8, 420)
	local item = {8010014,8020014,8030014,8040014,8050014,8051014,8060014,8061014,8070014,8071014,8080014,
					8081014,8090014,8091014,8100014,8110014,8111014,8121014,8122014,8130014}
	role.AddRoleItem(MapID, InstanceID, TargetID, item[math.random(#item)], 1, 3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, item[math.random(#item)], 1, 3, 8, 420)
end

function I4700213_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	if(ziliaopianlibao == 0) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 444268)--����Ƭ��δ�������޷�ʹ���������ȴ�����Ƭ��������ʹ��
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 32
	else
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 5) then
			--��ʾ��ұ����ռ䲻��
			bRet = 40
		end
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700213, 1, "I4700213_GiftBag")
aux.RegisterItemEvent(4700213, 0, "I4700213_CanUseGiftBag")








