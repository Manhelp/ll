--���ﺰ���ű�

-- �������ս������
function c1010011_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20063)
       end
	unit.AddBuff(MapID, InstanceID,CreatureID,5055902,CreatureID)
end

aux.RegisterCreatureEvent(1010011, 2, "c1010011_OnEnterCombat")

