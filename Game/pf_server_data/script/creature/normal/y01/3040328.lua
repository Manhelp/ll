----scriptdata[0]��ǽ������Ƿ�ˢ��
----scriptdata[1]���3040306�Ƿ����� 0δ���� 1 ����
----scriptdata[2]���3040307�Ƿ����� 0δ���� 1 ����
----scriptdata[3]���3040332�Ƿ���� 0������ 1 ����

----scriptdata[4]���3040317�Ƿ����� 0δ���� 1 ����
----scriptdata[5]���3040318�Ƿ����� 0δ���� 1 ����
----scriptdata[6]���3040319�Ƿ����� 0δ���� 1 ����
----scriptdata[7]��¼3040329��id






function s5936601_Cast(MapID, InstanceID, SkillID, OwnerID)

	if MapID==3870157173 then
		
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100144)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)


		
		
		local bossid = map.GetMapScriptData(MapID,InstanceID,1,7)

	 	unit.CancelBuff(MapID, InstanceID, bossid, 5938001)
	end

end
aux.RegisterSkillEvent(5936601, 1, "s5936601_Cast")



function s5936701_Cast(MapID, InstanceID, SkillID, OwnerID)

	if MapID==3870157173 then
		

		local x,y,z = unit.GetPosition(MapID, InstanceID,OwnerID)

		map.MapCreateCreature(MapID, InstanceID, 3040327, x+5, y, z+5)
		map.MapCreateCreature(MapID, InstanceID, 3040327, x-5, y, z+5)
		map.MapCreateCreature(MapID, InstanceID, 3040327, x+5, y, z-5)
		map.MapCreateCreature(MapID, InstanceID, 3040327, x-5, y, z-5)
		
	end

end
aux.RegisterSkillEvent(5936701, 1, "s5936701_Cast")



--[[
function c3040328_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10054)
	

end

aux.RegisterCreatureEvent(3040328, 2, "c3040328_OnEnterCombat")



function c3040328_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10055)

end

aux.RegisterCreatureEvent(3040328, 4, "c3040328_OnDie")

]]--

