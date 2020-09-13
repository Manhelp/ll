
--20349��ɫ��Ȼ��ʹ��4010192�����ֳ�ѩ֮��6011018������ɼ����4010193�ʻ� ��ѩ������

function Q20349_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	
	--�������s05����������ʹ��
	if MapID ~= 3424071720 then
	    bRet = 43
	else
	    --�õ���ҵ�ǰ����
		local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	
		if x < 2478 or x > 2775 or z < 2524 or z > 2914 then  --�������������귶Χ������ʹ��
			bRet = 43
		else
			local FreeSize = role.GetBagFreeSize(RoleID)
			if(FreeSize < 1) then
			--��ʾ��ұ����ռ䲻��
				bRet = 40
			end
		end
	end
	--����
	return bRet, bIgnore
end

aux.RegisterItemEvent(4010192, 0, "Q20349_CanUse")

function Q20349_QuestUsable(MapID, InstanceID, TypeID, RoleID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 6011018, x+1, y, z+1)
	
end

function Q20349_OnInvest(MapID, InstanceID, QuestID, RoleID, CreatureTypeID)

	role.AddRoleItem(MapID, InstanceID, RoleID, 4010193, 1, 0, 0, 402)
end


--ע��
aux.RegisterItemEvent(4010192, 1, "Q20349_QuestUsable")
aux.RegisterQuestEvent(20349, 9, "Q20349_OnInvest")
