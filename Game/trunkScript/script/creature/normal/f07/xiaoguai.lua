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


function f07huayao_OnEnterCombat(MapID, InstanceID, CreatureID)

	local p = math.random(1,100)
	if p<15 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10083)
	elseif p<30 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10084)
	elseif p<45 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10085)
	end
	
end

aux.RegisterCreatureEvent(3040130, 2, "f07huayao_OnEnterCombat")
aux.RegisterCreatureEvent(3040131, 2, "f07huayao_OnEnterCombat")
aux.RegisterCreatureEvent(3040132, 2, "f07huayao_OnEnterCombat")
aux.RegisterCreatureEvent(3040133, 2, "f07huayao_OnEnterCombat")
aux.RegisterCreatureEvent(3040134, 2, "f07huayao_OnEnterCombat")
aux.RegisterCreatureEvent(3040135, 2, "f07huayao_OnEnterCombat")


function f07guyongbing_OnEnterCombat(MapID, InstanceID, CreatureID)

	local p = math.random(1,100)
	if p<15 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10086)
	elseif p<30 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10087)
	
	end
	
end

aux.RegisterCreatureEvent(3040136, 2, "f07guyongbing_OnEnterCombat")
aux.RegisterCreatureEvent(3040137, 2, "f07guyongbing_OnEnterCombat")
aux.RegisterCreatureEvent(3040138, 2, "f07guyongbing_OnEnterCombat")
aux.RegisterCreatureEvent(3040139, 2, "f07guyongbing_OnEnterCombat")

function f07weibing_OnEnterCombat(MapID, InstanceID, CreatureID)

	local p = math.random(1,100)
	if p<15 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10097)
	elseif p<30 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10098)
	elseif p<45 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10099)
	
	end
	
end

aux.RegisterCreatureEvent(3040144, 2, "f07weibing_OnEnterCombat")
aux.RegisterCreatureEvent(3040145, 2, "f07weibing_OnEnterCombat")
aux.RegisterCreatureEvent(3040146, 2, "f07weibing_OnEnterCombat")
aux.RegisterCreatureEvent(3040147, 2, "f07weibing_OnEnterCombat")
aux.RegisterCreatureEvent(3040148, 2, "f07weibing_OnEnterCombat")
aux.RegisterCreatureEvent(3040149, 2, "f07weibing_OnEnterCombat")

