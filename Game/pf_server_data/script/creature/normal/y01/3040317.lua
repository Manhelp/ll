----scriptdata[0]��ǽ������Ƿ�ˢ��
----scriptdata[1]���3040306�Ƿ����� 0δ���� 1 ����
----scriptdata[2]���3040307�Ƿ����� 0δ���� 1 ����
----scriptdata[3]���3040332�Ƿ���� 0������ 1 ����

----scriptdata[4]���3040317�Ƿ����� 0δ���� 1 ����
----scriptdata[5]���3040318�Ƿ����� 0δ���� 1 ����
----scriptdata[6]���3040319�Ƿ����� 0δ���� 1 ����


--292��3289��408

function c3040317_OnEnterCombat(MapID, InstanceID, CreatureID)


	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100140)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10204)

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

	map.OpenCloseDoor(MapID, InstanceID, 1119, nil)

	
end

aux.RegisterCreatureEvent(3040317, 2, "c3040317_OnEnterCombat")


function c3040317_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	
	
	local bingxiisdead = map.GetMapScriptData(MapID,InstanceID,1,5)
	local heianxiisdead = map.GetMapScriptData(MapID,InstanceID,1,6)
	
	map.SetMapScriptData(MapID,InstanceID,1,4,1)
	--���3040317����
	if bingxiisdead == 1 and heianxiisdead == 1 then

		cre.MonsterSay(MapID, InstanceID, TargetID, 10094)
	
		map.MapCreateCreature(MapID, InstanceID, 3040326, 292,3289,408)
		
		map.OpenCloseDoor(MapID, InstanceID, 1119, 1)

		


	end
	

end

aux.RegisterCreatureEvent(3040317, 4, "c3040317_OnDie")
