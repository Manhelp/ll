--RoleDataType["XinyangCollect"]  < 20
--��ȡ����
function q30307_OnQuestInit(MapID, InstanceID, QuestID, RoleID, NPCID)

	--��������
	local Quest_Level = {
		11,11,14,14,16,16,19,19,21,21,24,24,26,26,29,29,31,31,34,34,36,36,39,39,41,41,41,41,44,44,44,44,46,46,46,46,49,49,
		49,49,51,51,51,51,51,54,54,54,54,54,56,56,56,56,56,59,59,59,59,59,61,61,61,61,61,61,61,61,64,64,64,64,64,64,64,64,
		66,66,66,66,66,66,66,66,69,69,69,69,69,69,69,69,71,71,71,71,71,71,71,71,74,74,74,74,74,74,74,74,74,76,76,76,76,76,
		76,76,76,76,79,79,79,79,79,79,79,79,79,81,81,81,81,81,81,81,81,81,84,84,84,84,84,84,84,84,84,86,86,86,86,86,86,86,
		86,86,89,89,89,89,89,89,89,89,89,91,91,91,91,91,91,91,91,91,94,94,94,94,94,94,94,94,94,96,96,96,96,96,96,96,96,96,
		99,99,99,99,99,99,99,99,99,100,100,100,100,100,100,100,100,100
	}
	local Index = 0
	local Max = 1
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	for k,v in pairs(Quest_Level) do
		if rolelevel > v then
			Max = k
		end
	end

	if	Max <= 10 then
		Index = math.random(1,Max)
	else
		Index = math.random(Max-10,Max)
	end

	local Quest_RandomCollect = {}
	Quest_RandomCollect[1]={TargetType=QuestTargetType["Collect"],TargetID1=4010001,Num1=10,RewMoney=960,RewXP=740,MsgID=19001}--������ʯ
	Quest_RandomCollect[2]={TargetType=QuestTargetType["Collect"],TargetID1=4010020,Num1=3,RewMoney=3530,RewXP=2060,MsgID=19002}--��������
	Quest_RandomCollect[3]={TargetType=QuestTargetType["Collect"],TargetID1=4010001,Num1=12,RewMoney=1390,RewXP=900,MsgID=19003}--������ʯ
	Quest_RandomCollect[4]={TargetType=QuestTargetType["Collect"],TargetID1=4010020,Num1=3,RewMoney=4260,RewXP=2100,MsgID=19004}--��������
	Quest_RandomCollect[5]={TargetType=QuestTargetType["Collect"],TargetID1=4010002,Num1=12,RewMoney=1520,RewXP=1200,MsgID=19005}--��������
	Quest_RandomCollect[6]={TargetType=QuestTargetType["Collect"],TargetID1=4010021,Num1=3,RewMoney=4670,RewXP=2800,MsgID=19006}--���ĺ��ο�
	Quest_RandomCollect[7]={TargetType=QuestTargetType["Collect"],TargetID1=4010002,Num1=15,RewMoney=2130,RewXP=1520,MsgID=19007}--��������
	Quest_RandomCollect[8]={TargetType=QuestTargetType["Collect"],TargetID1=4010021,Num1=4,RewMoney=6980,RewXP=3800,MsgID=19008}--���ĺ��ο�
	Quest_RandomCollect[9]={TargetType=QuestTargetType["Collect"],TargetID1=4010003,Num1=15,RewMoney=2280,RewXP=2020,MsgID=19009}--������ʯ
	Quest_RandomCollect[10]={TargetType=QuestTargetType["Collect"],TargetID1=4010022,Num1=4,RewMoney=7470,RewXP=5020,MsgID=19010}--����֮ɳ
	Quest_RandomCollect[11]={TargetType=QuestTargetType["Collect"],TargetID1=4010003,Num1=18,RewMoney=3020,RewXP=2440,MsgID=19011}--������ʯ
	Quest_RandomCollect[12]={TargetType=QuestTargetType["Collect"],TargetID1=4010022,Num1=5,RewMoney=10330,RewXP=6360,MsgID=19012}--����֮ɳ
	Quest_RandomCollect[13]={TargetType=QuestTargetType["Collect"],TargetID1=4010004,Num1=18,RewMoney=3180,RewXP=3060,MsgID=19013}--���뱦��
	Quest_RandomCollect[14]={TargetType=QuestTargetType["Collect"],TargetID1=4010023,Num1=5,RewMoney=10850,RewXP=7940,MsgID=19014}--���֮��
	Quest_RandomCollect[15]={TargetType=QuestTargetType["Collect"],TargetID1=4010004,Num1=20,RewMoney=3830,RewXP=3440,MsgID=19015}--���뱦��
	Quest_RandomCollect[16]={TargetType=QuestTargetType["Collect"],TargetID1=4010023,Num1=5,RewMoney=11770,RewXP=8040,MsgID=19016}--���֮��
	Quest_RandomCollect[17]={TargetType=QuestTargetType["Collect"],TargetID1=4010005,Num1=22,RewMoney=4440,RewXP=4540,MsgID=19017}--ѩԭ��ʯ
	Quest_RandomCollect[18]={TargetType=QuestTargetType["Collect"],TargetID1=4010024,Num1=6,RewMoney=14910,RewXP=11620,MsgID=19018}--ѩɽ��¶
	Quest_RandomCollect[19]={TargetType=QuestTargetType["Collect"],TargetID1=4010005,Num1=24,RewMoney=5170,RewXP=5020,MsgID=19019}--ѩԭ��ʯ
	Quest_RandomCollect[20]={TargetType=QuestTargetType["Collect"],TargetID1=4010024,Num1=6,RewMoney=15910,RewXP=11760,MsgID=19020}--ѩɽ��¶
	Quest_RandomCollect[21]={TargetType=QuestTargetType["Collect"],TargetID1=4010006,Num1=26,RewMoney=5890,RewXP=7260,MsgID=19021}--ѩԭ��ʯ
	Quest_RandomCollect[22]={TargetType=QuestTargetType["Collect"],TargetID1=4010025,Num1=7,RewMoney=19510,RewXP=18300,MsgID=19022}--ѩԭ�й�
	Quest_RandomCollect[23]={TargetType=QuestTargetType["Collect"],TargetID1=4010006,Num1=28,RewMoney=6740,RewXP=7920,MsgID=19023}--ѩԭ��ʯ
	Quest_RandomCollect[24]={TargetType=QuestTargetType["Collect"],TargetID1=4010025,Num1=7,RewMoney=20720,RewXP=18540,MsgID=19024}--ѩԭ�й�
	Quest_RandomCollect[25]={TargetType=QuestTargetType["Collect"],TargetID1=4010007,Num1=30,RewMoney=7510,RewXP=11120,MsgID=19025}--ѩ�����
	Quest_RandomCollect[26]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=40,RewMoney=36670,RewXP=17380,MsgID=19026}--�ᾧ
	Quest_RandomCollect[27]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=100,RewMoney=71630,RewXP=27160,MsgID=19027}--ԭ��
	Quest_RandomCollect[28]={TargetType=QuestTargetType["Collect"],TargetID1=4010026,Num1=8,RewMoney=24620,RewXP=27780,MsgID=19028}--����ԭҶ
	Quest_RandomCollect[29]={TargetType=QuestTargetType["Collect"],TargetID1=4010007,Num1=32,RewMoney=8470,RewXP=12000,MsgID=19029}--ѩ�����
	Quest_RandomCollect[30]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=40,RewMoney=38010,RewXP=19900,MsgID=19030}--�ᾧ
	Quest_RandomCollect[31]={TargetType=QuestTargetType["Collect"],TargetID1=4010026,Num1=8,RewMoney=26050,RewXP=28120,MsgID=19031}--����ԭҶ
	Quest_RandomCollect[32]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=100,RewMoney=74230,RewXP=31080,MsgID=19032}--ԭ��
	Quest_RandomCollect[33]={TargetType=QuestTargetType["Collect"],TargetID1=4010008,Num1=34,RewMoney=9330,RewXP=16300,MsgID=19033}--ѩ�屦��
	Quest_RandomCollect[34]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=40,RewMoney=38880,RewXP=21620,MsgID=19034}--�ᾧ
	Quest_RandomCollect[35]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=100,RewMoney=75940,RewXP=33780,MsgID=19035}--ԭ��
	Quest_RandomCollect[36]={TargetType=QuestTargetType["Collect"],TargetID1=4010027,Num1=9,RewMoney=30400,RewXP=40460,MsgID=19036}--ѩ��֮��
	Quest_RandomCollect[37]={TargetType=QuestTargetType["Collect"],TargetID1=4010008,Num1=36,RewMoney=10350,RewXP=17480,MsgID=19037}--ѩ�屦��
	Quest_RandomCollect[38]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=40,RewMoney=39900,RewXP=24280,MsgID=19038}--�ᾧ
	Quest_RandomCollect[39]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=100,RewMoney=77930,RewXP=37920,MsgID=19039}--ԭ��
	Quest_RandomCollect[40]={TargetType=QuestTargetType["Collect"],TargetID1=4010027,Num1=9,RewMoney=31850,RewXP=40960,MsgID=19040}--ѩ��֮��
	Quest_RandomCollect[41]={TargetType=QuestTargetType["Collect"],TargetID1=4010009,Num1=38,RewMoney=11310,RewXP=22900,MsgID=19041}--������ʯ
	Quest_RandomCollect[42]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=40,RewMoney=40750,RewXP=26140,MsgID=19042}--�ᾧ
	Quest_RandomCollect[43]={TargetType=QuestTargetType["Collect"],TargetID1=3050024,Num1=100,RewMoney=81490,RewXP=52280,MsgID=19043}--����Ԫ��
	Quest_RandomCollect[44]={TargetType=QuestTargetType["Collect"],TargetID1=4010028,Num1=10,RewMoney=36630,RewXP=56480,MsgID=19044}--������
	Quest_RandomCollect[45]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=150,RewMoney=119360,RewXP=61260,MsgID=19045}--ԭ��
	Quest_RandomCollect[46]={TargetType=QuestTargetType["Collect"],TargetID1=4010009,Num1=40,RewMoney=12450,RewXP=24400,MsgID=19046}--������ʯ
	Quest_RandomCollect[47]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=40,RewMoney=41720,RewXP=29100,MsgID=19047}--�ᾧ
	Quest_RandomCollect[48]={TargetType=QuestTargetType["Collect"],TargetID1=3050024,Num1=100,RewMoney=83440,RewXP=58200,MsgID=19048}--����Ԫ��
	Quest_RandomCollect[49]={TargetType=QuestTargetType["Collect"],TargetID1=4010028,Num1=11,RewMoney=42120,RewXP=62880,MsgID=19049}--������
	Quest_RandomCollect[50]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=150,RewMoney=122230,RewXP=68200,MsgID=19050}--ԭ��
	Quest_RandomCollect[51]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=40,RewMoney=42540,RewXP=31120,MsgID=19051}--�ᾧ
	Quest_RandomCollect[52]={TargetType=QuestTargetType["Collect"],TargetID1=4010010,Num1=42,RewMoney=13510,RewXP=31280,MsgID=19052}--���ֱ���
	Quest_RandomCollect[53]={TargetType=QuestTargetType["Collect"],TargetID1=3050024,Num1=100,RewMoney=85070,RewXP=62220,MsgID=19053}--����Ԫ��
	Quest_RandomCollect[54]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=150,RewMoney=124620,RewXP=72920,MsgID=19054}--ԭ��
	Quest_RandomCollect[55]={TargetType=QuestTargetType["Collect"],TargetID1=4010029,Num1=11,RewMoney=43530,RewXP=76800,MsgID=19055}--���ֻ���
	Quest_RandomCollect[56]={TargetType=QuestTargetType["Collect"],TargetID1=4010010,Num1=44,RewMoney=14760,RewXP=33160,MsgID=19056}--���ֱ���
	Quest_RandomCollect[57]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=40,RewMoney=43480,RewXP=34180,MsgID=19057}--�ᾧ
	Quest_RandomCollect[58]={TargetType=QuestTargetType["Collect"],TargetID1=3050024,Num1=100,RewMoney=86950,RewXP=68360,MsgID=19058}--����Ԫ��
	Quest_RandomCollect[59]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=150,RewMoney=127370,RewXP=80120,MsgID=19059}--ԭ��
	Quest_RandomCollect[60]={TargetType=QuestTargetType["Collect"],TargetID1=4010029,Num1=12,RewMoney=49530,RewXP=84760,MsgID=19060}--���ֻ���
	Quest_RandomCollect[61]={TargetType=QuestTargetType["Collect"],TargetID1=3010046,Num1=1,RewMoney=12630,RewXP=41180,MsgID=19061}--����ҩˮ
	Quest_RandomCollect[62]={TargetType=QuestTargetType["Collect"],TargetID1=4010011,Num1=46,RewMoney=15830,RewXP=41500,MsgID=19062}--���ֽ���
	Quest_RandomCollect[63]={TargetType=QuestTargetType["Collect"],TargetID1=3010044,Num1=1,RewMoney=14030,RewXP=46320,MsgID=19063}--����֮ҩ
	Quest_RandomCollect[64]={TargetType=QuestTargetType["Collect"],TargetID1=3010045,Num1=1,RewMoney=14030,RewXP=46320,MsgID=19064}--ħ��֮ҩ
	Quest_RandomCollect[65]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=60,RewMoney=66000,RewXP=54480,MsgID=19065}--�ᾧ
	Quest_RandomCollect[66]={TargetType=QuestTargetType["Collect"],TargetID1=3050025,Num1=100,RewMoney=88000,RewXP=72640,MsgID=19066}--�е�Ԫ��
	Quest_RandomCollect[67]={TargetType=QuestTargetType["Collect"],TargetID1=4010030,Num1=13,RewMoney=55050,RewXP=109940,MsgID=19067}--���Ҽ�ʯ
	Quest_RandomCollect[68]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=200,RewMoney=171880,RewXP=113500,MsgID=19068}--ԭ��
	Quest_RandomCollect[69]={TargetType=QuestTargetType["Collect"],TargetID1=3010046,Num1=1,RewMoney=12530,RewXP=43740,MsgID=19069}--����ҩˮ
	Quest_RandomCollect[70]={TargetType=QuestTargetType["Collect"],TargetID1=4010011,Num1=48,RewMoney=17200,RewXP=43800,MsgID=19070}--���ֽ���
	Quest_RandomCollect[71]={TargetType=QuestTargetType["Collect"],TargetID1=3010044,Num1=1,RewMoney=13920,RewXP=49200,MsgID=19071}--����֮ҩ
	Quest_RandomCollect[72]={TargetType=QuestTargetType["Collect"],TargetID1=3010045,Num1=1,RewMoney=13920,RewXP=49200,MsgID=19072}--ħ��֮ҩ
	Quest_RandomCollect[73]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=60,RewMoney=67360,RewXP=59520,MsgID=19073}--�ᾧ
	Quest_RandomCollect[74]={TargetType=QuestTargetType["Collect"],TargetID1=3050025,Num1=100,RewMoney=89810,RewXP=79360,MsgID=19074}--�е�Ԫ��
	Quest_RandomCollect[75]={TargetType=QuestTargetType["Collect"],TargetID1=4010030,Num1=13,RewMoney=57330,RewXP=111200,MsgID=19075}--���Ҽ�ʯ
	Quest_RandomCollect[76]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=200,RewMoney=175400,RewXP=124000,MsgID=19076}--ԭ��
	Quest_RandomCollect[77]={TargetType=QuestTargetType["Collect"],TargetID1=3010046,Num1=1,RewMoney=12430,RewXP=45360,MsgID=19077}--����ҩˮ
	Quest_RandomCollect[78]={TargetType=QuestTargetType["Collect"],TargetID1=3010044,Num1=1,RewMoney=13810,RewXP=51020,MsgID=19078}--����֮ҩ
	Quest_RandomCollect[79]={TargetType=QuestTargetType["Collect"],TargetID1=3010045,Num1=1,RewMoney=13810,RewXP=51020,MsgID=19079}--ħ��֮ҩ
	Quest_RandomCollect[80]={TargetType=QuestTargetType["Collect"],TargetID1=4010012,Num1=50,RewMoney=18370,RewXP=54020,MsgID=19080}--���ӱ���
	Quest_RandomCollect[81]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=60,RewMoney=68110,RewXP=62920,MsgID=19081}--�ᾧ
	Quest_RandomCollect[82]={TargetType=QuestTargetType["Collect"],TargetID1=3050025,Num1=100,RewMoney=90810,RewXP=83880,MsgID=19082}--�е�Ԫ��
	Quest_RandomCollect[83]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=200,RewMoney=177360,RewXP=131080,MsgID=19083}--ԭ��
	Quest_RandomCollect[84]={TargetType=QuestTargetType["Collect"],TargetID1=4010031,Num1=14,RewMoney=63280,RewXP=141780,MsgID=19084}--�Ų�ԭ��
	Quest_RandomCollect[85]={TargetType=QuestTargetType["Collect"],TargetID1=3010046,Num1=1,RewMoney=12290,RewXP=47600,MsgID=19085}--����ҩˮ
	Quest_RandomCollect[86]={TargetType=QuestTargetType["Collect"],TargetID1=3010044,Num1=1,RewMoney=13650,RewXP=53560,MsgID=19086}--����֮ҩ
	Quest_RandomCollect[87]={TargetType=QuestTargetType["Collect"],TargetID1=3010045,Num1=1,RewMoney=13650,RewXP=53560,MsgID=19087}--ħ��֮ҩ
	Quest_RandomCollect[88]={TargetType=QuestTargetType["Collect"],TargetID1=4010012,Num1=52,RewMoney=19860,RewXP=56820,MsgID=19088}--���ӱ���
	Quest_RandomCollect[89]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=60,RewMoney=69400,RewXP=68080,MsgID=19089}--�ᾧ
	Quest_RandomCollect[90]={TargetType=QuestTargetType["Collect"],TargetID1=3050025,Num1=100,RewMoney=92540,RewXP=90760,MsgID=19090}--�е�Ԫ��
	Quest_RandomCollect[91]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=200,RewMoney=180730,RewXP=141820,MsgID=19091}--ԭ��
	Quest_RandomCollect[92]={TargetType=QuestTargetType["Collect"],TargetID1=4010031,Num1=14,RewMoney=65800,RewXP=143380,MsgID=19092}--�Ų�ԭ��
	Quest_RandomCollect[93]={TargetType=QuestTargetType["Collect"],TargetID1=4010013,Num1=54,RewMoney=21120,RewXP=68840,MsgID=19093}--������ʯ
	Quest_RandomCollect[94]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=60,RewMoney=70120,RewXP=71640,MsgID=19094}--�ᾧ
	Quest_RandomCollect[95]={TargetType=QuestTargetType["Collect"],TargetID1=3050026,Num1=100,RewMoney=93490,RewXP=95520,MsgID=19095}--�ߵ�Ԫ��
	Quest_RandomCollect[96]={TargetType=QuestTargetType["Collect"],TargetID1=3010046,Num1=2,RewMoney=24310,RewXP=98120,MsgID=19096}--����ҩˮ
	Quest_RandomCollect[97]={TargetType=QuestTargetType["Collect"],TargetID1=3010044,Num1=2,RewMoney=27010,RewXP=110380,MsgID=19097}--����֮ҩ
	Quest_RandomCollect[98]={TargetType=QuestTargetType["Collect"],TargetID1=3010045,Num1=2,RewMoney=27010,RewXP=110380,MsgID=19098}--ħ��֮ҩ
	Quest_RandomCollect[99]={TargetType=QuestTargetType["Collect"],TargetID1=4010032,Num1=15,RewMoney=72200,RewXP=179280,MsgID=19099}--����ɽʯ
	Quest_RandomCollect[100]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=250,RewMoney=228250,RewXP=186560,MsgID=19100}--ԭ��
	Quest_RandomCollect[101]={TargetType=QuestTargetType["Collect"],TargetID1=4010013,Num1=56,RewMoney=22630,RewXP=72180,MsgID=19101}--������ʯ
	Quest_RandomCollect[102]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=60,RewMoney=70980,RewXP=77180,MsgID=19102}--�ᾧ
	Quest_RandomCollect[103]={TargetType=QuestTargetType["Collect"],TargetID1=3010043,Num1=2,RewMoney=23820,RewXP=102300,MsgID=19103}--����֮ҩ
	Quest_RandomCollect[104]={TargetType=QuestTargetType["Collect"],TargetID1=3010046,Num1=2,RewMoney=23820,RewXP=102300,MsgID=19104}--����ҩˮ
	Quest_RandomCollect[105]={TargetType=QuestTargetType["Collect"],TargetID1=3050026,Num1=100,RewMoney=94640,RewXP=102920,MsgID=19105}--�ߵ�Ԫ��
	Quest_RandomCollect[106]={TargetType=QuestTargetType["Collect"],TargetID1=3010044,Num1=2,RewMoney=26460,RewXP=115080,MsgID=19106}--����֮ҩ
	Quest_RandomCollect[107]={TargetType=QuestTargetType["Collect"],TargetID1=3010045,Num1=2,RewMoney=26460,RewXP=115080,MsgID=19107}--ħ��֮ҩ
	Quest_RandomCollect[108]={TargetType=QuestTargetType["Collect"],TargetID1=4010032,Num1=16,RewMoney=79550,RewXP=193340,MsgID=19108}--����ɽʯ
	Quest_RandomCollect[109]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=250,RewMoney=231060,RewXP=200980,MsgID=19109}--ԭ��
	Quest_RandomCollect[110]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=60,RewMoney=71670,RewXP=80920,MsgID=19110}--�ᾧ
	Quest_RandomCollect[111]={TargetType=QuestTargetType["Collect"],TargetID1=4010014,Num1=58,RewMoney=23980,RewXP=86540,MsgID=19111}--����ɽ����
	Quest_RandomCollect[112]={TargetType=QuestTargetType["Collect"],TargetID1=3010043,Num1=2,RewMoney=23510,RewXP=104820,MsgID=19112}--����֮ҩ
	Quest_RandomCollect[113]={TargetType=QuestTargetType["Collect"],TargetID1=3010046,Num1=2,RewMoney=23510,RewXP=104820,MsgID=19113}--����ҩˮ
	Quest_RandomCollect[114]={TargetType=QuestTargetType["Collect"],TargetID1=3050026,Num1=100,RewMoney=95560,RewXP=107880,MsgID=19114}--�ߵ�Ԫ��
	Quest_RandomCollect[115]={TargetType=QuestTargetType["Collect"],TargetID1=3010044,Num1=2,RewMoney=26120,RewXP=117920,MsgID=19115}--����֮ҩ
	Quest_RandomCollect[116]={TargetType=QuestTargetType["Collect"],TargetID1=3010045,Num1=2,RewMoney=26120,RewXP=117920,MsgID=19116}--ħ��֮ҩ
	Quest_RandomCollect[117]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=250,RewMoney=233280,RewXP=210700,MsgID=19117}--ԭ��
	Quest_RandomCollect[118]={TargetType=QuestTargetType["Collect"],TargetID1=4010033,Num1=17,RewMoney=86490,RewXP=237760,MsgID=19118}--�����λ�
	Quest_RandomCollect[119]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=60,RewMoney=72850,RewXP=86540,MsgID=19119}--�ᾧ
	Quest_RandomCollect[120]={TargetType=QuestTargetType["Collect"],TargetID1=4010014,Num1=60,RewMoney=25730,RewXP=90480,MsgID=19120}--����ɽ����
	Quest_RandomCollect[121]={TargetType=QuestTargetType["Collect"],TargetID1=3010043,Num1=2,RewMoney=23050,RewXP=108160,MsgID=19121}--����֮ҩ
	Quest_RandomCollect[122]={TargetType=QuestTargetType["Collect"],TargetID1=3010046,Num1=2,RewMoney=23050,RewXP=108160,MsgID=19122}--����ҩˮ
	Quest_RandomCollect[123]={TargetType=QuestTargetType["Collect"],TargetID1=3050026,Num1=100,RewMoney=97140,RewXP=115380,MsgID=19123}--�ߵ�Ԫ��
	Quest_RandomCollect[124]={TargetType=QuestTargetType["Collect"],TargetID1=3010044,Num1=2,RewMoney=25610,RewXP=121680,MsgID=19124}--����֮ҩ
	Quest_RandomCollect[125]={TargetType=QuestTargetType["Collect"],TargetID1=3010045,Num1=2,RewMoney=25610,RewXP=121680,MsgID=19125}--ħ��֮ҩ
	Quest_RandomCollect[126]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=250,RewMoney=237140,RewXP=225360,MsgID=19126}--ԭ��
	Quest_RandomCollect[127]={TargetType=QuestTargetType["Collect"],TargetID1=4010033,Num1=17,RewMoney=89700,RewXP=240340,MsgID=19127}--�����λ�
	Quest_RandomCollect[128]={TargetType=QuestTargetType["Collect"],TargetID1=4010015,Num1=62,RewMoney=27180,RewXP=107060,MsgID=19128}--ƽ������
	Quest_RandomCollect[129]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=80,RewMoney=98000,RewXP=120540,MsgID=19129}--�ᾧ
	Quest_RandomCollect[130]={TargetType=QuestTargetType["Collect"],TargetID1=3050027,Num1=100,RewMoney=98000,RewXP=120540,MsgID=19130}--�ص�Ԫ��
	Quest_RandomCollect[131]={TargetType=QuestTargetType["Collect"],TargetID1=3010043,Num1=3,RewMoney=34020,RewXP=165320,MsgID=19131}--����֮ҩ
	Quest_RandomCollect[132]={TargetType=QuestTargetType["Collect"],TargetID1=3010046,Num1=3,RewMoney=34020,RewXP=165320,MsgID=19132}--����ҩˮ
	Quest_RandomCollect[133]={TargetType=QuestTargetType["Collect"],TargetID1=3010044,Num1=3,RewMoney=37800,RewXP=185980,MsgID=19133}--����֮ҩ
	Quest_RandomCollect[134]={TargetType=QuestTargetType["Collect"],TargetID1=3010045,Num1=3,RewMoney=37800,RewXP=185980,MsgID=19134}--ħ��֮ҩ
	Quest_RandomCollect[135]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=300,RewMoney=287100,RewXP=282520,MsgID=19135}--ԭ��
	Quest_RandomCollect[136]={TargetType=QuestTargetType["Collect"],TargetID1=4010034,Num1=18,RewMoney=97120,RewXP=291360,MsgID=19136}--ڤɽ��ʯ
	Quest_RandomCollect[137]={TargetType=QuestTargetType["Collect"],TargetID1=4010015,Num1=64,RewMoney=28930,RewXP=111680,MsgID=19137}--ƽ������
	Quest_RandomCollect[138]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=80,RewMoney=99020,RewXP=128520,MsgID=19138}--�ᾧ
	Quest_RandomCollect[139]={TargetType=QuestTargetType["Collect"],TargetID1=3050027,Num1=100,RewMoney=99020,RewXP=128520,MsgID=19139}--�ص�Ԫ��
	Quest_RandomCollect[140]={TargetType=QuestTargetType["Collect"],TargetID1=3010043,Num1=3,RewMoney=33050,RewXP=169440,MsgID=19140}--����֮ҩ
	Quest_RandomCollect[141]={TargetType=QuestTargetType["Collect"],TargetID1=3010046,Num1=3,RewMoney=33050,RewXP=169440,MsgID=19141}--����ҩˮ
	Quest_RandomCollect[142]={TargetType=QuestTargetType["Collect"],TargetID1=3010044,Num1=3,RewMoney=36720,RewXP=190620,MsgID=19142}--����֮ҩ
	Quest_RandomCollect[143]={TargetType=QuestTargetType["Collect"],TargetID1=3010045,Num1=3,RewMoney=36720,RewXP=190620,MsgID=19143}--ħ��֮ҩ
	Quest_RandomCollect[144]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=300,RewMoney=290090,RewXP=301180,MsgID=19144}--ԭ��
	Quest_RandomCollect[145]={TargetType=QuestTargetType["Collect"],TargetID1=4010034,Num1=19,RewMoney=105680,RewXP=310820,MsgID=19145}--ڤɽ��ʯ
	Quest_RandomCollect[146]={TargetType=QuestTargetType["Collect"],TargetID1=4010016,Num1=66,RewMoney=30330,RewXP=131020,MsgID=19146}--ڤɽ����
	Quest_RandomCollect[147]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=80,RewMoney=99360,RewXP=133840,MsgID=19147}--�ᾧ
	Quest_RandomCollect[148]={TargetType=QuestTargetType["Collect"],TargetID1=3050027,Num1=100,RewMoney=99360,RewXP=133840,MsgID=19148}--�ص�Ԫ��
	Quest_RandomCollect[149]={TargetType=QuestTargetType["Collect"],TargetID1=3010043,Num1=3,RewMoney=32260,RewXP=171680,MsgID=19149}--����֮ҩ
	Quest_RandomCollect[150]={TargetType=QuestTargetType["Collect"],TargetID1=3010046,Num1=3,RewMoney=32260,RewXP=171680,MsgID=19150}--����ҩˮ
	Quest_RandomCollect[151]={TargetType=QuestTargetType["Collect"],TargetID1=3010044,Num1=3,RewMoney=35850,RewXP=193140,MsgID=19151}--����֮ҩ
	Quest_RandomCollect[152]={TargetType=QuestTargetType["Collect"],TargetID1=3010045,Num1=3,RewMoney=35850,RewXP=193140,MsgID=19152}--ħ��֮ҩ
	Quest_RandomCollect[153]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=300,RewMoney=291080,RewXP=313700,MsgID=19153}--ԭ��
	Quest_RandomCollect[154]={TargetType=QuestTargetType["Collect"],TargetID1=4010035,Num1=19,RewMoney=107470,RewXP=353600,MsgID=19154}--����֮��
	Quest_RandomCollect[155]={TargetType=QuestTargetType["Collect"],TargetID1=4010016,Num1=68,RewMoney=32190,RewXP=136420,MsgID=19155}--ڤɽ����
	Quest_RandomCollect[156]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=80,RewMoney=100320,RewXP=141880,MsgID=19156}--�ᾧ
	Quest_RandomCollect[157]={TargetType=QuestTargetType["Collect"],TargetID1=3050027,Num1=100,RewMoney=100320,RewXP=141880,MsgID=19157}--�ص�Ԫ��
	Quest_RandomCollect[158]={TargetType=QuestTargetType["Collect"],TargetID1=3010043,Num1=3,RewMoney=31190,RewXP=174240,MsgID=19158}--����֮ҩ
	Quest_RandomCollect[159]={TargetType=QuestTargetType["Collect"],TargetID1=3010046,Num1=3,RewMoney=31190,RewXP=174240,MsgID=19159}--����ҩˮ
	Quest_RandomCollect[160]={TargetType=QuestTargetType["Collect"],TargetID1=3010044,Num1=3,RewMoney=34650,RewXP=196020,MsgID=19160}--����֮ҩ
	Quest_RandomCollect[161]={TargetType=QuestTargetType["Collect"],TargetID1=3010045,Num1=3,RewMoney=34650,RewXP=196020,MsgID=19161}--ħ��֮ҩ
	Quest_RandomCollect[162]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=300,RewMoney=293900,RewXP=332540,MsgID=19162}--ԭ��
	Quest_RandomCollect[163]={TargetType=QuestTargetType["Collect"],TargetID1=4010035,Num1=20,RewMoney=116530,RewXP=376120,MsgID=19163}--����֮��
	Quest_RandomCollect[164]={TargetType=QuestTargetType["Collect"],TargetID1=3050028,Num1=100,RewMoney=101100,RewXP=147380,MsgID=19164}--ǧ��Ԫ��
	Quest_RandomCollect[165]={TargetType=QuestTargetType["Collect"],TargetID1=4010017,Num1=70,RewMoney=33850,RewXP=158400,MsgID=19165}--���ɰ׽�
	Quest_RandomCollect[166]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=100,RewMoney=126370,RewXP=184220,MsgID=19166}--�ᾧ
	Quest_RandomCollect[167]={TargetType=QuestTargetType["Collect"],TargetID1=3010043,Num1=5,RewMoney=50810,RewXP=292560,MsgID=19167}--����֮ҩ
	Quest_RandomCollect[168]={TargetType=QuestTargetType["Collect"],TargetID1=3010046,Num1=5,RewMoney=50810,RewXP=292560,MsgID=19168}--����ҩˮ
	Quest_RandomCollect[169]={TargetType=QuestTargetType["Collect"],TargetID1=3010044,Num1=5,RewMoney=56450,RewXP=329140,MsgID=19169}--����֮ҩ
	Quest_RandomCollect[170]={TargetType=QuestTargetType["Collect"],TargetID1=3010045,Num1=5,RewMoney=56450,RewXP=329140,MsgID=19170}--ħ��֮ҩ
	Quest_RandomCollect[171]={TargetType=QuestTargetType["Collect"],TargetID1=4010036,Num1=21,RewMoney=124960,RewXP=445500,MsgID=19171}--����֮צ
	Quest_RandomCollect[172]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=400,RewMoney=394910,RewXP=460540,MsgID=19172}--ԭ��
	Quest_RandomCollect[173]={TargetType=QuestTargetType["Collect"],TargetID1=3050028,Num1=100,RewMoney=102000,RewXP=155820,MsgID=19173}--ǧ��Ԫ��
	Quest_RandomCollect[174]={TargetType=QuestTargetType["Collect"],TargetID1=4010017,Num1=72,RewMoney=35830,RewXP=164620,MsgID=19174}--���ɰ׽�
	Quest_RandomCollect[175]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=100,RewMoney=127500,RewXP=194780,MsgID=19175}--�ᾧ
	Quest_RandomCollect[176]={TargetType=QuestTargetType["Collect"],TargetID1=3010043,Num1=5,RewMoney=48830,RewXP=294680,MsgID=19176}--����֮ҩ
	Quest_RandomCollect[177]={TargetType=QuestTargetType["Collect"],TargetID1=3010046,Num1=5,RewMoney=48830,RewXP=294680,MsgID=19177}--����ҩˮ
	Quest_RandomCollect[178]={TargetType=QuestTargetType["Collect"],TargetID1=3010044,Num1=5,RewMoney=54250,RewXP=331500,MsgID=19178}--����֮ҩ
	Quest_RandomCollect[179]={TargetType=QuestTargetType["Collect"],TargetID1=3010045,Num1=5,RewMoney=54250,RewXP=331500,MsgID=19179}--ħ��֮ҩ
	Quest_RandomCollect[180]={TargetType=QuestTargetType["Collect"],TargetID1=4010036,Num1=22,RewMoney=134750,RewXP=471520,MsgID=19180}--����֮צ
	Quest_RandomCollect[181]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=400,RewMoney=398420,RewXP=486920,MsgID=19181}--ԭ��
	Quest_RandomCollect[182]={TargetType=QuestTargetType["Collect"],TargetID1=3050028,Num1=100,RewMoney=102730,RewXP=161460,MsgID=19182}--ǧ��Ԫ��
	Quest_RandomCollect[183]={TargetType=QuestTargetType["Collect"],TargetID1=4010018,Num1=74,RewMoney=37590,RewXP=189900,MsgID=19183}--��������
	Quest_RandomCollect[184]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=100,RewMoney=128410,RewXP=201820,MsgID=19184}--�ᾧ
	Quest_RandomCollect[185]={TargetType=QuestTargetType["Collect"],TargetID1=3010043,Num1=5,RewMoney=47520,RewXP=295040,MsgID=19185}--����֮ҩ
	Quest_RandomCollect[186]={TargetType=QuestTargetType["Collect"],TargetID1=3010046,Num1=5,RewMoney=47520,RewXP=295040,MsgID=19186}--����ҩˮ
	Quest_RandomCollect[187]={TargetType=QuestTargetType["Collect"],TargetID1=3010044,Num1=5,RewMoney=52800,RewXP=331920,MsgID=19187}--����֮ҩ
	Quest_RandomCollect[188]={TargetType=QuestTargetType["Collect"],TargetID1=3010045,Num1=5,RewMoney=52800,RewXP=331920,MsgID=19188}--ħ��֮ҩ
	Quest_RandomCollect[189]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=400,RewMoney=401280,RewXP=504520,MsgID=19189}--ԭ��
	Quest_RandomCollect[190]={TargetType=QuestTargetType["Collect"],TargetID1=4010037,Num1=22,RewMoney=137540,RewXP=529240,MsgID=19190}--��ħʯ
	Quest_RandomCollect[191]={TargetType=QuestTargetType["Collect"],TargetID1=3050028,Num1=100,RewMoney=103110,RewXP=169920,MsgID=19191}--ǧ��Ԫ��
	Quest_RandomCollect[192]={TargetType=QuestTargetType["Collect"],TargetID1=4010018,Num1=76,RewMoney=39530,RewXP=197000,MsgID=19192}--��������
	Quest_RandomCollect[193]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=100,RewMoney=128880,RewXP=212400,MsgID=19193}--�ᾧ
	Quest_RandomCollect[194]={TargetType=QuestTargetType["Collect"],TargetID1=3010043,Num1=5,RewMoney=45160,RewXP=294000,MsgID=19194}--����֮ҩ
	Quest_RandomCollect[195]={TargetType=QuestTargetType["Collect"],TargetID1=3010046,Num1=5,RewMoney=45160,RewXP=294000,MsgID=19195}--����ҩˮ
	Quest_RandomCollect[196]={TargetType=QuestTargetType["Collect"],TargetID1=3010044,Num1=5,RewMoney=50180,RewXP=330760,MsgID=19196}--����֮ҩ
	Quest_RandomCollect[197]={TargetType=QuestTargetType["Collect"],TargetID1=3010045,Num1=5,RewMoney=50180,RewXP=330760,MsgID=19197}--ħ��֮ҩ
	Quest_RandomCollect[198]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=400,RewMoney=402740,RewXP=530980,MsgID=19198}--ԭ��
	Quest_RandomCollect[199]={TargetType=QuestTargetType["Collect"],TargetID1=4010037,Num1=23,RewMoney=147230,RewXP=558900,MsgID=19199}--��ħʯ
	Quest_RandomCollect[200]={TargetType=QuestTargetType["Collect"],TargetID1=3050029,Num1=100,RewMoney=103680,RewXP=172800,MsgID=19200}--����Ԫ��
	Quest_RandomCollect[201]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=100,RewMoney=129600,RewXP=216000,MsgID=19201}--�ᾧ
	Quest_RandomCollect[202]={TargetType=QuestTargetType["Collect"],TargetID1=4010019,Num1=78,RewMoney=41070,RewXP=224640,MsgID=19202}--��ħ����
	Quest_RandomCollect[203]={TargetType=QuestTargetType["Collect"],TargetID1=3010043,Num1=5,RewMoney=44550,RewXP=293340,MsgID=19203}--����֮ҩ
	Quest_RandomCollect[204]={TargetType=QuestTargetType["Collect"],TargetID1=3010046,Num1=5,RewMoney=44550,RewXP=293340,MsgID=19204}--����ҩˮ
	Quest_RandomCollect[205]={TargetType=QuestTargetType["Collect"],TargetID1=3010044,Num1=5,RewMoney=49500,RewXP=330000,MsgID=19205}--����֮ҩ
	Quest_RandomCollect[206]={TargetType=QuestTargetType["Collect"],TargetID1=3010045,Num1=5,RewMoney=49500,RewXP=330000,MsgID=19206}--ħ��֮ҩ
	Quest_RandomCollect[207]={TargetType=QuestTargetType["Collect"],TargetID1=3050001,Num1=400,RewMoney=405000,RewXP=540000,MsgID=19207}--ԭ��
	Quest_RandomCollect[208]={TargetType=QuestTargetType["Collect"],TargetID1=4010038,Num1=24,RewMoney=155520,RewXP=648000,MsgID=19208}--��ħԭʯ

	--��ʼ������Ķ�̬����
	role.QuestInit(RoleID, QuestID, GetRandQuestTable(Quest_RandomCollect, Index))
	--ͬ���ͻ�����������������˵��
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 11, QuestID)
	msg.AddMsgEvent(MsgID, 1, Quest_RandomCollect[Index].MsgID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end

--�������
function q30307_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

	role.SetRoleScriptData(RoleID, 1, RoleDataType["XinyangCollect"], role.GetRoleScriptData(RoleID, 1, RoleDataType["XinyangCollect"])+1)
	role.ModRoleAttValue(MapID, InstanceID, RoleID, 107, 100)

end


aux.RegisterQuestEvent(30307, 1, "q30307_OnComplete")
aux.RegisterQuestEvent(30307, 7, "q30307_OnQuestInit")

function q30307_CheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	local CurTime = tonumber(os.date("%j"))
	local LastTime = math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["XinyangCollect"])/100)
	if CurTime ~= LastTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["XinyangCollect"], CurTime*100)
	end
	local Num = role.GetRoleScriptData(RoleID, 1, RoleDataType["XinyangCollect"])%100

	local rolelevel = role.GetRoleQuestNum(RoleID)

	if Num >= 20 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440009)			--��������ɵ���索���������Ѿ��ﵽ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	else
	    if role.GetRoleAttValue(MapID, InstanceID, RoleID, 106) <= 10 then
			return 1
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 440103)			--ֻ�������10�����µ���ҿ��Խ�ȡ������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		end
	end
end

aux.RegisterQuestEvent(30307, 4, "q30307_CheckAccept")
