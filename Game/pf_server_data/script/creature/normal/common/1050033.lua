--���ﺰ���ű�

-- �������ս������
function c1050033_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=30 then
		--30%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20138)
       end
end

aux.RegisterCreatureEvent(10500233, 2, "c1050033_OnEnterCombat")