----scriptdata[0]��ǽ������Ƿ�ˢ��
----scriptdata[1]���3040075
----scriptdata[2]���3040076
----scriptdata[3]���3040077
----scriptdata[4]���3040078
----scriptdata[5]���3040086
----scriptdata[6]���3040087
----scriptdata[7]���3040088
----scriptdata[8]���3040089
----scriptdata[9] record 3040088 ID
----scriptdata[10] record 3040089 ID


function c3040088_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10068)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040088, 2, "c3040088_OnEnterCombat")






function c3040088_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	
	

	map.SetMapScriptData(MapID, InstanceID, 1, 7, 1)
	
	local boss_04 = map.GetMapScriptData(MapID,InstanceID,1,10)

	local a = map.GetMapScriptData(MapID,InstanceID,1,8)
	if a==0 then

		cre.MonsterSay(MapID, InstanceID, TargetID, 10073)
		cre.MonsterSay(MapID, InstanceID, boss_04, 10072)
		--�ҵ�������������˭ɱ������
		
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100090)
		--3040088������3040089��ŭ
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		unit.AddBuff(MapID,InstanceID,boss_04,5057801,boss_04)
	else
		cre.MonsterSay(MapID, InstanceID, boss_04, 10075)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100089)
		--�����徻��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		local bchest=map.GetMapScriptData(MapID,InstanceID,1,11)
		if bchest == 0 then
			map.MapCreateCreature(MapID, InstanceID, 6010032, 225, 10325, 816)
			
			map.MapCreateCreature(MapID, InstanceID, 5010236, 215, 10325, 806)
			
			map.SetMapScriptData(MapID, InstanceID, 1, 11, 1)
		end

	end


end

aux.RegisterCreatureEvent(3040088, 4, "c3040088_OnDie")
