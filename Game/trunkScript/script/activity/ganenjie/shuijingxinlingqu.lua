crystalNPC = 0
function CreateCrystalNPC()
	crystalNPC = map.MapCreateColCreature(3424073512, -1, 5610107, 505, 23510, 759, 1, "")
	local MsgID1 = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID1, 102, 300096)--ˮ��Ů�ͳ���
	msg.DispatchBroadcast(MsgID1,-1,-1,-1)
end

function DeleteCrystalNPC()
	map.MapDeleteCreature(3424073512, -1, crystalNPC)
	crystalNPC = 0
end




function x5610107_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local curDate = tonumber(os.date("%j"))
	local record =role.GetRoleScriptData(RoleID, 1, RoleDataType["shuijingxinlingqu"])
	local lastDate =(record-record%10)/10
	local temp =record%10
    --��ȡ��ҵȼ�
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if rolelevel > 100 then
	    rolelevel = 100
	end
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	213001)		--
		msg.AddMsgEvent(MsgID, 24,	TargetID)
		if curDate ~= lastDate then
			msg.AddMsgEvent(MsgID, 21,	8)			--��ʼ��
			msg.AddMsgEvent(MsgID, 1,	213004)
		else
			msg.AddMsgEvent(MsgID, 21,	9)			--��ȡˮ����
			msg.AddMsgEvent(MsgID, 1,	213005)		--
		end
		msg.AddMsgEvent(MsgID, 21,	10)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	100001)		--��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 8 then
		if role.GetRoleLevel(MapID, InstanceID, RoleID) < 70 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	213006)		--���ȼ����㡱
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    elseif role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 76005) then
			unit.CancelBuff(MapID,InstanceID,RoleID,7600501)
			unit.AddBuff(MapID, InstanceID, RoleID,7600501,RoleID)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["shuijingxinlingqu"], curDate*10)
		else
			unit.AddBuff(MapID, InstanceID, RoleID,7600501,RoleID)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["shuijingxinlingqu"], curDate*10)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	213007)		--���������ˣ��Ϳ�������ȡ�����ˮ������
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
        end
	elseif TalkIndex == 9 then
		if role.GetRoleLevel(MapID, InstanceID, RoleID) < 70 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	213006)		--���ȼ����㡱
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    elseif role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 76005) then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	213003)		--����δ�������޷���ȡˮ����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif temp ~= 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	213002)		--������ȡ����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400056, 5, -1, 8, 420)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["shuijingxinlingqu"], curDate*10+1)
        end
	end
end
aux.RegisterCreatureEvent(5610107, 7, "x5610107_OnTalk")
