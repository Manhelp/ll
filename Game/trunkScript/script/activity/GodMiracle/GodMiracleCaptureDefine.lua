-------------------------------------------------------------------------------
-- Copyright (c) 2004 TENGWU Entertainment All rights reserved.
-- filename: GodMiracleCpatureDefine.h
-- author: chdeng
-- actor:
-- data: 2010-01-25
-- last:
-- brief: ������ṹ����
-------------------------------------------------------------------------------
-- ������
ClanGodMiracleNum = 5
Minrecord 	 = 0
ActiveID = 25

MapID_test02 = 3618849499
MapID_FX	 = 2216111541
MapID_ZL	 = 2216112053
MapID_WS	 = 2216110261
MapID_S02  = 3424073512

--!\ brief: ��ͼ�Ƿ���������������
function IsInGodAreaMap(MapID)
	if MapID == MapID_FX or MapID == MapID_ZL or MapID == MapID_WS then
		return	1
	else
		return 0
	end
end

-- �õĵ�ö��
GodEnum =  {Null=0, Npc1=1, Npc2=2, Npc3=3, Defend=1, Capture=2, TRUE=1,FALSE=0, StdScoresTick=150}

-- ���������񼣵Ľṹ��
ClanGodMiracle_Proto = {}

ClanGodMiracle_Proto[MapID_FX] 	   = {PosX1=0,PosY1=0,PosZ1=0, PosX2=0,PosY2=0,PosZ2=0, PosX3=0,PosY3=0,PosZ3=0, ClanGodMiracleID=1, NPCID1 = 6010012, NPCID2 = 6010013, NPCID3 = 6010014, NPCIDObj1=0, NPCIDObj2=0, NPCIDObj3=0, DefAreaID=1139, AttAreaID=1140}
ClanGodMiracle_Proto[MapID_ZL] 	   = {PosX1=0,PosY1=0,PosZ1=0, PosX2=0,PosY2=0,PosZ2=0, PosX3=0,PosY3=0,PosZ3=0, ClanGodMiracleID=2, NPCID1 = 6010015, NPCID2 = 6010016, NPCID3 = 6010017, NPCIDObj1=0, NPCIDObj2=0, NPCIDObj3=0, DefAreaID=0, AttAreaID=0}
ClanGodMiracle_Proto[MapID_WS] 	   = {PosX1=0,PosY1=0,PosZ1=0, PosX2=0,PosY2=0,PosZ2=0, PosX3=0,PosY3=0,PosZ3=0, ClanGodMiracleID=3, NPCID1 = 6010018, NPCID2 = 6010019, NPCID3 = 6010020, NPCIDObj1=0, NPCIDObj2=0, NPCIDObj3=0, DefAreaID=0, AttAreaID=0}

ClanGodMiracle_Pos = {}
ClanGodMiracle_Pos[MapID_FX]	   = { x1=383, y1=5100,   z1=163,   x2=827, y2=5743,   z2=832, x3=510, y3=4840,   z3=529}			--- 1���� 2ħ�� 3�˽�
ClanGodMiracle_Pos[MapID_ZL]	   = { x1=376, y1=3260,   z1=1087, x2=543, y2=2498,   z2=687, x3=932, y3=2664,   z3=786}			--- 1���� 2ħ�� 3�˽�
ClanGodMiracle_Pos[MapID_WS]	   = { x1=231, y1=13351, z1=880,   x2=675, y2=14815, z2=606, x3=753, y3=14063, z3=960}			--- 1���� 2ħ�� 3�˽�

RoleEnter_Pos = {}
RoleEnter_Pos[MapID_FX]	= { x1=869,	 y1=103,  z1=140,   x2=158,  y2=119,  z2=776}					--����ڣս��
RoleEnter_Pos[MapID_ZL]	= { x1=805,  y1=59,    z1=1295, x2=364,  y2=67,    z2=320}					--�߻�֮��ս��		
RoleEnter_Pos[MapID_WS]	= { x1=450,  y1=282,  z1=541,   x2=302,  y2=272,  z2=1178}					--�����ѹ�ս��

-- ��������
Flag_BeforeCap_BuffID	= {God = 9001801, People = 9002101, Monster = 9002401}	-- ����
Flag_Attack_BuffID			= {God = 9001901, People = 9002201, Monster = 9002501}	-- ����
Flag_Defance_BuffID		= {God = 9002001, People = 9002301, Monster = 9002601}	-- ����
Flag_Opening_BuffID		= {God = 9002701, People = 9002901, Monster = 9003101}	-- ��ӡ
Flag_Rest_BuffID				= {God = 9002801, People = 9003001, Monster = 9003201}	-- ��ս


S02_Trans_Pos = {x=611, y=23095, z=671}

--ս������ԱNPC��ID
TransmitorID_FX = 5010110		--����ڣս��
TransmitorID_ZL = 5010238		--�߻�֮��ս��
TransmitorID_WS = 5010239		--����ս��

GodMiracleTypeID = {}
GodMiracleTypeID[TransmitorID_FX] = {GodMiracleTypeID=0, MapID=MapID_FX}
GodMiracleTypeID[TransmitorID_ZL] = {GodMiracleTypeID=1, MapID=MapID_ZL}
GodMiracleTypeID[TransmitorID_WS] = {GodMiracleTypeID=2, MapID=MapID_WS}

--  \breif: ͨ������ID�õ����Ǹ�����
function GetGodStackeIndex(MapID, NpcTypeID)
	if ClanGodMiracle_Proto[MapID].NPCID1 == NpcTypeID then
		return GodEnum.Npc1
	elseif ClanGodMiracle_Proto[MapID].NPCID2 == NpcTypeID then
		return GodEnum.Npc2
	elseif ClanGodMiracle_Proto[MapID].NPCID3 == NpcTypeID then
		return GodEnum.Npc3
	end
	return GodEnum.Null
end

-- \breif: �õ�����ID
function GetAreaID(MapID)
	return ClanGodMiracle_Proto[MapID].GodAreaID
end

-- \breif: �õ���ID(Ϊ�ϲ��ṩ)
function GetGodMiracleIDMinOne(MapID)
	return ClanGodMiracle_Proto[MapID].ClanGodMiracleID -1
end

broadCastGlag = {flag1 = 0, flag2 = 0, flag3=0, flag4=0, flag5=0, flag6=0}
function ClearBroaCastGlag()
	broadCastGlag.flag1=0
	broadCastGlag.flag2=0
	broadCastGlag.flag3=0
	broadCastGlag.flag4=0
	broadCastGlag.flag5=0
	broadCastGlag.flag6=0
end

-- �������ʱ��Ϣ
ClanGodMiracle_CurInfo = {}
ClanGodMiracle_CurInfo[1] = {MapID = MapID_FX,	   DefendGuild=0, CaptureGuild=0, Npc1 = -1, Npc2=-1, Npc3=-1, DefendSocre = 0, CaptureScores=0, tick=0, InitFlag=0, GdMiracleID = 5510630, openFlag = 0}
ClanGodMiracle_CurInfo[2] = {MapID = MapID_ZL,	   DefendGuild=0, CaptureGuild=0, Npc1 = -1, Npc2=-1, Npc3=-1, DefendSocre = 0, CaptureScores=0, tick=0, InitFlag=0, GdMiracleID = 5510631, openFlag = 0}
ClanGodMiracle_CurInfo[3] = {MapID = MapID_WS,	   DefendGuild=0, CaptureGuild=0, Npc1 = -1, Npc2=-1, Npc3=-1, DefendSocre = 0, CaptureScores=0, tick=0, InitFlag=0, GdMiracleID = 5510632, openFlag = 0}

-- �������
Capture_Result = {}
Capture_Result[1] = {MapID = MapID_FX, DefendGuild=0, CaptureGuild=0, WinOrLoseFlag = 0, GdMiracleID = 5510630}
Capture_Result[2] = {MapID = MapID_ZL, DefendGuild=0, CaptureGuild=0, WinOrLoseFlag = 0, GdMiracleID = 5510631}
Capture_Result[3] = {MapID = MapID_WS, DefendGuild=0, CaptureGuild=0, WinOrLoseFlag = 0, GdMiracleID = 5510632}

-- \breif: ����񼣵�������
function ClearGodMiracleResult()
	for k, v in pairs(Capture_Result) do
		Capture_Result[k].DefendGuild = 0
		Capture_Result[k].CaptureGuild = 0
		Capture_Result[k].WinOrLoseFlag = 0
	end
end

-- \breif: �õ����ر�־
function GetOpenFlag(MapID)
	return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].openFlag
end

-- \breif: ���ĳ��
function ClearGodMiracle(GodMiracleID)
	ClanGodMiracle_CurInfo[GodMiracleID].DefendGuild 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].CaptureGuild 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].Npc1 			= -1
	ClanGodMiracle_CurInfo[GodMiracleID].Npc2 			= -1
	ClanGodMiracle_CurInfo[GodMiracleID].Npc3 			= -1
	ClanGodMiracle_CurInfo[GodMiracleID].DefendSocre 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].CaptureScores 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].tick 			= 0
end

-- \breif: ����������������Ϣ
function ResetGodMiracle(GodMiracleID)
	ClanGodMiracle_CurInfo[GodMiracleID].CaptureGuild 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].Npc1 			= -1
	ClanGodMiracle_CurInfo[GodMiracleID].Npc2 			= -1
	ClanGodMiracle_CurInfo[GodMiracleID].Npc3 			= -1
	ClanGodMiracle_CurInfo[GodMiracleID].DefendSocre 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].CaptureScores 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].tick 			= 0
end

-- \breif: �õ���ͼID
function GetCurInfoMapID(GodMiracleID)			return ClanGodMiracle_CurInfo[GodMiracleID].MapID end

-- \breif:  ��ʼ����
function SetInitFlag(MapID, inFlag)			ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].InitFlag = inFlag	end
function GetInitFlag(MapID)					return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].InitFlag	end

-- \breif: �õ������÷�������ID
function GetDefendGuildID(MapID) 				return   ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendGuild 	 end
function SetDefendGuildID(MapID, inGuildID)	ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendGuild = inGuildID end

-- \breif: �õ��������������ID
function GetCaptureGuildID(MapID) 				return   ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureGuild   end
function SetCaptureGuildID(MapID, inGuildID)	ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureGuild = inGuildID end

-- \breif: ���Ӻ�����TICK
function Gettick(MapID)						return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].tick	end
function SetTick(MapID, inTick)				ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].tick = inTick	end
function ModTick(MapID, inTick)				ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].tick = ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].tick + inTick  end

-- \breif: ���ú͵��׻���
--  param2: �ط��򹥷��ı�־
function GetScores(MapID, DOrCFlag)
	if DOrCFlag ==  GodEnum.Defend then
		return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendSocre
	elseif DOrCFlag ==  GodEnum.Capture then
		return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureScores
	end

end

function SetScores(MapID, DOrCFlag, inScores)
	if DOrCFlag ==  GodEnum.Defend then
		 ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendSocre = inScores
	elseif DOrCFlag == GodEnum.Capture then
		 ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureScores = inScores
	end
end

function ModScores(MapID, DOrCFlag, inScores)
	if DOrCFlag ==  GodEnum.Defend then
		ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendSocre =  ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendSocre + inScores
	elseif DOrCFlag ==  GodEnum.Capture then
		ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureScores =  ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureScores + inScores
	end
end


-- \breif: ���ú͵õ�ռ���񼣵İ���ID
function GetCapGodStacke(MapID, NpcType)
	if NpcType == GodEnum.Npc1 then
		return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc1
	elseif NpcType == GodEnum.Npc2 then
		return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc2
	elseif NpcType == GodEnum.Npc3 then
		return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc3
	end
end

function SetCapGodStacke(MapID, NpcType, GuildID)
	local flagType = 0
	local PosX=0
	local PosY=0
	local PosZ=0
	if NpcType == GodEnum.Npc1 then
		 PosX=ClanGodMiracle_Proto[MapID].PosX1
		 PosY=ClanGodMiracle_Proto[MapID].PosY1
		 PosZ=ClanGodMiracle_Proto[MapID].PosZ1
		 ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc1 = GuildID
		 flagType = 0
	elseif NpcType == GodEnum.Npc2 then
		 PosX=ClanGodMiracle_Proto[MapID].PosX3
		 PosY=ClanGodMiracle_Proto[MapID].PosY3
		 PosZ=ClanGodMiracle_Proto[MapID].PosZ3
		 ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc2 = GuildID
 		 flagType = 2
	elseif NpcType == GodEnum.Npc3 then
		 PosX=ClanGodMiracle_Proto[MapID].PosX2
		 PosY=ClanGodMiracle_Proto[MapID].PosY2
		 PosZ=ClanGodMiracle_Proto[MapID].PosZ2
		 ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc3 = GuildID
		 flagType = 1
	else
		 return
	end

	-- ֪ͨ�ͻ�����������������
	guild.SyncFlagAttack(GetGodMiracleIDMinOne(MapID), flagType, GetGuildCapType(NpcType, MapID), MapID, 0, 1, PosX, PosY, PosZ)	-- ��
end

-- \breif: �鿴����ID�Ƿ���������ɵ�������
--  param2:
function IsInGodCaptureGuild(MapID, GuildID)

	if ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendGuild == 0 and ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureGuild == 0 then
		return 0
	end

	if GuildID == ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendGuild or GuildID == ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureGuild then
		return 1
	end

	return 0

end

-- \breif: ����������ڵİ����ǹ��������ط�
function GetRoleGuildDefendOrCapture(MapID, GuildID)

	--ȡ������Ӫ���ͣ�1��ʾ���أ�2��ʾ������0��ʾû�в�սȨ
	local guildAlliance = guild.GetGuildAllianceType(GetGodMiracleIDMinOne(MapID), GuildID)
	if guildAlliance == 1 then
		return GodEnum.Defend
	elseif guildAlliance == 2 then
		return GodEnum.Capture
	else
		return GodEnum.Null
	end

end

-- \breif: �鿴ĳ����ռ�м�������
function GuildCapturePillar(MapID, GuildID)
	if GuildID == 0 then
		return 0
	end

	local Pillar = 0
		if ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc1 == GuildID then
			Pillar = Pillar + 1
		end

		if ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc2 == GuildID then
			Pillar = Pillar + 1
		end

		if ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc3 == GuildID then
			Pillar = Pillar + 1
		end

	return Pillar
end

-- \breif:  �õ�����ռ���񼣵ĸ���
--  param1: ����ID
function GetGuildCaptureNum(GuildID)
	if GuildID ==  0 then
		return 0
	end

	local CapNum = 0
	for k, v in pairs(ClanGodMiracle_CurInfo) do
		if ClanGodMiracle_CurInfo[k].DefendGuild == GuildID then
			CapNum  = CapNum  + 1
		end
	end
	return CapNum
end

-- \breif:  �����񼣸����õ���ҵ�ǰʹ�õ�BUFFID
--  param1: ��ȡ�񼣵ĸ���
--function GetBuffIDbyGodMiracleNum(godMiracleNum)
--	if godMiracleNum == 1 then
--		return 9000701
--	elseif godMiracleNum == 2  then
--		return 9000802
--	elseif godMiracleNum == 3 then
--		return 9000903
--	elseif godMiracleNum == 4 then
--		return 9001004
--	elseif godMiracleNum == 5 then
--		return 9001105
--	end
--end

function InitGodMiracleOpenFlag()
	ClanGodMiracle_CurInfo[1].openFlag = TianShenZhong_OpenFlag;
	ClanGodMiracle_CurInfo[2].openFlag = QiHuaZhiHai_OpenFlag;
	ClanGodMiracle_CurInfo[3].openFlag = BenLeiLieGu_OpenFlag;
end

-- \breif: ��������
function CreateGodmiracle()
	for k, v in pairs(ClanGodMiracle_CurInfo) do
		if GetOpenFlag(ClanGodMiracle_CurInfo[k].MapID) == 1 then
			local MapID = ClanGodMiracle_CurInfo[k].MapID
			ClanGodMiracle_Proto[MapID].NPCIDObj1 = map.MapCreateCreature(MapID, 0, ClanGodMiracle_Proto[MapID].NPCID1, ClanGodMiracle_Pos[MapID].x1, ClanGodMiracle_Pos[MapID].y1, ClanGodMiracle_Pos[MapID].z1)
			ClanGodMiracle_Proto[MapID].NPCIDObj2 = map.MapCreateCreature(MapID, 0, ClanGodMiracle_Proto[MapID].NPCID2, ClanGodMiracle_Pos[MapID].x2, ClanGodMiracle_Pos[MapID].y2, ClanGodMiracle_Pos[MapID].z2)
			ClanGodMiracle_Proto[MapID].NPCIDObj3 = map.MapCreateCreature(MapID, 0, ClanGodMiracle_Proto[MapID].NPCID3, ClanGodMiracle_Pos[MapID].x3, ClanGodMiracle_Pos[MapID].y3, ClanGodMiracle_Pos[MapID].z3)
			ClanGodMiracle_Proto[MapID].PosX1,ClanGodMiracle_Proto[MapID].PosY1,ClanGodMiracle_Proto[MapID].PosZ1=unit.GetPosition(MapID, 0,ClanGodMiracle_Proto[MapID].NPCIDObj1);
			ClanGodMiracle_Proto[MapID].PosX2,ClanGodMiracle_Proto[MapID].PosY2,ClanGodMiracle_Proto[MapID].PosZ2=unit.GetPosition(MapID, 0,ClanGodMiracle_Proto[MapID].NPCIDObj3);
			ClanGodMiracle_Proto[MapID].PosX3,ClanGodMiracle_Proto[MapID].PosY3,ClanGodMiracle_Proto[MapID].PosZ3=unit.GetPosition(MapID, 0,ClanGodMiracle_Proto[MapID].NPCIDObj2);
		end
	end
end

-- \breif: �õ��񼣵�ռ�����
function InitAllGodMiracle()

	--����
	CreateGodmiracle()

	SetDefendGuildID(MapID_FX ,act.GetActScriptData(ActiveID, 1, 0))
	SetDefendGuildID(MapID_ZL ,act.GetActScriptData(ActiveID, 1, 2))
	SetDefendGuildID(MapID_WS ,act.GetActScriptData(ActiveID, 1, 4))

	local WeekDay = os.date("%A")

	if WeekDay == "Saturday" then
		
		local Hour = tonumber(os.date("%H"))
		local Min  = tonumber(os.date("%M"))

		if (Hour == 19 and Min > 50)  or Hour == 20 then
			-- ���ý�������
			SetCaptureGuildID(MapID_FX ,act.GetActScriptData(ActiveID, 1, 10))
			SetCaptureGuildID(MapID_ZL ,act.GetActScriptData(ActiveID, 1, 12))
			SetCaptureGuildID(MapID_WS ,act.GetActScriptData(ActiveID, 1, 14))
			broadCastGlag.flag2 = 1
		end

		if Hour == 20 then
			-- ������Ϊ����״̬
			guild.SetGodCurState(0, 2)
			guild.SetGodCurState(1, 2)
			guild.SetGodCurState(2, 2)
		end

	end

	for k, v in pairs(ClanGodMiracle_CurInfo) do

		if GetOpenFlag(ClanGodMiracle_CurInfo[k].MapID) == 1 then
			-- �����񼣵ĵ�ǰ״̬,�������ϸ���ɫBUFF
			local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(ClanGodMiracle_CurInfo[k].MapID))
			if godCurState  == 0 then -- δ������
				ChangePPillarColor(ClanGodMiracle_CurInfo[k].MapID, 0)
			elseif godCurState   == 1 then -- ����
				ChangePPillarColor(ClanGodMiracle_CurInfo[k].MapID, 1)
			end
		end
	end
end

-- \breif: �ı�������ɫ
--  param2:���ӵ�״̬�� 0 δ������ 1����
function ChangePPillarColor(MapID, ColorType)
	if ColorType == 0 then
		unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj1, Flag_BeforeCap_BuffID.God, ClanGodMiracle_Proto[MapID].NPCIDObj1)
		unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj2, Flag_BeforeCap_BuffID.People, ClanGodMiracle_Proto[MapID].NPCIDObj2)
		unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj3, Flag_BeforeCap_BuffID.Monster, ClanGodMiracle_Proto[MapID].NPCIDObj3)
	elseif ColorType == 1 then
		unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj1, Flag_Rest_BuffID.God, ClanGodMiracle_Proto[MapID].NPCIDObj1)
		unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj2, Flag_Rest_BuffID.People, ClanGodMiracle_Proto[MapID].NPCIDObj2)
		unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj3, Flag_Rest_BuffID.Monster, ClanGodMiracle_Proto[MapID].NPCIDObj3)
	end
end

Minrecord = 0

-- \breif: �㲥���
function BroadcastGodMsgMin()

	--���ʱ��
	local Min  = tonumber(os.date("%M"))

	Minrecord = Minrecord + 1

	-- ÿСʱ��ʾһ��ս��״̬
	if Minrecord == 59 then
		for k, v in pairs(ClanGodMiracle_CurInfo) do
			-------------------------------------------------------------------------------- world(big)
			-- ��ǰ�������Դ���δ��ռ��״̬����������������ᣡ
			if ClanGodMiracle_CurInfo[k].DefendGuild == 0 and ClanGodMiracle_CurInfo[k].openFlag ~= 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100054)
				msg.AddMsgEvent(MsgID, 5  , ClanGodMiracle_CurInfo[k].GdMiracleID)
				msg.DispatchWorldMsgEvent(MsgID)
			end
		end
		Minrecord = 0
	end

end

BMBroad = 0
ClearBroaCastFlg = 0

-- \breif: �㲥���
-- remark: ÿtick����
function BroadcastGodMsgTick()

	local WeekDay = os.date("%A")
	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	local Sec = tonumber(os.date("%S"))

	-------------------------------------------------------------------------------- world(bif)
	-- ���յ�����������ս���۱����Ѿ���ʼ���������ɰ����ڰ��ɽ����н��б�����
	if WeekDay == "Monday" then
		if BMBroad == 0 and Hour >=8 and Hour <= 20 and Min == 0 then
			local MsgIDBM = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgIDBM, 101, 1401)
			msg.DispatchWorldMsgEvent(MsgIDBM)
			BMBroad = 1
		end

		if Hour >=8 and Hour <= 20 and Min == 1 and BMBroad == 1 then
			BMBroad = 0
		end

		if Hour == 8 and Min == 0 and ClearBroaCastFlg == 0 then
			ClearBroaCastGlag()
			ClearBroaCastFlg = 1
		end
		if Hour == 8 and Min == 1 and ClearBroaCastFlg == 1 then
			ClearBroaCastFlg = 0
		end
	end

	for k, v in pairs(ClanGodMiracle_CurInfo) do

		-------------------------------------------------------------------------------- guild
		-- ������ȷ���μӽ���20��00�ġ���������ս����������׼����ӭ��ս����
		if WeekDay == "Saturday" and Hour == 18 and Min == 0 then
			-- ***************�õ��������ID***********************
			SetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID, guild.GetApplyGuildID(k-1))

				if broadCastGlag.flag1 == 0 then

					--if GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID) ~= 0 then

						-- �ط�
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 102, 100057)
						msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
						msg.DispatchGuildMsgEvent(MsgID, GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))

						-- ����
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 102, 100057)
						msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
						msg.DispatchGuildMsgEvent(MsgID, GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID))

						-- �ط�ͬ��
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 102, 100057)
						msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
						msg.DispatchAllianceGuildMsgEvent(MsgID, 1, k-1)

						-- ����ͬ��
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 102, 100057)
						msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
						msg.DispatchAllianceGuildMsgEvent(MsgID, 2, k-1)
						
						if ClanGodMiracle_CurInfo[k].MapID == MapID_WS then
							broadCastGlag.flag1 = 1
						end
				--	end
			end
		end

		-------------------------------------------------------------------------------- guild
		-- ����������ս����10���Ӻ�ʼ������ǰ��������׼��ս����
		if WeekDay == "Saturday" and Hour == 19 and Min == 50 then

			if broadCastGlag.flag2 == 0 then
				--if GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID) ~= 0 then

					-- �����������
					map.MoveRole2AnotherMap(ClanGodMiracle_CurInfo[k].MapID, 0, MapID_S02, S02_Trans_Pos.x, S02_Trans_Pos.y, S02_Trans_Pos.z)

					-- ��ǰ��ʼ���ý�������
					SetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID, guild.GetApplyGuildID(k-1))

					-- �ط�
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 100058)
					msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
					msg.DispatchGuildMsgEvent(MsgID, GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))

					-- ����
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 100058)
					msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
					msg.DispatchGuildMsgEvent(MsgID, GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID))

					-- �ط�ͬ��
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 100058)
					msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
					msg.DispatchAllianceGuildMsgEvent(MsgID, 1, k-1)

					-- ����ͬ��
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 100058)
					msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
					msg.DispatchAllianceGuildMsgEvent(MsgID, 2, k-1)

					if ClanGodMiracle_CurInfo[k].MapID == MapID_WS then
						broadCastGlag.flag2 = 1
					end
				--end
			end
		end

		-- ���ð��ɽ���ս��״̬�������񼣽�������״̬
		if WeekDay == "Saturday" and  Hour == 20 and Min == 0 then
			if broadCastGlag.flag5 == 0 then
				--if GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID) ~= 0 then

					-- ���ϲ�������
					guild.SetGuildStatus(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)
					guild.SetGuildStatus(GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)

					-- ������Ϊ����״̬
					guild.SetGodCurState(k-1, 2)


					--��������ɫ�仯����ɫ
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_Rest_BuffID.God)
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_Rest_BuffID.People)
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_Rest_BuffID.Monster)

					unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_BeforeCap_BuffID.God, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1)
					unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_BeforeCap_BuffID.People, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2)
					unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_BeforeCap_BuffID.Monster, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3)

					CalFlag = 0

					if ClanGodMiracle_CurInfo[k].MapID == MapID_WS then
						broadCastGlag.flag5 = 1
					end
				--end
			end
		end

		-- &lt;p1&gt;Ӯ��&lt;p2&gt;��ռ��Ȩ��
		if broadCastGlag.flag6 ==0 and WeekDay == "Saturday" and  Hour == 21 and Min == 1 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100061)
			msg.AddMsgEvent(MsgID, 19, ClanGodMiracle_CurInfo[k].DefendGuild)
			msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
			msg.DispatchMapMsgEvent(MsgID, ClanGodMiracle_CurInfo[k].MapID, 0)

			if ClanGodMiracle_CurInfo[k].MapID == MapID_WS then
				broadCastGlag.flag6 = 1
			end
		end
	end

	-------------------------------------------------------------------------------- world(big)
	if broadCastGlag.flag3 == 0 then
		-- ���յ�������ս�Ѿ���ʼ��
		if WeekDay == "Saturday" and  Hour == 20 and Min == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100059)
			msg.DispatchWorldMsgEvent(MsgID)
			broadCastGlag.flag3 = 1;
		end
	end


	-------------------------------------------------------------------------------- world
	-- ����������ս�����䶨����ʤ���ɳ�Ա��������Ӿ��飬��Ǯ�͵��ߵ��ʵ�����״̬������ʤ����
	if broadCastGlag.flag4 == 0 then
		if Hour == 21 and Min == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100060)
			msg.DispatchWorldMsgEvent(MsgID)

			broadCastGlag.flag4 = 1
		end
	end

end


-- �㲥ս�����
function BroadcastCaptureResult()


	for k, v in pairs(Capture_Result) do

		-- �޽���
		if Capture_Result[k].WinOrLoseFlag  == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 1434)
			msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
			msg.DispatchWorldMsgEvent(MsgID)
		--�ط�ʤ
		elseif Capture_Result[k].WinOrLoseFlag  == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 1432)
			msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
			msg.AddMsgEvent(MsgID, 19, Capture_Result[k].DefendGuild)
			msg.AddMsgEvent(MsgID, 19, Capture_Result[k].CaptureGuild)
			msg.DispatchWorldMsgEvent(MsgID)

		--��Ӯ
		elseif Capture_Result[k].WinOrLoseFlag  == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 1433)
			msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
			msg.AddMsgEvent(MsgID, 19, Capture_Result[k].DefendGuild)
			msg.AddMsgEvent(MsgID, 19, Capture_Result[k].CaptureGuild)
			msg.DispatchWorldMsgEvent(MsgID)
		end
	end


	--[[local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 1436)
	msg.DispatchWorldMsgEvent(MsgID)--]]

	--guild.SendAllCoptureGodGuildMember()

	ClearGodMiracleResult();
end

-- \breif:  ��ȡ�񼣵�ռ�����ID
--  param1: �񼣵�ID
--  remark: �ɹ�������ID ʧ��:0
function GetGuildByGodMiracle(MapID)
	return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendGuild
end

