----scriptdata[0]��ǽ������Ƿ�ˢ��
----scriptdata[1]���3040306�Ƿ����� 0δ���� 1 ����
----scriptdata[2]���3040307�Ƿ����� 0δ���� 1 ����
----scriptdata[3]���3040332�Ƿ���� 0������ 1 ����

----scriptdata[4]���3040317�Ƿ����� 0δ���� 1 ����
----scriptdata[5]���3040318�Ƿ����� 0δ���� 1 ����
----scriptdata[6]���3040319�Ƿ����� 0δ���� 1 ����
----scriptdata[7]��¼3040329��id

--292��3289��408

function c3040326_OnEnterCombat(MapID, InstanceID, CreatureID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100141)
	--���boss
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10207)

	map.OpenCloseDoor(MapID, InstanceID, 1119, nil)

	
end

aux.RegisterCreatureEvent(3040326, 2, "c3040326_OnEnterCombat")


---����ս������

function c3040326_OnLeaveCombat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 1119, 1)
end

aux.RegisterCreatureEvent(3040326, 3, "c3040326_OnLeaveCombat")




function c3040326_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	
	
	map.MapCreateCreature(MapID, InstanceID, 3040328, 140,61,243)

	local bossid = map.MapCreateCreature(MapID, InstanceID, 3040329, 140,347,175)
	
	map.SetMapScriptData(MapID,InstanceID,1,7,bossid)
		
	map.OpenCloseDoor(MapID, InstanceID, 1119, 1)


end

aux.RegisterCreatureEvent(3040326, 4, "c3040326_OnDie")
