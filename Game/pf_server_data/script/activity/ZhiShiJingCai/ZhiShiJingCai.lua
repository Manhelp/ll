---���ļ���baixiao��ΪF�ƻ���zhishijingcai,Ϊά�����㣬�ű���δ�滻

BaiXiaoSheng_CreatureID = {}			-- ��Ŵ���֪ʶ���º󷵻ص�id�����ڻ������ɾ��npc
BaiXiaoSheng_Attend = {}				-- �Ƿ��ڴ˻ʱ���ڲμӹ�
BaiXiaoSheng_Questions = {}				-- ������
BaiXiaoSheng_RightAnswer = {}			-- ��ȷ�����ĸ�ѡ���е�����
BaiXiaoSheng_Normal_Question_Point = 10	-- ������ͨ��Ŀ�ķ���
BaiXiaoSheng_Hard_Question_Point = 20 	-- ����������Ŀ�ķ���
BaiXiaoSheng_Time = {}					-- ��Ŵ��⿪ʼʱ��ϵͳʱ�䣬��Ӧ��ɫid
BaiXiaoSheng_Role_Point = {}			-- ��һ�õķ�������Ӧ��ɫid
BaiXiaoSheng_Stepindex = {}				-- ÿ����ҵĻ����
BaiXiaoSheng_Question_Type = {}			-- ��ǰ��Ŀ���ͣ��������ѻ������ͨ
BaiXiaoSheng_Role_Level = {}			-- ��ҵ�ǰ�ȼ�
BaiXiaoSheng_Time_Counter = 45			-- ����ĵ���ʱ
BaiXiaoSheng_Current_Question = {}      -- ��ҵ�ǰ����

BaiXiaoSheng_Hard_Question_Num = 20	-- �������ѵ���Ŀ��
BaiXiaoSheng_Normal_Question_Num = 98	-- ������ͨ����Ŀ��

BX_CallDeep = 0							-- �ݹ�������


BaiXiao_Hard_Question_Table = {}
BaiXiao_Normal_Question_Table = {}

g_BX_RoleNum = 0						-- �μ�֪ʶ���´�������Ҹ���
tbl_BX_RoleID = {}						-- ��g_BX_RoleNumΪ������Ӧ��ҵ�RoleID
tbl_BX_Index = {}						-- ��RoleIDΪ������Ӧ��ҵĴ���Index
tbl_BX_Questions = {}					-- ����Ѿ��ش������Ŀ

-- npc�Ի��������������
function n5510301_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		-- ��ʼ���ձ�
		BaiXiaoSheng_InitNullTable(RoleID)
	end

	-- �õ�������
	BaiXiaoSheng_Role_Level[tbl_BX_Index[RoleID]] = role.GetRoleLevel(MapID, InstanceID, RoleID)

	if BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 200001)	-- "���Ƿ�����μ�֪ʶ���»��"
		msg.AddMsgEvent(MsgID, 21, 4)		-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 100001)	-- "ȷ��"
		msg.AddMsgEvent(MsgID, 21, 5)		-- ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100002)	-- "ȡ��"
		msg.AddMsgEvent(MsgID, 24, TargetID)
		BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 2
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] == 2 then
		if TalkIndex == 4 then			-- ѡ����ȷ���μӰ�ť��
			if BaiXiaoSheng_Role_Level[tbl_BX_Index[RoleID]] <= 30 then	-- �ȼ�����30���Ļ�������ʾ
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 200002)		-- "֪ʶ���»������30�����ϲſɲ���"
				msg.AddMsgEvent(MsgID, 21, 4)			-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001)		-- "ȷ��"
				BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			elseif BaiXiaoSheng_Attend[tbl_BX_Index[RoleID]] == 1 then	-- �ڴ˻ʱ�����Ѳμӹ�
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 200003)		-- "���Ѳμӹ�����֪ʶ���»���ɼ����μ��¸�ʱ��Ļ��"
				msg.AddMsgEvent(MsgID, 21, 4)			-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001)		-- "ȷ��"
				BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			else
				-- ���������ڿ�ʼ��
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 200004)		--�������ڿ�ʼ!
				msg.AddMsgEvent(MsgID, 21, 4)
				msg.AddMsgEvent(MsgID, 1, 100001)
				msg.AddMsgEvent(MsgID, 24, TargetID)
				BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 3
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				BaiXiaoSheng_Attend[tbl_BX_Index[RoleID]] = 1				-- �Ѳμӹ����λ
				BaiXiaoSheng_Current_Question[tbl_BX_Index[RoleID]] = {question, index}
			end
		elseif TalkIndex == 5 then
			BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1
		else
			BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1
		end


	elseif BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] == 3 then				-- ���⿪ʼ
		BaiXiaoSheng_GenerateQuestion(RoleID, TargetID)
		BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 4


	elseif BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] == 4 then	-- �����������ʱ��
		if TalkIndex == 5 then						-- �����������ҵ�ǰ�÷�Ӧ�õĽ���
			BaiXiaoSheng_RewardRole(MapID, InstanceID, RoleID)
		else
			-- ������⻨�ѵ�ʱ��
			local time1 = BaiXiaoSheng_Time[tbl_BX_Index[RoleID]]
			local dtime = os.difftime(os.time(), time1)

			-- �жϴ��Ƿ���ȷ
			if BaiXiaoSheng_RightAnswer[tbl_BX_Index[RoleID]] == TalkIndex then
				-- �ش�������ʱ�䲻ͬ���÷ֲ�ͬ
				if dtime <= 45 then				-- 45���ڻش����ȷ�𰸣������Ŀ4����
					if BaiXiaoSheng_Question_Type[tbl_BX_Index[RoleID]] == 1 then  	-- �������ѵ÷�
						BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] = BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] + BaiXiaoSheng_Hard_Question_Point
					else							-- ������ͨ�÷�
						BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] = BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] + BaiXiaoSheng_Normal_Question_Point
					end
				else
					if BaiXiaoSheng_Question_Type[tbl_BX_Index[RoleID]] == 1 then  	-- �������ѵ÷�
						BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] = BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] + BaiXiaoSheng_Hard_Question_Point/2
					else							-- ������ͨ�÷�
						BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] = BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] + BaiXiaoSheng_Normal_Question_Point/2
					end
				end

			end

			-- ������Ŀ
			if BaiXiaoSheng_Questions[tbl_BX_Index[RoleID]] <= 30 then    -- ������С��30��ʱ����������Ŀ
				BaiXiaoSheng_GenerateQuestion(RoleID, TargetID)
			else
				-- �������
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 40, -1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				BaiXiaoSheng_RewardRole(MapID, InstanceID, RoleID)
				BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1
			end

			-- �������
			if (role.IsRoleOnLine(MapID, InstanceID, RoleID) == 0) then
				-- ����Ѿ��μӹ�������õ�ǰ������Ӧ�Ľ��������һ�Զ���ֹ
				if (BaiXiaoSheng_Attend[tbl_BX_Index[RoleID]] == 1) then
					BaiXiaoSheng_RewardRole(MapID, InstanceID, RoleID)
					BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1
				end
			end
		end
	end
end


-- ��ʼ���ձ��Ĭ��ֵ
function BaiXiaoSheng_InitNullTable(RoleID)
	-- ����һЩ�ձ��Ĭ��ֵ
	if tbl_BX_Index[RoleID] == nil then
		g_BX_RoleNum = g_BX_RoleNum + 1
		tbl_BX_RoleID[g_BX_RoleNum] = RoleID
		tbl_BX_Index[RoleID] = g_BX_RoleNum
		tbl_BX_Questions[g_BX_RoleNum] = {}
	end

	if BaiXiaoSheng_Stepindex[g_BX_RoleNum] == nil then
		BaiXiaoSheng_Stepindex[g_BX_RoleNum] = 1
	end
	if BaiXiaoSheng_Attend[g_BX_RoleNum] == nil then
		BaiXiaoSheng_Attend[g_BX_RoleNum] = 0
	end
	if BaiXiaoSheng_Questions[g_BX_RoleNum] == nil then
		BaiXiaoSheng_Questions[g_BX_RoleNum] = 1
	end
    if BaiXiaoSheng_Role_Point[g_BX_RoleNum] == nil then
        BaiXiaoSheng_Role_Point[g_BX_RoleNum] = 0
    end
	if BaiXiaoSheng_Question_Type[g_BX_RoleNum] == nil then
	    BaiXiaoSheng_Question_Type[g_BX_RoleNum] = 0
	end
end

-- ��ʼ�����
function BaiXiaoSheng_InitQuestionTable()
	-- ��id_msg�а������Ѵ������ݷ������
	for i=1, BaiXiaoSheng_Hard_Question_Num do
		local id = 601001 + (i-1)*5
		BaiXiao_Hard_Question_Table[i] = {Content = id, RightAnswer = id+1, WrongAnswer1 = id+2, WrongAnswer2 = id+3, WrongAnswer3 = id+4}
	end

	-- ��id_msg�а�����ͨ�������ݷ������
	for i=1, BaiXiaoSheng_Normal_Question_Num do
		local id = 600001 + (i-1)*5
		BaiXiao_Normal_Question_Table[i] = {Content = id, RightAnswer = id+1, WrongAnswer1 = id+2, WrongAnswer2 = id+3, WrongAnswer3 = id+4}
	end

end

-- �����������ҵ��ߣ�������ҽ���
function BaiXiaoSheng_RewardRole(MapID, InstanceID, RoleID)

	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if level > 100 then
		level = 100
	end
	local FreeSize = role.GetBagFreeSize(RoleID)
	if FreeSize >= 1 then
	    if level >= 75 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4090006, 5, -1, 8, 420)
		elseif level >= 55 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4090005, 4, -1, 8, 420)
		elseif level >= 40 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4090004, 3, -1, 8, 420)
		end
	end

	level = math.floor(level/5)*5
	--[[��Ǯ
	local RoleIndex = tbl_BX_Index[RoleID]
	local addmoney = 5000 * BaiXiaoSheng_Role_Point[RoleIndex] / 360
	role.AddRoleSilver(MapID, InstanceID, RoleID, addmoney, 0)
]]
	-- ����
	local BaiXiaoSheng_reward = {
	[20]=7245,[25]=8820,[30]=23940,[35]=30240,
	[40]=45360,[45]=54180,[50]=79500,[55]=93000,
	[60]=120960,[65]=139860,[70]=192780,[75]=253260,
	[80]=327600,[85]=406350,[90]=495810,[95]=595980,[100]=595980
	}

	local addexp = BaiXiaoSheng_reward[level]
	role.AddRoleExp(MapID, InstanceID, RoleID, addexp * BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]]/360)

	BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1

	if role.GetTrialState(RoleID, 3014) == 1 then
		role.SetTrialCompleteNum(RoleID, 3014, role.GetTrialCompleteNum(RoleID,3014) + 1)
    end
	if role.GetTrialState(RoleID, 3014) == 1 and role.GetTrialCompleteNum(RoleID,3014) >= Trail_maxnum[3014] then
		role.TrialComplete(RoleID,3014)
	end
	role.NotifyFBBS(RoleID,11,0)
end

-- ���ز��ظ�����Ŀ  ����0Ϊ������Ŀ 1Ϊ��ͨ��Ŀ
function BX_GetQuestion(RoleIndex, n)
	BX_CallDeep = BX_CallDeep + 1		-- �ݹ����+1
	local question
	local index



	if n == 0 then
		if table.maxn(BaiXiao_Hard_Question_Table) == 0 then	--�ж�֮ǰ�Ļ���й����У��Ƿ������ؽű��������������ԭ����������ÿ�
			BaiXiaoSheng_InitQuestionTable()
		end
		index = math.random(1, table.maxn(BaiXiao_Hard_Question_Table))
		question = BaiXiao_Hard_Question_Table[index]
	else
		if table.maxn(BaiXiao_Normal_Question_Table) == 0 then	--�ж�֮ǰ�Ļ���й����У��Ƿ������ؽű��������������ԭ����������ÿ�
			BaiXiaoSheng_InitQuestionTable()
		end
		index = math.random(1, table.maxn(BaiXiao_Normal_Question_Table))
		question = BaiXiao_Normal_Question_Table[index]
	end

	if tbl_BX_Questions[RoleIndex] == nil then
		local question_table = {}
		tbl_BX_Questions[RoleIndex] = {question_table}
		table.insert(tbl_BX_Questions[RoleIndex], question.Content)
		return question
	end

	local flag = 0
	for k, v in pairs(tbl_BX_Questions[RoleIndex]) do
		if v == question.Content then
			flag = 1
			break
		end
	end

	if flag == 1 then
		if BX_CallDeep >= 30 then			-- �ݹ���ȳ���30�Ļ���ֱ�ӷ����ظ�����
			return question
		else
			return BX_GetQuestion(RoleIndex, n)
		end
	else
		table.insert(tbl_BX_Questions[RoleIndex], question.Content)
		return question
	end
end

-- ������Ŀ��ѡ��
function BaiXiaoSheng_GenerateQuestion(RoleID, TargetID)
	local RoleIndex = tbl_BX_Index[RoleID]
	if math.fmod(BaiXiaoSheng_Questions[RoleIndex], 5	) == 0 then  -- ��������5�ı���ʱ��ȡ�������ѵ���Ŀ
		BX_CallDeep = 0
		BaiXiaoSheng_Current_Question[RoleIndex].question = BX_GetQuestion(RoleIndex, 0)

		local MsgID = msg.BeginMsgEvent()

		msg.AddMsgEvent(MsgID, 40, BaiXiaoSheng_Current_Question[RoleIndex].question.Content)	-- ��Ŀ����
		msg.AddMsgEvent(MsgID, 1, 200005)
		msg.AddMsgEvent(MsgID, 12, BaiXiaoSheng_Role_Point[RoleIndex])		-- �÷�


		-- ��ȷ�����ĸ�ѡ���е����λ��
		BaiXiaoSheng_RightAnswer[RoleIndex] = math.random(0, 3)
		local index = 0
		for i=0, 3 do									-- �ĸ�ѡ��
			if BaiXiaoSheng_RightAnswer[RoleIndex] == i then
				msg.AddMsgEvent(MsgID, 21, i)			-- ѡ��
				msg.AddMsgEvent(MsgID, 1, BaiXiaoSheng_Current_Question[RoleIndex].question.RightAnswer)	-- ѡ������
			else
				msg.AddMsgEvent(MsgID, 21, i)
				msg.AddMsgEvent(MsgID, 1, BaiXiaoSheng_Current_Question[RoleIndex].question.WrongAnswer1+index)
				index = index + 1
			end
		end

		BaiXiaoSheng_Question_Type[RoleIndex] = 1		-- ��ǰ��Ŀ����
		-- ����ǰʱ�����table��
		BaiXiaoSheng_Time[RoleIndex] = os.time()
		-- ����ʱ
		msg.AddMsgEvent(MsgID, 9, BaiXiaoSheng_Time_Counter)
		-- �ѻش���Ŀ��
		msg.AddMsgEvent(MsgID, 12, BaiXiaoSheng_Questions[RoleIndex]-1)
		msg.AddMsgEvent(MsgID, 24, TargetID)
		BaiXiaoSheng_Questions[RoleIndex] = BaiXiaoSheng_Questions[RoleIndex] + 1 	-- ��ǰ������+1
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	-- ����������ͨ��Ŀ
	else
		BX_CallDeep = 0
		BaiXiaoSheng_Current_Question[RoleIndex].question = BX_GetQuestion(RoleIndex, 1)

		local MsgID = msg.BeginMsgEvent()

		msg.AddMsgEvent(MsgID, 40, BaiXiaoSheng_Current_Question[RoleIndex].question.Content)	-- ��Ŀ����
		msg.AddMsgEvent(MsgID, 1, 200005)
		msg.AddMsgEvent(MsgID, 12, BaiXiaoSheng_Role_Point[RoleIndex])		-- �÷�

		-- ��ȷ�����ĸ�ѡ���е����λ��
		BaiXiaoSheng_RightAnswer[RoleIndex] = math.random(0, 3)
		local index = 0
		for i=0, 3 do
			if BaiXiaoSheng_RightAnswer[RoleIndex] == i then
				msg.AddMsgEvent(MsgID, 21, i)
				msg.AddMsgEvent(MsgID, 1, BaiXiaoSheng_Current_Question[RoleIndex].question.RightAnswer)
			else
				msg.AddMsgEvent(MsgID, 21, i)
				msg.AddMsgEvent(MsgID, 1, BaiXiaoSheng_Current_Question[RoleIndex].question.WrongAnswer1+index)
				index = index + 1
			end
		end

		BaiXiaoSheng_Question_Type[RoleIndex] = 0		-- ��ǰ��Ŀ����
		-- ����ǰʱ�����table��
		BaiXiaoSheng_Time[RoleIndex] = os.time()

		-- ����ʱ45��
		msg.AddMsgEvent(MsgID, 9, BaiXiaoSheng_Time_Counter)

		-- �ѻش���Ŀ��
		msg.AddMsgEvent(MsgID, 12, BaiXiaoSheng_Questions[RoleIndex]-1)
		msg.AddMsgEvent(MsgID, 24, TargetID)
		BaiXiaoSheng_Questions[RoleIndex] = BaiXiaoSheng_Questions[RoleIndex] + 1 	-- �Ѵ�����+1
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end

-- ���ʼ
function BaiXiaoSheng_OnStart(actID)
	g_BX_RoleNum = 0
	tbl_BX_RoleID = {}
	tbl_BX_Index = {}
	tbl_BX_Questions = {}

	-- �����������
	math.randomseed(os.time())
	-- ��ʼ�����
	BaiXiaoSheng_InitQuestionTable()

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100037)
	msg.DispatchBroadcast(MsgID, -1, -1, -1)

	-- �ڸ�������ˢ��npc֪ʶ����

	BaiXiaoSheng_CreatureID[1] = map.MapCreateCreature(3424073512, -1, 5510301, 510, 23167, 300)---------------------------------
	BaiXiaoSheng_CreatureID[2] = map.MapCreateCreature(3424073512, -1, 5510301, 520, 23167, 310)---------------------------------
	BaiXiaoSheng_CreatureID[3] = map.MapCreateCreature(3424073512, -1, 5510301, 510, 23167, 310)----------------------------------

end


-- npc����ʱ�䵽����������ڸ�������ɾ����npc
function BaiXiaoSheng_OnEnd(actID)
	map.MapDeleteCreature(3424073512, -1, BaiXiaoSheng_CreatureID[1])
	map.MapDeleteCreature(3424073512, -1, BaiXiaoSheng_CreatureID[2])
	map.MapDeleteCreature(3424073512, -1, BaiXiaoSheng_CreatureID[3])

	-- ��ձ�������
	BaiXiaoSheng_Attend = {}
	BaiXiaoSheng_Questions = {}
	BaiXiaoSheng_RightAnswer = {}
	BaiXiaoSheng_Time = {}
	BaiXiaoSheng_Role_Point = {}
	BaiXiaoSheng_Stepindex = {}
	BaiXiaoSheng_Role_Questions = {}
	BaiXiaoSheng_Question_Type = {}
	BaiXiao_Hard_Question_Table = {}
	BaiXiao_Normal_Question_Table = {}
	BaiXiaoSheng_Role_Level = {}
	BaiXiaoSheng_Current_Question = {}
	tbl_BX_RoleID = {}
	tbl_BX_Index = {}
	tbl_BX_Questions = {}
	g_BX_RoleNum = 0
end

-- ע����Ӧ�¼�
aux.RegisterCreatureEvent(5510301, 7, "n5510301_OnTalk")
aux.RegisterActEvent(23, 3, "BaiXiaoSheng_OnEnd")
aux.RegisterActEvent(24, 3, "BaiXiaoSheng_OnEnd")
aux.RegisterActEvent(23, 2, "BaiXiaoSheng_OnStart")
aux.RegisterActEvent(24, 2, "BaiXiaoSheng_OnStart")
