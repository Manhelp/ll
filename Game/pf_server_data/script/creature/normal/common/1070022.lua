--���ﺰ���ű�

-- �������ս������
function c1070022_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20201)
       end
end

aux.RegisterCreatureEvent(1070022, 2, "c1070022_OnEnterCombat")