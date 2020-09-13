function I4830026_GiftBag(MapID, InstanceID, TypeID, TargetID) --�е�
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 8460201, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8460202, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8460203, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8460204, 1, 6, 8, 420)
end

function I4830026_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4830026, 1, "I4830026_GiftBag")
aux.RegisterItemEvent(4830026, 0, "I4830026_CanUseGiftBag")

function I4830027_GiftBag(MapID, InstanceID, TypeID, TargetID) --Ů��
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 8460211, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8460212, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8460213, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8460214, 1, 6, 8, 420)
end

function I4830027_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4830027, 1, "I4830027_GiftBag")
aux.RegisterItemEvent(4830027, 0, "I4830027_CanUseGiftBag")

function I4830008_GiftBag(MapID, InstanceID, TypeID, TargetID) --����С���
	role.AddRoleItem(MapID, InstanceID, TargetID, 4830024, 1, -1, 8, 420)
    local gold = 0
	local temp = math.random(1000)
	if temp<=700 then
		gold=math.random(5,15)
	elseif temp<=800 then
	    gold=math.random(16,25)
	elseif	temp<=850 then
	    gold=math.random(26,35)
	elseif	temp<=900 then
	    gold=math.random(36,45)
	else
	    gold=math.random(46,50)
	end
	role.AddRoleSilver(MapID, InstanceID, TargetID, gold*10000, 101)
	local r = math.random(100)
	if r>90 then
		--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
		role.AddRoleItem(MapID, InstanceID, TargetID, 4830011, 1, -1, 8, 420)
	end


end

function I4830008_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	local freegold=role.GetRoleSilver(MapID,InstanceID,TargetID)
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	elseif(freegold > 9999499999) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 306002)--��Ҫ����x�񱳰��ռ�ſɼ�������
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 999
	end
	return bRet, bIgnore
end



aux.RegisterItemEvent(4830008, 1, "I4830008_GiftBag")
aux.RegisterItemEvent(4830008, 0, "I4830008_CanUseGiftBag")


function I4830009_GiftBag(MapID, InstanceID, TypeID, TargetID) --���ڴ���
	role.AddRoleItem(MapID, InstanceID, TargetID, 4830024, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4830011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4830010, 3, -1, 8, 420)
    local gold = 0
	local temp = math.random(1000)
	if temp<=300 then
		gold=math.random(200,250)
	elseif temp<=400 then
	    gold=math.random(251,300)
	elseif	temp<=500 then
	    gold=math.random(301,350)
	elseif	temp<=600 then
	    gold=math.random(351,400)
	elseif	temp<=800 then
	    gold=math.random(401,450)
	else
	    gold=math.random(451,500)
	end
	role.AddRoleSilver(MapID, InstanceID, TargetID, gold*10000, 101)
	local r = math.random(100)
	if r>95 then
		--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
		role.AddRoleItem(MapID, InstanceID, TargetID, 4830018, 1, -1, 8, 420)
	end


end

function I4830009_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	local freegold=role.GetRoleSilver(MapID,InstanceID,TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40

		--�жϽ�Ǯ�Ƿ񳬳�����
    elseif(freegold > 9997999999) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 306002)--��Ҫ����x�񱳰��ռ�ſɼ�������
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 999
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4830009, 1, "I4830009_GiftBag")
aux.RegisterItemEvent(4830009, 0, "I4830009_CanUseGiftBag")


function I4830006_GiftBag(MapID, InstanceID, TypeID, TargetID)--�в�
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local num = role.GetRoleScriptData(TargetID, 1, RoleDataType["jubaopen"])
	if num == nil then
		num = 0
	end
	local r = math.random(1000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={0,1,-1,265}
    tbl[2]={4830024,1,-1,200}  --2012
    tbl[3]={4830023,1,-1,100}  --�����
    tbl[4]={3020116,1,-1,100}  --����ʯ+10
    tbl[5]={3030147,1,-1,50}  --�챦ʯ+4
    tbl[6]={3030131,1,-1,50}  --ר�����ʯ[4��]
    tbl[7]={3030140,1,-1,50}  --ר��̫��ʯ[4��]
    tbl[8]={3030141,1,-1,50}  --ר���¹�ʯ[4��]
    tbl[9]={3030134,1,-1,50}   --ר������ʯ[4��]
	tbl[10]={3020119,1,-1,30}  --����ʯ+13
    tbl[11]={3030152,1,-1,10}  --ר���¹�ʯ[5��]
	tbl[12]={3030153,1,-1,10}   --ר��̫��ʯ[5��]
	tbl[13]={3030137,1,-1,10}  --ר�к챦ʯ[5��]
    tbl[14]={3030139,1,-1,10}  --ר������ʯ[5��]
    tbl[15]={3030138,1,-1,10}  --ר�����ʯ[5��]
    tbl[16]={3020121,1,-1,5} --15����ʯ
	local player=role.GetRoleLevel(MapID, InstanceID, TargetID)
	if player%2 ~= 0 then   --%ȡ�����˼��������ȡ������ż���ģ�
		player =  player + 1  ---�����Զ�+1���ż��
	end
	local gold = math.random(player/2,player*2)

	for i = 1,16 do
		temp = temp + tbl[i][4]
		if temp >= r then
			index = i
			break
		end
	end
	if(index == 3) then
		if(num >= 50 and num < 100) then
			if(math.random(2)==1)then
				index = 1
			end
		elseif(num >= 100 and num < 150) then
			if(math.random(3) <=2)then
				index = 1
			end
		elseif( num >= 150) then
			index = 1
		end
		if index == 3 then
			role.SetRoleScriptData(TargetID, 1, RoleDataType["jubaopen"],num+1)
		end
	end
	if index == 1 then
		role.AddRoleSilver(MapID, InstanceID, TargetID, gold*10000, 101)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)
	end
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800239, 1, 420)

end

function I4830006_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	local freegold=role.GetRoleSilver(MapID,InstanceID,TargetID)
	local player=role.GetRoleLevel(MapID, InstanceID, TargetID)
	local num = role.GetRoleItemNum(TargetID, 4800239)
	if num < 1 then
		bRet = 103
	elseif(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
			--�жϽ�Ǯ�Ƿ񳬳�����
    elseif(freegold > 9999999999-(player+1)*2*10000) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 306002)--��Ҫ����x�񱳰��ռ�ſɼ�������
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 999
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4830006, 1, "I4830006_GiftBag")
aux.RegisterItemEvent(4830006, 0, "I4830006_CanUseGiftBag")


function I4830023_OnUse(MapID, InstanceID, TypeID, TargetID) --�����
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4830028, 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4830024, 1, 420)
end

function I4830023_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	local num = role.GetRoleItemNum(TargetID, 4830024)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	elseif num < 1 then
		bRet = 104
	end

	return bRet, bIgnore
end

aux.RegisterItemEvent(4830023, 1, "I4830023_OnUse")
aux.RegisterItemEvent(4830023, 0, "I4830023_CanUse")



function I3090035_GiftBag(MapID, InstanceID, TypeID, TargetID) --�е�
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 8440401, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8440402, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8440403, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8440404, 1, 6, 8, 420)
end

function I3090035_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090035, 1, "I3090035_GiftBag")
aux.RegisterItemEvent(3090035, 0, "I3090035_CanUseGiftBag")

function I3090036_GiftBag(MapID, InstanceID, TypeID, TargetID) --Ů��
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 8440411, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8440412, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8440413, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8440414, 1, 6, 8, 420)
end

function I3090036_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090036, 1, "I3090036_GiftBag")
aux.RegisterItemEvent(3090036, 0, "I3090036_CanUseGiftBag")
