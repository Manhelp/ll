
quest_hechenglu = {}

--�ϳ�¯5511023   a=������ʯ ��ֵΪ1,b=������ ��ֵΪ2,c=�����۳� ��ֵΪ3
function hecheglu1_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local i = role.GetRoleItemNum(RoleID, 4010212)--�ж����޺ϳ���
	local a = role.GetRoleItemNum(RoleID, 4010015)--ƽ�����꣨4010015��
	local b = role.GetRoleItemNum(RoleID, 4010034)--ڤɽ��ʯ��4010034��
	local c = role.GetRoleItemNum(RoleID, 4080041)--����(4080041)
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
					msg.AddMsgEvent(MsgID, 1, 405018)	-- ƽ������
				end
				if math.floor(quest_hechenglu[RoleID]%100/10) ~= 2 and math.floor(quest_hechenglu[RoleID]%10) ~= 2 and math.floor(quest_hechenglu[RoleID]/100) ~= 2 then
					msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ���
					msg.AddMsgEvent(MsgID, 1, 405019)	-- ڤɽ��ʯ
				end
				if math.floor(quest_hechenglu[RoleID]%100/10) ~= 3 and math.floor(quest_hechenglu[RoleID]%10) ~= 3 and math.floor(quest_hechenglu[RoleID]/100) ~= 3 then
					msg.AddMsgEvent(MsgID, 21, 6)	-- ѡ����
					msg.AddMsgEvent(MsgID, 1, 405020)	-- ����
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
						role.RemoveFromRole(MapID, InstanceID, RoleID, 4010015, 10, 402)--10��ƽ�����꣨4010015��
						role.RemoveFromRole(MapID, InstanceID, RoleID, 4010034, 5, 402)--5��ڤɽ��ʯ��4010034��
						role.RemoveFromRole(MapID, InstanceID, RoleID, 4080041, 1, 402)--1������
						local Tbl = {}
						Tbl[123] = 4010224
						Tbl[132] = 4010222
						Tbl[213] = 4010223
						Tbl[231] = 4010220
						Tbl[312] = 4010221
						Tbl[321] = 4010219
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
aux.RegisterCreatureEvent(5511024, 7, "hecheglu1_OnTalk")

function hechenglu1_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local i = role.GetRoleItemNum(RoleID, 4010212)
	if i > 0 then
		role.RemoveFromRole(MapID, InstanceID, RoleID,4010212, 1, 402)
	end

end
aux.RegisterQuestEvent(60207,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60208,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60209,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60210,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60211,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60212,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60213,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60214,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60215,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60216,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60217,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60218,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60219,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60220,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60221,11,"hechenglu1_OnComplete")
