---mapscriptdata[0] ��¼���������Ƿ���ˢ������
---mapscriptdata[1] ��¼��һ��boss��id

---mapscriptdata[2] ��¼�ڼ�������
---mapscriptdata[3] ��¼boss1�Ƿ�ɱ��

function c3040219_OnEnterCombat(MapID, InstanceID, CreatureID)

	--cre.MonsterSay(MapID, InstanceID, CreatureID, 10027)
	--����������������Щ�����ܽ����
	

end

aux.RegisterCreatureEvent(3040219, 2, "c3040219_OnEnterCombat")

function c3040219_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	--cre.MonsterSay(MapID, InstanceID, TargetID, 10029)
	---�����������ˡ�
	
	local x,y,z = unit.GetPosition(MapID, InstanceID,TargetID)
	
	map.MapCreateCreature(MapID, InstanceID, 3040222, x, y, z)


end
aux.RegisterCreatureEvent(3040219, 4, "c3040219_OnDie")
