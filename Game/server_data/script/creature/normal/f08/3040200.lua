---mapscriptdata[0] ��¼���������Ƿ���ˢ������
---mapscriptdata[1] ��¼��һ��boss��id

---mapscriptdata[2] ��¼�ڼ�������
---mapscriptdata[3] ��¼boss1�Ƿ�ɱ��

function c3040200_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10110)
	--ʲô�˴��������������ȹ�����һ��

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

end

aux.RegisterCreatureEvent(3040200, 2, "c3040200_OnEnterCombat")

function c3040200_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	cre.MonsterSay(MapID, InstanceID, TargetID, 10111)
	
	
	map.OpenCloseDoor(MapID, InstanceID, 872, 1)

	--map.OpenCloseDoor(MapID, InstanceID, 879, 1)

	
	

end
aux.RegisterCreatureEvent(3040200, 4, "c3040200_OnDie")
