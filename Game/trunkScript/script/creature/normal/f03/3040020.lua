----scriptdata[1]��¼3040018�Ƿ�����
----scriptdata[2]��¼3040019�Ƿ�����
----scriptdata[3]��¼�Ƿ�ˢ��4������
----scriptdata[4]��¼3040020�Ƿ���������������
----scriptdata[5]��¼3040021�Ƿ���������������
----scriptdata[6]��¼3040024�Ƿ�����
----scriptdata[7]��¼3040025�Ƿ�����
----scriptdata[8]��Ǳ���
----scriptdata[9]��ǽ������Ƿ�ˢ��


function c3040020_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID, InstanceID, 1, 4)
	if a==0 then
		-------------------------addmsg����һ������Ҫɱ

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100051)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		map.SetMapScriptData(MapID, InstanceID, 1, 4, 1)
	elseif a==1 then
		-------------------------����ɱ��2�δӶ�ˢ������
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100047)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		map.SetMapScriptData(MapID, InstanceID, 1, 4, 2)
		map.MapCreateCreature(MapID, InstanceID, 3040024, 926, 1820, 255)
	end	
end

aux.RegisterCreatureEvent(3040020, 4, "c3040020_OnDie")