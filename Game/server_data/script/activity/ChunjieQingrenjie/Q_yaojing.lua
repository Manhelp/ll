--1���ٻ�����
--2���ٻ�ʱ��
--3�� ��¼����
touming = 0 -- ��ʼ͸��NPCIDΪ0
function Mrbobo_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local Num = role.GetRoleItemNum(RoleID, 4800073)--����֮��
	--local Act = act.GetActIsStart(38)
	local curhour = tonumber(os.date("%H"))
	local lTime =  map.GetMapScriptData(MapID, InstanceID, 1, 2)--ʹ��ʱ��
	local cTime = os.time()--��ǰʱ��
	local a = map.GetMapScriptData(MapID,InstanceID,1,1)--�ٻ�����
	local s = os.difftime(cTime,lTime) -- ������һ���ٻ�ʱ��
	local CurTime = tonumber(os.date("%j"))--����
	local CurTime1 = map.GetMapScriptData(MapID, InstanceID, 1, 3)
	local day = os.date("%A")--����
	if CurTime ~= CurTime1 then
		a = 0
		map.SetMapScriptData(MapID, InstanceID, 1, 1, a)
	end

	--if Act ~= true then
	--	if TalkIndex == -1 then
	--		local MsgID = msg.BeginMsgEvent()
	--		msg.AddMsgEvent(MsgID, 20, 206566)		--������������\n  ���˽��������ڱ��λ�ڽ����ٻ���ʱ��Ϊÿ���18��00-24��00��ÿ30���ӿ��ٻ�һ�Σ���Ҫ����10������֮�ǣ�������Ʒ2���Ӻ����˽�����������ڲ����������ĳ��λ�á��ܲ��������ɾ�Ҫ����������ˡ���
	--		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
	--		msg.AddMsgEvent(MsgID, 21, -1)			--ȡ����ť
	--		msg.AddMsgEvent(MsgID, 1, 206552)		--�����û�п�ʼ��
	--		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	--	end
	--elseif Act == true then
	if Num < 10 then
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206566)		--������������\n    ����
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, -1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 206554)		--�����ϵ�����֮����������10�����޷��ٻ�������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	else
		if a > 4 then
			if TalkIndex == -1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206566)		--������������\n    ����
				msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21, -1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1, 206555)		--�����ٻ������Ĵ����Ѵﵽ5��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			if s < 3600 then
				if TalkIndex == -1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206566)		--������������\n    ����
					msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21, -1)			--ȡ����ť
					--msg.AddMsgEvent(MsgID, 1, 206556)		--������һ���ٻ�������û�е���30����
					msg.AddMsgEvent(MsgID, 1, 206565)		--������һ���ٻ�������û�е���1Сʱ
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				if TalkIndex == -1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206557)		--������������\n ��ȷ��Ҫ�ٻ�����ô����
					msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21, 4)			--ȡ����ť
					msg.AddMsgEvent(MsgID, 1, 100001)		--ȷ��
					msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
					msg.AddMsgEvent(MsgID, 1, 100002)		--ȡ��
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				elseif TalkIndex == 4 then
					if Num < 10 then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 206566)		--������������\n    ����
						msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
						msg.AddMsgEvent(MsgID, 21, 6)			--ȡ����ť
						msg.AddMsgEvent(MsgID, 1, 206554)		--�����ϵ�����֮����������10�����޷��ٻ�������
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					else
						if touming == 0 then
							role.RemoveFromRole(MapID, InstanceID, RoleID, 4800073, 10, 101)---------����֮��
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 102, 300041)    --[<p1>]������10������֮�ǣ�2����֮����Լ�������ڲ����������г���
							msg.AddMsgEvent(MsgID, 2, RoleID)
							msg.DispatchWorldMsgEvent(MsgID)
							touming = map.MapCreateCreature(MapID, InstanceID, 6030012, 260, 2999, 79)
							local CurTime = tonumber(os.date("%j"))--����
							local cTime = os.time()
							map.SetMapScriptData(MapID, InstanceID, 1, 2, cTime)
							map.SetMapScriptData(MapID, InstanceID, 1, 3, CurTime)
						end
					end
				end
			end
		end
	end
end
aux.RegisterCreatureEvent(5600028, 7, "Mrbobo_OnTalk")


function Qyaojing_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,1)
	local b = math.random(1,3)
	if b == 1 then
		map.MapCreateCreature(MapID, InstanceID, 5600054, 260, 3130, 350)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 300042)    --���˽ڵ����������Ѿ������ڲ���������<260,350>��
		msg.DispatchWorldMsgEvent(MsgID)
		map.SetMapScriptData(MapID, InstanceID, 1, 1, a+1)
		touming = 0
	elseif b == 2 then
		map.MapCreateCreature(MapID, InstanceID, 5600054, 128, 3117, 199)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 300043)    --���˽ڵ����������Ѿ������ڲ���������<128,199>��
		msg.DispatchWorldMsgEvent(MsgID)
		map.SetMapScriptData(MapID, InstanceID, 1, 1, a+1)
		touming = 0
	elseif b == 3 then
		map.MapCreateCreature(MapID, InstanceID, 5600054, 390, 2981, 238)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 300044)    --���˽ڵ����������Ѿ������ڲ���������<390,238>��
		msg.DispatchWorldMsgEvent(MsgID)
		map.SetMapScriptData(MapID, InstanceID, 1, 1, a+1)
		touming = 0
	end

end
aux.RegisterCreatureEvent(6030012,13,"Qyaojing_OnDie")
