--s52,3373741864
--1���ɿ����Ի�����
--2����õ��Ի�����
qingrenjie_chuansongren = 0 -- ��¼���˽ڴ�����

--���Ͳ���������
function luomancun_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --��ȡ��ҵȼ�
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206501)		--�����������崫���ˣ�\n����������������ŮŮ������������֮�أ�������Ҫ��Ů��ϲ��ܴ��Ŀ�꣬��Ҫ���벩���������� \nÿ�ܶ������ġ�����������18��00~24��00����,��Ҫ���ʱ�䡣��
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 206502)		--���ǵ�,��Ҫ���벩�������塱
		msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local sex = role.GetRoleSex(RoleID)
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level < 30 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206503)		--���������崫���ˣ�\n    ֻ�г���30������Ҳ��ܽ��벩�������塣
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100001)		--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if sex == 1 then
				role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3373741864, 105, 3000, 369)--���� 105, 3000, 369

			elseif sex == 0 then
				role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3373741864, 411, 2955, 314)--Ů��411��2955��314

			end
		end
	end
end
aux.RegisterCreatureEvent(5600026, 7, "luomancun_OnTalk")
--�����ж�
function Nannv_OnEnterArea(MapID, InstanceID, RoleID, objID)
	local sex = role.GetRoleSex(RoleID)
	if objID == 500 then--Ů��

		if sex == 1 then
				local QuestID1 = msg.BeginMsgEvent()
				msg.AddMsgEvent(QuestID1, 13, 0)
				msg.AddMsgEvent(QuestID1, 1, 206528)--��߲�����������ݣ���������Ůʿ�ġ�
				msg.DispatchRoleMsgEvent(RoleID, QuestID1)
				role.RoleGotoNewMap(MapID, InstanceID, RoleID, MapID, 251, 3016, 215)--�м����� 251, 3016, 215
		end
	elseif objID == 501 then--����

		if sex == 0 then
				local QuestID1 = msg.BeginMsgEvent()
				msg.AddMsgEvent(QuestID1, 13, 0)
				msg.AddMsgEvent(QuestID1, 1, 206516)--��߲�����������ݣ�����������ʿ�ġ�
				msg.DispatchRoleMsgEvent(RoleID, QuestID1)
				role.RoleGotoNewMap(MapID, InstanceID, RoleID, MapID, 251, 3016, 215)--�м�����
		end
	end
end
aux.RegisterMapEvent( "s52", 7, "Nannv_OnEnterArea")
--������
function s52_OnPlayerEnter(MapID, InstanceID,RoleID)
	local Q_wupin = {}
	--���˽���Ʒ
	Q_wupin[1] = {id = 4800062}
	Q_wupin[2] = {id = 4800063}
	Q_wupin[3] = {id = 4800064}
	Q_wupin[4] = {id = 4800065}	
	Q_wupin[5] = {id = 4800066}
	Q_wupin[6] = {id = 4800067}	
	Q_wupin[7] = {id = 4800068}
	Q_wupin[8] = {id = 4800069}	
	Q_wupin[9] = {id = 4800070}
	Q_wupin[10] = {id = 4800071}
	for i=1,10 do
		local num = role.GetRoleItemNum(RoleID, Q_wupin[i].id)
		role.RemoveFromRole(MapID, InstanceID, RoleID, Q_wupin[i].id, num, 101)
	end
end
--ע��
aux.RegisterMapEvent("s52", 3, "s52_OnPlayerEnter")
--�뿪���˽�
function Qchukou_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206548)		--����ս��Ǵ����ˣ�\n    ��Ҫ�뿪������������\n    �뿪����<I>��̵���<I>��</I>�����<I>��</I>����¶<I>��</I>�߼��ɿ�����<I>��</I>�ɿ���ģ��<I>��</I>�߼������<I>��</I>����õ��<I>��</I>ȼ��õ��<I>��</I>����ɿ���<I>��</I>�����ɿ���</I>������ɾ������
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100001)		--��ȷ����
		msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then

		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3424073512, 685, 23510, 620)--

	end
end
aux.RegisterCreatureEvent(5600034, 7, "Qchukou_OnTalk")
--õ����ֲ
function meigui_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local sex = role.GetRoleSex(RoleID)
	local growtime = role.GetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"])--��õ��Ի�����
	local buff = role.IsRoleHaveBuff(MapID, InstanceID,RoleID,60005)--45s buff ID
	local a = math.random(1, 100)
	local laodong_num = role.GetRoleItemNum(RoleID, 4800076)--�Ͷ�NUM
	local day = os.date("%A")
	local curhour = tonumber(os.date("%H"))
	if (day == "Tuesday" or day == "Thursday" or day == "Saturday" or day == "Sunday") then
		if (curhour >= 18 and curhour <= 24) then
			if sex == 1 then
				if buff ~= true then
					if laodong_num > 0 then
						if TalkIndex == -1 then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 206504)	-- "��ȷ��Ҫ�չ�õ�廨ô��"
							msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
							msg.AddMsgEvent(MsgID, 1, 100001)	-- "ȷ��"
							msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ���
							msg.AddMsgEvent(MsgID, 1, 100002)	-- "ȡ��"
							msg.AddMsgEvent(MsgID, 24, TargetID)
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						elseif TalkIndex == 4 then	
							if growtime < 5 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206505)	-- "�չ�õ�廨��Ҫ����1�ݡ���¶����1�ݡ�����¶�������ν�ˮ֮������45�룻\n������¶����Ч���ǡ���¶����5����"
								msg.AddMsgEvent(MsgID, 21, 6)	-- ѡ��һ
								msg.AddMsgEvent(MsgID, 1, 206506)	-- "ʹ�á���¶��"
								msg.AddMsgEvent(MsgID, 21, 7)	-- ѡ���
								msg.AddMsgEvent(MsgID, 1, 206507)	-- "ʹ�á�����¶��"
								msg.AddMsgEvent(MsgID, 24, TargetID)
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							elseif growtime >= 5 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206508)	-- "�չ�õ�廨��Ҫ����1�ݡ����������1�ݡ��߼�������������ν�ˮ֮������45�룻\n���߼����������Ч���ǡ����������5����"
								msg.AddMsgEvent(MsgID, 21, 8)	-- ѡ��һ
								msg.AddMsgEvent(MsgID, 1, 206509)	-- "ʹ�á��������"
								msg.AddMsgEvent(MsgID, 21, 9)	-- ѡ���
								msg.AddMsgEvent(MsgID, 1, 206510)	-- "ʹ�á��߼��������"
								msg.AddMsgEvent(MsgID, 24, TargetID)
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							end
						elseif TalkIndex == 6 then
							local QNum = role.GetRoleItemNum(RoleID, 4800060)--����¶������
							if QNum <= 0 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206511) --"��û�С���¶�����޷���õ�廨���й��"
								msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
								msg.AddMsgEvent(MsgID, 1, 100002)	-- "�뿪"
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								local FreeSize = role.GetBagFreeSize(RoleID)
								if FreeSize >= 3 then
									unit.AddBuff(MapID, InstanceID, RoleID, 6000501, RoleID)--45sbuff
									role.RemoveFromRole(MapID, InstanceID, RoleID, 4800060, 1, 101)---------ɾ������¶��
									local c = math.random(1, 1000)
									if c <= 10 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800068, 1, -1, 8, 420) --����õ��
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300032)--XX��ñ���õ�壬�ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 10 and c <= 15 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800069, 1, -1, 8, 420) --ȼ��õ��
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300033)--XX���ȼ��õ�壬�ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 15 and c <= 17 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800073, 1, -1, 8, 420) --����֮��
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300036)--XX�������֮�ǣ��ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 17 and c <= 27 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800052, 1, -1, 8, 420) --�氮õ��
									end
									local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
									if level >= 30 and level <= 39 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 2000)
									elseif level >= 40 and level <= 49 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 4000)
									elseif level >= 50 and level <= 59 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 5000)
									elseif level >= 60 and level <= 69 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 8000)
									elseif level >= 70 and level <= 79 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 10000)
									elseif level >= 80 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 15000)
									end
									growtime = growtime + 1
									if growtime == 5 then
										if a <= 95 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800062, 5, -1, 8, 420)--�������*3
										elseif a > 95 then
											local b = math.random(1, 10)
											if b <= 1 then
												role.AddRoleItem(MapID, InstanceID, RoleID, 4800065, 1, -1, 8, 420)
												local MsgID = msg.BeginMsgEvent();	--ע��㲥���
												msg.AddMsgEvent(MsgID,102,300030)--<p1>��ø߼��ɿ����ۣ��ڴ�ף��
												msg.AddMsgEvent(MsgID, 2, RoleID)
												msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											elseif b > 1 then
												role.AddRoleItem(MapID, InstanceID, RoleID, 4800066, 1, -1, 8, 420)
												local MsgID = msg.BeginMsgEvent();	--ע��㲥���
												msg.AddMsgEvent(MsgID,102,300031)--<p1>����ɿ���ģ�ߣ��ڴ�ף��
												msg.AddMsgEvent(MsgID, 2, RoleID)
												msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											end
										end
									end
									role.SetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"], growtime)--��¼��������
								else
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 26, 100003)
									msg.AddMsgEvent(MsgID, 9, 3)
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
								end
							end
						elseif TalkIndex == 7 then
							local QNum = role.GetRoleItemNum(RoleID, 4800064)--������¶������
							if QNum <= 0 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206512) --"��û�С�����¶�����޷���õ�廨���й��"
								msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
								msg.AddMsgEvent(MsgID, 1, 100002)	-- "�뿪"
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								local FreeSize = role.GetBagFreeSize(RoleID)
								if FreeSize >= 3 then
									unit.AddBuff(MapID, InstanceID, RoleID, 6000501, RoleID)--45sbuff
									role.RemoveFromRole(MapID, InstanceID, RoleID, 4800064, 1, 101)---------ɾ������¶
									local c = math.random(1, 1000)
									if c <= 10 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800068, 1, -1, 8, 420) --����õ��
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300032)--XX��ñ���õ�壬�ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 10 and c <= 15 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800069, 1, -1, 8, 420) --ȼ��õ��
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300033)--XX���ȼ��õ�壬�ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 15 and c <= 17 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800073, 1, -1, 8, 420) --����֮��
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300036)--XX�������֮�ǣ��ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 17 and c <= 27 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800052, 1, -1, 8, 420) --�氮õ��
									end
									local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
									if level >= 30 and level <= 39 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 14000)
									elseif level >= 40 and level <= 49 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 28000)
									elseif level >= 50 and level <= 59 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 35000)
									elseif level >= 60 and level <= 69 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 56000)
									elseif level >= 70 and level <= 79 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 70000)
									elseif level >= 80 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 105000)
									end
									growtime = growtime + 5
									if growtime >= 5 then
										if a <= 95 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800062, 5, -1, 8, 420)--�������*3
										elseif a > 95 then
											local b = math.random(1, 10)
											if b <= 1 then
												role.AddRoleItem(MapID, InstanceID, RoleID, 4800065, 1, -1, 8, 420)
												local MsgID = msg.BeginMsgEvent();	--ע��㲥���
												msg.AddMsgEvent(MsgID,102,300030)--<p1>��ø߼��ɿ����ۣ��ڴ�ף��
												msg.AddMsgEvent(MsgID, 2, RoleID)
												msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											elseif b > 1 then
												role.AddRoleItem(MapID, InstanceID, RoleID, 4800066, 1, -1, 8, 420)
												local MsgID = msg.BeginMsgEvent();	--ע��㲥���
												msg.AddMsgEvent(MsgID,102,300031)--<p1>����ɿ���ģ�ߣ��ڴ�ף��
												msg.AddMsgEvent(MsgID, 2, RoleID)
												msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											end
										end
									end
									role.SetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"], growtime)--��¼��������
								else
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 26, 100003)
									msg.AddMsgEvent(MsgID, 9, 3)
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
								end
							end
						elseif TalkIndex == 8 then
							local QNum = role.GetRoleItemNum(RoleID, 4800063)--�������������
							if QNum <= 0 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206513) --"��û�С�����������޷�Ϊõ�廨���г���"
								msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
								msg.AddMsgEvent(MsgID, 1, 100002)	-- "�뿪"
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								local FreeSize = role.GetBagFreeSize(RoleID)
								if FreeSize >= 3 then
									unit.AddBuff(MapID, InstanceID, RoleID, 6000501, RoleID)--45sbuff
									role.RemoveFromRole(MapID, InstanceID, RoleID, 4800063, 1, 101)---------ɾ�������
									local c = math.random(1, 1000)
									if c <= 10 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800068, 1, -1, 8, 420) --����õ��
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300032)--XX��ñ���õ�壬�ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 10 and c <= 15 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800069, 1, -1, 8, 420) --ȼ��õ��
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300033)--XX���ȼ��õ�壬�ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 15 and c <= 17 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800073, 1, -1, 8, 420) --����֮��
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300036)--XX�������֮�ǣ��ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 17 and c <= 27 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800052, 1, -1, 8, 420) --�氮õ��
									elseif c > 27 and c <= 37 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800072, 1, -1, 8, 420) --�������� 
									end
									local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
									if level >= 30 and level <= 39 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 3000)
									elseif level >= 40 and level <= 49 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 6000)
									elseif level >= 50 and level <= 59 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 10000)
									elseif level >= 60 and level <= 69 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 12000)
									elseif level >= 70 and level <= 79 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 20000)
									elseif level >= 80 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 25000)
									end
									growtime = growtime + 1
									if growtime >= 10 then
										role.RemoveFromRole(MapID, InstanceID, RoleID, 4800076, 1, 101)--ɾ���Ͷ�֤
										if a <= 95 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800075, 1, -1, 8, 420)
											growtime = 0
										elseif a > 95 and a <= 99 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800075, 1, -1, 8, 420)
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800068, 1, -1, 8, 420)
											local MsgID = msg.BeginMsgEvent();	--ע��㲥���
											msg.AddMsgEvent(MsgID,102,300032)--XX��ñ���õ�壬�ڴ�ף��
											msg.AddMsgEvent(MsgID, 2, RoleID)
											msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											growtime = 0
										elseif a >99 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800075, 1, -1, 8, 420)
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800069, 1, -1, 8, 420)
											local MsgID = msg.BeginMsgEvent();	--ע��㲥���
											msg.AddMsgEvent(MsgID,102,300033)--XX���ȼ��õ�壬�ڴ�ף��
											msg.AddMsgEvent(MsgID, 2, RoleID)
											msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											growtime = 0
										end
									end
									role.SetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"], growtime)--��¼��������
								else
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 26, 100003)
									msg.AddMsgEvent(MsgID, 9, 3)
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
								end
							end
						elseif TalkIndex == 9 then
							local QNum = role.GetRoleItemNum(RoleID, 4800067)--���߼������������
							if QNum <= 0 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206514) --"��û�С��߼�����������޷�Ϊõ�廨���г���"
								msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
								msg.AddMsgEvent(MsgID, 1, 100002)	-- "�뿪"
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								local FreeSize = role.GetBagFreeSize(RoleID)
								if FreeSize >= 3 then
									unit.AddBuff(MapID, InstanceID, RoleID, 6000501, RoleID)--45sbuff
									role.RemoveFromRole(MapID, InstanceID, RoleID, 4800067, 1, 101)---------ɾ���߼������
									local c = math.random(1, 1000)
									if c <= 10 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800068, 1, -1, 8, 420) --����õ��
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300032)--XX��ñ���õ�壬�ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 10 and c <= 15 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800069, 1, -1, 8, 420) --ȼ��õ��
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300033)--XX���ȼ��õ�壬�ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 15 and c <= 17 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800073, 1, -1, 8, 420) --����֮��
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300036)--XX�������֮�ǣ��ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 17 and c <= 27 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800052, 1, -1, 8, 420) --�氮õ��
									elseif c > 27 and c <= 37 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800072, 1, -1, 8, 420) --�������� 
									end
									local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
									if level >= 30 and level <= 39 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 21000)
									elseif level >= 40 and level <= 49 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 42000)
									elseif level >= 50 and level <= 59 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 70000)
									elseif level >= 60 and level <= 69 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 84000)
									elseif level >= 70 and level <= 79 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 140000)
									elseif level >= 80 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 175000)
									end
									growtime = growtime + 5
									if growtime >= 10 then
										role.RemoveFromRole(MapID, InstanceID, RoleID, 4800076, 1, 101)--ɾ���Ͷ�֤
										if a <= 95 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800075, 1, -1, 8, 420)
											growtime = 0
										elseif a > 95 and a <= 99 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800075, 1, -1, 8, 420)
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800068, 1, -1, 8, 420)
											local MsgID = msg.BeginMsgEvent();	--ע��㲥���
											msg.AddMsgEvent(MsgID,102,300032)--XX��ñ���õ�壬�ڴ�ף��
											msg.AddMsgEvent(MsgID, 2, RoleID)
											msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											growtime = 0
										elseif a >99 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800075, 1, -1, 8, 420)
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800069, 1, -1, 8, 420)
											local MsgID = msg.BeginMsgEvent();	--ע��㲥���
											msg.AddMsgEvent(MsgID,102,300033)--XX���ȼ��õ�壬�ڴ�ף��
											msg.AddMsgEvent(MsgID, 2, RoleID)
											msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											growtime = 0
										end
									end
									role.SetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"], growtime)--��¼��������
								else
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 26, 100003)
									msg.AddMsgEvent(MsgID, 9, 3)
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
								end
							end
						end
					else
						local QuestID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(QuestID1, 13, 0)
							msg.AddMsgEvent(QuestID1, 1, 206549)--������û���������Ͷ�֤�����ܽ��в�����
							msg.DispatchRoleMsgEvent(RoleID, QuestID1)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206515) --����һ�ε�ʱ�仹û�е����뵽ʱ��������
					msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 100002)	-- "�뿪"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					
				end
			elseif sex == 0 then
				local QuestID1 = msg.BeginMsgEvent()
				msg.AddMsgEvent(QuestID1, 13, 0)
				msg.AddMsgEvent(QuestID1, 1, 206516)--��߲�����������ݣ�����������ʿ�ġ�
				msg.DispatchRoleMsgEvent(RoleID, QuestID1)
			end
		else
			local QuestID1 = msg.BeginMsgEvent()
			msg.AddMsgEvent(QuestID1, 13, 0)
			msg.AddMsgEvent(QuestID1, 1, 206567)--����������������Ͷ�ֻ����ÿ�ܶ����ġ������յ�18:00����24:00���У�����ʱ�䲩������������Ҫ��Ϣ�ġ�
			msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		end
	else
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 206567)--����������������Ͷ�ֻ����ÿ�ܶ����ġ������յ�18:00����24:00���У�����ʱ�䲩������������Ҫ��Ϣ�ġ�
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
	end
end
aux.RegisterCreatureEvent(6030010, 7, "meigui_OnTalk")
--�ɿ�������
function qiaokeli_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local sex = role.GetRoleSex(RoleID)
	local growtime = role.GetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"])--�ɿ����Ի�����
	local buff = role.IsRoleHaveBuff(MapID, InstanceID,RoleID, 60005)--45s buff ID
	local a = math.random(1, 100)
	local laodong_num = role.GetRoleItemNum(RoleID, 4800076)--�Ͷ�NUM
	local day = os.date("%A")
	local curhour = tonumber(os.date("%H"))
	if (day == "Tuesday" or day == "Thursday" or day == "Saturday" or day == "Sunday") then
		if (curhour >= 18 and curhour <= 24) then
			if sex == 0 then
				if buff ~= true then
					if laodong_num > 0 then
						if TalkIndex == -1 then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 206517)	-- "��ȷ��Ҫ�����ɿ���ô��"
							msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
							msg.AddMsgEvent(MsgID, 1, 100001)	-- "ȷ��"
							msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ���
							msg.AddMsgEvent(MsgID, 1, 100002)	-- "ȡ��"
							msg.AddMsgEvent(MsgID, 24, TargetID)
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						elseif TalkIndex == 4 then
							if  growtime < 5 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206518)	-- "�����ɿ�����Ҫ����1�ݡ��ɿ����ۡ���1�ݡ��߼��ɿ����ۡ�����������֮������45�룻\n���߼��ɿ����ۡ���Ч���ǡ��ɿ����ۡ���5����"
								msg.AddMsgEvent(MsgID, 21, 6)	-- ѡ��һ
								msg.AddMsgEvent(MsgID, 1, 206519)	-- "ʹ�á��ɿ����ۡ�"
								msg.AddMsgEvent(MsgID, 21, 7)	-- ѡ���
								msg.AddMsgEvent(MsgID, 1, 206520)	-- "ʹ�á��߼��ɿ����ۡ�"
								msg.AddMsgEvent(MsgID, 24, TargetID)
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							elseif growtime >= 5 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206521)	-- "�����ɿ�����Ҫ����1�ݡ���̵��ߡ���1�ݡ��ɿ���ģ�ߡ�����������֮������45�룻\n���ɿ���ģ�ߡ���Ч���ǡ���̵��ߡ���5����"
								msg.AddMsgEvent(MsgID, 21, 8)	-- ѡ��һ
								msg.AddMsgEvent(MsgID, 1, 206522)	-- "ʹ�á���̵��ߡ�"
								msg.AddMsgEvent(MsgID, 21, 9)	-- ѡ���
								msg.AddMsgEvent(MsgID, 1, 206523)	-- "ʹ�á��ɿ���ģ�ߡ�"
								msg.AddMsgEvent(MsgID, 24, TargetID)
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							end
						elseif TalkIndex == 6 then
							local QNum = role.GetRoleItemNum(RoleID, 4800061)--���ɿ����ۡ�����
							if QNum <= 0 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206524) --"��û�С��ɿ����ۡ����޷������ɿ���������"
								msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
								msg.AddMsgEvent(MsgID, 1, 100002)	-- "�뿪"
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								local FreeSize = role.GetBagFreeSize(RoleID)
								if FreeSize >= 3 then
									unit.AddBuff(MapID, InstanceID, RoleID, 6000501, RoleID)--45sbuff
									role.RemoveFromRole(MapID, InstanceID, RoleID, 4800061, 1, 101)---------ɾ���ɿ�����
									local c = math.random(1, 1000)
									if c <= 10 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800070, 1, -1, 8, 420) --����ɿ���
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300034)--XX�������ɿ������ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 10 and c <= 15 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800071, 1, -1, 8, 420) --�����ɿ���
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300035)--XX��þ����ɿ������ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 15 and c <= 17 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800073, 1, -1, 8, 420) --����֮��
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300036)--XX�������֮�ǣ��ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 17 and c <= 27 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800051, 1, -1, 8, 420) --�氮�ɿ���
									end
									local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
									if level >= 30 and level <= 39 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 2000)
									elseif level >= 40 and level <= 49 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 4000)
									elseif level >= 50 and level <= 59 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 5000)
									elseif level >= 60 and level <= 69 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 8000)
									elseif level >= 70 and level <= 79 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 10000)
									elseif level >= 80 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 15000)
									end
									growtime = growtime + 1
									if growtime == 5 then
										if a <= 95 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800063, 5, -1, 8, 420)--�����*3
										elseif a > 95 then--�߼������*1������¶*1
											local b = math.random(1, 10)
											if b > 1 then
												role.AddRoleItem(MapID, InstanceID, RoleID, 4800067, 1, -1, 8, 420)
												local MsgID = msg.BeginMsgEvent();	--ע��㲥���
												msg.AddMsgEvent(MsgID,102,300046)--XX��ø߼���������ڴ�ף��
												msg.AddMsgEvent(MsgID, 2, RoleID)
												msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											elseif b <= 1 then
												role.AddRoleItem(MapID, InstanceID, RoleID, 4800064, 1, -1, 8, 420)
												local MsgID = msg.BeginMsgEvent();	--ע��㲥���
												msg.AddMsgEvent(MsgID,102,300045)--XX�������¶���ڴ�ף��
												msg.AddMsgEvent(MsgID, 2, RoleID)
												msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											end
										end
									end
									role.SetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"], growtime)--��¼��������
								else
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 26, 100003)
									msg.AddMsgEvent(MsgID, 9, 3)
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
								end
							end
						elseif TalkIndex == 7 then
							local QNum = role.GetRoleItemNum(RoleID, 4800065)--���߼��ɿ����ۡ�����
							if QNum <= 0 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206525) --"��û�С��߼��ɿ����ۡ����޷������ɿ���������"
								msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
								msg.AddMsgEvent(MsgID, 1, 100002)	-- "�뿪"
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								local FreeSize = role.GetBagFreeSize(RoleID)
								if FreeSize >= 3 then
									unit.AddBuff(MapID, InstanceID, RoleID, 6000501, RoleID)--45sbuff
									role.RemoveFromRole(MapID, InstanceID, RoleID, 4800065, 1, 101)---------ɾ���ɿ�����
									local c = math.random(1, 1000)
									if c <= 10 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800070, 1, -1, 8, 420) --����ɿ���
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300034)--XX�������ɿ������ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 10 and c <= 15 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800071, 1, -1, 8, 420) --�����ɿ���
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300035)--XX��þ����ɿ������ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 15 and c <= 17 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800073, 1, -1, 8, 420) --����֮��
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300036)--XX�������֮�ǣ��ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 17 and c <= 27 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800051, 1, -1, 8, 420) --�氮�ɿ���
									end
									local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
									if level >= 30 and level <= 39 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 14000)
									elseif level >= 40 and level <= 49 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 28000)
									elseif level >= 50 and level <= 59 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 35000)
									elseif level >= 60 and level <= 69 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 56000)
									elseif level >= 70 and level <= 79 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 70000)
									elseif level >= 80 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 105000)
									end
									growtime = growtime + 5
									if growtime >= 5 then
										if a <= 95 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800063, 5, -1, 8, 420)--�����*3
										elseif a > 95 then--�߼������*1������¶*1
											local b = math.random(1, 10)
											if b > 1 then
												role.AddRoleItem(MapID, InstanceID, RoleID, 4800067, 1, -1, 8, 420)
												local MsgID = msg.BeginMsgEvent();	--ע��㲥���
												msg.AddMsgEvent(MsgID,102,300046)--XX��ø߼���������ڴ�ף��
												msg.AddMsgEvent(MsgID, 2, RoleID)
												msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											elseif b <= 1 then
												role.AddRoleItem(MapID, InstanceID, RoleID, 4800064, 1, -1, 8, 420)
												local MsgID = msg.BeginMsgEvent();	--ע��㲥���
												msg.AddMsgEvent(MsgID,102,300045)--XX�������¶���ڴ�ף��
												msg.AddMsgEvent(MsgID, 2, RoleID)
												msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											end
										end
									end
									role.SetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"], growtime)--��¼��������
								else
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 26, 100003)
									msg.AddMsgEvent(MsgID, 9, 3)
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
								end
							end
						elseif TalkIndex == 8 then
							local QNum = role.GetRoleItemNum(RoleID, 4800062)--����̵��ߡ�����
							if QNum <= 0 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206526) --"��û�С���̵��ߡ����޷�Ϊ�ɿ�������"
								msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
								msg.AddMsgEvent(MsgID, 1, 100002)	-- "�뿪"
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								local FreeSize = role.GetBagFreeSize(RoleID)
								if FreeSize >= 3 then
									unit.AddBuff(MapID, InstanceID, RoleID, 6000501, RoleID)--45sbuff
									role.RemoveFromRole(MapID, InstanceID, RoleID, 4800062, 1, 101)---------ɾ���ɿ�����
									local c = math.random(1, 1000)
									if c <= 10 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800070, 1, -1, 8, 420) --����ɿ���
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300034)--XX�������ɿ������ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 10 and c <= 15 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800071, 1, -1, 8, 420) --�����ɿ���
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300035)--XX��þ����ɿ������ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 15 and c <= 17 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800073, 1, -1, 8, 420) --����֮��
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300036)--XX�������֮�ǣ��ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 17 and c <= 27 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800051, 1, -1, 8, 420) --�氮�ɿ���
									elseif c > 27 and c <= 37 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800072, 1, -1, 8, 420) --�������� 
									end
									local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
									if level >= 30 and level <= 39 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 3000)
									elseif level >= 40 and level <= 49 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 6000)
									elseif level >= 50 and level <= 59 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 10000)
									elseif level >= 60 and level <= 69 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 12000)
									elseif level >= 70 and level <= 79 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 20000)
									elseif level >= 80 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 25000)
									end
									growtime = growtime + 1
									if growtime >= 10 then
										role.RemoveFromRole(MapID, InstanceID, RoleID, 4800076, 1, 101)--ɾ���Ͷ�֤
										if a <= 95 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800074, 1, -1, 8, 420)
											growtime = 0
										elseif a > 95 and a <= 99 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800074, 1, -1, 8, 420)
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800070, 1, -1, 8, 420)
											local MsgID = msg.BeginMsgEvent();	--ע��㲥���
											msg.AddMsgEvent(MsgID,102,300034)--XX�������ɿ������ڴ�ף��
											msg.AddMsgEvent(MsgID, 2, RoleID)
											msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											growtime = 0
										elseif a >99 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800074, 1, -1, 8, 420)
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800071, 1, -1, 8, 420)
											local MsgID = msg.BeginMsgEvent();	--ע��㲥���
											msg.AddMsgEvent(MsgID,102,300035)--XX��þ����ɿ������ڴ�ף��
											msg.AddMsgEvent(MsgID, 2, RoleID)
											msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											growtime = 0
										end
									end
									role.SetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"], growtime)--��¼��������
								else
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 26, 100003)
									msg.AddMsgEvent(MsgID, 9, 3)
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
								end
							end
						elseif TalkIndex == 9 then
							local QNum = role.GetRoleItemNum(RoleID, 4800066)--���ɿ���ģ�ߡ�����
							if QNum <= 0 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206527) --"��û�С��ɿ���ģ�ߡ����޷�Ϊ�ɿ�������"
								msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
								msg.AddMsgEvent(MsgID, 1, 100002)	-- "�뿪"
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								local FreeSize = role.GetBagFreeSize(RoleID)
								if FreeSize >= 3 then
									unit.AddBuff(MapID, InstanceID, RoleID, 6000501, RoleID)--45sbuff
									role.RemoveFromRole(MapID, InstanceID, RoleID, 4800066, 1, 101)---------ɾ���ɿ�����
									local c = math.random(1, 1000)
									if c <= 10 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800070, 1, -1, 8, 420) --����ɿ���
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300034)--XX�������ɿ������ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 10 and c <= 15 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800071, 1, -1, 8, 420) --�����ɿ���
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300035)--XX��þ����ɿ������ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 15 and c <= 17 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800073, 1, -1, 8, 420) --����֮��
										local MsgID = msg.BeginMsgEvent();	--ע��㲥���
										msg.AddMsgEvent(MsgID,102,300036)--XX�������֮�ǣ��ڴ�ף��
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 17 and c <= 27 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800051, 1, -1, 8, 420) --�氮�ɿ���
									elseif c > 27 and c <= 37 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800072, 1, -1, 8, 420) --�������� 
									end
									local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
									if level >= 30 and level <= 39 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 21000)
									elseif level >= 40 and level <= 49 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 42000)
									elseif level >= 50 and level <= 59 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 70000)
									elseif level >= 60 and level <= 69 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 84000)
									elseif level >= 70 and level <= 79 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 140000)
									elseif level >= 80 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 175000)
									end
									growtime = growtime + 5
									if growtime >= 10 then
										role.RemoveFromRole(MapID, InstanceID, RoleID, 4800076, 1, 101)--ɾ���Ͷ�֤
										if a <= 95 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800074, 1, -1, 8, 420)
											growtime = 0
										elseif a > 95 and a <= 99 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800074, 1, -1, 8, 420)
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800070, 1, -1, 8, 420)
											local MsgID = msg.BeginMsgEvent();	--ע��㲥���
											msg.AddMsgEvent(MsgID,102,300034)--XX�������ɿ������ڴ�ף��
											msg.AddMsgEvent(MsgID, 2, RoleID)
											msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											growtime = 0
										elseif a >99 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800074, 1, -1, 8, 420)
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800071, 1, -1, 8, 420)
											local MsgID = msg.BeginMsgEvent();	--ע��㲥���
											msg.AddMsgEvent(MsgID,102,300035)--XX��þ����ɿ������ڴ�ף��
											msg.AddMsgEvent(MsgID, 2, RoleID)
											msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											growtime = 0
										end
									end
									role.SetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"], growtime)--��¼��������
								else
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 26, 100003)
									msg.AddMsgEvent(MsgID, 9, 3)
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
								end
							end
						end
					else
						local QuestID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(QuestID1, 13, 0)
						msg.AddMsgEvent(QuestID1, 1, 206549)--������û���������Ͷ�֤�����ܽ��в�����
						msg.DispatchRoleMsgEvent(RoleID, QuestID1)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206515) --����һ�ε�ʱ�仹û�е����뵽ʱ��������
					msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 100002)	-- "�뿪"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			elseif sex == 1 then
				local QuestID1 = msg.BeginMsgEvent()
				msg.AddMsgEvent(QuestID1, 13, 0)
				msg.AddMsgEvent(QuestID1, 1, 206528)--��߲�����������ݣ���������Ůʿ�ġ�
				msg.DispatchRoleMsgEvent(RoleID, QuestID1)
			end
		else
			local QuestID1 = msg.BeginMsgEvent()
			msg.AddMsgEvent(QuestID1, 13, 0)
			msg.AddMsgEvent(QuestID1, 1, 206567)--����������������Ͷ�ֻ����ÿ�ܶ����ġ������յ�18:00����24:00���У�����ʱ�䲩������������Ҫ��Ϣ�ġ�
			msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		end
	else
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 206567)--����������������Ͷ�ֻ����ÿ�ܶ����ġ������յ�18:00����24:00���У�����ʱ�䲩������������Ҫ��Ϣ�ġ�
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
	end
end
aux.RegisterCreatureEvent(6030011, 7, "qiaokeli_OnTalk")
