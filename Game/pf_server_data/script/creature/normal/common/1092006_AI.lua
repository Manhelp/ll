--1092006	������Լ¶����
--5106101	���＼��	����AI����	������
function s5106101_Cast(MapID, InstanceID, SkillID, OwnerID)
	local a = math.random(1,100)
	if a > 50 then
	cre.MonsterSay(MapID, InstanceID, OwnerID, 20257)
	end
end
aux.RegisterSkillEvent(5106101, 1, "s5106101_Cast")


