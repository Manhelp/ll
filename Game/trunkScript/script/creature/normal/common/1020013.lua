--���ﺰ���ű�

-- �������ս������
function c1020013_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=35 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20073)
       end
	unit.AddBuff(MapID, InstanceID,CreatureID,5055901,CreatureID)
end

aux.RegisterCreatureEvent(1020013, 2, "c1020013_OnEnterCombat")

