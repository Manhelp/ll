--��è�峤����
function XMzuzhang_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local curmon = tonumber(os.date("%m"))--��
	local curday = tonumber(os.date("%d"))--��
	local curyear = tonumber(os.date("%Y"))--��
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444438)		--����è�峤:\n    1��18�յ�30�տɽ���ŵ�Ƿ��۳�����\n    ŵ�Ƿ�������һ��������ս�ĵط���ֻ�е���һ��ˮƽ������ս��ֻ�е���60�����ϵ��˲ſ��Խ��롣\n    ���뷽����Ҫ����һ�ŷ��۴�Ʊ��
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 444442)		--������ŵ�Ƿ��ۡ�
		msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if curyear == 2012 and ((curmon == 1 and curday >= 17) or (curmon == 2 and curday <= 10)) then
			if level < 60 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444455)		--������û�з��۴�Ʊ���޷�����ŵ�Ƿ���
				msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21, 6)
				msg.AddMsgEvent(MsgID, 1, 100001)		--ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif role.GetRoleItemNum(RoleID, 4830025) >= 1 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4830025, 1, 420)--��Ʊ
				role.RoleGotoNewMap(MapID, InstanceID, RoleID, 1232817742, 1174, 2557, 1070)--1174��2557��1070
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444439)		--������û�з��۴�Ʊ���޷�����ŵ�Ƿ���
				msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21, 6)
				msg.AddMsgEvent(MsgID, 1, 100001)		--ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444440)		--��������ʱ���ڣ��޷���ŭ�Ƿ���
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 6)
			msg.AddMsgEvent(MsgID, 1, 100001)		--ȷ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(6040020, 7, "XMzuzhang_OnTalk")
function XMfalao_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444441)		--����è����:\n    ��ȷ��Ҫ�뿪������������뿪�ˣ���Ҫ��������Ҫ�ٴ�����һ�Ŵ�Ʊ�ġ�����˼�����С���
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100001)		--ȷ��
		msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3424073512, 617, 23510, 594)--617��23510��594
	end
end
aux.RegisterCreatureEvent(6040021, 7, "XMfalao_OnTalk")






