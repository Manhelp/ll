--���ﺰ���ű�

-- �������ս������
function c1070056_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=30 then
		--30%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20224)
       end
end

aux.RegisterCreatureEvent(1070056, 2, "c1070056_OnEnterCombat")