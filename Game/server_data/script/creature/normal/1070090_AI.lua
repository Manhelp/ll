
function s5000101_Cast(MapID, InstanceID, SkillID, OwnerID)
	
	local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	map.MapCreateCreature(MapID, InstanceID, 1070091, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1070091, x+2, y, z+2)
	map.MapCreateCreature(MapID, InstanceID, 1070091, x-2, y, z+2)
	map.MapCreateCreature(MapID, InstanceID, 1070091, x+2, y, z-2)
	map.MapCreateCreature(MapID, InstanceID, 1070091, x-2, y, z-2)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    	--ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���з�����
   	g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 5, 20)
	
end
aux.RegisterSkillEvent(5000101, 1, "s5000101_Cast")


