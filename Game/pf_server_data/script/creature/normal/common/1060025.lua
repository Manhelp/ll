--���ﺰ���ű�

-- �������ս������
function c1060025_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20164)
       end
end

aux.RegisterCreatureEvent(1060025, 2, "c1060025_OnEnterCombat")