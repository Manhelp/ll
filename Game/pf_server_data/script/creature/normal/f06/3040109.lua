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

function c3040109_OnEnterCombat(MapID, InstanceID, CreatureID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100094)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10077)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040109, 2, "c3040109_OnEnterCombat")






function c3040109_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10078)

	

	map.SetMapScriptData(MapID, InstanceID, 1, 16, 1)
	
	local bset = map.GetMapScriptData(MapID, InstanceID, 1,15)
	if bset == 1 then

		map.MapCreateCreature(MapID, InstanceID, 3040116, 394, 15057, 279)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100095)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		
	end
	
	
end

aux.RegisterCreatureEvent(3040109, 4, "c3040109_OnDie")
