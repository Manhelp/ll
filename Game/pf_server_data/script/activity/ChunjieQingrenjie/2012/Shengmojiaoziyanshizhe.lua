function n6040019_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444444) -- "���紵��ս���ޣ���������˭��˭����Һã����ǻʹ�ߣ���ӭ�������Ǿ�ӭ�µ�ʱ�̲μ�ʥħ�����硣ף������������󣬾����࣬������࣬��Ǯ��ࡣ"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, 7)
			msg.AddMsgEvent(MsgID, 1, 444445) -- �������˽�������������
			msg.AddMsgEvent(MsgID, 21, 8)
			msg.AddMsgEvent(MsgID, 1, 444446)   --"�����˽�ʲôʱ��ʼ��"
			msg.AddMsgEvent(MsgID, 21, 9)
			msg.AddMsgEvent(MsgID, 1, 444447)   --"����֪���������������
			msg.AddMsgEvent(MsgID, 21, 10)
			msg.AddMsgEvent(MsgID, 1, 444448)   --"ʹ����ã�ʹ���ټ���"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	elseif TalkIndex == 7 then
		local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444449) -- "�������̺����ף�ֻҪ���ռ����ӽ���<D>���ڵ����</D>�Ϳ����ˣ�<I>ˮ������</I>���Դ�<M>�´���è</M>�����Լ�<N>ŵ�Ƿ���</N>�����ռ���<I>��������</I>���Դ�<D>�Ʋ�ʹ��</D>����õ�������������;��������ʱ��������"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 444450) -- ���õģ���֪���ˡ���
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	elseif TalkIndex == 8 then
		local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444451) -- "�ڴ����ڼ�ÿ���<S>��0:00����6:00����12:00����18:00��</S>������ʥħ������������Щʱ��֮ǰϣ����λ�ܹ������������ռ�ɢ���������½�Ľ��ӣ�����<D>���ڵ����</D>����ÿ��ġ�5��00��5��59����11��00��11��59����17��00��17��59����23��00��23��59��ʱ������ǻ���⽱��ʱ�䣬����ǰ��������ҿ�����<D>���ڵ����</D>��ȡ���⽱����ǧ��Ҫ�����"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 444450) -- ���õģ���֪���ˡ���
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	elseif TalkIndex == 9 then
		local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444452)--"�������!�ڻʱ���ڣ�ÿ���Ͻ�5�����ӿ��Ի��һ���Ľ��������ظ���������ÿ�λ���⽱��ʱ�䣬�����Ͻ����������Ĳ�ͬ����Ϊ���������ڵ���500�������ڵ���1000�������ڵ���3000�������ҽ�����ȫ�����񵵴β�ͬ�Ľ�����������ʾ������ǰ��������ҿ�����<D>���ڵ����</D>��ȡ���⽱����ǧ��Ҫ�����"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 444450) -- ���õģ���֪���ˡ���
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	end
end
aux.RegisterCreatureEvent(6040019, 7, "n6040019_OnTalk")
