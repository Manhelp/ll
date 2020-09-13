--���ʲ���ʦ
--��¼��ȡ���� RoleDataType["QiZhiCePing"] = 71 ����λ������λ���죬��λ����
function c5610051On_Talk(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID, TalkIndex)
	if role.GetRoleScriptData(RoleID, 1, RoleDataType["QiZhiCePing"]) == 0 then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["QiZhiCePing"], 10)
	end
	local done = math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["QiZhiCePing"])/10)
	local qizhiSum = role.GetWardrobeDetailInfo(MapID,InstanceID,RoleID)  --�������
	local mtable = {}
	mtable[6] = 1000
	mtable[5] = 750
	mtable[4] = 500
	mtable[3] = 350
	mtable[2] = 200
	mtable[1] = 50
	mtable[0] = 0
	for i=6,0,-1 do
		if qizhiSum and qizhiSum >= mtable[i] then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["QiZhiCePing"], done*10+i)
			break
		end
	end
	local can = role.GetRoleScriptData(RoleID, 1, RoleDataType["QiZhiCePing"])%10
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444100) 		--
		msg.AddMsgEvent(MsgID, 21, 4) 			--��ť
		msg.AddMsgEvent(MsgID, 1, 444101) 		--�����ҵ������ܺ�
		msg.AddMsgEvent(MsgID, 21, 5)   		--��ť
		msg.AddMsgEvent(MsgID, 1, 444112)   	--�뿪
		msg.AddMsgEvent(MsgID, 24, CreatureID) 	--npcid
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if role.GetBagFreeSize(RoleID) < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444102) 			--���ı����������޷���ý����������������
			msg.AddMsgEvent(MsgID, 21, 5)   			--��ť
			msg.AddMsgEvent(MsgID, 1, 444112)   		--ȡ��
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if can > 0 and done < 7 then
				local MsgID = msg.BeginMsgEvent()
				if done > can then
					msg.AddMsgEvent(MsgID, 20, 444103)			--����ǰ�������ܺ�Ϊ��xxx����û�дﵽ��һ�׶ν�������ȡҪ��\nע����1�׶ν�����Ҫ����50-199������ȡ
					msg.AddMsgEvent(MsgID, 9, qizhiSum)
				else
					msg.AddMsgEvent(MsgID, 20, 444104)			--����ǰ�������ܺ�Ϊ��xxx��������ȡ��x�׶εĽ�����\nע����1�׶ν�����Ҫ����50-199������ȡ
					msg.AddMsgEvent(MsgID, 9, qizhiSum)
					msg.AddMsgEvent(MsgID, 9, done)
					msg.AddMsgEvent(MsgID, 21, 6) 					--��ť
					msg.AddMsgEvent(MsgID, 1, 444105) 				--��ȡ����
				end
				msg.AddMsgEvent(MsgID, 21, 5) 					--��ť
				msg.AddMsgEvent(MsgID, 1, 444112) 				--ȡ��
				msg.AddMsgEvent(MsgID, 24, CreatureID) 			--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif done > 6 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444106) 				--����������ĳƺ�ʵ�����飡
				msg.AddMsgEvent(MsgID, 21, 5)   				--��ť
				msg.AddMsgEvent(MsgID, 1, 444112)   			--ȡ��
				msg.AddMsgEvent(MsgID, 24, CreatureID) 			--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444103) 			--����ǰ�������ܺ�Ϊ��xxx����û�дﵽ��һ�׶ν�������ȡҪ��\nע����1�׶ν�����Ҫ����50-199������ȡ
				msg.AddMsgEvent(MsgID, 9, qizhiSum)
				msg.AddMsgEvent(MsgID, 21, 5)   			--��ť
				msg.AddMsgEvent(MsgID, 1, 444112)   		--ȡ��
				msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	elseif TalkIndex == 6 then
		if role.GetBagFreeSize(RoleID) < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444102) 			--���ı����������޷���ý����������������
			msg.AddMsgEvent(MsgID, 21, 5)   			--��ť
			msg.AddMsgEvent(MsgID, 1, 444112)   		--ȡ��
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local s = role.GetRoleSex(RoleID)
			local ntable = {}
			ntable[1] = {3080022,3080023}
			ntable[2] = {3080024,3080025}
			ntable[3] = {3080026,3080027}
			ntable[4] = {3080028,3080029}
			ntable[5] = {3080030,3080031}
			ntable[6] = {3080032,3080033}
			for k,v in pairs(ntable) do
				if done == k and can > k - 1 then
					if s == 1 then
						role.AddRoleItem(MapID, InstanceID, RoleID, v[1], 1, -1, 8, 104)
					else
						role.AddRoleItem(MapID, InstanceID, RoleID, v[2], 1, -1, 8, 104)
					end
					role.SetRoleScriptData(RoleID, 1, RoleDataType["QiZhiCePing"], 10*(k+1))
					break
				end
			end
		end
	end
end
aux.RegisterCreatureEvent(5610051, 7, "c5610051On_Talk")
