--���ﺰ���ű�

-- �������ս������
function c1070076_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20238)
       end
end

aux.RegisterCreatureEvent(1070076, 2, "c1070076_OnEnterCombat")