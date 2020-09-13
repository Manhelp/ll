--��ʳ���չ��̶Ի�
function mofabu_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local num1 = role.GetRoleItemNum(RoleID, 4100201)
	local num2 = role.GetRoleItemNum(RoleID, 4100202)
	local num3 = role.GetRoleItemNum(RoleID, 4100203)
	local num4 = role.GetRoleItemNum(RoleID, 4100204)
	local num5 = role.GetRoleItemNum(RoleID, 4100205)
	local num6 = role.GetRoleItemNum(RoleID, 4100206)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 420052)	--���ܶһ�ħ�����ı���
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 11)		--�Ƿ���Ҫ�һ�ħ����
		msg.AddMsgEvent(MsgID, 1,  420053)	--����Ҫ�һ�ħ������
		msg.AddMsgEvent(MsgID, 21, 10)		--ȡ������10
		msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 11 then		--�һ�ħ����
		local MsgID = msg.BeginMsgEvent()
		if num1 < 12 and num2 < 12 and num3 < 12 and num4 < 12 and num5 < 12 and num6 < 12 then
			msg.AddMsgEvent(MsgID, 20, 420061)	--������Ҫ����12�������ʵ�ſɶһ���
		    msg.AddMsgEvent(MsgID, 21, 10)		--������һ�
			msg.AddMsgEvent(MsgID, 1,  420009)	--
		else
			msg.AddMsgEvent(MsgID, 20, 420054)	--��ѡ������ʲô�һ�
		end
		msg.AddMsgEvent(MsgID, 24, TargetID)
		if num1 >= 12 then
			msg.AddMsgEvent(MsgID, 21, 12)		--������һ�
			msg.AddMsgEvent(MsgID, 1,  420055)	--
		end
		if num2 >= 12 then
			msg.AddMsgEvent(MsgID, 21, 13)		--�û��Ҷһ�
			msg.AddMsgEvent(MsgID, 1,  420056)	--
		end
		if num3 >= 12 then
			msg.AddMsgEvent(MsgID, 21, 14)		--��������һ�
			msg.AddMsgEvent(MsgID, 1,  420057)	--
		end
		if num4 >= 12 then
			msg.AddMsgEvent(MsgID, 21, 15)		--��ˮ���۶һ�
			msg.AddMsgEvent(MsgID, 1,  420058)	--
		end
		if num5 >= 12 then
			msg.AddMsgEvent(MsgID, 21, 16)		--����÷�һ�
			msg.AddMsgEvent(MsgID, 1,  420059)	--
		end
		if num6 >= 12 then
			msg.AddMsgEvent(MsgID, 21, 17)		--��Ҭ�׶һ�
			msg.AddMsgEvent(MsgID, 1,  420060)	--
		end
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 12 then		--�һ�ħ����
		local num = role.GetRoleItemNum(RoleID, 4100201)
		local FreeSize = role.GetBagFreeSize(RoleID)
		if num >= 12 and FreeSize >= 1 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100201, 12, 420)	--ɾ��12�����
			role.AddRoleItem(MapID, InstanceID, RoleID, 3050109, 1, -1, 8, 420)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420062)	---���Ի�����ʾ������û���㹻�ĵ��ߡ�
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 10)		--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100002)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 13 then		--�һ�ħ����
		local num = role.GetRoleItemNum(RoleID, 4100202)
		local FreeSize = role.GetBagFreeSize(RoleID)
		if num >= 12 and FreeSize >= 1 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100202, 12, 420)	--ɾ��12������
			role.AddRoleItem(MapID, InstanceID, RoleID, 3050109, 1, -1, 8, 420)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420062)	---���Ի�����ʾ������û���㹻�ĵ��ߡ�
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 10)		--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100002)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 14 then		--�һ�ħ����
		local num = role.GetRoleItemNum(RoleID, 4100203)
		local FreeSize = role.GetBagFreeSize(RoleID)
		if num >= 12 and FreeSize >= 1 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100203, 12, 420)	--ɾ��12��������
			role.AddRoleItem(MapID, InstanceID, RoleID, 3050109, 1, -1, 8, 420)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420062)	---���Ի�����ʾ������û���㹻�ĵ��ߡ�
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 10)		--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100002)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 15 then		--�һ�ħ����
		local num = role.GetRoleItemNum(RoleID, 4100204)
		local FreeSize = role.GetBagFreeSize(RoleID)
		if num >= 12 and FreeSize >= 1 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100204, 12, 420)	--ɾ��12��ˮ����
			role.AddRoleItem(MapID, InstanceID, RoleID, 3050109, 1, -1, 8, 420)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420062)	---���Ի�����ʾ������û���㹻�ĵ��ߡ�
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 10)		--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100002)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 16 then		--�һ�ħ����
		local num = role.GetRoleItemNum(RoleID, 4100205)
		local FreeSize = role.GetBagFreeSize(RoleID)
		if num >= 12 and FreeSize >= 1 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100205, 12, 420)	--ɾ��12����÷
			role.AddRoleItem(MapID, InstanceID, RoleID, 3050109, 1, -1, 8, 420)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420062)	---���Ի�����ʾ������û���㹻�ĵ��ߡ�
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 10)		--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100002)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 17 then		--�һ�ħ����
		local num = role.GetRoleItemNum(RoleID, 4100206)
		local FreeSize = role.GetBagFreeSize(RoleID)
		if num >= 12 and FreeSize >= 1 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100206, 12, 420)	--ɾ��12��Ҭ��
			role.AddRoleItem(MapID, InstanceID, RoleID, 3050109, 1, -1, 8, 420)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420062)	---���Ի�����ʾ������û���㹻�ĵ��ߡ�
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 10)		--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100002)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5516003, 7, "mofabu_OnTalk")
