--1092001	ѩ֮Ů���εϿ���
--5101201	���＼��	����AI����	���

function s5101201_Cast(MapID, InstanceID, SkillID, OwnerID)
	local a = math.random(1,100)
	if a >50 then
		cre.MonsterSay(MapID, InstanceID, OwnerID, 20254)
	end
end
aux.RegisterSkillEvent(5101201, 1, "s5101201_Cast")


