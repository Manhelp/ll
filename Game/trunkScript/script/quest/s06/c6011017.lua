--�㼣3
function c6011017_OnInvest(MapID, InstanceID, QuestID, RoleID, CreatureTypeID)
	
	if QuestID == 50034 then 
		local Rate = math.random(1, 100)
			if Rate <= 15 then  --15%���� 
				local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
					--if x == 1216.28 and z == 2090.48  then  --��ɽ�ַ�Χ
					if x > 1206 and x < 1226 and z > 2080 and z < 2100 then
					map.MapCreateCreature(MapID, InstanceID, 1080056, x, y, z)--1080056	��������
					--elseif x == 2610.72 and z == 1174.5  then  --�����ط�Χ
					elseif x > 2600 and x < 2620 and z > 1164 and z < 1184  then
					map.MapCreateCreature(MapID, InstanceID, 1080058, x+2, y, z)--1080058	�������
					--elseif x == 2638.52 and z == 2020.4 then  --̫ƽ�·�Χ
					elseif x > 2628 and x <2658 and z > 2010 and z < 2030 then
					map.MapCreateCreature(MapID, InstanceID, 1080058, x, y, z)--1080058	�������
					end
			elseif Rate > 15 and Rate <= 66 then--15%���㼣
				role.AddRoleItem(MapID, InstanceID, RoleID, 4030003, 1, -1, 8, 420)
			elseif Rate > 66 and Rate <= 70 then--10%���װ��
				role.AddRoleItem(MapID, InstanceID, RoleID, 3110013, 1, -1, 8, 420)
			elseif Rate > 70 and Rate <=90 then--40%�����������ͽ�Ǯ
				role.AddRoleExp(MapID, InstanceID, RoleID, 4100)
				role.AddRoleSilver(MapID, InstanceID, RoleID, 1740)
			elseif Rate > 90 and Rate <=100 then--20%���DEBUFF
				unit.AddBuff(MapID, InstanceID, RoleID, 9003501, RoleID )
				local QuestID1 = msg.BeginMsgEvent()
				msg.AddMsgEvent(QuestID1, 13, 0)
				msg.AddMsgEvent(QuestID1, 1, 404004)
				msg.DispatchRoleMsgEvent(RoleID, QuestID1)
			end
	else
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 404002)
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		
	end
	
end

--ע��
aux.RegisterQuestEvent(50034, 9, "c6011017_OnInvest")
