

function c3040044_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10039)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040044, 2, "c3040044_OnEnterCombat")

function c3040044_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10040)

	map.MapCreateCreature(MapID, InstanceID, 3040045, 457, 5845, 423)
	
	
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100069)    
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(3040044, 4, "c3040044_OnDie")