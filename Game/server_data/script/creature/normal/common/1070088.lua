--���ﺰ���ű�

-- �������ս������
function c1070088_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20250)
       end
end

aux.RegisterCreatureEvent(1070088, 2, "c1070088_OnEnterCombat")