---mapscriptdata[0] ��¼���������Ƿ���ˢ������
---mapscriptdata[1] ��¼��һ��boss��id

---mapscriptdata[2] ��¼�ڼ�������
---mapscriptdata[3] ��¼boss1�Ƿ�ɱ��

function c3040247_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10118)
	--ʲô�˴��������������ȹ�����һ��

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

end



aux.RegisterCreatureEvent(3040247, 2, "c3040247_OnEnterCombat")


function s5936201_Cast(MapID, InstanceID, SkillID, OwnerID)
		
	cre.MonsterSay(MapID, InstanceID,OwnerID, 10119)
	
end
aux.RegisterSkillEvent(5936201, 1, "s5936201_Cast")





function c3040247_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	cre.MonsterSay(MapID, InstanceID, TargetID, 10120)
	
	map.MapCreateCreature(MapID, InstanceID, 3040249, 339, 1152, 528)
	map.OpenCloseDoor(MapID, InstanceID, 878, 1)
	
	--map.OpenCloseDoor(MapID, InstanceID, 877, 1)
	
	

end
aux.RegisterCreatureEvent(3040247, 4, "c3040247_OnDie")
