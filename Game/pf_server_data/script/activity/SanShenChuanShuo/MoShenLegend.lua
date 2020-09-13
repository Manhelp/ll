local Quest_30201 = {}
Quest_30201[1]={TargetType=QuestTargetType["Kill"],TargetID1=1020016,Num1=1,RewMoney=6277,RewXP=7020,RewItemID1=4080067,value1=1,MsgID=11001,}--
Quest_30201[2]={TargetType=QuestTargetType["Kill"],TargetID1=1030018,Num1=1,RewMoney=13837,RewXP=20700,RewItemID1=4080067,value1=1,MsgID=11002,}--
Quest_30201[3]={TargetType=QuestTargetType["Kill"],TargetID1=1030019,Num1=1,RewMoney=15300,RewXP=25200,RewItemID1=4080067,value1=1,MsgID=11003,}--
Quest_30201[4]={TargetType=QuestTargetType["Kill"],TargetID1=1040043,Num1=1,TargetID2=1040044,Num2=1,RewMoney=26820,RewXP=39900,RewItemID1=4080067,value1=1,MsgID=11004,}--
Quest_30201[5]={TargetType=QuestTargetType["Kill"],TargetID1=1040046,Num1=1,TargetID2=1040044,Num2=1,RewMoney=29160,RewXP=50400,RewItemID1=4080067,value1=1,MsgID=11005,}--
Quest_30201[6]={TargetType=QuestTargetType["Kill"],TargetID1=1040047,Num1=1,TargetID2=1040048,Num2=1,RewMoney=39150,RewXP=75600,RewItemID1=4080067,value1=1,MsgID=11006,}--
Quest_30201[7]={TargetType=QuestTargetType["Kill"],TargetID1=1040045,Num1=1,TargetID2=1040049,Num2=1,RewMoney=41850,RewXP=90300,RewItemID1=4080067,value1=1,MsgID=11007,}--
Quest_30201[8]={TargetType=QuestTargetType["Kill"],TargetID1=1050037,Num1=1,TargetID2=1050038,Num2=1,RewMoney=44325,RewXP=198450,RewItemID1=4080067,value1=1,MsgID=11008,}--
Quest_30201[9]={TargetType=QuestTargetType["Kill"],TargetID1=1050038,Num1=1,TargetID2=1050041,Num2=1,RewMoney=46800,RewXP=231525,RewItemID1=4080067,value1=1,MsgID=11009,}--
Quest_30201[10]={TargetType=QuestTargetType["Kill"],TargetID1=1050041,Num1=1,TargetID2=1050042,Num2=1,RewMoney=73912,RewXP=269325,RewItemID1=4080067,value1=1,MsgID=11010,}--
Quest_30201[11]={TargetType=QuestTargetType["Kill"],TargetID1=1060048,Num1=1,TargetID2=1060047,Num2=1,TargetID3=1060049,Num3=1,RewMoney=77287,RewXP=349650,RewItemID1=4080067,value1=1,MsgID=11011,}--
Quest_30201[12]={TargetType=QuestTargetType["Kill"],TargetID1=1060051,Num1=1,TargetID2=1060052,Num2=1,TargetID3=1060053,Num3=1,RewMoney=97200,RewXP=481950,RewItemID1=4080067,value1=1,MsgID=11012,}--
Quest_30201[13]={TargetType=QuestTargetType["Kill"],TargetID1=1060054,Num1=1,TargetID2=1060055,Num2=1,RewMoney=101250,RewXP=633150,RewItemID1=4080067,value1=1,MsgID=11013,}--
Quest_30201[14]={TargetType=QuestTargetType["Kill"],TargetID1=1070073,Num1=1,TargetID2=1070074,Num2=1,TargetID3=1070075,Num3=1,RewMoney=117000,RewXP=819000,RewItemID1=4080067,value1=1,MsgID=11014,}--
Quest_30201[15]={TargetType=QuestTargetType["Kill"],TargetID1=1070076,Num1=1,TargetID2=1070077,Num2=1,TargetID3=1070078,Num3=1,RewMoney=121500,RewXP=1015875,RewItemID1=4080067,value1=1,MsgID=11015,}--
Quest_30201[16]={TargetType=QuestTargetType["Kill"],TargetID1=1070079,Num1=1,TargetID2=1070080,Num2=1,RewMoney=156937,RewXP=1239525,RewItemID1=4080067,value1=1,MsgID=11016,}--
Quest_30201[17]={TargetType=QuestTargetType["Kill"],TargetID1=1070082,Num1=1,TargetID2=1070085,Num2=1,RewMoney=162000,RewXP=1489950,RewItemID1=4080067,value1=1,MsgID=11017,}--


--���ڱ������ڱ��λ���Ƿ�����ɹ������ȫ��table�������RoleIDΪ��������Ӧ��¼��ͬ���
MoShenLegend_Quest_IsComplete = {}
--��¼ÿ�λ��������ɵ��ܴ���
MoShenLegend_CompleteQuestTime = 0
--�������б�ˢ���ĻNPC����ħʹ�ߡ���CreatureID
MoShen_CreatureID = {}
--����ÿ����ˢ����NPC����Ϣ
MoShen_CreatureID[1] = {MapID=3424073512,x=505,y=23167,z=304,CreatureID=0}
MoShenLegend_TimerMin = 0		--��¼������˶೤ʱ��

function MoShenLegend_OnStart(actID)	--���ʼ
	local TempTable = {}
	MoShenLegend_Quest_IsComplete = deepcopy(TempTable)
	MoShenLegend_TimerMin = 0
	MoShenLegend_CompleteQuestTime = 0

	--ˢ���NPC ��ħʹ��[5510403]
	MoShen_CreatureID[1].CreatureID = map.MapCreateCreature(MoShen_CreatureID[1].MapID, -1, 5510403, MoShen_CreatureID[1].x, MoShen_CreatureID[1].y, MoShen_CreatureID[1].z)

	--���ʼ���Ź㲥��
	--������ħ����������ʼ����λ��ҿ��ڶ�ս���[���꣬����]�ҡ���ħʹ�ߡ���ȡ�������ɺ���з��Ľ�������
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID,102,100021)
	msg.DispatchBroadcast(MsgID,-1,-1,-1)
end

function MoShenLegend_OnTimerMin(actID)	--�ÿ����1����

	MoShenLegend_TimerMin = MoShenLegend_TimerMin + 1
	if MoShen_CreatureID[1].CreatureID == 0 then
		MoShen_CreatureID[1].CreatureID = map.MapCreateCreature(MoShen_CreatureID[1].MapID, -1, 5510403, MoShen_CreatureID[1].x, MoShen_CreatureID[1].y, MoShen_CreatureID[1].z)
		--ħ�����������ڽ����У�����10�����ϵ���Ҿ����ڶ�ս���[���꣬����]�ġ���ħʹ�ߡ�����ȡ�����׬ȡ�������
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100022)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end
	if MoShenLegend_TimerMin==5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100022)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)

	elseif MoShenLegend_TimerMin==10 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100022)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	elseif MoShenLegend_TimerMin==50 then
	--����е�50����ʱ���Ź㲥��
	--����ħʹ�ߡ�����10���Ӻ󷵻���ͥ����Ҫ�������������뾡��ǰ������������ʱ���ռ���´λ���������
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100028)
		msg.AddMsgEvent(MsgID,5,5510403)
		msg.AddMsgEvent(MsgID,9,10)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	elseif MoShenLegend_TimerMin==55 then
	--����е�55����ʱ���Ź㲥��
	--����ħʹ�ߡ�����5���Ӻ󷵻���ͥ����Ҫ�������������뾡��ǰ������������ʱ���ռ���´λ���������
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100028)
		msg.AddMsgEvent(MsgID,5,5510403)
		msg.AddMsgEvent(MsgID,9,5)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end
end



function MoShenLegend_OnEnd(actID)	--�����
	--�жϻNPC�Ƿ��Ѵﵽ����ʱ���Զ���ʧ����δ��ʧ���ڳ�����ɾ����NPC�������ͷ�NPC��CreatureID
	if MoShen_CreatureID[1].CreatureID ~= nil and unit.IsDead(MoShen_CreatureID[1].MapID,-1,MoShen_CreatureID[1].CreatureID)==false then
		map.MapDeleteCreature(MoShen_CreatureID[1].MapID,-1,MoShen_CreatureID[1].CreatureID)
		MoShen_CreatureID[1].CreatureID = nil
	else
		MoShen_CreatureID[1].CreatureID = nil
	end

	MoShenLegend_Quest_IsComplete = {}
	MoShenLegend_CompleteQuestTime = 0
	MoShenLegend_TimerMin = 0
	--��������Ź㲥��
	--������ħ���������Ѿ���������δ����������ҿɼ����������񣬲����´λʱ�������񣡡�
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID,102,100023)
	msg.DispatchBroadcast(MsgID,-1,-1,-1)
end


--��ȡ����--
function q30201_OnQuestInit(MapID, InstanceID, QuestID, RoleID, NPCID)

	local index = 0
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	rolelevel = math.floor(rolelevel/5)
	if rolelevel <= 3 or rolelevel==nil then index = 1
	elseif rolelevel >= 20 then index = 17
	else index = rolelevel -2
	end
	--��ʼ������Ķ�̬����
	role.QuestInit(RoleID, QuestID, GetRandQuestTable(Quest_30201, index))
	--ͬ���ͻ�����������������˵��
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 11, QuestID)
	msg.AddMsgEvent(MsgID, 1, Quest_30201[index].MsgID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
aux.RegisterQuestEvent(30201, 7, "q30201_OnQuestInit")


function MoShenLegend_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)	--����������
	MoShenLegend_CompleteQuestTime = MoShenLegend_CompleteQuestTime + 1
	MoShenLegend_Quest_IsComplete[RoleID] = 1
	local index = 0
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)

	if rolelevel >= 80 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4820081, 25, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 3500011, 1, -1, 8, 420)
	elseif rolelevel >= 60 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4820081, 22, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 3500011, 1, -1, 8, 420)
	elseif rolelevel >= 40 then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 4820081, 20, -1, 8, 420)
	else
	    role.AddRoleItem(MapID, InstanceID, RoleID, 4820081, 5, -1, 8, 420)
    end

	rolelevel = math.floor(rolelevel/5)
	if rolelevel <= 3 or rolelevel==nil then index = 1
	elseif rolelevel >= 20 then index = 17
	else index = rolelevel -2
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 201004)--��ϲ�����<p1>�����ܼƻ�þ���<p2>�㼰��Ǯ<p3>��"
	msg.AddMsgEvent(MsgID, 11, QuestID)
	if MoShenLegend_CompleteQuestTime ==1 then	--��ҵ�һ��������񣬶�����������1������ͽ�Ǯ����
		role.AddRoleExp(MapID, InstanceID, RoleID, Quest_30201[index].RewXP)
		role.AddRoleSilver(MapID, InstanceID, RoleID, Quest_30201[index].RewMoney, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201001)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100024)
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewXP*2)
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewMoney*2)
	elseif MoShenLegend_CompleteQuestTime==10 then	--��ҵ�10��������񣬶�����������0.5������ͽ�Ǯ����
		local ex = math.floor(Quest_30201[index].RewXP/2)
		local go = math.floor(Quest_30201[index].RewMoney/2)
		role.AddRoleExp(MapID, InstanceID, RoleID, ex)
		role.AddRoleSilver(MapID, InstanceID, RoleID, go, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201002)--��ϲ���ڱ��λ�е�&lt;p1&gt;��������񣬶�����1.5�����齱����
		msg.AddMsgEvent(MsgID_1, 9, 10)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100025)
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewXP+ex)
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewMoney+go)
	elseif MoShenLegend_CompleteQuestTime==50 then	--��ҵ�50��������񣬶�����������0.5������ͽ�Ǯ����
		local ex = math.floor(Quest_30201[index].RewXP/2)
		local go = math.floor(Quest_30201[index].RewMoney/2)
		role.AddRoleExp(MapID, InstanceID, RoleID, ex)
		role.AddRoleSilver(MapID, InstanceID, RoleID, go, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201002)--��ϲ���ڱ��λ�е�&lt;p1&gt;��������񣬶�����1.5�����齱����
		msg.AddMsgEvent(MsgID_1, 9, 50)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100027)
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewXP+ex)
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewMoney+go)
	elseif MoShenLegend_CompleteQuestTime==100 then	--��ҵ�100��������񣬶�����������0.5������ͽ�Ǯ����
		local ex = math.floor(Quest_30201[index].RewXP/2)
		local go = math.floor(Quest_30201[index].RewMoney/2)
		role.AddRoleExp(MapID, InstanceID, RoleID, ex)
		role.AddRoleSilver(MapID, InstanceID, RoleID, go, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201002)--��ϲ���ڱ��λ�е�&lt;p1&gt;��������񣬶�����1.5�����齱����
		msg.AddMsgEvent(MsgID_1, 9, 100)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100026)
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewXP+ex)
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewMoney+go)
	else
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewXP)
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewMoney)
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)

	if role.GetTrialState(RoleID, 1010) == 1 then
		role.SetTrialCompleteNum(RoleID, 1010, role.GetTrialCompleteNum(RoleID,1010) + 1)
    end
	if role.GetTrialState(RoleID, 1010) == 1 and role.GetTrialCompleteNum(RoleID,1010) >= Trail_maxnum[1010] then
		role.TrialComplete(RoleID,1010)
	end

	role.NotifyFBBS(RoleID,14,0)
	--ħ�����
	UpdateSeverData(49, 1)
	--��Ҽ���
	UpdateRoleData(RoleID, 12, 1)
end



function MoShenLegend_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
    local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if rolelevel >= 50 then
		--�жϱ������пռ��Ƿ��㹻
		local FreeSize = role.GetBagFreeSize(RoleID)
		if(FreeSize < 2) then
			--��ʾ��ұ����ռ䲻��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)--��Ҫ����x�񱳰��ռ�ſɼ�������
			msg.AddMsgEvent(MsgID, 9, 2)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		end
	elseif rolelevel >= 40 then
		--�жϱ������пռ��Ƿ��㹻
		local FreeSize = role.GetBagFreeSize(RoleID)
		if(FreeSize < 1) then
			--��ʾ��ұ����ռ䲻��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)--��Ҫ����x�񱳰��ռ�ſɼ�������
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		end
	elseif rolelevel >= 30 then
	    --�жϱ������пռ��Ƿ��㹻
		local FreeSize = role.GetBagFreeSize(RoleID)
		if(FreeSize < 1) then
			--��ʾ��ұ����ռ䲻��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)--��Ҫ����x�񱳰��ռ�ſɼ�������
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		end
	end

	if MoShenLegend_Quest_IsComplete[RoleID] ~= nil then
		local MsgID_1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 26, 201003)--ÿ�λ�ڼ�ֻ�����һ������
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		return 0
	end
	return 1
end

function MoShenLegend_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	if MoShenLegend_Quest_IsComplete[RoleID] ~= nil then
		local MsgID_1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 26, 201005)--���������Ѿ���ɣ������ٽ�
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		return 0
	end
	return 1
end

--[[________________________ע������¼�________________________]]--

--���ʼ
aux.RegisterActEvent(9, 2, "MoShenLegend_OnStart")
aux.RegisterActEvent(10, 2, "MoShenLegend_OnStart")
aux.RegisterActEvent(11, 2, "MoShenLegend_OnStart")
aux.RegisterActEvent(12, 2, "MoShenLegend_OnStart")
--�����
aux.RegisterActEvent(9, 3, "MoShenLegend_OnEnd")
aux.RegisterActEvent(10, 3, "MoShenLegend_OnEnd")
aux.RegisterActEvent(11, 3, "MoShenLegend_OnEnd")
aux.RegisterActEvent(12, 3, "MoShenLegend_OnEnd")

--��ԡ����ӡ�Ϊ��λ��ʱ
aux.RegisterActEvent(9, 4, "MoShenLegend_OnTimerMin")
aux.RegisterActEvent(10, 4, "MoShenLegend_OnTimerMin")
aux.RegisterActEvent(11, 4, "MoShenLegend_OnTimerMin")
aux.RegisterActEvent(12, 4, "MoShenLegend_OnTimerMin")



--�����ɡ�ħ�������������
aux.RegisterQuestEvent(30201, 1, "MoShenLegend_OnComplete")

--�������Ƿ�ɽ�ȡ��ħ�������������
aux.RegisterQuestEvent(30201, 4, "MoShenLegend_OnCheckAccept")

--�������Ƿ�ɽ�ȡ��ħ�������������
aux.RegisterQuestEvent(30201, 5, "MoShenLegend_OnCheckComplete")

