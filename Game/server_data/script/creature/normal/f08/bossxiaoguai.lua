---mapscriptdata[0] ��¼���������Ƿ���ˢ������
---mapscriptdata[1] ��¼��һ��boss��id

---mapscriptdata[2] ��¼�ڼ�������
---mapscriptdata[3] ��¼boss1�Ƿ�ɱ��




function bossxiaoguai_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	--cre.MonsterSay(MapID, InstanceID, TargetID, 10029)
	---�ҵ���������ڰ�Ů��������ǿ�������

	local death = map.GetMapScriptData(MapID,InstanceID,1,3)
	local bossid = map.GetMapScriptData(MapID,InstanceID,1,1)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100122)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	
	if death == 0 then
		if TargetTypeID == 3040239 or TargetTypeID == 3040240    then
			unit.AddBuff(MapID, InstanceID,bossid,5935701,bossid)
		elseif TargetTypeID == 3040241 or TargetTypeID == 3040242     then
			unit.AddBuff(MapID, InstanceID,bossid,5935801,bossid)
		elseif TargetTypeID == 3040243 or TargetTypeID == 3040244     then
			unit.AddBuff(MapID, InstanceID,bossid,5935901,bossid)
		elseif TargetTypeID == 3040245      then
			unit.AddBuff(MapID, InstanceID,bossid,5936001,bossid)
		end
	end

end
aux.RegisterCreatureEvent(3040239, 4, "bossxiaoguai_OnDie")
aux.RegisterCreatureEvent(3040240, 4, "bossxiaoguai_OnDie")
aux.RegisterCreatureEvent(3040241, 4, "bossxiaoguai_OnDie")
aux.RegisterCreatureEvent(3040242, 4, "bossxiaoguai_OnDie")
aux.RegisterCreatureEvent(3040243, 4, "bossxiaoguai_OnDie")
aux.RegisterCreatureEvent(3040244, 4, "bossxiaoguai_OnDie")
aux.RegisterCreatureEvent(3040245, 4, "bossxiaoguai_OnDie")

