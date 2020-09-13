---mapscriptdata[0] ��¼���������Ƿ���ˢ������
---mapscriptdata[1] ��¼��һ��boss��id

---mapscriptdata[2] ��¼�ڼ�������
---mapscriptdata[3] ��¼boss1�Ƿ�ɱ��

function c3040249_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10121)
	
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100129)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

end

aux.RegisterCreatureEvent(3040249, 2, "c3040249_OnEnterCombat")

function c3040249_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	--cre.MonsterSay(MapID, InstanceID, TargetID, 10029)
	
	map.MapCreateCreature(MapID, InstanceID, 3040228, 222, 1151, 636)
	map.MapCreateCreature(MapID, InstanceID, 3040232, 462, 1151, 688)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100130)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	map.OpenCloseDoor(MapID, InstanceID, 877, 1)
	map.OpenCloseDoor(MapID, InstanceID, 880, 1)
	map.OpenCloseDoor(MapID, InstanceID, 881, 1)
	map.OpenCloseDoor(MapID, InstanceID, 882, 1)
	map.OpenCloseDoor(MapID, InstanceID, 883, 1)
	

end
aux.RegisterCreatureEvent(3040249, 4, "c3040249_OnDie")
