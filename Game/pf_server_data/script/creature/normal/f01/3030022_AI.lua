---mapscriptdata[1] ��¼���������Ƿ���ˢ������
--[2]��¼��һ�Ƿ���ɱ��
--[3]��¼�϶��Ƿ���ɱ��
--[4]��¼�����Ƿ���ɱ��
--[5]��¼�����Ƿ���ɱ��

function c3030022_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10050)
	
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
	
	
end

aux.RegisterCreatureEvent(3030022, 2, "c3030022_OnEnterCombat")



---����ˢ������
function c3030022_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	
	
	
	
	map.MapCreateCreature(MapID, InstanceID, 3030023, 338, 50, 56)

	map.MapCreateCreature(MapID, InstanceID, 3030024, 349, 50, 68)
	map.MapCreateCreature(MapID, InstanceID, 3030024, 345, 50, 75)
	map.MapCreateCreature(MapID, InstanceID, 3030024, 340, 50, 84)	
	map.MapCreateCreature(MapID, InstanceID, 3030024, 335, 50, 95)

	map.MapCreateCreature(MapID, InstanceID, 3030025, 320, 50, 51)
	map.MapCreateCreature(MapID, InstanceID, 3030025, 316, 50, 59)
	map.MapCreateCreature(MapID, InstanceID, 3030025, 312, 50, 69)	
	map.MapCreateCreature(MapID, InstanceID, 3030025, 307, 50, 80)

end

aux.RegisterCreatureEvent(3030022, 4, "c3030022_OnDie")



