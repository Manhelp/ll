
--��15��������ʱ�䣬����ʱ��ʹ��
function c15_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
    if Arena[InstanceID] ~= nil then
		if ArenaCreature_Timer[TargetID]~= nil and ArenaCreature_Timer[TargetID].TypeID == 4 then
			for i = 1,6 do
				local Map_ID,Instance_ID = role.GetRoleMapID(Arena[InstanceID].Player[i])
				if Map_ID ~= nil and Instance_ID ~= nil and Map_ID == MapID and Instance_ID == InstanceID then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 206258)          --ս������<color=0xffffff00>15<color=0xfffff7e0>���ʼ
					msg.DispatchRoleMsgEvent(Arena[InstanceID].Player[i], MsgID)     --������Ϣ
				end
			end
		elseif ArenaCreature_Timer[TargetID]~= nil and ArenaCreature_Timer[TargetID].TypeID == 9 then
			for i = 1,6 do
				local Map_ID,Instance_ID = role.GetRoleMapID(Arena[InstanceID].Player[i])
				if Map_ID ~= nil and Instance_ID ~= nil and Map_ID == MapID and Instance_ID == InstanceID then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 206259)          --���Ž���<color=0xffffff00>45<color=0xfffff7e0>����
					msg.DispatchRoleMsgEvent(Arena[InstanceID].Player[i], MsgID)     --������Ϣ
				end
			end
		end
	end
	ArenaCreature_Timer[TargetID] = nil
end



--��30��������ʱ�䣬����ʱ��ʹ��
function c30_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
    if Arena[InstanceID] ~= nil then
		if ArenaCreature_Timer[TargetID]~= nil and ArenaCreature_Timer[TargetID].TypeID == 7 then
			if Arena[InstanceID].Battle ~= nil and Arena[InstanceID].Battle == 1 then
				for i = 1,6 do
					local Map_ID,Instance_ID = role.GetRoleMapID(Arena[InstanceID].Player[i])
					if Map_ID ~= nil and Instance_ID ~= nil and Map_ID == MapID and Instance_ID == InstanceID then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 101, 206260)          --ս������<color=0xffffff00>30<color=0xfffff7e0>������
				        msg.DispatchRoleMsgEvent(Arena[InstanceID].Player[i], MsgID)
					end
				end
			end
        end
	end
    ArenaCreature_Timer[TargetID] = nil
end



--��1���ӹ������ʱ�䣬����ʱ��ʹ��
function c60_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)

    if Arena[InstanceID].Battle == 2 then
		for i=1,6 do
			local Map_ID,Instance_ID = role.GetRoleMapID(Arena[InstanceID].Player[i])
			if Map_ID ~= nil and Instance_ID ~= nil and Map_ID == MapID and Instance_ID == InstanceID then
				unit.UnsetState(MapID, InstanceID, Arena[InstanceID].Player[i], 10)
				unit.UnsetState(MapID, InstanceID, Arena[InstanceID].Player[i], 11)
				if unit.IsDead(MapID, InstanceID, Arena[InstanceID].Player[i]) then
					role.ForcedToRevive(MapID, InstanceID, Arena[InstanceID].Player[i])
				end
				role.DeletRoleFromArenaTable(Arena[InstanceID].Player[i])
			end
		end
		map.DeleteInstance(MapID, InstanceID)
		role.ShowArenaRoomFrame(Arena[InstanceID].Room, 0, 0, 0, 0, 0, 0)
		RemoveValue_Arena (InstanceID)
	elseif Arena[InstanceID].Battle == 0 then

		for i=1,6 do
			local Map_ID,Instance_ID = role.GetRoleMapID(Arena[InstanceID].Player[i])
			if Map_ID ~= nil and Instance_ID ~= nil and Map_ID == MapID and Instance_ID == InstanceID then
			else
				if ArenaPlayer[Arena[InstanceID].Player[i]] ~= nil and ArenaPlayer[Arena[InstanceID].Player[i]].Info[InstanceID] ~= nil then
					if ArenaPlayer[Arena[InstanceID].Player[i]].Now == InstanceID then
						ArenaPlayer[Arena[InstanceID].Player[i]].Now = 0
						role.DeletRoleFromArenaTable(Arena[InstanceID].Player[i])
					end
				end
			end
		end

		map.OpenCloseDoor(MapID, InstanceID, ArenaMap[InstanceID%3].Door1, 1)
		map.OpenCloseDoor(MapID, InstanceID, ArenaMap[InstanceID%3].Door2, 1)

		if Arena[InstanceID].BlueNum == 0 or Arena[InstanceID].RedNum == 0 then

			Arena[InstanceID].Battle = 2
			ShowArenaFinalFrame(MapID, InstanceID)

			local creature1 = map.MapCreateCreature(MapID, InstanceID, 1094003, 6, 1, 9)
			ArenaCreature_Timer[creature1]= {TypeID = 6}      --60�봫�ͼ�ʱ

		else

			Arena[InstanceID].Battle = 1

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 206261)          --<color=0xffff0000>ս����ʼ<color=0xfffff7e0>
			for i=1,6 do
				local Map_ID,Instance_ID = role.GetRoleMapID(Arena[InstanceID].Player[i])
				if Map_ID ~= nil and Instance_ID ~= nil and Map_ID == MapID and Instance_ID == InstanceID then
					msg.DispatchRoleMsgEvent(Arena[InstanceID].Player[i], MsgID)     --������Ϣ
				end
			end
			local creature1 = map.MapCreateCreature(MapID, InstanceID, 1094004, 6, 1, 9)
			ArenaCreature_Timer[creature1]= {TypeID = 5}      --10���ӽ����ʱ
		end
	end

	ArenaCreature_Timer[TargetID] = nil

end



--��10���ӵĹ������ʱ�䣬����ʱ��ʹ��
function c600_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)

    if ArenaCreature_Timer[TargetID]~= nil and ArenaCreature_Timer[TargetID].TypeID == 2 then
	    if Arena[InstanceID] ~= nil and Arena[InstanceID].Battle ~= nil and Arena[InstanceID].Battle == 1 then
			for i=1,6 do
				local Map_ID,Instance_ID = role.GetRoleMapID(Arena[InstanceID].Player[i])
				if Map_ID ~= nil and Instance_ID ~= nil and Map_ID == MapID and Instance_ID == InstanceID then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 206262)          --ս������<color=0xffffff00>1<color=0xfffff7e0>���Ӻ����
					msg.DispatchRoleMsgEvent(Arena[InstanceID].Player[i], MsgID)     --������Ϣ
				end
			end
			local creature1 = map.MapCreateCreature(MapID, InstanceID, 1094002, 6, 1, 9)
			ArenaCreature_Timer[creature1]= {TypeID = 7}      --30������ʱ
		end
	else
	    if Arena[InstanceID] ~= nil and Arena[InstanceID].Battle ~= nil and Arena[InstanceID].Battle == 1 then
		    Arena[InstanceID].Battle = 2

		    ShowArenaFinalFrame(MapID, InstanceID)

		    local creature1 = map.MapCreateCreature(MapID, InstanceID, 1094003, 6, 1, 9)
	        ArenaCreature_Timer[creature1]= {TypeID = 6}      --30�봫�ͼ�ʱ
		end
	end

	ArenaCreature_Timer[TargetID] = nil
end

--ע��
aux.RegisterCreatureEvent(1094001, 13, "c15_OnDisappear")
aux.RegisterCreatureEvent(1094002, 13, "c30_OnDisappear")
aux.RegisterCreatureEvent(1094003, 13, "c60_OnDisappear")
aux.RegisterCreatureEvent(1094004, 13, "c600_OnDisappear")

