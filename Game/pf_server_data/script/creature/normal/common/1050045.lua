--���ﺰ���ű�

-- �������ս������
function c1050045_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20147)
       end
end

aux.RegisterCreatureEvent(10500245, 2, "c1050045_OnEnterCombat")