function q30835_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	--�����ҵ�ǰ�����������ħ������
	local curGodPoint = 0
	local curMonsterPoint = 0
	curGodPoint,curMonsterPoint = role.GetRoleSoulPoint(MapID,InstanceID,RoleID)
	--�����ҵ�ǰ������ħ��
	local curGodSoul = 0
	local curMonsterSoul = 0
	curGodSoul,curMonsterSoul = role.GetRoleSoul(MapID,InstanceID,RoleID)
	--���ݵ�ǰ�����������������ӱ���ȷ�����������������
	local GodPoint = 0
	local MonsterPoint = 0
	local GodSoul = 0
	local MonsterSoul = 0
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69374) then
		GodPoint = 500*shengwangjiabei
		GodSoul = 200*shengwangjiabei
	else
		GodPoint = 500
		GodSoul = 200
	end
	if(not role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69373)) then
		if curMonsterPoint >= GodPoint*0.5 then
			MonsterPoint = GodPoint*(-0.5)
		else
			MonsterPoint = curMonsterPoint*(-1)
		end
	end
	role.SetRoleSoulPoint(MapID,InstanceID,RoleID,curGodPoint+GodPoint,curMonsterPoint+MonsterPoint)
	role.SetRoleSoul(MapID,InstanceID,RoleID,curGodSoul+GodSoul,curMonsterSoul+MonsterSoul)

end

aux.RegisterQuestEvent(30835, 1, "q30835_OnComplete")


function q30836_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	--�����ҵ�ǰ�����������ħ������
	local curGodPoint = 0
	local curMonsterPoint = 0
	curGodPoint,curMonsterPoint = role.GetRoleSoulPoint(MapID,InstanceID,RoleID)
	--�����ҵ�ǰ������ħ��
	local curGodSoul = 0
	local curMonsterSoul = 0
	curGodSoul,curMonsterSoul = role.GetRoleSoul(MapID,InstanceID,RoleID)
	--���ݵ�ǰ�����������������ӱ���ȷ��������������ħ��
	local GodPoint = 0
	local MonsterPoint = 0
	local GodSoul = 0
	local MonsterSoul = 0
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69374) then
		MonsterPoint = 500*shengwangjiabei
		MonsterSoul = 200*shengwangjiabei
	else
		MonsterPoint = 500
		MonsterSoul = 200
	end
	if(not role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69373)) then
		if curGodPoint >= MonsterPoint*0.5 then
			GodPoint = MonsterPoint*(-0.5)
		else
			GodPoint = curGodPoint*(-1)
		end
	end
	role.SetRoleSoulPoint(MapID,InstanceID,RoleID,curGodPoint+GodPoint,curMonsterPoint+MonsterPoint)
	role.SetRoleSoul(MapID,InstanceID,RoleID,curGodSoul+GodSoul,curMonsterSoul+MonsterSoul)

end

aux.RegisterQuestEvent(30836, 1, "q30836_OnComplete")


function q30837_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	--�����ҵ�ǰ�����������ħ������
	local curGodPoint = 0
	local curMonsterPoint = 0
	curGodPoint,curMonsterPoint = role.GetRoleSoulPoint(MapID,InstanceID,RoleID)
	--�����ҵ�ǰ������ħ��
	local curGodSoul = 0
	local curMonsterSoul = 0
	curGodSoul,curMonsterSoul = role.GetRoleSoul(MapID,InstanceID,RoleID)
	--���ݵ�ǰ�����������������ӱ���ȷ�����������������
	local GodPoint = 0
	local MonsterPoint = 0
	local GodSoul = 0
	local MonsterSoul = 0
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69374) then
		GodPoint = 500*shengwangjiabei
		GodSoul = 200*shengwangjiabei
	else
		GodPoint = 500
		GodSoul = 200
	end
	if(not role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69373)) then
		if curMonsterPoint >= GodPoint*0.5 then
			MonsterPoint = GodPoint*(-0.5)
		else
			MonsterPoint = curMonsterPoint*(-1)
		end
	end
	role.SetRoleSoulPoint(MapID,InstanceID,RoleID,curGodPoint+GodPoint,curMonsterPoint+MonsterPoint)
	role.SetRoleSoul(MapID,InstanceID,RoleID,curGodSoul+GodSoul,curMonsterSoul+MonsterSoul)
end

aux.RegisterQuestEvent(30837, 1, "q30837_OnComplete")


function q30838_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	--�����ҵ�ǰ�����������ħ������
	local curGodPoint = 0
	local curMonsterPoint = 0
	curGodPoint,curMonsterPoint = role.GetRoleSoulPoint(MapID,InstanceID,RoleID)
	--�����ҵ�ǰ������ħ��
	local curGodSoul = 0
	local curMonsterSoul = 0
	curGodSoul,curMonsterSoul = role.GetRoleSoul(MapID,InstanceID,RoleID)
	--���ݵ�ǰ�����������������ӱ���ȷ��������������ħ��
	local GodPoint = 0
	local MonsterPoint = 0
	local GodSoul = 0
	local MonsterSoul = 0
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69374) then
		MonsterPoint = 500*shengwangjiabei
		MonsterSoul = 200*shengwangjiabei
	else
		MonsterPoint = 500
		MonsterSoul = 200
	end
	if(not role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69373)) then
		if curGodPoint >= MonsterPoint*0.5 then
			GodPoint = MonsterPoint*(-0.5)
		else
			GodPoint = curGodPoint*(-1)
		end
	end
	role.SetRoleSoulPoint(MapID,InstanceID,RoleID,curGodPoint+GodPoint,curMonsterPoint+MonsterPoint)
	role.SetRoleSoul(MapID,InstanceID,RoleID,curGodSoul+GodSoul,curMonsterSoul+MonsterSoul)
end

aux.RegisterQuestEvent(30838, 1, "q30838_OnComplete")



function q30839_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	--�����ҵ�ǰ������ħ��
	local curGodSoul = 0
	local curMonsterSoul = 0
	curGodSoul,curMonsterSoul = role.GetRoleSoul(MapID,InstanceID,RoleID)
	--���ݵ�ǰ�����������������ӱ���ȷ�����������������
	local GodSoul = 0
	local MonsterSoul = 0
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69374) then
		GodSoul = 100*shengwangjiabei
	else
		GodSoul = 100
	end
	role.SetRoleSoul(MapID,InstanceID,RoleID,curGodSoul+GodSoul,curMonsterSoul+MonsterSoul)
end

aux.RegisterQuestEvent(30839, 1, "q30839_OnComplete")


function q30840_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	--�����ҵ�ǰ������ħ��
	local curGodSoul = 0
	local curMonsterSoul = 0
	curGodSoul,curMonsterSoul = role.GetRoleSoul(MapID,InstanceID,RoleID)
	--���ݵ�ǰ�����������������ӱ���ȷ��������������ħ��
	local GodSoul = 0
	local MonsterSoul = 0
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69374) then
		MonsterSoul = 100*shengwangjiabei
	else
		MonsterSoul = 100
	end
	role.SetRoleSoul(MapID,InstanceID,RoleID,curGodSoul+GodSoul,curMonsterSoul+MonsterSoul)
end

aux.RegisterQuestEvent(30840, 1, "q30840_OnComplete")





function q30841_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	--�����ҵ�ǰ������ħ��
	local curGodSoul = 0
	local curMonsterSoul = 0
	curGodSoul,curMonsterSoul = role.GetRoleSoul(MapID,InstanceID,RoleID)
	--���ݵ�ǰ�����������������ӱ���ȷ�����������������
	local GodSoul = 0
	local MonsterSoul = 0
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69374) then
		GodSoul = 100*shengwangjiabei
	else
		GodSoul = 100
	end
	role.SetRoleSoul(MapID,InstanceID,RoleID,curGodSoul+GodSoul,curMonsterSoul+MonsterSoul)
end

aux.RegisterQuestEvent(30841, 1, "q30841_OnComplete")


function q30842_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	--�����ҵ�ǰ������ħ��
	local curGodSoul = 0
	local curMonsterSoul = 0
	curGodSoul,curMonsterSoul = role.GetRoleSoul(MapID,InstanceID,RoleID)
	--���ݵ�ǰ�����������������ӱ���ȷ��������������ħ��
	local GodSoul = 0
	local MonsterSoul = 0
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69374) then
		MonsterSoul = 100*shengwangjiabei
	else
		MonsterSoul = 100
	end
	role.SetRoleSoul(MapID,InstanceID,RoleID,curGodSoul+GodSoul,curMonsterSoul+MonsterSoul)
end

aux.RegisterQuestEvent(30842, 1, "q30842_OnComplete")
