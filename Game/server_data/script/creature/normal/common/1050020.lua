--���ﺰ���ű�

-- �������ս������
function c1050020_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20127)
       end
end

aux.RegisterCreatureEvent(1050020, 2, "c1050020_OnEnterCombat")