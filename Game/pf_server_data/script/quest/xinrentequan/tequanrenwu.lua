--��ȡ����
function q30724_OnQuestInit(MapID, InstanceID, QuestID, RoleID, NPCID)

	--��������
	local Quest_TeQuan = {}
	Quest_TeQuan[10]={TargetType=QuestTargetType["Kill"],TargetID1=1020001,Num1=50,RewMoney=8260,RewXP=3800,MsgID=17001}    --�߻�֮�� �õ�
	Quest_TeQuan[11]={TargetType=QuestTargetType["Kill"],TargetID1=1020002,Num1=60,RewMoney=9910,RewXP=4560,MsgID=17002}    --�߻�֮�� ���
	Quest_TeQuan[12]={TargetType=QuestTargetType["Kill"],TargetID1=1020003,Num1=60,RewMoney=10820,RewXP=4970,MsgID=17003}    --�߻�֮�� ��ʳ����
	Quest_TeQuan[13]={TargetType=QuestTargetType["Kill"],TargetID1=1020004,Num1=60,RewMoney=11390,RewXP=5370,MsgID=17004}    --�߻�֮�� ������
	Quest_TeQuan[14]={TargetType=QuestTargetType["Kill"],TargetID1=1020005,Num1=60,RewMoney=11960,RewXP=5780,MsgID=17005}    --�߻�֮�� ʳ����
	Quest_TeQuan[15]={TargetType=QuestTargetType["Kill"],TargetID1=1020006,Num1=80,RewMoney=16720,RewXP=8260,MsgID=17006}    --�߻�֮�� �Ե�
	Quest_TeQuan[16]={TargetType=QuestTargetType["Kill"],TargetID1=1020007,Num1=80,RewMoney=17500,RewXP=8820,MsgID=17007}    --�߻�֮�� ��Ұ��
	Quest_TeQuan[17]={TargetType=QuestTargetType["Kill"],TargetID1=1020008,Num1=80,RewMoney=18280,RewXP=9380,MsgID=17008}    --��˹̹��Ѩ ��ѨҰ��
	Quest_TeQuan[18]={TargetType=QuestTargetType["Kill"],TargetID1=1020009,Num1=80,RewMoney=18840,RewXP=9950,MsgID=17009}    --��˹̹��Ѩ ��Ѩħ��
	Quest_TeQuan[19]={TargetType=QuestTargetType["Kill"],TargetID1=1020010,Num1=80,RewMoney=19630,RewXP=10520,MsgID=17010}    --�߻�֮�� ��Ұ��
	Quest_TeQuan[20]={TargetType=QuestTargetType["Kill"],TargetID1=1020010,Num1=80,RewMoney=19630,RewXP=10520,MsgID=17011}    --�߻�֮�� ��Ұ��
	Quest_TeQuan[21]={TargetType=QuestTargetType["Kill"],TargetID1=1030002,Num1=100,RewMoney=26250,RewXP=14590,MsgID=17012}    --�����ѹ� �ѹȺ���
	Quest_TeQuan[22]={TargetType=QuestTargetType["Kill"],TargetID1=1030003,Num1=100,RewMoney=27270,RewXP=15320,MsgID=17013}    --�����ѹ� �ѹ�ħ��
	Quest_TeQuan[23]={TargetType=QuestTargetType["Kill"],TargetID1=1030004,Num1=100,RewMoney=28000,RewXP=16060,MsgID=17014}    --�����ѹ� Сʯ��
	Quest_TeQuan[24]={TargetType=QuestTargetType["Kill"],TargetID1=1030005,Num1=100,RewMoney=29030,RewXP=16800,MsgID=17015}    --�����ѹ� ��ŭ�ľ�ʯ
	Quest_TeQuan[25]={TargetType=QuestTargetType["Kill"],TargetID1=1030006,Num1=100,RewMoney=29770,RewXP=17550,MsgID=17016}    --�����ѹ� �ѹȰ���
	Quest_TeQuan[26]={TargetType=QuestTargetType["Kill"],TargetID1=1030007,Num1=100,RewMoney=30510,RewXP=18310,MsgID=17017}    --�����ѹ� ����������
	Quest_TeQuan[27]={TargetType=QuestTargetType["Kill"],TargetID1=1030008,Num1=100,RewMoney=31560,RewXP=19070,MsgID=17018}    --�����ѹ� �ѹ�ħ��
	Quest_TeQuan[28]={TargetType=QuestTargetType["Kill"],TargetID1=1030009,Num1=100,RewMoney=32320,RewXP=19840,MsgID=17019}    --���涴 ������
	Quest_TeQuan[29]={TargetType=QuestTargetType["Kill"],TargetID1=1030011,Num1=100,RewMoney=33080,RewXP=20610,MsgID=17020}    --���涴 ����ʯԪ��
	Quest_TeQuan[30]={TargetType=QuestTargetType["Kill"],TargetID1=1030013,Num1=150,RewMoney=51230,RewXP=32090,MsgID=17021}    --�����ѹ� ���׾�ʯ
	Quest_TeQuan[31]={TargetType=QuestTargetType["Kill"],TargetID1=1040003,Num1=150,RewMoney=52390,RewXP=33270,MsgID=17022}    --������ѩ ѩ��ǿ��
	Quest_TeQuan[32]={TargetType=QuestTargetType["Kill"],TargetID1=1040004,Num1=150,RewMoney=53560,RewXP=35500,MsgID=17023}    --������ѩ аħ��Ů
	Quest_TeQuan[33]={TargetType=QuestTargetType["Kill"],TargetID1=1040006,Num1=150,RewMoney=54730,RewXP=38100,MsgID=17024}    --������ ����
	Quest_TeQuan[34]={TargetType=QuestTargetType["Kill"],TargetID1=1040007,Num1=150,RewMoney=55910,RewXP=40720,MsgID=17025}    --������ ����
	Quest_TeQuan[35]={TargetType=QuestTargetType["Kill"],TargetID1=1040008,Num1=150,RewMoney=57580,RewXP=43360,MsgID=17026}    --������ѩ ��������
	Quest_TeQuan[36]={TargetType=QuestTargetType["Kill"],TargetID1=1040010,Num1=150,RewMoney=58780,RewXP=46020,MsgID=17027}    --������ѩ ��������
	Quest_TeQuan[37]={TargetType=QuestTargetType["Kill"],TargetID1=1040012,Num1=150,RewMoney=59990,RewXP=48710,MsgID=17028}    --������ѩ ����������
	Quest_TeQuan[38]={TargetType=QuestTargetType["Kill"],TargetID1=1040022,Num1=200,RewMoney=81610,RewXP=68550,MsgID=17029}    --������ѩ ��Ѫ����
	Quest_TeQuan[39]={TargetType=QuestTargetType["Kill"],TargetID1=1040021,Num1=200,RewMoney=83240,RewXP=72660,MsgID=17030}    --������ѩ ħ��
	Quest_TeQuan[40]={TargetType=QuestTargetType["Kill"],TargetID1=1040024,Num1=200,RewMoney=84890,RewXP=76800,MsgID=17031}    --���Ķ� ��������
	Quest_TeQuan[41]={TargetType=QuestTargetType["Kill"],TargetID1=1040013,Num1=150,RewMoney=66160,RewXP=63890,MsgID=17032}    --������ѩ ����
	Quest_TeQuan[42]={TargetType=QuestTargetType["Kill"],TargetID1=1040013,Num1=150,RewMoney=66160,RewXP=63890,MsgID=17033}    --������ѩ ����
	Quest_TeQuan[43]={TargetType=QuestTargetType["Kill"],TargetID1=1040015,Num1=200,RewMoney=89890,RewXP=89910,MsgID=17034}    --������ѩ ���ĵ�����
	Quest_TeQuan[44]={TargetType=QuestTargetType["Kill"],TargetID1=1040017,Num1=200,RewMoney=91570,RewXP=94680,MsgID=17035}    --������ѩ ������
	Quest_TeQuan[45]={TargetType=QuestTargetType["Kill"],TargetID1=1040018,Num1=200,RewMoney=93270,RewXP=99470,MsgID=17036}    --������ѩ ���ܵķ�ͽ
	Quest_TeQuan[46]={TargetType=QuestTargetType["Kill"],TargetID1=1040020,Num1=200,RewMoney=94980,RewXP=104310,MsgID=17037}    --������ѩ ��ħ��Ů
	Quest_TeQuan[47]={TargetType=QuestTargetType["Kill"],TargetID1=1040026,Num1=200,RewMoney=96030,RewXP=109180,MsgID=17038}    --������ѩ ħڤ��ʦ
	Quest_TeQuan[48]={TargetType=QuestTargetType["Kill"],TargetID1=1040028,Num1=200,RewMoney=97760,RewXP=114080,MsgID=17039}    --������ѩ �����콫
	Quest_TeQuan[49]={TargetType=QuestTargetType["Kill"],TargetID1=1040031,Num1=250,RewMoney=124380,RewXP=149400,MsgID=17040}    --������ѩ ���ް�ռ��
	Quest_TeQuan[50]={TargetType=QuestTargetType["Kill"],TargetID1=1050001,Num1=250,RewMoney=126560,RewXP=156250,MsgID=17041}    --���޻��� ����Ұ��
	Quest_TeQuan[51]={TargetType=QuestTargetType["Kill"],TargetID1=1050003,Num1=250,RewMoney=128760,RewXP=163150,MsgID=17042}    --���޻��� ���ֵ���
	Quest_TeQuan[52]={TargetType=QuestTargetType["Kill"],TargetID1=1050004,Num1=250,RewMoney=130980,RewXP=170100,MsgID=17043}    --���޻��� ������ͽ
	Quest_TeQuan[53]={TargetType=QuestTargetType["Kill"],TargetID1=1050006,Num1=250,RewMoney=132350,RewXP=177740,MsgID=17044}    --���޻��� ����ħ��
	Quest_TeQuan[54]={TargetType=QuestTargetType["Kill"],TargetID1=1050008,Num1=250,RewMoney=134590,RewXP=185420,MsgID=17045}    --���޻��� ħ����
	Quest_TeQuan[55]={TargetType=QuestTargetType["Kill"],TargetID1=1050009,Num1=250,RewMoney=136840,RewXP=193170,MsgID=17046}    --���޻��� �����ʿ
	Quest_TeQuan[56]={TargetType=QuestTargetType["Kill"],TargetID1=1050011,Num1=250,RewMoney=139100,RewXP=200960,MsgID=17047}    --���� ������Ů
	Quest_TeQuan[57]={TargetType=QuestTargetType["Kill"],TargetID1=1050012,Num1=250,RewMoney=140510,RewXP=208820,MsgID=17048}    --���� ������ʦ
	Quest_TeQuan[58]={TargetType=QuestTargetType["Kill"],TargetID1=1050015,Num1=250,RewMoney=142800,RewXP=216720,MsgID=17049}    --���޻��� ���ֺ�����
	Quest_TeQuan[59]={TargetType=QuestTargetType["Kill"],TargetID1=1050016,Num1=300,RewMoney=174130,RewXP=270400,MsgID=17050}    --���޻��� ��������


	local Index = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if Index >59 then
	    Index = 59
	elseif Index < 10 then
	    Index = 10
	end

	--��ʼ������Ķ�̬����
	role.QuestInit(RoleID, QuestID, GetRandQuestTable(Quest_TeQuan, Index))
	--ͬ���ͻ�����������������˵��
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 11, QuestID)
	msg.AddMsgEvent(MsgID, 1, Quest_TeQuan[Index].MsgID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end

--�������
function q30724_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

	role.SetRoleScriptData(RoleID, 1, RoleDataType["Tequanrenwu"], role.GetRoleScriptData(RoleID, 1, RoleDataType["Tequanrenwu"])+1)

end

aux.RegisterQuestEvent(30724, 1, "q30724_OnComplete")
aux.RegisterQuestEvent(30724, 7, "q30724_OnQuestInit")


--��Ȩ���������ȡ
function I4400503_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddQuest(TargetID, 30724)
end

function I4400503_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local CurTime = tonumber(os.date("%j"))
	local LastTime = math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["Tequanrenwu"])/10)
	if CurTime ~= LastTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Tequanrenwu"], CurTime*10)
	end
	local Num = role.GetRoleScriptData(RoleID, 1, RoleDataType["Tequanrenwu"])%10
	local a = role.GetRoleQuestNum(RoleID)

	if Num >= 5 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206426)--ÿ��ֻ�����5����Ȩ��������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 32,false
	elseif role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30724) then
	    return 32,false
	elseif a >= 30 then
		return 21,false
	else
		return 0,false
	end
end

aux.RegisterItemEvent(4400503, 1, "I4400503_OnUse")
aux.RegisterItemEvent(4400503, 0, "I4400503_CanUse")

function tequan_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	if aux.IsOldSerNewPlayer(RoleID) and role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69370) then
		return 1
	else
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206427)--ֻ��ӵ��������Ȩ����Ҳ��ܽ�ȡ������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    return 0
	end
end

aux.RegisterQuestEvent(30724, 4, "tequan_OnCheckAccept")
aux.RegisterQuestEvent(30725, 4, "tequan_OnCheckAccept")
aux.RegisterQuestEvent(30726, 4, "tequan_OnCheckAccept")
aux.RegisterQuestEvent(30727, 4, "tequan_OnCheckAccept")
aux.RegisterQuestEvent(30728, 4, "tequan_OnCheckAccept")
aux.RegisterQuestEvent(30729, 4, "tequan_OnCheckAccept")
aux.RegisterQuestEvent(30730, 4, "tequan_OnCheckAccept")
aux.RegisterQuestEvent(30731, 4, "tequan_OnCheckAccept")
aux.RegisterQuestEvent(30732, 4, "tequan_OnCheckAccept")
aux.RegisterQuestEvent(30733, 4, "tequan_OnCheckAccept")
aux.RegisterQuestEvent(30734, 4, "tequan_OnCheckAccept")
aux.RegisterQuestEvent(30735, 4, "tequan_OnCheckAccept")
aux.RegisterQuestEvent(30736, 4, "tequan_OnCheckAccept")
aux.RegisterQuestEvent(30737, 4, "tequan_OnCheckAccept")
