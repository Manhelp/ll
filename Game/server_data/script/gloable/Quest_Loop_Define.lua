--����������Զ���
--��̬����ֻ��д��Ϊ0���������ɣ����������ɴ˺����Զ���Ϊ0
QuestTargetType = {}
QuestTargetType["Kill"]    = 1		--��ɱĳ�ֹ���
QuestTargetType["Collect"] = 2		--�ռ���Ʒ
QuestTargetType["NPCTalk"] = 3		--NPC�Ի�


--�õ��������table��һ�������������ԣ�tablename ��������Num ����������ֵ��
function GetRandQuestTable(tablename, Index)

    local function Isnil(VAL)
	    if VAL == nil then
		    return 0
		else
		    return VAL
		end
	end

	--��ɱ�������͵�����
	local TargetTypeTB=
	{[1]=Isnil(tablename[Index].TargetID1),
	[2]=Isnil(tablename[Index].TargetID2),
	[3]=Isnil(tablename[Index].TargetID3),
	[4]=Isnil(tablename[Index].TargetID4),
	[5]=Isnil(tablename[Index].Num1),
	[6]=Isnil(tablename[Index].Num2),
	[7]=Isnil(tablename[Index].Num3),
	[8]=Isnil(tablename[Index].Num4),
	[9]=Isnil(tablename[Index].MsgID),
	[10]=Isnil(tablename[Index].RewMoney),
	[11]=Isnil(tablename[Index].RewXP),
	[12]=Isnil(tablename[Index].RewItemID1),
	[13]=Isnil(tablename[Index].value1),
	[14]=Isnil(tablename[Index].RewItemID2),
	[15]=Isnil(tablename[Index].value2),
	[16]=Isnil(tablename[Index].RewItemID3),
	[17]=Isnil(tablename[Index].value3),
	[18]=Isnil(tablename[Index].RewItemID4),
	[19]=Isnil(tablename[Index].value4),
	[20]=Isnil(tablename[Index].RewChoicesItemID1),
	[21]=Isnil(tablename[Index].Cvalue1),
	[22]=Isnil(tablename[Index].RewChoicesItemID2),
	[23]=Isnil(tablename[Index].Cvalue2),
	[24]=Isnil(tablename[Index].RewChoicesItemID3),
	[25]=Isnil(tablename[Index].Cvalue3),
	[26]=Isnil(tablename[Index].RewChoicesItemID4),
	[27]=Isnil(tablename[Index].Cvalue4),
	[28]=Isnil(tablename[Index].TargetType)}

	--for i = 1,28 do
	--	if TargetTypeTB[i] == nil then TargetTypeTB[i] = 0 end
	--end

	if TargetTypeTB[28] == QuestTargetType["Kill"] then
		--���ع���ID������
		return QuestTargetType["Kill"], TargetTypeTB[9], TargetTypeTB[1], TargetTypeTB[5], TargetTypeTB[2], TargetTypeTB[6], TargetTypeTB[3], TargetTypeTB[7], TargetTypeTB[4], TargetTypeTB[8],
		TargetTypeTB[10],TargetTypeTB[11],TargetTypeTB[12],TargetTypeTB[13],TargetTypeTB[14],TargetTypeTB[15],TargetTypeTB[16],TargetTypeTB[17],TargetTypeTB[18],TargetTypeTB[19],TargetTypeTB[20],
		TargetTypeTB[21],TargetTypeTB[22],TargetTypeTB[23],TargetTypeTB[24],TargetTypeTB[25],TargetTypeTB[26],TargetTypeTB[27]
	end

	--�ռ���Ʒ���͵�����
	if TargetTypeTB[28] == QuestTargetType["Collect"] then
		--������ƷTypeID��������
		return QuestTargetType["Collect"], TargetTypeTB[9], TargetTypeTB[1], TargetTypeTB[5], TargetTypeTB[2], TargetTypeTB[6], TargetTypeTB[3], TargetTypeTB[7], TargetTypeTB[4], TargetTypeTB[8],
		TargetTypeTB[10],TargetTypeTB[11],TargetTypeTB[12],TargetTypeTB[13],TargetTypeTB[14],TargetTypeTB[15],TargetTypeTB[16],TargetTypeTB[17],TargetTypeTB[18],TargetTypeTB[19],TargetTypeTB[20],
		TargetTypeTB[21],TargetTypeTB[22],TargetTypeTB[23],TargetTypeTB[24],TargetTypeTB[25],TargetTypeTB[26],TargetTypeTB[27]
	end

	--NPC�Ի����͵�����
	if TargetTypeTB[28] == QuestTargetType["NPCTalk"] then
		--����NPCTypeID
		return QuestTargetType["NPCTalk"], TargetTypeTB[9], TargetTypeTB[1],TargetTypeTB[2], TargetTypeTB[3], TargetTypeTB[4],
		TargetTypeTB[10],TargetTypeTB[11],TargetTypeTB[12],TargetTypeTB[13],TargetTypeTB[14],TargetTypeTB[15],TargetTypeTB[16],TargetTypeTB[17],TargetTypeTB[18],TargetTypeTB[19],TargetTypeTB[20],
		TargetTypeTB[21],TargetTypeTB[22],TargetTypeTB[23],TargetTypeTB[24],TargetTypeTB[25],TargetTypeTB[26],TargetTypeTB[27]
    end
end

