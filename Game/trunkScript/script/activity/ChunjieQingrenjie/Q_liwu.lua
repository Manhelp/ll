function Q_meigui_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	local sex = role.GetRoleSex(RoleID)
	local FreeSize = role.GetBagFreeSize(TargetID)
	local FreeSize1 = role.GetBagFreeSize(RoleID)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local level1 = role.GetRoleLevel(Target_MapID, Target_InstanceID, TargetID)
	if (level1 < 30) then
		--��ʾ��ҵȼ�����
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100009)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		bRet = 32
		return bRet, bIgnore
	elseif sex == 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206559)--����Ʒֻ������ʹ�á�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     
		bRet = 8
		return bRet, bIgnore
	elseif (FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100010)   --Ŀ��������ҵĿռ䲻��
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		bRet = 32
		return bRet, bIgnore
	elseif (FreeSize1 < 1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
		return bRet, bIgnore
	end
end
aux.RegisterItemEvent(4800052, 0, "Q_meigui_CanUse")--õ��


function Q_meigui_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	local sex = role.GetRoleSex(TargetID)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4800072, 1, -1, 8, 420)
	if sex == 1 then
		role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 4800077, 1, -1, 8, 420)--��ͬ־��ף����
		local MsgID = msg.BeginMsgEvent();	
		msg.AddMsgEvent(MsgID,13,206561)--XX����һ��õ�塭��
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
	if sex == 0 then
		role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 4800078, 1, -1, 8, 420)--���氮���顱
		local MsgID = msg.BeginMsgEvent();	
		msg.AddMsgEvent(MsgID,13,206561)--XX����һ��õ�塭��
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
end
aux.RegisterItemEvent(4800052, 1, "Q_meigui_OnUse")

function Q_qiaokeli_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	local sex = role.GetRoleSex(RoleID)
	local FreeSize = role.GetBagFreeSize(TargetID)
	local FreeSize1 = role.GetBagFreeSize(RoleID)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local level1 = role.GetRoleLevel(Target_MapID, Target_InstanceID, TargetID)
	if (level1 < 30) then
		--��ʾ��ҵȼ�����
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100009)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		bRet = 32
		return bRet, bIgnore
	elseif sex == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206558)--����Ʒֻ��Ů��ʹ�á�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     
		bRet = 8
		return bRet, bIgnore
	elseif (FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100010)   --Ŀ��������ҵĿռ䲻��
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		bRet = 32
		return bRet, bIgnore
	elseif (FreeSize1 < 1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
		return bRet, bIgnore
	end
end
aux.RegisterItemEvent(4800051, 0, "Q_qiaokeli_CanUse")--�ɿ���

function Q_qiaokeli_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	local sex = role.GetRoleSex(TargetID)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4800072, 1, -1, 8, 420)
	if sex == 1 then
		role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 4800078, 1, -1, 8, 420)--���氮���顱
		local MsgID = msg.BeginMsgEvent();	
		msg.AddMsgEvent(MsgID,13,206560)--XX����һ���ɿ�������
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
	if sex == 0 then
		role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 4800079, 1, -1, 8, 420)--����˿�ߡ�
		local MsgID = msg.BeginMsgEvent();	
		msg.AddMsgEvent(MsgID,13,206560)--XX����һ���ɿ�������
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
end
aux.RegisterItemEvent(4800051, 1, "Q_qiaokeli_OnUse")