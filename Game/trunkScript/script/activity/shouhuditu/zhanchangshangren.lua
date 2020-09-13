function NPC6040050_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --��ȡ��ҵȼ�
	local x = role.GetWarGold(MapID, InstanceID,RoleID)
	if x < 10 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 207123)		--ս��BUFF��:\n    �����ϲ���10ս����ң��޷��һ�
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 14)			
		msg.AddMsgEvent(MsgID, 1, 100001)		--ȷ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	else
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 207122)		--ս��BUFF��:\n    ʹ��10ս����ҿ��Ի�ȡ��ǿBUFF֧���뱣��
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 4)			
			msg.AddMsgEvent(MsgID, 1, 207112)		--��������10
			msg.AddMsgEvent(MsgID, 21, 5)			
			msg.AddMsgEvent(MsgID, 1, 207113)		--��������10
			msg.AddMsgEvent(MsgID, 21, 6)			
			msg.AddMsgEvent(MsgID, 1, 207114)		--��Ԫ����10
			msg.AddMsgEvent(MsgID, 21, 7)			
			msg.AddMsgEvent(MsgID, 1, 207115)		--��������10
			msg.AddMsgEvent(MsgID, 21, 8)			
			msg.AddMsgEvent(MsgID, 1, 207116)		--�����������150
			msg.AddMsgEvent(MsgID, 21, 9)			
			msg.AddMsgEvent(MsgID, 1, 207117)		--�����Ԫ����150
			msg.AddMsgEvent(MsgID, 21, 10)			
			msg.AddMsgEvent(MsgID, 1, 207118)		--��׼����50
			msg.AddMsgEvent(MsgID, 21, 11)			
			msg.AddMsgEvent(MsgID, 1, 207119)		--��������50
			msg.AddMsgEvent(MsgID, 21, 12)			
			msg.AddMsgEvent(MsgID, 1, 207120)		--�˺���������5
			msg.AddMsgEvent(MsgID, 21, 13)			
			msg.AddMsgEvent(MsgID, 1, 207121)		--�˺���������5
			msg.AddMsgEvent(MsgID, 21, 14)			
			msg.AddMsgEvent(MsgID, 1, 100002)		--ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 4 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3922801, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		elseif TalkIndex == 5 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3922901, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		elseif TalkIndex == 6 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3923001, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		elseif TalkIndex == 7 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3923101, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		elseif TalkIndex == 8 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3923201, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		elseif TalkIndex == 9 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3923301, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		elseif TalkIndex == 10 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3923401, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		elseif TalkIndex == 11 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3923501, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		elseif TalkIndex == 12 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3923601, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		elseif TalkIndex == 13 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3923701, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		end
	end
end
aux.RegisterCreatureEvent(6040050, 7, "NPC6040050_OnTalk")

function NPC6040057_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --��ȡ��ҵȼ�
	local x = role.GetRoleItemNum(RoleID, 4820269)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 207124)		--ս�������:\n    һ���һ�����Զһ�500ս�����
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			
		msg.AddMsgEvent(MsgID, 1, 100001)		--��������10
		msg.AddMsgEvent(MsgID, 21, 5)
		msg.AddMsgEvent(MsgID, 1, 100002)		--ȡ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if x < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 207125)		--ս�������:\n    ս����Ҷһ�������1��
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 5)
			msg.AddMsgEvent(MsgID, 1, 100001)		--ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4820269, 1, 420)
			role.SetWarGold(MapID, InstanceID,RoleID,500)
		end
	end
end
aux.RegisterCreatureEvent(6040057, 7, "NPC6040057_OnTalk")
Zshouweishibing = 0
ZLshouweishibing = 0 
function NPC6040051_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --��ȡ��ҵȼ�
	local table1 = {}
	table1[1] = {800,4200,140}
	table1[2] = {811,4200,140}
	table1[3] = {848,4200,145}
	table1[4] = {811,4200,103}
	local x = role.GetWarGold(MapID, InstanceID,RoleID)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 207126)		--ս�������:\n    200ս����ҿ��Զһ�4������ʿ����500��ҿ��Զһ�4������������ʿ��
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			
		msg.AddMsgEvent(MsgID, 1, 207128)		--��ļ����ʿ��4020176
		msg.AddMsgEvent(MsgID, 21, 5)			
		msg.AddMsgEvent(MsgID, 1, 207129)		--��ļ����������ʿ��4020177
		msg.AddMsgEvent(MsgID, 21, 6)
		msg.AddMsgEvent(MsgID, 1, 100002)		--ȡ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if x < 200 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 207127)		--ս�������:\n    ս����Ҷһ�������200��
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 6)
			msg.AddMsgEvent(MsgID, 1, 100001)		--ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if Zshouweishibing > 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 207131)		--ս�������:\n    ����ʿ���Ѿ����֣��������ٻ�
				msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21, 6)
				msg.AddMsgEvent(MsgID, 1, 100001)		--ȡ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if ZLshouweishibing > 0 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 207132)		--ս�������:\n    ����������ʿ��������ʿ������ͬʱ����
					msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21, 6)
					msg.AddMsgEvent(MsgID, 1, 100001)		--ȡ��
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					for i = 1,4 do
						Zshouweishibing = Zshouweishibing + 1
						map.MapCreateColCreature(MapID, InstanceID,4020176, table1[i][1], table1[i][2], table1[i][3], 1)
					end
					role.SetWarGold(MapID, InstanceID,RoleID,-200)
				end
			end
		end
	elseif TalkIndex == 5 then
		if x < 500 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 207130)		--ս�������:\n    ս����Ҷһ�������500��
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 6)
			msg.AddMsgEvent(MsgID, 1, 100001)		--ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if ZLshouweishibing > 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 207133)		--ս�������:\n    ����������ʿ���Ѿ����֣��������ٻ�
				msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21, 6)
				msg.AddMsgEvent(MsgID, 1, 100001)		--ȡ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if Zshouweishibing > 0 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 207132)		--ս�������:\n    ����������ʿ��������ʿ������ͬʱ����
					msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21, 6)
					msg.AddMsgEvent(MsgID, 1, 100001)		--ȡ��
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					for i = 1,4 do
						ZLshouweishibing = ZLshouweishibing + 1
						map.MapCreateColCreature(MapID, InstanceID,4020177, table1[i][1], table1[i][2], table1[i][3], 1)
					end
					role.SetWarGold(MapID, InstanceID,RoleID,-500)
				end
			end
		end
	end
end
aux.RegisterCreatureEvent(6040051, 7, "NPC6040051_OnTalk")
function zhanchangshouwei_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	Zshouweishibing = Zshouweishibing -1
end
aux.RegisterCreatureEvent(4020176, 4, "zhanchangshouwei_OnDie")
function Lzhanchangshouwei_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	ZLshouweishibing = ZLshouweishibing -1
end
aux.RegisterCreatureEvent(4020177, 4, "Lzhanchangshouwei_OnDie")