--role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])ʮλ��ʾ��������λ��ʾ���ִ���
--cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0)�Ƿ��ǵ���
--cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 1)Ĭ���������
--cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 2)�����������


--�����ʱ����
GuessFingersInterval = {}
GuessFingersQuanWang = {}
--GuessFingersInterval[RoleID] = {startTime = xxx, lastTime = xxx, step = xxx ,lianSheng = xxx��idTwo = xxx}

--�Ի�
GuessingFingers = {}
GuessingFingers[1] = {Nid = 443030,jiandao = 20,shitou = 60, bu = 20, rare = 0}
GuessingFingers[2] = {Nid = 443031,jiandao = 50,shitou = 50, bu = 0, rare = 0}
GuessingFingers[3] = {Nid = 443032,jiandao = 20,shitou = 20, bu = 60, rare = 0}
GuessingFingers[4] = {Nid = 443033,jiandao = 30,shitou = 70, bu = 0, rare = 0}
GuessingFingers[5] = {Nid = 443034,jiandao = 20,shitou = 20, bu = 60, rare = 0}
GuessingFingers[6] = {Nid = 443035,jiandao = 35,shitou = 35, bu = 30, rare = 0}
GuessingFingers[7] = {Nid = 443036,jiandao = 20,shitou = 20, bu = 60, rare = 0}
GuessingFingers[8] = {Nid = 443037,jiandao = 20,shitou = 60, bu = 20, rare = 0}
GuessingFingers[9] = {Nid = 443038,jiandao = 30,shitou = 35, bu = 35, rare = 0}
GuessingFingers[10] = {Nid = 443039,jiandao = 60,shitou = 20, bu = 20, rare = 0}
GuessingFingers[11] = {Nid = 443040,jiandao = 0,shitou = 50, bu = 50, rare = 0}
GuessingFingers[12] = {Nid = 443041,jiandao = 35,shitou = 30, bu = 35, rare = 0}
GuessingFingers[13] = {Nid = 443042,jiandao = 50,shitou = 0, bu = 50, rare = 0}
GuessingFingers[14] = {Nid = 443043,jiandao = 60,shitou = 20, bu = 20, rare = 0}
GuessingFingers[15] = {Nid = 443044,jiandao = 20,shitou = 20, bu = 60, rare = 0}
GuessingFingers[16] = {Nid = 443045,jiandao = 50,shitou = 0, bu = 50, rare = 0}
GuessingFingers[17] = {Nid = 443046,jiandao = 20,shitou = 20, bu = 60, rare = 0}
GuessingFingers[18] = {Nid = 443047,jiandao = 20,shitou = 60, bu = 20, rare = 0}
GuessingFingers[19] = {Nid = 443048,jiandao = 20,shitou = 20, bu = 60, rare = 0}
GuessingFingers[20] = {Nid = 443049,jiandao = 20,shitou = 60, bu = 20, rare = 0}
GuessingFingers[21] = {Nid = 443050,jiandao = 50,shitou = 50, bu = 0, rare = 0}
GuessingFingers[22] = {Nid = 443051,jiandao = 60,shitou = 20, bu = 20, rare = 0}
GuessingFingers[23] = {Nid = 443052,jiandao = 30,shitou = 60, bu = 10, rare = 0}
GuessingFingers[24] = {Nid = 443053,jiandao = 50,shitou = 50, bu = 0, rare = 0}
GuessingFingers[25] = {Nid = 443054,jiandao = 20,shitou = 60, bu = 20, rare = 0}
GuessingFingers[26] = {Nid = 443055,jiandao = 20,shitou = 60, bu = 20, rare = 0}
GuessingFingers[27] = {Nid = 443056,jiandao = 40,shitou = 0, bu = 60, rare = 0}
GuessingFingers[28] = {Nid = 443057,jiandao = 50,shitou = 50, bu = 0, rare = 0}
GuessingFingers[29] = {Nid = 443058,jiandao = 20,shitou = 20, bu = 60, rare = 0}
GuessingFingers[30] = {Nid = 443059,jiandao = 50,shitou = 0, bu = 50, rare = 0}
GuessingFingers[31] = {Nid = 443060,jiandao = 50,shitou = 0, bu = 50, rare = 0}
GuessingFingers[32] = {Nid = 443061,jiandao = 0,shitou = 50, bu = 50, rare = 0}
GuessingFingers[33] = {Nid = 443062,jiandao = 20,shitou = 20, bu = 60, rare = 0}
GuessingFingers[34] = {Nid = 443063,jiandao = 50,shitou = 50, bu = 0, rare = 0}
GuessingFingers[40] = {Nid = 443069,jiandao = 0,shitou = 100, bu = 0, rare = 30}
GuessingFingers[41] = {Nid = 443070,jiandao = 0,shitou = 100, bu = 0, rare = 20}
GuessingFingers[42] = {Nid = 443071,jiandao = 100,shitou = 0, bu = 0, rare = 20}
GuessingFingers[43] = {Nid = 443072,jiandao = 0,shitou = 0, bu = 100, rare = 20}
GuessingFingers[44] = {Nid = 443073,jiandao = -100,shitou = -100, bu = -100, rare = 15}
GuessingFingers[45] = {Nid = 443074,jiandao = -100,shitou = -100, bu = -100, rare = 15}
GuessingFingers[46] = {Nid = 443075,jiandao = -100,shitou = -100, bu = -100, rare = 15}
GuessingFingers[47] = {Nid = 443076,jiandao = 100,shitou = 100, bu = 100, rare = 20}
GuessingFingers[48] = {Nid = 443077,jiandao = 100,shitou = 0, bu = 0, rare = 20}
GuessingFingers[49] = {Nid = 443078,jiandao = 0,shitou = 100, bu = 0, rare = 20}
GuessingFingers[50] = {Nid = 443079,jiandao = 0,shitou = 0, bu = 100, rare = 20}
GuessingFingers[51] = {Nid = 443080,jiandao = -100,shitou = -100, bu = -100, rare = 15}
GuessingFingers[52] = {Nid = 443081,jiandao = -100,shitou = -100, bu = -100, rare = 15}
GuessingFingers[53] = {Nid = 443082,jiandao = 100,shitou = 100, bu = 100, rare = 20}
GuessingFingers[54] = {Nid = 443083,jiandao = 35,shitou = 35, bu = 30, rare = 30}
GuessingFingers[55] = {Nid = 443084,jiandao = 0,shitou = 100, bu = 0, rare = 20}
GuessingFingers[56] = {Nid = 443085,jiandao = 30,shitou = 35, bu = 35, rare = 30}
GuessingFingers[57] = {Nid = 443086,jiandao = 35,shitou = 40, bu = 35, rare = 20}
GuessingFingers[58] = {Nid = 443087,jiandao = 100,shitou = 100, bu = 100, rare = 20}
GuessingFingers[59] = {Nid = 443088,jiandao = -100,shitou = -100, bu = -100, rare = 15}
GuessingFingers[60] = {Nid = 443089,jiandao = 0,shitou = 0, bu = 100, rare = 25}
GuessingFingers[61] = {Nid = 443090,jiandao = 40,shitou = 35, bu = 35, rare = 10}
GuessingFingers[62] = {Nid = 443091,jiandao = 100,shitou = 100, bu = 100, rare = 20}
GuessingFingers[63] = {Nid = 443092,jiandao = -100,shitou = -100, bu = -100, rare = 15}
GuessingFingers[64] = {Nid = 443093,jiandao = 0,shitou = 0, bu = 100, rare = 20}
GuessingFingers[65] = {Nid = 443094,jiandao = 100,shitou = 0, bu = 0, rare = 20}
GuessingFingers[66] = {Nid = 443095,jiandao = 0,shitou = 100, bu = 0, rare = 20}
GuessingFingers[67] = {Nid = 443096,jiandao = 0,shitou = 100, bu = 0, rare = 20}
GuessingFingers[68] = {Nid = 443097,jiandao = 100,shitou = 0, bu = 0, rare = 20}
GuessingFingers[69] = {Nid = 443098,jiandao = 0,shitou = 0, bu = 100, rare = 20}
GuessingFingers[70] = {Nid = 443099,jiandao = 0,shitou = 0, bu = 100, rare = 20}
GuessingFingers[71] = {Nid = 443100,jiandao = -100,shitou = -100, bu = -100, rare = 15}
GuessingFingers[72] = {Nid = 443101,jiandao = 100,shitou = 0, bu = 0, rare = 20}
GuessingFingers[73] = {Nid = 443102,jiandao = 100,shitou = 0, bu = 0, rare = 20}
GuessingFingers[74] = {Nid = 443103,jiandao = 100,shitou = 0, bu = 0, rare = 20}
GuessingFingers[75] = {Nid = 443104,jiandao = 100,shitou = 100, bu = 100, rare = 20}
GuessingFingers[76] = {Nid = 443105,jiandao = -100,shitou = -100, bu = -100, rare = 15}
GuessingFingers[77] = {Nid = 443106,jiandao = -100,shitou = -100, bu = -100, rare = 15}
GuessingFingers[78] = {Nid = 443107,jiandao = 40,shitou = 10, bu = 50, rare = 20}
GuessingFingers[79] = {Nid = 443108,jiandao = 50,shitou = 40, bu = 10, rare = 20}
GuessingFingers[80] = {Nid = 443109,jiandao = 10,shitou = 50, bu = 40, rare = 20}
GuessingFingers[81] = {Nid = 443110,jiandao = 100,shitou = 0, bu = 0, rare = 20}
GuessingFingers[82] = {Nid = 443111,jiandao = 0,shitou = 100, bu = 0, rare = 20}
GuessingFingers[83] = {Nid = 443112,jiandao = 30,shitou = 40, bu = 30, rare = 30}
GuessingFingers[84] = {Nid = 443113,jiandao = 0,shitou = 50, bu = 50, rare = 20}
GuessingFingers[85] = {Nid = 443114,jiandao = 0,shitou = 50, bu = 50, rare = 20}
GuessingFingers[86] = {Nid = 443115,jiandao = 30,shitou = 35, bu = 35, rare = 20}
GuessingFingers[87] = {Nid = 443116,jiandao = 20,shitou = 20, bu = 60, rare = 20}
GuessingFingers[88] = {Nid = 443117,jiandao = 0,shitou = 0, bu = 100, rare = 20}
GuessingFingers[89] = {Nid = 443118,jiandao = 0,shitou = 100, bu = 0, rare = 20}
GuessingFingers[90] = {Nid = 443119,jiandao = 100,shitou = 0, bu = 0, rare = 20}

function GuessFingersIndex(MapID, InstanceID, RoleID, CreatureID, FingersNum, lun, ind)
	--return indexid
	local cTime = os.time()
	local st = GuessFingersInterval[RoleID].startTime
	local lt = GuessFingersInterval[RoleID].lastTime
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local added = 0			--����ֵ
	local indexid = 0		--���ص�ʤ����ϢID
	local randomN = math.random(100)
	--if lun < 4 then
	--��ʼ
	if st == 0 then
		st = cTime
		GuessFingersInterval[RoleID].startTime = cTime
		lt = cTime
		GuessFingersInterval[RoleID].lastTime = cTime
	else
		if os.difftime(cTime,lt) < 60 then					--�ϴκͱ��β�ȭ��ʱ����
			if os.difftime(cTime,st) > 180 then				--���ߺ�ʼ��ȭ�ͱ��β�ȭ��ʱ����
				added = 5
			elseif os.difftime(cTime,st) > 240 then
				added = 10
			elseif os.difftime(cTime,st) > 300 then
				added = 20
			elseif os.difftime(cTime,st) > 480 then
				added = 50
			end
		end
		GuessFingersInterval[RoleID].lastTime = cTime
	end
	--��������2��index
	if GuessFingersInterval[RoleID].idTwo > 0 then
		ind = GuessFingersInterval[RoleID].idTwo
	end
	--�ж�����
	if FingersNum == 2 then
		--ʤ
		if randomN < GuessingFingers[ind].bu + added then
			indexid = 443023
			role.SetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"], role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])+1)
		--ƽ
		elseif randomN < GuessingFingers[ind].bu + GuessingFingers[ind].jiandao  + added then
			indexid = 443021
		else
			indexid = 443022
			role.SetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"], role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])-role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])%10)
		end
		GuessFingersInterval[RoleID].idTwo = 0
	elseif FingersNum == 0 then
		--ʤ
		if randomN < GuessingFingers[ind].jiandao + added then
			indexid = 443024
			role.SetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"], role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])+1)
		elseif randomN < GuessingFingers[ind].jiandao + GuessingFingers[ind].shitou  + added then
			indexid = 443025
		else
			indexid = 443026
			role.SetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"], role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])-role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])%10)
		end
		GuessFingersInterval[RoleID].idTwo = 0
	elseif FingersNum == 5 then
		--ʤ
		if randomN < GuessingFingers[ind].shitou + added then
			indexid = 443028
			role.SetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"], role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])+1)
		elseif randomN < GuessingFingers[ind].shitou + GuessingFingers[ind].bu  + added then
			indexid = 443029
		else
			indexid = 443027
			role.SetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"], role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])-role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])%10)
		end
		GuessFingersInterval[RoleID].idTwo = 0
	end

	return indexid
end

function CaiQuanExp(MapID, InstanceID, RoleID, lun)
    local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if lun == 1 then
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])%10 > 4 then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"], role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])+10)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"], role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])-role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])%10)
			--��һ�ֽ������
			role.AddRoleExp(MapID, InstanceID, RoleID, (level-30)*1000+3600)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400703, 1, -1, 8, 300)

		end
	elseif lun == 2 then
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])%10 > 5 then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"], role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])+10)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"], role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])-role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])%10)
			--�ڶ��ֽ������
			role.AddRoleExp(MapID, InstanceID, RoleID, (level-30)*1000+5400)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400703, 1, -1, 8, 300)
		end
	elseif lun == 3 then
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])%10 > 6 then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"], role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])+10)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"], role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])-role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])%10)
			--�����ֽ������
			role.AddRoleExp(MapID, InstanceID, RoleID, (level-30)*1000+9000)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400703, 1, -1, 8, 300)
			GuessFingersInterval[RoleID] = nil
		end
	end
end

--��ս��С��
function c5610042On_Talk(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID, TalkIndex)
	local lev = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if lev < 30 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 443130) 			--��ս��С�£��ҲŲ���30�����µ�С������~
		msg.AddMsgEvent(MsgID, 21, 5)   			--��ť
		msg.AddMsgEvent(MsgID, 1, 443131)   		--�뿪
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	else

		local CurTime = tonumber(os.date("%j"))
		local UpdateTime = math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])/100)
		local creUpdate = cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0)
		if CurTime ~= UpdateTime then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"], CurTime*100+10)
		end
		if CurTime ~= creUpdate then
			cre.SetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0, CurTime)
			cre.SetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 1, math.random(1,34))
		end
		local curLun = math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])%100/10)
		local curTimes = role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])%10
		local ide = cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 1)

		local left = 5
		if curLun == 1 then
			left = 5
		elseif curLun == 2 then
			left = 6
		elseif curLun == 3 then
			left = 7
		end
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, GuessingFingers[ide].Nid) 	--���һ����ʾ�����
			msg.AddMsgEvent(MsgID, 21, 4) 			--��ť
			msg.AddMsgEvent(MsgID, 1, 443000) 		--�����ȭ��ս
			msg.AddMsgEvent(MsgID, 21, 10)   		--��ť
			msg.AddMsgEvent(MsgID, 1, 443001)   	--ѯ�ʹ���
			msg.AddMsgEvent(MsgID, 21, 5)   		--��ť
			msg.AddMsgEvent(MsgID, 1, 100002)   	--ȡ��
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 4 then
			local MsgID = msg.BeginMsgEvent()
			if role.GetBagFreeSize(RoleID) < 1 then
				msg.AddMsgEvent(MsgID, 20, 443002) 		--�����ﶼû�пռ���
				msg.AddMsgEvent(MsgID, 21, 5)   		--��ť
				msg.AddMsgEvent(MsgID, 1, 100002)   	--ȡ��
			else
				if CurTime == UpdateTime and curLun < 4 then
					msg.AddMsgEvent(MsgID, 20, 443003) 	--���������ĵ�x����ս��������ʤx�֣���Ϳ��Ի����Ӧ������
					msg.AddMsgEvent(MsgID, 9, curLun)	--����
					msg.AddMsgEvent(MsgID, 9, left-curTimes)	--����
					msg.AddMsgEvent(MsgID, 21, 6)   	--��ť
					msg.AddMsgEvent(MsgID, 1, 443004)   --��ʼ��ȭ
					msg.AddMsgEvent(MsgID, 21, -1)   	--��ť
					msg.AddMsgEvent(MsgID, 1, 100002)   --ȡ��
				else
					msg.AddMsgEvent(MsgID, 20, 443005) 	--������Ѿ���ս3����Ŷ��
					msg.AddMsgEvent(MsgID, 21, 5)   		--��ť
					msg.AddMsgEvent(MsgID, 1, 100002)   	--ȡ��
				end
			end
			msg.AddMsgEvent(MsgID, 24, CreatureID) --npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 6 then
			local MsgID = msg.BeginMsgEvent()
			if role.GetBagFreeSize(RoleID) < 1 then
				msg.AddMsgEvent(MsgID, 20, 443002) 		--�����ﶼû�пռ���
				msg.AddMsgEvent(MsgID, 21, -1)   		--��ť
				msg.AddMsgEvent(MsgID, 1, 100002)   	--ȡ��
			else
			    if GuessFingersInterval[RoleID] == nil then
					GuessFingersInterval[RoleID] = {}
					GuessFingersInterval[RoleID].startTime = 0
					GuessFingersInterval[RoleID].lastTime = 0
					GuessFingersInterval[RoleID].idTwo = 0
				end
				local ho = math.random(40,90)
				if math.random(100) <= GuessingFingers[ho].rare then
					GuessFingersInterval[RoleID].idTwo = ho
				end
				if GuessFingersInterval[RoleID].idTwo > 0 then
					ide = GuessFingersInterval[RoleID].idTwo
				end
				msg.AddMsgEvent(MsgID, 20, GuessingFingers[ide].Nid)			--����Ի�
				msg.AddMsgEvent(MsgID, 21, 7) 			-- ��ť
				msg.AddMsgEvent(MsgID, 1, 443006) 		-- ����
				msg.AddMsgEvent(MsgID, 21, 8) 			-- ��ť
				msg.AddMsgEvent(MsgID, 1, 443007) 		-- ʯͷ
				msg.AddMsgEvent(MsgID, 21, 9) 			-- ��ť
				msg.AddMsgEvent(MsgID, 1, 443008) 		-- ��
				msg.AddMsgEvent(MsgID, 21, 11) 			-- ��ť
				msg.AddMsgEvent(MsgID, 1, 443009) 		-- ����
			end
			msg.AddMsgEvent(MsgID, 24, CreatureID) --npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 7 then
		    if curLun < 4 then
				local indexid = GuessFingersIndex(MapID, InstanceID, RoleID, CreatureID, 2, curLun, ide)
				curTimes = role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])%10
				local cha = left-curTimes
				local MsgID = msg.BeginMsgEvent()
				if cha ~= 0 then
					msg.AddMsgEvent(MsgID, 20, indexid) 	--ʤƽ����Ϣ
					msg.AddMsgEvent(MsgID, 2, RoleID) 		--���
					--[[if indexid == 443023 then
						msg.AddMsgEvent(MsgID, 9, cha)			--����
					else
						msg.AddMsgEvent(MsgID, 9, cha+1)			--����
					end]]
					msg.AddMsgEvent(MsgID, 9, cha)
					if indexid == 443023 or indexid == 443021 then
						msg.AddMsgEvent(MsgID, 21, 6) 		--��ť
						msg.AddMsgEvent(MsgID, 1, 443010) 	--����
						msg.AddMsgEvent(MsgID, 21, 4)   		--��ť
						msg.AddMsgEvent(MsgID, 1, 100002) 		--ȡ��
					else
						msg.AddMsgEvent(MsgID, 21, 4)   			--��ť
						msg.AddMsgEvent(MsgID, 1, 443009) 			--����
					end
				else
					CaiQuanExp(MapID, InstanceID, RoleID, curLun)
					msg.AddMsgEvent(MsgID, 20, 443018) 		--ʤ+�Ѿ�Ӯ�ñ���ʤ��
					msg.AddMsgEvent(MsgID, 2, RoleID) 		--���
					msg.AddMsgEvent(MsgID, 21, -1)   		--��ť
					msg.AddMsgEvent(MsgID, 1, 443133) 			--��ʼ��һ��
				end
				msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 8 then
		    if curLun < 4 then
				local indexid = GuessFingersIndex(MapID, InstanceID, RoleID, CreatureID, 0, curLun, ide)
				curTimes = role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])%10
				local cha = left-curTimes
				local MsgID = msg.BeginMsgEvent()
				if cha ~= 0 then
					msg.AddMsgEvent(MsgID, 20, indexid) 	--ʤƽ����Ϣ
					msg.AddMsgEvent(MsgID, 2, RoleID) 		--���
					--[[if indexid == 443024 then
						msg.AddMsgEvent(MsgID, 9, cha)			--����
					else
						msg.AddMsgEvent(MsgID, 9, cha+1)			--����
					end]]
					msg.AddMsgEvent(MsgID, 9, cha)
					if indexid == 443024 or indexid == 443025 then
						msg.AddMsgEvent(MsgID, 21, 6) 		--��ť
						msg.AddMsgEvent(MsgID, 1, 443010) 	--����
						msg.AddMsgEvent(MsgID, 21, 4)   		--��ť
						msg.AddMsgEvent(MsgID, 1, 100002) 		--ȡ��
					else
						msg.AddMsgEvent(MsgID, 21, 4)   			--��ť
						msg.AddMsgEvent(MsgID, 1, 443009) 			--����
					end
				else
					CaiQuanExp(MapID, InstanceID, RoleID, curLun)
					msg.AddMsgEvent(MsgID, 20, 443019) 		--ʤ+�Ѿ�Ӯ�ñ���ʤ��
					msg.AddMsgEvent(MsgID, 2, RoleID) 		--���
					msg.AddMsgEvent(MsgID, 21, -1)   		--��ť
					msg.AddMsgEvent(MsgID, 1, 443133) 			--��ʼ��һ��
				end
				msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 9 then
		    if curLun < 4 then
				local indexid = GuessFingersIndex(MapID, InstanceID, RoleID, CreatureID, 5, curLun, ide)
				curTimes = role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])%10
				local cha = left-curTimes
				local MsgID = msg.BeginMsgEvent()
				if cha ~= 0 then
					msg.AddMsgEvent(MsgID, 20, indexid) 	--ʤƽ����Ϣ
					msg.AddMsgEvent(MsgID, 2, RoleID) 		--���
					--[[if indexid == 443028 then
						msg.AddMsgEvent(MsgID, 9, cha)			--����
					else
						msg.AddMsgEvent(MsgID, 9, cha+1)			--����
					end]]
					msg.AddMsgEvent(MsgID, 9, cha)
					if indexid == 443028 or indexid == 443029 then
						msg.AddMsgEvent(MsgID, 21, 6) 		--��ť
						msg.AddMsgEvent(MsgID, 1, 443010) 	--����
						msg.AddMsgEvent(MsgID, 21, 4)   		--��ť
						msg.AddMsgEvent(MsgID, 1, 100002) 		--ȡ��
					else
						msg.AddMsgEvent(MsgID, 21, 4)   			--��ť
						msg.AddMsgEvent(MsgID, 1, 443009) 			--����
					end
				else
					CaiQuanExp(MapID, InstanceID, RoleID, curLun)
					msg.AddMsgEvent(MsgID, 20, 443020) 		--ʤ+�Ѿ�Ӯ�ñ���ʤ��
					msg.AddMsgEvent(MsgID, 2, RoleID) 		--���
					msg.AddMsgEvent(MsgID, 21, -1)   		--��ť
					msg.AddMsgEvent(MsgID, 1, 443133) 			--��ʼ��һ��
				end
				msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 10 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 443011) 			--����˵��
			msg.AddMsgEvent(MsgID, 9, (lev-30)*1000+3600) 			--1�ֽ�������
			msg.AddMsgEvent(MsgID, 9, (lev-30)*1000+5400) 			--2�ֽ�������
			msg.AddMsgEvent(MsgID, 9, (lev-30)*1000+9000) 			--3�ֽ�������
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.AddMsgEvent(MsgID, 21, -1) 				-- ��ť
			msg.AddMsgEvent(MsgID, 1, 443012) 			--����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 11 then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"], role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])-role.GetRoleScriptData(RoleID, 1, RoleDataType["GuessFingers"])%10)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 443013) 			--�㾹ȻͶ�������ˣ��ߺߣ�Ҳ�ã�ʡ���Ҷ����ˡ�
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.AddMsgEvent(MsgID, 21, 6) 				--��ť
			msg.AddMsgEvent(MsgID, 1, 443004) 			--��ʼ��ȭ
			msg.AddMsgEvent(MsgID, 21, 5)   			--��ť
			msg.AddMsgEvent(MsgID, 1, 100002) 			--ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5610042, 7, "c5610042On_Talk")


--ȭ������
function c5610043_On_Talk(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID, TalkIndex)

	if TalkIndex == -1 then
	    if GuessFingersQuanWang[RoleID] ~= nil then
		    local k = math.floor(GuessFingersQuanWang[RoleID].Exp*GuessFingersQuanWang[RoleID].ExpPercent)
			role.AddRoleExp(MapID, InstanceID, RoleID, k)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 443132) 		--ȭ��Q:\n    ��һ�����ҽ�����X�β�ȭ�����������X�㾭��
			msg.AddMsgEvent(MsgID, 9, GuessFingersQuanWang[RoleID].LianSheng)
			msg.AddMsgEvent(MsgID, 9, k)
			msg.AddMsgEvent(MsgID, 21, -1)   		--��ť
			msg.AddMsgEvent(MsgID, 1, 100001)   	--ȡ��
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			GuessFingersQuanWang[RoleID] = nil
		elseif role.GetRoleItemNum(RoleID, 4400703) < 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 443014) 		--���Ҳ�ȭ��������Ҫ��һ����ȭʤ�����¡�
			msg.AddMsgEvent(MsgID, 21, 5)   		--��ť
			msg.AddMsgEvent(MsgID, 1, 100002)   	--ȡ��
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 443015) 		--�Ǿ���һ��ʤ����!
			msg.AddMsgEvent(MsgID, 21, 4) 			--��ť
			msg.AddMsgEvent(MsgID, 1, 443000) 		--�����ȭ��ս
			msg.AddMsgEvent(MsgID, 21, 5)   		--��ť
			msg.AddMsgEvent(MsgID, 1, 100002)   	--ȡ��
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
    elseif TalkIndex == 4 then

		if role.GetBagFreeSize(RoleID) < 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 443016) 		--�����ռ䲻�㣬�����޷���ȡ�����ġ�
			msg.AddMsgEvent(MsgID, 21, -1)   		--��ť
			msg.AddMsgEvent(MsgID, 1, 100002)   	--ȡ��
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else

		    if role.RemoveFromRole(MapID, InstanceID, RoleID, 4400703, 1, 300) == 0 then
			    if GuessFingersQuanWang[RoleID] == nil then
					GuessFingersQuanWang[RoleID] = {ExpPercent=1,LianSheng=0,Step=0,Exp=0,TotleTimes=0}
				end
				local expset = {12000,14000,16000,18000,20000,22000,24000,26000,28000,30000,34000,36000,38000,40000,42000,47000,49000,51000,53000,55000,60000,63000,65000,67000,69000,75000,77000,80000,82000,84000,87000,89000,92000,94000,96000,99000,101000,104000,106000,108000,111000,113000,116000,118000,120000,123000,125000,128000,130000,132000,135000,137000,140000,142000,144000,147000,149000,152000,154000,156000,159000,161000,164000,166000,168000,171000,173000,176000,178000,180000,183000}

				local lev = role.GetRoleLevel(MapID, InstanceID, RoleID)
				if lev < 30 then
				    lev = 30
				elseif lev > 100 then
				    lev = 100
				end
				GuessFingersQuanWang[RoleID].Exp = math.floor(expset[lev-29]/12*7)
			    GuessFingersQuanWang[RoleID].Step = 1        --1�ǿ��Բ�ȭ��2�ǿ�����ȡ������0��ʲô��������
				--���ͳ��
				UpdateRoleData(RoleID, 20, 1)
			end

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 443017) 		--׼�����ˣ��Ǿͳ��а�!
			msg.AddMsgEvent(MsgID, 21, 6) 			--��ť
			msg.AddMsgEvent(MsgID, 1, 443006) 		--����
			msg.AddMsgEvent(MsgID, 21, 7) 			--��ť
			msg.AddMsgEvent(MsgID, 1, 443007) 		--ʯͷ
			msg.AddMsgEvent(MsgID, 21, 8) 			--��ť
			msg.AddMsgEvent(MsgID, 1, 443008) 		--��
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end

	elseif TalkIndex == 6 or TalkIndex == 7 or TalkIndex == 8 then
	    if GuessFingersQuanWang[RoleID] ~= nil and GuessFingersQuanWang[RoleID].Step == 1 then

		    local JieGuo = {}
			JieGuo[6] = {ying = 443123,ping = 443121,shu = 443122}    --������
			JieGuo[7] = {ying = 443124,ping = 443125,shu = 443126}    --��ʯͷ
			JieGuo[8] = {ying = 443128,ping = 443129,shu = 443127}    --����

			local ran = math.random(100)
			local k = GuessFingersQuanWang[RoleID].TotleTimes * 2          --ÿ�β�ȭ�ܴ�����һ��ʤ���ĸ��ʽ���0.02��ƽ�ĸ��ʽ���0.02

			if ran > 52 + k then		--ʤ

				GuessFingersQuanWang[RoleID].LianSheng = GuessFingersQuanWang[RoleID].LianSheng + 1
				GuessFingersQuanWang[RoleID].TotleTimes = GuessFingersQuanWang[RoleID].TotleTimes + 1
				GuessFingersQuanWang[RoleID].ExpPercent = GuessFingersQuanWang[RoleID].ExpPercent + 0.2

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, JieGuo[TalkIndex].ying)			--ʤ����Ϣ
				msg.AddMsgEvent(MsgID, 2, RoleID)			--���
				msg.AddMsgEvent(MsgID, 9, GuessFingersQuanWang[RoleID].LianSheng) --��Ӯ����
				msg.AddMsgEvent(MsgID, 9, math.floor(GuessFingersQuanWang[RoleID].Exp*GuessFingersQuanWang[RoleID].ExpPercent))
				msg.AddMsgEvent(MsgID, 21, 10) 			-- ��ť
				msg.AddMsgEvent(MsgID, 1, 443010) 		-- ����
				msg.AddMsgEvent(MsgID, 21, 9) 			-- ��ť
				msg.AddMsgEvent(MsgID, 1, 443009) 		-- ����
				msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			elseif ran > 4 + 2*k then	--ƽ

				GuessFingersQuanWang[RoleID].TotleTimes = GuessFingersQuanWang[RoleID].TotleTimes + 1
				GuessFingersQuanWang[RoleID].ExpPercent = GuessFingersQuanWang[RoleID].ExpPercent + 0.1

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, JieGuo[TalkIndex].ping)			--ʤ����Ϣ
				msg.AddMsgEvent(MsgID, 2, RoleID)			--���
				msg.AddMsgEvent(MsgID, 9, GuessFingersQuanWang[RoleID].LianSheng) --��Ӯ����
				msg.AddMsgEvent(MsgID, 9, math.floor(GuessFingersQuanWang[RoleID].Exp*GuessFingersQuanWang[RoleID].ExpPercent))
				msg.AddMsgEvent(MsgID, 21, 10) 			-- ��ť
				msg.AddMsgEvent(MsgID, 1, 443010) 		-- ����
				msg.AddMsgEvent(MsgID, 21, 9) 			-- ��ť
				msg.AddMsgEvent(MsgID, 1, 443009) 		-- ����
				msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			elseif ran >= 0 then	--��

				GuessFingersQuanWang[RoleID].ExpPercent = math.floor(GuessFingersQuanWang[RoleID].ExpPercent/2*100)/100

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, JieGuo[TalkIndex].shu)			--ʤ����Ϣ
				msg.AddMsgEvent(MsgID, 2, RoleID)			--���
				msg.AddMsgEvent(MsgID, 9, GuessFingersQuanWang[RoleID].LianSheng) --��Ӯ����
				msg.AddMsgEvent(MsgID, 9, math.floor(GuessFingersQuanWang[RoleID].Exp*GuessFingersQuanWang[RoleID].ExpPercent))
				msg.AddMsgEvent(MsgID, 21, 9) 			-- ��ť
				msg.AddMsgEvent(MsgID, 1, 443009) 		-- ����
				msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			end
		end

    elseif TalkIndex == 9 then

	    if GuessFingersQuanWang[RoleID] ~= nil then

		    local k = math.floor(GuessFingersQuanWang[RoleID].Exp*GuessFingersQuanWang[RoleID].ExpPercent)
			role.AddRoleExp(MapID, InstanceID, RoleID, k)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 443132) 		--ȭ��Q:\n    ��һ�����ҽ�����X�β�ȭ�����������X�㾭��
			msg.AddMsgEvent(MsgID, 9, GuessFingersQuanWang[RoleID].LianSheng)
			msg.AddMsgEvent(MsgID, 9, k)
			msg.AddMsgEvent(MsgID, 21, -1)   		--��ť
			msg.AddMsgEvent(MsgID, 1, 100001)   	--ȡ��
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			GuessFingersQuanWang[RoleID] = nil

		end
	elseif TalkIndex == 10 then

		if GuessFingersQuanWang[RoleID] ~= nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 443017) 		--׼�����ˣ��Ǿͳ��а�!
			msg.AddMsgEvent(MsgID, 21, 6) 			--��ť
			msg.AddMsgEvent(MsgID, 1, 443006) 		--����
			msg.AddMsgEvent(MsgID, 21, 7) 			--��ť
			msg.AddMsgEvent(MsgID, 1, 443007) 		--ʯͷ
			msg.AddMsgEvent(MsgID, 21, 8) 			--��ť
			msg.AddMsgEvent(MsgID, 1, 443008) 		--��
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5610043, 7, "c5610043_On_Talk")


