 function n6040101_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curhour = tonumber(os.date("%H"))

	local time1 = curmon*10000 + curday *100--����ʱ
	if time1 ~=role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_time"]) then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_baoming"], 0)--����0û���б���,1��ͨ��2��רҵ��
		role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_time"], time1)--
	end
	local x = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_baoming"])

	local money = role.GetRoleSilver(MapID, InstanceID,RoleID)

	local lev = role.GetRoleLevel(MapID, InstanceID,RoleID)

	local juan = role.GetRoleItemNum(RoleID, 4810035)

	if TalkIndex == -1 then
		if curhour == 20 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211123) -- ������Ŀ�����ˣ�\n    ѡ���������������,��ͨ�鱨����Ҫ20��,רҵ�鱨����Ҫ50���һ��רҵ������,�����ֻ�ܲμ�һ�����ÿ��20:00�������ܱ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 4)
			msg.AddMsgEvent(MsgID, 1, 211124) -- �μ���ͨ�����
			msg.AddMsgEvent(MsgID, 21, 5)
			msg.AddMsgEvent(MsgID, 1, 211125) -- �μ�רҵ�����
			msg.AddMsgEvent(MsgID, 21, 8)
			msg.AddMsgEvent(MsgID, 1, 211126) --��������׼����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211123) -- ������Ŀ�����ˣ�\n    ѡ���������������,��ͨ�鱨����Ҫ20��,רҵ�鱨����Ҫ50���һ��רҵ������,�����ֻ�ܲμ�һ�����ÿ��20:00�������ܱ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end

	elseif TalkIndex == 4 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 211127) -- ������Ŀ�����ˣ�\n    ��ȷ��Ҫ������ͨ������𣿲μ���ͨ��֮��Ͳ��ܲμ�רҵ������ˡ�
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 6)
		msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
		msg.AddMsgEvent(MsgID, 21, 99)
		msg.AddMsgEvent(MsgID, 1, 100002) -- ȡ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 211127) -- ������Ŀ�����ˣ�\n    ��ȷ��Ҫ����רҵ������𣿲μ���ͨ��֮��Ͳ��ܲμ���ͨ������ˡ�
		msg.AddMsgEvent(MsgID, 24, TargetID) --
		msg.AddMsgEvent(MsgID, 21, 7)
		msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
		msg.AddMsgEvent(MsgID, 21, 99)
		msg.AddMsgEvent(MsgID, 1, 100002) -- ȡ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 6 then
		if x == 1 then--ҵ����
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211128) -- ������Ŀ�����ˣ�\n    ���Ѿ�������ͨ�����,�����ٱ����ˡ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif x == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211129) -- ������Ŀ�����ˣ�\n    רҵ�����ͨ�������ֻ�ܲμ�һ��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif lev < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211145) -- ������Ŀ�����ˣ�\n    ��ĵȼ�����50��,���ܱ���������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif money < 200000 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211146) -- ������Ŀ�����ˣ�\n    ��������Ҫ���ɱ�����,�����Ϻ���û��ô��Ǯ��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_baoming"], 1)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_got"], 0)
			role.DecRoleSilver(MapID, InstanceID,RoleID,200000, 400)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211130) -- ������Ŀ�����ˣ�\n    ���ܱ�����ͨ��������Ѿ������ɹ�,<A>21:00</A>��ʽ����,��ȥ�������ȴ�<A>�ܲ�������</A>ˢ�°ɰɡ�\n    С�������ϵ�ճҺը��,���������ٶ�,���˷�ը������������ٶȡ�
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 7 then
		if x == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211129) -- ������Ŀ�����ˣ�\n    רҵ�����ͨ�������ֻ�ܲμ�һ��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif x == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211131) -- ������Ŀ�����ˣ�\n    ���Ѿ�����רҵ�����,�����ٱ����ˡ�
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif lev < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211145) -- ������Ŀ�����ˣ�\n    ��ĵȼ�����50��,���ܱ���������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif money < 500000 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211146) -- ������Ŀ�����ˣ�\n    ��������Ҫ���ɱ�����,�����Ϻ���û��ô��Ǯ��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif juan < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211147) -- ������Ŀ�����ˣ�\n    ����רҵ�����Ҫ����רҵ������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else


				role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_baoming"], 2)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_got"], 0)
				role.RemoveFromRole(MapID, InstanceID,RoleID,4810035, 1, 400)	--ɾ��һ��������
				role.DecRoleSilver(MapID, InstanceID,RoleID,500000, 420)

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211132) -- ������Ŀ�����ˣ�\n    ���ܱ���רҵ��������Ѿ������ɹ�,<A>21:00</A>��ʽ����,��ȥ�������ȴ�<A>�ܲ�������</A>ˢ�°ɰɡ�\n    С�������ϵ�ճҺը��,���������ٶ�,���˷�ը������������ٶȡ�
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)


		end
	elseif TalkIndex == 8 then

		role.RoleGotoNewMap(MapID, InstanceID, RoleID, MapID, 555, 2550, 418)--�м�����

	end
end
aux.RegisterCreatureEvent(6040101, 7, "n6040101_OnTalk")
--[[

�ܲ��յ�Ĵ�����a7a  2682

��Ӿ�յ�Ĵ�����a7b  2683

--]]




function paobu1_OnEnterTrigger(MapID, InstanceID, RoleID,objID)--�����������¼�
	local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curhour = tonumber(os.date("%H"))
	local curmin = tonumber(os.date("%M"))

	local curtime = curhour*100 + curmin

	local baoming = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_baoming"])
	local baoming2 = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_baoming"])

	local jingyan = {}		--��ɱ�������ľ���ֵ
	jingyan[50] = 518400
	jingyan[52] = 615730
	jingyan[53] = 721250
	jingyan[54] = 840140
	jingyan[55] = 969210
	jingyan[56] = 1184840
	jingyan[57] = 1406370
	jingyan[58] = 1725650
	jingyan[59] = 2107380
	jingyan[60] = 2539290
	jingyan[61] = 2271290
	jingyan[62] = 2023840
	jingyan[63] = 1798460
	jingyan[64] = 1596680
	jingyan[65] = 1769480
	jingyan[66] = 1824770
	jingyan[67] = 1852420
	jingyan[68] = 1880070
	jingyan[69] = 1907720
	jingyan[70] = 1935360
	jingyan[71] = 1963010
	jingyan[72] = 1990660
	jingyan[73] = 2018310
	jingyan[74] = 2045960
	jingyan[75] = 2073600
	jingyan[76] = 2101250
	jingyan[77] = 2128900
	jingyan[78] = 2156550
	jingyan[79] = 2184200
	jingyan[80] = 3686400
	jingyan[81] = 3732480
	jingyan[82] = 3778560
	jingyan[83] = 3824640
	jingyan[84] = 3870720
	jingyan[85] = 3916800
	jingyan[86] = 3962880
	jingyan[87] = 4008960
	jingyan[88] = 4055040
	jingyan[89] = 4101120
	jingyan[90] = 4147200
	jingyan[91] = 4193280
	jingyan[92] = 4239360
	jingyan[93] = 4285440
	jingyan[94] = 4331520
	jingyan[95] = 4377600
	jingyan[96] = 4423680
	jingyan[97] = 4469760
	jingyan[98] = 4515840
	jingyan[99] = 4561920
	jingyan[100] = 4608000

	if objID == 2142 then


	end

	if objID == 2682 then		--�յ㴥�����жϲ���¼������Ϣ
		local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 59473)
		if bhave == true then
			unit.CancelBuff(MapID, InstanceID, RoleID, 5947301)
		end
		if baoming == 1 or baoming == 2 then	--����Ƿ��ѱ���
			local shiduan = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_shiduan"])	--�����Ҳμ�ʱ����Ϣ
			if shiduan == 1 then		--����
				local qiansan = {}
				for i = 1 ,3 do
					qiansan[i] = map.GetMapScriptData(MapID,InstanceID,1,i)
				end

				local x = 0 --�Ƿ�ǰ�����
				for q = 1 ,3 do
					if qiansan[q] == RoleID then
						x = 1
					end
				end

				if x ~= 1 then
					if qiansan[1] == nil or qiansan[1] == 0 then
						map.SetMapScriptData(MapID,InstanceID,1,1,RoleID)

						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 101, 300111)    --<p1>�ڱ���������Ŀ���¶�ھ�,��ϲ�ɺأ�
						msg.AddMsgEvent(MsgID, 2, RoleID)

						msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)

						local QuestID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(QuestID1, 13, 0)
						msg.AddMsgEvent(QuestID1, 1, 211176)--��ϲ�����˹ھ�����ȥ�佱Ա�����콱�ɡ�
						msg.DispatchRoleMsgEvent(RoleID, QuestID1)

					elseif qiansan[2] == nil or qiansan[2] == 0 then
						map.SetMapScriptData(MapID,InstanceID,1,2,RoleID)
						local QuestID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(QuestID1, 13, 0)
						msg.AddMsgEvent(QuestID1, 1, 211177)--��ϲ�������Ǿ�����ȥ�佱Ա�����콱�ɡ�
						msg.DispatchRoleMsgEvent(RoleID, QuestID1)
					elseif qiansan[3] == nil or qiansan[3] == 0 then
						map.SetMapScriptData(MapID,InstanceID,1,3,RoleID)
						local QuestID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(QuestID1, 13, 0)
						msg.AddMsgEvent(QuestID1, 1, 211178)--��ϲ�����˼�������ȥ�佱Ա�����콱�ɡ�
						msg.DispatchRoleMsgEvent(RoleID, QuestID1)
					elseif qiansan[1] > 0 and qiansan[2] > 0 and qiansan[3] > 0 then
						local QuestID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(QuestID1, 13, 0)
						msg.AddMsgEvent(QuestID1, 1, 211179)--��ϲ������˱�������ȥ�佱Ա�����콱�ɡ�
						msg.DispatchRoleMsgEvent(RoleID, QuestID1)
					end
				end

			end
		end
	end

	if objID == 2247 then


	end

	if objID == 2683 then		--�յ㴥�����жϲ���¼������Ϣ
		local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 59473)
		if bhave == true then
			unit.CancelBuff(MapID, InstanceID, RoleID, 5947301)
		end
		if baoming2 == 1 or baoming2 == 2 then	--����Ƿ��ѱ���
			local shiduan = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_shiduan"])	--�����Ҳμ�ʱ����Ϣ
			if shiduan == 1 then		--����
				local qiansan = {}
				for i = 11 ,13 do
					qiansan[i] = map.GetMapScriptData(MapID,InstanceID,1,i)
				end

				local x = 0 --�Ƿ�ǰ�����
				for q = 11 ,13 do
					if qiansan[q] == RoleID then
						x = 1
					end
				end

				if x ~= 1 then
					if qiansan[11] == nil or qiansan[11] == 0 then
						map.SetMapScriptData(MapID,InstanceID,1,11,RoleID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 101, 300112)    --<p1>�ڱ�����Ӿ��Ŀ���¶�ھ�,��ϲ�ɺأ�
						msg.AddMsgEvent(MsgID, 2, RoleID)

						msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)

						local QuestID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(QuestID1, 13, 0)
						msg.AddMsgEvent(QuestID1, 1, 211176)--��ϲ�����˹ھ�����ȥ�佱Ա�����콱�ɡ�
						msg.DispatchRoleMsgEvent(RoleID, QuestID1)

					elseif qiansan[12] == nil or qiansan[12] == 0 then
						map.SetMapScriptData(MapID,InstanceID,1,12,RoleID)

						local QuestID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(QuestID1, 13, 0)
						msg.AddMsgEvent(QuestID1, 1, 211177)--��ϲ�������Ǿ�����ȥ�佱Ա�����콱�ɡ�
						msg.DispatchRoleMsgEvent(RoleID, QuestID1)
					elseif qiansan[13] == nil or qiansan[13] == 0 then
						map.SetMapScriptData(MapID,InstanceID,1,13,RoleID)

						local QuestID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(QuestID1, 13, 0)
						msg.AddMsgEvent(QuestID1, 1, 211178)--��ϲ�����˼�������ȥ�佱Ա�����콱�ɡ�
						msg.DispatchRoleMsgEvent(RoleID, QuestID1)
					elseif qiansan[11] > 0 and qiansan[12] > 0 and qiansan[13] > 0 then
						local QuestID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(QuestID1, 13, 0)
						msg.AddMsgEvent(QuestID1, 1, 211179)--��ϲ������˱�������ȥ�佱Ա�����콱�ɡ�
						msg.DispatchRoleMsgEvent(RoleID, QuestID1)
					end
				end
			end
			
		end
	end
end
aux.RegisterMapEvent("s65",7, "paobu1_OnEnterTrigger")


function aoyun_OnTimerMin(actID)
	local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curhour = tonumber(os.date("%H"))
	local curmin = tonumber(os.date("%M"))
	local MapID = 3390517288
	local InstanceID = -1
	local curtime = curhour*100 + curmin
	local qingkongpaobu = 0

	if curtime >= 2050 and curtime <= 2053 then	--�ܲ�������ʼǰ10��������ܲ�����
		for i = 1 , 3 do
			map.SetMapScriptData(3390517288,-1,1,i,0)
		end
	end
	if curtime >= 2110 and curtime <= 2113 then	--��Ӿ������ʼǰ10���������Ӿ����
		for i = 11 , 13 do
			map.SetMapScriptData(3390517288,-1,1,i,0)
		end
	end
	if curtime >= 2130 and curtime <= 2140 then	--��Ӿ������ʼǰ10���������Ӿ����
		for i = 30 , 35 do
			map.SetMapScriptData(3390517288,-1,1,i,0)
		end
	end
	if curtime == 2055 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 211189)    --���ܽ���5���Ӻ�ʼ����,����Ȥ������뵽���˵��� 504, 247λ�ñ���
		msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)
	end
	if curtime == 2057 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 211190)    --���ܽ���3���Ӻ�ʼ����,����Ȥ������뵽���˵���504, 247λ�ñ���
		msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)
	end
	if curtime == 2059 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 211191)    --���ܽ���1���Ӻ�ʼ����,����Ȥ������뵽���˵���504, 247λ�ñ���
		msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)
	end


	if curtime == 2115 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 211192)    --��Ӿ����5���Ӻ�ʼ����,����Ȥ������뵽���˵��� 512, 253λ�ñ���
		msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)
	end
	if curtime == 2117 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 211193)    --��Ӿ����3���Ӻ�ʼ����,����Ȥ������뵽���˵���512, 253λ�ñ���
		msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)
	end
	if curtime == 2119 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 211194)    --��Ӿ����1���Ӻ�ʼ����,����Ȥ������뵽���˵���512, 253λ�ñ���
		msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)
	end

	if curtime == 2135 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 211195)    --��������Ŀ����5���Ӻ�ʼ����,����Ȥ������뵽���˵��� 495, 243λ�ñ���
		msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)
	end
	if curtime == 2137 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 211196)    --��������Ŀ����3���Ӻ�ʼ����,����Ȥ������뵽���˵��� 495, 243λ�ñ���
		msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)
	end
	if curtime == 2139 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 211197)    --��������Ŀ����1���Ӻ�ʼ����,����Ȥ������뵽���˵��� 495, 243λ�ñ���
		msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)
	end
	if curtime == 2140 then
		map.MapCreateColCreature(MapID, InstanceID,6040138, 1,1,1, 1)
		map.MapCreateColCreature(MapID, InstanceID,6040139, 1,1,1, 1)
		map.MapCreateColCreature(MapID, InstanceID,6040140, 1,1,1, 1)
	end
	if curtime == 2143 or curtime == 2146 or curtime == 2149 or curtime == 2152 or curtime == 2155 or curtime == 2158 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 211206)    --��������Ŀ��һ����&lt;p1&gt;�����ĵ�ǰ����Ϊ&lt;p2&gt;������Ŭ��
		msg.AddMsgEvent(MsgID, 2, map.GetMapScriptData(MapID,InstanceID,1,30))   
		msg.AddMsgEvent(MsgID, 9, map.GetMapScriptData(MapID,InstanceID,1,33))   
		msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)
	end
	if curtime == 2200 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 211211)    --��������Ŀ�Ѿ�����
		msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)
		local MsgID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID1, 101, 211212)    --��������Ŀ��һ����&lt;p1&gt;�����ĵ�ǰ����Ϊ&lt;p2&gt;������Ŭ��
		msg.AddMsgEvent(MsgID1, 2, map.GetMapScriptData(MapID,InstanceID,1,30))   
		msg.AddMsgEvent(MsgID1, 9, map.GetMapScriptData(MapID,InstanceID,1,33))   
		msg.DispatchMapMsgEvent(MsgID1,3390517288,-1,-1)
	end
	
	if curtime == 1200 then				--���ÿ�ս��ƻ���������Ϣ
		for i = 11, 13 do
			act.SetActScriptData(56, 1, i, 0)
			act.SetActScriptData(56, 1, i+4, 0)
		end
		act.SaveActScriptData(56)
	end

	if curtime == 2120 then
		local tab = {}		--��������
		local po = {}		--��������
		local a = {}		--����˳������
		local b = {}		--������������
		po[1] = {741,2400,514}
		po[2] = {711,2400,562}
		po[3] = {749,2400,618}
		po[4] = {764,2400,681}
		po[5] = {835,2400,695}
		po[6] = {840,2400,775}

		a[1] = {736,525}
		a[2] = {712,574}
		a[3] = {746,632}
		a[4] = {780,676}
		a[5] = {838,709}
		a[6] = {826,785}
		b[1] = {743,501}
		b[2] = {716,550}
		b[3] = {746,605}
		b[4] = {750,672}
		b[5] = {826,683}
		b[6] = {848,762}

		tab[2] = 6040133 
		tab[3] = 6040134
		tab[4] = 6040135
		tab[5] = 6040133
		tab[6] = 6040134
		tab[7] = 6040135
		tab[8] = 6040133
		tab[9] = 6040134


		local r = math.random(3)
		for i =1 ,6 do
			local ID
			local x = r+i
			ID = map.MapCreateCreature(MapID, InstanceID, tab[x], po[i][1], po[i][2], po[i][3] )
			if x == 3 or x == 6 or x == 9 then
				unit.SetFaceTo(MapID, InstanceID, ID, b[i][1], b[i][2])
			else
				unit.SetFaceTo(MapID, InstanceID, ID, a[i][1], a[i][2])
			end
		end
	end

	if curtime == 2030 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 211198)    --20:50���ڰ��˵���ˢ�°��˱���,���������м��ʻ�ð��˽��¼��������˻���ߡ�
		msg.DispatchMapMsgEvent(MsgID)
	end

	if curtime == 2045 or curtime == 2110 or curtime == 2130 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 211199)    --���˱��佫��5���Ӻ��ڰ��˵���ˢ��,���׼������ɣ�
		msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)
	end


	if curtime == 2050 or curtime == 2115 or curtime == 2135 then		--���˱���ˢ��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 211200)    --���˱���ˢ����,��ҿ�ʼ����ɣ�
		msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)

		local a1 = 0
		local a2 = 0
		local a3 = 0
		local aoyunbaoxiang = {}
		aoyunbaoxiang[1]={618,2426,369}
		aoyunbaoxiang[2]={610,2449,351}
		aoyunbaoxiang[3]={589,2574,320}
		aoyunbaoxiang[4]={588,2533,302}
		aoyunbaoxiang[5]={578,2550,295}
		aoyunbaoxiang[6]={555,2575,292}
		aoyunbaoxiang[7]={540,2584,279}
		aoyunbaoxiang[8]={519,2649,268}
		aoyunbaoxiang[9]={493,2746,270}
		aoyunbaoxiang[10]={470,2779,281}
		aoyunbaoxiang[11]={470,2712,295}
		aoyunbaoxiang[12]={462,2650,320}
		aoyunbaoxiang[13]={467,2636,344}
		aoyunbaoxiang[14]={489,2582,355}
		aoyunbaoxiang[15]={515,2567,360}
		aoyunbaoxiang[16]={531,2564,345}
		aoyunbaoxiang[17]={551,2582,352}
		aoyunbaoxiang[18]={571,2589,366}
		aoyunbaoxiang[19]={586,2554,378}
		aoyunbaoxiang[20]={561,2573,343}

		local r1 = math.random(20)
		local r2 = r1 - 1
		while r1 <= 20 do
			if a1 < 2 then
				map.MapCreateCreature(MapID, InstanceID, 6040116, aoyunbaoxiang[r1][1], aoyunbaoxiang[r1][2], aoyunbaoxiang[r1][3] )
				a1 = a1 + 1
			
			elseif a2 < 6 then
				map.MapCreateCreature(MapID, InstanceID, 6040117, aoyunbaoxiang[r1][1], aoyunbaoxiang[r1][2], aoyunbaoxiang[r1][3] )
				a2 = a2 + 1
			
			elseif a3 < 12 then	
				map.MapCreateCreature(MapID, InstanceID, 6040118, aoyunbaoxiang[r1][1], aoyunbaoxiang[r1][2], aoyunbaoxiang[r1][3] )
				a3 = a3 + 1
			end
			r1 = r1 + 1
		end
		while r2 > 0 do
			if a1 < 2 then
				map.MapCreateCreature(MapID, InstanceID, 6040116, aoyunbaoxiang[r2][1], aoyunbaoxiang[r2][2], aoyunbaoxiang[r2][3] )
				a1 = a1 + 1
			
			elseif a2 < 6 then
				map.MapCreateCreature(MapID, InstanceID, 6040117, aoyunbaoxiang[r2][1], aoyunbaoxiang[r2][2], aoyunbaoxiang[r2][3] )
				a2 = a2 + 1
			
			elseif a3 < 12 then	
				map.MapCreateCreature(MapID, InstanceID, 6040118, aoyunbaoxiang[r2][1], aoyunbaoxiang[r2][2], aoyunbaoxiang[r2][3] )
				a3 = a3 + 1
			end
			r2 = r2 - 1
		end
	end

	local ID1 , ID2 = 0 , 0
	if curtime == 2100 then
		ID1 = map.MapCreateCreature(MapID, InstanceID, 6040142, 555, 2550, 418)
		unit.SetFaceTo(MapID, InstanceID, 6040142, 557, 2550, 420)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 211201)    --�ܲ�������ʼ�����ܲ����������￪�������Ϳ������յ����ˣ�
		msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)
	end
	if curtime == 2130 then				--�㲥֪ͨǰ����
		local q = {}
		for i =1 , 3 do
			q[i] = map.GetMapScriptData(MapId, InstanceID,1, i)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 211209)    --����������Ŀ�ѽ����������������ӻԾ�μӣ�
		msg.AddMsgEvent(MsgID, 2, q[1])
		msg.AddMsgEvent(MsgID, 2, q[2])
		msg.AddMsgEvent(MsgID, 2, q[3])

		msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)
	end
	
	if curtime == 2120 then
		ID2 = map.MapCreateColCreature(MapID, InstanceID, 6040143, 718,2594,393, 1, "")
		unit.SetFaceTo(MapID, InstanceID, 6040143, 708, 2467, 401)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 211202)    --��Ӿ������ʼ������Ӿ���������￪�������Ϳ������յ����ˣ�
		msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)
	end
	if curtime == 2150 then
		local q = {}
		for i =11 , 13 do
			q[i] = map.GetMapScriptData(MapId, InstanceID,1, i)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 211210)    --������Ӿ��Ŀ��Ŀ�ѽ����������������ӻԾ�μӣ�
		msg.AddMsgEvent(MsgID, 2, q[11])
		msg.AddMsgEvent(MsgID, 2, q[12])
		msg.AddMsgEvent(MsgID, 2, q[13])

		msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)
	end
	
	if curtime == 2107 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 211203)    --?	���˵���������һ�������ع����ɱ���ع���ɻ�÷ḻ�Ľ�����
		msg.DispatchMapMsgEvent(MsgID,3390517288,-1,-1)
	end


	if curtime == 2130 or curtime == 2135 or curtime == 2131 or curtime == 2136 or curtime == 2132 or curtime == 2137 or curtime == 2133 or curtime == 2138 or curtime == 2134 or curtime == 2139 or curtime == 2135 or curtime == 2140 then		
			--����λ��ˢ��
		local tab = {}
		tab[1]={618,2426,369}
		tab[2]={610,2449,351}
		tab[3]={589,2574,320}
		tab[4]={588,2533,302}
		tab[5]={578,2550,295}
		tab[6]={555,2575,292}
		tab[7]={540,2584,279}
		tab[8]={519,2649,268}
		tab[9]={493,2746,270}
		tab[10]={470,2779,281}
		tab[11]={470,2712,295}
		tab[12]={462,2650,320}
		tab[13]={467,2636,344}
		tab[14]={489,2582,355}
		tab[15]={515,2567,360}
		tab[16]={531,2564,345}
		tab[17]={551,2582,352}
		tab[18]={571,2589,366}
		tab[19]={586,2554,378}
		tab[20]={561,2573,343}
		tab[21]={509,2586,291}
		tab[22]={495,2591,303}
		tab[23]={459,5660,330}
		tab[24]={494,2573,362}
		tab[25]={606,2481,373}
		tab[26]={605,2449,292}
		tab[27]={586,2512,245}
		tab[28]={543,2647,245}
		tab[29]={532,2638,261}
		tab[30]={511,2574,295}

		for i = 1,30 do
			local R = math.random(10)
			if R >= 1 and R <= 4 then
				map.MapCreateCreature(MapID, InstanceID, 6040123, tab[i][1], tab[i][2], tab[i][3] )
			elseif R >= 5 and R <= 8 then
				map.MapCreateCreature(MapID, InstanceID, 6040124, tab[i][1], tab[i][2], tab[i][3] )
			elseif R == 9 then
				map.MapCreateCreature(MapID, InstanceID, 6040125, tab[i][1], tab[i][2], tab[i][3] )
			elseif R == 10 then
				map.MapCreateCreature(MapID, InstanceID, 6040126, tab[i][1], tab[i][2], tab[i][3] )
			end
		end
	end

end
aux.RegisterActEvent(56, 4, "aoyun_OnTimerMin")



 function n6040115_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)		--�����佱��
	

	local jingyan = {}		--��ɱ�������ľ���ֵ
	jingyan[50] = 518400
	jingyan[52] = 615730
	jingyan[53] = 721250
	jingyan[54] = 840140
	jingyan[55] = 969210
	jingyan[56] = 1184840
	jingyan[57] = 1406370
	jingyan[58] = 1725650
	jingyan[59] = 2107380
	jingyan[60] = 2539290
	jingyan[61] = 2271290
	jingyan[62] = 2023840
	jingyan[63] = 1798460
	jingyan[64] = 1596680
	jingyan[65] = 1769480
	jingyan[66] = 1824770
	jingyan[67] = 1852420
	jingyan[68] = 1880070
	jingyan[69] = 1907720
	jingyan[70] = 1935360
	jingyan[71] = 1963010
	jingyan[72] = 1990660
	jingyan[73] = 2018310
	jingyan[74] = 2045960
	jingyan[75] = 2073600
	jingyan[76] = 2101250
	jingyan[77] = 2128900
	jingyan[78] = 2156550
	jingyan[79] = 2184200
	jingyan[80] = 3686400
	jingyan[81] = 3732480
	jingyan[82] = 3778560
	jingyan[83] = 3824640
	jingyan[84] = 3870720
	jingyan[85] = 3916800
	jingyan[86] = 3962880
	jingyan[87] = 4008960
	jingyan[88] = 4055040
	jingyan[89] = 4101120
	jingyan[90] = 4147200
	jingyan[91] = 4193280
	jingyan[92] = 4239360
	jingyan[93] = 4285440
	jingyan[94] = 4331520
	jingyan[95] = 4377600
	jingyan[96] = 4423680
	jingyan[97] = 4469760
	jingyan[98] = 4515840
	jingyan[99] = 4561920
	jingyan[100] = 4608000

	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 211134) -- �����佱�ˣ�\n    ��һ�����ܱ�����ǰ�����ֱ���\n    �ھ�    <p1>\n    �Ǿ�    <p2>\n    ����    <p3>\n    ��������ѡ�ֽ���ò��뽱��\n    ��һ����Ӿ������ǰ�����ֱ���\n    �ھ�    <p4>\n    �Ǿ�    <p5>\n    ����    <p6>\n    ��������ѡ�ֽ���ò��뽱��\n    ��һ��������������ǰ�����ֱ���\n    �ھ�    <p4>\n    �Ǿ�    <p5>\n    ����    <p6>\n    ��������ѡ�ֽ���ò��뽱��
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 2, map.GetMapScriptData(MapId, InstanceID,1, 1))
		msg.AddMsgEvent(MsgID, 2, map.GetMapScriptData(MapId, InstanceID,1, 2))
		msg.AddMsgEvent(MsgID, 2, map.GetMapScriptData(MapId, InstanceID,1, 3))

		msg.AddMsgEvent(MsgID, 2, map.GetMapScriptData(MapId, InstanceID,1, 11))
		msg.AddMsgEvent(MsgID, 2, map.GetMapScriptData(MapId, InstanceID,1, 12))
		msg.AddMsgEvent(MsgID, 2, map.GetMapScriptData(MapId, InstanceID,1, 13))

		msg.AddMsgEvent(MsgID, 2, map.GetMapScriptData(MapId, InstanceID,1, 30))
		msg.AddMsgEvent(MsgID, 2, map.GetMapScriptData(MapId, InstanceID,1, 31))
		msg.AddMsgEvent(MsgID, 2, map.GetMapScriptData(MapId, InstanceID,1, 32))

		msg.AddMsgEvent(MsgID, 21, 4)
		msg.AddMsgEvent(MsgID, 1, 211142) -- ��ȡ���ܽ���
		msg.AddMsgEvent(MsgID, 21, 5)
		msg.AddMsgEvent(MsgID, 1, 211143) -- ��ȡ��Ӿ����
		msg.AddMsgEvent(MsgID, 21, 6)
		msg.AddMsgEvent(MsgID, 1, 211185) -- ��ȡ����������
		msg.AddMsgEvent(MsgID, 21, 7)
		msg.AddMsgEvent(MsgID, 1, 211186) -- ����˵��
		msg.AddMsgEvent(MsgID, 21, 99)
		msg.AddMsgEvent(MsgID, 1, 100002) -- ȡ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local baoming = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_baoming"])	--�����ұ�����Ϣ
		local got = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_got"])		--����Ƿ�����ȡ������
		local shiduan = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_shiduan"])   --�����Ҳ���ʱ����Ϣ

		if baoming ==  0 or baoming == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211135) -- �����佱�ˣ�\n    ��û�б����μ���һ������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif  got == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211136) -- �����佱�ˣ�\n    ���Ѿ���ȡ��������
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif  shiduan == 0 or shiduan == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211141) -- �����佱�ˣ�\n    �㲢û�вμ���һ������
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif  role.GetBagFreeSize(RoleID) < 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211213) -- "�����佱�ˣ�\n    �����ռ䲻�㡣"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
			local qiansan = {}
			for i = 1 ,3 do
				qiansan[i] = map.GetMapScriptData(MapID,InstanceID,1,i)
			end

			if qiansan[1] == RoleID then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211137) -- �����佱�ˣ�\n    ��ϲ�����˹ھ���������Ľ�����
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, -1)
				msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				local level = role.GetRoleLevel(MapID,InstanceID,RoleID)	--��ɱ���������Ҿ��齱��
				role.AddRoleExp(MapID, InstanceID, RoleID, jingyan[level])

				--���轱��,�ж�רҵ���������
				if baoming == 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 100, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 3, -1, 8, 420)	--����

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_shiduan"], 0)
				elseif baoming == 2 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 600, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 18, -1, 8, 420)	--����
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 20, -1, 8, 420)

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_shiduan"], 0)
				end
			elseif qiansan[2] == RoleID then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211138) -- �����佱�ˣ�\n    ��ϲ�������Ǿ���������Ľ�����
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, -1)
				msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				local level = role.GetRoleLevel(MapID,InstanceID,RoleID)	--��ɱ���������Ҿ��齱��
				role.AddRoleExp(MapID, InstanceID, RoleID, jingyan[level])

				--���轱��,�ж�רҵ���������
				if baoming == 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 70, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 2, -1, 8, 420)	--����

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_shiduan"], 0)
				elseif baoming == 2 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 420, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 12, -1, 8, 420)	--����
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 20, -1, 8, 420)

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_shiduan"], 0)
				end

			elseif qiansan[3] == RoleID then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211139) -- �����佱�ˣ�\n    ��ϲ�����˼�����������Ľ�����
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, -1)
				msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				local level = role.GetRoleLevel(MapID,InstanceID,RoleID)	--��ɱ���������Ҿ��齱��
				role.AddRoleExp(MapID, InstanceID, RoleID, jingyan[level])

				--���轱��,�ж�רҵ���������
				if baoming == 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 50, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 1, -1, 8, 420)	--����

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_shiduan"], 0)
				elseif baoming == 2 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 300, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 6, -1, 8, 420)	--����
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 20, -1, 8, 420)

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_shiduan"], 0)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211140) -- �����佱�ˣ�\n    ��ֻ����˲��뽱,�ٽ�������
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, -1)
				msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				local level = role.GetRoleLevel(MapID,InstanceID,RoleID)	--��ɱ���������Ҿ��齱��
				role.AddRoleExp(MapID, InstanceID, RoleID, jingyan[level])

				--���轱��,�ж�רҵ���������
				if baoming == 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 20, -1, 8, 420)	--���˽���

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_shiduan"], 0)
				elseif baoming == 2 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 120, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 20, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_shiduan"], 0)
				end
			end
		end

	elseif TalkIndex == 5 then
		local baoming = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_baoming"])	--�����ұ�����Ϣ
		local got = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_got"])		--����Ƿ�����ȡ������
		local shiduan = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_shiduan"])   --�����Ҳ���ʱ����Ϣ

		if baoming ==  0 or baoming == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211135) -- �����佱�ˣ�\n    ��û�б����μ���һ������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif  got == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211136) -- �����佱�ˣ�\n    ���Ѿ���ȡ��������
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif  shiduan == 0 or shiduan == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211141) -- �����佱�ˣ�\n    �㲢û�вμ���һ������
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif  role.GetBagFreeSize(RoleID) < 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211213) -- "�����佱�ˣ�\n    �����ռ䲻�㡣"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ

		else
			local qiansan = {}
			for i = 11 ,13 do
				qiansan[i] = map.GetMapScriptData(MapID,InstanceID,1,i)
			end

			if qiansan[11] == RoleID then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211137) -- �����佱�ˣ�\n    ��ϲ�����˹ھ���������Ľ�����
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, -1)
				msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				local level = role.GetRoleLevel(MapID,InstanceID,RoleID)	--��ɱ���������Ҿ��齱��
				role.AddRoleExp(MapID, InstanceID, RoleID, jingyan[level])

				--���轱��,�ж�רҵ���������
				if baoming == 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 100, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 3, -1, 8, 420)	--����

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_shiduan"], 0)
				elseif baoming == 2 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 600, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 18, -1, 8, 420)	--����
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 20, -1, 8, 420)

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_shiduan"], 0)
				end
			elseif qiansan[12] == RoleID then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211138) -- �����佱�ˣ�\n    ��ϲ�������Ǿ���������Ľ�����
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, -1)
				msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				local level = role.GetRoleLevel(MapID,InstanceID,RoleID)	--��ɱ���������Ҿ��齱��
				role.AddRoleExp(MapID, InstanceID, RoleID, jingyan[level])

				--���轱��,�ж�רҵ���������
				if baoming == 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 70, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 2, -1, 8, 420)	--����

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_shiduan"], 0)
				elseif baoming == 2 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 420, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 12, -1, 8, 420)	--����
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 20, -1, 8, 420)

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_shiduan"], 0)
				end

			elseif qiansan[13] == RoleID then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211139) -- �����佱�ˣ�\n    ��ϲ�����˼�����������Ľ�����
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, -1)
				msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				local level = role.GetRoleLevel(MapID,InstanceID,RoleID)	--��ɱ���������Ҿ��齱��
				role.AddRoleExp(MapID, InstanceID, RoleID, jingyan[level])

				--���轱��,�ж�רҵ���������
				if baoming == 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 50, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 1, -1, 8, 420)	--����

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_shiduan"], 0)
				elseif baoming == 2 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 300, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 6, -1, 8, 420)	--����
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 20, -1, 8, 420)

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_shiduan"], 0)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211140) -- �����佱�ˣ�\n    ��ֻ����˲��뽱,�ٽ�������
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, -1)
				msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				local level = role.GetRoleLevel(MapID,InstanceID,RoleID)	--��ɱ���������Ҿ��齱��
				role.AddRoleExp(MapID, InstanceID, RoleID, jingyan[level])

				--���轱��,�ж�רҵ���������
				if baoming == 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 20, -1, 8, 420)	--���˽���

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_shiduan"], 0)
				elseif baoming == 2 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 120, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 20, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_shiduan"], 0)
				end
			end
		end

	elseif TalkIndex == 6 then
		local baoming = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_baoming"])	--�����ұ�����Ϣ
		local got = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_got"])		--����Ƿ�����ȡ������
		local shiduan = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_shiduan"])   --�����Ҳ���ʱ����Ϣ
		local jifen = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_jifen"])
		
		local t = {}
		t[50]=777600
		t[51]=923595
		t[52]=1081875
		t[53]=1260210
		t[54]=1453815
		t[55]=1777260
		t[56]=2109555
		t[57]=2588475
		t[58]=3161070
		t[59]=3808935
		t[60]=3406935
		t[61]=3035760
		t[62]=2697690
		t[63]=2395020
		t[64]=2654220
		t[65]=2695680
		t[66]=2737155
		t[67]=2778630
		t[68]=2820105
		t[69]=2861580
		t[70]=2903040
		t[71]=2944515
		t[72]=2985990
		t[73]=3027465
		t[74]=3068940
		t[75]=3110400
		t[76]=3151875
		t[77]=3193350
		t[78]=3234825
		t[79]=3276300
		t[80]=5529600
		t[81]=5598720
		t[82]=5667840
		t[83]=5736960
		t[84]=5806080
		t[85]=5875200
		t[86]=5944320
		t[87]=6013440
		t[88]=6082560
		t[89]=6151680
		t[90]=6220800
		t[91]=6289920
		t[92]=6359040
		t[93]=6428160
		t[94]=6497280
		t[95]=6566400
		t[96]=6635520
		t[97]=6704640
		t[98]=6773760
		t[99]=6842880
		t[100]=6912000

		if baoming ==  0 or baoming == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211135) -- �����佱�ˣ�\n    ��û�б����μ���һ������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif  got == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211136) -- �����佱�ˣ�\n    ���Ѿ���ȡ��������
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif  jifen == 0 or jifen == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211141) -- �����佱�ˣ�\n    �㲢û�вμ���һ������
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif  role.GetBagFreeSize(RoleID) < 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211213) -- "�����佱�ˣ�\n    �����ռ䲻�㡣"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
			local qiansan = {}
			for i = 30 ,32 do
				qiansan[i] = map.GetMapScriptData(MapID,InstanceID,1,i)
			end

			if qiansan[30] == RoleID then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211137) -- �����佱�ˣ�\n    ��ϲ�����˹ھ���������Ľ�����
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, -1)
				msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				local level = role.GetRoleLevel(MapID,-1,RoleID)	--��ɱ���������Ҿ��齱��
				role.AddRoleExp(MapID, -1, RoleID, t[level])

				--���轱��,�ж�רҵ���������
				if baoming == 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 300, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 10, -1, 8, 420)	--����

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_shiduan"], 0)
				elseif baoming == 2 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 1800, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 60, -1, 8, 420)	--����
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 20, -1, 8, 420)

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_shiduan"], 0)
				end
			elseif qiansan[31] == RoleID then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211138) -- �����佱�ˣ�\n    ��ϲ�������Ǿ���������Ľ�����
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, -1)
				msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)


				local level = role.GetRoleLevel(MapID,-1,RoleID)	--��ɱ���������Ҿ��齱��
				role.AddRoleExp(MapID, -1, RoleID, t[level])
				--���轱��,�ж�רҵ���������
				if baoming == 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 200, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 7, -1, 8, 420)	--����

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_shiduan"], 0)
				elseif baoming == 2 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 1200, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 42, -1, 8, 420)	--����
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 20, -1, 8, 420)

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_shiduan"], 0)
				end

			elseif qiansan[32] == RoleID then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211139) -- �����佱�ˣ�\n    ��ϲ�����˼�����������Ľ�����
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, -1)
				msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)


				local level = role.GetRoleLevel(MapID,-1,RoleID)	--��ɱ���������Ҿ��齱��
				role.AddRoleExp(MapID, -1, RoleID, t[level])

				--���轱��,�ж�רҵ���������
				if baoming == 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 100, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 4, -1, 8, 420)	--����

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_shiduan"], 0)
				elseif baoming == 2 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 600, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 24, -1, 8, 420)	--����
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 20, -1, 8, 420)

					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_shiduan"], 0)
				end
			elseif jifen > 0 then
				local level = role.GetRoleLevel(MapID,-1,RoleID)	--��ɱ���������Ҿ��齱��
				role.AddRoleExp(MapID, -1, RoleID, t[level])

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211140) -- �����佱�ˣ�\n    ��ֻ����˲��뽱,�ٽ�������
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, -1)
				msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				--���轱��,�ж�רҵ���������
				if baoming == 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 60, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 2, -1, 8, 420)	--����
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_shiduan"], 0)
				elseif baoming == 2 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 360, -1, 8, 420)	--���˽���
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 12, -1, 8, 420)	--����
					role.AddRoleItem(MapID, InstanceID, RoleID, 4810037, 20, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_got"], 1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_shiduan"], 0)
				end
			end
		end
	elseif TalkIndex == 7 then	--����˵��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 211187) -- �����佱�ˣ�\n    �ھ������10�����˽��º�3�����˽���\n    �Ǿ������7�����˽��º�2�����˽���\n    ���������5�����˽��º�1�����˽���\n    ���뽱��Ϊ2�����˽���\n
		msg.AddMsgEvent(MsgID, 24, TargetID) --
		msg.AddMsgEvent(MsgID, 21, -1)
		msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	end
end
aux.RegisterCreatureEvent(6040115, 7, "n6040115_OnTalk")
