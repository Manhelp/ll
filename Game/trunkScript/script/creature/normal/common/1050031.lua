--���ﺰ���ű�

-- �������ս������
function c1050031_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=30 then
		--30%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20136)
       end
end

aux.RegisterCreatureEvent(10500231, 2, "c1050031_OnEnterCombat")