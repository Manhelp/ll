--���ﺰ���ű�

-- �������ս������
function c1070061_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=30 then
		--30%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20228)
       end
end

aux.RegisterCreatureEvent(1070061, 2, "c1070061_OnEnterCombat")