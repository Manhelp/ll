--Ұ��
function c5610004_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local MsgID = msg.BeginMsgEvent()
	if TalkIndex == -1 then
		if cre.IsHaveBuff(MapID, InstanceID, TargetID, 61002) == true then
			msg.AddMsgEvent(MsgID, 20, 442043)		--����һֻ���Ǳ����Ұ��ʹ��һ������ҩ����������ƽ������
			msg.AddMsgEvent(MsgID, 21, 4)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001)		--��ȷ����
			msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
		else
			msg.AddMsgEvent(MsgID, 20, 442044)		--��~~��~~
		    msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
		end
	elseif TalkIndex == 4 then
	    if cre.IsHaveBuff(MapID, InstanceID, TargetID, 61002) == true then
			if role.GetRoleItemNum(RoleID, 4810005) < 1 then
				msg.AddMsgEvent(MsgID, 20, 442042)		--ȱ�پ���ҩ��
			    msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
			elseif role.GetBagFreeSize(RoleID) < 1 then
				msg.AddMsgEvent(MsgID, 20, 100003)		--ȱ�پ���ҩ��
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 9, 1)
			    msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4810005, 1, 401)
				unit.CancelBuff(MapID, InstanceID, TargetID, 6100201)
				getFaith(MapID, InstanceID, 20, RoleID)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4810019, 1, -1, 1, 104)
				msg.AddMsgEvent(MsgID, 15, 442045)		--�����Ұ���Ѿ��ָ���ƽ��
			    msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
			end
		end
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
-- ע��
aux.RegisterCreatureEvent(5610004, 7, "c5610004_OnTalk")


--Ұ��
function c5610005_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local MsgID = msg.BeginMsgEvent()
	if TalkIndex == -1 then
		if cre.IsHaveBuff(MapID, InstanceID, TargetID, 61002) == true then
			msg.AddMsgEvent(MsgID, 20, 442046)		--����һֻ���Ǳ����Ұ�ܣ�ʹ��һ������ҩ����������ƽ������
			msg.AddMsgEvent(MsgID, 21, 4)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
			msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
		else
			msg.AddMsgEvent(MsgID, 20, 442047)		--��~~��~~
			msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
		end
	elseif TalkIndex == 4 then
	    if cre.IsHaveBuff(MapID, InstanceID, TargetID, 61002) == true then
			if role.GetRoleItemNum(RoleID, 4810005) < 1 then
				msg.AddMsgEvent(MsgID, 20, 300001)		--ȱ�پ���ҩ��
				msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
			elseif role.GetBagFreeSize(RoleID) < 1 then
				msg.AddMsgEvent(MsgID, 20, 100003)		--ȱ�پ���ҩ��
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 9, 1)
			    msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4810005, 1, 401)
				unit.CancelBuff(MapID, InstanceID, TargetID, 6100201)
				getFaith(MapID, InstanceID, 15, RoleID)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4810019, 1, -1, 1, 104)
			 	msg.AddMsgEvent(MsgID, 20, 442048)		--�����Ұ���Ѿ��ָ���ƽ��
			    msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
			end
		end
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
-- ע��
aux.RegisterCreatureEvent(5610005, 7, "c5610005_OnTalk")
