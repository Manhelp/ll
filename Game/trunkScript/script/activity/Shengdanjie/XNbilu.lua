
function XNbilu_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local a = role.GetRoleItemNum(RoleID, 4800010)--�������
	local b = role.GetRoleItemNum(RoleID, 4800011)--����
	local c = role.GetRoleItemNum(RoleID, 4800012)--����
	local d = role.GetRoleItemNum(RoleID, 4800013)--С�����
	local e = role.GetRoleItemNum(RoleID, 4800014)--��Ѭ����
	local f = role.GetRoleItemNum(RoleID, 4800015)--ʥ����
	local g = role.GetRoleItemNum(RoleID, 4800016)--��˹ͼ���
	local h = role.GetRoleItemNum(RoleID, 4800020)--�߼�����
	local i = role.GetRoleItemNum(RoleID, 4800021)--������

	local FreeBag = role.GetBagFreeSize(RoleID)
	local x = act.GetActIsStart(36)--����
	if x == true then
		if FreeBag < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 100003)	-- �뱣֤��������һ����λ�����ϳ�
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.AddMsgEvent(MsgID, 21, -1)	    -- ѡ��һ
			msg.AddMsgEvent(MsgID, 1, 100001)	-- ȷ��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if TalkIndex == -1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206317)	-- ��ѡ����Ҫ������ʳ��
				msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 206318)	-- ����С�����
				msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ���
				msg.AddMsgEvent(MsgID, 1, 206319)	-- ������Ѭ����
				msg.AddMsgEvent(MsgID, 21, 6)	-- ѡ����
				msg.AddMsgEvent(MsgID, 1, 206320)	-- ����ʥ����
				msg.AddMsgEvent(MsgID, 21, 7)	-- ѡ����
				msg.AddMsgEvent(MsgID, 1, 206321)	-- ����������
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
			if TalkIndex == 4 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206322)	-- ����С����ɣ�ÿ����һ��С����ɾ���Ҫ�������2��������1��������2������ȷ��Ҫ����С�����ô��
				msg.AddMsgEvent(MsgID, 21, 8)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 100001)	-- ȷ��
				msg.AddMsgEvent(MsgID, 21, -1)	-- ѡ���
				msg.AddMsgEvent(MsgID, 1, 100002)	-- ȡ��
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			end
			if TalkIndex == 8 then
				if a >= 2 and b >= 1 and c >= 2 then
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800010, 2, 402)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800011, 1, 402)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800012, 2, 402)
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800013, 1, 0, 0, 402)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 405023)	-- ������Ʒ�����޷��ϳɣ��뼯����Ʒ�����ϳ�
					msg.AddMsgEvent(MsgID, 21, -1)	    -- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 100001)	-- ȷ��
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
			if TalkIndex == 5 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206330)	-- ������Ѭ����,ÿ����һ����Ѭ���Ⱦ���Ҫ����2��������4�����߼�����1������ȷ��Ҫ������Ѭ����ô��
				msg.AddMsgEvent(MsgID, 21, 9)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 100001)	-- ȷ��
				msg.AddMsgEvent(MsgID, 21, -1)	-- ѡ���
				msg.AddMsgEvent(MsgID, 1, 100002)	-- ȡ��
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
			if TalkIndex == 9 then
				if c >= 2 and b >= 4 and h >= 1 then
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800012, 2, 402)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800011, 4, 402)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800020, 1, 402)
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800014, 1, 0, 0, 402)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 405023)	-- ������Ʒ�����޷��ϳɣ��뼯����Ʒ�����ϳ�
					msg.AddMsgEvent(MsgID, 21, -1)	    -- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 100001)	-- ȷ��
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
			if TalkIndex == 6 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206323)	-- ����ʥ���𼦣�ÿ����һ��ʥ���𼦾���Ҫ����5��������5����������1������ȷ��Ҫ����ʥ����ô��
				msg.AddMsgEvent(MsgID, 21, 10)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 100001)	-- ȷ��
				msg.AddMsgEvent(MsgID, 21, -1)	-- ѡ���
				msg.AddMsgEvent(MsgID, 1, 100002)	-- ȡ��
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			end
			if TalkIndex == 10 then
				if c >= 5 and b >= 5 and i >= 1 then
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800012, 5, 402)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800011, 5, 402)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800021, 1, 402)
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800015, 1, 0, 0, 402)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 405023)	-- ������Ʒ�����޷��ϳɣ��뼯����Ʒ�����ϳ�
					msg.AddMsgEvent(MsgID, 21, -1)	    -- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 100001)	-- ȷ��
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
			if TalkIndex == 7 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206324)	-- ���������ͣ�ÿ����һ�������;���Ҫ��ͼ˹���1������Ѭ����2����ʥ����1�� С�����5������ȷ��Ҫ����������ô��
				msg.AddMsgEvent(MsgID, 21, 11)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 100001)	-- ȷ��
				msg.AddMsgEvent(MsgID, 21, -1)	-- ѡ���
				msg.AddMsgEvent(MsgID, 1, 100002)	-- ȡ��
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
			if TalkIndex == 11 then
				if g >= 1 and e >= 1 and f >= 1 and d >= 5 then
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800016, 1, 402)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800014, 1, 402)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800015, 1, 402)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800013, 5, 402)
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800017, 1, 0, 0, 402)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 405023)	-- ������Ʒ�����޷��ϳɣ��뼯����Ʒ�����ϳ�
					msg.AddMsgEvent(MsgID, 21, -1)	    -- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 100001)	-- ȷ��
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 206331)--���û�п�ʼ
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
	end
end
aux.RegisterCreatureEvent(5600014, 7, "XNbilu_OnTalk")
