
--����ܶ�Ա�Ի��ű�
function c6011030_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --����ǵ�һ�ζԻ�
    local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206756)		--������ܶ�Ա��ʹ��\n         �����ǵ�XX�ڽ���ܶ�Ա������յĴ󽱻�����XXXX��С��������xxxx��\n         ÿ������21��30�ֽ���������յĿ���������������3λ���˶�ƽ�ֽ��𣨴󽱵Ŀ�������Ϊ10%�������ڽ���Ϊÿ��XX��\n         ����н���Ҳ����ߣ��򽱽���ۻ����´ο������н�����Ҫ�ӽ����п۳�5%�������ѡ���
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 9,	act.GetActScriptData(29, 1, benrijiangli_Index))
		msg.AddMsgEvent(MsgID, 9,	act.GetActScriptData(29, 1, jiangchi_JBZDY)+act.GetActScriptData(29, 1, benrijiangli_JBZDY))
		msg.AddMsgEvent(MsgID, 9,	act.GetActScriptData(29, 1, benrijiangli_JBZDY))
		msg.AddMsgEvent(MsgID, 9,	act.GetActScriptData(29, 1, benrijiangli_JBZDY_LastGold))
		msg.AddMsgEvent(MsgID, 21,	4)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	206757)		--��������
		msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	100002)		--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if rolelevel < 20 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206758)		--������ܶ�Ա��ʹ��\n         ֻ��20�����ϵ���Ҳ��ܲ������ܶ�Ա�����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	100001)		--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local jinbizongdongyuan = role.GetRoleScriptData(RoleID, 1, RoleDataType["Time_JBZDY"])
			local benrijiangli_CurTime = act.GetActScriptData(29, 1, benrijiangli_Index)
			if jinbizongdongyuan == benrijiangli_CurTime then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206759)		--������ܶ�Ա��ʹ��\n         ���Ѿ������μ��˻������21��30�����ߵȺ��ȡ��������ߡ���
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	100001)		--��ȡ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
			    local money = math.floor(rolelevel/2)
				if role.GetRoleSilver(MapID, InstanceID, RoleID) < money*10000 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	206760)		--������ܶ�Ա��ʹ��\n         ����ҪXX����ܲ������ܶ�Ա�����
					msg.AddMsgEvent(MsgID, 24,	TargetID)
					msg.AddMsgEvent(MsgID, 9, money)
					msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
					msg.AddMsgEvent(MsgID, 1,	100001)		--��ȡ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					role.DecRoleSilver(MapID, InstanceID, RoleID, money*10000, 452)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["Time_JBZDY"],benrijiangli_CurTime)
					for k,v in pairs(tbl_JBZDY_Online) do
						if v == RoleID then
							table.remove(tbl_JBZDY_Online,k)
						end
					end
					for k,v in pairs(tbl_JBZDY_Offline) do
						if v == RoleID then
							table.remove(tbl_JBZDY_Offline,k)
						end
					end
					table.insert(tbl_JBZDY_Online,RoleID)
					local bigmoney = math.floor(money*0.3)
					local smallmoney = money - bigmoney
					act.SetActScriptData(29, 1, jiangchi_JBZDY,act.GetActScriptData(29, 1, jiangchi_JBZDY)+bigmoney)
					act.SetActScriptData(29, 1, benrijiangli_JBZDY,act.GetActScriptData(29, 1, benrijiangli_JBZDY)+smallmoney)
					act.SaveActScriptData(29)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 71, 206753)		--�����ɹ����󽱽�������XXX��С����������XXX���������9��30����ʱ����Ҳ����ߣ���ý��𽫻��Զ��ۼƽ��󽱽��ء�
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(29, 1, jiangchi_JBZDY)+act.GetActScriptData(29, 1, benrijiangli_JBZDY))
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(29, 1, benrijiangli_JBZDY))
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
        end
	end
end

-- ע��
aux.RegisterCreatureEvent(6011030, 7, "c6011030_OnTalk")

