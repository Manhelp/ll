--��������ʼʱ�����Ľű�
Arena = {}                    --������¼��������ͼ������Ӧ�ľ������������Լ���������ʱ��״̬�Լ�6����ҵ�ID
ArenaMapNum = 0               --������¼��������ͼ���˶��ٸ�
ArenaPlayer = {}              --������¼���뾺��������ҵ�����
ArenaCreature_Timer = {}      --������¼�������еļ�ʱ���������ڵĸ���ID������ID��
--[[ArenaCreature_Timer[creatureID]= {TimerTypeID}
	   TimerTypeIDö��ֵ
	   1 1���ӵĿ��ż�ʱ��
	   2 10���ӵĵ���ʱ��ʱ��
	   3 30��Ĺ��ɼ�ʱ������15��ģ�
	   4 15������Լ�ʱ��������15��Ϳ��ŵļ�ʱ����
	   5 10���ӵĽ����ʱ�����ڿ��ź�ˢ�����ӿ��أ����û�����򲻻ᴥ��Ч��
	   6 30��Ĵ��ͼ�ʱ��
	   7 30������ս��,�ͷ��ڴ�
	   9 15�뱨ʱ
	   ]]

ArenaMap = {}
ArenaMap[1] = {ArenaMapID = 2092183860,Door1 = 175,Door2 = 174,RedPoint = {x=210, y=207, z=71},BluePoint = {x=42, y=207, z=182},ExitPoint = {x=551, y=459, z=485},BoxPoint = {x=128, y=11330, z=118}}
ArenaMap[2] = {ArenaMapID = 2092184116,Door1 = 491,Door2 = 490,RedPoint = {x=39, y=14, z=169},BluePoint = {x=217, y=14, z=114},ExitPoint = {x=822, y=470, z=609},BoxPoint = {x=129, y=530, z=151}}
ArenaMap[0] = {ArenaMapID = 2092184372,Door1 = 174,Door2 = 175,RedPoint = {x=123, y=16, z=46},BluePoint = {x=260, y=16, z=338},ExitPoint = {x=286, y=470, z=749},BoxPoint = {x=191, y=1280, z=179}}


function Arena_ArenaOnStart(MapID, InstanceID, RoomID, RoleID1, RoleID2, RoleID3, RoleID4, RoleID5, RoleID6)

	--Ϊ�����������ͼ�͸���
	MapID, InstanceID = GetArenaMapID()

    --���������Ӫ
	RoleID1,RoleID2,RoleID3,RoleID4,RoleID5,RoleID6 = PlayerRandom_Arena(RoleID1,RoleID2,RoleID3,RoleID4,RoleID5,RoleID6)

	--Ϊ��������Ϣ����ʼֵ
    DefineValue_Arena (InstanceID, RoomID, RoleID1, RoleID2, RoleID3, RoleID4, RoleID5, RoleID6)

	--����ҵ������;���������Ϣ,����������û�н��뾺���������۵�����
	local Role = {}  --���ID������
	Role[1] = RoleID1
	Role[2] = RoleID2
	Role[3] = RoleID3
	Role[4] = RoleID4
	Role[5] = RoleID5
	Role[6] = RoleID6

	for i=1,6 do
	    local MapID_Arena,InstanceID_Arena = role.GetRoleMapID(Role[i])
		if act.GetActIsStart(31) == true then
			msg.SendRoleSwitchMsg(1, MapID_Arena, InstanceID_Arena, Role[i], 1, 31)
        else
		    msg.SendRoleSwitchMsg(1, MapID_Arena, InstanceID_Arena, Role[i], 1, 32)
		end
		unit.AddBuff(MapID_Arena, InstanceID_Arena, Role[i], 6936001, Role[i])
		role.SetRoleScriptData(Role[i], 1, RoleDataType["Point_Arena"], Lose_Point_Arena(InstanceID, Role[i]))
	end

	--�ھ�������ˢ������
	if math.random(10) == 5 then
		map.MapCreateCreature(MapID, InstanceID, 5512005, ArenaMap[InstanceID%3].BoxPoint.x, ArenaMap[InstanceID%3].BoxPoint.y,ArenaMap[InstanceID%3].BoxPoint.z)
	else
	    map.MapCreateCreature(MapID, InstanceID, 5512004, ArenaMap[InstanceID%3].BoxPoint.x, ArenaMap[InstanceID%3].BoxPoint.y,ArenaMap[InstanceID%3].BoxPoint.z)
	end

	--�ھ�������ˢ����ʱ�õ�͸����
	local creature1 = map.MapCreateCreature(MapID, InstanceID, 1094003, 6, 1, 9)
	local creature2 = map.MapCreateCreature(MapID, InstanceID, 1094002, 6, 1, 9)
	local creature3 = map.MapCreateCreature(MapID, InstanceID, 1094004, 6, 1, 9)
	local creature4 = map.MapCreateCreature(MapID, InstanceID, 1094001, 6, 1, 9)

	ArenaCreature_Timer[creature1]= {TypeID = 1}      --1���ӿ���
	ArenaCreature_Timer[creature2]= {TypeID = 3}      --30���ʱ
	ArenaCreature_Timer[creature3]= {TypeID = 2}      --10���Ӽ�ʱ
	ArenaCreature_Timer[creature4]= {TypeID = 9}      --15��

end

function Arena_OnClick(MapID, InstanceID, RoleID, TOMsgID, ToMapID, x, y, z)

	if ArenaPlayer[RoleID] ~= nil then

		--��ȡ��Ҫ����Ķ�������InstanceID
		local InstanceID_Now = GetNowArenaInstanceID(RoleID)

		if InstanceID_Now ~= 0 then

			--��ȡ��������ʼ��ʱ�������ʱ��
			local Hour 		= tonumber(os.date("%H"))
			local Min  		= tonumber(os.date("%M"))
			local Second	= tonumber(os.date("%S"))

			local Hour_open = math.floor(Arena[InstanceID_Now].OpenTime / 10000)
			local Min_open = math.floor((Arena[InstanceID_Now].OpenTime % 10000) / 100)
			local Second_open = Arena[InstanceID_Now].OpenTime % 100

			--ֻ��1�����ڵ�����뾺����������Ч
			local function ShorterThan1Min()
				if Hour - Hour_open == 1 then
					if Min - Min_open == -59 then
						if Second <= Second_open then
							return true
						end
					end
				elseif Hour == Hour_open then
					if Min - Min_open == 1 then
						if Second <= Second_open then
							return true
						end
					elseif Min == Min_open then
						if Second >= Second_open then
							return true
						end
					end
				else
					return false
				end
			end

			if ShorterThan1Min() and unit.IsDead(MapID, InstanceID, RoleID) ~= true then
				if ArenaPlayer[RoleID] ~= nil and ArenaPlayer[RoleID].Info[InstanceID_Now] ~= nil then
					if ArenaPlayer[RoleID].Info[InstanceID_Now].Side == "red" then
						if role.IsRoleInStatus(RoleID, 1024) then
							role.CancelRoleStallState(MapID, InstanceID, RoleID)
						end
						role.RoleGotoNewMap(MapID, InstanceID, RoleID, ArenaMap[InstanceID_Now%3].ArenaMapID, ArenaMap[InstanceID_Now%3].RedPoint.x, ArenaMap[InstanceID_Now%3].RedPoint.y, ArenaMap[InstanceID_Now%3].RedPoint.z)
						role.SetRoleScriptData(RoleID, 1, RoleDataType["Point_Arena"], 0)
					elseif ArenaPlayer[RoleID].Info[InstanceID_Now].Side == "blue" then
						if role.IsRoleInStatus(RoleID, 1024) then
							role.CancelRoleStallState(MapID, InstanceID, RoleID)
						end
						role.RoleGotoNewMap(MapID, InstanceID, RoleID, ArenaMap[InstanceID_Now%3].ArenaMapID, ArenaMap[InstanceID_Now%3].BluePoint.x, ArenaMap[InstanceID_Now%3].BluePoint.y, ArenaMap[InstanceID_Now%3].BluePoint.z)
						role.SetRoleScriptData(RoleID, 1, RoleDataType["Point_Arena"], 0)
					end
				end
			elseif unit.IsDead(MapID, InstanceID, RoleID) == true then
			    --role.DeletRoleFromArenaTable(RoleID)
				local Pt = Lose_Point_Arena(InstanceID_Now, RoleID)
				--���ø�����Ѿ��뿪�˶�����
				ArenaPlayer[RoleID].Now = 0

				role.ModRoleAttValue(MapID, InstanceID, RoleID, 79, -Pt)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["Point_Arena"], 0)

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71, 206251)   --������������״̬���޷����ͽ��붷��������ʧȥ��X��������ֵ��
				msg.AddMsgEvent(MsgID, 9, Pt)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			else
			    --role.DeletRoleFromArenaTable(RoleID)
				local Pt = Lose_Point_Arena(InstanceID_Now, RoleID)
				--��ʾ����Ѿ��޷����뾺����
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71, 206252)   --�������ı����Ѿ���ʼ�����޷�����˴α�������ʧȥ��X������ֵ��
				msg.AddMsgEvent(MsgID, 9, Pt)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ

				--���ø�����Ѿ��뿪�˶�����
				ArenaPlayer[RoleID].Now = 0

				role.ModRoleAttValue(MapID, InstanceID, RoleID, 79, -Pt)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["Point_Arena"], 0)
			end

		else
		    role.DeletRoleFromArenaTable(RoleID)
			local rongyu = role.GetRoleAttValue(MapID, InstanceID, RoleID, 79)
			local function CountPoint_OnClick(Point)
				if Point <= 100 then
					return 0
				elseif Point <= 250 then
					return 1
				elseif Point <= 500 then
					return 2
				elseif Point <= 750 then
					return 4
				elseif Point > 750 then
					return math.floor((Point - 750)/150) + 6
				else
					return 0
				end
			end
			local Pt = CountPoint_OnClick(rongyu)
			--��ʾ����Ѿ��޷����뾺����
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 206252)   --�������ı����Ѿ���ʼ�����޷�����˴α�������ʧȥ��X������ֵ��
			msg.AddMsgEvent(MsgID, 9, Pt)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ

			role.ModRoleAttValue(MapID, InstanceID, RoleID, 79, -Pt)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["Point_Arena"], 0)
		end
	end
end

--��ҽ��뾺����
function Arena_OnPlayerEnter(MapID, InstanceID, RoleID)

	--Ϊ��һ���Ѫ������ͬʱˢ�����еļ���
	unit.AddBuff(MapID, InstanceID, RoleID, 6936101, RoleID)

	--Ϊ�����ʾ�з���Ϣ
	ShowEnemyInformation(MapID, InstanceID, RoleID, 1)

	--����Ҽ��붷�����Ķ���
	LetPlayerJoinTeam(MapID, InstanceID, RoleID)

	--��ʾ������ʣ��ʱ��
	ShowArenaTime(MapID, InstanceID, RoleID)

	--�����ٶ���ҽ��ж���Ŀ۷�
	role.SetRoleScriptData(RoleID, 1, RoleDataType["Point_Arena"], 0)

	--��¼����
	local curTime = tonumber(os.date("%j"))
	if arena_Roles[RoleID] == nil then
		UpdateSeverData(56, 1)
		arena_Roles[RoleID] = curTime
	elseif arena_Roles[RoleID] ~= curTime then
		arena_Roles = {}
		arena_Roles[RoleID] = curTime
		UpdateSeverData(56, 1)
	end
end

--����ھ�������ɱ��
function Arena_KillPeople(MapID, InstanceID, RoleID, KillerID)
	if ArenaPlayer[RoleID] ~= nil and ArenaPlayer[RoleID].Info[InstanceID] ~= nil and Arena[InstanceID] ~= nil then
	    if unit.IsPlayer(KillerID) then
			if ArenaPlayer[KillerID].Info[InstanceID] ~= nil then

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 206253)           --���Ѿ�������ɱ��������<color=0xffffff00>XXX<color=0xfffff7e0>��
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ

				--��ʾ��ҵ�Xն
				ArenaPlayer[KillerID].Info[InstanceID].Kill = ArenaPlayer[KillerID].Info[InstanceID].Kill + 1
				if ArenaPlayer[KillerID].Info[InstanceID].Kill > 3 then
				    ArenaPlayer[KillerID].Info[InstanceID].Kill = 3
				end
				role.ShowKillerPictureInArena(KillerID, ArenaPlayer[KillerID].Info[InstanceID].Kill)

				--��ʾ��ɱ��ҵ�������ʾ
				for i =1,6 do
					if role.IsRoleOnLine(MapID, InstanceID, Arena[InstanceID].Player[i]) == 1 and ArenaPlayer[Arena[InstanceID].Player[i]] ~= nil and ArenaPlayer[Arena[InstanceID].Player[i]].Info[InstanceID] ~= nil and Arena[InstanceID].Player[i] ~= RoleID then
						if ArenaPlayer[KillerID].Info[InstanceID].Side == "red" then
							if ArenaPlayer[Arena[InstanceID].Player[i]].Info[InstanceID].Side == "red" then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 101, 206254)          --<color=0xffffff00>XXX<color=0xfffff7e0>ɱ����<color=0xffff0000>XXX<color=0xfffff7e0>
								msg.AddMsgEvent(MsgID, 2, KillerID)
								msg.AddMsgEvent(MsgID, 2, RoleID)
								msg.DispatchRoleMsgEvent(Arena[InstanceID].Player[i], MsgID)     --������Ϣ
							else
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 101, 206255)          --���Ķ���<color=0xffff0000>XXX<color=0xfffff7e0>��<color=0xffffff00>XXX<color=0xfffff7e0>ɱ����
								msg.AddMsgEvent(MsgID, 2, RoleID)
								msg.AddMsgEvent(MsgID, 2, KillerID)
								msg.DispatchRoleMsgEvent(Arena[InstanceID].Player[i], MsgID)     --������Ϣ
							end
						else
							if ArenaPlayer[Arena[InstanceID].Player[i]].Info[InstanceID].Side == "red" then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 101, 206255)          --���Ķ���<color=0xffff0000>XXX<color=0xfffff7e0>��<color=0xffffff00>XXX<color=0xfffff7e0>ɱ����
								msg.AddMsgEvent(MsgID, 2, RoleID)
								msg.AddMsgEvent(MsgID, 2, KillerID)
								msg.DispatchRoleMsgEvent(Arena[InstanceID].Player[i], MsgID)     --������Ϣ
							else
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 101, 206254)          --<color=0xffffff00>XXX<color=0xfffff7e0>ɱ����<color=0xffff0000>XXX<color=0xfffff7e0>
								msg.AddMsgEvent(MsgID, 2, KillerID)
								msg.AddMsgEvent(MsgID, 2, RoleID)
								msg.DispatchRoleMsgEvent(Arena[InstanceID].Player[i], MsgID)     --������Ϣ
							end
						end
					end
				end
			end
		end

		--Ϊ�����ʾս��
		ShowEnemyInformation(MapID, InstanceID, RoleID, 2)

		if ArenaPlayer[RoleID] ~= nil and ArenaPlayer[RoleID].Info[InstanceID] ~= nil then
			if ArenaPlayer[RoleID].Info[InstanceID].Side == "red" then
				Arena[InstanceID].RedNum = Arena[InstanceID].RedNum - 1
				if Arena[InstanceID].RedNum <= 0 then
					if Arena[InstanceID].Battle ~= 2 then

						Arena[InstanceID].Battle = 2 --0:δ��ʼ��1����ʼ�ˣ�2��������
						ShowArenaFinalFrame(MapID, InstanceID)

						local creature1 = map.MapCreateCreature(MapID, InstanceID, 1094003, 6, 1, 9)
						ArenaCreature_Timer[creature1]= {TypeID = 6}      --30�봫�ͼ�ʱ
					end
				end
			elseif ArenaPlayer[RoleID].Info[InstanceID].Side == "blue" then
			    Arena[InstanceID].BlueNum = Arena[InstanceID].BlueNum - 1
			    if Arena[InstanceID].BlueNum <= 0 then
					if Arena[InstanceID].Battle ~= 2 then

						Arena[InstanceID].Battle = 2 --0:δ��ʼ��1����ʼ�ˣ�2��������
						ShowArenaFinalFrame(MapID, InstanceID)

						local creature1 = map.MapCreateCreature(MapID, InstanceID, 1094003, 6, 1, 9)
						ArenaCreature_Timer[creature1]= {TypeID = 6}      --30�봫�ͼ�ʱ
					end
				end
			end
		end
	end
end

--����뿪������

function Arena_LeaveMap(MapID, InstanceID, RoleID)

	local TeamID_Old = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	if TeamID_Old ~= nil and TeamID_Old ~= 4294967295 then
		role.LetRoleLeaveTeam(TeamID_Old, RoleID)
	end

	role.DeletRoleFromArenaTable(RoleID)
	unit.UnsetState(MapID, InstanceID, RoleID, 10)
	unit.UnsetState(MapID, InstanceID, RoleID, 11)

	if ArenaPlayer[RoleID] ~= nil and ArenaPlayer[RoleID].Info[InstanceID] ~= nil and Arena[InstanceID] ~= nil then
		--���ս��û�н���
		if Arena[InstanceID].Battle ~= 2 then
		    --role.DeletRoleFromArenaTable(RoleID)
			local TotleTimes_Arena = role.GetRoleScriptData(RoleID, 1, RoleDataType["TotleTimes_Arena"])
			role.SetRoleScriptData(RoleID, 1, RoleDataType["TotleTimes_Arena"], TotleTimes_Arena + 1)
			role.ModRoleAttValue(MapID, InstanceID, RoleID, 79, -Lose_Point_Arena(InstanceID, RoleID))
			ArenaPlayer[RoleID].Now = 0
			role.SetRoleScriptData(RoleID, 1, RoleDataType["Point_Arena"], 0)
			for i=1,6 do
				if Arena[InstanceID].Player[i] ~= RoleID then
				    local Map_ID,Instance_ID = role.GetRoleMapID(Arena[InstanceID].Player[i])
					if Map_ID ~= nil and Instance_ID ~= nil and Map_ID == MapID and Instance_ID == InstanceID and role.IsRoleOnLine(MapID, InstanceID, Arena[InstanceID].Player[i]) == 1 then
						--��ʾ���
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 206256)   --XXX�뿪�˾�����
						msg.AddMsgEvent(MsgID, 2, RoleID)
						msg.DispatchRoleMsgEvent(Arena[InstanceID].Player[i], MsgID)     --������Ϣ
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 206256)   --XXX�뿪�˾�����
						msg.AddMsgEvent(MsgID, 2, RoleID)
						msg.DispatchRoleMsgEvent(Arena[InstanceID].Player[i], MsgID)     --������Ϣ
				    end
				end
			end

			--Ϊ�����ʾս��
			ShowEnemyInformation(MapID, InstanceID, RoleID, 3)

			--���Խ���
			if ArenaPlayer[RoleID].Info[InstanceID].Side == "red" then
			    Arena[InstanceID].RedNum = 0
			    for i = 1,3 do
				    local Map_ID,Instance_ID = role.GetRoleMapID(Arena[InstanceID].Player[i])
					if Map_ID ~= nil and Instance_ID ~= nil and Map_ID == MapID and Instance_ID == InstanceID and role.IsRoleOnLine(MapID, InstanceID, Arena[InstanceID].Player[i]) == 1 and unit.IsDead(MapID, InstanceID, Arena[InstanceID].Player[i]) ~= true and Arena[InstanceID].Player[i] ~= RoleID then
                        Arena[InstanceID].RedNum = Arena[InstanceID].RedNum + 1
					end
				end
				if Arena[InstanceID].RedNum <= 0 then
					if Arena[InstanceID].Battle == 1 then

						Arena[InstanceID].Battle = 2 --0:δ��ʼ��1����ʼ�ˣ�2��������
						ShowArenaFinalFrame(MapID, InstanceID)

						local creature1 = map.MapCreateCreature(MapID, InstanceID, 1094003, 6, 1, 9)
						ArenaCreature_Timer[creature1]= {TypeID = 6}      --30�봫�ͼ�ʱ
					end
				end
			elseif ArenaPlayer[RoleID].Info[InstanceID].Side == "blue" then
			    Arena[InstanceID].BlueNum = 0
				for i = 4,6 do
				    local Map_ID,Instance_ID = role.GetRoleMapID(Arena[InstanceID].Player[i])
					if Map_ID ~= nil and Instance_ID ~= nil and Map_ID == MapID and Instance_ID == InstanceID and role.IsRoleOnLine(MapID, InstanceID, Arena[InstanceID].Player[i]) == 1 and unit.IsDead(MapID, InstanceID, Arena[InstanceID].Player[i]) ~= true and Arena[InstanceID].Player[i] ~= RoleID then
                        Arena[InstanceID].BlueNum = Arena[InstanceID].BlueNum + 1
					end
				end
			    if Arena[InstanceID].BlueNum <= 0 then
					if Arena[InstanceID].Battle == 1 then

						Arena[InstanceID].Battle = 2 --0:δ��ʼ��1����ʼ�ˣ�2��������
						ShowArenaFinalFrame(MapID, InstanceID)

						local creature1 = map.MapCreateCreature(MapID, InstanceID, 1094003, 6, 1, 9)
						ArenaCreature_Timer[creature1]= {TypeID = 6}      --30�봫�ͼ�ʱ
					end
				end
			end
		end
	end
end

--��Ҹ���ű�
function Arena_RoloRevive(MapID, InstanceID, RoleID, Type, ReviveHP, ReviveMP, x, y, z, RebornMapID)

    ReviveHP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 8)
	ReviveMP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 10)
    role.DeletRoleFromArenaTable(RoleID)
	local k = math.random(0,2)
	return ReviveHP, ReviveMP,ArenaMap[k].ExitPoint.x, ArenaMap[k].ExitPoint.y*51,ArenaMap[k].ExitPoint.z, 3424073512

end

--���������뷿��
function JoinArenaRoom(MapID, InstanceID, Type, RoomID, RoleID)
    local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["Point_Arena"])
	if ArenaPlayer[RoleID] ~= nil then
		--��ȡ��Ҫ����Ķ�������InstanceID
		local InstanceID_Now = GetNowArenaInstanceID(RoleID)
		if InstanceID_Now ~= 0 then
		    ArenaPlayer[RoleID].Now = 0
		end
	end
    if k > 0 then
	    role.ModRoleAttValue(MapID, InstanceID, RoleID, 79, -k)
	    role.SetRoleScriptData(RoleID, 1, RoleDataType["Point_Arena"], 0)
	end
	if act.GetActIsStart(31) ~= true and act.GetActIsStart(32) ~= true then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206257)   --������������δ��ʼ���޷�ִ�иò���
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		return false
	elseif MapID == 3424073512 then
		return 1
	else
		return false
	end
end

-- �������ĵ��ҹ�ϵ�ж� -- ��ͬɫ����ȫ�ǵ���
function Arena_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
    local a,b = -1,-1
	-- �������
	if Arena[InstanceID] ~= nil and unit.IsPlayer(SrcID) and unit.IsPlayer(TargetID) then
        for i=1, 6 do
		    if Arena[InstanceID].Player[i] == SrcID then
			    if i <= 3 then
			        a = 1
				    break
				else
				    a = 0
					break
				end
			end
		end

		for i=1, 6 do
		    if Arena[InstanceID].Player[i] == TargetID then
			    if i <= 3 then
			        b = 1
				    break
				else
				    b = 0
					break
				end
			end
		end

		if a ~= b and a~= -1 and b ~= -1 then
			return false, true, false, true
		elseif a == b and a~= -1 then
		    return true, false, false, true
		end
	end

	return false, false, false, false
end



--ע��û������¼�
aux.RegisterMapEvent("a01", 2, "Arena_OnPlayerEnter")
aux.RegisterMapEvent("a02", 2, "Arena_OnPlayerEnter")
aux.RegisterMapEvent("a03", 2, "Arena_OnPlayerEnter")
aux.RegisterActEvent(31, 8, "Arena_OnClick")
aux.RegisterActEvent(32, 8, "Arena_OnClick")
aux.RegisterWorldEvent(1, "Arena_ArenaOnStart")
aux.RegisterMapEvent("a01", 5, "Arena_KillPeople")
aux.RegisterMapEvent("a02", 5, "Arena_KillPeople")
aux.RegisterMapEvent("a03", 5, "Arena_KillPeople")
aux.RegisterMapEvent("a01", 3, "Arena_LeaveMap")
aux.RegisterMapEvent("a02", 3, "Arena_LeaveMap")
aux.RegisterMapEvent("a03", 3, "Arena_LeaveMap")
aux.RegisterMapEvent("a01", 11, "Arena_RoloRevive")
aux.RegisterMapEvent("a02", 11, "Arena_RoloRevive")
aux.RegisterMapEvent("a03", 11, "Arena_RoloRevive")
aux.RegisterRoleEvent(36, "JoinArenaRoom")
aux.RegisterRoleEvent(37, "JoinArenaRoom")
aux.RegisterMapEvent("a01", 16, "Arena_FriendEnemy")
aux.RegisterMapEvent("a02", 16, "Arena_FriendEnemy")
aux.RegisterMapEvent("a03", 16, "Arena_FriendEnemy")
