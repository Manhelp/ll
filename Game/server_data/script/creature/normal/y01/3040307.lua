----scriptdata[0]��ǽ������Ƿ�ˢ��
----scriptdata[1]���3040306�Ƿ����� 0δ���� 1 ����
----scriptdata[2]���3040307�Ƿ����� 0δ���� 1 ����
----scriptdata[3]���3040332�Ƿ���� 0������ 1 ����



function c3040307_OnEnterCombat(MapID, InstanceID, CreatureID)

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
	
	unit.AddBuff(MapID, InstanceID, CreatureID,5938301,CreatureID)
	
	map.OpenCloseDoor(MapID, InstanceID, 1110, nil)
end

aux.RegisterCreatureEvent(3040307, 2, "c3040307_OnEnterCombat")


function c3040307_OnLeaveCombat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 1110, 1)
end
aux.RegisterCreatureEvent(3040307, 3, "c3040307_OnLeaveCombat")


function c3040307_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	
	
	local yanshiisdead = map.GetMapScriptData(MapID,InstanceID,1,1)
	local timerisdead = map.GetMapScriptData(MapID,InstanceID,1,3)
	--��ü�ʱ��״̬
	map.SetMapScriptData(MapID,InstanceID,1,2,1)
	--���3040307����
	if timerisdead == 1 and yanshiisdead == 1 then

		
	
		map.OpenCloseDoor(MapID, InstanceID, 1110, 1)
		map.OpenCloseDoor(MapID, InstanceID, 1111, 1)

		map.MapCreateCreature(MapID, InstanceID, 3040317, 414,701,254)
		map.MapCreateCreature(MapID, InstanceID, 3040318, 413,701,231)
		map.MapCreateCreature(MapID, InstanceID, 3040319, 427,702,243)
		

		local MsgID = msg.BeginMsgEvent()
		--msg.AddMsgEvent(MsgID, 102, 100139)
		--��������ͬʱ������
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	else
		cre.MonsterSay(MapID, InstanceID, TargetID, 10203)
		
		local MsgID = msg.BeginMsgEvent()
		--msg.AddMsgEvent(MsgID, 102, 100137)
		--�ٶ�ɱ����һ������
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		map.MapCreateCreature(MapID, InstanceID, 3040332, 100, 100, 100)
		map.SetMapScriptData(MapID,InstanceID,1,3,1)
		--ˢ����ʱ�����Ѽ�ʱ��״̬λ��Ϊ1

	end

	

end

aux.RegisterCreatureEvent(3040307, 4, "c3040307_OnDie")
