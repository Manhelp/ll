----scriptdata[0]��ǽ������Ƿ�ˢ��
----scriptdata[1]����ٻ������ڼ�����
----scriptdata[2]���3040141�Ƿ�����
----scriptdata[3]���3040143�Ƿ�����
----scriptdata[4]���3040150�Ƿ�����
----scriptdata[5]���3040151�Ƿ�����
----scriptdata[6]��¼3040152��ID
----scriptdata[7]���3040164�Ƿ�����
----scriptdata[8]���3040165�Ƿ�����
----scriptdata[9]���3040166�Ƿ�����
----scriptdata[10]����Ƿ�ˢ������


function c3040150_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10100)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040150, 2, "c3040150_OnEnterCombat")






function c3040150_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10101)
	
	

	local creisdead = map.GetMapScriptData(MapID,InstanceID,1,5)
	map.SetMapScriptData(MapID,InstanceID,1,4,1)

	if creisdead == 1 then
	
		
		local creID = map.MapCreateCreature(MapID, InstanceID, 3040152, 500, 5670, 1360)
		map.SetMapScriptData(MapID,InstanceID,1,6,creID)


		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100115)
		--�������ط���ˢ��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	else
		local MsgID = msg.BeginMsgEvent()
		--msg.AddMsgEvent(MsgID, 102, 100086)
		--����һ��ûɱ
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	end

end

aux.RegisterCreatureEvent(3040150, 4, "c3040150_OnDie")
