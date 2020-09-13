function Tianxuanzhe_OnClick(MapID, InstanceID, RoleID, TOMsgID, ToMapID, x, y, z)

	local curhour = tonumber(os.date("%H"))--Сʱ
	local curmin = tonumber(os.date("%M"))--����
	local day = os.date("%A")--����
    if TOMsgID == 206701 then
		if (day == "Wednesday" or day == "Sunday") and 20 == curhour and (curmin == 50 or curmin == 51) then
			CreatTianXuanRoleObj(RoleID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 100006) -- �����ȷ����ʱ������Ѿ�������ʱ�����ƣ�������ǰ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	else
        if (day == "Wednesday" or day == "Sunday") and 21 == curhour and (curmin == 0 or curmin == 1) then
			if role.IsRoleInStatus(RoleID, 1024) then
				role.CancelRoleStallState(MapID, InstanceID, RoleID)
			end
			local k = math.random(9)
			local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleID)
			if Target_MapID ~= nil and Target_InstanceID ~= nil then
				role.RoleGotoNewMap(Target_MapID, Target_InstanceID, RoleID, S59_MapID, S59_Point[k].x, S59_Point[k].y, S59_Point[k].z)      --����
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 100006) -- �����ȷ����ʱ������Ѿ�������ʱ�����ƣ�������ǰ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	end
end



function Tianxuanzhe_OnStart(actID)
	Tianxuanzhe_baoming = 0
    TianXuan_ChooseSide()
	for k,v in pairs(TianXuan_Role) do
	    local Target_MapID, Target_InstanceID = role.GetRoleMapID(k)
		if Target_MapID ~= nil and Target_InstanceID ~= nil then
			msg.SendRoleSwitchMsg(4, Target_MapID, Target_InstanceID, k, 1, 47)
		end
	end
end

function Tianxuanzhe_OnTimerMin(actID)

    local Min = tonumber(os.date("%M"))
	if Min == 57 or Min == 58 or Min == 59 then
	    for k,v in pairs(TianXuan_Battle) do
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 206703)  --��ѡ��֮ս����XX���Ӻ����
			msg.AddMsgEvent(MsgID, 9, 60-Min)
			msg.DispatchMapMsgEvent(MsgID, S59_MapID,k)
		end
	end
	if Min % 2 == 0 then
	    for k,v in pairs(TianXuan_Battle) do
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 206704)  --�µ���ѡ���Ѿ�����
			msg.DispatchMapMsgEvent(MsgID, S59_MapID,k)
		    local BattleObj = TianXuan_Battle[k]
            BattleObj:ChooseTianXuanZhiRen()
			BattleObj.Area:ReSetTianXuanArea()
		end
	end
	for k,v in pairs(TianXuan_Battle) do
		local BattleObj = TianXuan_Battle[k]
		BattleObj:SetPlayerPoint_TianXuan()
		BattleObj:ResetInSidePlayer_TianXuan()
	end

end

function Tianxuanzhe_OnEnd(actID)

    for k,v in pairs(TianXuan_Battle) do
		local BattleObj = TianXuan_Battle[k]
		BattleObj:SetPlayerFinalPoint_TianXuan()
		map.DeleteInstance(S59_MapID, k)
	end

    --ս�����󣬿��ܿ������ս��
	TianXuan_Battle = {}
	--��Ӫ����
	TianXuan_Side = {}
	--��Ҷ���
	TianXuan_Role = {}
	--ˢ�µ����������
	TianXuan_Area = {}
    TianXuan_Roletotal = 0
end

--ע��û������¼�
aux.RegisterActEvent(47, 8, "Tianxuanzhe_OnClick")
--ע��û������¼�
aux.RegisterActEvent(47, 3, "Tianxuanzhe_OnEnd")
aux.RegisterActEvent(48, 3, "Tianxuanzhe_OnEnd")
aux.RegisterActEvent(47, 2, "Tianxuanzhe_OnStart")
aux.RegisterActEvent(47, 4, "Tianxuanzhe_OnTimerMin")
aux.RegisterActEvent(48, 2, "Tianxuanzhe_OnStart")
aux.RegisterActEvent(48, 4, "Tianxuanzhe_OnTimerMin")

--������NPC�Ի��ű�
function x5610081_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curyear = tonumber(os.date("%Y"))
	local curtime = curyear*10000 + curmon*100 + curday
    --��ȡ��ҵȼ�
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if rolelevel < 50 then
	    rolelevel = 50
	elseif rolelevel > 100 then
	    rolelevel = 100
	end
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206705)		--������ͳ��ӵ£�\n         ��������������ÿ���������յ�8��50�ֿ�������Ҫ��Ϊ�����Ķ���������Ψһ�Ļ��ᣡ��ֻ��50��������ҿ��Բ���˻����
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		if Tianxuanzhe_baoming == 1 then
			msg.AddMsgEvent(MsgID, 21,	4)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	206706)		--����ѡ��֮ս������
		end
		if act.GetActIsStart(47) == true or act.GetActIsStart(48) == true then
			msg.AddMsgEvent(MsgID, 21,	9)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	206707)		--�����붷����������
		elseif act.GetActIsStart(47) ~= true and act.GetActIsStart(48) ~= true then
            if role.GetRoleScriptData(RoleID, 1, RoleDataType["Tianxuanzhe_Point"]) ~= 0 then
				msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	206708)		--����ȡ������
			end
		end

		msg.AddMsgEvent(MsgID, 21,	6)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	206709)		--������ѵ������
		msg.AddMsgEvent(MsgID, 21,	7)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	206710)		--���һ�ս��װ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
	    if Tianxuanzhe_baoming ~= nil and Tianxuanzhe_baoming == 1 then
		    if TianXuan_Role[RoleID] ~= nil then
	            local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206711)		--������ͳ��ӵ£�\n         ���Ѿ��������ˣ��޷��ظ���������
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if rolelevel < 50 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	206712)		--������ͳ��ӵ£�\n         ֻ��50��������Ҳ��ܱ�������˻����
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
					msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					CreatTianXuanRoleObj(RoleID)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	206713)		--������ͳ��ӵ£�\n         �����ɹ��������9����׼ʱ��ʼ����
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
					msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	elseif TalkIndex == 5 then
	    if act.GetActIsStart(47) ~= true and act.GetActIsStart(48) ~= true then
		    local point = role.GetRoleScriptData(RoleID, 1, RoleDataType["Tianxuanzhe_Point"])
		    local exp1 = math.floor(TianXuan_Exp[rolelevel] * 2 * point / 60)
			local exp2 = 8 * point
			local item = 0
			if point > 80 then
			    item = 4820242
			elseif point > 40 then
			    item = 4820241
			else
			    item = 4820240
			end
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206714)		--������ͳ��ӵ£�\n         �ϴλ�У��������ȡ�Ľ���������XXXX�㣬����ֵXXX�㣬���⽱��ΪXXX����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 9,	exp1)
			msg.AddMsgEvent(MsgID, 9,	exp2)
			msg.AddMsgEvent(MsgID, 4,	item)
			msg.AddMsgEvent(MsgID, 21,	11)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 6 then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206715)		--������ͳ��ӵ£�\n         ��ѵ������PK�����ܵ��κεĳͷ�����ʹ��ͬһ���������֮��Ҳ���Ի����д�ս�����ɡ�ȷ��Ҫ����ѵ�����𣿡�
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	8)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	206709)		    --������ѵ������
		msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	100002)		--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 7 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206716)		--������ͳ��ӵ£�\n         ��ѡ����װֻ�ܴ���1��Сʱ����ֻ���ڶ�����������ʹ�á�\n         �һ���ѡ����װ��Ҫ100���Լ�40��������ң���ѡ����Ҫ�������ѡ����װ����
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	12)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	206717)		    --�������װ��
		msg.AddMsgEvent(MsgID, 21,	13)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	206718)		    --��������װ��
		msg.AddMsgEvent(MsgID, 21,	14)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	206719)		    --��Ǭ����װ��
		msg.AddMsgEvent(MsgID, 21,	15)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	206720)		    --��������װ��
		msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	100002)		--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 8 then
	    if Xunlianchang_TXZ == nil then
		    Xunlianchang_TXZ = {}
		end
		Xunlianchang_TXZ[RoleID] = 1
	    local k = math.random(9)
	    role.RoleGotoNewMap(MapID, InstanceID, RoleID, S59_MapID, S59_Point[k].x, S59_Point[k].y, S59_Point[k].z)      --����
	elseif TalkIndex == 9 then
	    if GetTianXuanRoleObj(RoleID) == nil then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206721)		--������ͳ��ӵ£�\n         ��û�б������޷����붷������������
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local k = math.random(9)
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, S59_MapID, S59_Point[k].x, S59_Point[k].y, S59_Point[k].z)      --����
        end
	elseif TalkIndex == 11 then
		local num = role.GetRoleScriptData(RoleID, 1, RoleDataType["Tianxuanzhe_time"])
		if num ~= curtime then
			local FreeSize = role.GetBagFreeSize(RoleID)
			if FreeSize < 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	100003)		--������ͳ��ӵ£�\n         ��û���㹻�ı����ռ䣬����һ�±����ٶһ���װ�������
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 9,	1)	--
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if act.GetActIsStart(47) ~= true and act.GetActIsStart(48) ~= true then
					local point = role.GetRoleScriptData(RoleID, 1, RoleDataType["Tianxuanzhe_Point"])
					local exp1 = math.floor(TianXuan_Exp[rolelevel] * 2.5 * point / 60)
					local exp2 = 8 * point
					local item = 0
					if point > 80 then
						item = 4820242
					elseif point > 40 then
						item = 4820241
					else
						item = 4820240
					end
					role.AddRoleExp(MapID, InstanceID, RoleID, exp1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["Tianxuanzhe_Point"],0)
					role.ModRoleAttValue(MapID, InstanceID, RoleID, 80, exp2)
					role.AddRoleItem(MapID, InstanceID, RoleID, item, 1, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["Tianxuanzhe_time"],curtime)
				end
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206732)		--������ͳ��ӵ£�\n         ������Ѿ��һ���װ����������ٴζһ���������������
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex >= 12 and TalkIndex <= 15 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if role.GetRoleSilver(MapID, InstanceID, RoleID) < 1000000 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206722)		--������ͳ��ӵ£�\n         ��û���㹻�Ľ�ң��޷��һ���װ�������
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if role.GetRoleItemNum(RoleID, 4820081) < 40 then
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206723)		--������ͳ��ӵ£�\n         ��û���㹻��������ң��޷��һ���װ�������
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if FreeSize < 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	206724)		--������ͳ��ӵ£�\n         ��û���㹻�ı����ռ䣬����һ�±����ٶһ���װ�������
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
					msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4820081, 40, 101)
					role.DecRoleSilver(MapID, InstanceID, RoleID, 1000000, 452)
					role.AddRoleItem(MapID, InstanceID, RoleID, 4820243+TalkIndex-12, 1, -1, 8, 420)
				end
			end
		end
	end
end

aux.RegisterCreatureEvent(5610081, 7, "x5610081_OnTalk")

--����������ĶԻ�
function x5610080_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206725)		    --��������������\n         �����뿪�������𣿡�
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
		msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	100002)		--��ȷ����
		if InstanceID ~= 100 and InstanceID ~= 101 then
			msg.AddMsgEvent(MsgID, 21,	7)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	206710)		--���һ�ս��װ����
		end
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if InstanceID == 100 or InstanceID == 101 then
			local num1 =role.GetRoleItemNum(RoleID, 4800283)
			local num2 =role.GetRoleItemNum(RoleID, 4800284)
			if num1 > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4800283, num1, 101)
			end
			if num2 > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4800284, num2, 101)
			end
		end
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, S59_ExitMapID, S59_ExitPoint.x, S59_ExitPoint.y, S59_ExitPoint.z)      --����
	elseif TalkIndex == 7 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206727)		--��������������\n         ��ѡ����װֻ�ܴ���1��Сʱ����ֻ���ڶ�����������ʹ�á��뿪����������ʱ�����������ϵ���ѡ����װ������ʧ��\n         �һ���ѡ����װ��Ҫ100���Լ�40��������ң���ѡ����Ҫ�������ѡ����װ����
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	12)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	206717)		    --�������װ��
		msg.AddMsgEvent(MsgID, 21,	13)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	206718)		    --��������װ��
		msg.AddMsgEvent(MsgID, 21,	14)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	206719)		    --��Ǭ����װ��
		msg.AddMsgEvent(MsgID, 21,	15)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	206720)		    --��������װ��
		msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	100002)		--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex >= 12 and TalkIndex <= 15 then
	    local FreeSize = role.GetBagFreeSize(RoleID)
		if role.GetRoleSilver(MapID, InstanceID, RoleID) < 1000000 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206728)		--��������������\n         ��û���㹻�Ľ�ң��޷��һ���װ�������
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if role.GetRoleItemNum(RoleID, 4820081) < 40 then
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206729)		--��������������\n         ��û���㹻��������ң��޷��һ���װ�������
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if FreeSize < 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	206730)		--��������������\n         ��û���㹻�ı����ռ䣬����һ�±����ٶһ���װ�������
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
					msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
				    role.RemoveFromRole(MapID, InstanceID, RoleID, 4820081, 40, 101)
					role.DecRoleSilver(MapID, InstanceID, RoleID, 1000000, 452)
				    role.AddRoleItem(MapID, InstanceID, RoleID, 4820243+TalkIndex-12, 1, -1, 8, 420)
				end
			end
		end
	end
end

aux.RegisterCreatureEvent(5610080, 7, "x5610080_OnTalk")
