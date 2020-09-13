--ħ�屦�ﴢ����

--0��¼�������Ŵ���
--10��¼����ȼ���
--1-6��¼����ID
--7λ��¼��ǰ����С������
--8λ��¼��ǰս���ķ���ID
--9λ��¼�����Ѷ����
--11-16��¼��Ӧ��ID
--21-26��¼��ͨ��ID
--��27λ��¼�����ID
--31-36��¼ÿ������ˢ�ִ���
--37λ��¼��ǰ�������ߵȼ�
--38λ��¼��ǰ����Ļ���
--39λ��6��Ӣ�۹�
--RoleDataType["mozubaoku"]= 88   ħ�屦�⿪����������
--RoleDataType["YXmozubaoku"]							= 111   --Ӣ��ħ�屦�⿪��������
--RoleDataType["YXmozubaokuNUM"]							= 112   --Ӣ��ħ�屦�⿪���������



function s56_OnPlayerEnter(MapID, InstanceID,RoleID)
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	local maxLevel = map.GetMapScriptData(MapID, InstanceID,1,37)
	if TeamID ~= nil and TeamID ~= 4294967295 then
		local Role = {}
		Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
		local level = 0
		for i = 1, 6 do
			if Role[i] and Role[i]~= 4294967295 then
				local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
				level = role.GetRoleLevel(Target_MapID, Target_InstanceID, Role[i])
			end
			if (level >= maxLevel) then
				maxLevel = level
			end
		end
	else
	    maxLevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	end
	map.SetMapScriptData(MapID, InstanceID, 1,37,maxLevel)  --37λ��¼��ǰ�������ߵȼ�
	if role.GetRoleScriptData(RoleID, 1, RoleDataType["YXmozubaoku"]) == 0 then
		if(maxLevel >= 60 and maxLevel <= 70) then
			map.SetMapScriptData(MapID, InstanceID, 1,10,1)   --10λ�ü�¼����ȼ���
		elseif (maxLevel >= 71 and maxLevel <= 85) then
			map.SetMapScriptData(MapID, InstanceID, 1,10,2)
		else
			map.SetMapScriptData(MapID, InstanceID, 1,10,3)
		end
	else
		if(maxLevel >= 60 and maxLevel <= 70) then
			map.SetMapScriptData(MapID, InstanceID, 1,10,1)   --10λ�ü�¼����ȼ���65
		elseif (maxLevel >= 71 and maxLevel <= 80) then--75
			map.SetMapScriptData(MapID, InstanceID, 1,10,2)
		elseif (maxLevel >= 81 and maxLevel <= 89) then--85
			map.SetMapScriptData(MapID, InstanceID, 1,10,3)
		elseif (maxLevel >= 90 and maxLevel <= 94) then--90
			map.SetMapScriptData(MapID, InstanceID, 1,10,4)
		elseif (maxLevel >= 95 and maxLevel <= 99) then--95
			map.SetMapScriptData(MapID, InstanceID, 1,10,5)
		else
			map.SetMapScriptData(MapID, InstanceID, 1,10,6)
		end
	end
	if(map.GetMapScriptData(MapID, InstanceID, 1,17)==0) then
		local teamLevel=map.GetMapScriptData(MapID, InstanceID, 1,10)
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["YXmozubaoku"]) == 0 then
			map.MapCreateColCreature(MapID, InstanceID, 1100152+teamLevel,305 ,39, 30, true, "");
		else
			map.MapCreateColCreature(MapID, InstanceID, 3041132+teamLevel,305 ,39, 30, true, "");
		end
		map.SetMapScriptData(MapID, InstanceID, 1,0,0)  --��¼���ŵĴ���
		map.SetMapScriptData(MapID, InstanceID, 1,17,1)
	end
end
aux.RegisterMapEvent("s56", 2, "s56_OnPlayerEnter");


function s56_1_OnPlayerEnter(MapID, InstanceID,RoleID)
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	local maxLevel = map.GetMapScriptData(MapID, InstanceID,1,37)
	if TeamID ~= nil and TeamID ~= 4294967295 then
		local Role = {}
		Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
		local level = 0
		for i = 1, 6 do
			if Role[i] and Role[i]~= 4294967295 then
				local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
				level = role.GetRoleLevel(Target_MapID, Target_InstanceID, Role[i])
			end
			if (level >= maxLevel) then
				maxLevel = level
			end
		end
	else
	    maxLevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	end
	map.SetMapScriptData(MapID, InstanceID, 1,37,maxLevel)  --37λ��¼��ǰ�������ߵȼ�
	if role.GetRoleScriptData(RoleID, 1, RoleDataType["YXmozubaoku"]) == 0 then
		if(maxLevel >= 60 and maxLevel <= 70) then
			map.SetMapScriptData(MapID, InstanceID, 1,10,1)   --10λ�ü�¼����ȼ���
		elseif (maxLevel >= 71 and maxLevel <= 85) then
			map.SetMapScriptData(MapID, InstanceID, 1,10,2)
		else
			map.SetMapScriptData(MapID, InstanceID, 1,10,3)
		end
	else
		if(maxLevel >= 60 and maxLevel <= 70) then
			map.SetMapScriptData(MapID, InstanceID, 1,10,1)   --10λ�ü�¼����ȼ���65
		elseif (maxLevel >= 71 and maxLevel <= 80) then--75
			map.SetMapScriptData(MapID, InstanceID, 1,10,2)
		elseif (maxLevel >= 81 and maxLevel <= 89) then--85
			map.SetMapScriptData(MapID, InstanceID, 1,10,3)
		elseif (maxLevel >= 90 and maxLevel <= 94) then--90
			map.SetMapScriptData(MapID, InstanceID, 1,10,4)
		elseif (maxLevel >= 95 and maxLevel <= 99) then--95
			map.SetMapScriptData(MapID, InstanceID, 1,10,5)
		else
			map.SetMapScriptData(MapID, InstanceID, 1,10,6)
		end
	end
	if(map.GetMapScriptData(MapID, InstanceID, 1,17)==0) then
		local teamLevel=map.GetMapScriptData(MapID, InstanceID, 1,10)
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["YXmozubaoku"]) == 0 then
			map.MapCreateColCreature(MapID, InstanceID, 1100152+teamLevel,305 ,39, 30, true, "");
		else
			map.MapCreateColCreature(MapID, InstanceID, 3041132+teamLevel,305 ,39, 30, true, "");
		end
		map.SetMapScriptData(MapID, InstanceID, 1,0,0)  --��¼���ŵĴ���
		map.SetMapScriptData(MapID, InstanceID, 1,17,1)
	end
end
aux.RegisterMapEvent("s56_1", 2, "s56_1_OnPlayerEnter");






function YXMZBKguai_On_Die(MapID, InstanceID, TargetID, TargetTypeID, RoleID)  -- Ӣ�۸������������¼�
	local monsterNum = map.GetMapScriptData(MapID, InstanceID,1,7)
	local monsterId = 0
	local doorId = 0;
	local change = map.GetMapScriptData(MapID, InstanceID,1,9)
	local temp = map.GetMapScriptData(MapID, InstanceID,1,10)
	local record = {1,2,3,4,5,6}
	local count = map.GetMapScriptData(MapID, InstanceID,1,0)
	local score = map.GetMapScriptData(MapID, InstanceID, 1,38)
	if monsterNum > 0 then
		map.SetMapScriptData(MapID, InstanceID,1,7,monsterNum-1)
	else
		doorId = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0)
		map.SetMapScriptData(MapID, InstanceID,1,27,0)
		YXMZBK_CreateMonster(MapID, InstanceID,doorId,change);
		if (count >= 3) then
			if map.GetMapScriptData(MapID, InstanceID, 1,39) == 1 then
				if(score <= 6) then
					monsterId = 3041119
				elseif (score >= 7  and score <= 9) then
					monsterId = 3041120
				elseif (score >= 10  and score <= 12) then
					monsterId = 3041121
				elseif (score >= 13  and score <= 14) then
					monsterId = 3041122
				else
					monsterId = 3041123
				end
				map.MapCreateColCreature(MapID, InstanceID,monsterId ,300 ,39, 230, true, ""); --ˢ��boss
			end
		else
			map.SetMapScriptData(MapID, InstanceID,1,39,0)
		end
		map.SetMapScriptData(MapID, InstanceID,1,8,0)
		map.SetMapScriptData(MapID, InstanceID,1,9,0)
	end


end
aux.RegisterCreatureEvent(3041101,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041102,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041103,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041104,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041105,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041106,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041107,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041108,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041109,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041110,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041111,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041112,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041113,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041114,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041115,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041116,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041117,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041118,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041125,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041126,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041127,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041128,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041129,4, "YXMZBKguai_On_Die");
aux.RegisterCreatureEvent(3041130,4, "YXMZBKguai_On_Die");

function MZBKputongguai_On_Die(MapID, InstanceID, TargetID, TargetTypeID, RoleID)  -- ��ͨ�������¼�
    local monsterNum = map.GetMapScriptData(MapID, InstanceID,1,7)
	map.SetMapScriptData(MapID, InstanceID,1,7,monsterNum-1)
end
aux.RegisterCreatureEvent(1100012,4, "MZBKputongguai_On_Die");
aux.RegisterCreatureEvent(1100013,4, "MZBKputongguai_On_Die");
aux.RegisterCreatureEvent(1100014,4, "MZBKputongguai_On_Die");
aux.RegisterCreatureEvent(1100015,4, "MZBKputongguai_On_Die");
aux.RegisterCreatureEvent(1100016,4, "MZBKputongguai_On_Die");
aux.RegisterCreatureEvent(1100017,4, "MZBKputongguai_On_Die");
aux.RegisterCreatureEvent(1100018,4, "MZBKputongguai_On_Die");
aux.RegisterCreatureEvent(1100019,4, "MZBKputongguai_On_Die");
aux.RegisterCreatureEvent(1100020,4, "MZBKputongguai_On_Die");
aux.RegisterCreatureEvent(1100021,4, "MZBKputongguai_On_Die");
aux.RegisterCreatureEvent(1100022,4, "MZBKputongguai_On_Die");
aux.RegisterCreatureEvent(1100023,4, "MZBKputongguai_On_Die");
aux.RegisterCreatureEvent(1100024,4, "MZBKputongguai_On_Die");
aux.RegisterCreatureEvent(1100025,4, "MZBKputongguai_On_Die");
aux.RegisterCreatureEvent(1100026,4, "MZBKputongguai_On_Die");
aux.RegisterCreatureEvent(1100027,4, "MZBKputongguai_On_Die");
aux.RegisterCreatureEvent(1100028,4, "MZBKputongguai_On_Die");
aux.RegisterCreatureEvent(1100029,4, "MZBKputongguai_On_Die");

function MZBKteshuguai_On_Die(MapID, InstanceID, TargetID, TargetTypeID, RoleID)  --����������¼�
	local monsterId = 0
	local monsterNum = 0;
	local doorId = 0;
	local change = map.GetMapScriptData(MapID, InstanceID,1,9)
	doorId = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0)
 	map.SetMapScriptData(MapID, InstanceID,1,27,0)
	MZBK_CreateMonster(MapID, InstanceID,doorId,change);
end
aux.RegisterCreatureEvent(1100030,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100031,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100032,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100033,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100034,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100035,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100036,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100037,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100038,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100039,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100040,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100041,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100042,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100043,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100044,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100045,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100046,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100047,4, "MZBKteshuguai_On_Die");

aux.RegisterCreatureEvent(1100048,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100049,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100050,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100051,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100052,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100053,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100054,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100055,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100056,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100057,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100058,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100059,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100060,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100061,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100062,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100063,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100064,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100065,4, "MZBKteshuguai_On_Die");

aux.RegisterCreatureEvent(1100066,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100067,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100068,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100069,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100070,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100071,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100072,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100073,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100074,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100075,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100076,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100077,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100078,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100079,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100080,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100081,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100082,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100083,4, "MZBKteshuguai_On_Die");

aux.RegisterCreatureEvent(1100084,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100085,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100086,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100087,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100088,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100089,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100090,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100091,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100092,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100093,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100094,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100095,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100096,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100097,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100098,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100099,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100100,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100101,4, "MZBKteshuguai_On_Die");

aux.RegisterCreatureEvent(1100102,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100103,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100104,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100105,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100106,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100107,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100108,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100109,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100110,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100111,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100112,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100113,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100114,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100115,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100116,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100117,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100118,4, "MZBKteshuguai_On_Die");
aux.RegisterCreatureEvent(1100119,4, "MZBKteshuguai_On_Die");

function MZBKteshuguai_On_Disappear(MapID, InstanceID, TargetID, TargetTypeID)  --�������ʧ�¼�
    local linghunId = 0
	local doorId = 0;
	local monsterTypeID = cre.GetCreatureTypeID(MapID, InstanceID, TargetID)
	doorId = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0)
    MZBK_CreateLinghun(MapID,InstanceID,doorId)
	for i = 1, 6 do
	    if(map.GetMapScriptData(MapID, InstanceID,1,10+i) == doorId) then
			map.SetMapScriptData(MapID, InstanceID,1,30+i,0)
		end
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 206602) --  ʱ�䵽�ˣ����¿�ʼ
	msg.AddMsgEvent(MsgID, 5, monsterTypeID)
	msg.DispatchMapMsgEvent(MsgID, MapID,InstanceID)
end
aux.RegisterCreatureEvent(1100030,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100031,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100032,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100033,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100034,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100035,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100036,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100037,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100038,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100039,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100040,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100041,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100042,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100043,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100044,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100045,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100046,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100047,13, "MZBKteshuguai_On_Disappear");

aux.RegisterCreatureEvent(1100048,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100049,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100050,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100051,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100052,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100053,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100054,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100055,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100056,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100057,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100058,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100059,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100060,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100061,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100062,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100063,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100064,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100065,13, "MZBKteshuguai_On_Disappear");

aux.RegisterCreatureEvent(1100066,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100067,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100068,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100069,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100070,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100071,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100072,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100073,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100074,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100075,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100076,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100077,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100078,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100079,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100080,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100081,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100082,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100083,13, "MZBKteshuguai_On_Disappear");

aux.RegisterCreatureEvent(1100084,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100085,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100086,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100087,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100088,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100089,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100090,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100091,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100092,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100093,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100094,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100095,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100096,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100097,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100098,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100099,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100100,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100101,13, "MZBKteshuguai_On_Disappear");

aux.RegisterCreatureEvent(1100102,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100103,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100104,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100105,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100106,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100107,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100108,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100109,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100110,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100111,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100112,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100113,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100114,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100115,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100116,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100117,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100118,13, "MZBKteshuguai_On_Disappear");
aux.RegisterCreatureEvent(1100119,13, "MZBKteshuguai_On_Disappear");


function MZBKtonglingguai_On_Die(MapID, InstanceID, TargetID, TargetTypeID, RoleID)  --ͳ��������¼�
    local count = map.GetMapScriptData(MapID, InstanceID,1,0)
	local monsterId = 0; --boss��Id
	local record = {1,2,3,4,5,6}
	local score = map.GetMapScriptData(MapID, InstanceID,1,38);
	local temp = map.GetMapScriptData(MapID, InstanceID,1,10)
	map.SetMapScriptData(MapID, InstanceID,1,8,0)
	map.SetMapScriptData(MapID, InstanceID,1,9,0)
	if (count == 3) then
	    if(score <= 6) then
			monsterId = 1100137
		elseif (score >= 7  and score <= 9) then
			monsterId = 1100140
		elseif (score >= 10  and score <= 12) then
			monsterId = 1100143
		elseif (score >= 13  and score <= 14) then
			monsterId = 1100146
		else
			monsterId = 1100149
		end
		map.MapCreateColCreature(MapID, InstanceID,monsterId+temp ,300 ,39, 230, true, ""); --ˢ��boss
	end
end
aux.RegisterCreatureEvent(1100120,4, "MZBKtonglingguai_On_Die");
aux.RegisterCreatureEvent(1100121,4, "MZBKtonglingguai_On_Die");
aux.RegisterCreatureEvent(1100122,4, "MZBKtonglingguai_On_Die");
aux.RegisterCreatureEvent(1100123,4, "MZBKtonglingguai_On_Die");
aux.RegisterCreatureEvent(1100124,4, "MZBKtonglingguai_On_Die");
aux.RegisterCreatureEvent(1100125,4, "MZBKtonglingguai_On_Die");
aux.RegisterCreatureEvent(1100126,4, "MZBKtonglingguai_On_Die");
aux.RegisterCreatureEvent(1100127,4, "MZBKtonglingguai_On_Die");
aux.RegisterCreatureEvent(1100128,4, "MZBKtonglingguai_On_Die");
aux.RegisterCreatureEvent(1100129,4, "MZBKtonglingguai_On_Die");
aux.RegisterCreatureEvent(1100130,4, "MZBKtonglingguai_On_Die");
aux.RegisterCreatureEvent(1100131,4, "MZBKtonglingguai_On_Die");
aux.RegisterCreatureEvent(1100132,4, "MZBKtonglingguai_On_Die");
aux.RegisterCreatureEvent(1100133,4, "MZBKtonglingguai_On_Die");
aux.RegisterCreatureEvent(1100134,4, "MZBKtonglingguai_On_Die");
aux.RegisterCreatureEvent(1100135,4, "MZBKtonglingguai_On_Die");
aux.RegisterCreatureEvent(1100136,4, "MZBKtonglingguai_On_Die");
aux.RegisterCreatureEvent(1100137,4, "MZBKtonglingguai_On_Die");

function MZBKBoss_On_Die(MapID, InstanceID, TargetID, TargetTypeID, RoleID)  --boss�����¼�
	map.OpenCloseDoor(MapID, InstanceID,460 , 1);  --�ر��ҵ��Ŵ�
	local position = {}
	local tempPos = {}
	position[1]={x=335,y=590,z=555}
	position[2]={x=335,y=590,z=530}
	position[3]={x=335,y=590,z=505}
	position[4]={x=335,y=590,z=480}
	position[5]={x=305,y=590,z=555}
	position[6]={x=305,y=461,z=530}
	position[7]={x=305,y=437,z=505}
	position[8]={x=305,y=590,z=480}
	position[9]={x=275,y=590,z=555}
	position[10]={x=275,y=590,z=530}
	position[11]={x=275,y=590,z=505}
	position[12]={x=275,y=590,z=480}
	for i =1, 12  do
		tempPos[i] = position[i]
	end
	for i =1 , 6  do
		table.remove(tempPos,math.random(#tempPos))
	end
	local score = map.GetMapScriptData(MapID, InstanceID,1,38);
	local bufflevel = 0
	if(score < 6) then
		i = 0
	elseif (score >= 7  and score <= 9) then
		i = 1
	elseif (score >= 10  and score <= 12) then
		i = 2
	elseif (score >= 13  and score <= 14) then
		i = 3
	else
		i = 4
	end
	for r = 1, 6 do
		map.MapCreateColCreature(MapID, InstanceID,5610059 ,tempPos[r].x ,tempPos[r].y, tempPos[r].z, true, ""); --ˢ������
		unit.AddBuff(MapID, InstanceID, 5610059,6105201+i*100,5610059)
	end
	--if role.GetRoleScriptData(RoleID, 1, RoleDataType["YXmozubaoku"]) ~= 0 then
		if TargetTypeID == 3041120 then
			map.MapCreateColCreature(MapID, InstanceID,3041131 ,position[12].x ,position[12].y, position[12].z, true, ""); --boss2����
		elseif TargetTypeID == 3041123 then
			map.MapCreateColCreature(MapID, InstanceID,3041132 ,position[12].x ,position[12].y, position[12].z, true, ""); --boss5����
		end
	--end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 206615)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
end
aux.RegisterCreatureEvent(1100138,4, "MZBKBoss_On_Die");
aux.RegisterCreatureEvent(1100139,4, "MZBKBoss_On_Die");
aux.RegisterCreatureEvent(1100140,4, "MZBKBoss_On_Die");
aux.RegisterCreatureEvent(1100141,4, "MZBKBoss_On_Die");
aux.RegisterCreatureEvent(1100142,4, "MZBKBoss_On_Die");
aux.RegisterCreatureEvent(1100143,4, "MZBKBoss_On_Die");
aux.RegisterCreatureEvent(1100144,4, "MZBKBoss_On_Die");
aux.RegisterCreatureEvent(1100145,4, "MZBKBoss_On_Die");
aux.RegisterCreatureEvent(1100146,4, "MZBKBoss_On_Die");
aux.RegisterCreatureEvent(1100147,4, "MZBKBoss_On_Die");
aux.RegisterCreatureEvent(1100148,4, "MZBKBoss_On_Die");
aux.RegisterCreatureEvent(1100149,4, "MZBKBoss_On_Die");
aux.RegisterCreatureEvent(1100150,4, "MZBKBoss_On_Die");
aux.RegisterCreatureEvent(1100151,4, "MZBKBoss_On_Die");
aux.RegisterCreatureEvent(1100152,4, "MZBKBoss_On_Die");

aux.RegisterCreatureEvent(3041119,4, "MZBKBoss_On_Die");
aux.RegisterCreatureEvent(3041120,4, "MZBKBoss_On_Die");
aux.RegisterCreatureEvent(3041121,4, "MZBKBoss_On_Die");
aux.RegisterCreatureEvent(3041122,4, "MZBKBoss_On_Die");
aux.RegisterCreatureEvent(3041123,4, "MZBKBoss_On_Die");

function MZBK_CreateDiwu(MapID, InstanceID) --ˢ�����ﺯ��
    local diwu = {}  --��������
	local diwuId=0
    diwu[1]={x=260,y=39,z=110}
    diwu[2]={x=260,y=39,z=230}
    diwu[3]={x=260,y=39,z=350}
    diwu[4]={x=350,y=39,z=350}
    diwu[5]={x=350,y=39,z=230}
    diwu[6]={x=350,y=39,z=110}
	for i = 1,#diwu do
	    if (map.GetMapScriptData(MapID, InstanceID,1,i) ==0) then
	        diwuId = map.MapCreateColCreature(MapID, InstanceID,5610061, diwu[i].x,diwu[i].y, diwu[i].z, true, "");
			if(i > 1) then
				unit.AddBuff(MapID, InstanceID, diwuId,6105201+i*100-200,diwuId)
			end
			map.SetMapScriptData(MapID, InstanceID,1,i,diwuId)  --1-6λ��¼���������id
			map.SetMapScriptData(MapID, InstanceID,1,10+i,325+i)     --11-16λ��¼��Ӧ�ŵ�id
--			map.SetMapScriptData(MapID, InstanceID,1,30+i,0)   --31-36λ��¼��Ӧ����ˢ�ִ���
		end
	end
end
function MZBK_CreateLinghun(MapID,InstanceID,doorId)
    local linghun={}
    linghun[326]={x=89,y=144,z=71}
	linghun[327]={x=89,y=144,z=268}
	linghun[328]={x=87,y=144,z=434}
	linghun[329]={x=520,y=144,z=431}
	linghun[330]={x=518,y=144,z=270}
	linghun[331]={x=525,y=144,z=70}
	linghunId = map.MapCreateColCreature(MapID, InstanceID,5610058 ,linghun[doorId].x ,linghun[doorId].y, linghun[doorId].z, true, ""); --ˢ����ʧ�����
	cre.SetCreatureScriptData(MapID, InstanceID, linghunId, 1, 0, doorId)
end
function MZBK_CreateMonster(MapID, InstanceID,doorId,change)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 206601) --  ս����ʼ��
	--msg.AddMsgEvent(MsgID, 5, TargetTypeID)
	msg.DispatchMapMsgEvent(MsgID, MapID,InstanceID)
    local monster={}; --��¼��������
	monster[326] = {}
	monster[327]={}
	monster[328]={}
	monster[329]={}
	monster[330]={}
	monster[331]={}
	monster[326][1]={x=110,y=39,z=110}
	monster[326][2]={x=110,y=39,z=70}
	monster[326][3]={x=110,y=39,z=30}
	monster[326][4]={x=60,y=39,z=110}
	monster[326][5]={x=60,y=39,z=70}
	monster[326][6]={x=60,y=39,z=30}
	monster[326][10]={x=89,y=150,z=71}    --��¼ͳ��ֵ�λ��

	monster[327][1]={x=110,y=39,z=310}
	monster[327][2]={x=110,y=39,z=270}
	monster[327][3]={x=110,y=39,z=230}
	monster[327][4]={x=60,y=39,z=310}
	monster[327][5]={x=60,y=39,z=270}
	monster[327][6]={x=60,y=39,z=230}
	monster[327][10]={x=89,y=150,z=268}

	monster[328][1]={x=110,y=39,z=470}
	monster[328][2]={x=110,y=39,z=430}
	monster[328][3]={x=110,y=39,z=390}
	monster[328][4]={x=60,y=39,z=470}
	monster[328][5]={x=60,y=39,z=430}
	monster[328][6]={x=60,y=39,z=390}
	monster[328][10]={x=87,y=150,z=434}

	monster[329][1]={x=495,y=39,z=390}
	monster[329][2]={x=495,y=39,z=430}
	monster[329][3]={x=495,y=39,z=470}
	monster[329][4]={x=545,y=39,z=390}
	monster[329][5]={x=545,y=39,z=430}
	monster[329][6]={x=545,y=39,z=470}
	monster[329][10]={x=520,y=150,z=431}

	monster[330][1]={x=495,y=39,z=230}
	monster[330][2]={x=495,y=39,z=270}
	monster[330][3]={x=495,y=39,z=310}
	monster[330][4]={x=545,y=39,z=230}
	monster[330][5]={x=545,y=39,z=270}
	monster[330][6]={x=545,y=39,z=310}
	monster[330][10]={x=518,y=150,z=270}

	monster[331][1]={x=495,y=39,z=30}
	monster[331][2]={x=495,y=39,z=70}
	monster[331][3]={x=495,y=39,z=110}
	monster[331][4]={x=545,y=39,z=30}
	monster[331][5]={x=545,y=39,z=70}
	monster[331][6]={x=545,y=39,z=110}
	monster[331][10]={x=525,y=150,z=70}
	local door={}   --��¼�ŵ�ID
	door[326]=0
	door[327]=3
	door[328]=6
	door[329]=9
	door[330]=12
	door[331]=15
	local num = 0
	local temp = 0  --��¼��ǰˢ�ֵķ���
	for i = 1, 6 do
	    if(map.GetMapScriptData(MapID, InstanceID,1,10+i) == doorId) then
			num = map.GetMapScriptData(MapID, InstanceID,1,30+i)
			temp = i
		end
	end
	local level = map.GetMapScriptData(MapID, InstanceID,1,10)  --��ȡ��ǰ����ȼ���
	local r = math.random(6)
	local s = 0
	local monsterId=0; --���ڼ�¼ˢ���Ĺ���ID
	local monsterNum=0 --��7λ��¼��ǰС�ֵ�����

		if(num < 5) then
			for i = 1,6 do
				if i ~= r then
					monsterNum=map.GetMapScriptData(MapID, InstanceID, 1,7)
					if (monsterNum < 20) then
						monsterId = map.MapCreateColCreature(MapID, InstanceID,1100011+level+door[doorId-change],monster[doorId][i].x ,monster[doorId][i].y, monster[doorId][i].z, true, "");  --ˢ����ͨ��
						map.SetMapScriptData(MapID, InstanceID, 1,20+i,monsterId)  --21-26λ��¼��ͨ��ID1100012
						map.SetMapScriptData(MapID, InstanceID, 1,7,monsterNum+1)
						cre.SetCreatureScriptData(MapID, InstanceID, monsterId, 1, 0, doorId)
					end
				else
					monsterId = map.MapCreateColCreature(MapID, InstanceID,1100029+level+num*18+door[doorId-change] ,monster[doorId][i].x ,monster[doorId][i].y, monster[doorId][i].z, true, "");  --ˢ�������
					map.SetMapScriptData(MapID, InstanceID, 1,27,monsterId)   --MapScriptData��27λ��¼�����ID
					cre.SetCreatureScriptData(MapID, InstanceID, monsterId, 1, 0, doorId)
				end
			end
			map.SetMapScriptData(MapID, InstanceID,1,30+temp,num+1)
		elseif(num == 5) then
			monsterId = map.MapCreateColCreature(MapID, InstanceID,1100119+level+door[doorId-change] ,monster[doorId][10].x ,monster[doorId][10].y, monster[doorId][10].z, true, "");  --ˢ��ͳ���
			map.SetMapScriptData(MapID, InstanceID,1,30+temp,0)
		end	

end


function YXMZBK_CreateMonster(MapID, InstanceID,doorId,change)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 206601) --  ս����ʼ��
	--msg.AddMsgEvent(MsgID, 5, TargetTypeID)
	msg.DispatchMapMsgEvent(MsgID, MapID,InstanceID)
    local monster={}; --��¼��������
	monster[326] = {}
	monster[327]={}
	monster[328]={}
	monster[329]={}
	monster[330]={}
	monster[331]={}
	monster[326][1]={x=110,y=39,z=110}
	monster[326][2]={x=110,y=39,z=70}
	monster[326][3]={x=110,y=39,z=30}
	monster[326][4]={x=60,y=39,z=110}
	monster[326][5]={x=60,y=39,z=70}
	monster[326][6]={x=60,y=39,z=30}
	monster[326][10]={x=89,y=150,z=71}    --��¼ͳ��ֵ�λ��

	monster[327][1]={x=110,y=39,z=310}
	monster[327][2]={x=110,y=39,z=270}
	monster[327][3]={x=110,y=39,z=230}
	monster[327][4]={x=60,y=39,z=310}
	monster[327][5]={x=60,y=39,z=270}
	monster[327][6]={x=60,y=39,z=230}
	monster[327][10]={x=89,y=150,z=268}

	monster[328][1]={x=110,y=39,z=470}
	monster[328][2]={x=110,y=39,z=430}
	monster[328][3]={x=110,y=39,z=390}
	monster[328][4]={x=60,y=39,z=470}
	monster[328][5]={x=60,y=39,z=430}
	monster[328][6]={x=60,y=39,z=390}
	monster[328][10]={x=87,y=150,z=434}

	monster[329][1]={x=495,y=39,z=390}
	monster[329][2]={x=495,y=39,z=430}
	monster[329][3]={x=495,y=39,z=470}
	monster[329][4]={x=545,y=39,z=390}
	monster[329][5]={x=545,y=39,z=430}
	monster[329][6]={x=545,y=39,z=470}
	monster[329][10]={x=520,y=150,z=431}

	monster[330][1]={x=495,y=39,z=230}
	monster[330][2]={x=495,y=39,z=270}
	monster[330][3]={x=495,y=39,z=310}
	monster[330][4]={x=545,y=39,z=230}
	monster[330][5]={x=545,y=39,z=270}
	monster[330][6]={x=545,y=39,z=310}
	monster[330][10]={x=518,y=150,z=270}

	monster[331][1]={x=495,y=39,z=30}
	monster[331][2]={x=495,y=39,z=70}
	monster[331][3]={x=495,y=39,z=110}
	monster[331][4]={x=545,y=39,z=30}
	monster[331][5]={x=545,y=39,z=70}
	monster[331][6]={x=545,y=39,z=110}
	monster[331][10]={x=525,y=150,z=70}
	local door={}   --��¼�ŵ�ID
	door[326]=0
	door[327]=3
	door[328]=6
	door[329]=9
	door[330]=12
	door[331]=15
	local num = 0
	local temp = 0  --��¼��ǰˢ�ֵķ���
	for i = 1, 6 do
	    if(map.GetMapScriptData(MapID, InstanceID,1,10+i) == doorId) then
			num = map.GetMapScriptData(MapID, InstanceID,1,30+i)
			temp = i
		end
	end
	local level = map.GetMapScriptData(MapID, InstanceID,1,10)  --��ȡ��ǰ����ȼ���
	local r = math.random(6)
	local s = 0
	local monsterId=0; --���ڼ�¼ˢ���Ĺ���ID
	local monsterNum=0 --��7λ��¼��ǰС�ֵ�����
	if(num ==0 or num ==1) then
			s = 0
		elseif(num ==2 or num ==3) then
			s = 6
		elseif(num == 5 or num == 4) then
			s = 12
		end
		if num < 6 then
			for i = 1,5 do
				if i < 5 then
					monsterNum=map.GetMapScriptData(MapID, InstanceID, 1,7)
					if (monsterNum < 20) then
						monsterId = map.MapCreateColCreature(MapID, InstanceID,3041100+s+level,monster[doorId][i].x ,monster[doorId][i].y, monster[doorId][i].z, true, "");  --ˢ����ͨ��
						map.SetMapScriptData(MapID, InstanceID, 1,20+i,monsterId)  --21-26λ��¼��ͨ��ID1100012
						map.SetMapScriptData(MapID, InstanceID, 1,7,monsterNum+1)
						cre.SetCreatureScriptData(MapID, InstanceID, monsterId, 1, 0, doorId)
					end
				else
					monsterId = map.MapCreateColCreature(MapID, InstanceID,3041124+level ,monster[doorId][i].x ,monster[doorId][i].y, monster[doorId][i].z, true, "");  --ˢ�������
					map.SetMapScriptData(MapID, InstanceID, 1,27,monsterId)   --MapScriptData��27λ��¼�����ID
					cre.SetCreatureScriptData(MapID, InstanceID, monsterId, 1, 0, doorId)
				end
			end
			map.SetMapScriptData(MapID, InstanceID,1,30+temp,num+1)
		end
		if num == 6 then
			map.SetMapScriptData(MapID, InstanceID,1,39,1)
		end
	

end





--��������Ի�
function menweidiaoxiang_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	local LeaderID = 0
	if TeamID ~= nil and TeamID ~= 4294967295 then
	    LeaderID = team.GetTeamLeaderID(TeamID)
	end
	if(LeaderID ~=nil and LeaderID ~= 4294967295 and LeaderID == RoleID) then
		if map.GetMapScriptData(MapID, InstanceID,1,7) <= 0 then
			if TalkIndex == -1 then
				local door = 0
				for i = 1, 6 do
					local diwuId=map.GetMapScriptData(MapID, InstanceID, 1,i)
					if(diwuId == TargetID) then
						door=map.GetMapScriptData(MapID, InstanceID, 1,10+i)
						if(door < 326) then
							door = 326
						elseif(door > 331) then
							door = 331
						end
					end
				end
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206603)   --��������Ի�
				msg.AddMsgEvent(MsgID, 9, door-325)
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
				msg.AddMsgEvent(MsgID, 1,100001)			--��ȷ����
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	100002)			--��ȡ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif TalkIndex == 4 then
				local door=0;
				local linghunId = 0;
				local num = 0;
				local score = map.GetMapScriptData(MapID, InstanceID, 1,38)
				num = map.GetMapScriptData(MapID, InstanceID, 1,0)
				local temp = 0
				temp = map.GetMapScriptData(MapID, InstanceID, 1,10)--����ȼ���
				local fighteDoor = map.GetMapScriptData(MapID, InstanceID, 1,8)
				if(fighteDoor ~= 0) then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206614)   --��������Ի�
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
					msg.AddMsgEvent(MsgID, 1,	100002)			--��ȡ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					for i = 1, 6 do
						local diwuId=map.GetMapScriptData(MapID, InstanceID, 1,i)
						if(diwuId == TargetID) then
							door=map.GetMapScriptData(MapID, InstanceID, 1,10+i)
							local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
							map.MapDeleteCreature(MapID, InstanceID,TargetID)  --ɾ����������
							map.SetMapScriptData(MapID, InstanceID, 1,i,1)  --�����ŵ�������¼���1
							map.SetMapScriptData(MapID, InstanceID, 1,38,score+i)
							if role.GetRoleScriptData(RoleID, 1, RoleDataType["YXmozubaoku"]) == 0 then
								monsterId = map.MapCreateColCreature(MapID, InstanceID,1100155+temp ,x,y,z, true, "");  --ˢ��������
							else
								monsterId = map.MapCreateColCreature(MapID, InstanceID,3041138+temp ,x,y,z, true, "");  --ˢ��������
							end
							unit.AddBuff(MapID, InstanceID, monsterId, 5941401, monsterId) --��ӳ�����Ч
							map.OpenCloseDoor(MapID, InstanceID,door , 1);  --������ſ���
							MZBK_CreateLinghun(MapID,InstanceID,door) --ˢ����ʧ�����
							map.SetMapScriptData(MapID, InstanceID, 1,0,num+1)
							map.SetMapScriptData(MapID, InstanceID, 1,8,door)
						end
					end
					num = map.GetMapScriptData(MapID, InstanceID, 1,0)
					if(num >=3) then  --����ѿ������ų���3�����򼤻�����ʣ�µ���������
						for i = 1 , 6 do
							local diwuId=map.GetMapScriptData(MapID, InstanceID, 1,i)
							if(diwuId ~=1) then
								local x,y,z = unit.GetPosition(MapID, InstanceID, diwuId)
								map.MapDeleteCreature(MapID, InstanceID,diwuId)  --ɾ����������
								map.SetMapScriptData(MapID, InstanceID, 1,i,-1)  --δ��������ʧ��������¼���-1

								if role.GetRoleScriptData(RoleID, 1, RoleDataType["YXmozubaoku"]) == 0 then
									monsterId = map.MapCreateColCreature(MapID, InstanceID,1100155+temp ,x,y,z, true, "");  --ˢ��������
								else
									monsterId = map.MapCreateColCreature(MapID, InstanceID,3041138+temp ,x,y,z, true, "");  --ˢ��������
								end
								unit.AddBuff(MapID, InstanceID, monsterId,5941401 , monsterId) --��ӳ�����Ч
							end
						end
					end
				end
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 605208)   --��ѹ�������ɾ��ڿ�����һ������
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100002)			--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206616)   --��������Ի�
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	100002)			--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(5610061, 7, "menweidiaoxiang_On_Talk")

--��ʧ�����Ի�
function linghun_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local doorId = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0)
	local teamLevel = map.GetMapScriptData(MapID, InstanceID, 1,10)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206604)   --��Ϊ��������Ǯ�������ĸ�����Ҫ�أ��ҵõ���ħ��ı��أ�����ȴʧȥ����������ҲҪ�ص��ҵĸ�����
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,100001)			--��ȷ����
		msg.AddMsgEvent(MsgID, 21,	5)			--�����Ѷ�
		msg.AddMsgEvent(MsgID, 1,206611)
		msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	100002)			--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		map.MapDeleteCreature(MapID, InstanceID,TargetID)  --ɾ����ʧ�����
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["YXmozubaoku"]) == 0 then
			MZBK_CreateMonster(MapID, InstanceID,doorId,0)   --ˢ����ͨ����\
		else
			YXMZBK_CreateMonster(MapID, InstanceID,doorId,0)   --ˢ��Ӣ�۹���\
		end

	elseif TalkIndex == 5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206612)   --����������ô����ɲ��ǰ׻�ģ���Ȼ�����ˣ�����������ڣ����������ͷĿ��ϵ���������԰��㽵���Ѷȡ������ѶȽ����ˣ����ĵ÷�Ҳ�ή�ͣ���������ˣ���Ȼ�ҵ�����Ҳ���ޣ�ÿ������ֻ�ܽ���һ���Ѷȣ��������޽��ͣ�
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	6)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,100001)			--��ȷ����
		msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	100002)			--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 6 then
		if(doorId <= 326 ) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206613)   --���Ѿ�������Ѷ��ˣ����򲻹��Ļ��ͳ�ȥ������������ɣ���Ȼ��Ҳ�����Ҹ�ǿ���Ķ��ѣ�
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100002)			--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			map.MapDeleteCreature(MapID, InstanceID,TargetID)  --ɾ����ʧ�����

			if role.GetRoleScriptData(RoleID, 1, RoleDataType["YXmozubaoku"]) == 0 then
				MZBK_CreateMonster(MapID, InstanceID,doorId,0)   --ˢ����ͨ����\
			else
				YXMZBK_CreateMonster(MapID, InstanceID,doorId,0)   --ˢ��Ӣ�۹���\
			end

			local score = map.GetMapScriptData(MapID, InstanceID, 1,38)
			map.SetMapScriptData(MapID, InstanceID, 1,38,score-1)
			map.SetMapScriptData(MapID, InstanceID, 1,9,1)
		end
	end

end
aux.RegisterCreatureEvent(5610058, 7, "linghun_On_Talk")

--����Ի�
function baoxiang_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local QuestExp = {}
	QuestExp[60]={RewMoney=127900,RewXP=507000}
	QuestExp[61]={RewMoney=131100,RewXP=525000}
	QuestExp[62]={RewMoney=134400,RewXP=544000}
	QuestExp[63]={RewMoney=137800,RewXP=564000}
	QuestExp[64]={RewMoney=141300,RewXP=582000}
	QuestExp[65]={RewMoney=144900,RewXP=606000}
	QuestExp[66]={RewMoney=148600,RewXP=627000}
	QuestExp[67]={RewMoney=152400,RewXP=648000}
	QuestExp[68]={RewMoney=156300,RewXP=670000}
	QuestExp[69]={RewMoney=160300,RewXP=720000}
	QuestExp[70]={RewMoney=164400,RewXP=1073000}
	QuestExp[71]={RewMoney=168600,RewXP=1107000}
	QuestExp[72]={RewMoney=172900,RewXP=1143000}
	QuestExp[73]={RewMoney=177300,RewXP=1180000}
	QuestExp[74]={RewMoney=181800,RewXP=1217000}
	QuestExp[75]={RewMoney=186400,RewXP=1256000}
	QuestExp[76]={RewMoney=191100,RewXP=1293000}
	QuestExp[77]={RewMoney=195900,RewXP=1331000}
	QuestExp[78]={RewMoney=200800,RewXP=1370000}
	QuestExp[79]={RewMoney=205900,RewXP=2116000}
	QuestExp[80]={RewMoney=211100,RewXP=2178000}
	QuestExp[81]={RewMoney=216400,RewXP=2241000}
	QuestExp[82]={RewMoney=221900,RewXP=2303000}
	QuestExp[83]={RewMoney=227500,RewXP=2370000}
	QuestExp[84]={RewMoney=233200,RewXP=2438000}
	QuestExp[85]={RewMoney=239100,RewXP=2505000}
	QuestExp[86]={RewMoney=245100,RewXP=2572000}
	QuestExp[87]={RewMoney=251300,RewXP=2640000}
	QuestExp[88]={RewMoney=257600,RewXP=2709000}
	QuestExp[89]={RewMoney=264100,RewXP=2782000}
	QuestExp[90]={RewMoney=270800,RewXP=3490000}
	QuestExp[91]={RewMoney=277600,RewXP=3546000}
	QuestExp[92]={RewMoney=284600,RewXP=3602000}
	QuestExp[93]={RewMoney=291800,RewXP=3662000}
	QuestExp[94]={RewMoney=299100,RewXP=3720000}
	QuestExp[95]={RewMoney=306600,RewXP=3777000}
	QuestExp[96]={RewMoney=314300,RewXP=3833000}
	QuestExp[97]={RewMoney=322200,RewXP=3887000}
	QuestExp[98]={RewMoney=330300,RewXP=3941000}
	QuestExp[99]={RewMoney=338600,RewXP=3996000}
	QuestExp[100]={RewMoney=347100,RewXP=4050000}

    if TalkIndex == -1 then
        local MsgID = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID, 20, 206606)   --���ضԻ�
	    msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
	    msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
	    msg.AddMsgEvent(MsgID, 1,100001)			--��ȷ����
	    msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
	    msg.AddMsgEvent(MsgID, 1,	100002)			--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
	    --local num = mozubaokuTimes[RoleID]
		local theDays = tonumber(os.date("%j"))
		local lastDay = role.GetRoleScriptData(RoleID, 1, RoleDataType["mozubaoku"])
		local score = map.GetMapScriptData(MapID, InstanceID,1,38);
		local level = map.GetMapScriptData(MapID, InstanceID,1,10)
		local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
		local tempLevel = 0
		if(rolelevel >= 60 and rolelevel <= 70) then
			tempLevel = 1
		elseif (rolelevel >= 71 and rolelevel <= 85) then
			tempLevel = 2
		else
			tempLevel = 3
		end
		if(theDays == lastDay) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206607) --  �Ѿ�������һ�Σ��޷��ٴο���
			msg.AddMsgEvent(MsgID, 21, -1)	-- ѡ��һ
			msg.AddMsgEvent(MsgID, 1, 100001)	-- "ȷ��"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			--------------------------���轱��
			--�жϱ������пռ��Ƿ��㹻
			local FreeSize = role.GetBagFreeSize(RoleID)
			if(FreeSize < 1) then
				--��ʾ��ұ����ռ䲻��
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)--��Ҫ����x�񱳰��ռ�ſɼ�������
				msg.AddMsgEvent(MsgID, 9, 2)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return 0
			end
			--mozubaokuTimes[RoleID] = 1
			map.MapDeleteCreature(MapID, InstanceID,TargetID)  --ɾ������
			role.SetRoleScriptData(RoleID, 1, RoleDataType["mozubaoku"],theDays)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["YXmozubaoku"],0)
			if MapID == 3510278698 then
				role.SetRoleScriptData(RoleID, 1, RoleDataType["YXmozubaokuNUM"],role.GetRoleScriptData(RoleID, 1, RoleDataType["YXmozubaokuNUM"])+1)
			end
			role.AddRoleExp(MapID, InstanceID, RoleID,QuestExp[rolelevel].RewXP)
			if(score <= 6) then
				i = 0
			elseif (score >= 7  and score <= 9) then
				i = 1
			elseif (score >= 10  and score <= 12) then
				i = 2
			elseif (score >= 13  and score <= 14) then
				i = 3
			else
				i = 4
			end
			if(i > 2) then
				local luck = math.random(100)
				if(luck <= i) then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800187+tempLevel-1, 1, -1, 8, 420)
				elseif(i < 4 and luck <= i*2) then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800172+i+1+(tempLevel-1)*5, 1, -1, 8, 420)
				else
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800172+i+(tempLevel-1)*5, 1, -1, 8, 420)
				end
			elseif(i>=0 and i<= 2 ) then
				local luck = math.random(100)
				if(luck <= i*4) then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800172+i+1+(tempLevel-1)*5, 1, -1, 8, 420)
				else
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800172+i+(tempLevel-1)*5, 1, -1, 8, 420)
				end
			end
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3424073512,610,23094,669 )
		end

	end

end
aux.RegisterCreatureEvent(5610059, 7, "baoxiang_On_Talk")

--[[function Mozubaoku_OnEnd(actID)

	map.MapDeleteCreature(3424073512, -1,5610060 )  --�����ɾ��NPC
    mozubaokuTimes = {}
end
aux.RegisterActEvent(42, 3, "Mozubaoku_OnEnd")]]

--Կ��ʹ��
function I4820052_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	if(MapID == 3373740840 or MapID == 3510278698) then
	local x,y,z=unit.GetPosition(MapID, InstanceID, TargetID)
		if(x<287 or x>321 or z<55 or z>73 ) then
		    bRet = 43
		end
	else
		    bRet = 47
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4820052, 0, "I4820052_CanUse")

function I4820052_Use(MapID, InstanceID, TypeID, TargetID)
		map.OpenCloseDoor(MapID, InstanceID,459, 1);  --��ڴ����ſ���
		MZBK_CreateDiwu(MapID, InstanceID);
end
aux.RegisterItemEvent(4820052, 1, "I4820052_Use")


--���⴫���ŵĶԻ�
function c5610060_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local theDays = tonumber(os.date("%j"))
	local lastDay = role.GetRoleScriptData(RoleID, 1, RoleDataType["mozubaoku"])
	local YXjinru = role.GetRoleScriptData(RoleID, 1, RoleDataType["YXmozubaoku"])
	--local Num = role.GetRoleScriptData(RoleID, 1, RoleDataType["YXmozubaokuNUM"])--����Ӣ�۱������
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	local menpiao = role.GetRoleItemNum(RoleID, 4810028)--��Ʊ����
	
	
	if theDays ~= lastDay  then
		role.SetRoleScriptData(RoleID, 1,  RoleDataType["YXmozubaoku"],0)
		role.SetRoleScriptData(RoleID, 1,  RoleDataType["YXmozubaokuNUM"],0)
	end

		if TeamID ==nil or TeamID == 4294967295 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 100004)		--����û����ӣ����ܽ���ø�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 6)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if GetTeamPlayerNum(MapID,InstanceID,RoleID) < 3 then----------------------------------------
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 	203002)	--������Ҫ�չ�3�����Ѳ��ܲμӴ˻��
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, 	6)		--ȡ����ť
				msg.AddMsgEvent(MsgID, 1, 	100002)	--��ȡ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if role.GetRoleLevel(MapID, InstanceID, RoleID) < 60 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206609)  		--ֻ��60�����ϵ���Ҳ��ܹ�����ħ�屦��Ѱ��
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.AddMsgEvent(MsgID, 21, 6)
					msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					if TalkIndex == -1 then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 206608)		--ħ�屦��������˱��أ�ֻҪ��ĵȼ�����60�����ҾͿ��Դ���ȥ��׼��������\nע�⣺����һ�����������䣬ֻ������������Ĺ���Ϳ��Ի�ñ��ء���ÿ������Ĺ��ﶼ��ͬ���ſڵĻ�����ʿ����Խ����������ǿ��Խ��.\����Ӣ��ħ�屦����Ҫÿ��1��Ӣ�۱�������֤
						msg.AddMsgEvent(MsgID, 24, TargetID)
						if role.GetRoleScriptData(RoleID, 1, RoleDataType["YXmozubaoku"]) == 0 then
							msg.AddMsgEvent(MsgID, 21, 4)			--
							msg.AddMsgEvent(MsgID, 1, 206617)			--������ͨħ�屦��
							msg.AddMsgEvent(MsgID, 21, 5)			--
							msg.AddMsgEvent(MsgID, 1, 206618)			--����Ӣ��ħ�屦��
						else
							msg.AddMsgEvent(MsgID, 21, 5)			--
							msg.AddMsgEvent(MsgID, 1, 206618)			--����Ӣ��ħ�屦��
						end
						msg.AddMsgEvent(MsgID, 21, 6)			--
						msg.AddMsgEvent(MsgID, 1, 100002)			--��ȡ����
						msg.DispatchRoleMsgEvent(RoleID, MsgID)

					elseif TalkIndex == 4 then
						--if role.GetRoleScriptData(RoleID, 1, RoleDataType["YXmozubaoku"]) == 0 then
							if theDays == lastDay  then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206607)  		--���λ�����Ѿ�������һ����ͨ�����ˣ����´λ�����ɣ�
								msg.AddMsgEvent(MsgID, 24, TargetID)
								msg.AddMsgEvent(MsgID, 21, 6)
								msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3373740840,416, 1, 30 )
								role.SetRoleScriptData(RoleID, 1,  RoleDataType["YXmozubaoku"],0)
							end
						--[[else
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 605206)  		--�����ڶ����Ѿ����˽���Ӣ�۸������㲻�ܽ�����ͨ������
							msg.AddMsgEvent(MsgID, 24, TargetID)
							msg.AddMsgEvent(MsgID, 21, 6)
							msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						end]]
					elseif TalkIndex == 5 then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 605207)		--
						msg.AddMsgEvent(MsgID, 24, TargetID)
						msg.AddMsgEvent(MsgID, 21, 7)			--
						msg.AddMsgEvent(MsgID, 1, 100001)			--ȷ��	
						msg.AddMsgEvent(MsgID, 21, 6)			--
						msg.AddMsgEvent(MsgID, 1, 100002)			--��ȡ����
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					elseif TalkIndex == 7 then
						if menpiao > 0 then
							--[[if YXjinru == 1  then
								if Num > 3 then
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 20, 206619)  		--���λ�����Ѿ����������α����ˣ����´λ�����ɣ�
									msg.AddMsgEvent(MsgID, 24, TargetID)
									msg.AddMsgEvent(MsgID, 21, 6)
									msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
								else
									role.RemoveFromRole(MapID, InstanceID, RoleID, 4810028, 1, 101)
									role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3373740840,416, 1, 30 )
								end
							else	]]
								role.SetRoleScriptData(RoleID, 1,  RoleDataType["YXmozubaoku"],1)--
								role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3510278698,416, 1, 30 )
								role.RemoveFromRole(MapID, InstanceID, RoleID, 4810028, 1, 101)
							--end
						else
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 206620)  		--��û��Ӣ�۱�������֤���޷�����Ӣ��ħ�屦�⣬��ӵ��Ӣ�۱�������֤������
							msg.AddMsgEvent(MsgID, 24, TargetID)
							msg.AddMsgEvent(MsgID, 21, 6)
							msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						end
					end
				end
			end
		end
end
aux.RegisterCreatureEvent(5610060, 7, "c5610060_On_Talk")
