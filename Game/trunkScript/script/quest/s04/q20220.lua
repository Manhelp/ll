
--20220�̻�ȼ��ʹ��4010190�̻����4010191�ź�ӡ��

function Q20220_CanUse0(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false

	--�������s04����������ʹ��
	if MapID ~= 3424071976 then
	    bRet = 43
	else
	    --�õ���ҵ�ǰ����
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)

	    if x < 406 or x > 418 or z < 933 or z > 945 then  --�������������귶Χ������ʹ��
	        bRet = 43
	    else
		--����С��1ʱ����ʹ��
		local FreeSize = role.GetBagFreeSize(RoleID)
		if (FreeSize < 1) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			bRet = 40
			return bRet, bIgnore
		end
	    end
	end
	--����
	return bRet, bIgnore
end

aux.RegisterItemEvent(4010190, 0, "Q20220_CanUse0")


function Q20220_QuestUsable(MapID, InstanceID, TypeID, RoleID)

	role.AddRoleItem(MapID, InstanceID, RoleID, 4010191, 1, 0, 0, 402)
		
end

function Q20220_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
		return bRet, bIgnore
	end
end


--ע��
aux.RegisterItemEvent(4010190, 1, "Q20220_QuestUsable")
aux.RegisterItemEvent(4010191, 0, "Q20220_CanUse")

