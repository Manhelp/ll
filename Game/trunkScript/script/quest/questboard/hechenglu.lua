
quest_hechenglu = {}

--�ϳ�¯5511023   a=������ʯ ��ֵΪ1,b=������ ��ֵΪ2,c=�����۳� ��ֵΪ3
function hecheglu_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

    local i = role.GetRoleItemNum(RoleID, 4010212)--�ж����޺ϳ���
    local a = role.GetRoleItemNum(RoleID, 4010009)--������ʯ
	local b = role.GetRoleItemNum(RoleID, 4010028)--������
	local c = role.GetRoleItemNum(RoleID, 4080026)--�����۳�
	if i >= 1 then
	    if a >= 10 and b >= 5 and c >= 1 then
			local function HCOnTalk(TargetID,RoleID)
				local MsgID = msg.BeginMsgEvent()
				if quest_hechenglu[RoleID] == 0 then
					msg.AddMsgEvent(MsgID, 20, 405007)	-- ��ѡ���һ��ʹ�õ���Ʒ�����ݲ�ͬ��ѡ��˳������յõ�����ƷҲ��һ����
				elseif quest_hechenglu[RoleID] < 10 then
					msg.AddMsgEvent(MsgID, 20, 405008)	-- �����ѡ�ڶ���ʹ�õ���Ʒ�����ݲ�ͬ��ѡ��˳������յõ�����ƷҲ��һ����
				elseif quest_hechenglu[RoleID] < 100 then
					msg.AddMsgEvent(MsgID, 20, 405009)	-- �����ѡ������ʹ�õ���Ʒ�����ݲ�ͬ��ѡ��˳������յõ�����ƷҲ��һ����
				else
					msg.AddMsgEvent(MsgID, 20, 405030)	-- ����ʹ��֮ǰ�Ѿ�ѡ���˳�����ϳ���Ʒ��
					msg.AddMsgEvent(MsgID, 21, 8)	    -- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 100001)	-- ȷ��
				end
				if math.floor(quest_hechenglu[RoleID]%100/10) ~= 1 and math.floor(quest_hechenglu[RoleID]%10) ~= 1 and math.floor(quest_hechenglu[RoleID]/100) ~= 1 then
					msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 405004)	-- ������ʯ
				end
				if math.floor(quest_hechenglu[RoleID]%100/10) ~= 2 and math.floor(quest_hechenglu[RoleID]%10) ~= 2 and math.floor(quest_hechenglu[RoleID]/100) ~= 2 then
					msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ���
					msg.AddMsgEvent(MsgID, 1, 405005)	-- ������
				end
				if math.floor(quest_hechenglu[RoleID]%100/10) ~= 3 and math.floor(quest_hechenglu[RoleID]%10) ~= 3 and math.floor(quest_hechenglu[RoleID]/100) ~= 3 then
					msg.AddMsgEvent(MsgID, 21, 6)	-- ѡ����
					msg.AddMsgEvent(MsgID, 1, 405006)	-- �����۳�
				end
				if quest_hechenglu[RoleID] ~= 0 then
					msg.AddMsgEvent(MsgID, 21, 7)	-- ѡ����
					msg.AddMsgEvent(MsgID, 1, 405031)	-- ���·�����Ʒ
				end
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end

			if quest_hechenglu[RoleID] == nil then
				quest_hechenglu[RoleID] = 0
			end
			if TalkIndex == -1 then
				HCOnTalk(TargetID,RoleID)
			elseif TalkIndex == 4 or TalkIndex == 5 or TalkIndex == 6 then
				local point = TalkIndex - 3
				if quest_hechenglu[RoleID] == 0 then
					quest_hechenglu[RoleID] = quest_hechenglu[RoleID] + point
					HCOnTalk(TargetID,RoleID)
				elseif quest_hechenglu[RoleID] < 10 then
					quest_hechenglu[RoleID] = quest_hechenglu[RoleID] + point * 10
					HCOnTalk(TargetID,RoleID)
				elseif quest_hechenglu[RoleID] < 100 then
					quest_hechenglu[RoleID] = quest_hechenglu[RoleID] + point * 100
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 405030)	-- ȷ����Ҫ�������Ĵ���ϳ���Ʒ��
					msg.AddMsgEvent(MsgID, 21, 8)	    -- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 100001)	-- ȷ��
					msg.AddMsgEvent(MsgID, 21, 7)	    -- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 405031)	-- ����ѡ�����
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			elseif TalkIndex == 7 then
				quest_hechenglu[RoleID] = nil
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 405031)	-- ������ѡ����Ҫ�������Ʒ����
				msg.AddMsgEvent(MsgID, 21, -1)	    -- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 100001)	-- ȷ��
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif TalkIndex == 8 then
				local FreeBag = role.GetBagFreeSize(RoleID)
				if FreeBag < 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 100003)	-- �뱣֤��������һ����λ�����ϳ�
					msg.AddMsgEvent(MsgID, 9, 1)
					msg.AddMsgEvent(MsgID, 21, 8)	    -- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 100001)	-- ȷ��
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					if a >= 10 and b >= 5 and c >= 1 then
						role.RemoveFromRole(MapID, InstanceID, RoleID, 4010009, 10, 402)--10��������ʯ
						role.RemoveFromRole(MapID, InstanceID, RoleID, 4010028, 5, 402)--5��������
						role.RemoveFromRole(MapID, InstanceID, RoleID, 4080026, 1, 402)--1�������۳�
						local Tbl = {}
						Tbl[123] = 4010218
						Tbl[132] = 4010216
						Tbl[213] = 4010217
						Tbl[231] = 4010214
						Tbl[312] = 4010215
						Tbl[321] = 4010213
						if quest_hechenglu[RoleID] ~= nil then
							role.AddRoleItem(MapID, InstanceID, RoleID, Tbl[quest_hechenglu[RoleID]], 1, 0, 0, 402)
						end
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 405023)	-- ���Ĳ��ϲ��㣬��ҪXX��XX,XX��XX��XX��XX����׼��������
						msg.AddMsgEvent(MsgID, 21, -1)	    -- ѡ��һ
						msg.AddMsgEvent(MsgID, 1, 100001)	-- ȷ��
						msg.AddMsgEvent(MsgID, 24, TargetID)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
					quest_hechenglu[RoleID] = nil
				end
			end
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 405023)	-- ���Ĳ��ϲ��㣬��ҪXX��XX,XX��XX��XX��XX����׼��������
			msg.AddMsgEvent(MsgID, 21, -1)	    -- ѡ��һ
			msg.AddMsgEvent(MsgID, 1, 100001)	-- ȷ��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	else
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 20, 405021)--���û�кϳ���������ʹ��ħ��¯
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
	end

end
aux.RegisterCreatureEvent(5511023, 7, "hecheglu_OnTalk")

function hechenglu_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local i = role.GetRoleItemNum(RoleID, 4010212)
	if i > 0 then
		role.RemoveFromRole(MapID, InstanceID, RoleID,4010212, 1, 402)
	end

end


aux.RegisterQuestEvent(60192,11,"hechenglu_OnComplete")
aux.RegisterQuestEvent(60193,11,"hechenglu_OnComplete")
aux.RegisterQuestEvent(60194,11,"hechenglu_OnComplete")
aux.RegisterQuestEvent(60195,11,"hechenglu_OnComplete")
aux.RegisterQuestEvent(60196,11,"hechenglu_OnComplete")
aux.RegisterQuestEvent(60197,11,"hechenglu_OnComplete")
aux.RegisterQuestEvent(60198,11,"hechenglu_OnComplete")
aux.RegisterQuestEvent(60200,11,"hechenglu_OnComplete")
aux.RegisterQuestEvent(60201,11,"hechenglu_OnComplete")
aux.RegisterQuestEvent(60202,11,"hechenglu_OnComplete")
aux.RegisterQuestEvent(60203,11,"hechenglu_OnComplete")
aux.RegisterQuestEvent(60204,11,"hechenglu_OnComplete")
aux.RegisterQuestEvent(60205,11,"hechenglu_OnComplete")
aux.RegisterQuestEvent(60206,11,"hechenglu_OnComplete")