----scriptdata[0]��ǽ������Ƿ�ˢ��
----scriptdata[1]���3040075
----scriptdata[2]���3040076
----scriptdata[3]���3040077
----scriptdata[4]���3040078
----scriptdata[5]���3040086


function c3040086_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10065)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040086, 2, "c3040086_OnEnterCombat")






function c3040086_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10040)
	

	map.SetMapScriptData(MapID, InstanceID, 1, 5, 1)
	
	
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100087)
	--3040086����
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	--ˢ��3040087
	
	map.MapCreateCreature(MapID, InstanceID, 3040087, 422, 11166, 380)
	
	


end

aux.RegisterCreatureEvent(3040086, 4, "c3040086_OnDie")
