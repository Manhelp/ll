--���ﺰ���ű�

-- �������ս������
function c1091004_OnEnterCombat(MapID, InstanceID, CreatureID)
	cre.MonsterSay(MapID, InstanceID, CreatureID, 30007)
end

aux.RegisterCreatureEvent(1091004, 2, "c1091004_OnEnterCombat")

