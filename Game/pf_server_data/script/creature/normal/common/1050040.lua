--���ﺰ���ű�

-- �������ս������
function c1050040_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20144)
       end
end

aux.RegisterCreatureEvent(10500240, 2, "c1050040_OnEnterCombat")