
function s5054504_Cast(MapID, InstanceID, SkillID, OwnerID)
	
	local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	
	map.MapCreateCreature(MapID, InstanceID, 1081029, x+2, y, z+2)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 20, 20)
	
end
aux.RegisterSkillEvent(5054504, 1, "s5054504_Cast")


