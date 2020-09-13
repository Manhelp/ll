function shendianchuansong_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --��ȡ��ҵȼ�
	--local Act = act.GetActIsStart(37)
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local day = os.date("%A")
	if (day == "Saturday" or day == "Sunday") then
	--if Act == true then
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206532)		--��Ԫ����������ˣ�\n  1������Ϊ40�����Ͽɽ��롣\n  2���븱�����������ƣ�����ӽ������ж϶����Ա�ȼ����������͵ȼ�����Ҳ��ɳ���10����������ʾ�������Ա�ȼ���������޷����븱����\n  3�����븱������ƣ��ֵ25�㣬����ʱ��Ϊ45���ӡ���
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 4)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 206541)		--������Ԫ����
			msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 4 then
			local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
			local minlv = 0		--��С
			local maxlv = 0		--���
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			if level < 30 then
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206533)		--Ԫ����������ˣ�\n    ֻ�г���30������Ҳ��ܽ���Ԫ����
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	100001)		--��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if TeamID ~= nil and TeamID ~= 4294967295 then
				local Role = {}
				Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
					for i=1, 6 do
						if Role[i] and Role[i]~= 4294967295 then
						    local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
						    local rolelevel = role.GetRoleLevel(Target_MapID, Target_InstanceID, Role[i])
							if minlv == 0 then
								minlv = rolelevel
								maxlv = rolelevel
							end
							if maxlv < rolelevel then
							    maxlv = rolelevel
							elseif minlv > rolelevel then
							    minlv = rolelevel
							end
						end
					end
					if maxlv - minlv > 15 then
					    local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20,	206534)		--Ԫ����������ˣ�\n    �������е���ҵȼ������󣨴���15�������޷�����Ԫ����
						msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
						msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
						msg.AddMsgEvent(MsgID, 1,	100001)		--��ȡ����
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					else
						role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3373741608, 91, 20, 217)--4567  791   10728

					end
				elseif TeamID ~= nil then
					role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3373741608, 91, 20, 217)

				end
			end
		end
	else
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206564)		--��Ԫ����������ˣ�\n  Ԫ��������ÿ����������ȫ�쿪������
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, -1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 100001)		--ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(1085204, 7, "shendianchuansong_OnTalk")
--j����Ԫ����������

function s53_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 206547)--����Ԫ�����
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

--ע��
aux.RegisterMapEvent("s53_1", 2, "s53_OnPlayerEnter")

--Ԫ��������
function shendianchukou_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --��ȡ��ҵȼ�
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206540)		--����Ҫ�뿪Ԫ�����ô����
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100001)		--��ȷ����
		msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then

		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3424073512, 685, 23510, 620)--

	end
end
aux.RegisterCreatureEvent(1085206, 7, "shendianchukou_OnTalk")
--[[1085203	����ճ����񷢲���
1085204	Ԫ��������
1085205	Ԫ��������
1085206	]]
