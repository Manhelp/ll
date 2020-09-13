----scriptdata[0]��ǽ������Ƿ�ˢ��
----scriptdata[1]���3040075
----scriptdata[2]���3040076
----scriptdata[3]���3040077
----scriptdata[4]���3040078


function c3040078_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10063)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040078, 2, "c3040078_OnEnterCombat")


function s5054503_Cast(MapID, InstanceID, SkillID, OwnerID)
	if MapID==1266371406 then

		--cre.MonsterSay(MapID, InstanceID, OwnerID, 10038)

		local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
		map.MapCreateCreature(MapID, InstanceID, 3040090, x+5, y, z+5)
		map.MapCreateCreature(MapID, InstanceID, 3040091, x-5, y, z+5)
		map.MapCreateCreature(MapID, InstanceID, 3040092, x+5, y, z-5)
		
		local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    		--ͬ����޸���ΧΪ30�����ӣ��߶�Ϊ20���з�����
   		g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 30, 20)
	end
end
aux.RegisterSkillEvent(5054503, 1, "s5054503_Cast")



function c3040078_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10064)
	

	map.SetMapScriptData(MapID, InstanceID, 1, 4, 1)
	
	local bossdeath_1=map.GetMapScriptData(MapID,InstanceID,1,1)
	local bossdeath_2=map.GetMapScriptData(MapID,InstanceID,1,2)
	local bossdeath_3=map.GetMapScriptData(MapID,InstanceID,1,3)
	
	if bossdeath_1 == 1 and bossdeath_2 == 1 and bossdeath_3 == 1 then
		map.OpenCloseDoor(MapID, InstanceID, 4588, 1)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100086)
		--�Ŵ�
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100085)
		--�Ŵ�
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	
	end
	
	


end

aux.RegisterCreatureEvent(3040078, 4, "c3040078_OnDie")
