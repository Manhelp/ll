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


function c3040152_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10105)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040152, 2, "c3040152_OnEnterCombat")


function s5054506_Cast(MapID, InstanceID, SkillID, OwnerID)
	
	cre.MonsterSay(MapID, InstanceID, OwnerID, 10104)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	
	local creatureID = map.GetMapScriptData(MapID,InstanceID,1,6)
	map.MapDeleteCreature(MapID, InstanceID, creatureID)
	---ɾ������

	map.MapCreateCreature(MapID, InstanceID, 3040164, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 3040165, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 3040166, x, y, z)


	
end
aux.RegisterSkillEvent(5054506, 1, "s5054506_Cast")


--[[
function c3040152_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10064)
	
	
	local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 102, 100086)
	--�Ŵ�
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(3040152, 4, "c3040152_OnDie")
]]--