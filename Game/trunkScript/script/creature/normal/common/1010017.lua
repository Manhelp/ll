--���ﺰ���ű�

-- �������ս������
function c1010017_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=75 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20067)
       end
end

aux.RegisterCreatureEvent(1010017, 2, "c1010017_OnEnterCombat")

