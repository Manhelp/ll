function huancaiwuqi_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local huancaiwuqi = act.GetActScriptData(55, 1, 10)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206499)		--�ò������һ���:\n    �һ�Ψһһ�ѻò�������
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100001)		--ȷ��
		msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if huancaiwuqi == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206496)		--�ò������һ���:\n    �Ѿ����˶һ�������Ʒ
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local num = role.GetRoleItemNum(RoleID, 4820262)
			if num < 60000 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206498)		--�ò������һ���:\n    �Ѿ����˶һ�������Ʒ
				msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if role.GetBagFreeSize(RoleID) < 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206497) -- "�ò������һ���:\n    ��ı����ռ䲻��1
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, 7)   --
					msg.AddMsgEvent(MsgID, 1, 100001)   --"ȷ��"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
				else
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4820262, 9999, 101)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4820262, 9999, 101)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4820262, 9999, 101)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4820262, 9999, 101)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4820262, 9999, 101)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4820262, 9999, 101)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4820262, 6, 101)
					role.AddRoleItem(MapID, InstanceID, RoleID, 4830071, 1, -1, 8, 420)
					act.SetActScriptData(55, 1, 10, 1)
					act.SaveActScriptData(55)
				end
			end
		end
	end
end
aux.RegisterCreatureEvent(6040052, 7, "huancaiwuqi_OnTalk")

