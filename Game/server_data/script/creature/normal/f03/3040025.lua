----scriptdata[1]��¼3040018�Ƿ�����
----scriptdata[2]��¼3040019�Ƿ�����
----scriptdata[3]��¼�Ƿ�ˢ��4������
----scriptdata[4]��¼3040020�Ƿ���������������
----scriptdata[5]��¼3040021�Ƿ���������������
----scriptdata[6]��¼3040024�Ƿ�����
----scriptdata[7]��¼3040025�Ƿ�����
----scriptdata[8]��Ǳ���
----scriptdata[9]��ǽ������Ƿ�ˢ��


function s5054501_Cast(MapID, InstanceID, SkillID, OwnerID)
	if MapID==1266371918 then
		cre.MonsterSay(MapID, InstanceID, OwnerID, 10037)
		local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
		map.MapCreateCreature(MapID, InstanceID, 3040027, x+10, y, z+10)
		map.MapCreateCreature(MapID, InstanceID, 3040027, x-10, y, z+10)
		map.MapCreateCreature(MapID, InstanceID, 3040028, x+10, y, z-10)
		map.MapCreateCreature(MapID, InstanceID, 3040028, x-10, y, z-10)
		local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    		--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   		 g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 100, 20)
	end
end
aux.RegisterSkillEvent(5054501, 1, "s5054501_Cast")




function c3040025_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10031)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040025, 2, "c3040025_OnEnterCombat")

function c3040025_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	cre.MonsterSay(MapID, InstanceID, TargetID, 10032)
	map.SetMapScriptData(MapID, InstanceID, 1, 7, 1)
	local a = map.GetMapScriptData(MapID, InstanceID, 1, 6)
	if a==1 then
		-------------------------addmsgˢ��3040023
		map.MapCreateCreature(MapID, InstanceID, 3040023, 853, 1381, 292)
		map.MapCreateCreature(MapID, InstanceID, 3040026, 843, 1381, 282)
		map.MapCreateCreature(MapID, InstanceID, 3040026, 843, 1381, 302)
		map.MapCreateCreature(MapID, InstanceID, 3040026, 863, 1381, 282)
		map.MapCreateCreature(MapID, InstanceID, 3040026, 863, 1381, 302)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100049)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	end	
	
end

aux.RegisterCreatureEvent(3040025, 4, "c3040025_OnDie")