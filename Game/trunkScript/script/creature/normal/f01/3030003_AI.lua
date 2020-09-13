---mapscriptdata[1] ��¼���������Ƿ���ˢ������
--[2]��¼��һ�Ƿ���ɱ��

function c3030003_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10050)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100080)    
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	--���ţ��Ź�
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
	
	map.OpenCloseDoor(MapID, InstanceID, 1110, nil)
end

aux.RegisterCreatureEvent(3030003, 2, "c3030003_OnEnterCombat")

---����ս������

function c3030003_OnLeaveCombat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 1110, 1)
end
aux.RegisterCreatureEvent(3030003, 3, "c3030003_OnLeaveCombat")


---����ˢ���϶�
function c3030003_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	map.OpenCloseDoor(MapID, InstanceID, 1110, 1)
	map.OpenCloseDoor(MapID, InstanceID, 1111, 1)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100082)    
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10051)
	map.MapCreateCreature(MapID, InstanceID, 3030007, 446, 702, 243)
	map.SetMapScriptData(MapID,InstanceID,1,2,1)


end

aux.RegisterCreatureEvent(3030003, 4, "c3030003_OnDie")



