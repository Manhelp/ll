--1092009	ڤ����������

function s5109301_Cast(MapID, InstanceID, SkillID, OwnerID)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	map.MapCreateCreature(MapID, InstanceID, 1070052, x+2, y, z+2)
	map.MapCreateCreature(MapID, InstanceID, 1070052, x-2, y, z+2)
	map.MapCreateCreature(MapID, InstanceID, 1070051, x+2, y, z-2)
	map.MapCreateCreature(MapID, InstanceID, 1070051, x-2, y, z-2)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    	--ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���з�����
   	g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 5, 20)
	cre.MonsterSay(MapID, InstanceID, OwnerID, 20253)
end
aux.RegisterSkillEvent(5109301, 1, "s5109301_Cast")


