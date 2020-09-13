---mapscriptdata[1] ��¼���������Ƿ���ˢ������
----[2]��¼3040008�Ƿ�ɱ��
----[3]��¼3040009�Ƿ�ɱ��
----[4]��¼�Ƿ��й���Χ��
----[5]��¼�Ƿ��������
----[6]��¼3040010�Ƿ�ɱ��
----[7]��¼�����Ƿ�ֻ�ܱ�ˢ��һ��


function c3040011_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10017)

	map.MapCreateCreature(MapID, InstanceID, 3040006, 338, 5433, 783)
	map.MapCreateCreature(MapID, InstanceID, 3040006, 338, 5433, 788)
	map.MapCreateCreature(MapID, InstanceID, 3040006, 338, 5433, 793)
	map.MapCreateCreature(MapID, InstanceID, 3040006, 338, 5433, 798)



	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ100�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 150, 40)
end

aux.RegisterCreatureEvent(3040011, 2, "c3040011_OnEnterCombat")




function c3040011_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100043)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	local baoxiang = map.GetMapScriptData(MapID, InstanceID, 1,7)
	if baoxiang ==0 then

		map.MapCreateCreature(MapID, InstanceID, 6010044, 419, 5913, 794)
		map.MapCreateCreature(MapID, InstanceID, 5010233, 431, 5877, 806)
		map.SetMapScriptData(MapID, InstanceID, 1,7,1)
	end

	cre.MonsterSay(MapID, InstanceID, TargetID, 10018)

	if role.GetTrialState(RoleID, 3010) == 1 then
		role.SetTrialCompleteNum(RoleID, 3010, role.GetTrialCompleteNum(RoleID,3010) + 1)
    end
	if role.GetTrialState(RoleID, 3010) == 1 and role.GetTrialCompleteNum(RoleID,3010) >= Trail_maxnum[3010] then
		role.TrialComplete(RoleID,3010)
	end

end

aux.RegisterCreatureEvent(3040011, 4, "c3040011_OnDie")



