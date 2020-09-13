---mapscriptdata[1] ��¼���������Ƿ���ˢ������
----[2]��¼3040008�Ƿ�ɱ��
----[3]��¼3040009�Ƿ�ɱ��
----[4]��¼�Ƿ��й���Χ��
----[5]��¼�Ƿ��������
----[6]��¼3040010�Ƿ�ɱ��

function s5051001_Cast(MapID, InstanceID, SkillID, OwnerID)
	if MapID==1266371662 then
		cre.MonsterSay(MapID, InstanceID, TargetID, 10028)
		local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
		map.MapCreateCreature(MapID, InstanceID, 3040031, x+10, y, z+10)
		map.MapCreateCreature(MapID, InstanceID, 3040032, x-10, y, z+10)
		map.MapCreateCreature(MapID, InstanceID, 3040033, x+10, y, z-10)
		map.MapCreateCreature(MapID, InstanceID, 3040034, x-10, y, z-10)
		local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    		--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   		 g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 100, 20)
	end
end
aux.RegisterSkillEvent(5051001, 1, "s5051001_Cast")


function c3040010_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	-----���ﺰ�� 3040011������
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100042)    
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10016)
	map.MapCreateCreature(MapID, InstanceID, 3040004, 393, 5862, 793)
	map.MapCreateCreature(MapID, InstanceID, 3040011, 419, 5913, 794)
	map.MapCreateCreature(MapID, InstanceID, 3040004, 420, 5862, 767)
	map.MapCreateCreature(MapID, InstanceID, 3040004, 446, 5862, 794)
	map.MapCreateCreature(MapID, InstanceID, 3040004, 419, 5862, 820)
	
	map.MapCreateCreature(MapID, InstanceID, 3040006, 393, 5793, 768)
	map.MapCreateCreature(MapID, InstanceID, 3040006, 443, 5793, 766)
	map.MapCreateCreature(MapID, InstanceID, 3040006, 446, 5793, 820)
	map.MapCreateCreature(MapID, InstanceID, 3040006, 393, 5793, 820)
	map.SetMapScriptData(MapID, InstanceID, 1, 6, 1)

end

aux.RegisterCreatureEvent(3040010, 4, "c3040010_OnDie")