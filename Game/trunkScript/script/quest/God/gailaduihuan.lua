--Ԥ���߸���
--����֮ʯ

--�������
function q30303_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	--role.AddRoleItem(MapID, InstanceID, RoleID, 4810005, 10, 0, 0, 104)  --����10������ҩ��
    CheckFaith(MapID, InstanceID, 300, RoleID)
end


aux.RegisterQuestEvent(30308, 1, "q30303_OnComplete")
aux.RegisterQuestEvent(30309, 1, "q30303_OnComplete")
aux.RegisterQuestEvent(30310, 1, "q30303_OnComplete")
aux.RegisterQuestEvent(30311, 1, "q30303_OnComplete")
aux.RegisterQuestEvent(30312, 1, "q30303_OnComplete")
aux.RegisterQuestEvent(30313, 1, "q30303_OnComplete")
aux.RegisterQuestEvent(30314, 1, "q30303_OnComplete")
aux.RegisterQuestEvent(30315, 1, "q30303_OnComplete")



--�����ᾧ


--�������
function q30304_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	--role.AddRoleItem(MapID, InstanceID, RoleID, 4810005, 20, 0, 0, 104)  --����10������ҩ��
    CheckFaith(MapID, InstanceID, 200, RoleID)
end



aux.RegisterQuestEvent(30316, 1, "q30304_OnComplete")
aux.RegisterQuestEvent(30317, 1, "q30304_OnComplete")
aux.RegisterQuestEvent(30318, 1, "q30304_OnComplete")
aux.RegisterQuestEvent(30319, 1, "q30304_OnComplete")
aux.RegisterQuestEvent(30320, 1, "q30304_OnComplete")
aux.RegisterQuestEvent(30321, 1, "q30304_OnComplete")
