--���ﺰ���ű�

-- �������ս������
function c1040038_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=30 then
		--30%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20105)
       end
end

aux.RegisterCreatureEvent(1040038, 2, "c1040038_OnEnterCombat")