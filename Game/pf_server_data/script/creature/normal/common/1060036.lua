--���ﺰ���ű�

-- �������ս������
function c1060036_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=30 then
		--30%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20170)
       end
end

aux.RegisterCreatureEvent(1060036, 2, "c1060036_OnEnterCombat")