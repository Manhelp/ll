--���ﺰ���ű�

-- �������ս������
function c1070067_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=30 then
		--30%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20233)
       end
end

aux.RegisterCreatureEvent(1070067, 2, "c1070067_OnEnterCombat")