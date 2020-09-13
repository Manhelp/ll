---mapscriptdata[0] ��¼���������Ƿ���ˢ������
---mapscriptdata[1] ��¼��һ��boss��id

---mapscriptdata[2] ��¼�ڼ�������
---mapscriptdata[3] ��¼boss1�Ƿ�ɱ��
---mapscriptdata[11] --��¼�����Ƿ�����
---mapscriptdata[12] --��¼�����Ƿ�����
---mapscriptdata[13] --��¼���boss�Ƿ�����
---mapscriptdata[14] --��¼ڤ��boss�Ƿ�����
function c3040251_OnEnterCombat(MapID, InstanceID, CreatureID)


	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

end

aux.RegisterCreatureEvent(3040251, 2, "c3040251_OnEnterCombat")

function c3040251_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10029)
	local temp = map.GetMapScriptData(MapID,InstanceID,1,13)
	if(temp == 1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100175)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
		local baozang = map.MapCreateCreature(MapID, InstanceID, 6011031, 399, 1151, 903)
		map.MapCreateCreature(MapID, InstanceID, 6011032, 399, 1151, 894)
		map.OpenCloseDoor(MapID, InstanceID, 889, 1)
		map.OpenCloseDoor(MapID, InstanceID, 890, 1)
		table.insert(tianjiebaozang, baozang)
	else
		local tianjieBOss = map.GetMapScriptData(MapID,InstanceID,1,23)
		unit.CancelBuff(MapID, InstanceID, tianjieBOss, 5942501)
		unit.AddBuff(MapID, InstanceID, tianjieBOss,5942301,tianjieBOss)
	end
	map.SetMapScriptData(MapID,InstanceID,1,14,1)
	map.SetMapScriptData(MapID,InstanceID,1,24,0)

end
aux.RegisterCreatureEvent(3040251, 4, "c3040251_OnDie")



function c3040251_On_Leave_Combat(MapID, InstanceID, CreatureID)
	local temp = map.GetMapScriptData(MapID,InstanceID,1,13)
	if(temp == 1) then
		local tianjieBOss = map.MapCreateCreature(MapID, InstanceID, 3040250, 349, 1151, 855)
		map.SetMapScriptData(MapID,InstanceID,1,23,tianjieBOss)
		map.SetMapScriptData(MapID,InstanceID,1,13,0)
		unit.AddBuff(MapID, InstanceID, tianjieBOss,5942501,tianjieBOss)
	end
	local mingjieBOss = map.GetMapScriptData(MapID,InstanceID,1,24)
	unit.AddBuff(MapID, InstanceID, mingjieBOss,5942601,mingjieBOss)
	unit.CancelBuff(MapID, InstanceID, mingjieBOss, 5942301)

end

aux.RegisterCreatureEvent(3040251, 3, "c3040251_On_Leave_Combat")
