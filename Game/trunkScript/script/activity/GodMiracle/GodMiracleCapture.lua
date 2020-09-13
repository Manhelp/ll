-------------------------------------------------------------------------------
-- Copyright (c) 2004 TENGWU Entertainment All rights reserved.
-- filename: GodMiracleCpatureDefine.h
-- author: chdeng
-- actor:
-- data: 2010-01-25
-- last:
-- brief: ������ṹ����
-------------------------------------------------------------------------------

--������ID
ActiveID = 25


-- \breif: ���е�ͼ��ʼ��ʱ,�����񼣹���״̬����ˢ��ָ�������
function GodMiracleCpature_OnActiveStart(ActiveID)
	if OpenGodMiracleFlag == 0 then
		return
	end
	
	InitGodMiracleOpenFlag()

	InitAllGodMiracle()
end


ResultbroadcastFlag = 0

CalFlag = 0

-- �Ƿ��Ѿ���������־
FinishedFlag = 0

-- \breif : ��ͼ��ʱ���º���
--  param1: �ID
--  param2������ʱ��
--  ramark: ����ʱ����ʱ�����񼣵�״̬
function GodMiracleCpature_OnActiveTimer(ActiveID, Sec)

	if OpenGodMiracleFlag == 0 then
		return
	end

	-- �ڲ���ʱ���²���
	BroadcastGodMsgTick()

	-- �����ʱ���²���
	guild.UpdateGodMiracle()

	---------------------------------------------------------------------------------------------------- ʱ�䵽����ս��
	--���ʱ��������ս��
	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	local Sec   = tonumber(os.date("%S"))


	if Hour == 21 and (Min == 0 or Min == 1) and FinishedFlag == 0 then
		CalFlag = 1
	end

	if  FinishedFlag == 1 and Hour == 21 and Min > 1 then
		FinishedFlag = 0
	end

	-- ��ʱ�䵽30��,��¼һ�λ���
	for k,v in pairs(ClanGodMiracle_CurInfo) do
		---------------------------------------------------------------------------------------------------- ���ɽ�ɢ
		-- ��ս״̬�������Ƿ��ɢ
		local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(ClanGodMiracle_CurInfo[k].MapID))
		if godCurState ==  1 then

			local GuildIDDiss = GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID)
			if GuildIDDiss~= nil and GuildIDDiss ~= 0 then

				-- �鿴ռ���߰����Ƿ��ɢ
				if guild.IsHaveGuild(GuildIDDiss) == false then

					local CaptureGodNum = 0
					-- �鿴������ռ���˶��ٸ���
					for k1, v in pairs(ClanGodMiracle_CurInfo) do
						if GetDefendGuildID(ClanGodMiracle_CurInfo[k1].MapID) == GuildIDDiss then
							CaptureGodNum = CaptureGodNum + 1

							-- �������㲥,�����������
							-- ###�������ƣ��������ѹ����ɱ�������ɽ�������
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 13, 1)
							msg.AddMsgEvent(MsgID, 1, 1409)
							msg.AddMsgEvent(MsgID, 5 , ClanGodMiracle_CurInfo[k1].GdMiracleID)
							msg.DispatchWorldMsgEvent(MsgID)

							-- ���ɽ�ɢ�����ӱ����ɫ
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_Attack_BuffID.God)
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_Attack_BuffID.People)
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_Attack_BuffID.Monster)

							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_Defance_BuffID.God)
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_Defance_BuffID.People)
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_Defance_BuffID.Monster)

							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_Rest_BuffID.God)
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_Rest_BuffID.People)
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_Rest_BuffID.Monster)

							unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_BeforeCap_BuffID.God, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1)
							unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_BeforeCap_BuffID.People, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2)
							unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_BeforeCap_BuffID.Monster, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3)

							-- ���C++��Lua��������
							guild.InitGodMiracle(k1-1)
							ClearGodMiracle(k1)

							-- ������Ϊδռ��״̬
							guild.SetGodCurState(k1-1, 0)


						end
					end

					-- ���ɽ�ɢ,�Ƴ�����������ҵ�����BUFF
					-- �õ����ɵ����г�Ա
					--[[local tmpRoleIDTab = guild.GetAllGuildMember(GuildIDDiss)
					if tmpRoleIDTab ~= nil then
						for k, v in pairs(tmpRoleIDTab) do
							if (RoleTable[k] ~= nil) then
								unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, tmpRoleIDTab[k], GetBuffIDbyGodMiracleNum(CaptureGodNum))
							end
						end
					end--]]

--					guild.AddBuffToAllGuildMember(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), GetBuffIDbyGodMiracleNum(CaptureGodNum))
				end
			end
		end

		---------------------------------------------------------------------------------------------------- ���ս���Ƿ����
		-- ������״̬��������
		local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(ClanGodMiracle_CurInfo[k].MapID))
		if godCurState == 2  then

			--BUG
			--if GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID) ~= 0 then

				------------------------------------------------------------------------------------------------- 30����˫������
				if CalFlag == 0 then
					ModTick(GetCurInfoMapID(k), 1)
					if  Gettick(GetCurInfoMapID(k)) == GodEnum.StdScoresTick then
						-- �ط�
						local pillarNum = GuildCapturePillar(GetCurInfoMapID(k), GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))
						if pillarNum > 0 then
							ModScores(GetCurInfoMapID(k), GodEnum.Defend, pillarNum)
							guild.SetGodGuildPoint(k-1, GodEnum.Defend, pillarNum)
						end

						-- ����
						local pillarNum1 = GuildCapturePillar(GetCurInfoMapID(k), GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID))
						if pillarNum1 > 0 then
							ModScores(GetCurInfoMapID(k), GodEnum.Capture, pillarNum1)
							guild.SetGodGuildPoint(k-1,  GodEnum.Capture, pillarNum1)
						end
						SetTick(ClanGodMiracle_CurInfo[k].MapID, 0)
					end
				end

				if CalFlag == 1  then
					--  ���������⴦��
					if GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID) == 0 then

						-- ��¼ս�����
						Capture_Result[k].WinOrLoseFlag = 0
						Capture_Result[k].DefendGuild   = GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID)
						Capture_Result[k].CaptureGuild  = 0

						-- ���ô��ڿ���״̬
						guild.SetGodCurState(k-1, 1)
						if GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID) == 0 then
							guild.SetGodCurState(k-1, 0)
						end

						-- ˫�����ɽ��ս��״̬
						guild.UnsetGuildStatus(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)
						guild.UnsetGuildStatus(GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)

						aux.WriteLog("begin:Cal(Capture==0 ):CalCaptureContributionAndExploit")
						--��㹱�׺͹�ѫ����
						guild.CalCaptureContributionAndExploit(k-1, GodEnum.Defend)
						aux.WriteLog("end:Cal(Capture==0 ):CalCaptureContributionAndExploit")

						-- ��ס��,������κ�BUFF
						-- �����ɳɹ��ĺ������Լ����񼣣�
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 1430)
						msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
						msg.DispatchGuildMsgEvent(MsgID, GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))

						-- �ط�ͬ��
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 1430)
						msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
						msg.DispatchAllianceGuildMsgEvent(MsgID, 1, k-1)

						--ResetGodMiracle(k)

					elseif GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID) ~= 0 then

						if  GetScores(GetCurInfoMapID(k), GodEnum.Defend) >= GetScores(GetCurInfoMapID(k), GodEnum.Capture) then -- �ط�ʤ

							-- ��¼ս�����
							Capture_Result[k].WinOrLoseFlag = 1
							Capture_Result[k].DefendGuild   = GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID)
							Capture_Result[k].CaptureGuild  = GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID)

							-- ���ô��ڿ���״̬
							guild.SetGodCurState(k-1, 1)
							aux.WriteLog("defend win: SetGodCurState(k-1, 1)\r\n")

							-- ˫�����ɽ��ս��״̬
							guild.UnsetGuildStatus(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)
							guild.UnsetGuildStatus(GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)
							aux.WriteLog("defend win: UnsetGuildStatus\r\n")

							--��㹱�׺͹�ѫ����
							aux.WriteLog("begin:Cal(Defend Win):CalCaptureContributionAndExploit\r\n")
							guild.CalCaptureContributionAndExploit(k-1, GodEnum.Defend)
							aux.WriteLog("end:Cal(Defend Win):CalCaptureContributionAndExploit\r\n")

							-- ��ס��,������κ�BUFF
							-- �����ɳɹ��ĺ������Լ����񼣣�
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 13, 1)
							msg.AddMsgEvent(MsgID, 1, 1430)
							msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
							msg.DispatchGuildMsgEvent(MsgID, GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))

							-- �ط�ͬ��
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 13, 1)
							msg.AddMsgEvent(MsgID, 1, 1430)
							msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
							msg.DispatchAllianceGuildMsgEvent(MsgID, 1, k-1)

							--ResetGodMiracle(k)

						elseif GetScores(GetCurInfoMapID(k), GodEnum.Defend) < GetScores(GetCurInfoMapID(k), GodEnum.Capture)  then -- ����ʤ

							-- ��¼ս�����
							Capture_Result[k].WinOrLoseFlag = 2
							Capture_Result[k].DefendGuild   = GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID)
							Capture_Result[k].CaptureGuild  = GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID)

							-- ���ô��ڿ���״̬
							guild.SetGodCurState(k-1, 1)
							aux.WriteLog("capture win: SetGodCurState(k-1, 1)\r\n")

							-- ˫�����ɽ��ս��״̬
							guild.UnsetGuildStatus(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)
							guild.UnsetGuildStatus(GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)
							aux.WriteLog("capture win: UnsetGuildStatus\r\n")

							--��㹱�׺͹�ѫ����
							aux.WriteLog("begin:Cal(Capture Win):CalCaptureContributionAndExploit\r\n")
							guild.CalCaptureContributionAndExploit(k-1, GodEnum.Capture)
							aux.WriteLog("end:Cal(Capture Win):CalCaptureContributionAndExploit\r\n")

							-- ��ʼ����
							SetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID, GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID))
							SetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID, 0)

							-- ����ͨ�������������ˡ����񼣵�ռ��Ȩ�����г�Ա�������״̬��ͬʱ��ҿ�ǰ���񼣹���������Ʒ
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 102, 100055)
							msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
							msg.DispatchGuildMsgEvent(MsgID, GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))

							-- ����ͬ��
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 102, 100055)
							msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
							msg.DispatchAllianceGuildMsgEvent(MsgID, 2, k-1)

							--ResetGodMiracle(k)

						end
					end

					-- �����ķ�ʤ,����������Ϊ��ɫ
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_Attack_BuffID.God)
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_Attack_BuffID.People)
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_Attack_BuffID.Monster)

					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_Defance_BuffID.God)
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_Defance_BuffID.People)
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_Defance_BuffID.Monster)

					-- ȡ�����ս��״̬
					--	ES_CSAttack		=	8,	// ��ս������Ӫ
					--	ES_CSDefence	=	9,	// ��ս������Ӫ
					--	EGCT_Defend	= 1,	// ����
					--	EGCT_Capture= 2,	// ����
					guild.UnsetStateOfAllRoleInBattleField(1, 9, GetGodMiracleIDMinOne(ClanGodMiracle_CurInfo[k].MapID))
					guild.UnsetStateOfAllRoleInBattleField(2, 8, GetGodMiracleIDMinOne(ClanGodMiracle_CurInfo[k].MapID))

					if GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID) ~= 0  then

						unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_BeforeCap_BuffID.God)
						unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_BeforeCap_BuffID.People)
						unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_BeforeCap_BuffID.Monster)

						unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_Rest_BuffID.God, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1)
						unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_Rest_BuffID.People, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2)
						unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_Rest_BuffID.Monster, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3)

					end

					aux.WriteLog("Setting Pillar corlor\r\n")

					ResetGodMiracle(k)

					ClearBroaCastGlag()
					ResultbroadcastFlag = 1

				end
			--end
		end
	end

	if ResultbroadcastFlag == 1 then
		aux.WriteLog("Broadcast cal result!\r\n")
		CalFlag = 0
		FinishedFlag	= 1
		BroadcastCaptureResult()
		ResultbroadcastFlag  = 0
		EnterFlag = 0

	end
end

-- \breif : �ÿ����ˢ��
--  param1: �ID
function GodMiracleCpature_OnActiveTimeMin(ActiveID)

	if OpenGodMiracleFlag == 0 then
		return
	end
	--  ����������,��¼��Ծʱ��
	guild.UpdateGodMiracleMin()

	BroadcastGodMsgMin()
end


-- \breif : ��ұ�ɱ
--  param1:
function GodMiracleCpature_OnRoleDie(MapID, InstanceID, BeKilledID, KillerID)

	if OpenGodMiracleFlag == 0 then
		return
	end

	-- ��ͼ�����ж�
	if IsInGodAreaMap(MapID) == 0 then
		return
	end

	-- ������״̬��������
	local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
	if godCurState ~= 2  and GetCaptureGuildID(MapID) ~= 0 then
		return
	end

	-- �õ�ɱ���߰���ID
	local killerGuildID = guild.GetRoleGuildID(KillerID)
	if killerGuildID ~= 4294967295 then
		local DOrCFlag = GetRoleGuildDefendOrCapture(MapID, killerGuildID)
		if DOrCFlag ~= GodEnum.Null then
			guild.SetGuildRoleDie(GetGodMiracleIDMinOne(MapID), KillerID, BeKilledID, DOrCFlag)
		end
	end

end



local EnterFlag = 0

-- \breif : ��ҽ�����
--  param3: ����ű���������ID
--  param4: ����Ľű�����ID
function GodMiracleCpature_OnRoleEnter(MapID, InstanceID, OwnerID)--, ObjAreaID

	if OpenGodMiracleFlag == 0 then
		return
	end

	-- ��ͼ�����ж�
	if IsInGodAreaMap(MapID) == 0 then
		return
	end

	-- ������״̬��������
	local WeekDay = os.date("%A")
	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
	if godCurState ~= 2 or GetCaptureGuildID(MapID) == 0 then
		if godCurState == 1 and WeekDay=="Saturday" and Hour == 19 and Min >= 50 then

		else
			return
		end
	end

	-- ��ҽ���ս��Ҫ�뿪����
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, OwnerID)
	if TeamID ~= nil then
		role.LetRoleLeaveTeam(TeamID, OwnerID)
	end

	-- ������ҽ����������ϲ㴴�����񼣽����������־
	-- ��C++�㴴����Ҳ�������ҿ��Լ�¼��Ծʱ��
	local guildID = guild.GetRoleGuildID(OwnerID)
	if guildID ~= 4294967295 then
		--ȡ������Ӫ���ͣ�1��ʾ���أ�2��ʾ������0��ʾû�в�սȨ
		local DOrCFlag = GetRoleGuildDefendOrCapture(MapID, guildID)
		if DOrCFlag ~= GodEnum.Null then

			-- ���ý�ɫ�����񼣱�־
			guild.GuildRoleEnterGodArea(GetGodMiracleIDMinOne(MapID), OwnerID, DOrCFlag)

			-- ���û�Ծʱ��
			guild.SetRoleStartActiveTimes(GetGodMiracleIDMinOne(MapID), OwnerID, DOrCFlag, 1)

			--	ES_CSAttack		=	8,	// ��ս������Ӫ
			--	ES_CSDefence	=	9,	// ��ս������Ӫ
			if DOrCFlag == GodEnum.Defend then
				unit.UnsetState(MapID, 0, OwnerID, 8)
				unit.SetState(MapID, 0, OwnerID, 9)
			elseif DOrCFlag == GodEnum.Capture then
				unit.UnsetState(MapID, 0, OwnerID, 9)
				unit.SetState(MapID, 0, OwnerID, 8)
			end

			if EnterFlag == 0 then

				--  ����������й㲥
				-- ���İ������ڲ����������ս�������ж��ѱ�Ϊ����սģʽ��
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1416)
				msg.DispatchRoleMsgEvent(OwnerID, MsgID)

				-- �ر���������
				role.UnSetRoleState(OwnerID, 1, 1)

				EnterFlag =  1

			end

			-- ֪ͨ�ͻ�����������������
			SyncFlagAttach(MapID, OwnerID)
		end
	end

end

function GetGuildCapType(NpcType, MapID)
	local CapGuildID = GetCapGodStacke(MapID, NpcType)
	local DefendGuildID = ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendGuild;
	local CaptureGuildID = ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureGuild;
	if CapGuildID == DefendGuildID then
		return 1
	elseif CapGuildID == CaptureGuildID then
		return 2
	else
		return 0
	end
end

function SyncFlagAttach(MapID, OwnerID)
	guild.SyncFlagAttack(GetGodMiracleIDMinOne(MapID), 0, GetGuildCapType(1, MapID), OwnerID, 0, 0, ClanGodMiracle_Proto[MapID].PosX1, ClanGodMiracle_Proto[MapID].PosY1, ClanGodMiracle_Proto[MapID].PosZ1)	-- ��
	guild.SyncFlagAttack(GetGodMiracleIDMinOne(MapID), 1, GetGuildCapType(3, MapID), OwnerID, 0, 0, ClanGodMiracle_Proto[MapID].PosX2, ClanGodMiracle_Proto[MapID].PosY2, ClanGodMiracle_Proto[MapID].PosZ2)	-- ��
	guild.SyncFlagAttack(GetGodMiracleIDMinOne(MapID), 2, GetGuildCapType(2, MapID), OwnerID, 0, 0, ClanGodMiracle_Proto[MapID].PosX3, ClanGodMiracle_Proto[MapID].PosY3, ClanGodMiracle_Proto[MapID].PosZ3)	-- ħ
end

-- \breif : ����뿪������
--  param3: �뿪�ű���������ID
--  param4: �뿪�Ľű�����ID
function GodMiracleCpature_OnRoleLeave(MapID, InstanceID, OwnerID)--, ObjAreaID

	if OpenGodMiracleFlag == 0 then
		return
	end

	-- ��ͼ�����ж�
	if IsInGodAreaMap(MapID) == 0 then
		return
	end

	-- ������״̬��������
	local WeekDay = os.date("%A")
	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
	if godCurState ~= 2 or GetCaptureGuildID(MapID) == 0 then
		if godCurState == 1 and WeekDay=="Saturday" and Hour == 19 and Min >= 50 then

		else
			return
		end
	end

	-- ��������뿪������
	local guildID = guild.GetRoleGuildID(OwnerID)
	if guildID ~= 4294967295 then

		local DOrCFlag = GetRoleGuildDefendOrCapture(MapID, guildID)
		if DOrCFlag ~= GodEnum.Null then

			guild.SetRoleStartActiveTimes(GetGodMiracleIDMinOne(MapID), OwnerID, DOrCFlag, 0)

			-- ȡ�����ս��״̬
			--	ES_CSAttack		=	8,	// ��ս������Ӫ
			--	ES_CSDefence	=	9,	// ��ս������Ӫ
			unit.UnsetState(MapID, 0, OwnerID, 8)
			unit.UnsetState(MapID, 0, OwnerID, 9)

			-- ����Ϊ����ģʽ
			local ERS_PeaceModel = 8388608
			role.SetRoleState(ERS_PeaceModel);

			--�����뿪�����������ս�ѽ����������ж��ѱ�Ϊͨ��ģʽ��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1417)
			msg.DispatchRoleMsgEvent(OwnerID, MsgID)--]]

			--֪ͨ�ͻ���
			guild.RoleLeaveArea(GetGodMiracleIDMinOne(MapID), OwnerID, GetRoleGuildDefendOrCapture(MapID, guildID))

			EnterFlag = 0
		end
	end
end

-- \breif : ���ָ�������Ƿ��ܱ��ͷ�
--  param3: SkillID(����ID)
--  param4: �ͷż�����ID
--  param5: Ŀ��ID
--  return: �����ʾ��Ϣ�Խű���Ϣ�����ͻ���,������ֻҪ������0��Ϊ����,c++����������
function GodMiracleCpature_CanCast(MapID, InstanceID, SkillID, OwnerID, TargetID)

	--ʹ���ֻؼ���
	if SkillID == 10508 then
		-- ��ͼ�����ж�
		if IsInGodAreaMap(MapID) == 0 then
			if MapID == 2092183860 or MapID == 2092184116 or MapID == 2092184372 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26,	206282)		--���������޷�ʹ�ø����
				msg.DispatchRoleMsgEvent(OwnerID, MsgID)
				return 999
			end
			return 0
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID,26, 1439)
			msg.DispatchRoleMsgEvent(OwnerID, MsgID)
			return 999
		end

	else
		-- ��ͼ�����ж�
		if IsInGodAreaMap(MapID) == 0 then
			return
		end

		-- ���ڿ���״̬,��������
		local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
		if godCurState  == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID,26, 1427)
			msg.DispatchRoleMsgEvent(OwnerID, MsgID)
			return 999
		end

		-- ��ȡ����ID(eroor:�ӿں���Ӧ����û�а��ɵ�����·���0��Ӧ�÷������޴���ֵ)
		local GuildID = guild.GetRoleGuildID(OwnerID);
		-- ��ȡ��Ұ���ְλ(GuildPos==nil ����ɲ�����)
		local GuildPos = -1
		GuildPos = guild.GetRoleGuildPos(GuildID , OwnerID);
		if GuildPos == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID,26, 1419)
			msg.DispatchRoleMsgEvent(OwnerID, MsgID)
			return 999
		end

		-- ���û�п�������״̬,���ɿ���
		-- ����δ��������״̬���޷����п����񼣲����� and role.IsRoleInStatus(OwnerID, 1048576) == false,1048576��������������ģʽ2��ֻ�ܹ����������
		if role.IsRoleInStatus(OwnerID, 4) == false then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1403)
			msg.DispatchRoleMsgEvent(OwnerID, MsgID)
			return 999
		end

		if godCurState  == 2 and GetDefendGuildID(MapID) ~= 0 then
			-- ��Ҳ���������ɷ�Χ!
			if GetDefendGuildID(MapID) ~= GuildID and GetCaptureGuildID(MapID) ~= GuildID then

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1429)
				msg.DispatchRoleMsgEvent(OwnerID, MsgID)

				return 999
			end
		end

		-- δ��ս���������ᣬ���ÿ���
		if godCurState  == 2 and GetDefendGuildID(MapID) ~= 0 and GetCaptureGuildID(MapID) == 0 then
			return 999
		end

		-- ���������ڱ����ɣ����ÿ���
		local TargetTypeID = unit.GetUnitTypeID(MapID, InstanceID, TargetID)
		local NpcIndex = GetGodStackeIndex(MapID, TargetTypeID)
		if NpcIndex == GodEnum.Null then return 999 end
		local guildID1 = GetCapGodStacke(MapID, NpcIndex)
		if guildID1 ~= 0 and guildID1 == GuildID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1425)
			msg.DispatchRoleMsgEvent(OwnerID, MsgID)
			return 999
		end

		--�ȼ��������ɿ���
		local OwnerLevel = role.GetRoleLevel(MapID, InstanceID, OwnerID)
		if OwnerLevel < 20 then
			return
		end

		if TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj1 then
			-- ���ӿ���Ч��
			unit.AddBuff(MapID, 0, TargetID, Flag_Opening_BuffID.God, TargetID)
		elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj2 then
			-- ���ӿ���Ч��
			unit.AddBuff(MapID, 0, TargetID, Flag_Opening_BuffID.People, TargetID)
		elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj3 then
			-- ���ӿ���Ч��
			unit.AddBuff(MapID, 0, TargetID, Flag_Opening_BuffID.Monster, TargetID)
		end
	end
end

-- \breif : �����ͷ�����Ч��
--  param3: SkillID(����ID)
--  param4: �ͷż�����ID
--  return: 1:֪ͨ�ϲ㲻Ҫ���ô˼���Ч�� 0:���෴
function GodMiracleCpature_CastSkill(MapID, InstanceID, SkillID, OwnerID, TargetID, TargetTypeID)

	-- ��ͼ�����ж�
	if IsInGodAreaMap(MapID) == 0 then
		return
	end

	-- �õ�ռ�����Ǹ�����
	local GodStackeIndex = GetGodStackeIndex(MapID, TargetTypeID)
	if GodStackeIndex == GodEnum.Null then return 1 end

	-- ��Ұ��ɽ�ɢ,���ɿ���
	local guildID = 0
	guildID = guild.GetRoleGuildID(OwnerID)
	local GuildPos
	GuildPos  = guild.GetRoleGuildPos(guildID, OwnerID);
	if GuildPos == nil then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1404)
		msg.DispatchRoleMsgEvent(OwnerID, MsgID)
		return 1
	end


	-- ���û�п�������״̬,���ɿ���
	if role.IsRoleInStatus(OwnerID, 1) == true then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1406)
		msg.DispatchRoleMsgEvent(OwnerID, MsgID)
		return 1
	end
	-- and role.IsRoleInStatus(OwnerID, 1048576) == false, 1048576��������������ģʽ2��ֻ�ܹ����������
	if role.IsRoleInStatus(OwnerID, 4) == false then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1406)
		msg.DispatchRoleMsgEvent(OwnerID, MsgID)
		return 1
	end

	-- ���������ڱ����ɣ����ÿ���
	local guildID1 = GetCapGodStacke(MapID, TargetTypeID)
	if guildID1 ~= 0 and guildID1  == guildID then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1415)
		msg.DispatchRoleMsgEvent(OwnerID, MsgID)
		return 1
	end

	--�ȼ��������ɿ���
	local OwnerLevel = role.GetRoleLevel(MapID, InstanceID, OwnerID)
	if OwnerLevel < 20 then
		return
	end


	-- ״̬0��1�����������һ��
	local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
	if godCurState  == 0 or (godCurState == 2 and GetDefendGuildID(MapID) == 0 ) then
		-- ���ð��ɻ�ȡ����
		SetCapGodStacke(MapID, GodStackeIndex, guildID)

		if TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj1 then
			-- ȡ������Ч��BUFF
			unit.CancelBuff(MapID, 0, TargetID, Flag_Opening_BuffID.God)

			-- �Ƴ���ɫЧ��
			unit.CancelBuff(MapID, 0, TargetID, Flag_BeforeCap_BuffID.God)

			-- �����ǹ���Ч��
			unit.AddBuff(MapID, 0, TargetID, Flag_Attack_BuffID.God, TargetID)

		elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj2 then
			-- ȡ������Ч��BUFF
			unit.CancelBuff(MapID, 0, TargetID, Flag_Opening_BuffID.People)

			-- �Ƴ���ɫЧ��
			unit.CancelBuff(MapID, 0, TargetID, Flag_BeforeCap_BuffID.People)

			-- �����ǹ���Ч��
			unit.AddBuff(MapID, 0, TargetID, Flag_Attack_BuffID.People, TargetID)

		elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj3 then
			-- ȡ������Ч��BUFF
			unit.CancelBuff(MapID, 0, TargetID, Flag_Opening_BuffID.Monster)

			-- �Ƴ���ɫЧ��
			unit.CancelBuff(MapID, 0, TargetID, Flag_BeforeCap_BuffID.Monster)

			-- �����ǹ���Ч��
			unit.AddBuff(MapID, 0, TargetID, Flag_Attack_BuffID.Monster, TargetID)

		end

		-- ������Χ�ڹ㲥
		-- �����������������������ƣ����ɵġ���������������������ƣ��ѿ����ˡ������������������ƣ���
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100066)
		msg.AddMsgEvent(MsgID, 19, guildID)
		msg.AddMsgEvent(MsgID, 2, OwnerID)
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.DispatchWorldMsgEvent(MsgID)

		local piller = GuildCapturePillar(MapID, guildID)
		if piller == 2 then
			-- ����c++��ռ�����
			guild.SetCaptureGodGuildID(GetGodMiracleIDMinOne(MapID), guildID)


			if godCurState == 0 then
				-- ���ô��ڿ���״̬
				guild.SetGodCurState(GetGodMiracleIDMinOne(MapID), 1)
			end

			ClearGodMiracle(ClanGodMiracle_Proto[MapID].ClanGodMiracleID)

			--����lua�˰���ռ����
			SetDefendGuildID(MapID, guildID)

			-- �����ڴ�����㲥
			-- ����ͨ�������������ˡ����񼣵�ռ��Ȩ�����г�Ա�������״̬��ͬʱ��ҿ�ǰ���񼣹���������Ʒ��
			local MsgID2 = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID2, 102, 100055)
			msg.AddMsgEvent(MsgID2, 5, ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].GdMiracleID)
			msg.DispatchGuildMsgEvent(MsgID2, guildID)

			-- ��һ��ռ���񼣲��������״̬,��������
			unit.CancelBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj1, Flag_BeforeCap_BuffID.God)
			unit.CancelBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj2, Flag_BeforeCap_BuffID.People)
			unit.CancelBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj3, Flag_BeforeCap_BuffID.Monster)

			unit.CancelBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj1, Flag_Attack_BuffID.God)
			unit.CancelBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj2, Flag_Attack_BuffID.People)
			unit.CancelBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj3, Flag_Attack_BuffID.Monster)

			unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj1, Flag_Rest_BuffID.God, ClanGodMiracle_Proto[MapID].NPCIDObj1)
			unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj2, Flag_Rest_BuffID.People, ClanGodMiracle_Proto[MapID].NPCIDObj2)
			unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj3, Flag_Rest_BuffID.Monster, ClanGodMiracle_Proto[MapID].NPCIDObj3)

			ResetGodMiracle(ClanGodMiracle_Proto[MapID].ClanGodMiracleID)

		end

	elseif godCurState == 2 then

		-- ����ʱ�䵽,���ɿ���
		local Hour = tonumber(os.date("%H"))
		if Hour == 21 then
			return
		end

		-- ���ð��ɻ�ȡ����
		SetCapGodStacke(MapID, GodStackeIndex, guildID)

		-- ������Χ�ڹ㲥
		-- �����������������������ƣ����ɵġ���������������������ƣ��ѿ����ˡ������������������ƣ���
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100066)
		msg.AddMsgEvent(MsgID, 19, guildID)
		msg.AddMsgEvent(MsgID, 2, OwnerID)
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.DispatchGuildMsgEvent(MsgID, GetCaptureGuildID(MapID))

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100066)
		msg.AddMsgEvent(MsgID, 19, guildID)
		msg.AddMsgEvent(MsgID, 2, OwnerID)
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.DispatchGuildMsgEvent(MsgID, GetDefendGuildID(MapID))

		-- ����ͬ��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100066)
		msg.AddMsgEvent(MsgID, 19, guildID)
		msg.AddMsgEvent(MsgID, 2, OwnerID)
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.DispatchAllianceGuildMsgEvent(MsgID, 2, GetGodMiracleIDMinOne(MapID))

		-- �ط�ͬ��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100066)
		msg.AddMsgEvent(MsgID, 19, guildID)
		msg.AddMsgEvent(MsgID, 2, OwnerID)
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.DispatchAllianceGuildMsgEvent(MsgID, 1, GetGodMiracleIDMinOne(MapID))

		if TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj1 then
			-- ȡ������Ч��BUFF
			unit.CancelBuff(MapID, 0, TargetID, Flag_Opening_BuffID.God)
		elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj2 then
			-- ȡ������Ч��BUFF
			unit.CancelBuff(MapID, 0, TargetID, Flag_Opening_BuffID.People)
		elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj3 then
			-- ȡ������Ч��BUFF
			unit.CancelBuff(MapID, 0, TargetID, Flag_Opening_BuffID.Monster)
		end

		-- ���ݹ������ط��仯���ӵ���ɫ
		if guildID ==GetDefendGuildID(MapID) then

			if TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj1 then
				unit.CancelBuff(MapID, 0, TargetID, Flag_BeforeCap_BuffID.God)
				unit.CancelBuff(MapID, 0, TargetID, Flag_Rest_BuffID.God)

				unit.CancelBuff(MapID, 0, TargetID, Flag_Attack_BuffID.God)
				unit.AddBuff(MapID, 0, TargetID, Flag_Defance_BuffID.God, TargetID)
			elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj2 then
				unit.CancelBuff(MapID, 0, TargetID, Flag_BeforeCap_BuffID.People)
				unit.CancelBuff(MapID, 0, TargetID, Flag_Rest_BuffID.People)

				unit.CancelBuff(MapID, 0, TargetID, Flag_Attack_BuffID.People)
				unit.AddBuff(MapID, 0, TargetID, Flag_Defance_BuffID.People, TargetID)
			elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj3 then
				unit.CancelBuff(MapID, 0, TargetID, Flag_BeforeCap_BuffID.Monster)
				unit.CancelBuff(MapID, 0, TargetID, Flag_Rest_BuffID.Monster)

				unit.CancelBuff(MapID, 0, TargetID, Flag_Attack_BuffID.Monster)
				unit.AddBuff(MapID, 0, TargetID, Flag_Defance_BuffID.Monster, TargetID)
			end

		elseif guildID == GetCaptureGuildID(MapID) then

			if TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj1 then
				unit.CancelBuff(MapID, 0, TargetID, Flag_BeforeCap_BuffID.God)
				unit.CancelBuff(MapID, 0, TargetID, Flag_Rest_BuffID.God)

				unit.CancelBuff(MapID, 0, TargetID, Flag_Defance_BuffID.God)
				unit.AddBuff(MapID, 0, TargetID, Flag_Attack_BuffID.God, TargetID)
			elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj2 then
				unit.CancelBuff(MapID, 0, TargetID, Flag_BeforeCap_BuffID.People)
				unit.CancelBuff(MapID, 0, TargetID, Flag_Rest_BuffID.People)

				unit.CancelBuff(MapID, 0, TargetID, Flag_Defance_BuffID.People)
				unit.AddBuff(MapID, 0, TargetID, Flag_Attack_BuffID.People, TargetID)
			elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj3 then
				unit.CancelBuff(MapID, 0, TargetID, Flag_BeforeCap_BuffID.Monster)
				unit.CancelBuff(MapID, 0, TargetID, Flag_Rest_BuffID.Monster)

				unit.CancelBuff(MapID, 0, TargetID, Flag_Defance_BuffID.Monster)
				unit.AddBuff(MapID, 0, TargetID, Flag_Attack_BuffID.Monster, TargetID)
			end

		end

	end

	return 0
end

--- breif: �Ծ��д��͹��ܵĵ���������
function GodMiracleCapture_ItemCanUse(MapID, InstanceID, TypeID, TargetID)

	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	local Sec   = tonumber(os.date("%S"))

	if Hour == 20 and (Min <= 30 and Min >= 0)then
		-- ����������᷶Χ��
		local guildID = 0
		guildID = guild.GetRoleGuildID(TargetID)
		local GuildPos
		GuildPos  = guild.GetRoleGuildPos(guildID, TargetID);
		if GuildPos ~= nil then
			if GetDefendGuildID(MapID) == guildID or GetCaptureGuildID(MapID) == guildID then
				return -1, 1
			end
		end
	end
	return 0, 0
end

--- breif: ս�������ߣ�������ʱ�ص�NPC�Ա�
function GodMiracleCpature_CanEnterWhenOnline(MapID, RoleID)
	-- ȡ�����ս��״̬
	--	ES_CSAttack		=	8,	// ��ս������Ӫ
	--	ES_CSDefence	=	9,	// ��ս������Ӫ
	role.UnsetStateBefAdd2Map(RoleID, 8)
	role.UnsetStateBefAdd2Map(RoleID, 9)
	return MapID_S02, S02_Trans_Pos.x, S02_Trans_Pos.y, S02_Trans_Pos.z
end

function GodMiracleCpature_OnRoleEnterArea(MapID, InstanceID, OwnerID, ObjAreaID)
	if OpenGodMiracleFlag == 0 then
		return
	end

	if role.IsRoleHaveBuff(MapID, InstanceID, OwnerID, 9001601) then
		return
	end

	-- ��ͼ�����ж�
	if IsInGodAreaMap(MapID) == 0 then
		return
	end

	-- ������״̬��������
	local WeekDay = os.date("%A")
	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
	if godCurState ~= 2 or GetCaptureGuildID(MapID) == 0 then
		if godCurState == 1 and WeekDay=="Saturday" and Hour == 19 and Min >= 50 then

		else
			return
		end
	end

	local guildID = guild.GetRoleGuildID(OwnerID)
	if guildID == 4294967295 then
		return
	end
	local DOrCFlag = GetRoleGuildDefendOrCapture(MapID, guildID)
	if DOrCFlag == GodEnum.Null then
		return
	end

	-- �ű������ж�
	if ObjAreaID ~= ClanGodMiracle_Proto[MapID].DefAreaID and ObjAreaID ~= ClanGodMiracle_Proto[MapID].AttAreaID then
		return
	end

	local NeedAddBuff = 0
	if DOrCFlag == GodEnum.Defend and ObjAreaID == ClanGodMiracle_Proto[MapID].DefAreaID then
		NeedAddBuff = 1
	elseif DOrCFlag == GodEnum.Capture and ObjAreaID == ClanGodMiracle_Proto[MapID].AttAreaID then
		NeedAddBuff = 1
	end

	if NeedAddBuff == 1 then
		role.AddBuff(MapID, InstanceID, OwnerID, 9001601, OwnerID)
	end
end

function GodMiracleCpature_OnRoleLeaveArea(MapID, InstanceID, OwnerID, ObjAreaID)
	if role.IsRoleHaveBuff(MapID, InstanceID, OwnerID, 9001601) then
		role.CancelBuff(MapID, InstanceID, OwnerID, 9001601)
	end
end

--�����̨����
function GodMiracleCpature_OnActOpenClose(ActiveID, IsOpen)
	OpenGodMiracleFlag = IsOpen

	TianShenZhong_OpenFlag = IsOpen
	QiHuaZhiHai_OpenFlag		= IsOpen
	BenLeiLieGu_OpenFlag		= IsOpen

	if IsOpen == 0 then
		-- ������Ϊ�ر�״̬
		guild.SetGodCurState(0, 3)
		guild.SetGodCurState(1, 3)
		guild.SetGodCurState(2, 3)
	else
		-- ������Ϊδռ��״̬
		guild.SetGodCurState(0, 0)
		guild.SetGodCurState(1, 0)
		guild.SetGodCurState(2, 0)
	end
end


------------------------------------------------------------------------------
-- register

-- ��ע���ʼ������ʱ���²���
aux.RegisterActEvent(ActiveID, 1, "GodMiracleCpature_OnActiveTimer")
aux.RegisterActEvent(ActiveID, 2, "GodMiracleCpature_OnActiveStart")
aux.RegisterActEvent(ActiveID, 4, "GodMiracleCpature_OnActiveTimeMin")
aux.RegisterActEvent(ActiveID, ESAE_OnActOpenClose, "GodMiracleCpature_OnActOpenClose")

-- map
aux.RegisterMapEvent("j01", 5, "GodMiracleCpature_OnRoleDie")
aux.RegisterMapEvent("j03", 5, "GodMiracleCpature_OnRoleDie")
aux.RegisterMapEvent("j04", 5, "GodMiracleCpature_OnRoleDie")

aux.RegisterMapEvent("j01", 2, "GodMiracleCpature_OnRoleEnter")
aux.RegisterMapEvent("j03", 2, "GodMiracleCpature_OnRoleEnter")
aux.RegisterMapEvent("j04", 2, "GodMiracleCpature_OnRoleEnter")

aux.RegisterMapEvent("j01", 3, "GodMiracleCpature_OnRoleLeave")
aux.RegisterMapEvent("j03", 3, "GodMiracleCpature_OnRoleLeave")
aux.RegisterMapEvent("j04", 3, "GodMiracleCpature_OnRoleLeave")

aux.RegisterMapEvent("j01", 7, "GodMiracleCpature_OnRoleEnterArea")
aux.RegisterMapEvent("j03", 7, "GodMiracleCpature_OnRoleEnterArea")
aux.RegisterMapEvent("j04", 7, "GodMiracleCpature_OnRoleEnterArea")

aux.RegisterMapEvent("j01", 23, "GodMiracleCpature_OnRoleLeaveArea")
aux.RegisterMapEvent("j03", 23, "GodMiracleCpature_OnRoleLeaveArea")
aux.RegisterMapEvent("j04", 23, "GodMiracleCpature_OnRoleLeaveArea")


-- ս�������ߣ�������ʱ�ص�NPC�Ա�
aux.RegisterMapEvent("j01", 12, "GodMiracleCpature_CanEnterWhenOnline")
aux.RegisterMapEvent("j03", 12, "GodMiracleCpature_CanEnterWhenOnline")
aux.RegisterMapEvent("j04", 12, "GodMiracleCpature_CanEnterWhenOnline")

-- skill
-- ���鼼��ע�����ʹ���¼���
aux.RegisterSkillEvent(9000201, 0, "GodMiracleCpature_CanCast")
aux.RegisterSkillEvent(9000201, 2, "GodMiracleCpature_CastSkill")
-- �ֻؼ���ʹ��
aux.RegisterSkillEvent(1050801, 0, "GodMiracleCpature_CanCast")

-- ʹ���ɵ����|�ɵ����|�ɵ����|����׷��
aux.RegisterItemEvent(1310001, 0, "GodMiracleCapture_ItemCanUse")
aux.RegisterItemEvent(1360003, 0, "GodMiracleCapture_ItemCanUse")
aux.RegisterItemEvent(1360102, 0, "GodMiracleCapture_ItemCanUse")
aux.RegisterItemEvent(1360110, 0, "GodMiracleCapture_ItemCanUse")



