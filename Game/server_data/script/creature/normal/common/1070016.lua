--���ﺰ���ű�

-- �������ս������
function c1070016_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20197)
       end
end

aux.RegisterCreatureEvent(1070016, 2, "c1070016_OnEnterCombat")