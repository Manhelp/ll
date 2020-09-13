
--��6040104  ���ư� �Ի�
function n6040104_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local curyear = tonumber(os.date("%y"))
	local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curhour = tonumber(os.date("%H"))
	local curmin = tonumber(os.date("%M"))

	local curtime = curyear*10000+curmon*100+curday
	local jiangpaijifen_time = role.GetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_time"])
	local nowtime = curhour*100 + curmin
	if (curtime - jiangpaijifen_time == 1 and nowtime >= 1200) or (curtime - jiangpaijifen_time > 1)then		--��ǰ������������ݶԱ�,�������12��ÿ�ջ�������
		role.SetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_today"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_got"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_time"], curtime)
	end

	local jingyan = {}
	jingyan[50] = 7776
	jingyan[51] = 9235
	jingyan[52] = 10818
	jingyan[53] = 12602
	jingyan[54] = 14538
	jingyan[55] = 17772
	jingyan[56] = 21095
	jingyan[57] = 25884
	jingyan[58] = 31610
	jingyan[59] = 38089
	jingyan[60] = 34069
	jingyan[61] = 30357
	jingyan[62] = 26976
	jingyan[63] = 23950
	jingyan[64] = 26542
	jingyan[65] = 26956
	jingyan[66] = 27371
	jingyan[67] = 27786
	jingyan[68] = 28201
	jingyan[69] = 28615
	jingyan[70] = 29030
	jingyan[71] = 29445
	jingyan[72] = 29859
	jingyan[73] = 30274
	jingyan[74] = 30689
	jingyan[75] = 31104
	jingyan[76] = 31518
	jingyan[77] = 31933
	jingyan[78] = 32348
	jingyan[79] = 32763
	jingyan[80] = 55296
	jingyan[81] = 55987
	jingyan[82] = 56678
	jingyan[83] = 57369
	jingyan[84] = 58060
	jingyan[85] = 58752
	jingyan[86] = 59443
	jingyan[87] = 60134
	jingyan[88] = 60825
	jingyan[89] = 61516
	jingyan[90] = 62208
	jingyan[91] = 62899
	jingyan[92] = 63590
	jingyan[93] = 64281
	jingyan[94] = 64972
	jingyan[95] = 65664
	jingyan[96] = 66355
	jingyan[97] = 67046
	jingyan[98] = 67737
	jingyan[99] = 68428
	jingyan[100] = 69120

	if TalkIndex == -1 then
		local nowtime = curmon*100 + curday
		if nowtime >= 724 and nowtime < 821  then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211100) -- "���ư�\n    ������������ύ���ƶһ����ºͻ���,�鿴���ư����ȡ�������\n    �㵱ǰ��ÿ�ջ����� <p1>\n    ������������<p2>"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, role.GetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_today"]))
			msg.AddMsgEvent(MsgID, 9, role.GetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_all"]))

			msg.AddMsgEvent(MsgID, 21, 4) -- ѡ�ť
			msg.AddMsgEvent(MsgID, 1, 211101) -- ���ύ���ơ�

			msg.AddMsgEvent(MsgID, 21, 5)   --ѡ�ť
			msg.AddMsgEvent(MsgID, 1, 211102)   --"�鿴���ư�"

			msg.AddMsgEvent(MsgID, 21, 6)   --ѡ�ť
			msg.AddMsgEvent(MsgID, 1, 211103)   --"��ȡ����"

			msg.AddMsgEvent(MsgID, 21, 7)   --ѡ�ť
			msg.AddMsgEvent(MsgID, 1, 211104)   --"�˵��"

			msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100002)   --"ȡ��"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211100) -- "���ư�\n    ������������ύ���ƶһ����ºͻ���,�鿴���ư����ȡ�������\n    �㵱ǰ��ÿ�ջ����� <p1>\n    ������������<p2>"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, role.GetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_today"]))
			msg.AddMsgEvent(MsgID, 9, role.GetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_all"]))

			msg.AddMsgEvent(MsgID, 21, 5)   --ѡ�ť
			msg.AddMsgEvent(MsgID, 1, 211102)   --"�鿴���ư�"

			msg.AddMsgEvent(MsgID, 21, 6)   --ѡ�ť
			msg.AddMsgEvent(MsgID, 1, 211103)   --"��ȡ����"

			msg.AddMsgEvent(MsgID, 21, 7)   --ѡ�ť
			msg.AddMsgEvent(MsgID, 1, 211104)   --"�˵��"

			msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100002)   --"ȡ��"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end

	elseif TalkIndex == 4 then			--�ύ����
		local lev = role.GetRoleLevel(MapID, InstanceID,RoleID)

		if lev < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211204) -- �ȼ�����50
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif nowtime >= 2230 or nowtime <= 1200 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211207) -- ���ư�\n    ���ڲ����ύ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if role.GetBagFreeSize(RoleID) < 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211105) -- "���ư�\n    ����Ҫ�пռ���ܶһ�������"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211122) -- "���ư�\n    �ύ���ƿ��Ի�ȡ���֣�\n        �ύ1ö���ƿɻ�ȡ30�����˽���,10�㽱�ƻ���\n    �ύ5ö���ƿɻ�ȡ165�����˽���,55�㽱�ƻ���\n    �ύ10ö���ƿɻ�ȡ370�����˽���,120�㽱�ƻ���"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid

				msg.AddMsgEvent(MsgID, 21, 8)   --ѡ�ť
				msg.AddMsgEvent(MsgID, 1, 211106)   --"�ύ����*1"

				msg.AddMsgEvent(MsgID, 21, 9)   --ѡ�ť
				msg.AddMsgEvent(MsgID, 1, 211107)   --"�ύ����*5"

				msg.AddMsgEvent(MsgID, 21, 10)   --ѡ�ť
				msg.AddMsgEvent(MsgID, 1, 211108)   --"�ύ����*10"

				msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
				msg.AddMsgEvent(MsgID, 1, 100002)   --"ȡ��"

				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end
		end
	elseif TalkIndex == 8 then				--�ύ1ö
		local a = role.GetRoleItemNum(RoleID, 4810037)
		if a < 1 then                              --�жϽ�������
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211109) -- "���ư�\n    ��û���㹻�Ľ��ƿ����ύ��"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
			local level = role.GetRoleLevel(MapID,-1,RoleID)	--�ύ���Ƹ�����Ҿ��齱��
			role.AddRoleExp(MapID, -1, RoleID, jingyan[level])
			role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 30, -1, 8, 420) --�������30�����˽���
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4810037, 1, 420)  --ɾ��һ������

			HuoDeJiangpaijifen(RoleID,10)				--�������10����

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211110) -- "���ư�\n    ��Ҫ��ý����ʹ�����Ľ������ɣ�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end

	elseif TalkIndex == 9 then					--�ύ5ö
		local a = role.GetRoleItemNum(RoleID, 4810037)
		if a < 5 then                                 --�жϽ��������Ƿ�5��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211109) -- "���ư�\n    ��û���㹻�Ľ��ƿ����ύ��"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
			local level = role.GetRoleLevel(MapID,-1,RoleID)	--�ύ���Ƹ�����Ҿ��齱��
			role.AddRoleExp(MapID, -1, RoleID, jingyan[level]*5)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 165, -1, 8, 420)     --�������165�����˽���
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4810037, 5, 420)		--ɾ��5������

			HuoDeJiangpaijifen(RoleID,55)				--�������55����

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211110) -- "���ư�\n    ��Ҫ��ý����ʹ�����Ľ������ɣ�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end


	elseif TalkIndex == 10 then				--�ύ10ö
		local a = role.GetRoleItemNum(RoleID, 4810037)
		if a < 10 then                                 --�жϽ��������Ƿ�10��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211109) -- "���ư�\n    ��û���㹻�Ľ��ƿ����ύ��"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
			local level = role.GetRoleLevel(MapID,-1,RoleID)	--�ύ���Ƹ�����Ҿ��齱��
			role.AddRoleExp(MapID, -1, RoleID, jingyan[level]*10)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 370, -1, 8, 420)     --�������370�����˽���
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4810037, 10, 420)		--ɾ��10������

			HuoDeJiangpaijifen(RoleID,120)					--�������120����

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211110) -- "���ư�\n    ��Ҫ��ý����ʹ�����Ľ������ɣ�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end



	elseif TalkIndex == 5 then                              --�鿴���ư�
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 211121) -- "���ư�\n    �鿴��������"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid

		msg.AddMsgEvent(MsgID, 21, 13) -- ѡ�ť
		msg.AddMsgEvent(MsgID, 1, 211111) -- ���鿴ÿ�ս��ƻ������С�

		msg.AddMsgEvent(MsgID, 21, 14)   --ѡ�ť
		msg.AddMsgEvent(MsgID, 1, 211112)   --"�鿴���影�ƻ�������"

		msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100002)   --"ȡ��"
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ

	elseif TalkIndex == 13 then		-- ÿ�ս��ƻ�������
		--[[
		local k
		for i = 11 ,13 do						--�ж�ÿ�ս��ư�ǰ�����Ƿ񶼴���
			if act.GetActScriptData(56, 1, i) > 0 then
				k = act.GetActScriptData(56, 1, i)
			else
				k = nil
				break
			end
		end
		if k then
		--]]
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211113) -- "���ư�\n    ��һ��<p1>  <p2>���ƻ���\n    �ڶ���<p3>  <p4>���ƻ���\n    ������<p5>  <p6>���ƻ���"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(56, 1, 11))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(56, 1, 15))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(56, 1, 12))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(56, 1, 16))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(56, 1, 13))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(56, 1, 17))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		--[[
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211114) -- "���ư�\n    ���ư�ûͳ����ȫ"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
		--]]


	elseif TalkIndex == 14 then		-- ���影�ƻ�������
		--[[
		local k2
		for i = 1 ,3 do						--�ж����影�ư�ǰ�����Ƿ񶼴���
			if act.GetActScriptData(56, 1, i) > 0 then
				k2 = act.GetActScriptData(56, 1, i)
			else
				k2 = nil
				break
			end
		end
		if k2 then
		--]]
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211113) -- "���ư�\n    ��һ��<p1>  <p2>���ƻ���\n    �ڶ���<p3>  <p4>���ƻ���\n    ������<p5>  <p6>���ƻ���"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(56, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(56, 1, 5))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(56, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(56, 1, 6))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(56, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(56, 1, 7))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		--[[
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211114) -- "���ư�\n    ���ư�ûͳ����ȫ"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
		--]]

	elseif TalkIndex == 6 then           --��ȡ����
		local curyear = tonumber(os.date("%y"))
		local curmon = tonumber(os.date("%m"))
		local curday = tonumber(os.date("%d"))


		local curtime = curmon*100+curday
		if curtime < 822 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211115) -- "���ư�\n    ÿ�ս���ÿ��ֻ����ȡһ��,����㵱ǰ��ÿ�ս��ƻ�������ǰ������н���,ÿ��0�㽫���ÿ�ս��ƻ��֡�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid

			msg.AddMsgEvent(MsgID, 21, 11)   --ѡ�ť
			msg.AddMsgEvent(MsgID, 1, 211116)   --"��ȡÿ�ս���"


			msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100002)   --"ȡ��"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211115) -- "���ư�\n    ÿ�ս���ÿ��ֻ����ȡһ��,����㵱ǰ��ÿ�ս��ƻ�������ǰ������н���,ÿ��0�㽫���ÿ�ս��ƻ��֡�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid

			msg.AddMsgEvent(MsgID, 21, 11)   --ѡ�ť
			msg.AddMsgEvent(MsgID, 1, 211116)   --"��ȡÿ�ս���"

			msg.AddMsgEvent(MsgID, 21, 12)   --ѡ�ť
			msg.AddMsgEvent(MsgID, 1, 211117)   --"��ȡ���ս���"


			msg.AddMsgEvent(MsgID, 21, 20)   --ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100002)   --"ȡ��"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end

	elseif TalkIndex == 11 then           --��ȡÿ�ս���
		local g = role.GetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_got"])		--ÿ�ս����Ƿ�����ȡ
		local level_today = nil			--levelΪ���������λ
		if g < 1 then									--��ҽ���û��ȡ������
			if nowtime >= 1200 and nowtime <=2230 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211208) -- ���ư�\n    ���ڲ�����ȡ����
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				for h = 11, 13 do
					if act.GetActScriptData(56, 1, h) == RoleID then		--���Ϊǰ����ʱ
						level_today = h
						break
					end
				end

				if level_today then
					--����level_today���轱��
					if level_today == 11 then
						if role.GetBagFreeSize(RoleID) < 4 then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 211105) -- "���ư�\n    ����Ҫ�пռ���ܶһ�������"
							msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
							msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
							msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
							msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
						else
							role.AddRoleItem(MapID, InstanceID, RoleID, 4820260, 1, -1, 8, 420)
							role.AddRoleItem(MapID, InstanceID, RoleID, 3020091, 1, -1, 8, 420)
							role.AddRoleItem(MapID, InstanceID, RoleID, 4820035, 1, -1, 8, 420)
							role.AddRoleItem(MapID, InstanceID, RoleID, 4810047, 1, -1, 8, 420)
						end
					end

					if level_today == 12 then
						if role.GetBagFreeSize(RoleID) < 4 then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 211105) -- "���ư�\n    ����Ҫ�пռ���ܶһ�������"
							msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
							msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
							msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
							msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
						else
							role.AddRoleItem(MapID, InstanceID, RoleID, 4820260, 1, -1, 8, 420)
							role.AddRoleItem(MapID, InstanceID, RoleID, 3020087, 1, -1, 8, 420)
							role.AddRoleItem(MapID, InstanceID, RoleID, 4820035, 1, -1, 8, 420)
							role.AddRoleItem(MapID, InstanceID, RoleID, 4810048, 1, -1, 8, 420)
						end
					end
					if level_today == 13 then
						if role.GetBagFreeSize(RoleID) < 3 then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 211105) -- "���ư�\n    ����Ҫ�пռ���ܶһ�������"
							msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
							msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
							msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
							msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
						else
							role.AddRoleItem(MapID, InstanceID, RoleID, 3020084, 1, -1, 8, 420)
							role.AddRoleItem(MapID, InstanceID, RoleID, 4820035, 1, -1, 8, 420)
							role.AddRoleItem(MapID, InstanceID, RoleID, 4810049, 1, -1, 8, 420)
						end
					end
					role.SetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_got"], 1)			--���콱���Ϊ����ȡ
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 211118) -- "���ư�\n    ��Ļ���̫����,��û�����㽱����"
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
				end
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211120) -- "���ư�\n    ���Ѿ���ȡ�������ˣ�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end

	elseif TalkIndex == 12 then
		local level_all	= nil		--levelΪ���������λ
		local g2 = role.GetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_gotall"])	--���影���Ƿ�����ȡ
		if g2 < 1 then
			for h = 1, 3 do
				if act.GetActScriptData(56, 1, h) == RoleID then			--���Ϊǰ����ʱ
					level_all = h
					break
				end
			end

			if level_all then
				--����level_all���轱��
				if level_all == 1 then
					if role.GetBagFreeSize(RoleID) < 4 then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 211105) -- "���ư�\n    ����Ҫ�пռ���ܶһ�������"
						msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
						msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
						msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
						msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
					else
						role.AddRoleItem(MapID, InstanceID, RoleID, 3020134, 1, -1, 8, 420)
						role.AddRoleItem(MapID, InstanceID, RoleID, 3030079, 1, -1, 8, 420)
						role.AddRoleItem(MapID, InstanceID, RoleID, 4820263, 1, -1, 8, 420)
						role.AddRoleItem(MapID, InstanceID, RoleID, 4222701, 1, -1, 8, 420)
					end
				end

				if level_all == 2 then
					if role.GetBagFreeSize(RoleID) < 3 then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 211105) -- "���ư�\n    ����Ҫ�пռ���ܶһ�������"
						msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
						msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
						msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
						msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
					else
						role.AddRoleItem(MapID, InstanceID, RoleID, 3020131, 1, -1, 8, 420)
						role.AddRoleItem(MapID, InstanceID, RoleID, 3030078, 1, -1, 8, 420)
						role.AddRoleItem(MapID, InstanceID, RoleID, 4820260, 1, -1, 8, 420)
					end
				end
				if level_all == 3 then
					if role.GetBagFreeSize(RoleID) < 3 then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 211105) -- "���ư�\n    ����Ҫ�пռ���ܶһ�������"
						msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
						msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
						msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
						msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
					else
						role.AddRoleItem(MapID, InstanceID, RoleID, 3020126, 1, -1, 8, 420)
						role.AddRoleItem(MapID, InstanceID, RoleID, 3030078, 1, -1, 8, 420)
						role.AddRoleItem(MapID, InstanceID, RoleID, 4820260, 1, -1, 8, 420)
					end
				end
				role.SetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_gotall"], 1)			--���콱���Ϊ����ȡ
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211118) -- "���ư�\n    ��Ļ���̫����,��û�����㽱����"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end

		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211120) -- "���ư�\n    ���Ѿ���ȡ�������ˣ�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ

		end

	elseif TalkIndex == 7 then           --�˵��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 211119) -- "���ư�\n    ÿ�ջ��ְ��һ�������<color=0xff9933ff>����ʯ �����+19 �Ϲŵ伮 95��ʥ�°�<color=0xffffffff>\n    ÿ�ջ��ֵڶ��������<color=0xff9933ff>����ʯ �����+15 �Ϲŵ伮 85��ʥ�°�<color=0xffffffff>\n    ÿ�ջ��ֵ���������� <color=0xff9933ff>�����+12 �Ϲŵ伮 82��ʥ�°�<color=0xffffffff>\n    �ܻ��ֵ�һ�������<color=0xff9933ff>����ʯ+28 8���챦ʯ ����ʯ �ƽ�����<color=0xffffffff>\n    �ܻ��ֵڶ��������<color=0xff9933ff>����ʯ+25 7���챦ʯ ����ʯ <color=0xffffffff>\n    �ܻ��ֵ����������<color=0xff9933ff>����ʯ+20 7���챦ʯ ����ʯ<color=0xffffffff>"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	end

end

aux.RegisterCreatureEvent(6040104, 7, "n6040104_OnTalk")

function HuoDeJiangpaijifen(RoleID, Num)

	local p = role.GetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_all"])
	local i = role.GetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_today"])
	if p == nil then
	    p = 0
	end

	i = i + Num
	p = p + Num
	role.SetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_today"], i)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_all"], p)

	--��������������в���
	local arrID = {}			--��ʱ�������ID
	local arrNum = {}			--��ʱ�����������
	local bNew = 1				--�Ƿ�Ϊǰ��
	for l=1,3 do
		arrID[l] = act.GetActScriptData(56, 1, l)
		if arrID[l] == nil then			--ǰ������,��������0
		    arrID[l] = 0
		end
		if arrID[l] == RoleID  then		--���Ϊǰ����ʱ,�����������������
			arrNum[l] = p
			bNew = 0	--������Ϊǰ��
		else		--��ҷ�ǰ����ʱ
			arrNum[l] = act.GetActScriptData(56, 1, l+4)	--��ǰ�������ƻ������ݱ�������ʱ����
			if arrNum[l] == nil then		--����������0
				 arrNum[l] = 0
			 end
		end
	end

	if bNew == 1 then				--�����ҷ�ǰ����,�������ݱ���������λ
		arrID[4] = RoleID;
		arrNum[4] = p;
	else						--������Ϊǰ����,������λ��0
		arrID[4] = 0;
		arrNum[4] = 0;
	end

	for m=1, 3 do					--ѭ����ǰ�����Ƚ�,���������ɴ�С������ʱ����
		for n=m+1, 4 do
			if arrNum[n] > arrNum[m] then
				local temp = arrNum[m]
				arrNum[m] = arrNum[n]
				arrNum[n] = temp
				temp = arrID[m]
				arrID[m] = arrID[n]
				arrID[n] = temp
			end
		end
	end

	for j=1,3 do					--�����ź������ʱ����ǰ�������������ݿ���
		act.SetActScriptData(56, 1, j, arrID[j])
		act.SetActScriptData(56, 1, j+4, arrNum[j])
	end


	--����ÿ�ջ������в���
	--[[
	local arrID2 = {}
	local arrNum2 = {}
	--]]
	local bNewb = 1
	for x=11, 13 do
		arrID[x] = act.GetActScriptData(56, 1, x)
		if arrID[x] == nil then
		    arrID[x] = 0
		end
		if arrID[x] == RoleID  then
			arrNum[x] = i
			bNewb = 0
		else
			arrNum[x] = act.GetActScriptData(56, 1, x+4)
			if arrNum[x] == nil then
				  arrNum[x] = 0
			 end
		end
	end

	if bNewb == 1 then
		arrID[14] = RoleID;
		arrNum[14] = i;
	else
		arrID[14] = 0;
		arrNum[14] = 0;
	end

	for c=11, 13 do
		for v=c+1, 14 do
			if arrNum[v] > arrNum[c] then
				local temp = arrNum[c]
				arrNum[c] = arrNum[v]
				arrNum[v] = temp
				temp = arrID[c]
				arrID[c] = arrID[v]
				arrID[v] = temp
			end
		end
	end

	for h=11, 13 do
		act.SetActScriptData(56, 1, h, arrID[h])
		act.SetActScriptData(56, 1, h+4, arrNum[h])
	end


	act.SaveActScriptData(56)

end












