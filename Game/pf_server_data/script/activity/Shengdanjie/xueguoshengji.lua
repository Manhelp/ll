
--scriptdata����0λ��¼�Ƿ�ʼˢ��
--scriptdata����1λ��¼�ӵ����ʼ�������Ѿ��ﵽ���ٷ���
--scriptdata, ��2λ��¼�ϴ�ˢ�ֵĵȼ�

--���ͽ�����

function x5600012_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --��ȡ��ҵȼ�
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206301)		--��ѩ��ʥ�������ˣ�\n    �����ѩ��ʥ����սΰ�������ѩ��ǿ�����������������ѩ�����ƶ��Ĺ���\n    ���ȣ���Ķ���������������4��������ѩ���ǻ���Ϊ�����Ʋ������Ƕ��ܾ����㽻�֣���Σ����ǵĶ�Ա֮��ĵȼ��������̫�࣬����ѩ���ǻῴ�������Ƕ��ܾ������ǽ��֣�������������Ҫ׼��һЩѩ��ϲ����
		--ѩ֮�������¸���ǵ�Ů��������Ů���ῴ�������Ƕ�����ͬ�����Ǹ����������֡�\n    ���������ս���еõ�ѩ���ǵ����أ���ô�㽫���л�����ϡ�е�ʥ����������������׼��������׼�������ҾͰ����ͽ�ѩ��ʥ����
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	206302)		--������ѩ��ʥ����
		msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	100002)		--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
		local Num = 0
		local minlv = 0
		local maxlv = 0
		local IsInXGSJ = false
		--local TotalLv = 0
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level < 30 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206303)		--��ѩ��ʥ�������ˣ�\n    ֻ�г���30������Ҳ��ܽ���ѩ��ʥ����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else              --if TeamID ~= nil and TeamID ~= 4294967295 then
			--[[local Role = {}
			Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
			for i=1, 6 do
				if Role[i] and Role[i]~= 4294967295 then
				    local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
				    local rolelevel = role.GetRoleLevel(Target_MapID, Target_InstanceID, Role[i])
					if minlv == 0 then
					    minlv = rolelevel
						maxlv = rolelevel
					end
					if maxlv < rolelevel then
					    maxlv = rolelevel
					elseif minlv > rolelevel then
					    minlv = rolelevel
					end
					if Target_MapID == 619034554 then
					    IsInXGSJ = true
					end
					--TotalLv = rolelevel + TotalLv
					Num = Num + 1
				end
			end
			if Num < 4 and not IsInXGSJ then
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206304)		--��ѩ��ʥ�������ˣ�\n    �������3���������������Ӳ��ܽ���ѩ��ʥ����
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	100001)		--��ȡ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			elseif maxlv - minlv > 20 then
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206305)		--��ѩ��ʥ�������ˣ�\n    �������е���ҵȼ������󣨴���20�������޷�����ѩ��ʥ����
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	100001)		--��ȡ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else]]
			    role.RoleGotoNewMap(MapID, InstanceID, RoleID, 619034554, 72, 14409, 105)
			--[[end
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206306)		--��ѩ��ʥ�������ˣ�\n    �����ӵ���Լ��Ķ����Ҳ���ͬ�������ѩ��ʥ����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)]]
		end
	end
end
--ע��ѩ��ʥ�������˶Ի�
aux.RegisterCreatureEvent(5600012, 7, "x5600012_OnTalk")

function x5600016_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    if TalkIndex == -1 then
	    if map.GetMapScriptData(MapID,InstanceID,1,0) ~= 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206307)		--������ѩ��Ů����\n     ����õ�ʥ�����������������ó��������������10��ѩ֮��ң��Ҿ��ú����ǿ�����һ�¡���
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	4)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	206308)		--���Ͻ���ҡ�
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100002)		--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206315)		--������ѩ��Ů����\n     �������սʤ�ҵ�������ô�����ϣ�����ʥ�������������ˡ���
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100002)		--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 4 then
	    if role.GetRoleItemNum(RoleID, 4800002) >= 10 then
		    role.RemoveFromRole(MapID, InstanceID, RoleID, 4800002, 10, 420)
			map.SetMapScriptData(MapID,InstanceID,1,0,1)
			map.MapCreateColCreature(MapID, InstanceID, 5513111, 1, 1, 1, "")
			map.SetMapScriptData(MapID,InstanceID,1,1,2)
			XGSJ_CreatCreature(MapID, InstanceID)
        else
    		local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206309)		--������ѩ��Ů����\n     û�д�ѩ֮����������ˣ���ı������Һ�ʧ������Ҫ���Ҷ���ʧȥ��������ԣ���
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	4)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	206308)		--���Ͻ���ҡ�
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100002)		--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--ע��
aux.RegisterCreatureEvent(5600016, 7, "x5600016_OnTalk")


--��15��������ʱ�䣬����ʱ��ʹ��
function c5513111_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
    map.SetMapScriptData(MapID,InstanceID,1,1,map.GetMapScriptData(MapID,InstanceID,1,1)+1)
	local k = map.GetMapScriptData(MapID,InstanceID,1,1)
	if k < 20 then
		map.MapCreateColCreature(MapID, InstanceID, 5513111, 1, 1, 1, "")
		map.MapCreateColCreature(MapID, InstanceID, 5513112, 1, 1, 1, "")
	end
	local monster = {}
    monster[2] = 1
	monster[4] = 1
	monster[5] = 1
	monster[6] = 1
	monster[8] = 1
	monster[10] = 1
	monster[12] = 1
	monster[14] = 1
	monster[15] = 1
	monster[16] = 1
	monster[18] = 1
	monster[20] = 1
	if monster[k] ~= nil then
	    XGSJ_CreatCreature(MapID, InstanceID)
	end
	if k == 4 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,	206310)		--��                1���Ӻ�BOSS�������ѩ�˼������֣�������׼������
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	elseif k == 9 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,	206311)		--��                1���Ӻ��ŭ����ѩ�˾��񼴽����֣�������׼������
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	elseif k == 14 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,	206312)		--��                      1���Ӻ����������߼������֣�������׼������
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	elseif k == 19 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,	206313)		--��         1���Ӻ�Զ������ѩ�˰����������֣�������׼��������ѩ��ʥ��������ս������
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end
end

aux.RegisterCreatureEvent(5513111, 13, "c5513111_OnDisappear")

--��15��������ʱ�䣬����ʱ��ʹ��
function c5513112_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
    local k = map.GetMapScriptData(MapID,InstanceID,1,1)
	if k % 2 == 1 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,	206314)		--��                            ��һ�ֵ�ѩ�˽�������30���ʼ����
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end
end

aux.RegisterCreatureEvent(5513112, 13, "c5513112_OnDisappear")

function XGSJ_CreatCreature(MapID, InstanceID)
    local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
	local totallevel = 0
	local num = 0
	local Player = {}
	local index = 0
	for u,v in pairs(RoleTbl) do
		if u ~= nil then
			local level = role.GetRoleLevel(MapID, InstanceID, v)
			totallevel = totallevel + level
			num = num + 1
			index = index + 1
			Player[index] = v
		end
	end
	local k = 1
	if num == 0 then
	    k = map.GetMapScriptData(MapID,InstanceID,1,2)
	else
	    k = math.floor(totallevel/num/10) - 2
		if k > 7 then
		   k = 7
		elseif k < 1 then
		   k = 1
		end
		map.SetMapScriptData(MapID,InstanceID,1,2,k)
	end
	local monster = {}
    monster[2] = {mon = {1095001,1095008}, num = {7,3}}
	monster[4] = {mon = {1095001,1095008}, num = {5,5}}
	monster[5] = {mon = {1095029}, num = {1}}
	monster[6] = {mon = {1095001,1095008}, num = {5,5}}
	monster[8] = {mon = {1095001,1095008,1095015}, num = {7,5,3}}
	monster[10] = {mon = {1095001,1095008,1095015,1095036}, num = {4,5,5,1}}
	monster[12] = {mon = {1095001,1095008,1095015}, num = {6,6,6}}
	monster[14] = {mon = {1095008,1095015}, num = {12,8}}
	monster[15] = {mon = {1095043}, num = {1}}
	monster[16] = {mon = {1095008,1095015,1095022}, num = {10,8,2}}
	monster[18] = {mon = {1095008,1095015,1095022}, num = {14,8,4}}
	monster[20] = {mon = {1095008,1095015,1095022,1095049}, num = {10,10,5,1}}
	local MapPoint = {}
	MapPoint[1] = {x = 180,y = 12169,z = 194}
	MapPoint[2] = {x = 197,y = 12037,z = 192}
	MapPoint[3] = {x = 175,y = 12190,z = 177}
	MapPoint[4] = {x = 194,y = 12128,z = 170}
	MapPoint[5] = {x = 191,y = 11908,z = 213}
	MapPoint[6] = {x = 202,y = 12179,z = 156}
	MapPoint[7] = {x = 167,y = 12090,z = 158}
	MapPoint[8] = {x = 213,y = 12117,z = 174}
	MapPoint[9] = {x = 223,y = 12002,z = 156}
	MapPoint[10] = {x = 214,y = 11879,z = 130}
	MapPoint[11] = {x = 196,y = 12038,z = 135}
	MapPoint[12] = {x = 182,y = 11891,z = 120}
	MapPoint[13] = {x = 163,y = 11964,z = 135}
	MapPoint[14] = {x = 146,y = 11902,z = 160}
	MapPoint[15] = {x = 145,y = 11890,z = 186}
	MapPoint[16] = {x = 159,y = 11991,z = 203}
	MapPoint[17] = {x = 164,y = 11838,z = 222}
	MapPoint[18] = {x = 181,y = 11850,z = 228}
	MapPoint[19] = {x = 174,y = 11903,z = 252}
	MapPoint[20] = {x = 196,y = 11790,z = 254}
	MapPoint[21] = {x = 216,y = 11760,z = 256}
	MapPoint[22] = {x = 215,y = 11911,z = 237}
	MapPoint[23] = {x = 222,y = 11940,z = 203}
	MapPoint[24] = {x = 221,y = 11974,z = 220}
	MapPoint[25] = {x = 238,y = 11783,z = 146}
	MapPoint[26] = {x = 186,y = 12183,z = 186}

	local minute = map.GetMapScriptData(MapID,InstanceID,1,1)
	local point = 1
	for u,v in pairs(monster[minute].mon) do
	    if u ~= nil then
		    for i = 1,monster[minute].num[u] do
			    if monster[minute].num[u] == 1 then
				    local MonID = map.MapCreateColCreature(MapID, InstanceID, v+k-1, MapPoint[26].x, MapPoint[26].y, MapPoint[26].z, "")
					if index and index > 0 then
						cre.AddEnmity(MapID, InstanceID, MonID, Player[math.random(1,index)], 1)
					end
				else
					local MonID = map.MapCreateColCreature(MapID, InstanceID, v+k-1, MapPoint[point].x, MapPoint[point].y, MapPoint[point].z, "")
					point = point + 1
					if index and index > 0 then
						cre.AddEnmity(MapID, InstanceID, MonID, Player[math.random(1,index)], 1)
					end
				end
			end
		end
	end
end
