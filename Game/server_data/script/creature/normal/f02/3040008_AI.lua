---mapscriptdata[1] ��¼���������Ƿ���ˢ������
----[2]��¼3040008�Ƿ�ɱ��
----[3]��¼3040009�Ƿ�ɱ��
----[4]��¼�Ƿ��й���Χ��
----[5]��¼�Ƿ��������
----[6]��¼3040010�Ƿ�ɱ����


function c3040008_OnEnterCombat(MapID, InstanceID, CreatureID)


	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
------------------------
	cre.MonsterSay(MapID, InstanceID, CreatureID, 10006)
	local baowei=map.GetMapScriptData(MapID, InstanceID, 1,4)
 

	if baowei==0 then
		map.MapCreateCreature(MapID, InstanceID, 3040002, x+10, y, z+10)
		map.MapCreateCreature(MapID, InstanceID, 3040007, x-10, y, z+10)
		map.MapCreateCreature(MapID, InstanceID, 3040002, x+10, y, z-10)
		map.MapCreateCreature(MapID, InstanceID, 3040006, x-10, y, z-10)
		map.SetMapScriptData(MapID,InstanceID,1,4,1)
	end	

	local TargetID2 = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID2, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040008, 2, "c3040008_OnEnterCombat")



function c3040008_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local b = map.GetMapScriptData(MapID, InstanceID, 1, 3)
	map.SetMapScriptData(MapID, InstanceID, 1, 2, 1)
	
	if b==1 then
		-----3040009����
		cre.MonsterSay(MapID, InstanceID, TargetID, 10008)
		map.MapCreateCreature(MapID, InstanceID, 3040010, 577, 4295, 147)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100041)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	else
		-----3040009δ��
		cre.MonsterSay(MapID, InstanceID, TargetID, 10007)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100039)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	end
	local x,y,z=784,6703,790
	map.MapCreateCreature(MapID, InstanceID, 3040007, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 3040006, x+7, y, z-5)
	map.MapCreateCreature(MapID, InstanceID, 3040004, x-7, y, z-5)



end

aux.RegisterCreatureEvent(3040008, 4, "c3040008_OnDie")



