

--����ʹ��
function c5610099_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local curmon = tonumber(os.date("%m"))--��
	local curday = tonumber(os.date("%d"))--��
	local curyear = tonumber(os.date("%Y"))--��
	local chunjieshizhe_start = false
	if curyear == 2012 and curmon == 1 and curday >=17 and curday <= 31  then
		chunjieshizhe_start = true
	else
		chunjieshizhe_start = false
	end
	if (chunjieshizhe_start == true and TalkIndex == -1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444601)				--����ʮ�£��ٹ�ͬ�죬�ҿ��Ǹ���������˺ܶ�ף�����Ͻ�ѡ����ϲ���İɣ����˿ɾ�û���ˡ���50�����µ�С���Ѿͱ�������ˣ���ȥ�����ɣ�
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)				--
		msg.AddMsgEvent(MsgID, 1, 444602)			--����ȡǩ���ơ�
		msg.AddMsgEvent(MsgID, 21, 5)				--
		msg.AddMsgEvent(MsgID, 1, 444603)			--���һ�������
		msg.AddMsgEvent(MsgID, 21, 30)				--
		msg.AddMsgEvent(MsgID, 1, 100002)			--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif (chunjieshizhe_start == false and TalkIndex == -1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444611)				--��Ѿ������ˣ��Ͻ�����ľ������߰ɣ������ҿ�Ҫ��ȥ�ˡ�
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 5)				--
		msg.AddMsgEvent(MsgID, 1, 444603)			--���һ�������
		msg.AddMsgEvent(MsgID, 21, 30)				--
		msg.AddMsgEvent(MsgID, 1, 100002)			--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444242)  			--���㵽��50�������ܲμ��������Ͻ�ȥ�����ɡ�
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444604)				--�����ǩ�������Ѿ���ȡ�ˣ������㹻��ǩ���ƿ��Զһ����ֵ��ߣ��������������ȡ��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, -1)				--
			msg.AddMsgEvent(MsgID, 1, 100001)			--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			local lastDate = role.GetRoleScriptData(RoleID, 1, RoleDataType["xinchunbaodao"])
			local curDate = tonumber(os.date("%j"))
			if(curDate == lastDate) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444605)				--�����ǩ�������Ѿ���ȡ���ˣ�����������
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, -1)				--
				msg.AddMsgEvent(MsgID, 1, 100001)			--��ȡ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if role.GetBagFreeSize(RoleID) < 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 100003)		--������<1
					msg.AddMsgEvent(MsgID, 9, 1)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					return 32,false
				else
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800261, 1, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["xinchunbaodao"],curDate)
				end
			end
		end
	elseif TalkIndex == 5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444606)				--��ѡ����Ҫ�һ���ǩ����������
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 6)				--
		msg.AddMsgEvent(MsgID, 1, 444607)			--��14������ǩ���ơ�
		msg.AddMsgEvent(MsgID, 21, 7)				--
		msg.AddMsgEvent(MsgID, 1, 444608)			--��7������ǩ���ơ�
		msg.AddMsgEvent(MsgID, 21, 8)				--
		msg.AddMsgEvent(MsgID, 1, 444609)			--��3������ǩ���ơ�
		msg.AddMsgEvent(MsgID, 21, -1)				--
		msg.AddMsgEvent(MsgID, 1, 100002)			--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 6 then
		local itemNum = role.GetRoleItemNum(RoleID, 4800261)
		if itemNum < 14 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444610)				--ǩ�����������㣬�޷��һ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, -1)				--
			msg.AddMsgEvent(MsgID, 1, 100002)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if role.GetBagFreeSize(RoleID) < 5 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)		--������<1
				msg.AddMsgEvent(MsgID, 9, 5)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return 32,false
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4800261, 14, 101)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4090011, 10, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4090010, 10, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820109, 8, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820047, 10, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820046, 10, -1, 8, 420)
				role.AddRoleSilver(MapID, InstanceID, RoleID, 30000000, 101)
			end
		end

	elseif(TalkIndex == 7) then
		local itemNum = role.GetRoleItemNum(RoleID, 4800261)
		if itemNum < 7 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444610)				--ǩ�����������㣬�޷��һ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, -1)				--
			msg.AddMsgEvent(MsgID, 1, 100002)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if role.GetBagFreeSize(RoleID) < 5 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)		--������<1
				msg.AddMsgEvent(MsgID, 9, 5)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return 32,false
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4800261, 7, 101)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4090011, 5, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4090010, 5, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820109, 3, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820047, 5, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820046, 5, -1, 8, 420)
				role.AddRoleSilver(MapID, InstanceID, RoleID, 8000000, 101)
			end
		end
	elseif(TalkIndex == 8) then
		local itemNum = role.GetRoleItemNum(RoleID, 4800261)
		if itemNum < 3 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444610)				--ǩ�����������㣬�޷��һ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, -1)				--
			msg.AddMsgEvent(MsgID, 1, 100002)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if role.GetBagFreeSize(RoleID) < 5 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)		--������<1
				msg.AddMsgEvent(MsgID, 9, 5)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return 32,false
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4800261, 3, 101)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4090011, 2, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4090010, 2, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820109, 1, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820047, 2, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820046, 2, -1, 8, 420)
				role.AddRoleSilver(MapID, InstanceID, RoleID, 2000000, 101)
			end
		end
	end
end
aux.RegisterCreatureEvent(5610099, 7, "c5610099_On_Talk")

