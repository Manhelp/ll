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




function c3040143_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10095)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040143, 2, "c3040143_OnEnterCombat")


function c3040143_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10096)
	

	local creisdead = map.GetMapScriptData(MapID,InstanceID,1,2)
	map.SetMapScriptData(MapID,InstanceID,1,3,1)

	if creisdead == 1 then
	
		map.OpenCloseDoor(MapID, InstanceID, 2213, 1)
		
		map.MapCreateCreature(MapID, InstanceID, 3040150, 258, 3840, 1503)
		map.MapCreateCreature(MapID, InstanceID, 3040151, 908, 3840, 1372)


		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100114)
		--�Ŵ�
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100113)
		--��δ��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	end

	

end

aux.RegisterCreatureEvent(3040143, 4, "c3040143_OnDie")
