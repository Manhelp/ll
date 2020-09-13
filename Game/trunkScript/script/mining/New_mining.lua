NewMining_Num = {}

--����Ƿ���ʹ��
function NEWmining_CanUse(MapID, InstanceID, TypeID, TargetID)
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003) --��Ҫ����&lt;p1&gt;�񱳰��ռ�ſɼ�������
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		return 43, false
	end

	if MapID == 52649974 or MapID == 52649206 or MapID == 52649462 then
		return 0, false
	end
	if MapID == 3934900289 then
		local bCanUse1 = map.IsInArea(MapID, InstanceID, TargetID, 160)
		local bCanUse2 = map.IsInArea(MapID, InstanceID, TargetID, 161)
		local bCanUse3 = map.IsInArea(MapID, InstanceID, TargetID, 163)
		if bCanUse1 == true or bCanUse2 == true or bCanUse3 == true then
			return 0, false
		end
	end
	return 43, false
end


--�Ͻ���
function NEWmining_OnUse(MapID, InstanceID, TypeID, TargetID)
    local ItemID = 4820105
	local curDate = tonumber(os.date("%j"))
	if NewMining_Num.date == nil then
		NewMining_Num.date = curDate
		NewMining_Num.tuzhi = 0
		NewMining_Num.huozhong1 = 0
		NewMining_Num.huozhong2 = 0
		NewMining_Num.yaoshi = 0
	elseif NewMining_Num.date ~= curDate then
		NewMining_Num.date = curDate
		NewMining_Num.tuzhi = 0
		NewMining_Num.huozhong1 = 0
		NewMining_Num.huozhong2 = 0
		NewMining_Num.yaoshi = 0
	end
	local index = math.random(300)
	local tbl = {}
	if MapID == 3934900289 then
		tbl = {273,2,5,1,1,3,15,0} --����Կ��ÿ��2����11-15�꾧ÿ��5����16-20�꾧��ͼֽ����ÿ��1����ϡ�в��������ƣ�������������
	else
	    tbl = {207,4,10,2,2,6,60,9}
	end
    local total = 0
	for i= 1,8 do
	    total = total + tbl[i]
		if index <= total then
		    index = i
			break
		end
	end
	if math.random(100) == 17 then
		if index == 2 then    --����Կ��
	        NewMining_Num.yaoshi = NewMining_Num.yaoshi + 1
			if NewMining_Num.yaoshi <= 4 then
				ItemID = 4820162
			end
		elseif index == 3 then   --11-15�꾧����
			NewMining_Num.huozhong1 = NewMining_Num.huozhong1 + 1
			if NewMining_Num.huozhong1 <= 10 then
				ItemID = 4820131 + math.random(5) + (math.random(2)-1)*20
			end
	    elseif index == 4 then   --16-20�꾧����
			NewMining_Num.huozhong2 = NewMining_Num.huozhong2 + 1
			if NewMining_Num.huozhong2 <= 2 then
				ItemID = 4820136 + math.random(5) + (math.random(2)-1)*20
			end
	    elseif index == 5 then    --ͼֽ
	        NewMining_Num.tuzhi = NewMining_Num.tuzhi + 1
			if NewMining_Num.tuzhi <= 3 then
				ItemID = 4820191 + math.random(36)
			end
		elseif index == 6 then    --��Ƭ
			ItemID = 4820119 + math.random(2)
		elseif index == 7 then  --��ͨ����
            ItemID = 4820121 + math.random(10) + (math.random(2)-1)*20
		elseif index == 8 then  --ϡ�жһ���ħ�����
		    ItemID = 4820111
		end
	else
	    if math.random(1000) >= 976 then
		    ItemID = 4820110
		end
	end

    if ItemID == 4820105 then
		if math.random(30) == 7 then
		    ItemID = ItemID + 1
		elseif math.random(20) == 7 then
		    ItemID = ItemID + 2
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, ItemID, 1, -1, 8, 420)
	role.FishingSendClient(MapID, InstanceID, TargetID) --���߿ͻ�������ʹ��(ԭ����)
end

aux.RegisterItemEvent(4820112, 0, "NEWmining_CanUse")
aux.RegisterItemEvent(4820112, 1, "NEWmining_OnUse")
