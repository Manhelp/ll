----scriptdata[1]��¼3040018�Ƿ�����
----scriptdata[2]��¼3040019�Ƿ�����
----scriptdata[3]��¼�Ƿ�ˢ��4������
----scriptdata[4]��¼3040020�Ƿ���������������
----scriptdata[5]��¼3040021�Ƿ���������������
----scriptdata[6]��¼3040024�Ƿ�����
----scriptdata[7]��¼3040025�Ƿ�����
----scriptdata[8]��Ǳ���
----scriptdata[9]��ǽ������Ƿ�ˢ��

function c3040024_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10030)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040024, 2, "c3040024_OnEnterCombat")


function c3040024_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	cre.MonsterSay(MapID, InstanceID, TargetID, 10032)
	map.SetMapScriptData(MapID, InstanceID, 1, 6, 1)
	local a = map.GetMapScriptData(MapID, InstanceID, 1, 7)
	if a==1 then
		-------------------------addmsgˢ��3040023
		map.MapCreateCreature(MapID, InstanceID, 3040023, 853, 1381, 292)
		map.MapCreateCreature(MapID, InstanceID, 3040026, 843, 1381, 282)
		map.MapCreateCreature(MapID, InstanceID, 3040026, 843, 1381, 302)
		map.MapCreateCreature(MapID, InstanceID, 3040026, 863, 1381, 282)
		map.MapCreateCreature(MapID, InstanceID, 3040026, 863, 1381, 302)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100049)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	end	
	
end

aux.RegisterCreatureEvent(3040024, 4, "c3040024_OnDie")