--���ﺰ���ű�

-- �������ս������
function c1070075_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20237)
       end
end

aux.RegisterCreatureEvent(1070075, 2, "c1070075_OnEnterCombat")