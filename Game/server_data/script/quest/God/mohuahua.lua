--ħ��ѩ����
function c5610007_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local MsgID = msg.BeginMsgEvent()
	if TalkIndex == -1 then
		msg.AddMsgEvent(MsgID, 20, 442034)		--����һ�걻��Ⱦ��ѩ������ʹ��һ������ҩ�����Խ��侻��
		msg.AddMsgEvent(MsgID, 21, 4)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
		msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100002)			--��ȡ����
		msg.AddMsgEvent(MsgID, 24, TargetID)
	elseif TalkIndex == 4 then
		if role.GetRoleItemNum(RoleID, 4810005) < 1 then
			msg.AddMsgEvent(MsgID, 20, 442042)		--ȱ�پ���ҩ��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4810005, 1, 401)
			getFaith(MapID, InstanceID, 50, RoleID)
			local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
			map.MapCreateColCreature(MapID, InstanceID, 6040007, x, y, z, 1, "")
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
			msg.AddMsgEvent(MsgID, 20, 442038)		--����Ⱦ��ѩ�������������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
		end
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
-- ע��
aux.RegisterCreatureEvent(5610007, 7, "c5610007_OnTalk")


--ħ�����黨
function c5610008_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local MsgID = msg.BeginMsgEvent()
	if TalkIndex == -1 then
		msg.AddMsgEvent(MsgID, 20, 442035)		--����һ�걻��Ⱦ�����黨��ʹ��һ������ҩ�����Խ��侻��
		msg.AddMsgEvent(MsgID, 21, 4)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
		msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100002)			--��ȡ����
		msg.AddMsgEvent(MsgID, 24, TargetID)
	elseif TalkIndex == 4 then
		if role.GetRoleItemNum(RoleID, 4810005) < 1 then
			msg.AddMsgEvent(MsgID, 26, 442042)		--ȱ�پ���ҩ��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4810005, 1, 401)
			getFaith(MapID, InstanceID, 50, RoleID)
			local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
			map.MapCreateColCreature(MapID, InstanceID, 6040008, x, y, z, 1, "")
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
			msg.AddMsgEvent(MsgID, 20, 442039)		--����Ⱦ�����黨���������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
		end
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
-- ע��
aux.RegisterCreatureEvent(5610008, 7, "c5610008_OnTalk")


--ħ�����ﻨ
function c5610009_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local MsgID = msg.BeginMsgEvent()
	if TalkIndex == -1 then
		msg.AddMsgEvent(MsgID, 20, 442036)		--����һ�걻��Ⱦ�����ﻨ��ʹ��һ������ҩ�����Խ��侻��
		msg.AddMsgEvent(MsgID, 21, 4)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
		msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100002)			--��ȡ����
		msg.AddMsgEvent(MsgID, 24, TargetID)
	elseif TalkIndex == 4 then
		if role.GetRoleItemNum(RoleID, 4810005) < 1 then
			msg.AddMsgEvent(MsgID, 26, 442042)		--ȱ�پ���ҩ��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4810005, 1, 401)
			getFaith(MapID, InstanceID, 50, RoleID)
			local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
			map.MapCreateColCreature(MapID, InstanceID, 6040009, x, y, z, 1, "")
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
			msg.AddMsgEvent(MsgID, 20, 442040)		--����Ⱦ�����ﻨ���������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
		end
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
-- ע��
aux.RegisterCreatureEvent(5610009, 7, "c5610009_OnTalk")


--ħ�����ﻨ
function c5610010_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local MsgID = msg.BeginMsgEvent()
	if TalkIndex == -1 then
		msg.AddMsgEvent(MsgID, 20, 442037)		--����һ�걻��Ⱦ��ѩ������ʹ��һ������ҩ�����Խ��侻��
		msg.AddMsgEvent(MsgID, 21, 4)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
		msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100002)			--��ȡ����
		msg.AddMsgEvent(MsgID, 24, TargetID)
	elseif TalkIndex == 4 then
		if role.GetRoleItemNum(RoleID, 4810005) < 1 then
			msg.AddMsgEvent(MsgID, 26, 442042)		--ȱ�پ���ҩ��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4810005, 1, 401)
			getFaith(MapID, InstanceID, 50, RoleID)
			local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
			map.MapCreateColCreature(MapID, InstanceID, 6040010, x, y, z, 1, "")
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
			msg.AddMsgEvent(MsgID, 20, 442041)		--����Ⱦ�����ﻨ���������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
		end
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
-- ע��
aux.RegisterCreatureEvent(5610010, 7, "c5610010_OnTalk")
