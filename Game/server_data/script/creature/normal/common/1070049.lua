--���ﺰ���ű�

-- �������ս������
function c1070049_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=30 then
		--30%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20219)
       end
end

aux.RegisterCreatureEvent(1070049, 2, "c1070049_OnEnterCombat")