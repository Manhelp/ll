----scriptdata[1]��¼3040018�Ƿ�����
----scriptdata[2]��¼3040019�Ƿ�����
----scriptdata[3]��¼�Ƿ�ˢ��4������
----scriptdata[4]��¼3040020�Ƿ���������������
----scriptdata[5]��¼3040021�Ƿ���������������
----scriptdata[6]��¼3040024�Ƿ�����
----scriptdata[7]��¼3040025�Ƿ�����
----scriptdata[8]��Ǳ���
----scriptdata[9]��ǽ������Ƿ�ˢ��



function c3040023_OnEnterCombat(MapID, InstanceID, CreatureID)

	
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040023, 2, "c3040023_OnEnterCombat")


function c3040023_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	-------------------------addmsgˢ��3040022
	cre.MonsterSay(MapID, InstanceID, TargetID, 10034)
	map.MapCreateCreature(MapID, InstanceID, 3040022, 1073, 1508, 245)
	map.MapCreateCreature(MapID, InstanceID, 3040026, 1059, 1425, 247)
	
		
	
end

aux.RegisterCreatureEvent(3040023, 4, "c3040023_OnDie")