--���ﺰ���ű�

-- �������ս������
function c1060031_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20168)
       end
end

aux.RegisterCreatureEvent(1060031, 2, "c1060031_OnEnterCombat")