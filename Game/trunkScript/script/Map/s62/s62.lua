function s62_Creature(curhour)
	map.CreateInstance(3390519080, curhour)
	local mijingtemp = math.random(2)
	if(mijingtemp == 1) then
		map.MapCreateColCreature(3424071720, -1,5600006 ,1180,10484,1089, true, "");
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 300091) --�����ؾ�֮���Ѿ������ڱ�����ѩ�ġ�&lt;p1&gt;��&lt;p2&gt;��,�������������������?
		msg.AddMsgEvent(MsgID, 9, 1180)
		msg.AddMsgEvent(MsgID, 9, 1089)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	else
		map.MapCreateColCreature(3424071720, -1,5600006 ,2642,7400,2557, true, "");
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 300091) --�����ؾ�֮���Ѿ������ڱ�����ѩ�ġ�&lt;p1&gt;��&lt;p2&gt;��,�������������������?
		msg.AddMsgEvent(MsgID, 9, 2642)
		msg.AddMsgEvent(MsgID, 9, 2557)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end
	map.MapCreateColCreature(3390519080, curhour,5600005 ,113,2010,192, true, "")
	local creature = {}
	local id = 0
	creature[1] = {115,1994,160}
	creature[2] = {115,1994,120}
	creature[3] = {135,1994,140}
	creature[4] = {95,2010,140}
	for i=1,4 do
		local temp = math.random(4)
		if(temp <= 2) then
			id = 1092012 + temp -1
		else
			id = 1092016 + temp -3
		end
		map.MapCreateColCreature(3390519080, curhour,id ,creature[i][1],creature[i][2],creature[i][3], true, "")
	end

end



function s62_delete(curhour)
	map.DeleteInstance(3390519080, curhour - 1)
end





--����ڻ�е��ߺ�����
function s62_CanEnterWhenOnline(MapID, RoleID)
	--������ҵ����ߵ�ͼ������todo
	return 3424073512,610,23094,669
end

aux.RegisterMapEvent("s62",  12, "s62_CanEnterWhenOnline")

-------------------------------------------------------------------------------------------------------
--�ҵ���ͼ�ĳ��ں�����
function s62_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3424073512,610,23094,669
end

aux.RegisterMapEvent("s62",  13, "s62_GetExporMapAndCoord")


--�ҵ�����ʵĸ���ʵ��
function s62_GetOnePerfectMap(MapID, InstanceID, RoleID)
	local curhour = tonumber(os.date("%H"))--Сʱ

	return curhour

end
aux.RegisterMapEvent("s62",  14, "s62_GetOnePerfectMap")



--�������Ƿ��ܹ���������
function s62_CanEnter(MapID, InstanceID, RoleID)

	return 1

end

aux.RegisterMapEvent("s62",  15, "s62_CanEnter")




function mijingyaojing_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local tbl_JXG = {}
	tbl_JXG[1] = 6010048
	tbl_JXG[2] = 6010052
	tbl_JXG[3] = 6010053
	tbl_JXG[4] = 6010054
	tbl_JXG[5] = 6010056
	tbl_JXG[6] = 6010057
	tbl_JXG[7] = 6010058
	tbl_JXG[8] = 6010059
    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local num = 0
	local temp = math.random(2)
	if(temp == 1 ) then
		map.MapCreateCreature(MapID, InstanceID, tbl_JXG[math.random(6)], x, y, z)
		num = num + 1
	elseif(temp == 2 and num < 5) then
		map.MapCreateCreature(MapID, InstanceID, tbl_JXG[math.random(6)], x, y, z)
		map.MapCreateCreature(MapID, InstanceID, tbl_JXG[math.random(6)], x-10, y, z+10)
		num = num + 1
	end
end

aux.RegisterCreatureEvent(1092012,4,"mijingyaojing_OnDie")
aux.RegisterCreatureEvent(1092013,4,"mijingyaojing_OnDie")
aux.RegisterCreatureEvent(1092016,4,"mijingyaojing_OnDie")
aux.RegisterCreatureEvent(1092017,4,"mijingyaojing_OnDie")
