--���ﺰ���ű�

-- �������ս������
function c1050047_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20149)
       end
end

aux.RegisterCreatureEvent(10500247, 2, "c1050047_OnEnterCombat")