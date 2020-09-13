----scriptdata[0]��ǽ������Ƿ�ˢ��
----scriptdata[1]���3040306�Ƿ����� 0δ���� 1 ����
----scriptdata[2]���3040307�Ƿ����� 0δ���� 1 ����
----scriptdata[3]���3040332�Ƿ���� 0������ 1 ����


function c3040306_OnEnterCombat(MapID, InstanceID, CreatureID)
	
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100134)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10202)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40������,�߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

	unit.AddBuff(MapID, InstanceID, CreatureID,5938201,CreatureID)

	map.OpenCloseDoor(MapID, InstanceID, 1110, nil)
end

aux.RegisterCreatureEvent(3040306, 2, "c3040306_OnEnterCombat")


---����ս������

function c3040306_OnLeaveCombat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 1110, 1)
end
aux.RegisterCreatureEvent(3040306, 3, "c3040306_OnLeaveCombat")




function c3040306_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	
	

	local timerisdead = map.GetMapScriptData(MapID,InstanceID,1,3)
	local yanmoisdead = map.GetMapScriptData(MapID,InstanceID,1,2)
	--��ü�ʱ��״̬
	map.SetMapScriptData(MapID,InstanceID,1,1,1)
	--���3040306����
	if timerisdead == 1 and yanmoisdead == 1 then

		
	
		map.OpenCloseDoor(MapID, InstanceID, 1110, 1)
		map.OpenCloseDoor(MapID, InstanceID, 1111, 1)
		

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100139)
		--��������ͬʱ������
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		map.MapCreateCreature(MapID, InstanceID, 3040317, 414,701,254)
		map.MapCreateCreature(MapID, InstanceID, 3040318, 413,701,231)
		map.MapCreateCreature(MapID, InstanceID, 3040319, 427,702,243)
		

	else

		cre.MonsterSay(MapID, InstanceID, TargetID, 10203)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100136)
		--�ٶ�ɱ����һ������
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		map.MapCreateCreature(MapID, InstanceID, 3040332, 100, 100, 100)
		map.SetMapScriptData(MapID,InstanceID,1,3,1)
		--ˢ����ʱ��,�Ѽ�ʱ��״̬λ��Ϊ0

	end

	

end

aux.RegisterCreatureEvent(3040306, 4, "c3040306_OnDie")
