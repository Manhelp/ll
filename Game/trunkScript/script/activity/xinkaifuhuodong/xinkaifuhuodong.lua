tianshihulalal = 0 
tianshihuhaha = 0 
--act.GetActScriptData(55, 1, 4)--����ȡ
--act.GetActScriptData(55, 1, 2)--����ȡ��ID
function n6040056_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local act55kai = act.GetActScriptData(55, 1, 4)--�����
	local act55ren = act.GetActScriptData(55, 1, 2)--��һ������80�������ID
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206630) -- "��ʹ������:\n    �µ������µĿ�ʼ��\n    ֻ��80�����ϲ��ҵ�һ�������˵ص��˲�����ȡ���������ҿ���ÿ���������
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, 4) -- 
		msg.AddMsgEvent(MsgID, 1, 206631) -- ��ȡ����
		msg.AddMsgEvent(MsgID, 21, 5) -- 
		msg.AddMsgEvent(MsgID, 1, 206632) -- �鿴��ȡ������
		msg.AddMsgEvent(MsgID, 21, 10)   --
		msg.AddMsgEvent(MsgID, 1, 100002)   --"ȡ��"
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	elseif TalkIndex == 4 then--580��23510��881 NPC ����
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level >= 80 then
			if act55kai ~= 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206633) -- "��ʹ������\n    ��ȡ����������Ľ����ɡ�
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21,6) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			else
				local MsgID = msg.BeginMsgEvent()
				if RoleID ~= act55ren then
					msg.AddMsgEvent(MsgID, 20, 206634) -- "��ʹ������\n    ��������Ѿ�������ȡ�ˣ�������ڡ��鿴��ȡ���������в鿴�������
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, 5) -- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 206632) -- ��ȷ��
				else
					msg.AddMsgEvent(MsgID, 20, 206637) -- ��ʹ������:\n    ���Ѿ���ȡ�������ˣ����ظ���ȡ�Ͳ���Ŷ��
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				end
				msg.AddMsgEvent(MsgID, 21, 10) -- 
				msg.AddMsgEvent(MsgID, 1, 100002) -- ȡ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206639) -- 
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, 10) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	elseif TalkIndex == 5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206635) -- "��ʹ������:\n    xx�����������һ����Ϊȫ��80������ң����ɹ�������������ÿ���������
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 2, act55ren)
		msg.AddMsgEvent(MsgID, 21, 10)   --
		msg.AddMsgEvent(MsgID, 1, 100001)   --"ȷ��"
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	elseif TalkIndex == 6 then
		if role.GetBagFreeSize(RoleID) < 4 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206636) -- "��ʹ������:\n    ��ı����ռ䲻��3
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, 7)   --
			msg.AddMsgEvent(MsgID, 1, 100001)   --"ȷ��"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
			local act55ling = act.GetActScriptData(55, 1, 3)
			if act55ling ~=1 then
				act.SetActScriptData(55, 1, 3, 1)
				act.SetActScriptData(55, 1, 2, RoleID)
				act.SetActScriptData(55, 1, 4, 1)
				act.SaveActScriptData(55)	
				tianshihulalal = map.MapCreateColCreature(3424073512, -1, 6040055, 580, 23510, 881, 1, "")
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102,300101)      --��ʹ�������Ѿ������ڶ�ս���<580,881>������������ҿ���ȥ��������ȡÿ��������������
				msg.DispatchWorldMsgEvent(MsgID)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820262, 9999, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 3020095, 1, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 3020097, 1, -1, 8, 420)
				role.AddRoleSilver(MapID, InstanceID, RoleID, 20000000)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206637) -- ��ʹ������:\n    ���Ѿ���ȡ�������ˣ����ظ���ȡ�Ͳ���Ŷ��
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, 10) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end
		end
	end 
end

aux.RegisterCreatureEvent(6040056, 7, "n6040056_OnTalk")

function xinkaifuhuodong_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local high,low = role.GetRoleLevelUpExp(MapID, InstanceID, RoleID)--�õ�����������辭��
	local high1,low1 = role.GetRoleExp(MapID, InstanceID, RoleID)--�õ���ҵ�ǰ���龭��
	local x = (high*1000000000 + low) - (high1*1000000000 + low1)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if level >= 10 and level <= 84 then
		role.AddRoleExp(MapID, InstanceID, RoleID, x)
	end
end
aux.RegisterQuestEvent(30797, 1, "xinkaifuhuodong_Complete")
function act55_OnTimerMin(actID)
	if tianshihulalal == 0 then
		local act55kai = act.GetActScriptData(55, 1, 4)--�����
		if xinkaifuhuodong == 1 then
			if act55kai == 1 then
				tianshihulalal = map.MapCreateColCreature(3424073512, -1, 6040055, 580, 23510, 881, 1, "")
			end
		end
	end
end
aux.RegisterActEvent(55, 4, "act55_OnTimerMin")