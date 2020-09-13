

--���괫���
function c5610100_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local curmon = tonumber(os.date("%m"))--��
	local curday = tonumber(os.date("%d"))--��
	local curyear = tonumber(os.date("%Y"))--��
	if (TalkIndex == -1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444701)				--����ʮ�£��ٹ�ͬ�죬�ҿ��Ǹ���������˺ܶ�ף�����Ͻ�ѡ����ϲ���İɣ����˿ɾ�û���ˡ���50�����µ�С���Ѿͱ�������ˣ���ȥ�����ɣ�
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)				--
		msg.AddMsgEvent(MsgID, 1, 444702)			--��ÿ�ձ�����
		msg.AddMsgEvent(MsgID, 21, 5)				--
		msg.AddMsgEvent(MsgID, 1, 444703)			--����ȡ������
		msg.AddMsgEvent(MsgID, 21, 6)				--
		msg.AddMsgEvent(MsgID, 1, 444704)			--���һ����ߡ�
		msg.AddMsgEvent(MsgID, 21, 30)				--
		msg.AddMsgEvent(MsgID, 1, 100002)			--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444242)  			--���㵽��50�������ܲμ��������Ͻ�ȥ�����ɡ�
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444705)				--�������Ѿ���ɱ����ˣ�����������������죬�Ϳ�����ȡ������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 10)				--�鿴�������
			msg.AddMsgEvent(MsgID, 1, 444706)
			msg.AddMsgEvent(MsgID, 21, -1)				--
			msg.AddMsgEvent(MsgID, 1, 100001)			--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			local lastDate = role.GetRoleScriptData(RoleID, 1, RoleDataType["clgbaodao"])
			local curDate = tonumber(os.date("%j"))
			local lastDays = role.GetRoleScriptData(RoleID, 1, RoleDataType["baodaotianshu"])
			local totalDays = role.GetRoleScriptData(RoleID, 1, RoleDataType["jileijingyan"])
			if(curDate == lastDate) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444243)				--�����㲻�Ǳ�����һ�����𣿿����������ģ��Դ���ô����ʹ��
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, 10)				--�鿴�������
				msg.AddMsgEvent(MsgID, 1, 444706)
				msg.AddMsgEvent(MsgID, 21, -1)				--
				msg.AddMsgEvent(MsgID, 1, 100001)			--��ȡ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if curDate - lastDate > 1 then
					lastDays = 1
				elseif curDate - lastDate == 1 or curDate == 0 then
					lastDays = lastDays + 1
				end
				role.SetRoleScriptData(RoleID, 1, RoleDataType["clgbaodao"], curDate)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["baodaotianshu"],lastDays)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["jileijingyan"],totalDays+1)
			end
		end
	elseif TalkIndex == 5 then
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		local lastDate = role.GetRoleScriptData(RoleID, 1, RoleDataType["clgbaodao"])
		local curDate = tonumber(os.date("%j"))
		local totalDays = role.GetRoleScriptData(RoleID, 1, RoleDataType["jileijingyan"])
		local lastdays = role.GetRoleScriptData(RoleID, 1, RoleDataType["baodaotianshu"])
		if level < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444242)  			--���㵽��50�������ܲμ��������Ͻ�ȥ�����ɡ�
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif lastdays < 3 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444707)       --���������������죬�޷���ȡ���飡
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100001)		--��ȷ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif totalDays <= 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444708)       --��û�п���ȡ�ľ��飬һ������ȥ��
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100001)		--��ȷ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif role.GetBagFreeSize(RoleID) < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)		--������<1
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 32,false
		else
			role.AddRoleExp(MapID, InstanceID, RoleID, totalDays*every_Exp[level])
			role.SetRoleScriptData(RoleID, 1, RoleDataType["clgjingyan"],curDate)
			role.AddRoleItem(MapID, InstanceID, RoleID,4830023, totalDays*3, -1, 8, 35)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["jileijingyan"],0)
		end
	elseif TalkIndex == 6 then
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444242)  			--���㵽��50�������ܲμ��������Ͻ�ȥ�����ɡ�
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444709)  			--���Ǻö�������ѡһ���ɣ�
			msg.AddMsgEvent(MsgID, 21, 7)
			msg.AddMsgEvent(MsgID, 1, 444710)		--���һ�+13����ʯ[��]��
			msg.AddMsgEvent(MsgID, 21, 8)
			msg.AddMsgEvent(MsgID, 1, 444711)		--���һ�+15����ʯ[��]��
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif(TalkIndex == 7) then
		local bangding = role.GetRoleItemNum(RoleID, 3020116)
		local bubangding = role.GetRoleItemNum(RoleID, 3020027)
		local duihuansilver = role.GetRoleSilver(MapID, InstanceID,RoleID)
		if(bangding + bubangding < 5) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444712)       --���ϲ��㣬�޷��һ�
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100001)		--��ȷ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif duihuansilver < 1500000 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444713)       --��Ǯ���㣬�޷��һ�
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100001)		--��ȷ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif role.GetBagFreeSize(RoleID) < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)		--������<1
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 32,false
		else
			if bangding >= 5 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3020116, 5, 101)
			elseif bangding >= 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3020116, bangding, 101)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3020027, 5-bangding, 101)
			end
			role.DecRoleSilver(MapID, InstanceID, RoleID, 1500000, 101)
 			role.AddRoleItem(MapID, InstanceID, RoleID,3020119, 1, -1, 8, 35)
		end
	elseif(TalkIndex == 8) then
		local bangding = role.GetRoleItemNum(RoleID, 3020119)
		local bubangding = role.GetRoleItemNum(RoleID, 3020030)
		local duihuansilver = role.GetRoleSilver(MapID, InstanceID,RoleID)
		if(bangding + bubangding < 5) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444712)       --���ϲ��㣬�޷��һ�
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100001)		--��ȷ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif duihuansilver < 8000000 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444713)       --��Ǯ���㣬�޷��һ�
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100001)		--��ȷ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif role.GetBagFreeSize(RoleID) < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)		--������<1
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 32,false
		else
			if bangding >= 5 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3020119, 5, 101)
			elseif bangding >= 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3020119, bangding, 101)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3020030, 5-bangding, 101)
			end
			role.DecRoleSilver(MapID, InstanceID, RoleID, 8000000, 101)
 			role.AddRoleItem(MapID, InstanceID, RoleID,3020121, 1, -1, 8, 35)
		end
	elseif(TalkIndex == 10) then
		local lastdays = role.GetRoleScriptData(RoleID, 1, RoleDataType["baodaotianshu"])
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444714)  			--������������Ϊ����
		msg.AddMsgEvent(MsgID, 9, lastdays)
		msg.AddMsgEvent(MsgID, 21, -1)
		msg.AddMsgEvent(MsgID, 1, 100001)		--��ȡ����
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(5610100, 7, "c5610100_On_Talk")
