--1092005	Ů����̩��ɯ
--5105301	���＼��	����AI����	ս��ŭ��
function s5105301_Cast(MapID, InstanceID, SkillID, OwnerID)

	cre.MonsterSay(MapID, InstanceID, OwnerID, 20256)
end
aux.RegisterSkillEvent(5105301, 1, "s5105301_Cast")


