
function WanShengJie_OnStart(actID)
	local k = act.GetActScriptData(30, 1, 20)
	local curyear = tonumber(os.date("%Y"))
	if k == 0 or k == nil or k ~= curyear then
	    for i=0, 19 do
		    act.SetActScriptData(30, 1, i, 0)
		end
		act.SetActScriptData(30, 1, 20, curyear)
		act.SaveActScriptData(30)
	end
end

--ע��û������¼�
--aux.RegisterActEvent(30, 2, "WanShengJie_OnStart")

--�������ʦ5600002�Ի�
function n5600002_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    if TalkIndex == -1 then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206201) -- "��ʥ�ڴ�ʹ��\n    �������������С�һ�����ǲ����ռ��˺ܶ���ֽ������11��8��24��00֮ǰ���������ֽ�����ҵ�10����ҽ���ĵ��߽�����\n  1��Ϊ��������ʥ��ʱװһ�ף���������ħ����һ����\n    ��2��5��Ϊ��������ʥ��ʱװһ�ף�30������ħ����һ����\n    ��6��10��Ϊ��������ʥ��ʱװһ�ף�7������ħ����һ����\n    ���Ѿ������ҵ���ֽ���ǣ�XXX��"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 9, role.GetRoleScriptData(RoleID, 1, RoleDataType["TangZhi"]))
		msg.AddMsgEvent(MsgID, 21, 5) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 206202) -- ����ֽ�������а�ǰʮ������
		msg.AddMsgEvent(MsgID, 21, 4)   --ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 206203)   --"������ֽ"
		if act.GetActIsStart(30) ~= true then
		    msg.AddMsgEvent(MsgID, 21, 6)   --ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 206219)   --"��ȡ����"
		end
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
    elseif TalkIndex == 4 then
	    if act.GetActIsStart(30) ~= true then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206204) -- "��ʥ�ڴ�ʹ��\n    ��ʥ����ֽ�ύ��ʱ����11��9��֮ǰ�����Ѿ������ʱ�䣡"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
			local a = role.GetRoleItemNum(RoleID, 4040006)
			if a > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4040006, 1, 420)
				HuoDeTangZhi(RoleID)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206201) -- "��ʥ�ڴ�ʹ��\n    �������������С�һ�����ǲ����ռ��˺ܶ���ֽ������11��9��֮ǰ���������ֽ�����ҵ�10����ҽ���������ľ�ϲŶ��"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 9, role.GetRoleScriptData(RoleID, 1, RoleDataType["TangZhi"]))
				msg.AddMsgEvent(MsgID, 21, 5) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 206202) -- ����ֽ�������а�ǰʮ������
				msg.AddMsgEvent(MsgID, 21, 4)   --ȡ����ť
				msg.AddMsgEvent(MsgID, 1, 206203)   --"������ֽ"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206205) -- "��ʥ�ڴ�ʹ��\n    ��û����ֽ���ѵ�����ʥ��������������ѷ����ǹ���"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end
		end
    elseif TalkIndex == 5 then
	    local k = nil
		for i = 0, 9 do
		    if act.GetActScriptData(30, 1, i) ~= nil and act.GetActScriptData(30, 1, i) > 0 then
			    k = i
			end
		end
		if k == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206206) -- "��ʥ�ڴ�ʹ��\n    ������ֽ���ң�����ǵ�һ�������ֽ�����˶��ˣ�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 0 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206207) -- "��ʥ�ڴ�ʹ��\n    �������Ͻ���ֽ�������ǣ���\n    ��ֽ����xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206208) -- "��ʥ�ڴ�ʹ��\n    �������Ͻ���ֽ�������ǣ���\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 11))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 1))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206209) -- "��ʥ�ڴ�ʹ��\n    �������Ͻ���ֽ�������ǣ���\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 11))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 12))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 2))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 3 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206210) -- "��ʥ�ڴ�ʹ��\n    �������Ͻ���ֽ�������ǣ���\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 11))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 12))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 13))
            msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 3))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 4 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206211) -- "��ʥ�ڴ�ʹ��\n    �������Ͻ���ֽ�������ǣ���\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 11))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 12))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 13))
            msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 14))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 4))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 5 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206212) -- "��ʥ�ڴ�ʹ��\n    �������Ͻ���ֽ�������ǣ���\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 11))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 12))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 13))
            msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 14))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 4))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 15))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 5))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 6 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206213) -- "��ʥ�ڴ�ʹ��\n    �������Ͻ���ֽ�������ǣ���\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 11))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 12))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 13))
            msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 14))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 4))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 15))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 5))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 16))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 6))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 7 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206214) -- "��ʥ�ڴ�ʹ��\n    �������Ͻ���ֽ�������ǣ���\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 11))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 12))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 13))
            msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 14))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 4))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 15))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 5))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 16))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 6))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 17))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 7))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 8 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206215) -- "��ʥ�ڴ�ʹ��\n    �������Ͻ���ֽ�������ǣ���\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 11))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 12))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 13))
            msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 14))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 4))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 15))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 5))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 16))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 6))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 17))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 7))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 18))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 8))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 9 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206216) -- "��ʥ�ڴ�ʹ��\n    �������Ͻ���ֽ�������ǣ���\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx\n    ��ֽ����xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 11))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 12))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 13))
            msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 14))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 4))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 15))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 5))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 16))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 6))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 17))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 7))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 18))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 8))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 19))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 9))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
    elseif TalkIndex == 6 then
	    local Rank = 0
        for q=0,9 do
			if act.GetActScriptData(30, 1, q) == RoleID then
				Rank = q + 1
			end
		end
        if Rank==0 then
            local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206220) -- "��ʥ�ڴ�ʹ��\n    ǰʮ������û���������Ŷ����û��ʲô�������Ը��㡣"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
        elseif act.GetActScriptData(30, 1, Rank+21) > 0 then
            local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206221) -- "��ʥ�ڴ�ʹ��\n    ���Ѿ���ȡ�������ˣ����ظ���ȡ�Ͳ���Ŷ��"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
        elseif role.GetBagFreeSize(RoleID) < 2 then
        	local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206222) -- "��ʥ�ڴ�ʹ��\n    ��ȥ����һ�±����ɣ���Ȼ��װ���½�Ʒ��Ŷ��"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 100001) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
		    local sex = role.GetRoleSex(RoleID)
			if sex == 1 then
			    role.AddRoleItem(MapID, InstanceID, RoleID, 3090032, 1, -1, 8, 420)
			elseif sex == 0 then
			    role.AddRoleItem(MapID, InstanceID, RoleID, 3090034, 1, -1, 8, 420)
			end
            if	Rank == 1 then
			    role.AddRoleItem(MapID, InstanceID, RoleID, 4220604, 1, -1, 8, 420)
			elseif Rank <= 5 then
			    role.AddRoleItem(MapID, InstanceID, RoleID, 4220603, 1, -1, 8, 420)
			elseif Rank <= 10 then
			    role.AddRoleItem(MapID, InstanceID, RoleID, 4220602, 1, -1, 8, 420)
			end
			act.SetActScriptData(30, 1, Rank+21, 1)
			act.SaveActScriptData(30)
		end
	end
end

aux.RegisterCreatureEvent(5600002, 7, "n5600002_OnTalk")

function I4040001_OnUse(MapID, InstanceID, TypeID, RoleID)
    local i = math.random(100)
	if i <= 40 then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 4040002, 1, 1, 8, 420)
	elseif i <= 80 then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 4040003, 1, 1, 8, 420)
	elseif i <= 95 then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 4040004, 1, 1, 8, 420)
	else
	    role.AddRoleItem(MapID, InstanceID, RoleID, 4040005, 1, 1, 8, 420)
	end
end

function I4040001_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4040001, 1, "I4040001_OnUse")
aux.RegisterItemEvent(4040001, 0, "I4040001_CanUse")


function I4040004_OnUse(MapID, InstanceID, TypeID, RoleID)

	role.AddRoleItem(MapID, InstanceID, RoleID, 8180001, 1, 3, 8, 420)

end

function I4040004_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4040004, 1, "I4040004_OnUse")
aux.RegisterItemEvent(4040004, 0, "I4040004_CanUse")

--����Ǿޱ
function I4040002_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
    local ItemID = 0
	local k = math.random(100)
	if k <= 80 then
	    ItemID = 4040003
	elseif k <= 90 then
	    ItemID = 4040004
	else
	    ItemID = 4040005
	end
    local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, ItemID, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4040006, 1, -1, 8, 420)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 206217)          --���յ�������XXX��XXXX��
    msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 4, ItemID)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)     --������Ϣ
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 0)
	msg.AddMsgEvent(MsgID, 1, 206217)          --���յ�������XXX��XXXX��
    msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 4, ItemID)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)     --������Ϣ
end

function I4040002_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2402)   --Ŀ��������ҵĿռ䲻��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		bRet = 32
	end
	local FreeSize1 = role.GetBagFreeSize(RoleID)
	if(FreeSize1 < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4040002, 1, "I4040002_OnUse")
aux.RegisterItemEvent(4040002, 0, "I4040002_CanUse")
