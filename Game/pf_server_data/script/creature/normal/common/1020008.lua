--���ﺰ���ű�

-- �������ս������
function c1020008_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20071)
       end
end

aux.RegisterCreatureEvent(1020008, 2, "c1020008_OnEnterCombat")

