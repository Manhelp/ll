---mapscriptdata[1] ��¼���������Ƿ���ˢ������
--[2]��¼��һ�Ƿ���ɱ��
--[3]��¼�϶��Ƿ���ɱ��
--[4]��¼�����Ƿ���ɱ��
--[5]��¼�����Ƿ���ɱ��






function xiaoguai_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	local a,b,c,d = map.GetMapScriptData(MapID, InstanceID, 4, 2,3,4,5)
	local poss=math.random(1,100)
	
	if poss>80 then

		if a==0 then
			cre.MonsterSay(MapID, InstanceID, TargetID, 10057)
		elseif b==0 then
			cre.MonsterSay(MapID, InstanceID, TargetID, 10058)
		elseif c==0 then
			cre.MonsterSay(MapID, InstanceID, TargetID, 10059)
		elseif d==0 then
			cre.MonsterSay(MapID, InstanceID, TargetID, 10060)
		end
	end

	
	possiblity=math.random(1,100)
	if TargetTypeID==3030004 and possiblity>80 then
		local x,y,z=unit.GetPosition(MapID, InstanceID,TargetID)
		map.MapCreateCreature(MapID, InstanceID, TargetTypeID, x, y, z)
	end
	if TargetTypeID==3030005 and possiblity>80 then
		local x,y,z=unit.GetPosition(MapID, InstanceID,TargetID)
		map.MapCreateCreature(MapID, InstanceID, TargetTypeID, x, y, z)
	end


end

aux.RegisterCreatureEvent(3030001, 4, "xiaoguai_OnDie")
aux.RegisterCreatureEvent(3030002, 4, "xiaoguai_OnDie")
aux.RegisterCreatureEvent(3030004, 4, "xiaoguai_OnDie")
aux.RegisterCreatureEvent(3030005, 4, "xiaoguai_OnDie")
aux.RegisterCreatureEvent(3030006, 4, "xiaoguai_OnDie")
aux.RegisterCreatureEvent(3030008, 4, "xiaoguai_OnDie")
aux.RegisterCreatureEvent(3030009, 4, "xiaoguai_OnDie")
aux.RegisterCreatureEvent(3030010, 4, "xiaoguai_OnDie")
aux.RegisterCreatureEvent(3030011, 4, "xiaoguai_OnDie")
aux.RegisterCreatureEvent(3030012, 4, "xiaoguai_OnDie")
aux.RegisterCreatureEvent(3030014, 4, "xiaoguai_OnDie")
aux.RegisterCreatureEvent(3030015, 4, "xiaoguai_OnDie")
aux.RegisterCreatureEvent(3030016, 4, "xiaoguai_OnDie")
aux.RegisterCreatureEvent(3030017, 4, "xiaoguai_OnDie")

function c3030027_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 3030028, x+5, y, z+5)
	map.MapCreateCreature(MapID, InstanceID, 3030028, x+5, y, z-5)
	map.MapCreateCreature(MapID, InstanceID, 3030028, x-5, y, z+5)
	map.MapCreateCreature(MapID, InstanceID, 3030028, x-5, y, z+5)
	map.MapCreateCreature(MapID, InstanceID, 3030028, x-3, y, z+7)
	map.MapCreateCreature(MapID, InstanceID, 3030028, x-7, y, z+3)


end
aux.RegisterCreatureEvent(3030027, 4, "c3030027_OnDie")