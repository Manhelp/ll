----scriptdata[1]��¼3040018�Ƿ�����
----scriptdata[2]��¼3040019�Ƿ�����
----scriptdata[3]��¼�Ƿ�ˢ��4������
----scriptdata[4]��¼3040020�Ƿ���������������
----scriptdata[5]��¼3040021�Ƿ���������������
----scriptdata[6]��¼3040024�Ƿ�����
----scriptdata[7]��¼3040025�Ƿ�����
----scriptdata[8]��Ǳ���
----scriptdata[9]��ǽ������Ƿ�ˢ��


function s5054502_Cast(MapID, InstanceID, SkillID, OwnerID)
	if MapID==1266371918 then
		cre.MonsterSay(MapID, InstanceID, OwnerID, 10038)
		local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
		map.MapCreateCreature(MapID, InstanceID, 3040029, x+10, y, z+10)
		map.MapCreateCreature(MapID, InstanceID, 3040029, x-10, y, z+10)
		map.MapCreateCreature(MapID, InstanceID, 3040030, x+10, y, z-10)
		map.MapCreateCreature(MapID, InstanceID, 3040030, x-10, y, z-10)
		local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    		--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   		g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 100, 20)
	end
end
aux.RegisterSkillEvent(5054502, 1, "s5054502_Cast")


function c3040021_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID, InstanceID, 1, 5)
	if a==0 then
		-------------------------addmsg����һ������Ҫɱ
		map.SetMapScriptData(MapID, InstanceID, 1, 5, 1)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100051)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
		
	elseif a==1 then
		-------------------------����ɱ��2�δӶ�ˢ������
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100048)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		map.SetMapScriptData(MapID, InstanceID, 1, 5, 2)
		map.MapCreateCreature(MapID, InstanceID, 3040025, 955, 1889, 372)
	end	
end

aux.RegisterCreatureEvent(3040021, 4, "c3040021_OnDie")