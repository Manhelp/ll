---mapscriptdata[0] ��¼���������Ƿ���ˢ������
---mapscriptdata[1] ��¼��һ��boss��id

---mapscriptdata[2] ��¼�ڼ�������
---mapscriptdata[3] ��¼boss1�Ƿ�ɱ��

function c3040232_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10027)
	--ʲô�˴��������������ȹ�����һ��

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

end

aux.RegisterCreatureEvent(3040232, 2, "c3040232_OnEnterCombat")

function c3040232_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10029)
	local temp = map.GetMapScriptData(MapID,InstanceID,1,12)
	if(temp == 1) then
		local tianjieBOss = map.MapCreateCreature(MapID, InstanceID, 3040250, 349, 1151, 855)
		local mingjieBOss = map.MapCreateCreature(MapID, InstanceID, 3040251, 330, 1151, 855)
		map.SetMapScriptData(MapID,InstanceID,1,23,tianjieBOss)
		map.SetMapScriptData(MapID,InstanceID,1,24,mingjieBOss)
	end
	map.OpenCloseDoor(MapID, InstanceID, 879, 1)
	map.SetMapScriptData(MapID,InstanceID,1,11,1)


end
aux.RegisterCreatureEvent(3040232, 4, "c3040232_OnDie")
