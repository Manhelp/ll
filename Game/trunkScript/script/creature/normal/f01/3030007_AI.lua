---mapscriptdata[1] ��¼���������Ƿ���ˢ������
--[2]��¼��һ�Ƿ���ɱ��
--[3]��¼�϶��Ƿ���ɱ��
--[4]��¼�����Ƿ���ɱ��
--[5]��¼�����Ƿ���ɱ��

function c3030007_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10052)
	
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
	
	
end

aux.RegisterCreatureEvent(3030007, 2, "c3030007_OnEnterCombat")



---����ˢ������
function c3030007_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	
	cre.MonsterSay(MapID, InstanceID, TargetID, 10053)
	map.MapCreateCreature(MapID, InstanceID, 3030013, 287, 3289, 414)
	
	map.SetMapScriptData(MapID,InstanceID,1,3,1)
	map.OpenCloseDoor(MapID, InstanceID, 1119, 1)

end

aux.RegisterCreatureEvent(3030007, 4, "c3030007_OnDie")



