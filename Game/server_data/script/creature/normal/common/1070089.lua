--���ﺰ���ű�

-- �������ս������
function c1070089_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20251)
       end
end

aux.RegisterCreatureEvent(1070089, 2, "c1070089_OnEnterCombat")