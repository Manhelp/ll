--���ﺰ���ű�

-- �������ս������
function c1040041_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=30 then
		--30%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20108)
       end
end

aux.RegisterCreatureEvent(1040041, 2, "c1040041_OnEnterCombat")