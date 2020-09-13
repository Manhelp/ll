---mapscriptdata[1] ��¼���������Ƿ���ˢ������
----[2]��¼3040008�Ƿ�ɱ��
----[3]��¼3040009�Ƿ�ɱ��
----[4]��¼�Ƿ��й���Χ��
----[5]��¼�Ƿ��������
----[6]��¼3040010�Ƿ�ɱ��

function c3040007_OnEnterCombat(MapID, InstanceID, CreatureID)
	cre.MonsterSay(MapID, InstanceID, CreatureID, 10002)
	local TargetID2 = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID2, 1, 30, 20)
end
aux.RegisterCreatureEvent(3040007, 2, "c3040007_OnEnterCombat")



function c3040006_OnEnterCombat(MapID, InstanceID, CreatureID)
	cre.MonsterSay(MapID, InstanceID, CreatureID, 10012)
	local TargetID2 = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID2, 1, 30, 20)
end
aux.RegisterCreatureEvent(3040006, 2, "c3040006_OnEnterCombat")



function c3040005_OnEnterCombat(MapID, InstanceID, CreatureID)
	cre.MonsterSay(MapID, InstanceID, CreatureID, 10005)
	local TargetID2 = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID2, 1, 30, 20)
end
aux.RegisterCreatureEvent(3040005, 2, "c3040005_OnEnterCombat")



function dwarf_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	----------------50%����ˢ����ꡣ
	local temp = math.random(1,100)
	local x,y,z=unit.GetPosition(MapID, InstanceID, TargetID)
	if temp<40 then
		-------------------ˢ����꺰��
		cre.MonsterSay(MapID, InstanceID, TargetID, 10001)
		if TargetTypeID==3040007 then
			map.MapCreateCreature(MapID, InstanceID, 3040031, x, y, z)
		elseif TargetTypeID==3040006 then
			map.MapCreateCreature(MapID, InstanceID, 3040032, x, y, z)
		elseif TargetTypeID==3040005 then
			map.MapCreateCreature(MapID, InstanceID, 3040033, x, y, z)
		elseif TargetTypeID==3040004 then
			map.MapCreateCreature(MapID, InstanceID, 3040034, x, y, z)
		end
	else
		local boss_1 = map.GetMapScriptData(MapID, InstanceID, 1, 2)
		local boss_2 = map.GetMapScriptData(MapID, InstanceID, 1, 3)
		local boss_3 = map.GetMapScriptData(MapID, InstanceID, 1, 6) 
		if boss_1 ==0 then
			cre.MonsterSay(MapID, InstanceID, TargetID, 10013)
		elseif boss_2==0 then
			cre.MonsterSay(MapID, InstanceID, TargetID, 10014)
		elseif boss_3==0 then
			cre.MonsterSay(MapID, InstanceID, TargetID, 10015)
		else 
			cre.MonsterSay(MapID, InstanceID, TargetID, 10016)
		end	
	end
end

aux.RegisterCreatureEvent(3040004, 4, "dwarf_OnDie")
aux.RegisterCreatureEvent(3040005, 4, "dwarf_OnDie")
aux.RegisterCreatureEvent(3040006, 4, "dwarf_OnDie")
aux.RegisterCreatureEvent(3040007, 4, "dwarf_OnDie")