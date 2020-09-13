--�����Ƿ���������ֵ
function HuoDeLiLian(MapID, InstanceID, RoleID, Num)
	local LiLian = role.GetRoleAwardPoint(RoleID)
    local Point_Quest = {}
	Point_Quest[1000] = 30805
	Point_Quest[2500] = 30806
	Point_Quest[4500] = 30807
	Point_Quest[7500] = 30808
	Point_Quest[11500] = 30809
	Point_Quest[16500] = 30810
	Point_Quest[26500] = 30811
	Point_Quest[51500] = 30812
	Point_Quest[101500] = 30813
	Point_Quest[401500] = 30814

	for k,v in pairs(Point_Quest) do
		if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, v) then
			if LiLian and LiLian < k then
			    --����һ�õ�����ֵ������������ڽ׶�����ֵ���ȶ������ڽ׶Σ�����ֵ��0~2Num֮�両��
				local val = 1
			    if k < 10000 then
				    val = k/10000
				end
			    if math.random(2) == 2 then
				    Num = Num + math.floor(math.random(Num)*val)
				else
				    Num = Num - math.floor(math.random(Num)*val)
				end
				role.AddRoleAwardData(RoleID,Num)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 443134)        --��������ɣ��޷�������������ֵ
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
			break
		end
	end
end
--������
function LiLian_CheakComplete(MapID, InstanceID, RoleID)
    local LiLian = role.GetRoleAwardPoint(RoleID)
    local Point_Quest = {}
	Point_Quest[1000] = 30805
	Point_Quest[2500] = 30806
	Point_Quest[4500] = 30807
	Point_Quest[7500] = 30808
	Point_Quest[11500] = 30809
	Point_Quest[16500] = 30810
	Point_Quest[26500] = 30811
	Point_Quest[51500] = 30812
	Point_Quest[101500] = 30813
	Point_Quest[401500] = 30814

	for k,v in pairs(Point_Quest) do
		if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, v) then
		    if LiLian and LiLian >= k then
				role.ModSpecialTargetValue(MapID, InstanceID, RoleID, v, 2)
			end
			break
		end
	end
end

--��Ч����ҩˮ3010040
function i3010040_OnUse(MapID, InstanceID, TypeID, TargetID)
	if role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 30809) == true then
		HuoDeLiLian(MapID, InstanceID, TargetID, 45)
		LiLian_CheakComplete(MapID, InstanceID, TargetID)
	elseif role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 30810) == true then
		HuoDeLiLian(MapID, InstanceID, TargetID, 35)
		LiLian_CheakComplete(MapID, InstanceID, TargetID)
	end
end
aux.RegisterItemEvent(3010040, 1, "i3010040_OnUse")

--���������[12Сʱ]5600006
function i5600006_OnUse(MapID, InstanceID, TypeID, TargetID)
	if role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 30811) == true then
		HuoDeLiLian(MapID, InstanceID, TargetID, 60)
		LiLian_CheakComplete(MapID, InstanceID, TargetID)
	elseif role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 30812) == true then
		HuoDeLiLian(MapID, InstanceID, TargetID, 45)
		LiLian_CheakComplete(MapID, InstanceID, TargetID)
	elseif role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 30813) == true then
		HuoDeLiLian(MapID, InstanceID, TargetID, 40)
		LiLian_CheakComplete(MapID, InstanceID, TargetID)
	elseif role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 30814) == true then
		HuoDeLiLian(MapID, InstanceID, TargetID, 35)
		LiLian_CheakComplete(MapID, InstanceID, TargetID)
	end
end
aux.RegisterItemEvent(5600006, 1, "i5600006_OnUse")

--��ȡ�������ж������Ƿ��Ѿ�����ֱ�����
function q30805_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	LiLian_CheakComplete(MapID, InstanceID, RoleID)
end
aux.RegisterQuestEvent(30805, 0, "q30805_OnAccept")
aux.RegisterQuestEvent(30806, 0, "q30805_OnAccept")
aux.RegisterQuestEvent(30807, 0, "q30805_OnAccept")
aux.RegisterQuestEvent(30808, 0, "q30805_OnAccept")
aux.RegisterQuestEvent(30809, 0, "q30805_OnAccept")
aux.RegisterQuestEvent(30810, 0, "q30805_OnAccept")
aux.RegisterQuestEvent(30811, 0, "q30805_OnAccept")
aux.RegisterQuestEvent(30812, 0, "q30805_OnAccept")
aux.RegisterQuestEvent(30813, 0, "q30805_OnAccept")
aux.RegisterQuestEvent(30814, 0, "q30805_OnAccept")
--�������+����
function qhuigouzhiyin_On_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	HuoDeLiLian(MapID, InstanceID, RoleID, 100)
end
aux.RegisterQuestEvent(30800, 1, "qhuigouzhiyin_On_Complete")
aux.RegisterQuestEvent(30801, 1, "qhuigouzhiyin_On_Complete")
aux.RegisterQuestEvent(30802, 1, "qhuigouzhiyin_On_Complete")
aux.RegisterQuestEvent(30803, 1, "qhuigouzhiyin_On_Complete")
aux.RegisterQuestEvent(30804, 1, "qhuigouzhiyin_On_Complete")



------------------------------------------------------------------����Ϊ�Ƿ�����������жϣ�����ÿ��һ��������д��һ��----------------------------------------------------
------------------------------------------------------------------ÿ��������ߵ�ʱ�򣬳�����������������Ҹú���������ΪqXXXXX_CheckComplete----------------------------------------------------
function q30805_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local LiLian = role.GetRoleAwardPoint(RoleID)
	if LiLian >= 1000 then
		return 1
	else
	    return 0
	end
end
aux.RegisterQuestEvent(30805, 5, "q30805_CheckComplete")

function q30806_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local LiLian = role.GetRoleAwardPoint(RoleID)
	if LiLian >= 2500 then
		return 1
	else
	    return 0
	end
end
aux.RegisterQuestEvent(30806, 5, "q30806_CheckComplete")

function q30807_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local LiLian = role.GetRoleAwardPoint(RoleID)
	if LiLian >= 4500 then
		return 1
	else
	    return 0
	end
end
aux.RegisterQuestEvent(30807, 5, "q30807_CheckComplete")

function q30808_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local LiLian = role.GetRoleAwardPoint(RoleID)
	if LiLian >= 7500 then
		return 1
	else
	    return 0
	end
end
aux.RegisterQuestEvent(30808, 5, "q30808_CheckComplete")

function q30809_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local LiLian = role.GetRoleAwardPoint(RoleID)
	if LiLian >= 11500 then
		return 1
	else
	    return 0
	end
end
aux.RegisterQuestEvent(30809, 5, "q30809_CheckComplete")

function q30810_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local LiLian = role.GetRoleAwardPoint(RoleID)
	if LiLian >= 16500 then
		return 1
	else
	    return 0
	end
end
aux.RegisterQuestEvent(30810, 5, "q30810_CheckComplete")

function q30811_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local LiLian = role.GetRoleAwardPoint(RoleID)
	if LiLian >= 26500 then
		return 1
	else
	    return 0
	end
end
aux.RegisterQuestEvent(30811, 5, "q30811_CheckComplete")

function q30812_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local LiLian = role.GetRoleAwardPoint(RoleID)
	if LiLian >= 51500 then
		return 1
	else
	    return 0
	end
end
aux.RegisterQuestEvent(30812, 5, "q30812_CheckComplete")

function q30813_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local LiLian = role.GetRoleAwardPoint(RoleID)
	if LiLian >= 101500 then
		return 1
	else
	    return 0
	end
end
aux.RegisterQuestEvent(30813, 5, "q30813_CheckComplete")

function q30814_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local LiLian = role.GetRoleAwardPoint(RoleID)
	if LiLian >= 401500 then
		return 1
	else
	    return 0
	end
end
aux.RegisterQuestEvent(30814, 5, "q30814_CheckComplete")








