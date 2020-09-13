--Ұ��
function c5610023_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local CurTime = tonumber(os.date("%j"))
	local LastTime = math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"])/100)
	if CurTime ~= LastTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"], CurTime*100)
	end
	local index = role.GetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"]) % 100
	if TalkIndex == -1 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 442052)		--������ȡ�ˣ�\n    ����㲻С��Ū�����������׸������Ҫ��Ʒ���ҿ��԰���������Ҫһ���������Ժ������С��һ���ˣ��ҿɲ���ÿһ�ζ�Ϊ�����С��ȥ�����������ס�
		msg.AddMsgEvent(MsgID, 24, TargetID)
		--��꾻��1
		if role.IsRoleCompleteQuest(MapID, InstanceID, RoleID, 40057) and role.GetRoleItemNum(RoleID, 4810004) < 1 and role.IsLearnedSkill(RoleID, 20107) == nil and  index%2 == 0 then
		    msg.AddMsgEvent(MsgID, 21, 4)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 442053)		--��ȡ��꾻��������
		end
		--����2
		if role.IsRoleCompleteQuest(MapID, InstanceID, RoleID, 40058) and role.GetRoleItemNum(RoleID, 4810007) < 1 and role.IsLearnedSkill(RoleID, 90005) == nil and  index%4 - index%2 == 0 then
		    msg.AddMsgEvent(MsgID, 21, 5)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 442054)		--��ȡ���ۼ�����
		end
		--��4
		if role.IsRoleCompleteQuest(MapID, InstanceID, RoleID, 40059) and role.GetRoleItemNum(RoleID, 4622001) < 1 and role.IsLearnedSkill(RoleID, 23000) == nil and  index%8 - index%4 == 0 then
		    msg.AddMsgEvent(MsgID, 21, 6)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 442055)		--��ȡ�񻯼�����
		end
		--����8
		if role.IsRoleCompleteQuest(MapID, InstanceID, RoleID, 40060) and role.GetRoleItemNum(RoleID, 4630301) < 1 and index%16 - index%8 == 0 then
		    msg.AddMsgEvent(MsgID, 21, 7)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 442056)		--��ȡ��ѡ��������
		end
        msg.DispatchRoleMsgEvent(RoleID, MsgID)
	else
	    if role.GetBagFreeSize(RoleID) < 1 then
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 20, 100003)
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.AddMsgEvent(MsgID, 21, 8)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if TalkIndex == 4 then
			    role.SetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"], role.GetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"])+1)
			    role.AddRoleItem(MapID, InstanceID, RoleID, 4810004, 1, -1, 8, 420)
			elseif TalkIndex == 5 then
			    role.SetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"], role.GetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"])+2)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4810007, 1, -1, 8, 420)
			elseif TalkIndex == 6 then
			    role.SetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"], role.GetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"])+4)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4622001, 1, -1, 8, 420)
			elseif TalkIndex == 7 then
			    role.SetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"], role.GetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"])+8)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4630301, 1, -1, 8, 420)
			end
		end
	end
end
-- ע��
aux.RegisterCreatureEvent(5610023, 7, "c5610023_OnTalk")

