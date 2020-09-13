---mapscriptdata[1] ��¼���������Ƿ���ˢ������
----[2]��¼3040008�Ƿ�ɱ��
----[3]��¼3040009�Ƿ�ɱ��
----[4]��¼�Ƿ��й���Χ��
----[5]��¼�Ƿ��������
----[6]��¼3040010�Ƿ�ɱ��


function c3040009_OnEnterCombat(MapID, InstanceID, CreatureID)
	local maifu=map.GetMapScriptData(MapID, InstanceID, 1,5)
 	if maifu==0 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10009)
		map.MapCreateCreature(MapID, InstanceID, 3040003, 821, 4295, 330)
		map.MapCreateCreature(MapID, InstanceID, 3040006, 824, 4295, 322)
		map.MapCreateCreature(MapID, InstanceID, 3040003, 827, 4295, 314)

		map.SetMapScriptData(MapID,InstanceID,1,5,1)
	end	

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040009, 2, "c3040009_OnEnterCombat")



function c3040009_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local a = map.GetMapScriptData(MapID, InstanceID, 1, 2)
	map.SetMapScriptData(MapID, InstanceID, 1, 3, 1)
	if a==1 then
		--------3040008����
		cre.MonsterSay(MapID, InstanceID, TargetID, 10011)
		map.MapCreateCreature(MapID, InstanceID, 3040010, 577, 4295, 147)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100041)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100040)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
		--------3040008δ��
		cre.MonsterSay(MapID, InstanceID, TargetID, 10010)
	end

end

aux.RegisterCreatureEvent(3040009, 4, "c3040009_OnDie")



