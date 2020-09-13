
--��5600015�Ի�
function n5600015_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curhour = tonumber(os.date("%H"))
	if curhour >= 14 and curhour <= 16 then
		curhour = 1
	elseif curhour >= 17 and curhour <= 20 then
		curhour = 2
	elseif curhour >= 21 and curhour <= 23 then
		curhour = 3
	else
		curhour = 4
	end
	local time1 = curmon*1000 + curday *10 + curhour
	if curhour ~= 4 and time1 ~= role.GetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin_time"]) then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin_time"], time1)
	end
	if TalkIndex == -1 then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206335) -- "������ṩӦ�̣�\n    ÿ��14����14��30�֡�17����17��30�֡�21����21��30�֣�������ṩӦ���Ὺʼ����ʱ�Ϳ��Խ�������������ø��ֽ����������Ϳ��������¯��������\n    �ύ�����Ϳ��Ի�����걦��Ľ�����ÿ��ʱ����ύ����������3���˻���������Ķ��⽱����\n ���λ���ύ�������ͣ� XX����"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 9, role.GetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin"]))
		msg.AddMsgEvent(MsgID, 21, 5) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 206336) -- ���鿴�ύ���С�
		if Xinniandayan_start == 1 then
			msg.AddMsgEvent(MsgID, 21, 4)   --ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 206337)   --"����������"
		else
		    msg.AddMsgEvent(MsgID, 21, 6)   --ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 206338)   --"��ȡ���н���"
		end
		msg.AddMsgEvent(MsgID, 21, 10)   --ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100002)   --"ȡ��"
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
    elseif TalkIndex == 4 then
	    if Xinniandayan_start == 1 then
			local a = role.GetRoleItemNum(RoleID, 4800017)
			if a > 0 then
			    if role.GetBagFreeSize(RoleID) < 1 then
				    local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206339) -- "������ṩӦ�̣�\n    �ѵ��㲻��Ҫ�ύ�����͵ı����𣿿�ȥ����һ�±��������ɣ�"
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
				else
				    role.AddRoleItem(MapID, InstanceID, RoleID, 4800018, 1, -1, 8, 420)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800017, 1, 420)
					unit.AddBuff(MapID, InstanceID, RoleID, 2053901, RoleID)
					HuoDeXinnianshipin(RoleID)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206340) -- "������ṩӦ�̣�\n    ��Ҫ��ý����ʹ���������������ɣ�"
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206341) -- "������ṩӦ�̣�\n    ��û�������ͣ��ҿɲ����ʱ���˷��������ϡ�"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end
		end
    elseif TalkIndex == 5 then
	    local k = nil
		for i = 0, 4 do
		    if act.GetActScriptData(36, 1, i) ~= nil and act.GetActScriptData(36, 1, i) > 0 then
			    k = i
			end
		end
		if k == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206342) -- "������ṩӦ�̣�\n    ���ڻ�û���˽��ɹ������͡�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 0 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206343) -- "������ṩӦ�̣�\n    �����ύ��������������ǣ�\n    XXX        XXX��������
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 5))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206344) -- "������ṩӦ�̣�\n    �����ύ��������������ǣ�\n    XXX        XXX��������\n    XXX        XXX��������
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 5))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 6))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206345) -- "������ṩӦ�̣�\n    �����ύ��������������ǣ�\n    XXX        XXX��������\n    XXX        XXX��������\n    XXX        XXX��������
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 5))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 6))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 7))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 3 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206346) -- "������ṩӦ�̣�\n    �����ύ��������������ǣ�\n    XXX        XXX��������\n    XXX        XXX��������\n    XXX        XXX��������\n    XXX        XXX��������
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 5))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 6))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 7))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 8))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 4 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206347) -- "������ṩӦ�̣�\n    �����ύ��������������ǣ�\n    XXX        XXX��������\n    XXX        XXX��������\n    XXX        XXX��������\n    XXX        XXX��������\n    XXX        XXX��������
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 5))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 6))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 7))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 8))
            msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 4))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 9))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	elseif TalkIndex == 6 then
	    local Rank = 0
        for q=0,2 do
			if act.GetActScriptData(36, 1, q) == RoleID then
				Rank = q + 1
			end
		end
        if Rank==0 then
            local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206348) -- "������ṩӦ�̣�\n    ǰ��������û���������Ŷ����û��ʲô�������Ը��㡣"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
        elseif act.GetActScriptData(36, 1, Rank+9) > 0 then
            local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206349) -- "������ṩӦ�̣�\n    ���Ѿ���ȡ�������ˣ����ظ���ȡ�Ͳ���Ŷ��"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
        elseif role.GetBagFreeSize(RoleID) < 1 then
        	local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206350) -- "������ṩӦ�̣�\n    ��ȥ����һ�±����ɣ���Ȼ��װ���½�Ʒ��Ŷ��"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4800019, 1, -1, 8, 420)
			act.SetActScriptData(36, 1, Rank+9, 1)
			act.SaveActScriptData(36)
		end
    end
end

aux.RegisterCreatureEvent(5600015, 7, "n5600015_OnTalk")

function HuoDeXinnianshipin(RoleID)
    local i = role.GetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin"])
	if i == nil then
	    i = 0
	end
	for q=0,4 do                       --�ݴ����޸�
	    if act.GetActScriptData(36, 1, q) == RoleID then
		    if act.GetActScriptData(36, 1, q+5) >= i then
			    i = act.GetActScriptData(36, 1, q+5)
			end
		end
	end
	i = i + 1
	role.SetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin"], i)

	--[[if i % 100 == 0 and i ~= 0 and i ~= nil then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 2370)  --<�����>�ڱ��λ���Ѿ�������XXX�������ͣ����������ܱȣ�
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 9, i)
		msg.DispatchWorldMsgEvent(MsgID)
	end]]

	local arrID = {}
	local arrNum = {}
	local bNew = 1
	for l=0,4 do
		arrID[l] = act.GetActScriptData(36, 1, l)
		if arrID[l] == nil then
		    arrID[l] = 0
		end
		if arrID[l] == RoleID  then
			arrNum[l] = i
			bNew = 0
		else
			arrNum[l] = act.GetActScriptData(36, 1, l+5)
			if arrNum[l] == nil then
		        arrNum[l] = 0
		    end
		end
	end

	if bNew == 1 then
		arrID[5] = RoleID;
		arrNum[5] = i;
	else
		arrID[5] = 0;
		arrNum[5] = 0;
	end

	for m=0, 4 do
		for n=m+1,5 do
			if arrNum[n] > arrNum[m] then
				local temp = arrNum[m]
				arrNum[m] = arrNum[n]
				arrNum[n] = temp
				temp = arrID[m]
				arrID[m] = arrID[n]
				arrID[n] = temp
			end
		end
	end

	for j=0,4 do
		act.SetActScriptData(36, 1, j, arrID[j])
		act.SetActScriptData(36, 1, j+5, arrNum[j])
	end

	act.SaveActScriptData(36)

end












