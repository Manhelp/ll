
function s5937801_Cast(MapID, InstanceID, SkillID, OwnerID)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)

	map.MapCreateCreature(MapID, InstanceID, 3040337, x+2, y, z+2)
	map.MapCreateCreature(MapID, InstanceID, 3040337, x+2, y, z+2)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 20, 20)

end
aux.RegisterSkillEvent(5937801, 1, "s5937801_Cast")


