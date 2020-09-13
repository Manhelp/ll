
--���ڱ������ڱ��λ���Ƿ�����ɹ������ȫ��table�������RoleIDΪ����
ZhanShenLegend_Quest_IsComplete = {}
--��¼ÿ�λ��������ɵ��ܴ���
ZhanShenLegend_CompleteQuestTime = 0
--�������б�ˢ���ĻNPC�����ս�񡱵�CreatureID
ZhanShen_CreatureID = {}

--����ÿ����ˢ����NPC����Ϣ
ZhanShen_CreatureID[1] = {MapID=3424073512,x=505,y=23167,z=304,CreatureID=0}

ZhanShenLegend_TimerMin = 0		--��¼������˶೤ʱ��

function ZhanShenLegend_OnStart(actID)	--���ʼ
	local TempTable = {}
	ZhanShenLegend_Quest_IsComplete = deepcopy(TempTable)

	--��ʼ���ȫ�ֱ���
	ZhanShenLegend_TimerMin = 0
	ZhanShenLegend_CompleteQuestTime = 0

	--ˢ���NPC ���ս��[5510402]
	ZhanShen_CreatureID[1].CreatureID = map.MapCreateColCreature(ZhanShen_CreatureID[1].MapID, -1, 5510402, ZhanShen_CreatureID[1].x, ZhanShen_CreatureID[1].y, ZhanShen_CreatureID[1].z,1)

	--���ʼ���Ź㲥��
	--������ս��˵���ʼ����λ��ҿ��ڶ�ս���[���꣬����]�ҡ����ս�񡱽�ȡ�������ɺ���з��Ľ�������
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID,102,100015)
	msg.DispatchBroadcast(MsgID,-1,-1,-1)
end

function ZhanShenLegend_OnTimerMin(actID)	--�ÿ����1����
	ZhanShenLegend_TimerMin = ZhanShenLegend_TimerMin+1
	if ZhanShen_CreatureID[1].CreatureID == 0 then
		ZhanShen_CreatureID[1].CreatureID = map.MapCreateCreature(ZhanShen_CreatureID[1].MapID, -1, 5510402, ZhanShen_CreatureID[1].x, ZhanShen_CreatureID[1].y, ZhanShen_CreatureID[1].z)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100015)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end
--ս��˵����ڽ����У�����10�����ϵ���Ҿ����ڶ�ս���[���꣬����]�ġ����ս�񡱴���ȡ�����׬ȡ�������
	if ZhanShenLegend_TimerMin==5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100016)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	elseif ZhanShenLegend_TimerMin==10 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100016)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	elseif ZhanShenLegend_TimerMin==50 then
	--����е�50����ʱ���Ź㲥��
	--�����ս�񡱽���10���Ӻ󷵻���ͥ����Ҫ�������������뾡��ǰ������������ʱ���ռ���´λ���������
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100028)
		msg.AddMsgEvent(MsgID,5,5510402)
		msg.AddMsgEvent(MsgID,9,10)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	elseif ZhanShenLegend_TimerMin==55 then
	--����е�55����ʱ
	--���Ź㲥��
	--�����ս�񡱽���5���Ӻ󷵻���ͥ����Ҫ�������������뾡��ǰ������������ʱ���ռ���´λ���������
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100028)
		msg.AddMsgEvent(MsgID,5,5510402)
		msg.AddMsgEvent(MsgID,9,5)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end
end

function ZhanShenLegend_OnEnd(actID)	--�����

	--�жϻNPC�Ƿ��Ѵﵽ����ʱ���Զ���ʧ����δ��ʧ���ڳ�����ɾ����NPC�������ͷ�NPC��CreatureID
	if ZhanShen_CreatureID[1].CreatureID ~= nil and unit.IsDead(ZhanShen_CreatureID[1].MapID,-1,ZhanShen_CreatureID[1].CreatureID)==false then
		map.MapDeleteCreature(ZhanShen_CreatureID[1].MapID,-1,ZhanShen_CreatureID[1].CreatureID)
		ZhanShen_CreatureID[1].CreatureID = nil
	else
		ZhanShen_CreatureID[1].CreatureID = nil
	end

	ZhanShenLegend_Quest_IsComplete = {}
	ZhanShenLegend_CompleteQuestTime = 0
	ZhanShenLegend_TimerMin = 0
	--��������Ź㲥��
	--������ս��˵��Ѿ���������δ����������ҿɼ����������񣬲����´λʱ�������񣡡�
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID,102,100017)
	msg.DispatchBroadcast(MsgID,-1,-1,-1)
end



local Quest_30202 = {}
Quest_30202[1]={TargetType=QuestTargetType["Kill"],TargetID1=1020004,Num1=10,TargetID2=1020005,Num2=10,TargetID3=1020014,Num3=3,RewMoney=6975,RewXP=7020,RewItemID1=4080067,value1=1,MsgID=12001,}--
Quest_30202[2]={TargetType=QuestTargetType["Kill"],TargetID1=1030002,Num1=20,TargetID2=1030003,Num2=20,TargetID3=1030014,Num3=3,RewMoney=15375,RewXP=20700,RewItemID1=4080067,value1=1,MsgID=12002,}--
Quest_30202[3]={TargetType=QuestTargetType["Kill"],TargetID1=1030007,Num1=20,TargetID2=1030006,Num2=20,TargetID3=1030017,Num3=3,RewMoney=17000,RewXP=25200,RewItemID1=4080067,value1=1,MsgID=12003,}--
Quest_30202[4]={TargetType=QuestTargetType["Kill"],TargetID1=1040001,Num1=30,TargetID2=1040003,Num2=30,TargetID3=1040033,Num3=5,RewMoney=29800,RewXP=39900,RewItemID1=4080067,value1=1,MsgID=12004,}--
Quest_30202[5]={TargetType=QuestTargetType["Kill"],TargetID1=1040012,Num1=30,TargetID2=1040021,Num2=30,TargetID3=1040035,Num3=5,RewMoney=32400,RewXP=50400,RewItemID1=4080067,value1=1,MsgID=12005,}--
Quest_30202[6]={TargetType=QuestTargetType["Kill"],TargetID1=1040024,Num1=40,TargetID2=1040025,Num2=40,TargetID3=1040036,Num3=5,RewMoney=43500,RewXP=75600,RewItemID1=4080067,value1=1,MsgID=12006,}--
Quest_30202[7]={TargetType=QuestTargetType["Kill"],TargetID1=1040026,Num1=40,TargetID2=1040027,Num2=40,TargetID3=1040038,Num3=5,RewMoney=46500,RewXP=90300,RewItemID1=4080067,value1=1,MsgID=12007,}--
Quest_30202[8]={TargetType=QuestTargetType["Kill"],TargetID1=1050001,Num1=40,TargetID2=1050003,Num2=40,TargetID3=1050027,Num3=5,RewMoney=49250,RewXP=198450,RewItemID1=4080067,value1=1,MsgID=12008,}--
Quest_30202[9]={TargetType=QuestTargetType["Kill"],TargetID1=1050010,Num1=40,TargetID2=1050012,Num2=40,TargetID3=1050029,Num3=5,RewMoney=52000,RewXP=231525,RewItemID1=4080067,value1=1,MsgID=12009,}--
Quest_30202[10]={TargetType=QuestTargetType["Kill"],TargetID1=1050017,Num1=60,TargetID2=1050018,Num2=60,TargetID3=1050034,Num3=10,RewMoney=82125,RewXP=269325,RewItemID1=4080067,value1=1,MsgID=12010,}--
Quest_30202[11]={TargetType=QuestTargetType["Kill"],TargetID1=1060008,Num1=60,TargetID2=1060007,Num2=60,TargetID3=1060034,Num3=10,RewMoney=85875,RewXP=349650,RewItemID1=4080067,value1=1,MsgID=12011,}--
Quest_30202[12]={TargetType=QuestTargetType["Kill"],TargetID1=1060016,Num1=25,TargetID2=1060017,Num2=25,TargetID3=1060040,Num3=15,RewMoney=108000,RewXP=481950,RewItemID1=4080067,value1=1,MsgID=12012,}--
Quest_30202[13]={TargetType=QuestTargetType["Kill"],TargetID1=1060023,Num1=70,TargetID2=1060028,Num2=70,TargetID3=1060044,Num3=15,RewMoney=112500,RewXP=633150,RewItemID1=4080067,value1=1,MsgID=12013,}--
Quest_30202[14]={TargetType=QuestTargetType["Kill"],TargetID1=1070002,Num1=80,TargetID2=1070009,Num2=80,TargetID3=1070053,Num3=15,RewMoney=130000,RewXP=819000,RewItemID1=4080067,value1=1,MsgID=12014,}--
Quest_30202[15]={TargetType=QuestTargetType["Kill"],TargetID1=1070015,Num1=80,TargetID2=1070016,Num2=80,TargetID3=1070056,Num3=15,RewMoney=135000,RewXP=1015875,RewItemID1=4080067,value1=1,MsgID=12015,}--
Quest_30202[16]={TargetType=QuestTargetType["Kill"],TargetID1=1070028,Num1=20,TargetID2=1070029,Num2=20,TargetID3=1070050,Num3=20,RewMoney=174375,RewXP=1239525,RewItemID1=4080067,value1=1,MsgID=12016,}--
Quest_30202[17]={TargetType=QuestTargetType["Kill"],TargetID1=1070035,Num1=100,TargetID2=1070042,Num2=100,TargetID3=1070069,Num3=15,RewMoney=180000,RewXP=1489950,RewItemID1=4080067,value1=1,MsgID=12017,}--


function ZhanShenLegend_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)	--����������
	ZhanShenLegend_Quest_IsComplete[RoleID] = 1
	ZhanShenLegend_CompleteQuestTime = ZhanShenLegend_CompleteQuestTime + 1

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
	if ZhanShenLegend_CompleteQuestTime==1 then	--��ҵ�һ��������񣬶�����������1������ͽ�Ǯ����
		role.AddRoleExp(MapID, InstanceID, RoleID, Quest_30202[index].RewXP)
		role.AddRoleSilver(MapID, InstanceID, RoleID, Quest_30202[index].RewMoney, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201001)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100018)	--��ϲ��ҡ�<p1>����һ����ɱ���ս��˵����񣬻��˫���������Ǯ������
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, Quest_30202[index].RewXP*2)
		msg.AddMsgEvent(MsgID, 9, Quest_30202[index].RewMoney*2)
	elseif ZhanShenLegend_CompleteQuestTime==10 then	--��ҵ�10��������񣬶�����������0.5������ͽ�Ǯ����
		local ex = math.floor(Quest_30202[index].RewXP/2)
		local go = math.floor(Quest_30202[index].RewMoney/2)
		role.AddRoleExp(MapID, InstanceID, RoleID, ex)
		role.AddRoleSilver(MapID, InstanceID, RoleID, go, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201002)--��ϲ���ڱ��λ�е�&lt;p1&gt;��������񣬶�����1.5�����齱����
		msg.AddMsgEvent(MsgID_1, 9, 10)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100019)	--��ҡ�<p1>���ڱ���ħ���������е�ʮ������˻���񣬻��1.5���������Ǯ������
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, Quest_30202[index].RewXP+ex)
		msg.AddMsgEvent(MsgID, 9, Quest_30202[index].RewMoney+go)
	elseif ZhanShenLegend_CompleteQuestTime==100 then	--��ҵ�100��������񣬶�����������0.5������ͽ�Ǯ����
		local ex = math.floor(Quest_30202[index].RewXP/2)
		local go = math.floor(Quest_30202[index].RewMoney/2)
		role.AddRoleExp(MapID, InstanceID, RoleID, ex)
		role.AddRoleSilver(MapID, InstanceID, RoleID, go, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201002)--��ϲ���ڱ��λ�е�&lt;p1&gt;��������񣬶�����1.5�����齱����
		msg.AddMsgEvent(MsgID_1, 9, 100)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100020)
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, Quest_30202[index].RewXP+ex)
		msg.AddMsgEvent(MsgID, 9, Quest_30202[index].RewMoney+go)
	else
		msg.AddMsgEvent(MsgID, 9, Quest_30202[index].RewXP)
		msg.AddMsgEvent(MsgID, 9, Quest_30202[index].RewMoney)
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)

	if role.GetTrialState(RoleID, 1008) == 1 then
		role.SetTrialCompleteNum(RoleID, 1008, role.GetTrialCompleteNum(RoleID,1008) + 1)
    end
	if role.GetTrialState(RoleID, 1008) == 1 and role.GetTrialCompleteNum(RoleID,1008) >= Trail_maxnum[1008] then
		role.TrialComplete(RoleID,1008)
	end

	role.NotifyFBBS(RoleID,15,0)
	--ս�����
	UpdateSeverData(49, 1)
	--��Ҽ���
	UpdateRoleData(RoleID, 12, 1)
end

--��ȡ����--
function q30202_OnQuestInit(MapID, InstanceID, QuestID, RoleID, NPCID)

	local index = 0
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	rolelevel = math.floor(rolelevel/5)
	if rolelevel <= 3 or rolelevel==nil then index = 1
	elseif rolelevel >= 20 then index = 17
	else index = rolelevel -2
	end
	--��ʼ������Ķ�̬����
	role.QuestInit(RoleID, QuestID, GetRandQuestTable(Quest_30202, index))
	--ͬ���ͻ�����������������˵��
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 11, QuestID)
	msg.AddMsgEvent(MsgID, 1, Quest_30202[index].MsgID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
aux.RegisterQuestEvent(30202, 7, "q30202_OnQuestInit")



function ZhanShenLegend_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	if ZhanShenLegend_Quest_IsComplete[RoleID] ~= nil then
		local MsgID_1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 26, 201005)--���������Ѿ���ɣ������ٽ�
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		return 0
	end
	return 1
end

function ZhanShenLegend_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

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

	if ZhanShenLegend_Quest_IsComplete[RoleID] ~= nil then
		local MsgID_1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 26, 201003)--ÿ�λ�ڼ�ֻ�����һ������
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		return 0
	end

	return 1
end

--[[________________________ע������¼�________________________]]--

--���ʼ
aux.RegisterActEvent(5, 2, "ZhanShenLegend_OnStart")
aux.RegisterActEvent(6, 2, "ZhanShenLegend_OnStart")
aux.RegisterActEvent(7, 2, "ZhanShenLegend_OnStart")
aux.RegisterActEvent(8, 2, "ZhanShenLegend_OnStart")

--�����
aux.RegisterActEvent(5, 3, "ZhanShenLegend_OnEnd")
aux.RegisterActEvent(6, 3, "ZhanShenLegend_OnEnd")
aux.RegisterActEvent(7, 3, "ZhanShenLegend_OnEnd")
aux.RegisterActEvent(8, 3, "ZhanShenLegend_OnEnd")

--��ԡ����ӡ�Ϊ��λ��ʱ
aux.RegisterActEvent(5, 4, "ZhanShenLegend_OnTimerMin")
aux.RegisterActEvent(6, 4, "ZhanShenLegend_OnTimerMin")
aux.RegisterActEvent(7, 4, "ZhanShenLegend_OnTimerMin")
aux.RegisterActEvent(8, 4, "ZhanShenLegend_OnTimerMin")

--�����ɡ�ս��˵������
aux.RegisterQuestEvent(30202, 1, "ZhanShenLegend_OnComplete")

aux.RegisterQuestEvent(30202, 4, "ZhanShenLegend_OnCheckAccept")

--�������Ƿ�ɽ�ȡ��ս��˵������
aux.RegisterQuestEvent(30202, 5, "ZhanShenLegend_OnCheckComplete")
