--aux.MultiRateReward(���ͣ����ʣ�����ʱ��) 
--���ͣ�0���� 1���ʣ����ʣ�100����100%������ʱ�䣺����Ϊ��λ���޷���ֵ

--�������
function tianxiafuliren_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --��ȡ��ҵȼ�
	local FreeSize = role.GetBagFreeSize(RoleID)
	local buff = role.IsRoleHaveBuff(MapID, InstanceID,RoleID,69371)
	local a = role.GetRoleItemNum(RoleID, 4820036)
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if rolelevel >= 50 then
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206488)		--��ȡ������Ķ����ɣ�ĪҪ̰�ư�~~~
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 4)			--��ȡ����
			msg.AddMsgEvent(MsgID, 1, 206489)		
			msg.AddMsgEvent(MsgID, 21, 5)			--�һ���ڤ������Ƭ
			msg.AddMsgEvent(MsgID, 1, 206490)		
			msg.AddMsgEvent(MsgID, 21, 6)			
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 4 then
			if buff == true then
				if FreeSize >= 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4820036, 1, -1, 8, 420) 
					unit.CancelBuff(MapID, InstanceID, RoleID,6937101) 
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 100003)
					msg.AddMsgEvent(MsgID, 9, 1)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206487)		--������û��ף��״̬
				msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21, 6)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1, 100001)		--ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 5 then
			if a >=1 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4820036, 1, 101)
				role.AddRoleExp(MapID, InstanceID, RoleID, 1000000)
				fuli_jifen()
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206486)		--������û����ڤ������Ƭ
				msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21, 6)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1, 100001)		--ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206493)		
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	6)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end
aux.RegisterCreatureEvent(5610054, 7, "tianxiafuliren_OnTalk")
function fuli_jifen()
	local x = act.GetActScriptData(42, 1, 1)
	if x < 100 then
		act.SetActScriptData(42, 1, 1, x+1)
	else
		act.SetActScriptData(42, 1, 1, 0)
		aux.MultiRateReward(0, 200, 10800) 
		aux.MultiRateReward(1, 200, 10800) 
	end
	act.SaveActScriptData(42)

end