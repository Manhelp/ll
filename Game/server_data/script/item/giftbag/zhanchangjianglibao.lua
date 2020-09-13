--ս�����

function I4500116_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local level = role.GetRoleLevel(MapID, InstanceID, TargetID)
	if level >= 40 and level <= 49 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090001, 4, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090007, 4, -1, 8, 420)
	elseif level >= 50 and level <= 79 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090002, 5, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090008, 4, -1, 8, 420)
	elseif level >= 80 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090003, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 5, -1, 8, 420)
	end
end
function I4500116_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4500116, 1, "I4500116_GiftBag")
aux.RegisterItemEvent(4500116, 0, "I4500116_CanUseGiftBag")


--��ȡս������ҫ�ж�

function zhandouderongyao_OnCheackComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local a = role.IsRoleHaveBuff(MapID, InstanceID,RoleID,90007)
	if a == false then
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 303007)--������û�С�ս������ҫ��״̬���޷���ȡ����
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		return 0
        else
                return 1
	end
end
aux.RegisterQuestEvent(30714, 4, "zhandouderongyao_OnCheackComplete")

--��ȡ�߼���Ա�ν�

function gaojiguanyuanjiajiang_OnCheackComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local a = role.IsRoleHaveBuff(MapID, InstanceID,RoleID,90042)
	if a == false then
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 303008)--������û�С�ս������ҫ��״̬���޷���ȡ����
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		return 0
	end
	return 1
end
aux.RegisterQuestEvent(30715, 4, "gaojiguanyuanjiajiang_OnCheackComplete")



--��ȡʤ������ҫ

function shengliderongyao_OnCheackComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local a = role.IsRoleHaveBuff(MapID, InstanceID,RoleID,90043)
	if a == false then
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 303009)--������û�С�ս������ҫ��״̬���޷���ȡ����
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		return 0
	end
	return 1
end
aux.RegisterQuestEvent(30716, 4, "shengliderongyao_OnCheackComplete")















