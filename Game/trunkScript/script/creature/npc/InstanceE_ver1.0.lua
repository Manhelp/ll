--��������
experienceTeams = {}--{teamid=?,mapid=?,lev=?}
experienceNPCID = 0

function c5610090On_Talk(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID, TalkIndex)
	local curRolelev = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	if curRolelev >= 40 and curRolelev < 90 then
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444331)					--��ӭ�����Լ�����˵�����½�����Ҫ�����,40�����ϲ��ܽ���
			msg.AddMsgEvent(MsgID, 24, CreatureID)
			msg.AddMsgEvent(MsgID, 21, 4)
			msg.AddMsgEvent(MsgID, 1, 444332)					--�������鸱��
			msg.AddMsgEvent(MsgID, 21, 5)
			msg.AddMsgEvent(MsgID, 1, 444333)					--�����������ӳ���λ��
			msg.AddMsgEvent(MsgID, 21, 9)
			msg.AddMsgEvent(MsgID, 1, 100002)					--ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 4 then
			if TeamID ~= nil and TeamID ~= 4294967295 then
				local allinstances = {}
				allinstances[1266371918] = {x = 681,y = 53,z = 1110}	--f03
				allinstances[1266371150] = {x = 99,y = 116,z = 39}		--f04
				allinstances[1266371406] = {x = 1280,y = 297,z = 452} --f05
				allinstances[1266370638] = {x = 248,y = 258,z = 56}	--f06
				allinstances[1266370894] = {x = 667,y = 76,z = 109}	--f07
				allinstances[1266370126] = {x = 958,y = 2,z = 113}	--f08
				local rolelevel = {}
				rolelevel[4] = {1266371918,1266371150,1266371406,1266370638}	--f03-f06
				rolelevel[5] = {1266371150,1266371406,1266370638,1266370894}	--f04-f07
				rolelevel[6] = {1266371406,1266370638,1266370894,}				--f05-f07,1266370126
				rolelevel[7] = {1266370638,1266370894}							--f06-f07
				rolelevel[8] = {1266370894}										--f07
				local haveIns = false
				for k in pairs(experienceTeams) do
					if TeamID == experienceTeams[k].teamid then
						local level = math.floor(curRolelev/10)
						if experienceTeams[k].lev == level then
							role.RoleGotoNewMap(MapID, InstanceID, RoleID, experienceTeams[k].mapid, allinstances[experienceTeams[k].mapid].x, allinstances[experienceTeams[k].mapid].y, allinstances[experienceTeams[k].mapid].z)
						else
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 26, 444334)				--���ļ��𲻷��ϸ�������ʱ�ĵȼ�Ҫ��
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						end
						haveIns = true
						break
					end
				end
				if haveIns == false then
					--����
					local canEnter = true
					local num = 0
					local Role = {}
					Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
					local referlevel = math.floor(curRolelev/10)
					for i = 1, 6 do
						if Role[i] and Role[i]~= 4294967295 then
							local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
							local level = math.floor(role.GetRoleLevel(Target_MapID, Target_InstanceID, Role[i])/10)
							if referlevel ~= level then
								canEnter = false
								break
							end
							num = num + 1
						end
					end
					if canEnter == true and num > 2 then
						local theMap = rolelevel[referlevel][math.random(#rolelevel[referlevel])]
						table.insert(experienceTeams, {teamid=TeamID,mapid=theMap,lev=referlevel})
						role.RoleGotoNewMap(MapID, InstanceID, RoleID, theMap, allinstances[theMap].x, allinstances[theMap].y, allinstances[theMap].z)
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 444335)				--�������г�Ա�����ϵȼ�Ҫ����߶�����������3��
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 444336)				--��Ҫ��Ӳ��ܽ������鸱��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 5 then
			if TeamID ~= nil and TeamID ~= 4294967295 then
				local haveIns = false
				for k in pairs(experienceTeams) do
					if TeamID == experienceTeams[k].teamid then
						local level = math.floor(curRolelev/10)
						if experienceTeams[k].lev == level then
							local LeaderID = team.GetTeamLeaderID(TeamID)
							local Target_MapID, Target_InstanceID = role.GetRoleMapID(LeaderID)
							local curX,curY,curZ = unit.GetPosition(Target_MapID, Target_InstanceID, LeaderID)
							if experienceTeams[k].mapid == Target_MapID then
								role.RoleGotoNewMap(MapID, InstanceID, RoleID, Target_MapID, curX, curY/50, curZ)
							else
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 26, 444337)				--�ӳ�����ָ����ͼ������
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							end
						else
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 26, 444334)				--���ļ��𲻷��ϸ�������ʱ�ĵȼ�Ҫ��
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						end
						haveIns = true
						break
					end
				end
				if haveIns == false then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 444339)				--��δ�������鸱��
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 444336)				--��Ҫ��Ӳ��ܽ������鸱��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 444338)					--40�����ϲ��ܽ���
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end
aux.RegisterCreatureEvent(5610090, 7, "c5610090On_Talk")

--��������֤
function f03_onCheckRolelevel(MapID, InstanceID, RoleID)
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleID)
	local TeamID = role.IsRoleHaveTeam(Target_MapID, Target_InstanceID, RoleID)
	for k in pairs(experienceTeams) do
		if experienceTeams[k].mapid == MapID then
			local level = math.floor(role.GetRoleLevel(Target_MapID, Target_InstanceID, RoleID)/10)
			if level == experienceTeams[k].lev then
				return 1
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 444334)				--���ļ��𲻷��ϸ�������ʱ�ĵȼ�Ҫ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return 0
			end
		end
	end
	return 0
end
aux.RegisterMapEvent("f03", 26, "f03_onCheckRolelevel")
aux.RegisterMapEvent("f04", 26, "f03_onCheckRolelevel")
aux.RegisterMapEvent("f05", 26, "f03_onCheckRolelevel")
aux.RegisterMapEvent("f06", 26, "f03_onCheckRolelevel")
aux.RegisterMapEvent("f07", 26, "f03_onCheckRolelevel")
aux.RegisterMapEvent("f08", 26, "f03_onCheckRolelevel")

function experience_OnStart(actID)
	experienceTeams = {}
	--ˢNPC
	experienceNPCID = map.MapCreateColCreature(3424073512, -1, 5610090, 591, 23094, 664, 1)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 300092)		--�߼���������Ա�Ѿ�ˢ����<591,664>����������߼������Ĵ̼��𣿸Ͻ����ɣ�
	msg.DispatchBroadcast(MsgID,-1,-1,-1)
end
aux.RegisterActEvent(51, 2, "experience_OnStart")
aux.RegisterActEvent(52, 2, "experience_OnStart")

function experience_OnEnd(actID)
	experienceTeams = nil
	--ɾNPC
	if experienceNPCID ~= 0 then
		map.MapDeleteCreature(3424073512, -1, experienceNPCID)
	end
end
aux.RegisterActEvent(51, 3, "experience_OnEnd")
aux.RegisterActEvent(52, 3, "experience_OnEnd")


--ȡ����NPC�趨
function c5610096_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444205)				--ȷ��Ҫ�ص���ս�����
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)					--
		msg.AddMsgEvent(MsgID, 1, 100001)				--��ȷ����
		msg.AddMsgEvent(MsgID, 21, 5)					--
		msg.AddMsgEvent(MsgID, 1, 100002)				--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3424073512, 610+math.random(5), 23094, 669+math.random(5))
	end
end
aux.RegisterCreatureEvent(5610096, 7, "c5610096_On_Talk")





















