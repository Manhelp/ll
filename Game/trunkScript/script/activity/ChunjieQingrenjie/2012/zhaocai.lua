--�вƵ������



function zhaocai_OnInvest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local y = role.GetBuffLayer(RoleID, 6106001)
	if y <= 8 then
		unit.AddBuff(MapID, InstanceID, RoleID, 6106001, RoleID)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 444433)--����Ч��ֻ�ܳ���30���ӣ�ֻ���ҵ�����ľ۱����������Ч�����������ӵ�10�κ󼴿ɽ������񣬻���ر�����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif y == 9 then
		unit.AddBuff(MapID, InstanceID, RoleID, 6106001, RoleID)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 444434)--����Ч�����ӵ�10���ˣ����Խ�������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 30862, 2)
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 444434)--����Ч�����ӵ�10���ˣ����Խ�������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 30862, 2)
	end
end
aux.RegisterCreatureEvent(6040025, 6, "zhaocai_OnInvest")

function zhaocai_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	unit.CancelBuff(MapID, InstanceID, RoleID, 6106001)
end
aux.RegisterQuestEvent(30862, 1, "zhaocai_OnComplete")


function zhaocai_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	if role.GetBuffLayer(RoleID, 6106001) > 9 then
		role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 30862, 2)
		return 1
	end
	return 0
end
aux.RegisterQuestEvent(30862, 5, "zhaocai_OnCheckComplete")
