--���ﺰ���ű�

-- �������ս������
function c1060020_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20159)
       end
end

aux.RegisterCreatureEvent(1060020, 2, "c1060020_OnEnterCombat")