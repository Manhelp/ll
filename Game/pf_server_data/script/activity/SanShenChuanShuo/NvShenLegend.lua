
--���ڱ������ڱ��λ���Ƿ�����ɹ������ȫ��table�������RoleIDΪ����
NvShenLegend_Quest_IsComplete = {}
--��¼ÿ�λ��������ɵ��ܴ���
NvShenLegend_CompleteQuestTime = 0
--��ˢ���ĻNPC��Ů����̴ӡ���CreatureID
NvShen_CreatureID = {}

--ÿ����ˢ����NPC����Ϣ505, 23167, 304
NvShen_CreatureID[1] = {MapID=3424073512,x=505,y=23167,z=304,CreatureID=0}--��ս���:[505, 23167, 304

NvShenLegend_TimerMin = 0	--��¼������˶೤ʱ��

function NvShenLegend_OnStart(actID)	--���ʼ
	local TempTable = {}
	NvShenLegend_Quest_IsComplete = deepcopy(TempTable)
	--��ʼ���ȫ�ֱ���
	NvShenLegend_TimerMin = 0
	NvShenLegend_CompleteQuestTime = 0

	--ˢ���NPC Ů����̴�[5510401]
	NvShen_CreatureID[1].CreatureID = map.MapCreateCreature(NvShen_CreatureID[1].MapID, -1, 5510401, NvShen_CreatureID[1].x, NvShen_CreatureID[1].y, NvShen_CreatureID[1].z)

	--���ʼ���Ź㲥��
	--������Ů���Ӣ�˻��ʼ����λ��ҿ��ڶ�ս���[���꣬����]���ҵ���Ů����̴ӡ���ȡ�������ɺ���з��Ľ�������
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID,102,100009)
	msg.DispatchBroadcast(MsgID,-1,-1,-1)
end

function NvShenLegend_OnTimerMin(actID)	--�ÿ����1����

	NvShenLegend_TimerMin = NvShenLegend_TimerMin+1


	if NvShen_CreatureID[1].CreatureID == 0 then
		NvShen_CreatureID[1].CreatureID = map.MapCreateCreature(NvShen_CreatureID[1].MapID, -1, 5510401, NvShen_CreatureID[1].x, NvShen_CreatureID[1].y, NvShen_CreatureID[1].z)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100009)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end
	--��Ů���Ӣ�˻���ڽ����У����еȼ��ﵽ10������Ҿ����ڶ�ս���[505��304]�ġ�Ů����̴ӡ�����ȡ�����׬ȡ���������
	if NvShenLegend_TimerMin==5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100010)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)

	elseif NvShenLegend_TimerMin==10 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100010)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	elseif NvShenLegend_TimerMin==50 then
	--����е�50����ʱ���Ź㲥��
	--Ů����̴ӽ���10���Ӻ󷵻���ͥ����Ҫ�����������뾡��ǰ������������ʱ���ռ���´λ���������
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100028)
		msg.AddMsgEvent(MsgID,5,5510401)
		msg.AddMsgEvent(MsgID,9,10)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	elseif NvShenLegend_TimerMin==55 then
	--����е�55����ʱ���Ź㲥��
	--��Ů����̴ӡ�����5���Ӻ󷵻���ͥ����Ҫ�������������뾡��ǰ������������ʱ���ռ���´λ���������
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100028)
		msg.AddMsgEvent(MsgID,5,5510401)
		msg.AddMsgEvent(MsgID,9,5)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)

	end
end



function NvShenLegend_OnEnd(actID)	--�����

	--�жϻNPC�Ƿ��Ѵﵽ����ʱ���Զ���ʧ����δ��ʧ���ڳ�����ɾ����NPC�������ͷ�NPC��CreatureID
	if NvShen_CreatureID[1].CreatureID ~= nil and unit.IsDead(NvShen_CreatureID[1].MapID,-1,NvShen_CreatureID[1].CreatureID)==false then
		map.MapDeleteCreature(NvShen_CreatureID[1].MapID,-1,NvShen_CreatureID[1].CreatureID)
		NvShen_CreatureID[1].CreatureID = nil
	else
		NvShen_CreatureID[1].CreatureID = nil
	end

	NvShenLegend_Quest_IsComplete = {}
	NvShenLegend_CompleteQuestTime = 0
	NvShenLegend_TimerMin = 0
	--��������Ź㲥��
	--�����η���˵��Ѿ���������δ����������ҿɼ����������񣬲����´λʱ�������񣡡�
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID,102,100011)
	msg.DispatchBroadcast(MsgID,-1,-1,-1)
end




function NvShenLegend_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)	--����������

	NvShenLegend_Quest_IsComplete[RoleID] = 1--��¼����ڱ��λ������ɹ�����
	NvShenLegend_CompleteQuestTime = NvShenLegend_CompleteQuestTime+1
	local NvShen_Reward = {}
	NvShen_Reward[30203] = {Exp=3510,Gold=1395}
	NvShen_Reward[30204] = {Exp=10350,Gold=3075}
	NvShen_Reward[30205] = {Exp=12600,Gold=3400}
	NvShen_Reward[30206] = {Exp=24480,Gold=5960}
	NvShen_Reward[30207] = {Exp=32880,Gold=6480}
	NvShen_Reward[30208] = {Exp=52800,Gold=8700}
	NvShen_Reward[30209] = {Exp=66300,Gold=9300}
	NvShen_Reward[30210] = {Exp=121500,Gold=9850}
	NvShen_Reward[30211] = {Exp=146250,Gold=10400}
	NvShen_Reward[30212] = {Exp=259200,Gold=16425}
	NvShen_Reward[30213] = {Exp=303075,Gold=17175}
	NvShen_Reward[30214] = {Exp=419580,Gold=21600}
	NvShen_Reward[30215] = {Exp=480330,Gold=22500}
	NvShen_Reward[30216] = {Exp=604800,Gold=26000}
	NvShen_Reward[30217] = {Exp=681300,Gold=27000}
	NvShen_Reward[30218] = {Exp=951750,Gold=34875}
	NvShen_Reward[30219] = {Exp=1058625,Gold=36000}

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 201004)--��ϲ�����<p1>�����ܼƻ�þ���<p2>�㼰��Ǯ<p3>��"
	msg.AddMsgEvent(MsgID, 11, QuestID)

	if NvShenLegend_CompleteQuestTime==1 then	--��ҵ�һ��������񣬶�����������1������ͽ�Ǯ����
		role.AddRoleExp(MapID, InstanceID, RoleID, NvShen_Reward[QuestID].Exp)
		role.AddRoleSilver(MapID, InstanceID, RoleID, NvShen_Reward[QuestID].Gold, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201001)--����ϲ���ڱ��λ�е�һ��������񣬶�����2�����齱������
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100012)
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, NvShen_Reward[QuestID].Exp*2)
		msg.AddMsgEvent(MsgID, 9, NvShen_Reward[QuestID].Gold*2)
	elseif NvShenLegend_CompleteQuestTime==10 then	--��ҵ�10��������񣬶�����������0.5������ͽ�Ǯ����
		local ex = math.floor(NvShen_Reward[QuestID].Exp/2)
		local go = math.floor(NvShen_Reward[QuestID].Gold/2)
		role.AddRoleExp(MapID, InstanceID, RoleID, ex)
		role.AddRoleSilver(MapID, InstanceID, RoleID, go, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201002)--��ϲ���ڱ��λ�е�&lt;p1&gt;��������񣬶�����1.5�����齱����
		msg.AddMsgEvent(MsgID_1, 9, 10)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100013)
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, ShiShen_Reward[QuestID].Exp+ex)
		msg.AddMsgEvent(MsgID, 9, ShiShen_Reward[QuestID].Gold+go)
	elseif NvShenLegend_CompleteQuestTime==100 then	--��ҵ�100��������񣬶�����������0.5������ͽ�Ǯ����
		local ex = math.floor(NvShen_Reward[QuestID].Exp/2)
		local go = math.floor(NvShen_Reward[QuestID].Gold/2)
		role.AddRoleExp(MapID, InstanceID, RoleID, ex)
		role.AddRoleSilver(MapID, InstanceID, RoleID, go, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201002)--��ϲ���ڱ��λ�е�&lt;p1&gt;��������񣬶�����1.5�����齱����
		msg.AddMsgEvent(MsgID_1, 9, 100)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100014)
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, NvShen_Reward[QuestID].Exp+ex)
		msg.AddMsgEvent(MsgID, 9, NvShen_Reward[QuestID].Gold+go)
	else
		msg.AddMsgEvent(MsgID, 9, NvShen_Reward[QuestID].Exp)
		msg.AddMsgEvent(MsgID, 9, NvShen_Reward[QuestID].Gold)
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)

	if role.GetTrialState(RoleID, 1009) == 1 then
		role.SetTrialCompleteNum(RoleID, 1009, role.GetTrialCompleteNum(RoleID,1009) + 1)
    end
	if role.GetTrialState(RoleID, 1009) == 1 and role.GetTrialCompleteNum(RoleID,1009) >= Trail_maxnum[1009] then
		role.TrialComplete(RoleID,1009)
	end
	--Ů�����
	UpdateSeverData(49, 1)
	--��Ҽ���
	UpdateRoleData(RoleID, 12, 1)
end

function NvShenLegend_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
    local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	--�жϱ������пռ��Ƿ��㹻
	--[[if rolelevel >= 50 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if(FreeSize < 1) then
			--��ʾ��ұ����ռ䲻��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)--��Ҫ����x�񱳰��ռ�ſɼ�������
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		end
	end]]
	--local FreeSize = role.GetBagFreeSize(RoleID)
	--[[if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)--��Ҫ����x�񱳰��ռ�ſɼ�������
		msg.AddMsgEvent(MsgID, 9, 3)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end]]
	if NvShenLegend_Quest_IsComplete[RoleID] ~= nil then
		local MsgID_1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 26, 201003)--ÿ�λ�ڼ�ֻ�����һ������
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		return 0
	end
	return 1
end


function NvShenLegend_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	if NvShenLegend_Quest_IsComplete[RoleID] ~= nil then
		local MsgID_1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 26, 201005)--���������Ѿ���ɣ������ٽ�
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		return 0
	end
	for i = 30203,30219 do
		if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, i) == true then
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_2, 26, 100007)--���Ѿ���ȡ������
		msg.DispatchRoleMsgEvent(RoleID, MsgID_2)
		return 0
		end
	end
	return 1
end


--[[________________________ע������¼�________________________]]--

--���ʼ
aux.RegisterActEvent(1, 2, "NvShenLegend_OnStart")
aux.RegisterActEvent(2, 2, "NvShenLegend_OnStart")
aux.RegisterActEvent(3, 2, "NvShenLegend_OnStart")
aux.RegisterActEvent(4, 2, "NvShenLegend_OnStart")

--�����
aux.RegisterActEvent(1, 3, "NvShenLegend_OnEnd")
aux.RegisterActEvent(2, 3, "NvShenLegend_OnEnd")
aux.RegisterActEvent(3, 3, "NvShenLegend_OnEnd")
aux.RegisterActEvent(4, 3, "NvShenLegend_OnEnd")
--��ԡ����ӡ�Ϊ��λ��ʱ
aux.RegisterActEvent(1, 4, "NvShenLegend_OnTimerMin")
aux.RegisterActEvent(2, 4, "NvShenLegend_OnTimerMin")
aux.RegisterActEvent(3, 4, "NvShenLegend_OnTimerMin")
aux.RegisterActEvent(4, 4, "NvShenLegend_OnTimerMin")

--�����ɡ�����˵������
aux.RegisterQuestEvent(30203, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30204, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30205, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30206, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30207, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30208, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30209, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30210, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30211, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30212, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30213, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30214, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30215, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30216, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30217, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30218, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30219, 1, "NvShenLegend_OnComplete")

--�������Ƿ�ɽ�ȡ������˵������
aux.RegisterQuestEvent(30203, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30204, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30205, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30206, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30207, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30208, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30209, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30210, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30211, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30212, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30213, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30214, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30215, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30216, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30217, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30218, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30219, 4, "NvShenLegend_OnCheckAccept")


--�������Ƿ����ɡ�����˵������
aux.RegisterQuestEvent(30203, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30204, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30205, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30206, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30207, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30208, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30209, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30210, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30211, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30212, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30213, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30214, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30215, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30216, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30217, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30218, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30219, 5, "NvShenLegend_OnCheckComplete")
