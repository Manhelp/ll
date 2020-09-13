--RoleDataType["received"]
--5600078npc

function c5600078_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local status = role.GetAccountReactiveStatus(MapID, InstanceID, RoleID)
	if status == 1 then
		local rescore = role.GetAccountReactiveScore(MapID, InstanceID, RoleID)
		local gblevel = role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)
		local tempa = 0
		local gbtable = {}
		gbtable[9] = {200,4700196}
		gbtable[8] = {160,4700195}
		gbtable[7] = {120,4700194}
		gbtable[6] = {80,4700193}
		gbtable[5] = {60,4700192}
		gbtable[4] = {35,4700191}
		gbtable[3] = {25,4700190}
		gbtable[2] = {15,4700189}
		gbtable[1] = {5,4700188}
		gbtable[0] = {1,4700187}
		for i=0,9 do
			if rescore >= gbtable[i][1] then
				tempa = i
			end
		end
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			if gblevel <= tempa and rescore > 0 then
				msg.AddMsgEvent(MsgID, 20, 444255)					--�𾴵�xxx:���ã���л����ʥħ֮Ѫ��һ��֧�֣��������ǵ��ۺ��϶����Ѿ�������Ϸ�еı���ɫ����Ϊ��ʿ�����ˡ��ڻع��ڼ䣬ֻҪ�ع���ʿ����Ϸ����������ר�������룬����Ϊ������һ���Ļ���֣���ʱ������ʹ�øû��ֶһ���Ӧ�Ľ�����/n����ר��������Ϊ��xxx�������ۻ�����Ϊ��xxx?
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 9, role.GetAccountReactiveKey(MapID, InstanceID, RoleID))
				msg.AddMsgEvent(MsgID, 9, rescore)
				msg.AddMsgEvent(MsgID, 9, gblevel+1)
				msg.AddMsgEvent(MsgID, 21, 4)
				msg.AddMsgEvent(MsgID, 1, 206219)					--����ȡ����
			else
				msg.AddMsgEvent(MsgID, 20, 444272)					--�𾴵�xxx:���ã���л����ʥħ֮Ѫ��һ��֧�֣��������ǵ��ۺ��϶����Ѿ�������Ϸ�еı���ɫ����Ϊ��ʿ�����ˡ��ڻع��ڼ䣬ֻҪ�ع���ʿ����Ϸ����������ר�������룬����Ϊ������һ���Ļ���֣���ʱ������ʹ�øû��ֶһ���Ӧ�Ľ�����/n����ר��������Ϊ��xxx�������ۻ�����Ϊ��xxx?
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 9, role.GetAccountReactiveKey(MapID, InstanceID, RoleID))
				msg.AddMsgEvent(MsgID, 9, rescore)
			end
			msg.AddMsgEvent(MsgID, 21, 5)
			msg.AddMsgEvent(MsgID, 1, 444256)					--������һ��
			msg.AddMsgEvent(MsgID, 21, 6)
			msg.AddMsgEvent(MsgID, 1, 444257)					--�����ֹ���
			msg.AddMsgEvent(MsgID, 21, 7)
			msg.AddMsgEvent(MsgID, 1, 100002)					--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 4 then
			if role.GetBagFreeSize(RoleID) > 0 then
				if rescore >= 1 and gblevel == 0 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700187, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				elseif rescore >= 5 and gblevel == 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700188, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				elseif rescore >= 15 and gblevel == 2 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700189, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				elseif rescore >= 25 and gblevel == 3 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700190, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				elseif rescore >= 35 and gblevel == 4 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700191, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				elseif rescore >= 60 and gblevel == 5 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700192, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				elseif rescore >= 80 and gblevel == 6 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700193, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				elseif rescore >= 120 and gblevel == 7 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700194, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				elseif rescore >= 160 and gblevel == 8 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700195, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				elseif rescore >= 200 and gblevel == 9 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700196, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 444267)				--����
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.AddMsgEvent(MsgID, 21, 7)
					msg.AddMsgEvent(MsgID, 1, 100002)				--��ȡ��
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
				--[[
				for k,v in pairs(gbtable) do
					if rescore >= v[1] and gblevel <= k then
						role.AddRoleItem(MapID, InstanceID, RoleID, v[2], 1, -1, 8, 420)
						role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 444267)				--����
						msg.AddMsgEvent(MsgID, 24, TargetID)
						msg.AddMsgEvent(MsgID, 21, 7)
						msg.AddMsgEvent(MsgID, 1, 100002)				--��ȡ��
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
					break
				end
				--]]
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444266)				--�����ռ䲻��
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, 7)
				msg.AddMsgEvent(MsgID, 1, 100002)			--��ȡ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 5 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444261)				--��Ӧ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 8)
			msg.AddMsgEvent(MsgID, 1, 444258)				--���������
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 443012)				--������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 6 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444259)				--���ֹ���
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 443012)				--������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 8 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444260)				--�������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 9)
			msg.AddMsgEvent(MsgID, 1, 444270)				--������
			msg.AddMsgEvent(MsgID, 21, 5)
			msg.AddMsgEvent(MsgID, 1, 443012)				--������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 9 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444271)				--�������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 8)
			msg.AddMsgEvent(MsgID, 1, 444269)				--������
			msg.AddMsgEvent(MsgID, 21, 5)
			msg.AddMsgEvent(MsgID, 1, 443012)				--������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif status == 2 then
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444262)					--�𾴵�xxx:���ã��ܸ����������»ص������ʥħ��½��ʥħ�������������Ӿ��ʡ����������ʿ�����˵������룬�����Ӧ�����������Ϊ�Է�����һ�����֡�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, RoleID)
			msg.AddMsgEvent(MsgID, 21, 4)
			msg.AddMsgEvent(MsgID, 1, 444263)					--������������
			msg.AddMsgEvent(MsgID, 21, 6)
			msg.AddMsgEvent(MsgID, 1, 100002)					--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 4 then
			if role.GetBagFreeSize(RoleID) > 0 then
				--����������
				role.PopInputReactiveKey(MapID, InstanceID, RoleID)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444266)				--�����ռ䲻��
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, 7)
				msg.AddMsgEvent(MsgID, 1, 100002)				--��ȡ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	else
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444265)					--�Բ��������߱��˴εĻ�ʸ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 6)
			msg.AddMsgEvent(MsgID, 1, 100002)					--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5600078, 7, "c5600078_On_Talk")


function ReactiveKeySuccessed(MapID,InstanceID,RoleID)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4700262, 1, -1, 8, 420)
end
aux.RegisterRoleEvent(85, "ReactiveKeySuccessed")
