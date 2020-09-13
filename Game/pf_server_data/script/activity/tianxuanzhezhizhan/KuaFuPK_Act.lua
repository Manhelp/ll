
KuaFuPKchsr = 0
KuaFuPKtalk = false
function KuaFuPKzhe_OnStart(actID)
	local curmon = tonumber(os.date("%m"))--��
	local curday = tonumber(os.date("%d"))--��
	local curyear = tonumber(os.date("%Y"))--��
	for k1,v1 in pairs(GuildNum) do
		guild.SetGuildValue(v1.guildID,0,0)
	end
	if curyear == 2012 and ((curmon == 3 and curday >= 6) or curmon > 3) then
		KuaFuPKtalk = true
		KuaFuPKzhe_baoming = 0
		local enemy = {}
		enemy[4800283] = {guildID = 0,enemyID = 0}
		enemy[4800284] = {guildID = 0,enemyID = 0}
		enemy[4800285] = {guildID = 0,enemyID = 0}
		enemy[4800286] = {guildID = 0,enemyID = 0}
		for k1,v1 in pairs(KuaFuPK_Role) do
			local num1 =role.GetRoleItemNum(v1.RoleID, 4800283)
			local num2 =role.GetRoleItemNum(v1.RoleID, 4800284)
			local num3 =role.GetRoleItemNum(v1.RoleID, 4800285)
			local num4 =role.GetRoleItemNum(v1.RoleID, 4800286)
			if num1 > 0 then
				enemy[4800283].guildID = guild.GetRoleGuildID(v1.RoleID)
				enemy[4800284].enemyID = guild.GetRoleGuildID(v1.RoleID)
			elseif num2 > 0 then
				enemy[4800284].guildID = guild.GetRoleGuildID(v1.RoleID)
				enemy[4800283].enemyID = guild.GetRoleGuildID(v1.RoleID)
			elseif num3 > 0 then
				enemy[4800285].guildID = guild.GetRoleGuildID(v1.RoleID)
				enemy[4800286].enemyID = guild.GetRoleGuildID(v1.RoleID)
			elseif num4 > 0 then
				enemy[4800286].guildID = guild.GetRoleGuildID(v1.RoleID)
				enemy[4800285].enemyID = guild.GetRoleGuildID(v1.RoleID)
			end
		end
		for k1,v1 in pairs(enemy) do
			if v1.guildID ~= 0 and v1.enemyID == 0 then
				guild.SetGuildValue(v1.guildID,1,0)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 100, 211040)  --һ��δ����
				msg.AddMsgEvent(MsgID, 19, v1.guildID)
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
			end
		end
		KuaFuPK_ChooseSide()
		for k,v in pairs(KuaFuPK_Role) do
			local guildID = guild.GetRoleGuildID(v.RoleID)
			if guildID == nil or guildID == 4294967295 or guildID == -1 then
				table.remove(KuaFuPK_Role,k)
			end
			for k1,v1 in pairs(GuildNum) do
				if guildID == v1.guildID then
					v1.side = v.Side
				end
			end
			local Target_MapID, Target_InstanceID = role.GetRoleMapID(k)
			if Target_MapID ~= nil and Target_InstanceID ~= nil then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71, 211039)  --������ʼ
				msg.DispatchRoleMsgEvent(v.RoleID, MsgID)
			end
		end
	end

end

function KuaFuPKzhe_OnTimerMin(actID)
	local curhour = tonumber(os.date("%H"))--Сʱ
	local curmin = tonumber(os.date("%M"))--����
	if curhour == 19 and curmin == 30 then
		local RoleTblIni1 = {}
		local RoleTblIni2 = {}
		RoleTblIni1 = map.GetMapAllRoleID(3373743144, 100)
		RoleTblIni2 = map.GetMapAllRoleID(3373743144, 101)
		if RoleTblIni1 ~= nil then
			for k,v in pairs(RoleTblIni1) do
				unit.CancelBuff(3373743144, 100,v, 6941101)
			end
		end
		if RoleTblIni2 ~= nil then
			for k,v in pairs(RoleTblIni2) do
				unit.CancelBuff(3373743144, 101, v, 6941101)
			end
		end
		for k,v in pairs(KuaFuPK_Role) do
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 211020)    --ս����ʼ�ˣ���ǰ��ɣ�
			msg.DispatchRoleMsgEvent(v.RoleID, MsgID)
		end
	end
	for k,v in pairs(KuaFuPK_Role) do
		if v.die == 1 then
			v.die = 2
		elseif v.die == 2 then
			role.ForcedToRevive(S59_MapID, v.InstanceID, v.RoleID)
			v.die = 0
		end
		local guildID = guild.GetRoleGuildID(v.RoleID)
		if guildID == nil or guildID == 4294967295 or guildID == -1 then
			local Target_MapID, Target_InstanceID = role.GetRoleMapID(v.RoleID)
			role.RoleGotoNewMap(Target_MapID, Target_InstanceID,v.RoleID, 3424073512, 610,23094,669)
		end
	end
	if ((curhour == 19 and curmin >= 30) or (curhour == 20)) and  curmin % 5 == 0 then
		for k1,v1 in pairs(GuildNum) do
			if v1.instanceID == 100 and KuaFuPK_Battle[100] ~= nil and  v1.curNum ~= 0 then
				if v1.side == 1 or v1.side == "Red" then
					v1.score = KuaFuPK_Battle[100].RedPlayer.Point
				else
					v1.score = KuaFuPK_Battle[100].BluePlayer.Point
				end
			elseif v1.instanceID == 101 and KuaFuPK_Battle[101] ~= nil and  v1.curNum ~= 0 then
				if v1.side == 1 or v1.side == "Red" then
					v1.score = KuaFuPK_Battle[101].RedPlayer.Point
				else
					v1.score = KuaFuPK_Battle[101].BluePlayer.Point
				end
			end
		end
		for k1,v1 in pairs(GuildNum) do
			v1.tempnum = v1.lastNum
			v1.lastNum = v1.curNum
			v1.curNum = 0
		end
		for kx,vx in pairs(KuaFuPK_Battle) do
			local curins = vx.instanceID
			for k,v in pairs(KuaFuPK_Role) do
				local guildID = guild.GetRoleGuildID(v.RoleID)
				for k1,v1 in pairs(GuildNum) do
					if guildID ~= nil and guildID ~= 4294967295 and guildID == v1.guildID then
						v1.curNum = v1.curNum + 1
					end
				end
			end
		end
		for k1,v1 in pairs(GuildNum) do
		local tempbaoming = true
			if KuaFuPK_Battle[v1.instanceID] ~= nil then
				if v1.curNum < 15 then
					v1.tag = false
					if v1.tempnum < 15 then
						for k,v in pairs(KuaFuPK_Role) do
							local guildID = guild.GetRoleGuildID(v.RoleID)
							if guildID == nil or guildID == 4294967295 or guildID == v1.guildID then
								local Target_MapID, Target_InstanceID = role.GetRoleMapID(v.RoleID)
								local num1 =role.GetRoleItemNum(v.RoleID, 4800283)
								local num2 =role.GetRoleItemNum(v.RoleID, 4800284)
								local num3 =role.GetRoleItemNum(v.RoleID, 4800285)
								local num4 =role.GetRoleItemNum(v.RoleID, 4800286)
								if num1 > 0 then
									role.RemoveFromRole(Target_MapID, Target_InstanceID, v.RoleID, 4800283, num1, 101)
								end
								if num2 > 0 then
									role.RemoveFromRole(Target_MapID, Target_InstanceID, v.RoleID, 4800284, num2, 101)
								end
								if num3 > 0 then
									role.RemoveFromRole(Target_MapID, Target_InstanceID, v.RoleID, 4800285, num3, 101)
								end
								if num4 > 0 then
									role.RemoveFromRole(Target_MapID, Target_InstanceID, v.RoleID, 4800286, num4, 101)
								end
								role.RoleGotoNewMap(Target_MapID, Target_InstanceID, v.RoleID, 3424073512,610,23094,669)
							end
						end
						v1.curNum = 0
						local tempid = 0
						local tempnum = 0
						local tempt = false
						for k2,v2 in pairs (GuildNum) do
							if v2.guildID ~= v1.guildID and v2.instanceID == v1.instanceID then
								tempid = v2.guildID
								tempnum = v2.curNum
								tempt = true
								v1.score = -1
								if v1.instanceID == 100 and KuaFuPK_Battle[100] ~= nil then
									if v1.side == 1 or v1.side == "Red"  then
										v1.score = KuaFuPK_Battle[100].RedPlayer.Point
										v2.score = KuaFuPK_Battle[100].BluePlayer.Point
									else
										v1.score = KuaFuPK_Battle[100].BluePlayer.Point
										v2.score = KuaFuPK_Battle[100].RedPlayer.Point
									end
								elseif v1.instanceID == 101 and KuaFuPK_Battle[101] ~= nil then
									if v1.side == 1 or v1.side == "Red"  then
										v1.score = KuaFuPK_Battle[101].RedPlayer.Point
										v2.score = KuaFuPK_Battle[101].BluePlayer.Point
									else
										v1.score = KuaFuPK_Battle[101].BluePlayer.Point
										v2.score = KuaFuPK_Battle[101].RedPlayer.Point
									end
								end
								if tempnum >= 15 then
									guild.SetGuildValue(v1.guildID,2,v1.score)
									guild.SetGuildValue(v2.guildID,1,v2.score)
								else
									guild.SetGuildValue(v1.guildID,2,v1.score)
									guild.SetGuildValue(v2.guildID,2,v2.score)
								end
							end
						end
						if tempt == false then
							guild.SetGuildValue(v1.guildID,1,0)
						end
						if KuaFuPK_Battle[v1.instanceID] ~= nil then
							KuaFuPK_Battle[v1.instanceID] = nil
						end
						map.DeleteInstance(S59_MapID, v1.instanceID)
						if tempid == nil or tempid == 0 then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 100, 211040)  --һ��δ����
							msg.AddMsgEvent(MsgID, 19, v1.guildID)
							msg.DispatchBroadcast(MsgID,-1,-1,-1)
							for k,v in pairs(KuaFuPK_Role) do
								if guild.GetRoleGuildID(v.RoleID) == v1.guildID then
									local MsgID1 = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID1, 71, 211047)  --һ��δ����
									msg.DispatchRoleMsgEvent(v.RoleID, MsgID1)
								end
							end
						elseif tempnum < 15 then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 100, 211036)  --˫������������
							msg.AddMsgEvent(MsgID, 19, v1.guildID)
							msg.AddMsgEvent(MsgID, 19, tempid)
							msg.DispatchBroadcast(MsgID,-1,-1,-1)
							for k,v in pairs(KuaFuPK_Role) do
								if guild.GetRoleGuildID(v.RoleID) == v1.guildID or guild.GetRoleGuildID(v.RoleID) == tempid then
									local MsgID1 = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID1, 71, 211046)  --һ��δ����
									msg.DispatchRoleMsgEvent(v.RoleID, MsgID1)
								end
							end
						else
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 100, 211022)  --һ��������������һ����ʤ
							msg.AddMsgEvent(MsgID, 19, v1.guildID)
							msg.AddMsgEvent(MsgID, 19, tempid)
							msg.DispatchBroadcast(MsgID,-1,-1,-1)
							for k,v in pairs(KuaFuPK_Role) do
								if guild.GetRoleGuildID(v.RoleID) == v1.guildID  then
									local MsgID1 = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID1, 71, 211044)  --һ��δ����
									msg.DispatchRoleMsgEvent(v.RoleID, MsgID1)
								elseif guild.GetRoleGuildID(v.RoleID) == tempid then
									local MsgID1 = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID1, 71, 211045)  --һ��δ����
									msg.DispatchRoleMsgEvent(v.RoleID, MsgID1)
								end
							end
						end
					end
				else
					v1.tag = true
				end
			end
		end
	end
    local Min = tonumber(os.date("%M"))
	if curhour == 20 and (Min == 27 or Min == 28 or Min == 29) then
	    for k,v in pairs(KuaFuPK_Battle) do
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 211034)  --��ѡ��֮ս����XX���Ӻ����
			msg.AddMsgEvent(MsgID, 9, 30-Min)
			msg.DispatchMapMsgEvent(MsgID, S59_MapID,k)
		end
	end
	for k,v in pairs(KuaFuPK_Battle) do
		if KuaFuPK_Battle[k] ~= nil then
			local BattleObj = KuaFuPK_Battle[k]
			BattleObj:SetPlayerPoint_KuaFuPK()
			BattleObj:ResetInSidePlayer_KuaFuPK()
		end
	end

end

function KuaFuPKzhe_OnEnd(actID)
	local record = {}
	record[100] = {redguild = 0 ,redpoint = 0 , blueguild = 0 , bluepoint = 0}
	record[101] = {redguild = 0 ,redpoint = 0 , blueguild = 0 , bluepoint = 0}
	local temp  = 0
	for k1,v1 in pairs(GuildNum) do
		if v1.instanceID == 100 and KuaFuPK_Battle[100] ~= nil then
			if v1.side == 1 or v1.side == "Red"  then
				v1.score = KuaFuPK_Battle[100].RedPlayer.Point
				record[100].redguild = v1.guildID
				record[100].redpoint = v1.score
			else
				v1.score = KuaFuPK_Battle[100].BluePlayer.Point
				record[100].blueguild = v1.guildID
				record[100].bluepoint = v1.score
			end
		elseif v1.instanceID == 101 and KuaFuPK_Battle[101] ~= nil then
			if v1.side == 1 or v1.side == "Red"  then
				v1.score = KuaFuPK_Battle[101].RedPlayer.Point
				record[101].redguild = v1.guildID
				record[101].redpoint = v1.score
			else
				v1.score = KuaFuPK_Battle[101].BluePlayer.Point
				record[101].blueguild = v1.guildID
				record[101].bluepoint = v1.score
			end
		end
		temp = guild.GetGuildValue(v1.guildID)
		guild.SetGuildValue(v1.guildID,temp,v1.score)
	end

	local tempMsg = 0
	for k,v in pairs(record) do
		if v.redguild ~= nil and v.redguild ~= 0 and v.blueguild ~= nil and v.blueguild ~= 0 then
			if v.redpoint > v.bluepoint then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 100, 211021)  --�������������һ����ʤ
				msg.AddMsgEvent(MsgID, 19, v.redguild)
				msg.AddMsgEvent(MsgID, 9, v.redpoint)
				msg.AddMsgEvent(MsgID, 19, v.blueguild)
				msg.AddMsgEvent(MsgID, 9, v.bluepoint)
				msg.AddMsgEvent(MsgID, 19, v.redguild)
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
				for k2,v2 in pairs(KuaFuPK_Role) do
					if guild.GetRoleGuildID(v2.RoleID) == v.redguild then
						local MsgID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID1, 71, 211042)  --�������������һ����ʤ
						msg.AddMsgEvent(MsgID1, 9, v.redpoint)
						msg.AddMsgEvent(MsgID1, 9, v.bluepoint)
						msg.DispatchRoleMsgEvent(v2.RoleID, MsgID1)
					elseif guild.GetRoleGuildID(v2.RoleID) == v.blueguild then
						local MsgID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID1, 71, 211043)  --�������������һ����ʤ
						msg.AddMsgEvent(MsgID1, 9, v.bluepoint)
						msg.AddMsgEvent(MsgID1, 9,  v.redpoint)
						msg.DispatchRoleMsgEvent(v2.RoleID, MsgID1)
					end
				end
				tempMsg = 211021
			elseif v.redpoint < v.bluepoint then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 100, 211021)  --���������������һ����ʤ
				msg.AddMsgEvent(MsgID, 19, v.redguild)
				msg.AddMsgEvent(MsgID, 9, v.redpoint)
				msg.AddMsgEvent(MsgID, 19, v.blueguild)
				msg.AddMsgEvent(MsgID, 9, v.bluepoint)
				msg.AddMsgEvent(MsgID, 19, v.blueguild)
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
				for k2,v2 in pairs(KuaFuPK_Role) do
					if guild.GetRoleGuildID(v2.RoleID) == v.redguild then
						local MsgID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID1, 71, 211043)  --�������������һ����ʤ
						msg.AddMsgEvent(MsgID1, 9, v.redpoint)
						msg.AddMsgEvent(MsgID1, 9, v.bluepoint)
						msg.DispatchRoleMsgEvent(v2.RoleID, MsgID1)
					elseif guild.GetRoleGuildID(v2.RoleID) == v.blueguild then
						local MsgID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID1, 71, 211042)  --�������������һ����ʤ
						msg.AddMsgEvent(MsgID1, 9, v.bluepoint)
						msg.AddMsgEvent(MsgID1, 9, v.redpoint)
						msg.DispatchRoleMsgEvent(v2.RoleID, MsgID1)
					end
				end
				tempMsg = 211021
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 100, 211037)  --�������������ƽ��
				msg.AddMsgEvent(MsgID, 19, v.redguild)
				msg.AddMsgEvent(MsgID, 9, v.redpoint)
				msg.AddMsgEvent(MsgID, 19, v.blueguild)
				msg.AddMsgEvent(MsgID, 9, v.bluepoint)
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
				for k2,v2 in pairs(KuaFuPK_Role) do
					if guild.GetRoleGuildID(v2.RoleID) == v.redguild or guild.GetRoleGuildID(v2.RoleID) == v.blueguild then
						local MsgID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID1, 71, 211041)  --�������������ƽ��
						msg.AddMsgEvent(MsgID1, 9, v.redpoint)
						msg.AddMsgEvent(MsgID1, 9, v.bluepoint)
						msg.DispatchRoleMsgEvent(v2.RoleID, MsgID1)
					end
				end
				tempMsg = 211037
			end
		end
	end



	if KuaFuPKchsr ~= nil and KuaFuPKchsr ~= 0 then
		map.MapDeleteCreature(3424073512, -1, KuaFuPKchsr)
	end
    for k,v in pairs(KuaFuPK_Battle) do
		if KuaFuPK_Battle[k] ~= nil then
			--local BattleObj = KuaFuPK_Battle[k]
			--BattleObj:SetPlayerFinalPoint_KuaFuPK()
			map.DeleteInstance(S59_MapID, k)
		end
	end


    --ս�����󣬿��ܿ������ս��
	KuaFuPK_Battle = {}
	--��Ӫ����
	KuaFuPK_Side = {}
	--��Ҷ���
	KuaFuPK_Role = {}
	--ˢ�µ����������
	KuaFuPK_Area = {}
    KuaFuPK_Roletotal = 0
	GuildNum = {}
	KuaFuPKtalk = false
end



--ע��û������¼�
aux.RegisterActEvent(54, 3, "KuaFuPKzhe_OnEnd")
aux.RegisterActEvent(54, 2, "KuaFuPKzhe_OnStart")
aux.RegisterActEvent(54, 4, "KuaFuPKzhe_OnTimerMin")


--������NPC�Ի��ű�
function x5610105_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    local tbl = {}
	tbl[30] = 21000
	tbl[31] = 21600
	tbl[32] = 23000
	tbl[33] = 24600
	tbl[34] = 26100
	tbl[35] = 27700
	tbl[36] = 29300
	tbl[37] = 30900
	tbl[38] = 31600
	tbl[39] = 33000
	tbl[40] = 35600
	tbl[41] = 37500
	tbl[42] = 39600
	tbl[43] = 41700
	tbl[44] = 55300
	tbl[45] = 57900
	tbl[46] = 60500
	tbl[47] = 63000
	tbl[48] = 65600
	tbl[49] = 68400
	tbl[50] = 71300
	tbl[51] = 74100
	tbl[52] = 77000
	tbl[53] = 80100
	tbl[54] = 83300
	tbl[55] = 86400
	tbl[56] = 89500
	tbl[57] = 92700
	tbl[58] = 95800
	tbl[59] = 117500
	tbl[60] = 121500
	tbl[61] = 125600
	tbl[62] = 129600
	tbl[63] = 134000
	tbl[64] = 138000
	tbl[65] = 142800
	tbl[66] = 147200
	tbl[67] = 151600
	tbl[68] = 156000
	tbl[69] = 189300
	tbl[70] = 194800
	tbl[71] = 200400
	tbl[72] = 206000
	tbl[73] = 211900
	tbl[74] = 217900
	tbl[75] = 253400
	tbl[76] = 260100
	tbl[77] = 266900
	tbl[78] = 273600
	tbl[79] = 280800
	tbl[80] = 288000
	tbl[81] = 295200
	tbl[82] = 302400
	tbl[83] = 310100
	tbl[84] = 317700
	tbl[85] = 368800
	tbl[86] = 377400
	tbl[87] = 386100
	tbl[88] = 394800
	tbl[89] = 404000
	tbl[90] = 413100
	tbl[91] = 422300
	tbl[92] = 431500
	tbl[93] = 441200
	tbl[94] = 450900
	tbl[95] = 508000
	tbl[96] = 518700
	tbl[97] = 529400
	tbl[98] = 540000
	tbl[99] = 551300
	tbl[100] = 562500
	local curmon = tonumber(os.date("%m"))--��
	local curday = tonumber(os.date("%d"))--��
	local curyear = tonumber(os.date("%Y"))--��
	local curhour = tonumber(os.date("%H"))--Сʱ
	local curmin = tonumber(os.date("%M"))--����
    --��ȡ��ҵȼ�
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if rolelevel > 100 then
	    rolelevel = 100
	end
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()

		if curyear == 2012 and ((curmon == 2 and curday >= 28) or (curmon == 3 and curday <= 4)) then
			msg.AddMsgEvent(MsgID, 20,	211002)		--���������Ԥѡ�ˣ�\n    ������������Ͼ�Ҫ��ʼ��������׼���ڼ䣬�Ͻ�ȥ�ռ��Ծ�֤֮�ɡ�
			msg.AddMsgEvent(MsgID, 24,	TargetID)
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	211008)		--���ύ�Ծ�֤֮��
			msg.AddMsgEvent(MsgID, 21,	6)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	211006)		--���鿴�ύ������
			msg.AddMsgEvent(MsgID, 21,	7)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	211007)		--���鿴�������
			msg.AddMsgEvent(MsgID, 21,	8)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	211031)		--���鿴�ύ������

		elseif curyear == 2012 and curmon == 3 and curday > 4 then
			msg.AddMsgEvent(MsgID, 20,	211005)		--���������Ԥѡ�ˣ�\n    ����������Ѿ���ʼ����ϸ������ȥ������ѯ��
			msg.AddMsgEvent(MsgID, 24,	TargetID)
			msg.AddMsgEvent(MsgID, 21,	6)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	211006)		--���鿴�ύ������
			msg.AddMsgEvent(MsgID, 21,	7)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	211007)		--���鿴�������
			msg.AddMsgEvent(MsgID, 21,	8)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	211031)		--���鿴�ύ������
		end
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 5 then
		local guildID = guild.GetRoleGuildID(RoleID)
		local num = role.GetRoleItemNum(RoleID, 4800282)
		local guildNu = guild.GetGuildValue(guildID)
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if guildNu == nil then
			guildNu = 0
		end
		if guildID == nil or guildID == -1 or guildID == 4294967295 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211018)		--���������Ԥѡ�ˣ�\n    ��û�й��ᡣ��
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif num > 0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4800282, 1, 420)
			TiJiaoDuijue(guildID)
			guildNu = guild.GetGuildValue(guildID)
			if level >= 30 then
				role.AddRoleExp(MapID, InstanceID, RoleID, tbl[level])
			else
				role.AddRoleExp(MapID, InstanceID, RoleID, level*700)
			end
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211024)		--���������Ԥѡ�ˣ�\n    �ύ�ɹ�����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 9,	guildNu)
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211010)		--���������Ԥѡ�ˣ�\n    ��û�жԾ�֤֮����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 6 then
		local k = nil
		local guildRank = {}
		guildRank[1] = {}
		guildRank[2] = {}
		local r = 0
		for i = 0, 9 do
			local guildID = act.GetActScriptData(54, 1, i)
			if guildID ~= nil and guildID ~= 0 and guildID ~= 4294967295 and guild.IsHaveGuild(guildID) then
				k = i
				guildRank[1][r] = act.GetActScriptData(54, 1, i)
				guildRank[2][r] = act.GetActScriptData(54, 1, 10+i)
				r = r + 1
			end
		end
		if k == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211015) -- "�������Ԥѡ�ˣ�\n    Ŀǰ��û�й����ύ�Ծ�֤֮"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 0 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211011) -- "�������Ԥѡ�ˣ�\n    ĿǰΪֹ��ֻ��һ�������ύ�˶Ծ�֤֮��\n    &lt;p1&gt;  &lt;p2&gt;��
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 19, guildRank[1][0])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][0])
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211012) -- "�������Ԥѡ�ˣ�\n    ĿǰΪֹ��ֻ�����������ύ�˶Ծ�֤֮��\n    &lt;p1&gt;  &lt;p2&gt;��\n    &lt;p3&gt;  &lt;p4&gt;��
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 19, guildRank[1][0])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][0])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][1])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][1])
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211013) -- "�������Ԥѡ�ˣ�\n    ĿǰΪֹ��ֻ�����������ύ�˶Ծ�֤֮��\n    &lt;p1&gt;  &lt;p2&gt;��\n    &lt;p3&gt;  &lt;p4&gt;��\n    &lt;p5&gt;  &lt;p6&gt;��
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 19, guildRank[1][0])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][0])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][1])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][1])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][2])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][2])
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 3 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211014) -- "�������Ԥѡ�ˣ�\n    ĿǰΪֹ��ֻ���ĸ������ύ�˶Ծ�֤֮��\n    &lt;p1&gt;  &lt;p2&gt;��\n    &lt;p3&gt;  &lt;p4&gt;��\n    &lt;p5&gt;  &lt;p6&gt;��\n    &lt;p7&gt;  &lt;p8&gt;��
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 19, guildRank[1][0])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][0])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][1])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][1])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][2])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][2])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][3])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][3])
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k >= 4 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211001) -- "�������Ԥѡ�ˣ�\n    ĿǰΪֹ���ύ�Ծ�֤֮������������ǣ�\n    &lt;p1&gt;  &lt;p2&gt;��\n    &lt;p3&gt;  &lt;p4&gt;��\n    &lt;p5&gt;  &lt;p6&gt;��\n    &lt;p7&gt;  &lt;p8&gt;��\n    &lt;p9&gt;  &lt;p10&gt;��
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 19, guildRank[1][0])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][0])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][1])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][1])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][2])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][2])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][3])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][3])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][4])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][4])
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	elseif TalkIndex == 7 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 211004) -- "�������Ԥѡ�ˣ�\n    ����������Ĳ����������\n    �������ͨ��Ԥѡ��ò����ʸ񣨼��ύ�Ծ�֤֮����������ǰ�壩\n    ��������Ҫ��15��80�����ϵĽ�ɫ��80�����ϵĽ�ɫ���ܽ�����������������\n    ���ݹٷ����ţ���ʱ�������������Ľ�����ϸ���̽����ڹ���������
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	elseif TalkIndex == 8 then
		local guildID = guild.GetRoleGuildID(RoleID)
		local guildNu = guild.GetGuildValue(guildID)
		if guildNu == nil then
			guildNu = 0
		end
		if guildID == nil or guildID == -1 or guildID == 4294967295 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211032)		--���������Ԥѡ�ˣ�\n    ��û�й��ᡣ��
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211033)		--���������Ԥѡ�ˣ�\n    �����ڹ���&lt;p1&gt;�ύ��&lt;p2&gt;���Ծ�֤֮��
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 19,	guildID)
			msg.AddMsgEvent(MsgID, 9,	guildNu)
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end

aux.RegisterCreatureEvent(5610105, 7, "x5610105_OnTalk")


function x5610106_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local curmon = tonumber(os.date("%m"))--��
	local curday = tonumber(os.date("%d"))--��
	local curyear = tonumber(os.date("%Y"))--��
	local curhour = tonumber(os.date("%H"))--Сʱ
	local curmin = tonumber(os.date("%M"))--����
	local guildID = guild.GetRoleGuildID(RoleID)
    --��ȡ��ҵȼ�
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if rolelevel > 100 then
	    rolelevel = 100
	end
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	211019)		--��������Դ����ˣ�\n    ս�����Ͼ�Ҫ��ʼ����׼��������
		msg.AddMsgEvent(MsgID, 24,	TargetID)
		if KuaFuPKtalk == false then
			msg.AddMsgEvent(MsgID, 21,	8)			--��������
			msg.AddMsgEvent(MsgID, 1,	211025)
		else
			msg.AddMsgEvent(MsgID, 21,	9)			--�������Գ���
			msg.AddMsgEvent(MsgID, 1,	211009)		--
		end
		msg.AddMsgEvent(MsgID, 21,	10)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	100001)		--��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 8 then
		if role.GetRoleLevel(MapID, InstanceID, RoleID) < 80 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211027)		--��������Դ����ˣ�\n    ��ȼ�����80�����޷����볡������
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    elseif role.GetRoleItemNum(RoleID, 4800283) < 1 and role.GetRoleItemNum(RoleID, 4800284) < 1 and role.GetRoleItemNum(RoleID, 4800285) < 1 and role.GetRoleItemNum(RoleID, 4800286) < 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211026)		--���������Ԥѡ�ˣ�\n    ��û�жԾ�֮��޷����볡������
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif guildID == nil or guildID == -1 or guildID == 4294967295 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211028)		--������Դ����ˣ�\n    ��û�й��ᣬ�޷����볡��
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if KuaFuPK_Role[RoleID] ~= nil then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	211029)		--������Դ����ˣ�\n    ���Ѿ������ˣ������ظ�������
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				CreatKuaFuPKRoleObj(RoleID)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	211030)		--������Դ����ˣ�\n    ��û�й��ᣬ�޷����볡��
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
        end
	elseif TalkIndex == 9 then
		if role.GetRoleLevel(MapID, InstanceID, RoleID) < 80 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211017)		--��������Դ����ˣ�\n    ��ȼ�����80�����޷����볡������
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    elseif role.GetRoleItemNum(RoleID, 4800283) < 1 and role.GetRoleItemNum(RoleID, 4800284) < 1 and role.GetRoleItemNum(RoleID, 4800285) < 1 and role.GetRoleItemNum(RoleID, 4800286) < 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211016)		--���������Ԥѡ�ˣ�\n    ��û�жԾ�֮��޷����볡������
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif guildID == nil or guildID == -1 or guildID == 4294967295 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211023)		--������Դ����ˣ�\n    ��û�й��ᣬ�޷����볡��
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if Xunlianchang_KuaFu == nil then
				Xunlianchang_KuaFu = {}
			end
			Xunlianchang_KuaFu[RoleID] = 1
			local k = math.random(9)
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, S59_MapID, S59_Point[k].x, S59_Point[k].y, S59_Point[k].z)      --����
        end
	end
end
aux.RegisterCreatureEvent(5610106, 7, "x5610106_OnTalk")

function TiJiaoDuijue(guildID)
    local i = guild.GetGuildValue(guildID)
	local guildNu = guild.GetGuildValue(guildID)
	if i == nil then
	    i = 0
	end
	for q=0,9 do                       --�ݴ����޸�
	    if act.GetActScriptData(54, 1, q) == guildID then
		    if act.GetActScriptData(54, 1, q+10) >= i then
			    i = act.GetActScriptData(54, 1, q+10)
			end
		end
	end
	i = i + 1
	guild.SetGuildValue(guildID,guildNu+1,0)

	local arrID = {}
	local arrNum = {}
	local bNew = 1
	for l=0,9 do
		arrID[l] = act.GetActScriptData(54, 1, l)
		if arrID[l] == nil then
		    arrID[l] = 0
		end
		if arrID[l] == guildID  then
			arrNum[l] = i
			bNew = 0
		else
			arrNum[l] = act.GetActScriptData(54, 1, l+10)
			if arrNum[l] == nil then
		        arrNum[l] = 0
		    end
		end
	end

	if bNew == 1 then
		arrID[10] = guildID;
		arrNum[10] = i;
	else
		arrID[10] = 0;
		arrNum[10] = 0;
	end

	for m=0, 9 do
		for n=m+1,10 do
			if arrNum[n] > arrNum[m] then
				local temp = arrNum[m]
				arrNum[m] = arrNum[n]
				arrNum[n] = temp
				temp = arrID[m]
				arrID[m] = arrID[n]
				arrID[n] = temp
			end
		end
	end

	for j=0,9 do
		act.SetActScriptData(54, 1, j, arrID[j])
		act.SetActScriptData(54, 1, j+10, arrNum[j])
	end

	act.SaveActScriptData(54)

end
