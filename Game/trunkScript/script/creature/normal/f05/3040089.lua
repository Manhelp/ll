----scriptdata[0]��ǽ������Ƿ�ˢ��
----scriptdata[1]���3040075
----scriptdata[2]���3040076
----scriptdata[3]���3040077
----scriptdata[4]���3040078
----scriptdata[5]���3040086
----scriptdata[6]���3040087
----scriptdata[7]���3040088
----scriptdata[8]���3040089
----scriptdata[9] record 3040088 ID
----scriptdata[10] record 3040089 ID
----scriptdata[11] �����Ƿ�ˢ��


function c3040089_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10069)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040089, 2, "c3040089_OnEnterCombat")






function c3040089_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)




	map.SetMapScriptData(MapID, InstanceID, 1, 8, 1)

	if role.GetTrialState(RoleID, 6013) == 1 then
		role.SetTrialCompleteNum(RoleID, 6013, role.GetTrialCompleteNum(RoleID,6013) + 1)
    end
	if role.GetTrialState(RoleID, 6013) == 1 and role.GetTrialCompleteNum(RoleID,6013) >= Trail_maxnum[6013] then
		role.TrialComplete(RoleID,6013)
	end

	local boss_03 = map.GetMapScriptData(MapID,InstanceID,1,9)

	local a = map.GetMapScriptData(MapID,InstanceID,1,7)
	if a==0 then

		cre.MonsterSay(MapID, InstanceID, boss_03, 10070)
		cre.MonsterSay(MapID, InstanceID, TargetID, 10074)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100091)
		--3040089������3040088��ɥ
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		unit.AddBuff(MapID,InstanceID,boss_03,5057802,boss_03)
	else
		cre.MonsterSay(MapID, InstanceID, boss_03, 10075)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100089)
		--�����徻��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		local bchest=map.GetMapScriptData(MapID,InstanceID,1,11)
		if bchest == 0 then
			map.MapCreateCreature(MapID, InstanceID, 6010032, 225, 10325, 816)
			map.MapCreateCreature(MapID, InstanceID, 6010037, 204, 10325, 797)

			map.MapCreateCreature(MapID, InstanceID, 5010236, 215, 10325, 806)

			map.SetMapScriptData(MapID, InstanceID, 1, 11, 1)
		end

	end

    local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
    for u,v in pairs(RoleTbl) do
		if role.GetBagFreeSize(v) > 0 and math.random(100) > 70 then
		    role.AddRoleItem(MapID, InstanceID, v, 4820022, 1, 1, 8, 420)
		end
	end
end

aux.RegisterCreatureEvent(3040089, 4, "c3040089_OnDie")
