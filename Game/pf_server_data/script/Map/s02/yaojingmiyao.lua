--ID 3060006  ������ҩ���һ��ɣ�ID 3070023 ������ҩ
function miyaoduihuanshang_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 400007)		--������ҩ�һ��ˣ�����Ҫ�һ��µ�������ҩô��
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100001)		--ȷ��
		msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 400008)		--������ҩ�һ��ˣ���ѡ��һ���ʽ
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 6)			--�һ�10��
		msg.AddMsgEvent(MsgID, 1, 400009)		--
		msg.AddMsgEvent(MsgID, 21, 7)			--�һ�5��
		msg.AddMsgEvent(MsgID, 1, 400010)		--
		msg.AddMsgEvent(MsgID, 21, 8)			--�һ�1��
		msg.AddMsgEvent(MsgID, 1, 400011)		--
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 6 then
		if role.GetRoleItemNum(RoleID, 3060006) < 10 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 400012)		--������ҩ�һ��ˣ�����10�����޷�10��һ����жһ�
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 3060006, 10, 101)
			role.AddRoleItem(MapID, InstanceID, RoleID, 3070023, 10, -1, 8, 420)
		end
	elseif TalkIndex == 7 then
		if role.GetRoleItemNum(RoleID, 3060006) < 5 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 400013)		--������ҩ�һ��ˣ�����5�����޷�5��һ����жһ�
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 3060006, 5, 101)
			role.AddRoleItem(MapID, InstanceID, RoleID, 3070023, 5, -1, 8, 420)
		end
	elseif TalkIndex == 8 then
		if role.GetRoleItemNum(RoleID, 3060006) < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 400014)		--������ҩ�һ��ˣ�����1�����޷�1��һ����жһ�
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 3060006, 1, 101)
			role.AddRoleItem(MapID, InstanceID, RoleID, 3070023, 1, -1, 8, 420)
		end
	end
end
aux.RegisterCreatureEvent(5610115, 7, "miyaoduihuanshang_OnTalk")

