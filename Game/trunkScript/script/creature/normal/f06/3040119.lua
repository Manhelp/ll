---[0]��¼�Ƿ�ˢ������
---[1]381, 15076, 235����ĵ���  517
---[2]393, 15076, 235����ĵ���  518
---[3]405, 15076, 235����ĵ���  519
---[4]381, 15076, 247����ĵ���  520

---[5]393, 15076, 247����ĵ���  521
---[6]405, 15076, 247����ĵ���  522
---[7]381, 15076, 259����ĵ���  523
---[8]393, 15076, 259����ĵ���  524
---[9]405, 15076, 259����ĵ���  525


--[10]��λ��¼1��9����ĵ���״̬��1��ʾ��buff 0��ʾ��buff


--[11]��¼��ȵ�ˢ�����������������60

--[12]��¼�Ƿ�������

--[13]��¼�Ƿ�ˢ����һ3040103
--[14]��¼��һ3040103�������
--[15]��¼�϶�3040108�������
--[16]��¼����3040109�������
--[17]��¼����3040116�������
--[18]��¼����3040119�������


function c3040119_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10081)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040119, 2, "c3040119_OnEnterCombat")






function c3040119_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10082)



	local bmonsterdead = map.GetMapScriptData(MapID,InstanceID,1,18)

	map.SetMapScriptData(MapID, InstanceID, 1, 18, 1)


	if bmonsterdead == 0 then

		map.MapCreateCreature(MapID, InstanceID, 6010033, 390, 15984, 99)

		map.MapCreateCreature(MapID, InstanceID, 5010237, 389, 16056, 118)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100098)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)


	end

	if role.GetTrialState(RoleID, 7011) == 1 then
		role.SetTrialCompleteNum(RoleID, 7011, role.GetTrialCompleteNum(RoleID,7011) + 1)
    end
	if role.GetTrialState(RoleID, 7011) == 1 and role.GetTrialCompleteNum(RoleID,7011) >= Trail_maxnum[7011] then
		role.TrialComplete(RoleID,7011)
	end

	local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
    for u,v in pairs(RoleTbl) do
		if role.GetBagFreeSize(v) > 0 and math.random(100) > 70 then
		    role.AddRoleItem(MapID, InstanceID, v, 4820022, 1, 1, 8, 420)
		end
	end

end

aux.RegisterCreatureEvent(3040119, 4, "c3040119_OnDie")
