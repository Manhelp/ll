function s60_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	local hunjingtype = role.GetEquipIDOnPos(RoleID, 13)
	if ObjID == 287 then
		if hunjingtype == 8220401 or hunjingtype == 8220403 or (hunjingtype >= 8220501 and hunjingtype <= 8220599) then
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648950, 26, 200, 32)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 444304)				--������Я����꾧װ�����ܽ��볡��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
	if ObjID == 288 then
		if hunjingtype == 8220402 or hunjingtype == 8220404 or (hunjingtype >= 8220601 and hunjingtype <= 8220699) then
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52649974, 440, 1, 768)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 444309)				--������Я��ħ�꾧װ�����ܽ��볡��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterMapEvent("s60",6,"s60_OnEnterTrigger")


--ͨ�����紫����s02ˢ
function c5610069_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444302)				--ȷ��Ҫͨ����磿
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)
		msg.AddMsgEvent(MsgID, 1, 100001)					--��ȷ��
		msg.AddMsgEvent(MsgID, 21, 5)
		msg.AddMsgEvent(MsgID, 1, 100002)				--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if rolelevel >= 80 then
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3390518568, 638, 138, 595)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444303)				--�ȼ����ٴﵽ80��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 5)
			msg.AddMsgEvent(MsgID, 1, 100002)				--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5610069, 7, "c5610069_On_Talk")

--����ڻ�е��ߺ�����
function s60_CanTakeOverWhenOnline(MapID, RoleID)
	return 3390518568, 638, 6913, 595
end
aux.RegisterMapEvent("s60",  12, "s60_CanTakeOverWhenOnline")

--�ҵ���ͼ�ĳ��ں�����
function s60_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3424073512, 610+math.random(5), 23094, 669+math.random(5)
end
aux.RegisterMapEvent("s60",  13, "s60_GetExporMapAndCoord")

--�ҵ�����ʵĸ���ʵ��
function s60GetOnePerfectMap(MapID, InstanceID, RoleID)
	return 1
end
aux.RegisterMapEvent("s60",  14, "s60GetOnePerfectMap")

--�������Ƿ��ܹ���������
function s60_CanEnter(MapID, InstanceID, RoleID)
	if role.GetRoleLevel(MapID, InstanceID, RoleID) >= 80 then
		return 1
	else
		return 0
	end
end
aux.RegisterMapEvent("s60",  15, "s60_CanEnter")

-- �������ĵ��ҹ�ϵ�ж� -- ��ͬɫ����ȫ�ǵ���
function s60_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
	-- �������
	if unit.IsPlayer(SrcID) and unit.IsPlayer(TargetID) then
        return true, false, false, true
	end

	return false, false, false, false
end

--ע��û������¼�
aux.RegisterMapEvent("s60", 16, "s60_FriendEnemy")
aux.RegisterMapEvent("s57_1", 16, "s60_FriendEnemy")
aux.RegisterMapEvent("s57_2", 16, "s60_FriendEnemy")
aux.RegisterMapEvent("s57_3", 16, "s60_FriendEnemy")
aux.RegisterMapEvent("s57_4", 16, "s60_FriendEnemy")

function s57_1_OnPlayerEnter(MapID, InstanceID,RoleID)
    local EquipID = role.GetEquipIDOnPos(RoleID, 13)
	if EquipID ~= nil then
		if (EquipID == 8220401 or EquipID == 8220403 or (EquipID >= 8220501 and EquipID <= 8220599)) then
			unit.AddBuff(MapID, InstanceID, RoleID, 5941702, RoleID)
		elseif (EquipID == 8220402 or EquipID == 8220404 or (EquipID >= 8220601 and EquipID <= 8220699)) then
			unit.AddBuff(MapID, InstanceID, RoleID, 5941902, RoleID)
		end
	end
end
--ע��
aux.RegisterMapEvent("s57_1", 2, "s57_1_OnPlayerEnter")
aux.RegisterMapEvent("s57_2", 2, "s57_1_OnPlayerEnter")
aux.RegisterMapEvent("s57_3", 2, "s57_1_OnPlayerEnter")
aux.RegisterMapEvent("s57_4", 2, "s57_1_OnPlayerEnter")
aux.RegisterMapEvent("s57_5", 2, "s57_1_OnPlayerEnter")
aux.RegisterMapEvent("s57_6", 2, "s57_1_OnPlayerEnter")
aux.RegisterMapEvent("s57_7", 2, "s57_1_OnPlayerEnter")


