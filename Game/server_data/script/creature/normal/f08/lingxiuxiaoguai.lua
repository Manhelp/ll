---mapscriptdata[0] ��¼���������Ƿ���ˢ������
---mapscriptdata[1] ��¼��һ��boss��id

---mapscriptdata[2] ��¼�ڼ�������
---mapscriptdata[3] ��¼boss1�Ƿ�ɱ��




function lingxiuxiaoguai_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	--cre.MonsterSay(MapID, InstanceID, TargetID, 10029)
	---�ظ�������

		
	unit.AddBuff(MapID, InstanceID,RoleID,5934901,RoleID)
		
	

end
aux.RegisterCreatureEvent(3040246, 4, "lingxiuxiaoguai_OnDie")
