--1092004	��Ů��ܽ����
--5104101	���＼��	����AI����	ʥ���â

function s5104101_Cast(MapID, InstanceID, SkillID, OwnerID)

	local a = math.random(1,100)
	if a >50 then
		cre.MonsterSay(MapID, InstanceID, OwnerID, 20255)
	end
end
aux.RegisterSkillEvent(5104101, 1, "s5104101_Cast")


