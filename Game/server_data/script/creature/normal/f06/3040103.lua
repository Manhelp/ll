---[0]��¼�Ƿ�ˢ������
---[1]381, 15076, 235����ĵ���  517
---[2]393, 15076, 235����ĵ���  518
---[3]405, 15076, 235����ĵ���  519
---[4]381, 15076, 247����ĵ���  520

---[5]393, 15076, 247����ĵ���  521
---[6]405, 15076, 247����ĵ���  522
---[7]381, 15076, 259����ĵ���  523
---[8]393, 15076, 259����ĵ���  524
---[9]405, 15076, 259����ĵ���  525


--[10]��λ��¼1��9����ĵ���״̬��1��ʾ��buff 0��ʾ��buff


--[11]��¼��ȵ�ˢ�����������������60

--[12]��¼�Ƿ�������

--[13]��¼�Ƿ�ˢ����һ3040103
--[14]��¼��һ3040103�������
--[15]��¼�϶�3040108�������
--[16]��¼����3040109�������
--[17]��¼����3040116�������
--[18]��¼����3040119�������


function c3040103_OnEnterCombat(MapID, InstanceID, CreatureID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100093)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10076)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040103, 2, "c3040103_OnEnterCombat")






function c3040103_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10064)
	

	map.SetMapScriptData(MapID, InstanceID, 1, 14, 1)
	
	
	

	map.MapCreateCreature(MapID, InstanceID, 3040108, 209, 14883, 389)
	map.MapCreateCreature(MapID, InstanceID, 3040111, 209, 14883, 380)
	map.MapCreateCreature(MapID, InstanceID, 3040111, 209, 14883, 398)

	map.MapCreateCreature(MapID, InstanceID, 3040109, 209, 14883, 445)
	map.MapCreateCreature(MapID, InstanceID, 3040110, 209, 14883, 436)
	map.MapCreateCreature(MapID, InstanceID, 3040110, 209, 14883, 454)


end

aux.RegisterCreatureEvent(3040103, 4, "c3040103_OnDie")
