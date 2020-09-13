---mapscriptdata[0] ��¼���������Ƿ���ˢ������
---mapscriptdata[1] ��¼��һ��boss��id

---mapscriptdata[2] ��¼�ڼ�������
---mapscriptdata[3] ��¼boss1�Ƿ�ɱ��

---mapscriptdata[4] ��¼boss2ˢ���ڼ���С��

function c3040216_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10114)
	--ħ��ֻҪ�����ڣ�������������
	map.OpenCloseDoor(MapID, InstanceID, 874, nil)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100125)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)


	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

end

aux.RegisterCreatureEvent(3040216, 2, "c3040216_OnEnterCombat")


function s5934701_Cast(MapID, InstanceID, SkillID, OwnerID)
	if MapID==1266370126 then
		cre.MonsterSay(MapID, InstanceID,OwnerID, 10115)

		local wave = map.GetMapScriptData(MapID,InstanceID,1,4)

		if wave < 6 then
			local x,y,z = unit.GetPosition(MapID, InstanceID,OwnerID)

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100126)
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

			map.MapCreateCreature(MapID, InstanceID, 3040246, x+5, y, z+5)
			map.MapCreateCreature(MapID, InstanceID, 3040246,  x-5, y, z+5)
			map.MapCreateCreature(MapID, InstanceID, 3040246,  x+5, y, z-5)
			map.MapCreateCreature(MapID, InstanceID, 3040246,  x-5, y, z-5)
			map.MapCreateCreature(MapID, InstanceID, 3040246,  x+7, y, z)
			map.MapCreateCreature(MapID, InstanceID, 3040246,  x, y, z+7)

			map.SetMapScriptData(MapID,InstanceID,1,4,wave+1)

		end

	end
end
aux.RegisterSkillEvent(5934701, 1, "s5934701_Cast")









function c3040216_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10116)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100127)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	map.MapCreateCreature(MapID, InstanceID, 5011013, 856, 447, 677)
	map.OpenCloseDoor(MapID, InstanceID, 874, 1)




end
aux.RegisterCreatureEvent(3040216, 4, "c3040216_OnDie")



function c3040216_On_Leave_Combat(MapID, InstanceID, CreatureID)
	unit.CancelBuff(MapID, InstanceID, CreatureID, 5934701)
	map.OpenCloseDoor(MapID, InstanceID, 874, 1)
end

aux.RegisterCreatureEvent(3040216, 3, "c3040216_On_Leave_Combat")
