--1092008	����������ɯ
--5108201	���＼��	����AI����	�Ի���

function s5108201_Cast(MapID, InstanceID, SkillID, OwnerID)
	local a = math.random(1,100)
	if a > 50 then
	cre.MonsterSay(MapID, InstanceID, OwnerID, 20259)
	end
end
aux.RegisterSkillEvent(5108201, 1, "s5108201_Cast")


