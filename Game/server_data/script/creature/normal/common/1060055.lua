--���ﺰ���ű�

-- �������ս������
function c1060055_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20183)
       end
end

aux.RegisterCreatureEvent(1060055, 2, "c1060055_OnEnterCombat")