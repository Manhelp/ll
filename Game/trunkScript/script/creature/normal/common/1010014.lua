--���ﺰ���ű�

-- �������ս������
function c1010014_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20065)
       end
end

aux.RegisterCreatureEvent(1010014, 2, "c1010014_OnEnterCombat")

