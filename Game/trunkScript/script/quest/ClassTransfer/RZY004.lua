--�������ʱ
--תְ����
function q41004_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 5)
	unit.AddBuff(mapID, instanceID, ownerID, 3202901, ownerID)
	role.NotifyFBBS(ownerID,9,5)
	if role.GetTrialState(ownerID, 2003) == 1 then
		role.SetTrialCompleteNum(ownerID, 2003, 1)
    end
	if role.GetTrialState(ownerID, 2003) == 1 and role.GetTrialCompleteNum(ownerID,2003) >= Trail_maxnum[2003] then
		role.TrialComplete(ownerID,2003)
	end
	--���20��תְ������ҵȼ�
	UpdateRoleData(ownerID, 2, role.GetRoleLevel(mapID, instanceID, ownerID))
end

--תְΪ����
function q41011_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 12)
	role.NotifyFBBS(ownerID,9,12)
	if role.GetTrialState(ownerID, 6003) == 1 then
		role.SetTrialCompleteNum(ownerID, 6003, 1)
    end
	if role.GetTrialState(ownerID, 6003) == 1 and role.GetTrialCompleteNum(ownerID,6003) >= Trail_maxnum[6003] then
		role.TrialComplete(ownerID,6003)
	end
end

--תְΪ����
function q41012_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 13)
	role.NotifyFBBS(ownerID,9,13)
	if role.GetTrialState(ownerID, 6003) == 1 then
		role.SetTrialCompleteNum(ownerID, 6003, 1)
    end
	if role.GetTrialState(ownerID, 6003) == 1 and role.GetTrialCompleteNum(ownerID,6003) >= Trail_maxnum[6003] then
		role.TrialComplete(ownerID,6003)
	end
end

--�仯ְҵ����
function q41016_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 5);
end
--ע��
aux.RegisterQuestEvent(41004, 1, "q41004_OnComplete")
aux.RegisterQuestEvent(41011, 1, "q41011_OnComplete")
aux.RegisterQuestEvent(41012, 1, "q41012_OnComplete")
aux.RegisterQuestEvent(41016, 1, "q41016_OnComplete")
