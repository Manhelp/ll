--���ﺰ���ű�

-- �������ս������
function c1050043_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20146)
       end
end

aux.RegisterCreatureEvent(10500243, 2, "c1050043_OnEnterCombat")