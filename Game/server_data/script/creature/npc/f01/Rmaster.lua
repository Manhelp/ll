---mapscriptdata[1] ��¼���������Ƿ���ˢ������
--[2]��¼��һ�Ƿ���ɱ��
--[3]��¼�϶��Ƿ���ɱ��
--[4]��¼�����Ƿ���ɱ��
--[5]��¼�����Ƿ���ɱ��
--[6]��¼�Ƿ�ˢ������
--[11]��¼�Ƿ�ˢ��30������

function aodeli_OnTalk(MapID, InstanceID, NPCID, NPCTypeID, TargetID, Index)
		if Index == -1 then
		  local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 	500002)	
			msg.AddMsgEvent(MsgID, 24,	NPCID)	--npcid
		    	msg.AddMsgEvent(MsgID, 21,	4)		--ȷ����ť
			msg.AddMsgEvent(MsgID, 1,100001)	--��ȷ����
			
			msg.AddMsgEvent(MsgID, 21,	6)		--�س�ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	500003)
			msg.DispatchRoleMsgEvent(TargetID, MsgID)
		end
		if Index == 4 then
			local a = map.GetMapScriptData(MapID,InstanceID,1,11)
			if a == 0 then	
				map.MapDeleteCreature(MapID, InstanceID, NPCID)
				map.MapCreateCreature(MapID, InstanceID, 3030020, 141, 61, 243)
		
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100099)
			
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

				map.MapCreateCreature(MapID, InstanceID, 3030027, 119, 61, 212)
				map.MapCreateCreature(MapID, InstanceID, 3030027, 140, 61, 207)
				map.MapCreateCreature(MapID, InstanceID, 3030027, 163, 61, 214)
				map.MapCreateCreature(MapID, InstanceID, 3030027, 176, 61, 233)
				map.MapCreateCreature(MapID, InstanceID, 3030027, 176, 61, 255)
				map.MapCreateCreature(MapID, InstanceID, 3030027, 162, 61, 274)
				map.MapCreateCreature(MapID, InstanceID, 3030027, 118, 61, 272)

				map.SetMapScriptData(MapID,InstanceID,1,11,1)
			end

		elseif Index == 6 then

			role.RoleGotoNewMap(MapID, InstanceID,TargetID,3424071976,1272,20830,1329)


		end
	
end

aux.RegisterCreatureEvent(5510636, 7, "aodeli_OnTalk")




