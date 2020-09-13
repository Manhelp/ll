

function s5039003_Cast(MapID, InstanceID, SkillID, OwnerID)
	if MapID==1266371150 then
		cre.MonsterSay(MapID, InstanceID, OwnerID, 10043)
		local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
		creatureID=math.random(1,3)
		if creatureID==1 then
			map.MapCreateCreature(MapID, InstanceID, 3040049, x+5, y, z+5)
		elseif creatureID==2 then
			map.MapCreateCreature(MapID, InstanceID, 3040050, x-5, y, z+5)
		elseif creatureID==3 then
			map.MapCreateCreature(MapID, InstanceID, 3040051, x-5, y, z-5)
		end
		local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    		--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   		 g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 40, 20)
	end
end
aux.RegisterSkillEvent(5039003, 1, "s5039003_Cast")



function c3040048_OnEnterCombat(MapID, InstanceID, CreatureID)

	
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040048, 2, "c3040048_OnEnterCombat")

function c3040048_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10044)

	map.MapCreateCreature(MapID, InstanceID, 3040056, 88, 5845, 130)
	
	
	local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 102, 100050)    
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(3040048, 4, "c3040048_OnDie")