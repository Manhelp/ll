--���ﺰ���ű�

-- �������ս������
function c1091001_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=75 then
		--75%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 30004)
       end
end

aux.RegisterCreatureEvent(1091001, 2, "c1091001_OnEnterCombat")

