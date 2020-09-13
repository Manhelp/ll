function n6040102_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curhour = tonumber(os.date("%H"))
	local curmin = tonumber(os.date("%M"))

	local time1 = curmon*10000 + curday *100--����ʱ
	if time1 ~=role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_time"]) then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_baoming"], 0)--����0û���б���,1��ͨ��2��רҵ��
		role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_time"], time1)--
	end
	local x = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_baoming"])

	local money = role.GetRoleSilver(MapID, InstanceID,RoleID)

	local lev = role.GetRoleLevel(MapID, InstanceID,RoleID)

	local juan = role.GetRoleItemNum(RoleID, 4810035)

	if TalkIndex == -1 then
		if curhour == 20 or (curhour == 21 and curmin < 20)then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211150) -- ��Ӿ��Ŀ�����ˣ�\n    ѡ���������������,��ͨ�鱨����Ҫ20��,רҵ�鱨����Ҫ50���һ��רҵ������,�����ֻ�ܲμ�һ�����ÿ��20:00������Ӿ������
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
			msg.AddMsgEvent(MsgID, 20, 211151) -- ��Ӿ��Ŀ�����ˣ�\n    ѡ���������������,��ͨ�鱨����Ҫ20��,רҵ�鱨����Ҫ50���һ��רҵ������,�����ֻ�ܲμ�һ�����ÿ��20:00������Ӿ������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID) 
		end
		 
	elseif TalkIndex == 4 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 211152) -- ��Ӿ��Ŀ�����ˣ�\n    ��ȷ��Ҫ������ͨ������𣿲μ���ͨ��֮��Ͳ��ܲμ�רҵ������ˡ�
		msg.AddMsgEvent(MsgID, 24, TargetID) 
		msg.AddMsgEvent(MsgID, 21, 6) 
		msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
		msg.AddMsgEvent(MsgID, 21, 99)
		msg.AddMsgEvent(MsgID, 1, 100002) -- ȡ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 211153) -- ��Ӿ��Ŀ�����ˣ�\n    ��ȷ��Ҫ����רҵ������𣿲μ���ͨ��֮��Ͳ��ܲμ���ͨ������ˡ�
		msg.AddMsgEvent(MsgID, 24, TargetID) --
		msg.AddMsgEvent(MsgID, 21, 7) 
		msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
		msg.AddMsgEvent(MsgID, 21, 99)
		msg.AddMsgEvent(MsgID, 1, 100002) -- ȡ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 6 then
		if x == 1 then--ҵ����
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211154) -- ��Ӿ��Ŀ�����ˣ�\n    ���Ѿ�������ͨ�����,�����ٱ����ˡ�
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif x == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211155) -- ��Ӿ��Ŀ�����ˣ�\n    רҵ�����ͨ�������ֻ�ܲμ�һ��
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif lev < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211156) -- ��Ӿ��Ŀ�����ˣ�\n    ��ĵȼ�����50��,���ܱ���������
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif money < 200000 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211157) -- ��Ӿ��Ŀ�����ˣ�\n    ��������Ҫ���ɱ�����,�����Ϻ���û��ô��Ǯ��
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)	
		else
			role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_baoming"], 1)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_got"], 0)
			role.DecRoleSilver(MapID, InstanceID,RoleID,200000, 400)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211158) -- ��Ӿ��Ŀ�����ˣ�\n    ��Ӿ������ͨ��������Ѿ������ɹ�,<A>21:20</A>��ʽ����.��ȥ׼�����ȴ���Ӿ<A>������</A>�ɡ�\n    ��Ӿ�����л�������ֲ�ͬ����ĺ��������кܶ����㣬ҪС������Ӱ������ٶȡ�
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 7 then
		if x == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211155) -- ��Ӿ��Ŀ�����ˣ�\n    רҵ�����ͨ�������ֻ�ܲμ�һ��
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif x == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211159) -- ��Ӿ��Ŀ�����ˣ�\n    ���Ѿ�����רҵ�����,�����ٱ����ˡ�
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif lev < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211156) -- ��Ӿ��Ŀ�����ˣ�\n    ��ĵȼ�����50��,���ܱ���������
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif money < 500000 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211157) -- ��Ӿ��Ŀ�����ˣ�\n    ��������Ҫ���ɱ�����,�����Ϻ���û��ô��Ǯ��
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)	
		elseif juan < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211160) -- ��Ӿ��Ŀ�����ˣ�\n    ����רҵ�����Ҫ����רҵ������
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)	
		else
			
			
				role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_baoming"], 2)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_got"], 0)
				role.RemoveFromRole(MapID, InstanceID,RoleID,4810035, 1, 400)	--ɾ��һ��������
				role.DecRoleSilver(MapID, InstanceID,RoleID,500000, 420)

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211161) -- ��Ӿ��Ŀ�����ˣ�\n    ��Ӿ����רҵ��������Ѿ������ɹ�,<A>21:20</A>��ʽ����.��ȥ׼�����ȴ���Ӿ<A>������</A>�ɡ�\n    ��Ӿ�����л�������ֲ�ͬ����ĺ��������кܶ����㣬ҪС������Ӱ������ٶȡ�
				msg.AddMsgEvent(MsgID, 24, TargetID) 
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			
			
		end
	elseif TalkIndex == 8 then

		role.RoleGotoNewMap(MapID, InstanceID, RoleID, MapID, 716, 2594, 389)--

	end
end
aux.RegisterCreatureEvent(6040102, 7, "n6040102_OnTalk")

function n6040133_On_Disappear(MapID, InstanceID, TypeID, AI)

	map.MapCreateCreature(MapID, InstanceID, 6040146, 733,2400,530 )

end

aux.RegisterCreatureEvent(6040133,13,"n6040133_On_Disappear")


function n6040146_On_Disappear(MapID, InstanceID, TypeID, AI)

	local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curhour = tonumber(os.date("%H"))
	local curmin = tonumber(os.date("%M"))

	local curtime = curhour*100 + curmin

	if curtime >= 2120 and curtime <= 2140 then
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
end
aux.RegisterCreatureEvent(6040146,13,"n6040146_On_Disappear")
