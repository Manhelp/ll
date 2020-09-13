

function c3040045_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10041)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040045, 2, "c3040045_OnEnterCombat")

function c3040045_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10042)

	map.MapCreateCreature(MapID, InstanceID, 3040048, 52, 5845, 457)
	
	
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100070)    
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(3040045, 4, "c3040045_OnDie")