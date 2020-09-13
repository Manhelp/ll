
YouXiWenDa_Attend = {}				-- �Ƿ��ڴ˻ʱ���ڲμӹ�
YouXiWenDa_Questions = {}				-- ������
YouXiWenDa_RightAnswer = {}			-- ��ȷ�����ĸ�ѡ���е�����
YouXiWenDa_Normal_Question_Point = 10	-- ��ͨ��Ŀ�ķ���
YouXiWenDa_Hard_Question_Point = 0 	-- ����������Ŀ�ķ���
YouXiWenDa_Time = {}					-- ��Ŵ��⿪ʼʱ��ϵͳʱ�䣬��Ӧ��ɫid
YouXiWenDa_Role_Point = {}			-- ��һ�õķ�������Ӧ��ɫid
YouXiWenDa_Stepindex = {}				-- ÿ����ҵĻ����
YouXiWenDa_Question_Type = {}			-- ��ǰ��Ŀ���ͣ��������ѻ������ͨ
YouXiWenDa_Role_Level = {}			-- ��ҵ�ǰ�ȼ�
YouXiWenDa_Time_Counter = 45			-- ����ĵ���ʱ
YouXiWenDa_Current_Question = {}      -- ��ҵ�ǰ����

YouXiWenDa_Hard_Question_Num = 0	-- �������ѵ���Ŀ��
YouXiWenDa_Normal_Question_Num = 77	-- ��ͨ����Ŀ��

WD_CallDeep = 0							-- �ݹ�������


YouXiWenDa_Hard_Question_Table = {}
YouXiWenDa_Normal_Question_Table = {}

g_WD_RoleNum = 0						-- �μ�֪ʶ���´�������Ҹ���
tbl_WD_RoleID = {}						-- ��g_WD_RoleNumΪ������Ӧ��ҵ�RoleID
tbl_WD_Index = {}						-- ��RoleIDΪ������Ӧ��ҵĴ���Index
tbl_WD_Questions = {}					-- ����Ѿ��ش������Ŀ

-- npc�Ի��������������
function n5510302_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		-- ��ʼ���ձ�
		YouXiWenDa_InitNullTable(RoleID)
	end

	-- �õ�������
	YouXiWenDa_Role_Level[tbl_WD_Index[RoleID]] = role.GetRoleLevel(MapID, InstanceID, RoleID)

	if YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 205001)	-- "���Ƿ�����μ���Ϸ֪ʶ�ʴ���"
		msg.AddMsgEvent(MsgID, 21, 4)		-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 100001)	-- "ȷ��"
		msg.AddMsgEvent(MsgID, 21, 5)		-- ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100002)	-- "ȡ��"
		msg.AddMsgEvent(MsgID, 24, TargetID)
		YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] = 2
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] == 2 then
		if TalkIndex == 4 then			-- ѡ����ȷ���μӰ�ť��
			if YouXiWenDa_Role_Level[tbl_WD_Index[RoleID]] < 10 or YouXiWenDa_Role_Level[tbl_WD_Index[RoleID]] > 30 then	-- �ȼ�����10���Ļ�������ʾ
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 205005)		-- "10�����Ͽ��Բ��롣"
				msg.AddMsgEvent(MsgID, 21, 4)			-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001)		-- "ȷ��"
				YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] = 1
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			elseif YouXiWenDa_Attend[tbl_WD_Index[RoleID]] == 1 then	-- �ڴ˻ʱ�����Ѳμӹ�
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 205002)		-- "���Ѳμӹ�����֪ʶ���»���ɼ����μ��¸�ʱ��Ļ��"
				msg.AddMsgEvent(MsgID, 21, 4)			-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001)		-- "ȷ��"
				YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] = 1
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			else
				-- ���������ڿ�ʼ��
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 205003)		--�������ڿ�ʼ!
				msg.AddMsgEvent(MsgID, 21, 4)
				msg.AddMsgEvent(MsgID, 1, 100001)
				msg.AddMsgEvent(MsgID, 24, TargetID)
				YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] = 3
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				YouXiWenDa_Attend[tbl_WD_Index[RoleID]]	= 1			-- �Ѳμӹ����λ
				YouXiWenDa_Current_Question[tbl_WD_Index[RoleID]] = {question, index}
			end
		elseif TalkIndex == 5 then
			YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] = 1
		else
			YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] = 1
		end


	elseif YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] == 3 then				-- ���⿪ʼ
		YouXiWenDa_GenerateQuestion(RoleID, TargetID)
		YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] = 4


	elseif YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] == 4 then	-- �����������ʱ��
		if TalkIndex == 5 then						-- �����������ҵ�ǰ�÷�Ӧ�õĽ���
			YouXiWenDa_RewardRole(MapID, InstanceID, RoleID)
		else
			-- ������⻨�ѵ�ʱ��
			local time1 = YouXiWenDa_Time[tbl_WD_Index[RoleID]]
			local dtime = os.difftime(os.time(), time1)

			-- �жϴ��Ƿ���ȷ
			if YouXiWenDa_RightAnswer[tbl_WD_Index[RoleID]] == TalkIndex then
				-- �ش�������ʱ�䲻ͬ���÷ֲ�ͬ
				if dtime <= 45 then				-- 45���ڻش����ȷ�𰸣������Ŀ4����
					if YouXiWenDa_Question_Type[tbl_WD_Index[RoleID]] == 1 then  	-- �������ѵ÷�
						YouXiWenDa_Role_Point[tbl_WD_Index[RoleID]] = YouXiWenDa_Role_Point[tbl_WD_Index[RoleID]] + YouXiWenDa_Hard_Question_Point
					else							-- ������ͨ�÷�
						YouXiWenDa_Role_Point[tbl_WD_Index[RoleID]] = YouXiWenDa_Role_Point[tbl_WD_Index[RoleID]] + YouXiWenDa_Normal_Question_Point
					end
				else
					if YouXiWenDa_Question_Type[tbl_WD_Index[RoleID]] == 1 then  	-- �������ѵ÷�
						YouXiWenDa_Role_Point[tbl_WD_Index[RoleID]] = YouXiWenDa_Role_Point[tbl_WD_Index[RoleID]] + YouXiWenDa_Hard_Question_Point/2
					else							-- ������ͨ�÷�
						YouXiWenDa_Role_Point[tbl_WD_Index[RoleID]] = YouXiWenDa_Role_Point[tbl_WD_Index[RoleID]] + YouXiWenDa_Normal_Question_Point/2
					end
				end

			end

			-- ������Ŀ
			if YouXiWenDa_Questions[tbl_WD_Index[RoleID]] <= 10 then    -- ������С��10��ʱ����������Ŀ
				YouXiWenDa_GenerateQuestion(RoleID, TargetID)
			else
				-- �������
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 40, -1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				YouXiWenDa_RewardRole(MapID, InstanceID, RoleID)
				YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] = 1
			end

			-- �������
			if (role.IsRoleOnLine(MapID, InstanceID, RoleID) == 0) then
				-- ����Ѿ��μӹ�������õ�ǰ������Ӧ�Ľ��������һ�Զ���ֹ
				if (YouXiWenDa_Attend[tbl_WD_Index[RoleID]] == 1) then
					YouXiWenDa_RewardRole(MapID, InstanceID, RoleID)
					YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] = 1
				end
			end
		end
	end
end


-- ��ʼ���ձ��Ĭ��ֵ
function YouXiWenDa_InitNullTable(RoleID)
	-- ����һЩ�ձ��Ĭ��ֵ
	if tbl_WD_Index[RoleID] == nil then
		g_WD_RoleNum = g_WD_RoleNum + 1
		tbl_WD_RoleID[g_WD_RoleNum] = RoleID
		tbl_WD_Index[RoleID] = g_WD_RoleNum
		tbl_WD_Questions[g_WD_RoleNum] = {}
	end

	if YouXiWenDa_Stepindex[g_WD_RoleNum] == nil then
		YouXiWenDa_Stepindex[g_WD_RoleNum] = 1
	end
	if YouXiWenDa_Attend[g_WD_RoleNum] == nil then
		YouXiWenDa_Attend[g_WD_RoleNum] = 0
	end
	if YouXiWenDa_Questions[g_WD_RoleNum] == nil then
		YouXiWenDa_Questions[g_WD_RoleNum] = 1
	end
    if YouXiWenDa_Role_Point[g_WD_RoleNum] == nil then
        YouXiWenDa_Role_Point[g_WD_RoleNum] = 0
    end
	if YouXiWenDa_Question_Type[g_WD_RoleNum] == nil then
	    YouXiWenDa_Question_Type[g_WD_RoleNum] = 0
	end
end

-- ��ʼ�����
function YouXiWenDa_InitQuestionTable()
	-- ��id_msg�а������Ѵ������ݷ������
	for i=1, YouXiWenDa_Normal_Question_Num do
		local id = 602001 + (i-1)*5
		YouXiWenDa_Normal_Question_Table[i] = {Content = id, RightAnswer = id+1, WrongAnswer1 = id+2, WrongAnswer2 = id+3, WrongAnswer3 = id+4}
	end
end

-- �����������ҵ��ߣ�������ҽ���
function YouXiWenDa_RewardRole(MapID, InstanceID, RoleID)
	local RoleIndex = tbl_WD_Index[RoleID]
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if level > 100 then
		level = 100
	end
	level = math.floor(level/5)*5

	-- ����
	local YouXiWenDa_reward = {
	[10]=1800,[15]=3000,
	[20]=5995,[25]=8190,[30]=15624
	}
	local addexp = YouXiWenDa_reward[level]
	role.AddRoleExp(MapID, InstanceID, RoleID, addexp*(YouXiWenDa_Role_Point[RoleIndex]/100))

	YouXiWenDa_Stepindex[RoleIndex] = 1

	if role.GetTrialState(RoleID, 1006) == 1 then
		role.SetTrialCompleteNum(RoleID, 1006, role.GetTrialCompleteNum(RoleID,1006) + 1)
    end
	if role.GetTrialState(RoleID, 1006) == 1 and role.GetTrialCompleteNum(RoleID,1006) >= Trail_maxnum[1006] then
		role.TrialComplete(RoleID,1006)
	end
end

-- ���ز��ظ�����Ŀ  ����0Ϊ������Ŀ 1Ϊ��ͨ��Ŀ
function WD_GetQuestion(RoleIndex, n)
	WD_CallDeep = WD_CallDeep + 1		-- �ݹ����+1
	local question
	local index



	if n == 0 then
		if table.maxn(YouXiWenDa_Hard_Question_Table) == 0 then	--�ж�֮ǰ�Ļ���й����У��Ƿ������ؽű��������������ԭ����������ÿ�
			YouXiWenDa_InitQuestionTable()
		end
		index = math.random(1, table.maxn(YouXiWenDa_Hard_Question_Table))
		question = YouXiWenDa_Hard_Question_Table[index]
	else
		if table.maxn(YouXiWenDa_Normal_Question_Table) == 0 then	--�ж�֮ǰ�Ļ���й����У��Ƿ������ؽű��������������ԭ����������ÿ�
			YouXiWenDa_InitQuestionTable()
		end
		index = math.random(1, table.maxn(YouXiWenDa_Normal_Question_Table))
		question = YouXiWenDa_Normal_Question_Table[index]
	end

	if tbl_WD_Questions[RoleIndex] == nil then
		local question_table = {}
		tbl_WD_Questions[RoleIndex] = {question_table}
		table.insert(tbl_WD_Questions[RoleIndex], question.Content)
		return question
	end

	local flag = 0
	for k, v in pairs(tbl_WD_Questions[RoleIndex]) do
		if v == question.Content then
			flag = 1
			break
		end
	end

	if flag == 1 then
		if WD_CallDeep >= 10 then			-- �ݹ���ȳ���10�Ļ���ֱ�ӷ����ظ�����
			return question
		else
			return WD_GetQuestion(RoleIndex, n)
		end
	else
		table.insert(tbl_WD_Questions[RoleIndex], question.Content)
		return question
	end
end

-- ������Ŀ��ѡ��
function YouXiWenDa_GenerateQuestion(RoleID, TargetID)
	local RoleIndex = tbl_WD_Index[RoleID]

	-- ������Ŀ

	WD_CallDeep = 0
	YouXiWenDa_Current_Question[RoleIndex].question = WD_GetQuestion(RoleIndex, 1)

	local MsgID = msg.BeginMsgEvent()

	msg.AddMsgEvent(MsgID, 40, YouXiWenDa_Current_Question[RoleIndex].question.Content)	-- ��Ŀ����
	msg.AddMsgEvent(MsgID, 1, 205004)
	msg.AddMsgEvent(MsgID, 12, YouXiWenDa_Role_Point[RoleIndex])		-- �÷�

	-- ��ȷ�����ĸ�ѡ���е����λ��
	YouXiWenDa_RightAnswer[RoleIndex] = math.random(0, 3)
	local index = 0
	for i=0, 3 do
		if YouXiWenDa_RightAnswer[RoleIndex] == i then
			msg.AddMsgEvent(MsgID, 21, i)
			msg.AddMsgEvent(MsgID, 1, YouXiWenDa_Current_Question[RoleIndex].question.RightAnswer)
		else
			msg.AddMsgEvent(MsgID, 21, i)
			msg.AddMsgEvent(MsgID, 1, YouXiWenDa_Current_Question[RoleIndex].question.WrongAnswer1+index)
			index = index + 1
		end
	end

	YouXiWenDa_Question_Type[RoleIndex] = 0		-- ��ǰ��Ŀ����
	-- ����ǰʱ�����table��
	YouXiWenDa_Time[RoleIndex] = os.time()

	-- ����ʱ45��
	msg.AddMsgEvent(MsgID, 9, YouXiWenDa_Time_Counter)

	-- �ѻش���Ŀ��
	msg.AddMsgEvent(MsgID, 12, YouXiWenDa_Questions[RoleIndex]-1)
	msg.AddMsgEvent(MsgID, 24, TargetID)
	YouXiWenDa_Questions[RoleIndex] = YouXiWenDa_Questions[RoleIndex] + 1 	-- �Ѵ�����+1
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

-- ���ʼ
function YouXiWenDa_OnStart(actID)
	g_WD_RoleNum = 0
	tbl_WD_RoleID = {}
	tbl_WD_Index = {}
	tbl_WD_Questions = {}

	-- �����������
	math.randomseed(os.time())
	-- ��ʼ�����
	YouXiWenDa_InitQuestionTable()
end


-- npc����ʱ�䵽����������ڸ�������ɾ����npc
function YouXiWenDa_OnEnd(actID)

	-- ��ձ�������
	YouXiWenDa_Attend = {}
	YouXiWenDa_Questions = {}
	YouXiWenDa_RightAnswer = {}
	YouXiWenDa_Time = {}
	YouXiWenDa_Role_Point = {}
	YouXiWenDa_Stepindex = {}
	YouXiWenDa_Role_Questions = {}
	YouXiWenDa_Question_Type = {}
	YouXiWenDa_Hard_Question_Table = {}
	YouXiWenDa_Normal_Question_Table = {}
	YouXiWenDa_Role_Level = {}
	YouXiWenDa_Current_Question = {}
	tbl_WD_RoleID = {}
	tbl_WD_Index = {}
	tbl_WD_Questions = {}
	g_WD_RoleNum = 0
end

-- ע����Ӧ�¼�
aux.RegisterCreatureEvent(5510302, 7, "n5510302_OnTalk")-------------------------
aux.RegisterActEvent(26, 3, "YouXiWenDa_OnEnd")
aux.RegisterActEvent(26, 2, "YouXiWenDa_OnStart")
