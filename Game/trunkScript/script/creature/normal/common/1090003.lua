--���ﺰ���ű�

-- �������ս������
function c1090003_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=75 then
		--75%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 30003)
       end
end

aux.RegisterCreatureEvent(1090003, 2, "c1090003_OnEnterCombat")

