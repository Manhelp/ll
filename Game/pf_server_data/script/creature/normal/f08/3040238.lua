---mapscriptdata[0] ��¼���������Ƿ���ˢ������
---mapscriptdata[1] ��¼��һ��boss��id

---mapscriptdata[2] ��¼�ڼ�������
---mapscriptdata[3] ��¼boss1�Ƿ�ɱ��

function c3040238_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10117)
	--���������ע���Ǹ�����

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

end

aux.RegisterCreatureEvent(3040238, 2, "c3040238_OnEnterCombat")

function c3040238_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	--cre.MonsterSay(MapID, InstanceID, TargetID, 10029)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100128)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	
	map.MapCreateCreature(MapID, InstanceID, 3040247, 439, 1327, 370)
	
	

	
	

end
aux.RegisterCreatureEvent(3040238, 4, "c3040238_OnDie")
