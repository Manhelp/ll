function f20_Creature()
	map.CreateInstance(1232817742, 1)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 444456) --�����ؾ�֮���Ѿ������ڱ�����ѩ�ġ�&lt;p1&gt;��&lt;p2&gt;��,�������������������?
	msg.AddMsgEvent(MsgID, 9, 2642)
	msg.AddMsgEvent(MsgID, 9, 2557)
	msg.DispatchBroadcast(MsgID,-1,-1,-1)
end



function f20_delete()
	map.DeleteInstance(1232817742, 1)
end

function f20_MonsterCreate(curhour,curmin)
	map.MapCreateColCreature(1232817742, 1, 3040367,1036,2297,1145, 1)
	map.MapCreateColCreature(1232817742, 1, 3040368,1036,2297,1275, 1)
	map.MapCreateColCreature(1232817742, 1, 3040369,1036,2297,970, 1)
end



--����ڻ�е��ߺ�����
function f20_CanEnterWhenOnline(MapID, RoleID)
	--������ҵ����ߵ�ͼ������todo
	return 3424073512,610,23094,669
end

aux.RegisterMapEvent("f20",  12, "f20_CanEnterWhenOnline")

-------------------------------------------------------------------------------------------------------
--�ҵ���ͼ�ĳ��ں�����
function f20_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3424073512,610,23094,669
end

aux.RegisterMapEvent("f20",  13, "f20_GetExporMapAndCoord")


--�ҵ�����ʵĸ���ʵ��
function f20_GetOnePerfectMap(MapID, InstanceID, RoleID)
	--local curhour = tonumber(os.date("%H"))--Сʱ

	return 1

end
aux.RegisterMapEvent("f20",  14, "f20_GetOnePerfectMap")



--�������Ƿ��ܹ���������
function f20_CanEnter(MapID, InstanceID, RoleID)

	return 1

end

aux.RegisterMapEvent("f20",  15, "f20_CanEnter")


