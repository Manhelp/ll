----scriptdata[0]��ǽ������Ƿ�ˢ��
----scriptdata[1]���3040075
----scriptdata[2]���3040076
----scriptdata[3]���3040077
----scriptdata[4]���3040078
----scriptdata[5]���3040086
----scriptdata[6]���3040087
----scriptdata[7]���3040088
----scriptdata[8]���3040089
----scriptdata[9] record 3040088
----scriptdata[10] record 3040089


function c3040087_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10066)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040087, 2, "c3040087_OnEnterCombat")






function c3040087_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10067)
	

	map.SetMapScriptData(MapID, InstanceID, 1, 6, 1)
	
	
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100088)
	--3040086����
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	--ˢ��3040088 3040089
	
	local boss_04 = map.MapCreateCreature(MapID, InstanceID, 3040089, 225, 10325, 816)
	
	map.SetMapScriptData(MapID, InstanceID, 1, 10, boss_04)
	
	local boss_03 = map.MapCreateCreature(MapID, InstanceID, 3040088, 204, 10325, 797)
	
	map.SetMapScriptData(MapID, InstanceID, 1, 9, boss_03)


end

aux.RegisterCreatureEvent(3040087, 4, "c3040087_OnDie")
