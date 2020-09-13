----scriptdata[1]��¼3040018�Ƿ�����
----scriptdata[2]��¼3040019�Ƿ�����
----scriptdata[3]��¼�Ƿ�ˢ��4������
----scriptdata[4]��¼3040020�Ƿ���������������
----scriptdata[5]��¼3040021�Ƿ���������������
----scriptdata[6]��¼3040024�Ƿ�����
----scriptdata[7]��¼3040025�Ƿ�����
----scriptdata[8]��Ǳ���
----scriptdata[9]��ǽ������Ƿ�ˢ��

function c3040022_OnEnterCombat(MapID, InstanceID, CreatureID)


	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040022, 2, "c3040022_OnEnterCombat")

function c3040022_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10035)

	local baoxiang = map.GetMapScriptData(MapID, InstanceID, 1,8)
	if baoxiang ==0 then

		map.MapCreateCreature(MapID, InstanceID, 6010045, 1073, 1508, 245)
		map.MapCreateCreature(MapID, InstanceID, 5010234, 1083, 1508, 255)

		--map.MapCreateCreature(MapID, InstanceID, 6010030, 1073, 1508, 245)
		--map.MapCreateCreature(MapID, InstanceID, 6010030, 696, 2621, 953)
		--map.MapCreateCreature(MapID, InstanceID, 6010030, 676, 2621, 943)

		map.SetMapScriptData(MapID, InstanceID, 1,8,1)
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100050)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	if role.GetTrialState(RoleID, 4012) == 1 then
		role.SetTrialCompleteNum(RoleID, 4012, role.GetTrialCompleteNum(RoleID,4012) + 1)
    end
	if role.GetTrialState(RoleID, 4012) == 1 and role.GetTrialCompleteNum(RoleID,4012) >= Trail_maxnum[4012] then
		role.TrialComplete(RoleID,4012)
	end
end

aux.RegisterCreatureEvent(3040022, 4, "c3040022_OnDie")
