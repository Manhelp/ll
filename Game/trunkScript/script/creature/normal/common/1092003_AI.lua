--1092003	�����������

function s5103301_Cast(MapID, InstanceID, SkillID, OwnerID)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	map.MapCreateCreature(MapID, InstanceID, 1060037, x+2, y, z+2)
	map.MapCreateCreature(MapID, InstanceID, 1060037, x-2, y, z+2)
	map.MapCreateCreature(MapID, InstanceID, 1060038, x+2, y, z-2)
	map.MapCreateCreature(MapID, InstanceID, 1060038, x-2, y, z-2)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    	--ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���з�����
   	g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 5, 20)
	cre.MonsterSay(MapID, InstanceID, OwnerID, 20252)
end
aux.RegisterSkillEvent(5103301, 1, "s5103301_Cast")


