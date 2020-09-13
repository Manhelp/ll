

function ArenaAct_OnStart(actID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100131)  --�����������Ѿ������˶�������ʼ�ĺŽǡ�˭����������ǿ�ߣ�˭���ܳ�Ϊ�µĶ���֮������������Ŀ�Դ���
	msg.DispatchBroadcast(MsgID, -1, -1, -1)

    map.MapCreateColCreature(3424073512, -1, 5512001, 556, 22980, 487, "")
	map.MapCreateColCreature(3424073512, -1, 5512001, 827, 23510, 614, "")
	map.MapCreateColCreature(3424073512, -1, 5512001, 291, 23510, 754, "")

end

function ArenaAct_OnTimerMin(actID)

    local Min = tonumber(os.date("%M"))
	if Min == 5 or Min == 15 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100132)  --���������Ž������1��Сʱ�������������ҽ��л����ó�ɫ��Ʒ������ר�ü��ܣ�
		msg.DispatchBroadcast(MsgID, -1, -1, -1)
	end

end

function ArenaAct_OnEnd(actID)

	role.CloseArenaWaitingRoom()

end

--ע��û������¼�
aux.RegisterActEvent(31, 3, "ArenaAct_OnEnd")
aux.RegisterActEvent(32, 3, "ArenaAct_OnEnd")
aux.RegisterActEvent(31, 2, "ArenaAct_OnStart")
aux.RegisterActEvent(31, 4, "ArenaAct_OnTimerMin")
aux.RegisterActEvent(32, 2, "ArenaAct_OnStart")
aux.RegisterActEvent(32, 4, "ArenaAct_OnTimerMin")

--������NPC�Ի��ű�
function x5512001_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --��ȡ��ҵȼ�
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206263)		--��������������\n         ��������ҫ���������֣��㻹�ڵ�ʲô�������붷�����ɣ���
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	206264)		--�����������������
		msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	206265)		--���������������
		msg.AddMsgEvent(MsgID, 21,	6)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	206266)		--����������������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
	    if act.GetActIsStart(31) ~= true and act.GetActIsStart(32) ~= true then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206280)		--��������������\n         ������ֻ��ÿ���ض���ʱ��ŻῪ�ţ�ȥ������˽�һ�����Ŀ���ʱ��ɣ���
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		elseif rolelevel < 40 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206267)		--��������������\n         ֻ��40�����ϵ���Ҳ��ܲ���������������������ĵȼ����㣬���ǻ�ȥĥ��ĥ�������ɣ���
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69360) then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206281)		--��������������\n         ������ȴ�����������״̬��ʧ֮������ٴμ��붷������������
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    role.DeletRoleFromArenaTable(RoleID)
		    role.JoinArena(MapID,InstanceID,RoleID)
		end
	elseif TalkIndex == 5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206268)		--��������������\n         ֻ��40�����ϵ���Ҳ��ܲ������������������\n          �������ڲ��������ﵽ6�����Ժ��Զ���ʼ����ʱ�����ǻ�Ϊ�����������ѺͶ��֡�������ʼʱ����1���ӵ�׼��ʱ�䣬֮�󽫻���10���ӵ�ս��ʱ�䡣\n          ������ʤ�������ǣ���ɱ���е��˻���ʱ�����ʱ����������ڶԷ���\n          �ڶ������л�ʤ����ҽ���������ֵ�������Ӷ������������а��ڶ��������������뿪���ߴ���˾������Ľ���ʱ�������ʧһ����������������ֵ�Ļ������ʧ������ֵ�ǿ���Լ���������ֵ�Ķ��ٶ��ı�ġ���\n          ÿ�����10������������������������ȡ���鼰���������Ľ�����ͬʱ�����ʹ�ö����������������л�õĶ�������������򶷼���ר����Ʒ����
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	100001)		--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 6 then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206269)		--��������������\n         ��ʲô���ܰ��������𣿡�
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	7)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	206270)		    --����ȡÿ�ս�����
		msg.AddMsgEvent(MsgID, 21,	8)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	206271)		    --��40����ֵ�һ�1����������ҡ�
		msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	100001)		--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 7 then
	    local TimesToday = role.GetRoleScriptData(RoleID, 1, RoleDataType["TimesToday_Arena"])
		if TimesToday > 1000 then
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 20,	206272)		--��������������\n         �������Ѿ���ȡ��һ�ζ����������ˣ��޷��ٴ���ȡ��
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	6)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TimesToday < 6 then
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 20,	206273)		--��������������\n         ������ֻ�����X����������������Ҫ���10��������ȡ��������
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 9,	TimesToday)
			msg.AddMsgEvent(MsgID, 21,	6)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local roleexp = math.floor(activity_expe[rolelevel]*0.8)
			role.AddRoleExp(MapID, InstanceID, RoleID, roleexp)
			role.ModRoleAttValue(MapID, InstanceID, RoleID, 80, 250)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["TimesToday_Arena"], TimesToday+1000)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206274)		--��������������\n         ������˶�����ÿ�ս�����XXX�㾭���XX�㶷��ֵ������ֵ���������ڶ�����װ���̴����򶷼�����Ʒ����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 9,	roleexp)
			msg.AddMsgEvent(MsgID, 9,	250)
			msg.AddMsgEvent(MsgID, 21,	6)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

			--�һ���������
			UpdateSeverData(57, 1)
			--��Ҽ���
			UpdateRoleData(RoleID, 13, 1)
		end
	elseif TalkIndex == 8 then
	    if role.GetRoleAttValue(MapID, InstanceID, RoleID, 80) < 25 then
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 20,	206275)		--��������������\n         �Բ�����û���㹻�Ķ���ֵ�����޷�Ϊ���Ի�����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	6)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    if role.GetBagFreeSize(RoleID) >= 1 then
			    role.ModRoleAttValue(MapID, InstanceID, RoleID, 80, -25)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4600001, 1, -1, 8, 420)
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206269)		--��������������\n         ��ʲô���ܰ��������𣿡�
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	7)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	206270)		    --����ȡÿ�ս�����
				msg.AddMsgEvent(MsgID, 21,	8)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	206271)		    --��40����ֵ�һ�1����������ҡ�
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	100001)		--��ȡ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
			    local MsgID = msg.BeginMsgEvent()
			    msg.AddMsgEvent(MsgID, 20,	206276)		--��������������\n         ��ȷ�����������㹻�Ŀ�λ�����һ�����
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	6)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	end
end

aux.RegisterCreatureEvent(5512001, 7, "x5512001_OnTalk")

--����������ĶԻ�
function ArenaBox_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

    if TalkIndex == -1 then
		if Arena[InstanceID] ~= nil and Arena[InstanceID].Battle ~= nil then
			if (Arena[InstanceID].Battle == 0 or Arena[InstanceID].Battle == 1) or ArenaPlayer[RoleID].Info[InstanceID].Win == 0 then
            	local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206277)		--��    ���Ǹ����������䣬����װ�Ķ����Ǹ�����������ʤ���ߵĽ��͡���
				msg.AddMsgEvent(MsgID, 21,	6)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif Arena[InstanceID].Battle == 2 and ArenaPlayer[RoleID].Info[InstanceID].Win == 1 then
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206278)		--��    ʤ���ߣ�����ȡ��ľ����������ɣ���
				msg.AddMsgEvent(MsgID, 24,	TargetID)
				msg.AddMsgEvent(MsgID, 21,	4)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	elseif TalkIndex == 4 and ArenaPlayer[RoleID].Info[InstanceID].Win == 1 then
	    if Arena[InstanceID] ~= nil and ArenaPlayer[RoleID] ~= nil and ArenaPlayer[RoleID].Info[InstanceID] ~= nil and Arena[InstanceID].Battle == 2 then
		    for i=1,6 do
				local Map_ID,Instance_ID = role.GetRoleMapID(Arena[InstanceID].Player[i])
				if Map_ID ~= nil and Instance_ID ~= nil and Map_ID == MapID and Instance_ID == InstanceID and ArenaPlayer[Arena[InstanceID].Player[i]] ~= nil and ArenaPlayer[Arena[InstanceID].Player[i]].Info[InstanceID] ~= nil then
					if ArenaPlayer[RoleID].Info[InstanceID].Side == ArenaPlayer[Arena[InstanceID].Player[i]].Info[InstanceID].Side then
					    ArenaBox_GetItem(MapID, InstanceID, TargetTypeID, Arena[InstanceID].Player[i])
					end
				end
			end
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
	end
end

aux.RegisterCreatureEvent(5512004, 7, "ArenaBox_OnTalk")
aux.RegisterCreatureEvent(5512005, 7, "ArenaBox_OnTalk")
