---mapscriptdata[1] ��¼���������Ƿ���ˢ������
--[2]��¼��һ�Ƿ���ɱ��
--[3]��¼�϶��Ƿ���ɱ��
--[4]��¼�����Ƿ���ɱ��
--[5]��¼�����Ƿ���ɱ��

function c3030013_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10061)
	--local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 102, 100081)
	--msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	--���ţ��Ź�
	--map.OpenCloseDoor(MapID, InstanceID, 1119, nil)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)


end

aux.RegisterCreatureEvent(3030013, 2, "c3030013_OnEnterCombat")

function c3030013_OnLeaveCombat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 1119, 1)
end
aux.RegisterCreatureEvent(3030013, 3, "c3030013_OnLeaveCombat")


---����ˢ������
function c3030013_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	map.OpenCloseDoor(MapID, InstanceID, 1119, 1)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10062)
	map.MapCreateCreature(MapID, InstanceID, 3030018, 141, 61, 243)

	map.SetMapScriptData(MapID,InstanceID,1,4,1)


end

aux.RegisterCreatureEvent(3030013, 4, "c3030013_OnDie")



