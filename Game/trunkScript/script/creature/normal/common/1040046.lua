--���ﺰ���ű�

-- �������ս������
function c1040046_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20112)
       end
end

aux.RegisterCreatureEvent(1040046, 2, "c1040046_OnEnterCombat")