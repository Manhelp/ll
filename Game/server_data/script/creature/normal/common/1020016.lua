--���ﺰ���ű�

-- �������ս������
function c1020016_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=75 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20076)
       end
end

aux.RegisterCreatureEvent(1020016, 2, "c1020016_OnEnterCombat")

