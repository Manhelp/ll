

function c3040056_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10045)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040056, 2, "c3040056_OnEnterCombat")

function c3040056_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10046)

	map.MapCreateCreature(MapID, InstanceID, 3040057, 272, 5968, 251)
	
	
	local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 102, 100050)    
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(3040056, 4, "c3040056_OnDie")