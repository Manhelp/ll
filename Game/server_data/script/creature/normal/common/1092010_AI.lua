--1092010	ħŮ�������
--5110101	���＼��	����AI����	�ڻ�
--5110401	���＼��	����AI����	��������

function s5110101_Cast(MapID, InstanceID, SkillID, OwnerID)
	local a = math.random(1,100)
	if a > 50 then
	cre.MonsterSay(MapID, InstanceID, OwnerID, 20260)
	end
end
aux.RegisterSkillEvent(5110101, 1, "s5110101_Cast")


function s5110401_Cast(MapID, InstanceID, SkillID, OwnerID)
	local a = math.random(1,100)
	if a > 50 then
	cre.MonsterSay(MapID, InstanceID, OwnerID, 20260)
	end
end
aux.RegisterSkillEvent(5110401, 1, "s5110401_Cast")
