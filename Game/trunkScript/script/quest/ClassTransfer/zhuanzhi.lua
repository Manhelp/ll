--����ȡ
function q41017_On_CheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	local class = role.GetRoleClass(RoleID)
	local temp = true
	for i = 0, 3 do
		if role.IsRoleCompleteQuest(MapID, InstanceID, RoleID, 41017+i) or role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 41017+i) then
			temp = false
		end
	end
	if class == 2 and temp == true then
		return 1
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 605100)				--��������ɵ����������Ѿ��ﵽ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
end
aux.RegisterQuestEvent(41017, 4, "q41017_On_CheckAccept")


--����ȡ
function q41018_On_CheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	local class = role.GetRoleClass(RoleID)
	local temp = true
	for i = 0, 3 do
		if role.IsRoleCompleteQuest(MapID, InstanceID, RoleID, 41017+i) or role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 41017+i) then
			temp = false
		end
	end
	if class == 3 and temp == true then
		return 1
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 605100)				--��������ɵ����������Ѿ��ﵽ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
end
aux.RegisterQuestEvent(41018, 4, "q41018_On_CheckAccept")




--����ȡ
function q41019_On_CheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	local class = role.GetRoleClass(RoleID)
	local temp = true
	for i = 0, 3 do
		if role.IsRoleCompleteQuest(MapID, InstanceID, RoleID, 41017+i) or role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 41017+i) then
			temp = false
		end
	end
	if class == 4 and temp == true then
		return 1
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 605100)				--��������ɵ����������Ѿ��ﵽ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
end
aux.RegisterQuestEvent(41019, 4, "q41019_On_CheckAccept")






--����ȡ
function q41020_On_CheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	local class = role.GetRoleClass(RoleID)
	local temp = true
	for i = 0, 3 do
		if role.IsRoleCompleteQuest(MapID, InstanceID, RoleID, 41017+i) or role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 41017+i) then
			temp = false
		end
	end
	if class == 5 and temp == true then
		return 1
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 605100)				--����������ͬ���͵�������������ͬ���������޷��ظ���ȡ
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
end
aux.RegisterQuestEvent(41020, 4, "q41020_On_CheckAccept")
