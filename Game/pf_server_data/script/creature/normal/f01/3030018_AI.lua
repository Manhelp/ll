---mapscriptdata[1] ��¼���������Ƿ���ˢ������
--[2]��¼��һ�Ƿ���ɱ��
--[3]��¼�϶��Ƿ���ɱ��
--[4]��¼�����Ƿ���ɱ��
--[5]��¼�����Ƿ���ɱ��
--[6]��¼�Ƿ�ˢ������


function s5055701_Cast(MapID, InstanceID, SkillID, OwnerID)

	if MapID==1266372430 then
		cre.MonsterSay(MapID, InstanceID, OwnerID, 10056)
		local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
		local position={{x=118,y=81,z=274},{x=100,y=81,z=199},{x=140,y=81,z=204},{x=163,y=81,z=267},{x=142,y=81,z=245}}

		local positionID=math.random(1,5)


		if TargetID~=-1 then


			role.RoleGotoNewMap(MapID, InstanceID, TargetID, MapID,  position[positionID].x, position[positionID].y, position[positionID].z)

			--role.RoleGotoArena(MapID, InstanceID, TargetID, MapID, InstanceID, position[positionID].x, position[positionID].y, position[positionID].z)
			unit.AddBuff(MapID, InstanceID, TargetID,5055701,OwnerID)
		end
	end

end
aux.RegisterSkillEvent(5055701, 1, "s5055701_Cast")




function c3030018_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10054)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3030018, 2, "c3030018_OnEnterCombat")

function c3030018_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	cre.MonsterSay(MapID, InstanceID, TargetID, 10055)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100079)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	if role.GetTrialState(RoleID, 2007) == 1 then
		role.SetTrialCompleteNum(RoleID, 2007, role.GetTrialCompleteNum(RoleID,2007) + 1)
    end
	if role.GetTrialState(RoleID, 2007) == 1 and role.GetTrialCompleteNum(RoleID,2007) >= Trail_maxnum[2007] then
		role.TrialComplete(RoleID,2007)
	end

	map.SetMapScriptData(MapID, InstanceID, 1, 5, 1)
	local a = map.GetMapScriptData(MapID, InstanceID, 1, 6)
	if a==0 then
		-------------------------ˢ��6010043
		map.MapCreateCreature(MapID, InstanceID, 6010043, 141, 61, 243)

		map.MapCreateCreature(MapID, InstanceID, 5510636, 151, 61, 253)

		map.SetMapScriptData(MapID, InstanceID, 1, 6, 1)
	end

	local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)

	for u,v in pairs(RoleTbl) do
		if u ~= nil and role.GetBagFreeSize(v) > 0 then
			if role.GetRoleItemNum(v, 4800034) < 1 and role.IsRoleHaveQuest(MapID, InstanceID, v, 30789) then
				role.AddRoleItem(MapID, InstanceID, v, 4800034, 1, 0, 8, 420)
			end
		end
	end

end

aux.RegisterCreatureEvent(3030018, 4, "c3030018_OnDie")
