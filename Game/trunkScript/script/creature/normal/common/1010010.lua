--���ﺰ���ű�

-- �������ս������
function c1010010_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20082)
       end
end

aux.RegisterCreatureEvent(1010010, 2, "c1010010_OnEnterCombat")

