--�������쳣����
function s51_OnPlayerEnter(MapID, InstanceID,RoleID)
	local n = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmEnter"])
	if n == 0 then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmEnter"], 1)
	end
end
aux.RegisterMapEvent("s51", 2, "s51_OnPlayerEnter")

--ע��μ���ֲ����ҵ�����
plant_data = {}

--[[
plant_data[RoleID] = {}
plant_data[RoleID] = nil]]

--�洢����
farm_exp = {}
farm_exp[0] = 0
farm_exp[1] = 10
farm_exp[2] = 640
farm_exp[3] = 7290
farm_exp[4] = 40960
farm_exp[5] = 156250
farm_exp[6] = 466560
farm_exp[7] = 1176490
farm_exp[8] = 2621440
farm_exp[9] = 0
--��Ӧ����
seed_exp = {}
--�����ջ�
seed_exp[5800010] = 8
seed_exp[5800014] = 160
seed_exp[5800018] = 160
seed_exp[5800022] = 160
seed_exp[5800026] = 160
seed_exp[5800030] = 160
seed_exp[5800034] = 160
seed_exp[5800038] = 4
seed_exp[5800042] = 4
seed_exp[5800046] = 8
seed_exp[5800050] = 16
seed_exp[5800054] = 40
seed_exp[5800058] = 80
seed_exp[5800062] = 160
seed_exp[5800066] = 240
seed_exp[5800070] = 320
--����
seed_exp[5800008] = 10
seed_exp[5800012] = 200
seed_exp[5800016] = 200
seed_exp[5800020] = 200
seed_exp[5800024] = 200
seed_exp[5800028] = 200
seed_exp[5800032] = 200
seed_exp[5800036] = 5
seed_exp[5800040] = 5
seed_exp[5800044] = 10
seed_exp[5800048] = 20
seed_exp[5800052] = 50
seed_exp[5800056] = 100
seed_exp[5800060] = 200
seed_exp[5800064] = 300
seed_exp[5800068] = 400
--һ���ջ�
seed_exp[5800009] = 2
seed_exp[5800013] = 40
seed_exp[5800017] = 40
seed_exp[5800021] = 40
seed_exp[5800025] = 40
seed_exp[5800029] = 40
seed_exp[5800033] = 40
seed_exp[5800037] = 1
seed_exp[5800041] = 1
seed_exp[5800045] = 2
seed_exp[5800049] = 4
seed_exp[5800053] = 10
seed_exp[5800057] = 20
seed_exp[5800061] = 40
seed_exp[5800065] = 60
seed_exp[5800069] = 80



--[[����ȫ�ֱ���
mound_point = {}
mound_point["MapID"]={}

mound_point = {}
mound_point.MapID ={}

mound_point = {MapID={}}
]]

--[[ˢ��
function f05_OnInit( MapID , InstanceID )
	local mound = {}
	for i=1 , 5 do
		mound[i] = {x = mound_point[i].x , y = mound_point[i].y , z = mound_point[i].z }
		map.MapCreateCreature(MapID, InstanceID, 5800001, mound[i].x , mound[i].y , mound[i].z)
	end
end
--ע��ũ����ͼ����
aux.RegisterMapEvent("s51", 0, "f05_OnInit")]]


-------------------------------�����������--------------------------------------------------------
function q40053_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local questdata = (zero%1000-zero%100)/100
	if questdata ~= 0 then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero-100)
	end
end

function q40053_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local bRet = 0
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local questdata = (zero%1000-zero%100)/100
	if questdata == 1 then
		bRet = 1
	else
		bRet = 0
	end
	return bRet
end
aux.RegisterQuestEvent(40053, 0, "q40053_OnAccept")
aux.RegisterQuestEvent(40053, 5, "q40053_CheckComplete")

function q40054_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local questdata = (zero%10000-zero%1000)/1000
	if questdata ~= 0 then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero-1000)
	end
end

function q40054_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local bRet = 0
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local questdata = (zero%10000-zero%1000)/1000
	if questdata == 1 then
		bRet = 1
	else
		bRet = 0
	end
	return bRet
end
aux.RegisterQuestEvent(40054, 0, "q40054_OnAccept")
aux.RegisterQuestEvent(40054, 5, "q40054_CheckComplete")
----------------�ͼ�����---------------------------------------------------------------------------------------
function mound_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local owner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local questdata = (zero%1000-zero%100)/100
	local farmnum = plant_data[RoleID]	--��ȡ����ҵ�����ֲ����
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if farmnum == nil then	--���������û����ֲ,��������ֲ������0
		farmnum = 0
	end
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 420001)	--���Ƿ�Ҫ���֣�
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 12)		--���֮������12
		msg.AddMsgEvent(MsgID, 1,  420003)	--����Ҫ��ֲ���֮����
		msg.AddMsgEvent(MsgID, 21, 13)		--����֮������13
		msg.AddMsgEvent(MsgID, 1,  420004)	--����Ҫ��ֲ����֮����
		msg.AddMsgEvent(MsgID, 21, 11)		--Ԫ��֮������11
		msg.AddMsgEvent(MsgID, 1,  420002)	--����Ҫ��ֲԪ��֮����
		msg.AddMsgEvent(MsgID, 21, 14)		--����֮������14
		msg.AddMsgEvent(MsgID, 1,  420005)	--����Ҫ��ֲ����֮����
		msg.AddMsgEvent(MsgID, 21, 15)		--ˮ��֮������15
		msg.AddMsgEvent(MsgID, 1,  420006)	--����Ҫ��ֲˮ��֮����
		msg.AddMsgEvent(MsgID, 21, 16)		--��÷֮������16
		msg.AddMsgEvent(MsgID, 1,  420007)	--����Ҫ��ֲ��÷֮����
		msg.AddMsgEvent(MsgID, 21, 17)		--Ҭ��֮������17
		msg.AddMsgEvent(MsgID, 1,  420008)	--����Ҫ��ֲҬ��֮����
		msg.AddMsgEvent(MsgID, 21, 10)		--ȡ������10
		msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 11 then		--Ԫ��֮��
		local cTime = os.time()
		local s = 0
		if lTime == 0 then
			s = 60*15
		else
			s = os.difftime(cTime,lTime)
		end
		if s >= 60*15 or owner ~= RoleID then
			if farmlevel >= 2 then
				if farmnum < farmlevel then
					seed = 4100101
					local num = role.GetRoleItemNum(RoleID, seed)
					if num >= 1 then
						local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)		--��ȡ��������
						role.RemoveFromRole(MapID, InstanceID, RoleID, seed, 1, 420)	--ɾ������
						map.MapDeleteCreature(MapID, InstanceID, TargetID)				--ɾ������
						local seedmound = map.MapCreateCreature(MapID, InstanceID, 5800007, x, y, z)--��ԭ���Ӵ�ˢ�²�������
						farmnum = farmnum + 1
						cre.SetCreatureScriptData(MapID, InstanceID, seedmound, 1, 1, RoleID)		--��һλ��¼���ֽ�ɫid
						plant_data[RoleID] = farmnum  --����ֲ��������ȫ�ֱ���
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 420010)			--��ʾû������
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420011)			--��ʾ��ֲ��������,������ֲ
					msg.AddMsgEvent(MsgID, 9, farmlevel)		--��ȡũ���ȼ�
					msg.AddMsgEvent(MsgID, 9, farmnum)			--��ȡ��ֲ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420030)			--���ĵȼ����㣬�޷���ֲ
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420012)			--��ʾ������ص���ջ�ʱ�䲻��15����,�޷��ٴ���ֲ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 12 then		--���֮��
		local cTime = os.time()
		local s = 0
		if lTime == 0 then
			s = 60*15
		else
			s = os.difftime(cTime,lTime)
		end
		if s >= 60*15 or owner ~= RoleID then
			if farmlevel >= 1 then
				if farmnum < farmlevel then
					seed = 4100108
					local num = role.GetRoleItemNum(RoleID, seed)
					if num >= 1 then
						local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)		--��¼��������
						role.RemoveFromRole(MapID, InstanceID, RoleID, seed, 1, 420)	--ɾ������
						map.MapDeleteCreature(MapID, InstanceID, TargetID)				--ɾ������
						local seedmound = map.MapCreateCreature(MapID, InstanceID, 5800035, x, y, z)--��ԭ���Ӵ�ˢ�²�������
						farmnum = farmnum + 1
						cre.SetCreatureScriptData(MapID, InstanceID, seedmound, 1, 1, RoleID)		--��һλ��¼���ֽ�ɫid
						plant_data[RoleID] = farmnum  --����ֲ����*10������������
						if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 40053) == true then
							role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 40053, 2)
							if questdata == 0 then
								role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+100)
							else
								role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
							end
						end
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 420010)			--��ʾû������
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420011)			--��ʾ��ֲ��������,������ֲ
					msg.AddMsgEvent(MsgID, 9, farmlevel)		--��ȡũ���ȼ�
					msg.AddMsgEvent(MsgID, 9, farmnum)			--��ȡ��ֲ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420030)			--���ĵȼ����㣬�޷���ֲ
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420012)			--��ʾ������ص���ջ�ʱ�䲻��15����,�޷��ٴ���ֲ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 13 then		--����֮��
		local cTime = os.time()
		local s = 0
		if lTime == 0 then
			s = 60*15
		else
			s = os.difftime(cTime,lTime)
		end
		if s >= 60*15 or owner ~= RoleID then
			if farmlevel >= 1 then
				if farmnum < farmlevel then
					seed = 4100109
					local num = role.GetRoleItemNum(RoleID, seed)
					if num >= 1 then
						local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)		--��¼��������
						role.RemoveFromRole(MapID, InstanceID, RoleID, seed, 1, 420)	--ɾ������
						map.MapDeleteCreature(MapID, InstanceID, TargetID)				--ɾ������
						local seedmound = map.MapCreateCreature(MapID, InstanceID, 5800039, x, y, z)--��ԭ���Ӵ�ˢ�²�������
						farmnum = farmnum + 1
						cre.SetCreatureScriptData(MapID, InstanceID, seedmound, 1, 1, RoleID)		--��һλ��¼���ֽ�ɫid
						plant_data[RoleID] = farmnum --����ֲ����*10������������
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 420010)			--��ʾû������
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420011)			--��ʾ��ֲ��������,������ֲ
					msg.AddMsgEvent(MsgID, 9, farmlevel)		--��ȡũ���ȼ�
					msg.AddMsgEvent(MsgID, 9, farmnum)			--��ȡ��ֲ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID,  1, 420030)			--���ĵȼ����㣬�޷���ֲ
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420012)			--��ʾ������ص���ջ�ʱ�䲻��15����,�޷��ٴ���ֲ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 14 then		--����֮��
		local cTime = os.time()
		local s = 0
		if lTime == 0 then
			s = 60*15
		else
			s = os.difftime(cTime,lTime)
		end
		if s >= 60*15 or owner ~= RoleID then
			if farmlevel >= 2 then
				if farmnum < farmlevel then
					seed = 4100110
					local num = role.GetRoleItemNum(RoleID, seed)
					if num >= 1 then
						local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)		--��¼��������
						role.RemoveFromRole(MapID, InstanceID, RoleID, seed, 1, 420)	--ɾ������
						map.MapDeleteCreature(MapID, InstanceID, TargetID)				--ɾ������
						local seedmound = map.MapCreateCreature(MapID, InstanceID, 5800043, x, y, z)--��ԭ���Ӵ�ˢ�²�������
						farmnum = farmnum + 1
						cre.SetCreatureScriptData(MapID, InstanceID, seedmound, 1, 1, RoleID)		--��һλ��¼���ֽ�ɫid
						plant_data[RoleID] = farmnum	 --����ֲ����*10������������
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 420010)			--��ʾû������
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420011)			--��ʾ��ֲ��������,������ֲ
					msg.AddMsgEvent(MsgID, 9, farmlevel)		--��ȡũ���ȼ�
					msg.AddMsgEvent(MsgID, 9, farmnum)			--��ȡ��ֲ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420030)			--���ĵȼ����㣬�޷���ֲ
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420012)			--��ʾ������ص���ջ�ʱ�䲻��15����,�޷��ٴ���ֲ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 15 then		--ˮ��֮��
		local cTime = os.time()
		local s = 0
		if lTime == 0 then
			s = 60*15
		else
			s = os.difftime(cTime,lTime)
		end
		if s >= 60*15 or owner ~= RoleID then
			if farmlevel >= 3 then
				if farmnum < farmlevel then
					seed = 4100111
					local num = role.GetRoleItemNum(RoleID, seed)
					if num >= 1 then
						local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)		--��¼��������
						role.RemoveFromRole(MapID, InstanceID, RoleID, seed, 1, 420)	--ɾ������
						map.MapDeleteCreature(MapID, InstanceID, TargetID)				--ɾ������
						local seedmound = map.MapCreateCreature(MapID, InstanceID, 5800047, x, y, z)--��ԭ���Ӵ�ˢ�²�������
						farmnum = farmnum + 1
						cre.SetCreatureScriptData(MapID, InstanceID, seedmound, 1, 1, RoleID)		--��һλ��¼���ֽ�ɫid
						plant_data[RoleID] = farmnum  --����ֲ����*10������������
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 420010)			--��ʾû������
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420011)			--��ʾ��ֲ��������,������ֲ
					msg.AddMsgEvent(MsgID, 9, farmlevel)		--��ȡũ���ȼ�
					msg.AddMsgEvent(MsgID, 9, farmnum)			--��ȡ��ֲ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420030)			--���ĵȼ����㣬�޷���ֲ
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420012)			--��ʾ������ص���ջ�ʱ�䲻��15����,�޷��ٴ���ֲ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 16 then		--��÷֮��
		local cTime = os.time()
		local s = 0
		if lTime == 0 then
			s = 60*15
		else
			s = os.difftime(cTime,lTime)
		end
		if s >= 60*15 or owner ~= RoleID then
			if farmlevel >= 4 then
				if farmnum < farmlevel then
					seed = 4100112
					local num = role.GetRoleItemNum(RoleID, seed)
					if num >= 1 then
						local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)		--��¼��������
						role.RemoveFromRole(MapID, InstanceID, RoleID, seed, 1, 420)	--ɾ������
						map.MapDeleteCreature(MapID, InstanceID, TargetID)				--ɾ������
						local seedmound = map.MapCreateCreature(MapID, InstanceID, 5800051, x, y, z)--��ԭ���Ӵ�ˢ�²�������
						farmnum = farmnum + 1
						cre.SetCreatureScriptData(MapID, InstanceID, seedmound, 1, 1, RoleID)		--��һλ��¼���ֽ�ɫid
						plant_data[RoleID] = farmnum 	  --����ֲ����*10������������
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 420010)			--��ʾû������
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420011)			--��ʾ��ֲ��������,������ֲ
					msg.AddMsgEvent(MsgID, 9, farmlevel)		--��ȡũ���ȼ�
					msg.AddMsgEvent(MsgID, 9, farmnum)			--��ȡ��ֲ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420030)			--���ĵȼ����㣬�޷���ֲ
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420012)			--��ʾ������ص���ջ�ʱ�䲻��15����,�޷��ٴ���ֲ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 17 then		--yemi
		local cTime = os.time()
		local s = 0
		if lTime == 0 then
			s = 60*15
		else
			s = os.difftime(cTime,lTime)
		end
		if s >= 60*15 or owner ~= RoleID then
			if farmlevel >= 5 then
				if farmnum < farmlevel then
					seed = 4100113
					local num = role.GetRoleItemNum(RoleID, seed)
					if num >= 1 then
						local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)		--��¼��������
						role.RemoveFromRole(MapID, InstanceID, RoleID, seed, 1, 420)	--ɾ������
						map.MapDeleteCreature(MapID, InstanceID, TargetID)				--ɾ������
						local seedmound = map.MapCreateCreature(MapID, InstanceID, 5800055, x, y, z)--��ԭ���Ӵ�ˢ�²�������
						farmnum = farmnum + 1
						cre.SetCreatureScriptData(MapID, InstanceID, seedmound, 1, 1, RoleID)		--��һλ��¼���ֽ�ɫid
						plant_data[RoleID] = farmnum  --����ֲ����*10������������
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 420010)			--��ʾû������
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420011)			--��ʾ��ֲ��������,������ֲ
					msg.AddMsgEvent(MsgID, 9, farmlevel)		--��ȡũ���ȼ�
					msg.AddMsgEvent(MsgID, 9, farmnum)			--��ȡ��ֲ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420030)			--���ĵȼ����㣬�޷���ֲ
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420012)			--��ʾ������ص���ջ�ʱ�䲻��15����,�޷��ٴ���ֲ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--ע��͵ͼ����ӶԻ�
aux.RegisterCreatureEvent(5800005, 7, "mound_OnTalk")


----------------�߼�����---------------------------------------------------------------------------------------
function mound_advanced_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local owner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local questdata = (zero%1000-zero%100)/100
	local farmnum = plant_data[RoleID]	--��ȡ����ҵ�����ֲ����
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if farmnum == nil then	--���������û����ֲ,��������ֲ������0
		farmnum = 0
	end
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 420001)	--���Ƿ�Ҫ���֣�
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 11)		--Ԫ��֮������11
		msg.AddMsgEvent(MsgID, 1,  420039)	--����Ҫ��ֲԪ��֮����
		msg.AddMsgEvent(MsgID, 21, 12)		--���֮������12
		msg.AddMsgEvent(MsgID, 1,  420040)	--����Ҫ��ֲ���֮����
		msg.AddMsgEvent(MsgID, 21, 13)		--����֮������13
		msg.AddMsgEvent(MsgID, 1,  420041)	--����Ҫ��ֲ����֮����
		msg.AddMsgEvent(MsgID, 21, 14)		--����֮������14
		msg.AddMsgEvent(MsgID, 1,  420042)	--����Ҫ��ֲ����֮����
		msg.AddMsgEvent(MsgID, 21, 15)		--ˮ��֮������15
		msg.AddMsgEvent(MsgID, 1,  420043)	--����Ҫ��ֲˮ��֮����
		msg.AddMsgEvent(MsgID, 21, 16)		--��÷֮������16
		msg.AddMsgEvent(MsgID, 1,  420044)	--����Ҫ��ֲ��÷֮����
		msg.AddMsgEvent(MsgID, 21, 17)		--Ҭ��֮������17
		msg.AddMsgEvent(MsgID, 1,  420045)	--����Ҫ��ֲҬ��֮����
		msg.AddMsgEvent(MsgID, 21, 18)		--Ҭ��֮������17
		msg.AddMsgEvent(MsgID, 1,  420046)	--����Ҫ��ֲҬ��֮����
		msg.AddMsgEvent(MsgID, 21, 19)		--Ҭ��֮������17
		msg.AddMsgEvent(MsgID, 1,  420047)	--����Ҫ��ֲҬ��֮����
		msg.AddMsgEvent(MsgID, 21, 20)		--Ҭ��֮������17
		msg.AddMsgEvent(MsgID, 1,  420048)	--����Ҫ��ֲҬ��֮����
		msg.AddMsgEvent(MsgID, 21, 10)		--ȡ������10
		msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex ~= 10 then		--��δ����
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 420049)	--���Ƿ�Ҫ���֣�
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 10)		--ȡ������10
		msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end
--ע��͸߼����ӶԻ�
aux.RegisterCreatureEvent(5800006, 7, "mound_advanced_OnTalk")


-------------��ˮ-------------------------------------------------------------------------------------------
--Ԫ��֮����ˮ
function yuansu_seed_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
--	cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2, 0)
--	local waternum = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420013)	--��ʾ���Խ�ˮ,�Լ�ˮ����Դ
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 11)		--��ˮ����11
			msg.AddMsgEvent(MsgID, 1,  420014)	--����ˮ��
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420015)	--�ⲻ����Ĳ���,��XXX��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100007)		--���������ϵ�ˮ������
		local cTime = os.time() --��ǰʱ��
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100007, 1, 420)			--ɾ��ˮ
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800008, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--��¼�����������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)				--��¼���������ʱ��
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420017)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--ע��Ͳ������ѶԻ�
aux.RegisterCreatureEvent(5800007, 7, "yuansu_seed_OnTalk")

--���֮����ˮ
function xiangguo_seed_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local questdata = (zero%10000-zero%1000)/1000
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420013)	--��ʾ���Խ�ˮ,�Լ�ˮ����Դ
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 11)		--��ˮ����11
			msg.AddMsgEvent(MsgID, 1,  420014)	--����ˮ��
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420015)	--�ⲻ����Ĳ���,��XXX��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100007)		--���������ϵ�ˮ������
		local cTime = os.time() --��ǰʱ��
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100007, 1, 420)			--ɾ��ˮ
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800036, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--��¼�����������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)				--��¼���������ʱ��
			if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 40054) == true then
				role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 40054, 2)
				if questdata == 0 then
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1000)
				else
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				end
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420017)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--ע��Ͳ������ѶԻ�
aux.RegisterCreatureEvent(5800035, 7, "xiangguo_seed_OnTalk")

--����֮����ˮ
function huotao_seed_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420013)	--��ʾ���Խ�ˮ,�Լ�ˮ����Դ
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 11)		--��ˮ����11
			msg.AddMsgEvent(MsgID, 1,  420014)	--����ˮ��
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420015)	--�ⲻ����Ĳ���,��XXX��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100007)		--���������ϵ�ˮ������
		local cTime = os.time() --��ǰʱ��
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100007, 1, 420)			--ɾ��ˮ
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800040, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--��¼�����������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)				--��¼���������ʱ��
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420017)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--ע��Ͳ������ѶԻ�
aux.RegisterCreatureEvent(5800039, 7, "huotao_seed_OnTalk")

--����֮����ˮ
function yuzi_seed_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420013)	--��ʾ���Խ�ˮ,�Լ�ˮ����Դ
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 11)		--��ˮ����11
			msg.AddMsgEvent(MsgID, 1,  420014)	--����ˮ��
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420015)	--�ⲻ����Ĳ���,��XXX��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100007)		--���������ϵ�ˮ������
		local cTime = os.time() --��ǰʱ��
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100007, 1, 420)			--ɾ��ˮ
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800044, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--��¼�����������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)				--��¼���������ʱ��
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420017)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--ע��Ͳ������ѶԻ�
aux.RegisterCreatureEvent(5800043, 7, "yuzi_seed_OnTalk")

--ˮ��֮����ˮ
function shuijing_seed_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420013)	--��ʾ���Խ�ˮ,�Լ�ˮ����Դ
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 11)		--��ˮ����11
			msg.AddMsgEvent(MsgID, 1,  420014)	--����ˮ��
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420015)	--�ⲻ����Ĳ���,��XXX��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100007)		--���������ϵ�ˮ������
		local cTime = os.time() --��ǰʱ��
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100007, 1, 420)			--ɾ��ˮ
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800048, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--��¼�����������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)				--��¼���������ʱ��
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420017)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--ע��Ͳ������ѶԻ�
aux.RegisterCreatureEvent(5800047, 7, "shuijing_seed_OnTalk")

--��÷֮����ˮ
function zimei_seed_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420013)	--��ʾ���Խ�ˮ,�Լ�ˮ����Դ
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 11)		--��ˮ����11
			msg.AddMsgEvent(MsgID, 1,  420014)	--����ˮ��
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420015)	--�ⲻ����Ĳ���,��XXX��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100007)		--���������ϵ�ˮ������
		local cTime = os.time() --��ǰʱ��
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100007, 1, 420)			--ɾ��ˮ
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800052, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--��¼�����������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)				--��¼���������ʱ��
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420017)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--ע��Ͳ������ѶԻ�
aux.RegisterCreatureEvent(5800051, 7, "zimei_seed_OnTalk")

--Ҭ��֮����ˮ
function yemi_seed_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420013)	--��ʾ���Խ�ˮ,�Լ�ˮ����Դ
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 11)		--��ˮ����11
			msg.AddMsgEvent(MsgID, 1,  420014)	--����ˮ��
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420015)	--�ⲻ����Ĳ���,��XXX��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100007)		--���������ϵ�ˮ������
		local cTime = os.time() --��ǰʱ��
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100007, 1, 420)			--ɾ��ˮ
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800056, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--��¼�����������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)				--��¼���������ʱ��
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420017)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--ע��Ͳ������ѶԻ�
aux.RegisterCreatureEvent(5800055, 7, "yemi_seed_OnTalk")

--------------С����ʧ,ˢ�´���----------------------------------------------------------------------------------------
--Ԫ��
function yuansuseed_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800007, 13, "yuansuseed_On_Disappear")

--���
function xiangguoseed_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800035, 13, "xiangguoseed_On_Disappear")

--����
function huotaoseed_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800039, 13, "huotaoseed_On_Disappear")

--����
function yuziseed_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800043, 13, "yuziseed_On_Disappear")

--ˮ��
function shuijingseed_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800047, 13, "shuijingseed_On_Disappear")

--��÷
function zimeiseed_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800051, 13, "zimeiseed_On_Disappear")

--Ҭ��
function yemiseed_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800055, 13, "yemiseed_On_Disappear")



------------����------------------------------------------------------------------------------------------------
--Ԫ��
function yuansu_sapling_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local cTime = os.time() --��ǰʱ��
	local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local s = os.difftime(cTime,lTime)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
	    if s >= 3600 then
		    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
			local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800009, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
		elseif seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420016)	--����������XXXʱ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 11)		--��ˮ����11
			msg.AddMsgEvent(MsgID, 1,  420018)	--����ܽ���ȵİ�֮Ȫ��
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420019)	--����XXX����,����XXXXʱ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 12)		--��ˮ����12
			msg.AddMsgEvent(MsgID, 1,  420020)	--����Ҫ������ˮ��
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100901)
		local num2 = role.GetRoleItemNum(RoleID, 4100902)
		if num > 0 or num2 > 0 then
			if num2 > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100902, 1, 420)			--ɾ����ˮ
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100901, 1, 420)			--ɾ�����󶨵�ˮ
			end
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800009, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--��¼�����������
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420021)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 12 then
				local num = role.GetRoleItemNum(RoleID, 4100903)
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100903, 1, 420)			--ɾ��ˮ
			if farmlevel < 9 then
				farmexp = farmexp + seed_exp[TargetTypeID]
				if farmexp >= farm_exp[farmlevel] then
					farmexp = farmexp - farm_exp[farmlevel]
					zero = zero + 1
					farmlevel = farmlevel + 1
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
					msg.AddMsgEvent(MsgID, 9, farmlevel)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local ex = farm_exp[farmlevel] - farmexp
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
					msg.AddMsgEvent(MsgID, 9, farmexp)
					msg.AddMsgEvent(MsgID, 9, ex)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420022)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--ע�������˵��
aux.RegisterCreatureEvent(5800008, 7, "yuansu_sapling_OnTalk")

--���
function xiangguo_sapling_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local cTime = os.time() --��ǰʱ��
	local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local s = os.difftime(cTime,lTime)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
	    if s >= 3600 then
		    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
			local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800037, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
		elseif seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420016)	--����������XXXʱ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 11)		--��ˮ����11
			msg.AddMsgEvent(MsgID, 1,  420018)	--����ܽ���ȵİ�֮Ȫ��
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420019)	--����XXX����,����XXXXʱ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 12)		--��ˮ����12
			msg.AddMsgEvent(MsgID, 1,  420020)	--����Ҫ������ˮ��
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100901)
		local num2 = role.GetRoleItemNum(RoleID, 4100902)
		if num > 0 or num2 > 0 then
			if num2 > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100902, 1, 420)			--ɾ����ˮ
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100901, 1, 420)			--ɾ�����󶨵�ˮ
			end
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800037, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--��¼�����������
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420021)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 12 then
		local num = role.GetRoleItemNum(RoleID, 4100903)
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100903, 1, 420)			--ɾ��ˮ
			if farmlevel < 9 then
				farmexp = farmexp + seed_exp[TargetTypeID]
				if farmexp >= farm_exp[farmlevel] then
					farmexp = farmexp - farm_exp[farmlevel]
					zero = zero + 1
					farmlevel = farmlevel + 1
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
					msg.AddMsgEvent(MsgID, 9, farmlevel)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local ex = farm_exp[farmlevel] - farmexp
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
					msg.AddMsgEvent(MsgID, 9, farmexp)
					msg.AddMsgEvent(MsgID, 9, ex)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420022)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--ע�������˵��
aux.RegisterCreatureEvent(5800036, 7, "xiangguo_sapling_OnTalk")

--����
function huotao_sapling_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local cTime = os.time() --��ǰʱ��
	local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local s = os.difftime(cTime,lTime)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if s >= 3600 then
		    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
			local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800041, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
		elseif seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420016)	--����������XXXʱ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 11)		--��ˮ����11
			msg.AddMsgEvent(MsgID, 1,  420018)	--����ܽ���ȵİ�֮Ȫ��
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420019)	--����XXX����,����XXXXʱ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 12)		--��ˮ����12
			msg.AddMsgEvent(MsgID, 1,  420020)	--����Ҫ������ˮ��
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100901)
		local num2 = role.GetRoleItemNum(RoleID, 4100902)
		if num > 0 or num2 > 0 then
			if num2 > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100902, 1, 420)			--ɾ����ˮ
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100901, 1, 420)			--ɾ�����󶨵�ˮ
			end
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800041, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--��¼�����������
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420021)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 12 then
				local num = role.GetRoleItemNum(RoleID, 4100903)
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100903, 1, 420)			--ɾ��ˮ
			if farmlevel < 9 then
				farmexp = farmexp + seed_exp[TargetTypeID]
				if farmexp >= farm_exp[farmlevel] then
					farmexp = farmexp - farm_exp[farmlevel]
					zero = zero + 1
					farmlevel = farmlevel + 1
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
					msg.AddMsgEvent(MsgID, 9, farmlevel)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local ex = farm_exp[farmlevel] - farmexp
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
					msg.AddMsgEvent(MsgID, 9, farmexp)
					msg.AddMsgEvent(MsgID, 9, ex)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420022)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--ע�������˵��
aux.RegisterCreatureEvent(5800040, 7, "huotao_sapling_OnTalk")

--����
function yuzi_sapling_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local cTime = os.time() --��ǰʱ��
	local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local s = os.difftime(cTime,lTime)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
	    if s >= 3600 then
		    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
			local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800045, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
		elseif seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420016)	--����������XXXʱ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 11)		--��ˮ����11
			msg.AddMsgEvent(MsgID, 1,  420018)	--����ܽ���ȵİ�֮Ȫ��
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420019)	--����XXX����,����XXXXʱ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 12)		--��ˮ����12
			msg.AddMsgEvent(MsgID, 1,  420020)	--����Ҫ������ˮ��
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100901)
		local num2 = role.GetRoleItemNum(RoleID, 4100902)
		if num > 0 or num2 > 0 then
			if num2 > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100902, 1, 420)			--ɾ����ˮ
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100901, 1, 420)			--ɾ�����󶨵�ˮ
			end
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800045, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--��¼�����������
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420021)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 12 then
				local num = role.GetRoleItemNum(RoleID, 4100903)
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100903, 1, 420)			--ɾ��ˮ
			if farmlevel < 9 then
				farmexp = farmexp + seed_exp[TargetTypeID]
				if farmexp >= farm_exp[farmlevel] then
					farmexp = farmexp - farm_exp[farmlevel]
					zero = zero + 1
					farmlevel = farmlevel + 1
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
					msg.AddMsgEvent(MsgID, 9, farmlevel)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local ex = farm_exp[farmlevel] - farmexp
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
					msg.AddMsgEvent(MsgID, 9, farmexp)
					msg.AddMsgEvent(MsgID, 9, ex)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420022)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--ע�������˵��
aux.RegisterCreatureEvent(5800044, 7, "yuzi_sapling_OnTalk")

--ˮ��
function shuijing_sapling_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local cTime = os.time() --��ǰʱ��
	local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local s = os.difftime(cTime,lTime)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
	    if s >= 3600 then
		    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
			local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800049, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
		elseif seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420016)	--����������XXXʱ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 11)		--��ˮ����11
			msg.AddMsgEvent(MsgID, 1,  420018)	--����ܽ���ȵİ�֮Ȫ��
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420019)	--����XXX����,����XXXXʱ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 12)		--��ˮ����12
			msg.AddMsgEvent(MsgID, 1,  420020)	--����Ҫ������ˮ��
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100901)
		local num2 = role.GetRoleItemNum(RoleID, 4100902)
		if num > 0 or num2 > 0 then
			if num2 > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100902, 1, 420)			--ɾ����ˮ
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100901, 1, 420)			--ɾ�����󶨵�ˮ
			end
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800049, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--��¼�����������
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420021)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 12 then
				local num = role.GetRoleItemNum(RoleID, 4100903)
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100903, 1, 420)			--ɾ��ˮ
			if farmlevel < 9 then
				farmexp = farmexp + seed_exp[TargetTypeID]
				if farmexp >= farm_exp[farmlevel] then
					farmexp = farmexp - farm_exp[farmlevel]
					zero = zero + 1
					farmlevel = farmlevel + 1
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
					msg.AddMsgEvent(MsgID, 9, farmlevel)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local ex = farm_exp[farmlevel] - farmexp
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
					msg.AddMsgEvent(MsgID, 9, farmexp)
					msg.AddMsgEvent(MsgID, 9, ex)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420022)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--ע�������˵��
aux.RegisterCreatureEvent(5800048, 7, "shuijing_sapling_OnTalk")

--��÷
function zimei_sapling_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local cTime = os.time() --��ǰʱ��
	local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local s = os.difftime(cTime,lTime)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
	    if s >= 3600 then
		    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
			local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800053, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
		elseif seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420016)	--����������XXXʱ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 11)		--��ˮ����11
			msg.AddMsgEvent(MsgID, 1,  420018)	--����ܽ���ȵİ�֮Ȫ��
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420019)	--����XXX����,����XXXXʱ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 12)		--��ˮ����12
			msg.AddMsgEvent(MsgID, 1,  420020)	--����Ҫ������ˮ��
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100901)
		local num2 = role.GetRoleItemNum(RoleID, 4100902)
		if num > 0 or num2 > 0 then
			if num2 > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100902, 1, 420)			--ɾ����ˮ
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100901, 1, 420)			--ɾ�����󶨵�ˮ
			end
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800053, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--��¼�����������
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420021)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 12 then
				local num = role.GetRoleItemNum(RoleID, 4100903)
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100903, 1, 420)			--ɾ��ˮ
			if farmlevel < 9 then
				farmexp = farmexp + seed_exp[TargetTypeID]
				if farmexp >= farm_exp[farmlevel] then
					farmexp = farmexp - farm_exp[farmlevel]
					zero = zero + 1
					farmlevel = farmlevel + 1
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
					msg.AddMsgEvent(MsgID, 9, farmlevel)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local ex = farm_exp[farmlevel] - farmexp
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
					msg.AddMsgEvent(MsgID, 9, farmexp)
					msg.AddMsgEvent(MsgID, 9, ex)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420022)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--ע�������˵��
aux.RegisterCreatureEvent(5800052, 7, "zimei_sapling_OnTalk")

--Ҭ��
function yemi_sapling_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local cTime = os.time() --��ǰʱ��
	local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local s = os.difftime(cTime,lTime)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
	    if s >= 3600 then
		    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
			local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800057, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
		elseif seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420016)	--����������XXXʱ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 11)		--��ˮ����11
			msg.AddMsgEvent(MsgID, 1,  420018)	--����ܽ���ȵİ�֮Ȫ��
			msg.AddMsgEvent(MsgID, 21, 10)		--���ܽ�ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420019)	--����XXX����,����XXXXʱ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 12)		--��ˮ����12
			msg.AddMsgEvent(MsgID, 1,  420020)	--����Ҫ������ˮ��
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100901)
		local num2 = role.GetRoleItemNum(RoleID, 4100902)
		if num > 0 or num2 > 0 then
			if num2 > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100902, 1, 420)			--ɾ����ˮ
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100901, 1, 420)			--ɾ�����󶨵�ˮ
			end
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800057, x, y, z)		--ˢ������
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--��¼�����������
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420021)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 12 then
				local num = role.GetRoleItemNum(RoleID, 4100903)
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100903, 1, 420)			--ɾ��ˮ
			if farmlevel < 9 then
				farmexp = farmexp + seed_exp[TargetTypeID]
				if farmexp >= farm_exp[farmlevel] then
					farmexp = farmexp - farm_exp[farmlevel]
					zero = zero + 1
					farmlevel = farmlevel + 1
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
					msg.AddMsgEvent(MsgID, 9, farmlevel)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local ex = farm_exp[farmlevel] - farmexp
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
					msg.AddMsgEvent(MsgID, 9, farmexp)
					msg.AddMsgEvent(MsgID, 9, ex)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420022)	--��ʾû��ˮ,���ܽ�ˮ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--ע�������˵��
aux.RegisterCreatureEvent(5800056, 7, "yemi_sapling_OnTalk")

--------------С����ʧ,ˢ�´���----------------------------------------------------------------------------------------
--Ԫ��
function yuansu_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800009, x, y, z)		--ˢ������
	cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420023)--����XXX������,��ǰ����ժ��
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)
end
aux.RegisterCreatureEvent(5800008, 13, "yuansu_On_Disappear")

--���
function xiangguo_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800037, x, y, z)		--ˢ������
	cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420023)--����XXX������,��ǰ����ժ��
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)
end
aux.RegisterCreatureEvent(5800036, 13, "xiangguo_On_Disappear")

--����
function huotao_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800041, x, y, z)		--ˢ������
	cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420023)--����XXX������,��ǰ����ժ��
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)
end
aux.RegisterCreatureEvent(5800040, 13, "huotao_On_Disappear")

--����
function yuzi_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800045, x, y, z)		--ˢ������
	cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420023)--����XXX������,��ǰ����ժ��
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)
end
aux.RegisterCreatureEvent(5800044, 13, "yuzi_On_Disappear")

--ˮ��
function shuijing_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800049, x, y, z)		--ˢ������
	cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420023)--����XXX������,��ǰ����ժ��
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)
end
aux.RegisterCreatureEvent(5800048, 13, "shuijing_On_Disappear")

--��÷
function zimei_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800053, x, y, z)		--ˢ������
	cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420023)--����XXX������,��ǰ����ժ��
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)
end
aux.RegisterCreatureEvent(5800052, 13, "zimei_On_Disappear")

--Ҭ��
function yemi_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800057, x, y, z)		--ˢ������
	cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420023)--����XXX������,��ǰ����ժ��
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)
end
aux.RegisterCreatureEvent(5800056, 13, "yemi_On_Disappear")

------------��һ���ջ�------------------------------------------------------------------------------------------------
--Ԫ��
function yuansu_first_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--����XXX�Ѿ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--�ջ񷵻�11
			msg.AddMsgEvent(MsgID, 1,  420025)	--���ջ�
			msg.AddMsgEvent(MsgID, 21, 10)		--���ջ񷵻�10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420026)	--����XXX����,�Ƿ���͵ȡ�ջ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 12)		--͵ժ����12
			msg.AddMsgEvent(MsgID, 1,  420025)	--��͵ժ��
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local a = math.random(1,200)
		if a <= 199 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400001, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400001, 1, -1, 8, 102)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400003, 1, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800010, x, y, z)	--ˢ�µڶ��ڳ���
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	elseif TalkIndex == 12 then
		local a = math.random(1,200)
				if a <= 199 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400001, 1, -1, 8, 102)
		else a = 200
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400001, 1, -1, 8, 102)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400003, 1, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800010, x, y, z)	--ˢ�µڶ��ڳ���
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 420027)--����XXXX��XXX͵����һ���֣�ʣ�µĹ�ʵ����1Сʱ����ʧ,�뼰ʱ��ժ��
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(seedowner, MsgID)
	end
end
aux.RegisterCreatureEvent(5800009, 7, "yuansu_first_OnTalk")

--���
function xiangguo_first_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--����XXX�Ѿ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--�ջ񷵻�11
			msg.AddMsgEvent(MsgID, 1,  420025)	--���ջ�
			msg.AddMsgEvent(MsgID, 21, 10)		--���ջ񷵻�10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420026)	--����XXX����,�Ƿ���͵ȡ�ջ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 12)		--͵ժ����12
			msg.AddMsgEvent(MsgID, 1,  420025)	--��͵ժ��
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100201, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100201, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800038, x, y, z)	--ˢ�µڶ��ڳ���
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	elseif TalkIndex == 12 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100201, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100201, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800038, x, y, z)	--ˢ�µڶ��ڳ���
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 420027)--����XXXX��XXX͵���ˣ�ʣ�µĹ�ʵ����1Сʱ����ʧ,�뼰ʱ��ժ��
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(seedowner, MsgID)
	end
end
aux.RegisterCreatureEvent(5800037, 7, "xiangguo_first_OnTalk")

--����
function huotao_first_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--����XXX�Ѿ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--�ջ񷵻�11
			msg.AddMsgEvent(MsgID, 1,  420025)	--���ջ�
			msg.AddMsgEvent(MsgID, 21, 10)		--���ջ񷵻�10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420026)	--����XXX����,�Ƿ���͵ȡ�ջ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 12)		--͵ժ����12
			msg.AddMsgEvent(MsgID, 1,  420025)	--��͵ժ��
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100202, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100202, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800042, x, y, z)	--ˢ�µڶ��ڳ���
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	elseif TalkIndex == 12 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100202, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100202, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800042, x, y, z)	--ˢ�µڶ��ڳ���
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 420027)--����XXXX��XXX͵���ˣ�ʣ�µĹ�ʵ����1Сʱ����ʧ,�뼰ʱ��ժ��
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(seedowner, MsgID)
	end
end
aux.RegisterCreatureEvent(5800041, 7, "huotao_first_OnTalk")

--����
function yuzi_first_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--����XXX�Ѿ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--�ջ񷵻�11
			msg.AddMsgEvent(MsgID, 1,  420025)	--���ջ�
			msg.AddMsgEvent(MsgID, 21, 10)		--���ջ񷵻�10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420026)	--����XXX����,�Ƿ���͵ȡ�ջ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 12)		--͵ժ����12
			msg.AddMsgEvent(MsgID, 1,  420025)	--��͵ժ��
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100203, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100203, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800046, x, y, z)	--ˢ�µڶ��ڳ���
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	elseif TalkIndex == 12 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100203, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100203, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800046, x, y, z)	--ˢ�µڶ��ڳ���
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 420027)--����XXXX��XXX͵���ˣ�ʣ�µĹ�ʵ����1Сʱ����ʧ,�뼰ʱ��ժ��
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(seedowner, MsgID)
	end
end
aux.RegisterCreatureEvent(5800045, 7, "yuzi_first_OnTalk")

--ˮ��
function shuijing_first_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--����XXX�Ѿ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--�ջ񷵻�11
			msg.AddMsgEvent(MsgID, 1,  420025)	--���ջ�
			msg.AddMsgEvent(MsgID, 21, 10)		--���ջ񷵻�10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420026)	--����XXX����,�Ƿ���͵ȡ�ջ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 12)		--͵ժ����12
			msg.AddMsgEvent(MsgID, 1,  420025)	--��͵ժ��
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100204, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100204, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800050, x, y, z)	--ˢ�µڶ��ڳ���
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	elseif TalkIndex == 12 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100204, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100204, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800050, x, y, z)	--ˢ�µڶ��ڳ���
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 420027)--����XXXX��XXX͵���ˣ�ʣ�µĹ�ʵ����1Сʱ����ʧ,�뼰ʱ��ժ��
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(seedowner, MsgID)
	end
end
aux.RegisterCreatureEvent(5800049, 7, "shuijing_first_OnTalk")

--��÷
function zimei_first_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--����XXX�Ѿ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--�ջ񷵻�11
			msg.AddMsgEvent(MsgID, 1,  420025)	--���ջ�
			msg.AddMsgEvent(MsgID, 21, 10)		--���ջ񷵻�10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420026)	--����XXX����,�Ƿ���͵ȡ�ջ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 12)		--͵ժ����12
			msg.AddMsgEvent(MsgID, 1,  420025)	--��͵ժ��
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100205, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100205, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800054, x, y, z)	--ˢ�µڶ��ڳ���
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	elseif TalkIndex == 12 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100205, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100205, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800054, x, y, z)	--ˢ�µڶ��ڳ���
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 420027)--����XXXX��XXX͵���ˣ�ʣ�µĹ�ʵ����1Сʱ����ʧ,�뼰ʱ��ժ��
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(seedowner, MsgID)
	end
end
aux.RegisterCreatureEvent(5800053, 7, "zimei_first_OnTalk")


--Ҭ�׹�
function yemi_first_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--����XXX�Ѿ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--�ջ񷵻�11
			msg.AddMsgEvent(MsgID, 1,  420025)	--���ջ�
			msg.AddMsgEvent(MsgID, 21, 10)		--���ջ񷵻�10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420026)	--����XXX����,�Ƿ���͵ȡ�ջ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 12)		--͵ժ����12
			msg.AddMsgEvent(MsgID, 1,  420025)	--��͵ժ��
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100206, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100206, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800058, x, y, z)	--ˢ�µڶ��ڳ���
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	elseif TalkIndex == 12 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100206, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100206, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800058, x, y, z)	--ˢ�µڶ��ڳ���
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--��¼�����������
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 420027)--����XXXX��XXX͵���ˣ�ʣ�µĹ�ʵ����1Сʱ����ʧ,�뼰ʱ��ժ��
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(seedowner, MsgID)
	end
end
aux.RegisterCreatureEvent(5800057, 7, "yemi_first_OnTalk")

---------------------------------����ڶ��ڲ�ժ--------------------------------------------------
--Ԫ��
function yuansu_second_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[RoleID]
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--����XXX�Ѿ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--�ջ񷵻�11
			msg.AddMsgEvent(MsgID, 1,  420025)	--���ջ�
			msg.AddMsgEvent(MsgID, 21, 10)		--���ջ񷵻�10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420028)	--����XXX����,�޷��ջ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local cTime = os.time()					--��¼ʱ��,Ϊ�Ժ����ֲcd��׼��
		local a = math.random(1,1000)
		if a <= 300 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400001, 2, -1, 8, 102)
		elseif a <= 860 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400002, 1, -1, 8, 102)
		elseif a <= 963 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400003, 1, -1, 8, 102)
		elseif a <= 983 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400004, 1, -1, 8, 102)
		elseif a <= 988 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400005, 1, -1, 8, 102)
		elseif a <= 990 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100107, 1, -1, 8, 102)
		elseif a <= 992 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100106, 1, -1, 8, 102)
		elseif a <= 994 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100105, 1, -1, 8, 102)
		elseif a <= 996 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100104, 1, -1, 8, 102)
		elseif a <= 998 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100103, 1, -1, 8, 102)
		elseif a <= 1000 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100102, 1, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)				--��¼�����������
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)
		farmnum = farmnum - 1
		if farmnum < 1 then
			plant_data[RoleID] = nil
		else
			plant_data[RoleID] = farmnum
		end
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5800010, 7, "yuansu_second_OnTalk")

--���
function xiangguo_second_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[RoleID]
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--����XXX�Ѿ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--�ջ񷵻�11
			msg.AddMsgEvent(MsgID, 1,  420025)	--���ջ�
			msg.AddMsgEvent(MsgID, 21, 10)		--���ջ񷵻�10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420028)	--����XXX����,�޷��ջ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local cTime = os.time()					--��¼ʱ��,Ϊ�Ժ����ֲcd��׼��
		local a = math.random(1,1000)
		if a <= 600 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100201, 5, -1, 8, 102)
		elseif a <= 1000 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100201, 5, -1, 8, 102)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100108, 1, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)				--��¼�����������
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)
		farmnum = farmnum - 1
		if farmnum < 1 then
			plant_data[RoleID] = nil
		else
			plant_data[RoleID] = farmnum
		end
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5800038, 7, "xiangguo_second_OnTalk")

--����
function huotao_second_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[RoleID]
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--����XXX�Ѿ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--�ջ񷵻�11
			msg.AddMsgEvent(MsgID, 1,  420025)	--���ջ�
			msg.AddMsgEvent(MsgID, 21, 10)		--���ջ񷵻�10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420028)	--����XXX����,�޷��ջ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local cTime = os.time()					--��¼ʱ��,Ϊ�Ժ����ֲcd��׼��
		local a = math.random(1,1000)
		if a <= 600 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100202, 5, -1, 8, 102)
		elseif a <= 1000 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100202, 5, -1, 8, 102)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100109, 1, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)				--��¼�����������
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)
		farmnum = farmnum - 1
		if farmnum < 1 then
			plant_data[RoleID] = nil
		else
			plant_data[RoleID] = farmnum
		end
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5800042, 7, "huotao_second_OnTalk")

--����
function yuzi_second_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[RoleID]
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--����XXX�Ѿ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--�ջ񷵻�11
			msg.AddMsgEvent(MsgID, 1,  420025)	--���ջ�
			msg.AddMsgEvent(MsgID, 21, 10)		--���ջ񷵻�10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420028)	--����XXX����,�޷��ջ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local cTime = os.time()					--��¼ʱ��,Ϊ�Ժ����ֲcd��׼��
		local a = math.random(1,1000)
		if a <= 600 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100203, 5, -1, 8, 102)
		elseif a <= 1000 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100203, 5, -1, 8, 102)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100110, 1, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)				--��¼�����������
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)
		farmnum = farmnum - 1
		if farmnum < 1 then
			plant_data[RoleID] = nil
		else
			plant_data[RoleID] = farmnum
		end
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5800046, 7, "yuzi_second_OnTalk")

--ˮ��
function shuijing_second_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[RoleID]
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--����XXX�Ѿ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--�ջ񷵻�11
			msg.AddMsgEvent(MsgID, 1,  420025)	--���ջ�
			msg.AddMsgEvent(MsgID, 21, 10)		--���ջ񷵻�10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420028)	--����XXX����,�޷��ջ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local cTime = os.time()					--��¼ʱ��,Ϊ�Ժ����ֲcd��׼��
		local a = math.random(1,1000)
		if a <= 600 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100204, 5, -1, 8, 102)
		elseif a <= 1000 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100204, 5, -1, 8, 102)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100111, 1, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)				--��¼�����������
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)
		farmnum = farmnum - 1
		if farmnum < 1 then
			plant_data[RoleID] = nil
		else
			plant_data[RoleID] = farmnum
		end
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5800050, 7, "shuijing_second_OnTalk")

--��÷
function zimei_second_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[RoleID]
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--����XXX�Ѿ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--�ջ񷵻�11
			msg.AddMsgEvent(MsgID, 1,  420025)	--���ջ�
			msg.AddMsgEvent(MsgID, 21, 10)		--���ջ񷵻�10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420028)	--����XXX����,�޷��ջ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local cTime = os.time()					--��¼ʱ��,Ϊ�Ժ����ֲcd��׼��
		local a = math.random(1,1000)
		if a <= 600 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100205, 5, -1, 8, 102)
		elseif a <= 1000 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100205, 5, -1, 8, 102)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100112, 1, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)				--��¼�����������
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)
		farmnum = farmnum - 1
		if farmnum < 1 then
			plant_data[RoleID] = nil
		else
			plant_data[RoleID] = farmnum
		end
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5800054, 7, "zimei_second_OnTalk")

--Ҭ��
function yemi_second_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[RoleID]
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--����XXX�Ѿ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--�ջ񷵻�11
			msg.AddMsgEvent(MsgID, 1,  420025)	--���ջ�
			msg.AddMsgEvent(MsgID, 21, 10)		--���ջ񷵻�10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420028)	--����XXX����,�޷��ջ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--����ˮ����10
			msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local cTime = os.time()					--��¼ʱ��,Ϊ�Ժ����ֲcd��׼��
		local a = math.random(1,1000)
		if a <= 600 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100206, 5, -1, 8, 102)
		elseif a <= 1000 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100206, 5, -1, 8, 102)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100113, 1, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)			--ɾ�����ֵ�����
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)				--��¼�����������
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)
		farmnum = farmnum - 1
		if farmnum < 1 then
			plant_data[RoleID] = nil
		else
			plant_data[RoleID] = farmnum
		end
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5800058, 7, "yemi_second_OnTalk")

-----------ʱ�䵽���Լ���ʧ,����ˢ������ֲ��-------------------------------------------------------------------
--Ԫ��
function yuansutree_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
--[[local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420029)--����XXX�ѹ���,�޷��ٴ��ջ�
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)]]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800010, 13, "yuansutree_On_Disappear")

--���
function xiangguotree_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
--[[local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420029)--����XXX�ѹ���,�޷��ٴ��ջ�
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)]]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800038, 13, "xiangguotree_On_Disappear")

--huotao
function huotaotree_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
--[[local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420029)--����XXX�ѹ���,�޷��ٴ��ջ�
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)]]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800042, 13, "huotaotree_On_Disappear")

--yuzi
function yuzitree_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
--[[local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420029)--����XXX�ѹ���,�޷��ٴ��ջ�
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)]]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800046, 13, "yuzitree_On_Disappear")

--shuijing
function shuijingtree_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
--[[local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420029)--����XXX�ѹ���,�޷��ٴ��ջ�
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)]]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800050, 13, "shuijingtree_On_Disappear")

--zimei
function zimeitree_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
--[[local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420029)--����XXX�ѹ���,�޷��ٴ��ջ�
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)]]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800054, 13, "zimeitree_On_Disappear")

--yemi
function yemitree_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--ˢ������
--[[local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420029)--����XXX�ѹ���,�޷��ٴ��ջ�
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)]]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800058, 13, "yemitree_On_Disappear")
-----------------------��ѯũ���ȼ�-----------------------------------------------
function guanli_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
	end
	if farmlevel == 9 then
		farmexp = 0
	end
	local lexp = farm_exp[farmlevel] - farmexp
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 420034)	--���ã���������ũ���Ĺ���Ա���������������Լ���ũ�����ԣ������ҾͶ��ˣ�
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 11)		--�鿴ũ���ȼ�����11
		msg.AddMsgEvent(MsgID, 1,  420035)	--���鿴ũ���ȼ���
		msg.AddMsgEvent(MsgID, 21, 12)		--�鿴����ֲ�����ӷ���12
		msg.AddMsgEvent(MsgID, 1,  420036)	--�鿴����ֲ������
		msg.AddMsgEvent(MsgID, 21, 13)		--�鿴����ֲ�����ӷ���12
		msg.AddMsgEvent(MsgID, 1,  420050)	--�鿴����ֲ������
		msg.AddMsgEvent(MsgID, 21, 10)		--ȡ������10
		msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 11 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 420037)	--����ũ���ȼ�Ϊx����ǰ�ѻ���XX���飬��ʣXXX���鼴������
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 9, farmlevel)
		msg.AddMsgEvent(MsgID, 9, farmexp)
		msg.AddMsgEvent(MsgID, 9, lexp)
		msg.AddMsgEvent(MsgID, 21, 10)		--ȡ������10
		msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 12 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 420038)	--����ũ���ȼ�ΪX��������ֲ����ֲ�
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 10)		--ȡ������10
		msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 13 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 420051)	--����ũ���ȼ�ΪX��������ֲ����ֲ�
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 10)		--ȡ������10
		msg.AddMsgEvent(MsgID, 1,  420009)	--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end
aux.RegisterCreatureEvent(5800086, 7, "guanli_OnTalk")

--------------------��������----------------------------------------------------
function s4100904_Use(MapID, InstanceID, TypeID, RoleID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end

	if farmlevel < 9 then
		farmexp = farmexp + 10
		if farmexp >= farm_exp[farmlevel] then
			farmexp = farmexp - farm_exp[farmlevel]
			zero = zero + 1
			farmlevel = farmlevel + 1
			role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420031)--��ϲ�㣬����ũ���ȼ�����X������ȥũ������Ա�������µȼ�ũ��Ȩ��
			msg.AddMsgEvent(MsgID, 9, farmlevel)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local ex = farm_exp[farmlevel] - farmexp
			role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420032)--����ũ��������۵�XX������XX����
			msg.AddMsgEvent(MsgID, 9, farmexp)
			msg.AddMsgEvent(MsgID, 9, ex)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 420033)--����ũ���ȼ��Ѿ�������ߣ��޷����ũ������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end
aux.RegisterItemEvent(4100904, 1, "s4100904_Use")

