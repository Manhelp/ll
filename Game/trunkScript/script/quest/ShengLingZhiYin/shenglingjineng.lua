function I3560001_Use(MapID, InstanceID, TypeID, RoleID)

	local shenglingSkill = role.IsLearnedSkill(RoleID, 59441) 
	if shenglingSkill == nil then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13,0)		--���Ѿ����ʥ���ٻ����ܣ����������ٻ�ʥ��Э����ս
		msg.AddMsgEvent(MsgID, 1,200008)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		role.AddSkill(RoleID, 5944101)
	end
end
aux.RegisterItemEvent(3560001, 1, "I3560001_Use")