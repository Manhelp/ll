--���ﺰ���ű�

-- �������ս������
function c1050048_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20150)
       end
end

aux.RegisterCreatureEvent(10500248, 2, "c1050048_OnEnterCombat")