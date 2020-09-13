---mapscriptdata[1] ��¼���������Ƿ���ˢ������
--[2]��¼��һ�Ƿ���ɱ��
--[3]��¼�϶��Ƿ���ɱ��
--[4]��¼�����Ƿ���ɱ��
--[5]��¼�����Ƿ���ɱ��

function c3030021_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10050)
	
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
	map.OpenCloseDoor(MapID, InstanceID, 1119, nil)
	
end

aux.RegisterCreatureEvent(3030021, 2, "c3030021_OnEnterCombat")


function c3030021_OnLeaveCombat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 1119, 1)
end
aux.RegisterCreatureEvent(3030021, 3, "c3030021_OnLeaveCombat")





---����ˢ������
function c3030021_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	
	

	map.OpenCloseDoor(MapID, InstanceID, 1119, 1)

	map.MapCreateCreature(MapID, InstanceID, 3030022, 416, 702, 240)
	
	map.MapCreateCreature(MapID, InstanceID, 3030025, 416, 702, 252)
	map.MapCreateCreature(MapID, InstanceID, 3030024, 429, 702, 240)
	map.MapCreateCreature(MapID, InstanceID, 3030024, 416, 702, 228)
	map.MapCreateCreature(MapID, InstanceID, 3030024, 400, 702, 240)
end

aux.RegisterCreatureEvent(3030021, 4, "c3030021_OnDie")



