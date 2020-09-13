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


function c3040142_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10091)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040142, 2, "c3040142_OnEnterCombat")


function s5933201_Cast(MapID, InstanceID, SkillID, OwnerID)
	
	--cre.MonsterSay(MapID, InstanceID, OwnerID, 10038)
		
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
	
	local indexbuffId = math.random(1,4)
	if indexbuffId == 1 then
    		unit.AddBuff(MapID, InstanceID,TargetID,5933201,OwnerID)
	elseif indexbuffId == 2 then
		unit.AddBuff(MapID, InstanceID,TargetID,5933401,OwnerID)
	elseif indexbuffId == 3 then
		unit.AddBuff(MapID, InstanceID,TargetID,5933501,OwnerID)
	else
		unit.AddBuff(MapID, InstanceID,TargetID,5933301,OwnerID)
	end
	
end
aux.RegisterSkillEvent(5933201, 1, "s5933201_Cast")



function c3040142_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10092)
	
	map.OpenCloseDoor(MapID, InstanceID, 2219, 1)

	map.MapCreateCreature(MapID, InstanceID, 3040141, 149, 3840, 897)
	map.MapCreateCreature(MapID, InstanceID, 3040143, 749, 3840, 1116)


	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100112)
	--�Ŵ�
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(3040142, 4, "c3040142_OnDie")
