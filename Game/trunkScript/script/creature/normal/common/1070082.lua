--���ﺰ���ű�

-- �������ս������
function c1070082_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20244)
       end
end

aux.RegisterCreatureEvent(1070082, 2, "c1070082_OnEnterCombat")