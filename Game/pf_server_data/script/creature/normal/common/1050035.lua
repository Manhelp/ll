--���ﺰ���ű�

-- �������ս������
function c1050035_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=30 then
		--30%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20140)
       end
end

aux.RegisterCreatureEvent(10500235, 2, "c1050035_OnEnterCombat")