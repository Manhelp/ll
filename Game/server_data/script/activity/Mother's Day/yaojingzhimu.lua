--����֮ĸˢ�£��߻�֮��606��3881��1303����ĸ�׽ڴ�ʹˢ�µص㣨��ս���700��23510��765��
--���ʼ
yaojingzhimu = 0
muqinjiedashi = 0 
Muqinjie_start = 0
function muqinjie_OnTimerMin(actID)
	local curmon = tonumber(os.date("%m"))--��
	local curday = tonumber(os.date("%d"))--��
	local curyear = tonumber(os.date("%Y"))--��
	if yaojingzhimu == 0 then
		if curyear == 2011 and curmon == 5 and (curday >= 4 and curday <= 17) then
			yaojingzhimu = map.MapCreateCreature(3424073256, -1, 5600055, 606, 3881, 1303)-- 28163   23510   29803
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 300049)    --����֮ĸ�Ѿ��������߻�֮����<606, 1303>����ĸ�׽ھʹ��Ѿ���ʼ�ˣ����Ҫ�����μӰ���
			msg.DispatchWorldMsgEvent(MsgID)
			Muqinjie_start = 1
		end
	else
		if curyear == 2011 and curmon == 5 and curday > 17 then
			map.MapDeleteCreature(3424073256, -1, yaojingzhimu)
			Muqinjie_start = 2
		end
	end	
end
aux.RegisterActEvent(39, 4, "muqinjie_OnTimerMin")
--�����
function muqinjie_OnEnd(actID)
	if yaojingzhimu ~= 0 then
		map.MapDeleteCreature(3424073256, -1, yaojingzhimu)
		Muqinjie_start = 2
	end
end
aux.RegisterActEvent(39, 3, "muqinjie_OnEnd")
--ħ���󱦲�
function c6010043_OnDie(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	local a = math.random(1,10)
	local curmon = tonumber(os.date("%m"))--��
	local curday = tonumber(os.date("%d"))--��
	local curyear = tonumber(os.date("%Y"))--��
	if curyear == 2011 and curmon == 5 and (curday >= 4 and curday <= 17) then
		--if Muqinjie_start == 1 then
			local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, KillerID)
			if TeamID ~= nil and TeamID ~= 4294967295 then
				local Role = {}
				Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
				for i=1, 6 do
					if a > 0 and a <= 6 then
						role.AddRoleItem(MapID, InstanceID, Role[i], 4800087, 2, 0, 0, 402)
					else
						role.AddRoleItem(MapID, InstanceID, Role[i], 4800088, 2, 0, 0, 402)
					end
				end
			elseif TeamID ~= nil then
				if a > 0 and a <= 6 then
					role.AddRoleItem(MapID, InstanceID, KillerID, 4800087, 2, 0, 0, 402)
				else
					role.AddRoleItem(MapID, InstanceID, KillerID, 4800088, 2, 0, 0, 402)
				end
			end
		end
	
end
--ע��
aux.RegisterCreatureEvent(6010043, 4, "c6010043_OnDie")
aux.RegisterCreatureEvent(6010044, 4, "c6010043_OnDie")
aux.RegisterCreatureEvent(6010045, 4, "c6010043_OnDie")
aux.RegisterCreatureEvent(6010031, 4, "c6010043_OnDie")
aux.RegisterCreatureEvent(6010032, 4, "c6010043_OnDie")
aux.RegisterCreatureEvent(6010033, 4, "c6010043_OnDie")
aux.RegisterCreatureEvent(6010034, 4, "c6010043_OnDie")
aux.RegisterCreatureEvent(6010035, 4, "c6010043_OnDie")
aux.RegisterCreatureEvent(6010049, 4, "c6010043_OnDie")

--����ܰ����
function c6030013_On_Die(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)	
	if Muqinjie_start == 1 then
		local FreeSize = role.GetBagFreeSize(KillerID)
		if(FreeSize >= 1) then
			role.AddRoleItem(MapID, InstanceID, KillerID, 4800086, 1, 0, 0, 402)
		elseif (FreeSize < 1) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.DispatchRoleMsgEvent(KillerID, MsgID)
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 206483)
		msg.DispatchRoleMsgEvent(KillerID, MsgID)
	end
end
--ע��
aux.RegisterCreatureEvent(6030013, 4, "c6030013_On_Die")
--����֮ĸ�Ի�
function yaojingzhimu_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	
	local JT2_num = role.GetRoleItemNum(RoleID, 3050010)--2�Ⱦ�������
	local JT3_num = role.GetRoleItemNum(RoleID, 3050011)--3�Ⱦ�������
	local HTB_num = role.GetRoleItemNum(RoleID, 4800090)--��ɫ���屾Դ
	local HTG_num = role.GetRoleItemNum(RoleID, 4800089)--��ɫ���屾Դ
	local num =  role.GetRoleItemNum(RoleID, 4800086)--����ܰ
	local Vocation = role.GetRoleClass(RoleID)--���ְҵ
	local a = math.random(1,9)
	local b = math.random(1,9)
	local CurTime = tonumber(os.date("%j"))
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["muqin_time"])
	local jingtie_num = role.GetRoleScriptData(RoleID, 1, RoleDataType["muqinyaojing"])
	if CurTime ~= UpdateTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["muqinyaojing"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["muqin_time"], CurTime)
	end
	if role.GetRoleLevel(MapID, InstanceID, RoleID) >= 30 then
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206432)		--������֮ĸ��\n    ��¡��¡���� (����֮ĸ�Ļ�����Ϊ�������ɢ���������岻���߽������)��
			msg.AddMsgEvent(MsgID, 24, TargetID)	
			msg.AddMsgEvent(MsgID, 21, 4)			
			msg.AddMsgEvent(MsgID, 1, 206433)		-- �޲������Ѻ�
			msg.AddMsgEvent(MsgID, 21, 5)			
			msg.AddMsgEvent(MsgID, 1, 206434)		-- �׸�ĸ�׵Ľ�������
			msg.AddMsgEvent(MsgID, 21, 18)
			msg.AddMsgEvent(MsgID, 1, 206471)   --"�һ��ƺ�<��ĸ��ף��>"
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100002)   --"ȡ��"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 4 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206435)		--������֮ĸ��\n     �����ü��εļ���ս���������Ǵ�ÿ�������У�ֻ�����ز��ݵĻ����Ѻۣ����������Ҫ�����ľ����ſ��Խ��Ѻ��޲��ɽᾧ�������ӵ���������Ǿ����������������߽����޲���\n     ����Ҫ�죬�����ҵĻ��������ʱ������Ŷ��������߽⣬���ǻ�����ɢ���ҿ��¾��޷������ˡ�
			msg.AddMsgEvent(MsgID, 24, TargetID)	
			msg.AddMsgEvent(MsgID, 21, 7)			
			msg.AddMsgEvent(MsgID, 1, 100001)		-- ȷ��
			msg.AddMsgEvent(MsgID, 21, -1)			
			msg.AddMsgEvent(MsgID, 1, 100002)		-- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 7 then
			if jingtie_num < 50 then
				if role.GetBagFreeSize(RoleID) < 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 100003) -- "��Ҫ����&lt;p1&gt;�񱳰��ռ�ſɼ���������"
					msg.AddMsgEvent(MsgID, 9, 1)
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206436)		--������֮ĸ��\n     ��ѡ��Ҫʹ�ú��ֲ��Ͻ����޲���(3�������ɶһ���ɫ����ᾧ��2�������ɶһ���ɫ����ᾧ��ÿ�նԶ���Զһ�50����)��
					msg.AddMsgEvent(MsgID, 24, TargetID)	
					msg.AddMsgEvent(MsgID, 21, 8)			
					msg.AddMsgEvent(MsgID, 1, 206437)		-- 3������
					msg.AddMsgEvent(MsgID, 21, 9)			
					msg.AddMsgEvent(MsgID, 1, 206438)		-- 2������
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			elseif jingtie_num >= 50 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206482)		--�������Ѿ��һ�50���ˣ������������һ����㻹����ͨ�����븱����û���ᾧ����
				msg.AddMsgEvent(MsgID, 24, TargetID)	
				msg.AddMsgEvent(MsgID, 21, -1)			
				msg.AddMsgEvent(MsgID, 1, 100001)		-- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 8 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206439)		--������֮ĸ��\n    ��ѡ������Ҫ�һ�����ɫ����ᾧ��(ÿ6��3�������ɶһ�1����ɫ����ᾧ)��
			msg.AddMsgEvent(MsgID, 24, TargetID)	
			msg.AddMsgEvent(MsgID, 21, 10)			
			msg.AddMsgEvent(MsgID, 1, 206440)		--  �һ�1����ɫ����ᾧ
			msg.AddMsgEvent(MsgID, 21, 11)			
			msg.AddMsgEvent(MsgID, 1, 206441)		--  �һ�3����ɫ����ᾧ
			msg.AddMsgEvent(MsgID, 21, 12)			
			msg.AddMsgEvent(MsgID, 1, 206442)		--  �һ�5����ɫ����ᾧ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 10 then
			if jingtie_num < 50 then
				if JT3_num < 6 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206443) --  3���������㣬�һ�ʧ�ܣ�
					msg.AddMsgEvent(MsgID, 21, -1)	-- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 100001)	-- "ȷ��"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					role.RemoveFromRole(MapID, InstanceID, RoleID, 3050011, 6, 101)--ɾ��3������
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800088, 1, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["muqinyaojing"], jingtie_num+1)
				end
			elseif jingtie_num >= 50 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206482)		--�������Ѿ��һ�50���ˣ������������һ����㻹����ͨ�����븱����û���ᾧ����
				msg.AddMsgEvent(MsgID, 24, TargetID)	
				msg.AddMsgEvent(MsgID, 21, -1)			
				msg.AddMsgEvent(MsgID, 1, 100001)		-- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 11 then
			if jingtie_num <= 47 then
				if JT3_num < 18 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206443) --  3���������㣬�һ�ʧ�ܣ�
					msg.AddMsgEvent(MsgID, 21, -1)	-- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 100001)	-- "ȷ��"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					role.RemoveFromRole(MapID, InstanceID, RoleID, 3050011, 18, 101)--ɾ��3������
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800088, 3, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["muqinyaojing"], jingtie_num+3)
				end
			elseif jingtie_num > 47 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206484)		--���Ѿ��һ���&lt;p1&gt;�������ֻ�ܶһ�50������ѡ�������Ķһ�����
				msg.AddMsgEvent(MsgID, 24, TargetID)	
				msg.AddMsgEvent(MsgID, 9, jingtie_num)
				msg.AddMsgEvent(MsgID, 21, -1)			
				msg.AddMsgEvent(MsgID, 1, 100001)		-- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 12 then
			if jingtie_num <= 45 then
				if JT3_num < 30 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206443) --  3���������㣬�һ�ʧ�ܣ�
					msg.AddMsgEvent(MsgID, 21, -1)	-- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 100001)	-- "ȷ��"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					role.RemoveFromRole(MapID, InstanceID, RoleID, 3050011, 30, 101)--ɾ��3������
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800088, 5, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["muqinyaojing"], jingtie_num+5)
				end
			elseif jingtie_num > 45 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206484)		--���Ѿ��һ���&lt;p1&gt;�������ֻ�ܶһ�50������ѡ�������Ķһ�����
				msg.AddMsgEvent(MsgID, 24, TargetID)	
				msg.AddMsgEvent(MsgID, 9, jingtie_num)
				msg.AddMsgEvent(MsgID, 21, -1)			
				msg.AddMsgEvent(MsgID, 1, 100001)		-- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 9 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206444)		--������֮ĸ��\n    ��ѡ������Ҫ�һ�����ɫ����ᾧ��(ÿ6��2�������ɶһ�1����ɫ����ᾧ)��
			msg.AddMsgEvent(MsgID, 24, TargetID)	
			msg.AddMsgEvent(MsgID, 21, 13)			
			msg.AddMsgEvent(MsgID, 1, 206445)		--  �һ�1����ɫ����ᾧ
			msg.AddMsgEvent(MsgID, 21, 14)			
			msg.AddMsgEvent(MsgID, 1, 206446)		--  �һ�3����ɫ����ᾧ
			msg.AddMsgEvent(MsgID, 21, 15)			
			msg.AddMsgEvent(MsgID, 1, 206447)		--  �һ�5����ɫ����ᾧ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 13 then
			if jingtie_num < 50 then
				if JT2_num < 6 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206448) --  2���������㣬�һ�ʧ�ܣ�
					msg.AddMsgEvent(MsgID, 21, -1)	-- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 100001)	-- "ȷ��"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					role.RemoveFromRole(MapID, InstanceID, RoleID, 3050010, 6, 101)--ɾ��2������
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800087, 1, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["muqinyaojing"], jingtie_num+1)
				end
			elseif jingtie_num >= 50 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206482)		--�������Ѿ��һ�50���ˣ������������һ����㻹����ͨ�����븱����û���ᾧ����
				msg.AddMsgEvent(MsgID, 24, TargetID)	
				msg.AddMsgEvent(MsgID, 21, -1)			
				msg.AddMsgEvent(MsgID, 1, 100001)		-- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 14 then
			if jingtie_num <= 47 then
				if JT2_num < 18 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206448) --  2���������㣬�һ�ʧ�ܣ�
					msg.AddMsgEvent(MsgID, 21, -1)	-- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 100001)	-- "ȷ��"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					role.RemoveFromRole(MapID, InstanceID, RoleID, 3050010, 18, 101)--ɾ��2������
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800087, 3, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["muqinyaojing"], jingtie_num+3)
				end
			elseif jingtie_num > 47 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206484)		--���Ѿ��һ���&lt;p1&gt;�������ֻ�ܶһ�50������ѡ�������Ķһ�����
				msg.AddMsgEvent(MsgID, 24, TargetID)	
				msg.AddMsgEvent(MsgID, 9, jingtie_num)
				msg.AddMsgEvent(MsgID, 21, -1)			
				msg.AddMsgEvent(MsgID, 1, 100001)		-- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 15 then
			if jingtie_num <= 45 then
				if JT2_num < 30 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206448) --  2���������㣬�һ�ʧ�ܣ�
					msg.AddMsgEvent(MsgID, 21, -1)	-- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 100001)	-- "ȷ��"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					role.RemoveFromRole(MapID, InstanceID, RoleID, 3050010, 30, 101)--ɾ��2������
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800087, 5, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["muqinyaojing"], jingtie_num+5)
				end
			elseif jingtie_num > 45 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206484)		--���Ѿ��һ���&lt;p1&gt;�������ֻ�ܶһ�50������ѡ�������Ķһ�����
				msg.AddMsgEvent(MsgID, 24, TargetID)	
				msg.AddMsgEvent(MsgID, 9, jingtie_num)
				msg.AddMsgEvent(MsgID, 21, -1)			
				msg.AddMsgEvent(MsgID, 1, 100001)		-- ȷ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 5 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206449)		--������֮ĸ��\n    1����ɫ���屾Դ���Զһ�3�Ű󶨵ı��׷���30���������޴����һ�\n    1����ɫ���屾Դ���Զһ�1�Ű󶨵ı��׷���30���������޴����һ���
			msg.AddMsgEvent(MsgID, 24, TargetID)	
			msg.AddMsgEvent(MsgID, 21, 16)			
			msg.AddMsgEvent(MsgID, 1, 206450)		--   �һ���ɫ���屾Դ
			msg.AddMsgEvent(MsgID, 21, 17)			
			msg.AddMsgEvent(MsgID, 1, 206451)		--   �һ���ɫ���屾Դ
			msg.AddMsgEvent(MsgID, 21, -1)	-- ѡ��һ
			msg.AddMsgEvent(MsgID, 1, 100002)	-- "ȡ��"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 16 then
			if HTB_num < 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206452) --  ��ȱ����ɫ���屾Դ���һ�ʧ�ܣ�
				msg.AddMsgEvent(MsgID, 21, -1)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 100001)	-- "ȷ��"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif role.GetBagFreeSize(RoleID) < 3 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 100003) -- "��Ҫ����&lt;p1&gt;�񱳰��ռ�ſɼ���������"
				msg.AddMsgEvent(MsgID, 9, 3)
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4800090, 1, 101)	
				role.AddRoleItem(MapID, InstanceID, RoleID, 3020057, 3, -1, 8, 420)--���׷�
			end
		elseif TalkIndex == 17 then
			if HTG_num < 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206453) --  ��ȱ����ɫ���屾Դ���һ�ʧ�ܣ�
				msg.AddMsgEvent(MsgID, 21, -1)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 100001)	-- "ȷ��"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif role.GetBagFreeSize(RoleID) < 3 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 100003) -- "��Ҫ����&lt;p1&gt;�񱳰��ռ�ſɼ���������"
				msg.AddMsgEvent(MsgID, 9, 3)
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4800089, 1, 101)	
				role.AddRoleItem(MapID, InstanceID, RoleID, 3020057, 1, -1, 8, 420)--���׷�
			end
		elseif TalkIndex == 18 then
			if num < 5 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206472) -- "����֮ĸ��\n    �����ϵĺ�ɫ����ܰ�ɲ���Ŷ������5�������ɡ�"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			elseif role.GetBagFreeSize(RoleID) < 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 100003) -- "��Ҫ����&lt;p1&gt;�񱳰��ռ�ſɼ���������"
				msg.AddMsgEvent(MsgID, 9, 1)
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4800086, 5, 101)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4800092, 1, -1, 8, 420)
			end
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206476) -- ���ȼ�����30�����޷��μӴ˻��
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	end
end
aux.RegisterCreatureEvent(5600055, 7, "yaojingzhimu_OnTalk")
