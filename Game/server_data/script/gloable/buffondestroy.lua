function Buff_OnDestroy ( MapID, InstanceID, RoleID, BuffID )	--buff��ʧ

--[[--��Сʱ��������һ��buff��ʧ����ӵڶ���buff
	if BuffID == 9700101 then
		unit.AddBuff(MapID, InstanceID, RoleID, 9700201, RoleID)

--��Сʱ�������ڶ���buff��ʧ����ӵ�����buff
	elseif BuffID == 9700201 then
		unit.AddBuff(MapID, InstanceID, RoleID, 9700301, RoleID)
------������Сʱ����
		role.BaibaoAddRoleItem(MapID, InstanceID, RoleID, 6020001, 2, -1, 12, 1200)
--��Сʱ������������buff��ʧ����ӵ��ĸ�buff
	elseif BuffID == 9700301 then
		unit.AddBuff(MapID, InstanceID, RoleID, 9700401, RoleID)
------������Сʱ����
		role.BaibaoAddRoleItem(MapID, InstanceID, RoleID, 3070010, 1, -1, 12, 1200)
	end]]
	if BuffID == 3206001 then--����
		local t = math.random(100)
		if t <= 5 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206101,RoleID)
		elseif t > 5 and t <= 30 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206301,RoleID)
		elseif t > 30 and t <= 50 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206401,RoleID)
		elseif t > 50 and t <= 70 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206601,RoleID)
		elseif t > 70 and t <= 100 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206701,RoleID)
		end
	end
	if BuffID == 3207101 then--��
		local t = math.random(100)
		if t <= 5 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206101,RoleID)
		elseif t > 5 and t <= 30 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206801,RoleID)
		elseif t > 30 and t <= 50 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206401,RoleID)
		elseif t > 50 and t <= 70 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206601,RoleID)
		elseif t > 70 and t <= 100 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206701,RoleID)
		end
	end
	if BuffID == 3207201 then--��
		local t = math.random(100)
		if t <= 5 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206101,RoleID)
		elseif t > 5 and t <= 30 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206901,RoleID)
		elseif t > 30 and t <= 50 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206401,RoleID)
		elseif t > 50 and t <= 70 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206601,RoleID)
		elseif t > 70 and t <= 100 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206701,RoleID)
		end
	end
	if BuffID == 3207301 then--����
		local t = math.random(100)
		if t <= 5 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206101,RoleID)
		elseif t > 5 and t <= 30 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206301,RoleID)
		elseif t > 30 and t <= 50 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206401,RoleID)
		elseif t > 50 and t <= 70 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206601,RoleID)
		elseif t > 70 and t <= 100 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206701,RoleID)
		end
	end
------����ҹ�
	if BuffID == 9008101 then
		local quest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30798)
		if quest == true then
			unit.AddBuff(MapID, InstanceID, RoleID,9008201,RoleID)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["zhongxiaye"], 0)
		end
	end
	if BuffID == 9008201 then
		local quest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30798)
		if quest == true then
			unit.AddBuff(MapID, InstanceID, RoleID,9008301,RoleID)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["zhongxiaye"], 0)
		end
	end
	if BuffID == 9008301 then
		local quest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30798)
		if quest == true then
			unit.AddBuff(MapID, InstanceID, RoleID,9008401,RoleID)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["zhongxiaye"], 0)
		end
	end
	if BuffID == 9008401 then
		local quest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30798)
		if quest == true then
			unit.AddBuff(MapID, InstanceID, RoleID,9008501,RoleID)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["zhongxiaye"], 0)
		end
	end
	if BuffID == 9008501 then
		local quest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30798)
		if quest == true then
			role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 30798, 2)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["zhongxiaye"], 1)
			return
		end
	end
end

aux.RegisterBuffEvent( 0, "Buff_OnDestroy" )

function q30798_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local zhongxiayehuodong = role.GetRoleScriptData(RoleID, 1, RoleDataType["zhongxiaye"])
	if zhongxiayehuodong == 1 then
		return 1
	end
end
aux.RegisterQuestEvent(30798, 5, "q30798_OnComplete")
function q30798_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	unit.AddBuff(MapID, InstanceID, RoleID,9008101,RoleID)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["zhongxiaye"], 0)
end
aux.RegisterQuestEvent(30798, 0, "q30798_Complete")

function q30798_OnCancel(MapID, InstanceID, QuestID, RoleID, NPCID)
	unit.CancelBuff(MapID, InstanceID,RoleID,9008101)
	unit.CancelBuff(MapID, InstanceID,RoleID,9008201)
	unit.CancelBuff(MapID, InstanceID,RoleID,9008301)
	unit.CancelBuff(MapID, InstanceID,RoleID,9008401)
	unit.CancelBuff(MapID, InstanceID,RoleID,9008501)
end
aux.RegisterQuestEvent(30798, 2, "q30798_OnCancel")