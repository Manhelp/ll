--���ﺰ���ű�

-- �������ս������
function c1050018_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20125)
       end
end

aux.RegisterCreatureEvent(1050018, 2, "c1050018_OnEnterCombat")