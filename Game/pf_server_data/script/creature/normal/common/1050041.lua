--���ﺰ���ű�

-- �������ս������
function c1050041_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20145)
       end
end

aux.RegisterCreatureEvent(10500241, 2, "c1050041_OnEnterCombat")