----scriptdata[1]��¼3040018�Ƿ�����
----scriptdata[2]��¼3040019�Ƿ�����
----scriptdata[3]��¼�Ƿ�ˢ��4������
----scriptdata[4]��¼3040020�Ƿ���������������
----scriptdata[5]��¼3040021�Ƿ���������������
----scriptdata[6]��¼3040024�Ƿ�����
----scriptdata[7]��¼3040025�Ƿ�����
----scriptdata[8]��Ǳ���
----scriptdata[9]��ǽ������Ƿ�ˢ��




function c3040012_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10020)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040012, 2, "c3040012_OnEnterCombat")






function c3040013_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10021)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040013, 2, "c3040013_OnEnterCombat")




function c3040014_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10022)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040014, 2, "c3040014_OnEnterCombat")




function werewolf_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	
	cre.MonsterSay(MapID, InstanceID, TargetID, 10023)
	

end

aux.RegisterCreatureEvent(3040012, 4, "werewolf_OnDie")
aux.RegisterCreatureEvent(3040013, 4, "werewolf_OnDie")
aux.RegisterCreatureEvent(3040014, 4, "werewolf_OnDie")

		



