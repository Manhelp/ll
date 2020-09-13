---mapscriptdata[1] ��¼���������Ƿ���ˢ������
--[2]��¼��һ�Ƿ���ɱ��
--[3]��¼�϶��Ƿ���ɱ��
--[4]��¼�����Ƿ���ɱ��
--[5]��¼�����Ƿ���ɱ��
--[6]��¼�Ƿ�ˢ������
--[11]��¼�Ƿ�ˢ��30������
--[12]��¼�Ƿ�ˢ��30������

function s5055702_Cast(MapID, InstanceID, SkillID, OwnerID)

	if MapID==1266372430 then
		cre.MonsterSay(MapID, InstanceID, OwnerID, 10056)
		local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
		local position={{x=292,y=1,z=71},{x=311,y=1,z=62},{x=328,y=1,z=76},{x=312,y=1,z=99},{x=342,y=1,z=69}}
		
		local positionID=math.random(1,5)

		
		if TargetID~=-1 then

			
			role.RoleGotoNewMap(MapID, InstanceID, TargetID, MapID,  position[positionID].x, position[positionID].y, position[positionID].z)
			map.MapCreateCreature(MapID, InstanceID, (3030023+math.random(1,2)), position[positionID].x+2, position[positionID].y, position[positionID].z+2)
			unit.AddBuff(MapID, InstanceID, TargetID,5055702,OwnerID)
			unit.AddBuff(MapID, InstanceID, TargetID,5055603,OwnerID)  
		end
	end

end
aux.RegisterSkillEvent(5055702, 1, "s5055702_Cast")



function c3030023_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10050)
	
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
	map.OpenCloseDoor(MapID, InstanceID, 1110, nil)
	
end

aux.RegisterCreatureEvent(3030023, 2, "c3030023_OnEnterCombat")

function c3030023_OnLeaveCombat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 1110, 1)
end
aux.RegisterCreatureEvent(3030023, 3, "c3030023_OnLeaveCombat")


---����ˢ������
function c3030023_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	
	--cre.MonsterSay(MapID, InstanceID, TargetID, 10051)
	map.OpenCloseDoor(MapID, InstanceID, 1110, 1)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100100)    
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)


	local a = map.GetMapScriptData(MapID,InstanceID,1,12)
	if a == 0 then	
		map.MapCreateCreature(MapID, InstanceID, 6010049, 338, 50, 56)
		map.MapCreateCreature(MapID, InstanceID, 5010232, 348, 50, 66)
		map.SetMapScriptData(MapID,InstanceID,1,12,1)
	end

	

end

aux.RegisterCreatureEvent(3030023, 4, "c3030023_OnDie")



