function I4830047_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID, ������ƷTypeID,����,Ʒ��,����ģʽ,log��������Ϣö��
	local r = math.random(10)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={8010020,1}
	tbl[2]={8050020,1}
	tbl[3]={8060020,1}
	tbl[4]={8070020,1}
	tbl[5]={8080020,1}
	tbl[6]={8090020,1}
	tbl[7]={8100020,1}
	tbl[8]={8110020,1}
	tbl[9]={8121020,1}
	tbl[10]={8130020,1}

	for i = 1,10 do
	    temp = temp + tbl[i][2]
		if temp >= r then
		    index = i
			break
		end
	end

	local high,low = role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], 1, -1, 8, 420)
	role.SetGrowEquipLevel(TargetID,high,low,1130)

end

function I4830047_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4830047, 1, "I4830047_GiftBag")
aux.RegisterItemEvent(4830047, 0, "I4830047_CanUseGiftBag")
