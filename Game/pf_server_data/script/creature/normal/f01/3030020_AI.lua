---mapscriptdata[1] ��¼���������Ƿ���ˢ������
--[2]��¼��һ�Ƿ���ɱ��
--[3]��¼�϶��Ƿ���ɱ��
--[4]��¼�����Ƿ���ɱ��
--[5]��¼�����Ƿ���ɱ��
--[6]��¼�Ƿ�ˢ������


function c3030020_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10052)
	
	
	--local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	-- g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 5, 20)
	
	
end

aux.RegisterCreatureEvent(3030020, 2, "c3030020_OnEnterCombat")






---����ˢ���϶�
function c3030020_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	
	
	map.MapCreateCreature(MapID, InstanceID, 3030021, 287, 3289, 414)
	
	map.MapCreateCreature(MapID, InstanceID, 3030026, 287, 3289, 384)
	map.MapCreateCreature(MapID, InstanceID, 3030026, 260, 3289, 428)
	


end

aux.RegisterCreatureEvent(3030020, 4, "c3030020_OnDie")



