--���ﺰ���ű�

-- �������ս������
function c1050024_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20131)
       end
end

aux.RegisterCreatureEvent(10500224, 2, "c1050024_OnEnterCombat")