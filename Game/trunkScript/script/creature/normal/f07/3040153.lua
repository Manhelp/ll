----scriptdata[0]��ǽ������Ƿ�ˢ��
----scriptdata[1]����ٻ������ڼ�����
----scriptdata[2]���3040141�Ƿ�����
----scriptdata[3]���3040143�Ƿ�����
----scriptdata[4]���3040150�Ƿ�����
----scriptdata[5]���3040151�Ƿ�����
----scriptdata[6]��¼3040152��ID
----scriptdata[7]���3040164�Ƿ�����
----scriptdata[8]���3040165�Ƿ�����
----scriptdata[9]���3040166�Ƿ�����
----scriptdata[10]����Ƿ�ˢ������


function c3040153_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10106)

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040153, 2, "c3040153_OnEnterCombat")

function c3040153_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10107)

	local baoxiang = map.GetMapScriptData(MapID, InstanceID, 1,10)
	if baoxiang ==0 then


		map.MapCreateCreature(MapID, InstanceID, 6010034, 476, 5664, 1790)
		--map.MapCreateCreature(MapID, InstanceID, 6010039, 501, 5664, 1790)

		map.SetMapScriptData(MapID, InstanceID, 1,10,1)
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100117)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)


	if role.GetTrialState(RoleID, 8023) == 1 then
		role.SetTrialCompleteNum(RoleID, 8023, role.GetTrialCompleteNum(RoleID,8023) + 1)
    	end
	if role.GetTrialState(RoleID, 8023) == 1 and role.GetTrialCompleteNum(RoleID,8023) >= Trail_maxnum[8023] then
		role.TrialComplete(RoleID,8023)
	end

	local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
    for u,v in pairs(RoleTbl) do
		if role.GetBagFreeSize(v) > 0 and math.random(100) > 70 then
		    role.AddRoleItem(MapID, InstanceID, v, 4820022, 1, 1, 8, 420)
		end
	end
end

aux.RegisterCreatureEvent(3040153, 4, "c3040153_OnDie")


