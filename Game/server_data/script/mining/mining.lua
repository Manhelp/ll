
--����Ƿ���ʹ��
function Imining_CanUse(MapID, InstanceID, TypeID, TargetID)
	local FreeSize = role.GetBagFreeSize(TargetID)
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, TargetID)
	if FreeSize < 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003) --��Ҫ����&lt;p1&gt;�񱳰��ռ�ſɼ�������
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		return 43, false
	end
	if rolelevel < 20 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 301003) --���㼶��20������ȥ���ɿ���ˡ�
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		return 32, false
	end
	if MapID == 3934900289 then
		local bCanUse1 = map.IsInArea(MapID, InstanceID, TargetID, 160)
		local bCanUse2 = map.IsInArea(MapID, InstanceID, TargetID, 161)
		local bCanUse3 = map.IsInArea(MapID, InstanceID, TargetID, 163)
		if bCanUse1 == true or bCanUse2 == true or bCanUse3 == true then
			return 0, false
		end
	end
	return 43, false
end


--�Ͻ���
function I3050033_OnUse(MapID, InstanceID, TypeID, TargetID)
	local a = math.random(10000)
	if a >=1 and a <=9780 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050001, 2, -1, 8, 420)	--97.2
	elseif a >= 9781 and a <=9790 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030102, 1, -1, 8, 420)	--0.1
	elseif a >= 9791 and a <=9800 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030103, 1, -1, 8, 420)	--0.1
	elseif a >= 9801 and a <= 10000 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020001, 1, -1, 8, 420)	--2
	end
	role.FishingSendClient(MapID, InstanceID, TargetID) --���߿ͻ�������ʹ��(ԭ����)
end

--�������
function I3050032_OnUse(MapID, InstanceID, TypeID, TargetID)
	local a = math.random(10000)
	if a >=1 and a <=8000 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050001, 2, -1, 8, 420)	--80
	elseif a >= 8001 and a <=8002 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030102, 1, -1, 8, 420)	--0.02
	elseif a >= 8003 and a <=8004 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030103, 1, -1, 8, 420)	--0.02
	elseif a >=8005 and a <=8205 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020001, 1, -1, 8, 420)	--2
	elseif a >=8206 and a <=10000 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050002, 1, -1, 8, 420)	--17.94
	end
	role.FishingSendClient(MapID, InstanceID, TargetID) --���߿ͻ�������ʹ��(ԭ����)
end



--��ͨ���
function I3050031_OnUse(MapID, InstanceID, TypeID, TargetID)
	local a = math.random(10000)
	if a >=1 and a <=8000 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050001, 1, -1, 8, 420)	--80
	elseif a >= 8001 and a <=8002 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030102, 1, -1, 8, 420)	--0.02
	elseif a >= 8003 and a <=8004 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030103, 1, -1, 8, 420)	--0.02
	elseif  a >=8005 and a <=8205 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020001, 1, -1, 8, 420)	--2
	elseif a >=8206 and a <=10000 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050002, 1, -1, 8, 420)	--17.94
	end
	role.FishingSendClient(MapID, InstanceID, TargetID) --���߿ͻ�������ʹ��(ԭ����)
end

--���ÿ��
function I3050030_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050001, 1, -1, 8, 420)
	role.FishingSendClient(MapID, InstanceID, TargetID) --���߿ͻ�������ʹ��(ԭ����)
end

aux.RegisterItemEvent(3050030, 0, "Imining_CanUse")
aux.RegisterItemEvent(3050031, 0, "Imining_CanUse")
aux.RegisterItemEvent(3050032, 0, "Imining_CanUse")
aux.RegisterItemEvent(3050033, 0, "Imining_CanUse")
aux.RegisterItemEvent(3050030, 1, "I3050030_OnUse")
aux.RegisterItemEvent(3050031, 1, "I3050031_OnUse")
aux.RegisterItemEvent(3050032, 1, "I3050032_OnUse")
aux.RegisterItemEvent(3050033, 1, "I3050033_OnUse")

--[[��ȡ��ҵ��ε��㾭��
function RGetRoleminingExp(MapID, InstanceID, RoleID)
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)

	--��������Ϊ �ȼ�*50
	local a = role.GetRoleScriptData(RoleID, 1, RoleDataType["miningLevel"])
	local b = role.GetRoleScriptData(RoleID, 1, RoleDataType["miningNum"])

	if a == 0 then
		a = 1
	end

	local i = a * 0.002
	local curhour = os.date("%H")

	if curhour == "18" or curhour == "19" or curhour == "20" or curhour == "21" then
		return mining_exp[rolelevel]* 0.8 *(i+1)
	else
		return mining_exp[rolelevel]*(i+1)
	end
end]]


--aux.RegisterRoleEvent(21, "RGetRoleminingExp")
