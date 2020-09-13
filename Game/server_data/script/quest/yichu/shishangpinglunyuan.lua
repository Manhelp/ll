--ʱ������Ա
--��¼��ȡ���� RoleDataType["ShiShangPingLun"] = 72 ����λ������λ���죬��λ����
function c5610052On_Talk(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID, TalkIndex)
	local _, level1, level2, level3, suitSum = role.GetWardrobeDetailInfo(MapID,InstanceID,RoleID)
	local collected = math.floor((level1+level2+level3)/suitSum*100)
	local done = math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["ShiShangPingLun"])/10)
	local otable = {}
	otable[4] = 100
	otable[3] = 80
	otable[2] = 60
	otable[1] = 40
	otable[0] = 0
	for i=4,0,-1 do
		if collected >= otable[i] then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["ShiShangPingLun"], done*10+i)
			break
		end
	end
	local can = role.GetRoleScriptData(RoleID, 1, RoleDataType["ShiShangPingLun"])%10
	--
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444113) 			--
		msg.AddMsgEvent(MsgID, 21, 4) 				--��ť
		msg.AddMsgEvent(MsgID, 1, 444107) 			--����ʱװ�ռ��ɹ�
		msg.AddMsgEvent(MsgID, 21, 7) 				--��ť
		msg.AddMsgEvent(MsgID, 1, 444114) 			--��ѯʱװ״̬�������
		msg.AddMsgEvent(MsgID, 21, 5)   			--��ť
		msg.AddMsgEvent(MsgID, 1, 444112)   		--�뿪
		msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if role.GetBagFreeSize(RoleID) < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444102) 		--���ı����������޷���ý����������������
			msg.AddMsgEvent(MsgID, 21, 5)   		--��ť
			msg.AddMsgEvent(MsgID, 1, 444112)   	--ȡ��
			msg.AddMsgEvent(MsgID, 24, CreatureID) 	--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if can > 0 and done < 5 then
				local MsgID = msg.BeginMsgEvent()
				if done >= can then
					msg.AddMsgEvent(MsgID, 20, 444115) 			--�����ռ����Ѿ��ﵽxxx%����û�дﵽ��һ�׶ν�������ȡҪ��ע�����ռ��ʳ���40%��60%��80%���ߴﵽ100%��õ���ͬ�ĳƺŽ�����
					msg.AddMsgEvent(MsgID, 9, collected)
				else
					msg.AddMsgEvent(MsgID, 20, 444108) 			--�����ռ����Ѿ��ﵽxxx%��������ȡ��x�׶εĽ�����ע�����ռ��ʳ���40%��60%��80%���ߴﵽ100%��õ���ͬ�ĳƺŽ�����
					msg.AddMsgEvent(MsgID, 9, collected)
					msg.AddMsgEvent(MsgID, 9, done+1)
					msg.AddMsgEvent(MsgID, 21, 6) 				--��ť
					msg.AddMsgEvent(MsgID, 1, 444105) 			--��ȡ����
				end
				msg.AddMsgEvent(MsgID, 21, -1) 				--��ť
				msg.AddMsgEvent(MsgID, 1, 100002) 			--ȡ��
				msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif done > 4 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444109) 			--���Ѿ���ù����������
				msg.AddMsgEvent(MsgID, 21, 5)   			--��ť
				msg.AddMsgEvent(MsgID, 1, 444112)   		--�뿪
				msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444115) 			--�����ռ����Ѿ��ﵽxxx%����û�дﵽ��һ�׶ν�������ȡҪ��ע�����ռ��ʳ���40%��60%��80%���ߴﵽ100%��õ���ͬ�ĳƺŽ�����
				msg.AddMsgEvent(MsgID, 9, collected)
				msg.AddMsgEvent(MsgID, 9, done+1)
				msg.AddMsgEvent(MsgID, 21, 5) 				--��ť
				msg.AddMsgEvent(MsgID, 1, 444112) 			--ȡ��
				msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	elseif TalkIndex == 6 then
		if role.GetBagFreeSize(RoleID) < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444102) 		--���ı����������޷���ý����������������
			msg.AddMsgEvent(MsgID, 21, 5)   		--��ť
			msg.AddMsgEvent(MsgID, 1, 444112)   	--ȡ��
			msg.AddMsgEvent(MsgID, 24, CreatureID) 	--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local ptable = {}
			ptable[0] = {40, 3080034}
			ptable[1] = {60, 3080035}
			ptable[2] = {80, 3080036}
			ptable[3] = {100, 3080037}
			for i=0,3 do
				if done == i and collected >= ptable[i][1] then
					role.SetRoleScriptData(RoleID, 1, RoleDataType["ShiShangPingLun"], 10*(i+1))
					role.AddRoleItem(MapID, InstanceID, RoleID, ptable[i][2], 1, -1, 8, 104)
					break
				end
			end
		end
	elseif TalkIndex == 7 then
		if done > 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444111) 					--ʱװ״̬�������³���ʱװ�ռ������йأ���������ϸ�����ݡ�
			msg.AddMsgEvent(MsgID, 21, -1)   					--��ť
			msg.AddMsgEvent(MsgID, 1, 100002)   				--����
			msg.AddMsgEvent(MsgID, 24, CreatureID) 				--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444116) 					--����δ�����³�ʱװ�ռ�״̬�������ܡ���ʾ������ʱװ�ռ����״δﵽ40%���ɿ���״̬�������ơ�
			msg.AddMsgEvent(MsgID, 21, -1)   					--��ť
			msg.AddMsgEvent(MsgID, 1, 100002)   				--����
			msg.AddMsgEvent(MsgID, 24, CreatureID) 				--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5610052, 7, "c5610052On_Talk")
