--���ﺰ���ű�

-- �������ս������
function c1060057_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20185)
       end
end

aux.RegisterCreatureEvent(1060057, 2, "c1060057_OnEnterCombat")