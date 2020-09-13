----scriptdata[0]��ǽ������Ƿ�ˢ��
----scriptdata[1]���3040306�Ƿ����� 0δ���� 1 ����
----scriptdata[2]���3040307�Ƿ����� 0δ���� 1 ����
----scriptdata[3]���3040332�Ƿ���� 0������ 1 ����



function jiangshi_OnEnterCombat(MapID, InstanceID, CreatureID)

	local poss = math.random(1,100)
	if poss > 80 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10200)
	end

	unit.AddBuff(MapID, InstanceID, CreatureID,5938101,CreatureID)
	
end

aux.RegisterCreatureEvent(3040300, 2, "jiangshi_OnEnterCombat")
aux.RegisterCreatureEvent(3040301, 2, "jiangshi_OnEnterCombat")
aux.RegisterCreatureEvent(3040302, 2, "jiangshi_OnEnterCombat")

aux.RegisterCreatureEvent(3040323, 2, "jiangshi_OnEnterCombat")
aux.RegisterCreatureEvent(3040324, 2, "jiangshi_OnEnterCombat")
aux.RegisterCreatureEvent(3040325, 2, "jiangshi_OnEnterCombat")



function nvpu_OnEnterCombat(MapID, InstanceID, CreatureID)

	local poss = math.random(1,100)
	if poss > 70 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10201)
	end
	
	
end

aux.RegisterCreatureEvent(3040308, 2, "nvpu_OnEnterCombat")
aux.RegisterCreatureEvent(3040309, 2, "nvpu_OnEnterCombat")
aux.RegisterCreatureEvent(3040310, 2, "nvpu_OnEnterCombat")
aux.RegisterCreatureEvent(3040311, 2, "nvpu_OnEnterCombat")



function nvpu_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10094)
	local x,y,z = unit.GetPosition(MapID,InstanceID,TargetID)
	
	local poss = math.random(1,3)
	if poss == 1 then
		map.MapCreateCreature(MapID, InstanceID, 3040323, x,y,z)
	elseif poss == 2 then
		map.MapCreateCreature(MapID, InstanceID, 3040324, x,y,z)
	elseif poss == 3 then
		map.MapCreateCreature(MapID, InstanceID, 3040325, x,y,z)
	end
	

end

aux.RegisterCreatureEvent(3040308, 4, "nvpu_OnDie")
aux.RegisterCreatureEvent(3040309, 4, "nvpu_OnDie")
aux.RegisterCreatureEvent(3040310, 4, "nvpu_OnDie")
aux.RegisterCreatureEvent(3040311, 4, "nvpu_OnDie")



function s5054509_Cast(MapID, InstanceID, SkillID, OwnerID)

	if MapID==3870157173 then
		
		local x,y,z = unit.GetPosition(MapID,InstanceID,OwnerID)

		map.MapCreateCreature(MapID, InstanceID, 3040300, x+2,y,z)
		map.MapCreateCreature(MapID, InstanceID, 3040301, x,y,z+2)
		map.MapCreateCreature(MapID, InstanceID, 3040302, x-2,y,z)
		
	end

end
aux.RegisterSkillEvent(5054509, 1, "s5054509_Cast")



function s5054510_Cast(MapID, InstanceID, SkillID, OwnerID)

	if MapID==3870157173 then
		
		local x,y,z = unit.GetPosition(MapID,InstanceID,OwnerID)

		map.MapCreateCreature(MapID, InstanceID, 3040312, x+2,y,z+2)
		map.MapCreateCreature(MapID, InstanceID, 3040313, x+2,y,z-2)
		
		
	end

end
aux.RegisterSkillEvent(5054510, 1, "s5054510_Cast")



