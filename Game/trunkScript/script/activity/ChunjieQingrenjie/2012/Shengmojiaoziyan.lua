--��6040018�Ի�
--RoleDataType["Jiaozishipin"]  ��ɫ����
--RoleDataType["jiaozishipin_time"] ʱ�����
--Jiaozi_start = 0 �����
--Jiaozi_start = 1 ���ʼ

Jiaozi_start = 0
xinchunshisheng = 0
shengmojiaoziyanshizhe = 0
xiongmaozhanglao = 0
xiongmaozuzhang = 0
shancaishizhe = 0
longzuchuanlingguan = 0

--���ڻ��ʼ
function SpringF_OnStart(ActID)
	xinchunshisheng = map.MapCreateColCreature(3424073512, -1, 6040018, 551, 23510, 794, 1)
	shengmojiaoziyanshizhe = map.MapCreateColCreature(3424073512, -1, 6040019, 563, 23510, 803, 1)
	xiongmaozhanglao = map.MapCreateColCreature(3424073512, -1, 6040023, 666, 23510, 796, 1)
	shancaishizhe = map.MapCreateColCreature(3424073512, -1, 6040024, 679, 23510, 789, 1)
	longzuchuanlingguan = map.MapCreateColCreature(3424073512, -1, 5610100, 558, 23510, 812, 1)
end
aux.RegisterActEvent(50, 2, "SpringF_OnStart")

function SpringF_OnEnd(ActID)
	map.MapDeleteCreature(3424073512, -1,xinchunshisheng)
	map.MapDeleteCreature(3424073512, -1,shengmojiaoziyanshizhe)
	map.MapDeleteCreature(3424073512, -1,xiongmaozhanglao)
	map.MapDeleteCreature(3424073512, -1,shancaishizhe)
	map.MapDeleteCreature(3424073512, -1,longzuchuanlingguan)
end
aux.RegisterActEvent(50, 3, "SpringF_OnEnd")

function Jiaozi_OnTimerMin(actID)
	local x = act.GetActScriptData(50, 1, 14)
	local curmin = tonumber(os.date("%M"))
	local curhour = tonumber(os.date("%H"))
--�ÿ����
	if curhour == 5 or curhour == 11 or curhour == 17 or curhour ==23 then
		Jiaozi_start = 0
		if curmin == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 300078)    --����ʥħ�������Ѿ��������´�ʥħ�����罫��1Сʱ�����С��������а�ǰ��������ҿ�����NPC����ȡ�������⽱���ˣ�
			msg.DispatchWorldMsgEvent(MsgID)
			if x > 1999  then
				act.AddAllRoleBuff(6106103)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 300076)    --ʥħ�������Ѿ������������ν��������ܹ��ռ���XXX�����ӣ�����������һ�á����㷹����Ч����
				msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 14))
				msg.DispatchWorldMsgEvent(MsgID)
			elseif x > 999 then
				act.AddAllRoleBuff(6106102)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 300074)    --ʥħ�������Ѿ������������ν��������ܹ��ռ���XXX�����ӣ�����������һ�á������ζ��Ч����
				msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 14))
				msg.DispatchWorldMsgEvent(MsgID)
			elseif x > 499 then
				act.AddAllRoleBuff(6106101)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 300075)    --ʥħ�������Ѿ������������ν��������ܹ��ռ���XXX�����ӣ�����������һ�á�θ�ڴ󿪡�Ч����
				msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 14))
				msg.DispatchWorldMsgEvent(MsgID)
			end
		elseif curmin == 55 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 300079)    --Ϊ��ӭ���´��ĵ��������ڵ��������������ս���Ϊ������������磬ʥħ�����绹��5���Ӿ�Ҫ��ʼ�ˣ���ҿ���׼��������������
			msg.DispatchWorldMsgEvent(MsgID)
		end
	else
		if Jiaozi_start == 0 then
			Jiaozi_start = 1
			for q=0,14 do                       --�ݴ����޸�
				act.SetActScriptData(50, 1, q, 0)
			end
			act.SaveActScriptData(50)
		end
	end
	if curmin == 15 or curmin == 45 then
		local XIONGMAO_cre = {}
		XIONGMAO_cre[1] = {mapID = 3424071720, creID =3040343, x =2175, y =9049, z =1143}
		XIONGMAO_cre[2] = {mapID = 3424071720, creID =3040343, x =1704, y =11546, z =1699}
		XIONGMAO_cre[3] = {mapID = 3424071720, creID =3040343, x =1238, y =8905, z =2154}
		XIONGMAO_cre[4] = {mapID = 3424071720, creID =3040343, x =975, y =7377, z =2579}
		XIONGMAO_cre[5] = {mapID = 3424071720, creID =3040343, x =927, y =9024, z =3226}
		XIONGMAO_cre[6] = {mapID = 3424071720, creID =3040343, x =1978, y =13918, z =3182}
		XIONGMAO_cre[7] = {mapID = 3424071720, creID =3040343, x =2497, y =14341, z =3286}
		XIONGMAO_cre[8] = {mapID = 3424071720, creID =3040343, x =2431, y =7985, z =2149}
		XIONGMAO_cre[9] = {mapID = 3424071720, creID =3040343, x =2847, y =7814, z =2288}
		XIONGMAO_cre[10] = {mapID = 3424071720, creID =3040343, x =3553, y =10394, z =1340}
		XIONGMAO_cre[11] = {mapID = 3424072488, creID =3040344, x =2590, y =4862, z =1745}
		XIONGMAO_cre[12] = {mapID = 3424072488, creID =3040344, x =2188, y =6000, z =1704}
		XIONGMAO_cre[13] = {mapID = 3424072488, creID =3040344, x =1452, y =5828, z =1256}
		XIONGMAO_cre[14] = {mapID = 3424072488, creID =3040344, x =1879, y =3100, z =479}
		XIONGMAO_cre[15] = {mapID = 3424072488, creID =3040344, x =902, y =6175, z =2154}
		XIONGMAO_cre[16] = {mapID = 3424072488, creID =3040344, x =1751, y =5748, z =1805}
		XIONGMAO_cre[17] = {mapID = 3424072488, creID =3040344, x =2475, y =4197, z =1875}
		XIONGMAO_cre[18] = {mapID = 3424072488, creID =3040344, x =2609, y =4800, z =1564}
		XIONGMAO_cre[19] = {mapID = 3424072488, creID =3040344, x =1998, y =5854, z =1115}
		XIONGMAO_cre[20] = {mapID = 3424072488, creID =3040344, x =2098, y =2920, z =1407}
		XIONGMAO_cre[21] = {mapID = 3424072232, creID =3040345, x =1081, y =12955, z =1441}
		XIONGMAO_cre[22] = {mapID = 3424072232, creID =3040345, x =1854, y =3456, z =1488}
		XIONGMAO_cre[23] = {mapID = 3424072232, creID =3040345, x =937, y =13130, z =1365}
		XIONGMAO_cre[24] = {mapID = 3424072232, creID =3040345, x =1468, y =3586, z =1162}
		XIONGMAO_cre[25] = {mapID = 3424072232, creID =3040345, x =2456, y =3634, z =1701}
		XIONGMAO_cre[26] = {mapID = 3424072232, creID =3040345, x =2267, y =3013, z =1986}
		XIONGMAO_cre[27] = {mapID = 3424072232, creID =3040345, x =1611, y =4631, z =2423}
		XIONGMAO_cre[28] = {mapID = 3424072232, creID =3040345, x =1547, y =4521, z =2354}
		XIONGMAO_cre[29] = {mapID = 3424072232, creID =3040345, x =1763, y =5351, z =2473}
		XIONGMAO_cre[30] = {mapID = 3424072232, creID =3040345, x =740, y =13287, z =1699}
		XIONGMAO_cre[31] = {mapID = 3424075048, creID =3040346, x =536, y =10494, z =2098}
		XIONGMAO_cre[32] = {mapID = 3424075048, creID =3040346, x =873, y =9996, z =1990}
		XIONGMAO_cre[33] = {mapID = 3424075048, creID =3040346, x =1238, y =11287, z =1435}
		XIONGMAO_cre[34] = {mapID = 3424075048, creID =3040346, x =1559, y =14102, z =769}
		XIONGMAO_cre[35] = {mapID = 3424075048, creID =3040346, x =1555, y =13378, z =434}
		XIONGMAO_cre[36] = {mapID = 3424075048, creID =3040346, x =1497, y =13226, z =357}
		XIONGMAO_cre[37] = {mapID = 3424075048, creID =3040346, x =2137, y =10489, z =357}
		XIONGMAO_cre[38] = {mapID = 3424075048, creID =3040346, x =2453, y =9117, z =493}
		XIONGMAO_cre[39] = {mapID = 3424075048, creID =3040346, x =2234, y =9325, z =1792}
		XIONGMAO_cre[40] = {mapID = 3424075048, creID =3040346, x =1770, y =10716, z =2130}
		for k in pairs(XIONGMAO_cre) do
			map.MapCreateColCreature(XIONGMAO_cre[k].mapID, -1, XIONGMAO_cre[k].creID, XIONGMAO_cre[k].x, XIONGMAO_cre[k].y, XIONGMAO_cre[k].z, 1)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 300082)      		--�´���è�Ѿ�����������أ���ȥ�������е�����ȥ�赲������è�����ְɣ��´���èֻ����10���ӣ������ץ����
		msg.DispatchWorldMsgEvent(MsgID)
	end
	if curhour == 20 and curmin ==0 then
		xiongmaozuzhang = map.MapCreateColCreature(3424073512, -1, 6040020, 677,23510,801, 1)
		f20_Creature()
	end
	if curhour == 20 and (curmin == 18 or curmin == 36 or curmin == 54) then
		f20_MonsterCreate()
	end
	if curhour == 21 and curmin == 0 then
		if xiongmaozuzhang ~= nil and xiongmaozuzhang ~= 0 then
			map.MapDeleteCreature(3424073512, -1, xiongmaozuzhang)
			xiongmaozuzhang = 0
		end
		f20_delete()
	end
end
aux.RegisterActEvent(50, 4, "Jiaozi_OnTimerMin")


function n6040018_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local CurTime = tonumber(os.date("%j"))
	local UpdateTime = math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["jiaozishipin_time"])/10)
	if CurTime ~= UpdateTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Jiaozishipin"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["jiaozishipin_time"], CurTime*10)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["jiaofucishu"], 0)
	end
	local curhour = tonumber(os.date("%H"))
	if curhour >= 0 and curhour <= 5 then
		curhour = 1
	elseif curhour >= 6 and curhour <= 11 then
		curhour = 2
	elseif curhour >= 12 and curhour <= 17 then
		curhour = 3
	elseif curhour >= 18 and curhour <= 23 then
		curhour = 4
	else
		curhour = 5
	end
	if curhour ~= 5 and curhour ~= role.GetRoleScriptData(RoleID, 1, RoleDataType["jiaozishipin_time"])%10 then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Jiaozishipin"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["jiaozishipin_time"], CurTime*10+curhour)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["jiaofucishu"], 0)
	end
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444410) -- "���ڵ������\n    ʥħ�������Ѿ���ʼ�����ھͿ��Խ��ɽ�������ø��ֽ�������\n    �ύ���ӿ��Ի���´��Ľ�����ÿ��ʱ���(0:00-4:00;6:00-10:00;12:00-16:00;18:00-22:00)�ύ��������3���˻���������Ķ��⽱����\n ���λȫ���ύ�Ľ������� ���λ���ύ�Ľ��ӣ� XX����"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 14))
		msg.AddMsgEvent(MsgID, 9, role.GetRoleScriptData(RoleID, 1, RoleDataType["Jiaozishipin"]))
		msg.AddMsgEvent(MsgID, 21, 5) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 206336) -- ���鿴�ύ���а�
		if Jiaozi_start == 1 then
			msg.AddMsgEvent(MsgID, 21, 4)   --
			msg.AddMsgEvent(MsgID, 1, 444412)   --"����5����������"
			msg.AddMsgEvent(MsgID, 21, 9)
			msg.AddMsgEvent(MsgID, 1, 444413)   --"����5��ˮ������"
		else
			msg.AddMsgEvent(MsgID, 21, 6)   --ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 206338)   --"��ȡ���н���"
		end
		msg.AddMsgEvent(MsgID, 21, 10)   --ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100002)   --"ȡ��"
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
    elseif TalkIndex == 4 then
			if role.GetBagFreeSize(RoleID) < 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444443) -- "���ڵ������\n	�װ�����ң���ı������Ѿ�װ�����������Ҫ����һ�²��ܻ�ý�����"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444415) -- "���ڵ������\n	����һ�괺�ڵ������������Ϊ����Ӣ������ʥħ�����磬��Ҫ���Լ���һ����ô���Ϲ�أ��Ͻ�5���������ӡ����ǲ��������ģ��ٺ١�\n"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, 11) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end
	elseif TalkIndex == 11 then
		local a = role.GetRoleItemNum(RoleID, 4830011)
		if a > 4 then
			if role.GetRoleScriptData(RoleID, 1, RoleDataType["jiaofucishu"]) < 30 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444416) -- "���ڵ������\n	���Ե�������������ţ��óԲ�����ӡ���ʥħ���������ڻ�û��ʼ�أ�������ٹ����ɡ����Ǹ�����´������������������\n    ��Ҫ��ý����ʹ�����ĸ���ˮ�����ɣ�"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
				if level >=30 and level <=39 then
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4830011, 5, 420)
					role.AddRoleItem(MapID, InstanceID, RoleID, 4830008, 2, -1, 8, 420)
					role.AddRoleExp(MapID, InstanceID, RoleID, 30000)
					HuoDeJiaozishipin(RoleID)
				elseif level >= 40 and level <=49 then
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4830011, 5, 420)
					role.AddRoleItem(MapID, InstanceID, RoleID, 4830008, 2, -1, 8, 420)
					role.AddRoleExp(MapID, InstanceID, RoleID, 100000)
					HuoDeJiaozishipin(RoleID)
				elseif level >= 50 and level <=59 then
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4830011, 5, 420)
					role.AddRoleItem(MapID, InstanceID, RoleID, 4830008, 2, -1, 8, 420)
					role.AddRoleExp(MapID, InstanceID, RoleID, 200000)
					HuoDeJiaozishipin(RoleID)
				elseif level >=60 and level <=79 then
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4830011, 5, 420)
					role.AddRoleItem(MapID, InstanceID, RoleID, 4830008, 2, -1, 8, 420)
					role.AddRoleExp(MapID, InstanceID, RoleID, 400000)
					HuoDeJiaozishipin(RoleID)
				elseif level >=80 then
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4830011, 5, 420)
					role.AddRoleItem(MapID, InstanceID, RoleID, 4830008, 2, -1, 8, 420)
					role.AddRoleExp(MapID, InstanceID, RoleID, 500000)
					HuoDeJiaozishipin(RoleID)
				elseif level <=29 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 444453) -- "���ڵ������\n	��û�и������ӣ��ҿɲ����ʱ���˷��������ϡ�"
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 444454)					--�ѳ����޶���������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444417) -- "���ڵ������\n	��û�и������ӣ��ҿɲ����ʱ���˷��������ϡ�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	elseif TalkIndex == 9 then
			if role.GetBagFreeSize(RoleID) < 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444443) -- "���ڵ������\n	�װ�����ң���ı������Ѿ�װ�����������Ҫ����һ�²��ܻ�ý�����"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444418) -- "���ڵ������\n	����һ�괺�ڵ������������Ϊ����Ӣ������ʥħ�����磬��Ҫ���Լ���һ����ô���Ϲ�أ��Ͻ�5��ˮ�����ӡ����ǲ��������ģ��ٺ١�\n "
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, 12) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end
	elseif TalkIndex == 12 then
		local a = role.GetRoleItemNum(RoleID, 4830010)
		if a > 4 then
			if role.GetRoleScriptData(RoleID, 1, RoleDataType["jiaofucishu"]) < 30 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444419) -- "���ڵ������\n    �Ǻǣ��ŵ���ζ������ʥħ���������Ͽ�ʼඣ�\n	��Ҫ��ý����ʹ������ˮ���������ɣ�"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
				if level >=30 and level <=39 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4830008, 1, -1, 8, 420)
					role.AddRoleExp(MapID, InstanceID, RoleID, 50000)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4830010, 5, 420)
					HuoDeJiaozishipin(RoleID)
				elseif level >= 40 and level <=49 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4830008, 1, -1, 8, 420)
					role.AddRoleExp(MapID, InstanceID, RoleID, 100000)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4830010, 5, 420)
					HuoDeJiaozishipin(RoleID)
				elseif level >= 50 and level <=59 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4830008, 1, -1, 8, 420)
					role.AddRoleExp(MapID, InstanceID, RoleID, 200000)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4830010, 5, 420)
					HuoDeJiaozishipin(RoleID)
				elseif level >=60 and level <= 79 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4830008, 1, -1, 8, 420)
					role.AddRoleExp(MapID, InstanceID, RoleID, 500000)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4830010, 5, 420)
					HuoDeJiaozishipin(RoleID)
				elseif level >=80 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4830008, 1, -1, 8, 420)
					role.AddRoleExp(MapID, InstanceID, RoleID, 1000000)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4830010, 5, 420)
					HuoDeJiaozishipin(RoleID)
				elseif level <=29 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 444453) -- "���ڵ������\n	��û�и������ӣ��ҿɲ����ʱ���˷��������ϡ�"
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 444454)					--�ѳ����޶���������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444420) -- "���ڵ������\n    ��û��ˮ�����ӣ��ҿɲ����ʱ���˷��������ϡ�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	elseif TalkIndex == 5 then
		local k = nil
		for i = 0, 4 do
		    if act.GetActScriptData(50, 1, i) ~= nil and act.GetActScriptData(50, 1, i) > 0 then
			    k = i
			end
		end
		if k == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444421) -- "���ڵ������\n    ���ڻ�û���˽��ɹ��κν��ӡ�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 0 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444422) -- "���ڵ������\n    �����ύ�´�������������ǣ�\n    XXX        XXX���´�����
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(50, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 5))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444423) -- "���ڵ������\n    �����ύ�´�������������ǣ�\n    XXX        XXX���´�����\n    XXX        XXX���´�����
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(50, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 5))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(50, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 6))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444424) -- "���ڵ������\n    �����ύ�´�������������ǣ�\n    XXX        XXX���´�����\n    XXX        XXX���´�����\n    XXX        XXX���´�����
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(50, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 5))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(50, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 6))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(50, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 7))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 3 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444425) -- "���ڵ������\n    �����ύ�´�������������ǣ�\n    XXX        XXX���´�����\n    XXX        XXX���´�����\n    XXX        XXX���´�����\n    XXX        XXX���´�����
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(50, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 5))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(50, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 6))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(50, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 7))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(50, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 8))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 4 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444426) -- "���ڵ������\n    �����ύ�´�������������ǣ�\n    XXX        XXX���´�����\n    XXX        XXX���´�����\n    XXX        XXX���´�����\n    XXX        XXX���´�����\n    XXX        XXX���´�����
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(50, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 5))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(50, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 6))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(50, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 7))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(50, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 8))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(50, 1, 4))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(50, 1, 9))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	elseif TalkIndex == 6 then
		local Rank = 0
		for q=0,2 do
			if act.GetActScriptData(50, 1, q) == RoleID then
				Rank = q + 1
			end
		end
		if Rank==0 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444427) -- "���ڵ������\n    ǰ��������û���������Ŷ����û��ʲô�������Ը��㡣"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif act.GetActScriptData(50, 1, Rank+9) > 0 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444428) -- "���ڵ������\n    ���Ѿ���ȡ�������ˣ����ظ���ȡ���ǲ���Ŷ��"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif role.GetBagFreeSize(RoleID) < 1 then
        	local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206350) -- "���ڵ������\n    ��ȥ����һ�±����ɣ���Ȼ��װ���½�Ʒ��Ŷ��"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
			--[[
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444452) -- "���ڵ������\n    ��л���ڱ���ʥħ�������������ľ޴��ף����ǽ���������ر�����Ǻǡ�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			--]]
			role.AddRoleItem(MapID, InstanceID, RoleID, 4830009, 1, -1, 8, 420)
			act.SetActScriptData(50, 1, Rank+9, 1)
			act.SaveActScriptData(50)
		end
    end
end

aux.RegisterCreatureEvent(6040018, 7, "n6040018_OnTalk")

function HuoDeJiaozishipin(RoleID)
	local i = role.GetRoleScriptData(RoleID, 1, RoleDataType["Jiaozishipin"])
	local x = act.GetActScriptData(50, 1, 14)

	if i == nil then
	    i = 0
	end
	if x == nil then
	    x = 0
	end
	for q=0,4 do                       --�ݴ����޸�
	    if act.GetActScriptData(50, 1, q) == RoleID then
		    if act.GetActScriptData(50, 1, q+5) >= i then
			    i = act.GetActScriptData(50, 1, q+5)
			end
		end
	end
	i = i + 5
	act.SetActScriptData(50, 1, 14, x+5)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["Jiaozishipin"], i)
	local arrID = {}
	local arrNum = {}
	local bNew = 1
	for l=0,4 do
		arrID[l] = act.GetActScriptData(50, 1, l)
		if arrID[l] == nil then
		    arrID[l] = 0
		end
		if arrID[l] == RoleID  then
			arrNum[l] = i
			bNew = 0
		else
			arrNum[l] = act.GetActScriptData(50, 1, l+5)
			if arrNum[l] == nil then
		        arrNum[l] = 0
		    end
		end
	end

	if bNew == 1 then
		arrID[5] = RoleID;
		arrNum[5] = i;
	else
		arrID[5] = 0;
		arrNum[5] = 0;
	end
	for m=0, 4 do
		for n=m+1,5 do
			if arrNum[n] > arrNum[m] then
				local temp = arrNum[m]
				arrNum[m] = arrNum[n]
				arrNum[n] = temp
				temp = arrID[m]
				arrID[m] = arrID[n]
				arrID[n] = temp
			end
		end
	end

	for j=0,4 do
		act.SetActScriptData(50, 1, j, arrID[j])
		act.SetActScriptData(50, 1, j+5, arrNum[j])
	end
	act.SaveActScriptData(50)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["jiaofucishu"], role.GetRoleScriptData(RoleID, 1, RoleDataType["jiaofucishu"])+1)
end












