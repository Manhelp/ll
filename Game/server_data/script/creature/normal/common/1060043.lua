--���ﺰ���ű�

-- �������ս������
function c1060043_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=30 then
		--30%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20174)
       end
end

aux.RegisterCreatureEvent(1060043, 2, "c1060043_OnEnterCombat")