--���ﺰ���ű�

-- �������ս������
function c1040010_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20088)
       end
end

aux.RegisterCreatureEvent(1040010, 2, "c1040010_OnEnterCombat")
