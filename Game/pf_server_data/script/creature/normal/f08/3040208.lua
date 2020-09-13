---mapscriptdata[0] ��¼���������Ƿ���ˢ������
---mapscriptdata[1] ��¼��һ��boss��id

---mapscriptdata[2] ��¼�ڼ�������
---mapscriptdata[3] ��¼boss1�Ƿ�ɱ��

function c3040208_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10112)
	--�����������ħ����ǿ��ĺڰ�ħ��
	map.OpenCloseDoor(MapID, InstanceID, 872, nil)

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

end

aux.RegisterCreatureEvent(3040208, 2, "c3040208_OnEnterCombat")


function c3040208_On_Leave_Combat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 872, 1)

end

aux.RegisterCreatureEvent(3040208, 3, "c3040208_On_Leave_Combat")


function s5054508_Cast(MapID, InstanceID, SkillID, OwnerID)
	if MapID==1266370126 then
		cre.MonsterSay(MapID, InstanceID,OwnerID, 10113)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100123)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		local wave = map.GetMapScriptData(MapID,InstanceID,1,2)
		if wave == 0 then
			map.MapCreateCreature(MapID, InstanceID, 3040239, 848, 63, 332)
			map.MapCreateCreature(MapID, InstanceID, 3040240, 857, 63, 335)
			map.MapCreateCreature(MapID, InstanceID, 3040239, 866, 63, 331)
			map.MapCreateCreature(MapID, InstanceID, 3040240, 867, 63, 321)
			map.MapCreateCreature(MapID, InstanceID, 3040239, 857, 63, 317)
			map.MapCreateCreature(MapID, InstanceID, 3040240, 849, 63, 321)
			map.MapCreateCreature(MapID, InstanceID, 3040245, 857, 63, 326)

			map.SetMapScriptData(MapID,InstanceID,1,2,wave+1)

		elseif wave == 1 then
			map.MapCreateCreature(MapID, InstanceID, 3040241, 848, 63, 332)
			map.MapCreateCreature(MapID, InstanceID, 3040242, 857, 63, 335)
			map.MapCreateCreature(MapID, InstanceID, 3040241, 866, 63, 331)
			map.MapCreateCreature(MapID, InstanceID, 3040242, 867, 63, 321)
			map.MapCreateCreature(MapID, InstanceID, 3040241, 857, 63, 317)
			map.MapCreateCreature(MapID, InstanceID, 3040242, 849, 63, 321)
			map.MapCreateCreature(MapID, InstanceID, 3040245, 857, 63, 326)


			map.SetMapScriptData(MapID,InstanceID,1,2,wave+1)
		else
			map.MapCreateCreature(MapID, InstanceID, 3040243, 848, 63, 332)
			map.MapCreateCreature(MapID, InstanceID, 3040244, 857, 63, 335)
			map.MapCreateCreature(MapID, InstanceID, 3040243, 866, 63, 331)
			map.MapCreateCreature(MapID, InstanceID, 3040244, 867, 63, 321)
			map.MapCreateCreature(MapID, InstanceID, 3040243, 857, 63, 317)
			map.MapCreateCreature(MapID, InstanceID, 3040244, 849, 63, 321)
			map.MapCreateCreature(MapID, InstanceID, 3040245, 857, 63, 326)


		end

	end
end
aux.RegisterSkillEvent(5054508, 1, "s5054508_Cast")









function c3040208_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10029)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100124)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	map.MapCreateCreature(MapID, InstanceID, 3040216, 856, 447, 677)

	map.SetMapScriptData(MapID,InstanceID,1,3,1)

	map.OpenCloseDoor(MapID, InstanceID, 875, 1)
	map.OpenCloseDoor(MapID, InstanceID, 874, 1)
	map.OpenCloseDoor(MapID, InstanceID, 872, 1)



	--map.OpenCloseDoor(MapID, InstanceID, 879, 1)


end
aux.RegisterCreatureEvent(3040208, 4, "c3040208_OnDie")
