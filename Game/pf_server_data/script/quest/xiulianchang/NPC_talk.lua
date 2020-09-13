
--������������
function c5610047_On_Talk(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID, TalkIndex)
    local CurTime = tonumber(os.date("%j"))
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["XiuLianChang_LingQu"])
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["XiuLianChang_Time"])
	local hour = math.floor(k/60)
	local minute = k%60
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 605005) 		--�����������\n    ֻ��70�����ϵ���Ҳ��ܽ�������������������ʣ�������ʱ��ΪXXСʱXX���ӣ�ʹ����������Ʊ������������ʣ������ʱ�䣨����48��Сʱ����
		msg.AddMsgEvent(MsgID, 9, hour)
        msg.AddMsgEvent(MsgID, 9, minute) --
		if rolelevel >= 70 then
		    msg.AddMsgEvent(MsgID, 21, 4)   		--��ť
			msg.AddMsgEvent(MsgID, 1, 605006)   	--����������
			if CurTime ~= UpdateTime then
		        msg.AddMsgEvent(MsgID, 21, 5)   		--��ť
				msg.AddMsgEvent(MsgID, 1, 605007)   	--��ȡ�����������Ʊ
			end
		end
		msg.AddMsgEvent(MsgID, 21, 20)   		--��ť
		msg.AddMsgEvent(MsgID, 1, 100002)   	--ȡ��
		msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
	    if rolelevel >= 70 then
		    if k >= 1 then
				role.SetRoleScriptData(RoleID, 1, RoleDataType["XiuLianChang_Time"],k-1)
			    role.RoleGotoNewMap(MapID, InstanceID, RoleID, 2748433913, 277, 200, 483)
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 605008) 		--�����������\n    ����Ҫ������������Ʊ���ܽ�����������
				msg.AddMsgEvent(MsgID, 21, -1)   		--��ť
				msg.AddMsgEvent(MsgID, 1, 100001)   	--ȡ��
				msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	elseif TalkIndex == 5 then
        if CurTime ~= UpdateTime then
		    if role.GetBagFreeSize(RoleID) < 1 then
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 605009) 		--�����������\n    ������ձ�����������ȡ��
				msg.AddMsgEvent(MsgID, 21, -1)   		--��ť
				msg.AddMsgEvent(MsgID, 1, 100001)   	--ȡ��
				msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
			    role.SetRoleScriptData(RoleID, 1, RoleDataType["XiuLianChang_LingQu"], CurTime)
			    role.AddRoleItem(MapID, InstanceID, RoleID, 4800131, 1, 1, 8, 420)
			end
		end
	end
end
aux.RegisterCreatureEvent(5610047, 7, "c5610047_On_Talk")


function c5610046_On_Talk(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID, TalkIndex)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local EXP_XIULIAN = {}
	EXP_XIULIAN[70] = 463050
	EXP_XIULIAN[71] = 478050
	EXP_XIULIAN[72] = 493140
	EXP_XIULIAN[73] = 509290
	EXP_XIULIAN[74] = 525540
	EXP_XIULIAN[75] = 541890
	EXP_XIULIAN[76] = 558350
	EXP_XIULIAN[77] = 574920
	EXP_XIULIAN[78] = 591590
	EXP_XIULIAN[79] = 696390
	EXP_XIULIAN[80] = 716800
	EXP_XIULIAN[81] = 737350
	EXP_XIULIAN[82] = 758020
	EXP_XIULIAN[83] = 779950
	EXP_XIULIAN[84] = 802020
	EXP_XIULIAN[85] = 824220
	EXP_XIULIAN[86] = 846560
	EXP_XIULIAN[87] = 891650
	EXP_XIULIAN[88] = 891650
	EXP_XIULIAN[89] = 915560
	EXP_XIULIAN[90] = 1057050
	EXP_XIULIAN[91] = 1084270
	EXP_XIULIAN[92] = 1111650
	EXP_XIULIAN[93] = 1140510
	EXP_XIULIAN[94] = 1169540
	EXP_XIULIAN[95] = 1331930
	EXP_XIULIAN[96] = 1364560
	EXP_XIULIAN[97] = 1397390
	EXP_XIULIAN[98] = 1430400
	EXP_XIULIAN[99] = 1465100
	EXP_XIULIAN[100] = 1500000
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if rolelevel > 100 then
	    rolelevel = 100
	elseif rolelevel < 70 then
	    rolelevel = 70
	end
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 605001) 		--��ˮ���һ��ˣ�\n    ��0�㿪ʼ��ÿ������Сʱ��������н�����ֵ��顣��ɱ���飬����������Я���Ļ�ˮ�������ң��һ����XXX�㾭�顣
		msg.AddMsgEvent(MsgID, 9, EXP_XIULIAN[rolelevel]*0.3)
		msg.AddMsgEvent(MsgID, 21, 4)   		--��ť
		msg.AddMsgEvent(MsgID, 1, 605002)   	--������ˮ����Ƭ*30
		msg.AddMsgEvent(MsgID, 21, 5)   		--��ť
		msg.AddMsgEvent(MsgID, 1, 605003)   	--������覴õĻ�ˮ��*3
		msg.AddMsgEvent(MsgID, 21, 6)   		--��ť
		msg.AddMsgEvent(MsgID, 1, 605004)   	--���������Ļ�ˮ��
		msg.AddMsgEvent(MsgID, 21, 20)   		--��ť
		msg.AddMsgEvent(MsgID, 1, 100002)   	--ȡ��
		msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
        local b = role.GetRoleItemNum(RoleID,4800134)
		if b < 30 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 605010) 		--��ˮ���һ��ˣ�\n    �����ӵ��30����ˮ����Ƭ���ܶһ�һ�ξ��顣
			msg.AddMsgEvent(MsgID, 21, -1)   		--��ť
			msg.AddMsgEvent(MsgID, 1, 100001)   	--ȡ��
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    role.RemoveFromRole(MapID, InstanceID, RoleID, 4800134, 30, 101)
			role.AddRoleExp(MapID, InstanceID, RoleID, EXP_XIULIAN[rolelevel]*0.3)
			--����ͳ��
			local curTime = tonumber(os.date("%j"))
			if lianHunQ_Roles[RoleID] == nil then
				UpdateSeverData(55, 1)
				lianHunQ_Roles[RoleID] = curTime
			elseif lianHunQ_Roles[RoleID] ~= curTime then
				lianHunQ_Roles = {}
				lianHunQ_Roles[RoleID] = curTime
				UpdateSeverData(55, 1)
			end
		end
	elseif TalkIndex == 5 then
        local b = role.GetRoleItemNum(RoleID,4800135)
		if b < 3 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 605011) 		--��ˮ���һ��ˣ�\n    �����ӵ��3����覴õĻ�ˮ�����ܶһ�һ�ξ��顣
			msg.AddMsgEvent(MsgID, 21, -1)   		--��ť
			msg.AddMsgEvent(MsgID, 1, 100001)   	--ȡ��
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    role.RemoveFromRole(MapID, InstanceID, RoleID, 4800135, 3, 101)
			role.AddRoleExp(MapID, InstanceID, RoleID, EXP_XIULIAN[rolelevel]*0.3)
			--����ͳ��
			local curTime = tonumber(os.date("%j"))
			if lianHunQ_Roles[RoleID] == nil then
				UpdateSeverData(55, 1)
				lianHunQ_Roles[RoleID] = curTime
			elseif lianHunQ_Roles[RoleID] ~= curTime then
				lianHunQ_Roles = {}
				lianHunQ_Roles[RoleID] = curTime
				UpdateSeverData(55, 1)
			end
		end
	elseif TalkIndex == 6 then
        local b = role.GetRoleItemNum(RoleID,4800136)
		if b < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 605012) 		--��ˮ���һ��ˣ�\n    �����ӵ��1�������Ļ�ˮ�����ܶһ�һ�ξ��顣
			msg.AddMsgEvent(MsgID, 21, -1)   		--��ť
			msg.AddMsgEvent(MsgID, 1, 100001)   	--ȡ��
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    role.RemoveFromRole(MapID, InstanceID, RoleID, 4800136, 1, 101)
			role.AddRoleExp(MapID, InstanceID, RoleID, EXP_XIULIAN[rolelevel]*0.3)
			--����ͳ��
			local curTime = tonumber(os.date("%j"))
			if lianHunQ_Roles[RoleID] == nil then
				UpdateSeverData(55, 1)
				lianHunQ_Roles[RoleID] = curTime
			elseif lianHunQ_Roles[RoleID] ~= curTime then
				lianHunQ_Roles = {}
				lianHunQ_Roles[RoleID] = curTime
				UpdateSeverData(55, 1)
			end
		end
	end
end
aux.RegisterCreatureEvent(5610046, 7, "c5610046_On_Talk")
