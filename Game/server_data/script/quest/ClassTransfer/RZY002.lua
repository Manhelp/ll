--�������ʱ
--תְΪ����
function q41002_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 3)
	unit.AddBuff(mapID, instanceID, ownerID, 3202901, ownerID)
	role.NotifyFBBS(ownerID,9,3)
	if role.GetTrialState(ownerID, 2003) == 1 then
		role.SetTrialCompleteNum(ownerID, 2003, 1)
    end
	if role.GetTrialState(ownerID, 2003) == 1 and role.GetTrialCompleteNum(ownerID,2003) >= Trail_maxnum[2003] then
		role.TrialComplete(ownerID,2003)
	end
	--���20��תְ������ҵȼ�
	UpdateRoleData(ownerID, 2, role.GetRoleLevel(mapID, instanceID, ownerID))
end

--תְΪ��ʸ
function q41007_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 8)
	role.NotifyFBBS(ownerID,9,8)
	if role.GetTrialState(ownerID, 6003) == 1 then
		role.SetTrialCompleteNum(ownerID, 6003, 1)
    end
	if role.GetTrialState(ownerID, 6003) == 1 and role.GetTrialCompleteNum(ownerID,6003) >= Trail_maxnum[6003] then
		role.TrialComplete(ownerID,6003)
	end
end

--תְΪռ��
function q41008_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 9)
	role.NotifyFBBS(ownerID,9,9)
	if role.GetTrialState(ownerID, 6003) == 1 then
		role.SetTrialCompleteNum(ownerID, 6003, 1)
    end
	if role.GetTrialState(ownerID, 6003) == 1 and role.GetTrialCompleteNum(ownerID,6003) >= Trail_maxnum[6003] then
		role.TrialComplete(ownerID,6003)
	end
end

--�任ְҵΪ����
function q41014_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 3);
end

--ע��
aux.RegisterQuestEvent(41002, 1, "q41002_OnComplete")
aux.RegisterQuestEvent(41007, 1, "q41007_OnComplete")
aux.RegisterQuestEvent(41008, 1, "q41008_OnComplete")
aux.RegisterQuestEvent(41014, 1, "q41014_OnComplete")

