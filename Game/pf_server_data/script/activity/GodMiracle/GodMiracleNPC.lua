function GodMiracle_On_Can_Buy(MapID, InstanceID, ShopID, RoleID, ItemID)

	local MasterGuildID = GetGuildByGodMiracle(MapID)

	local GuildID = guild.GetRoleGuildID(RoleID)

	if MasterGuildID~=GuildID then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1601)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end

	for i=1,8 do
		if ItemID == 3200818+i then

			local Role_Expoit = role.GetGuildExploit(RoleID)
			local Exploit_Needed = {}
			Exploit_Needed[3200819]=360
			Exploit_Needed[3200820]=360
			Exploit_Needed[3200821]=1080
			Exploit_Needed[3200822]=1080
			Exploit_Needed[3200823]=3240
			Exploit_Needed[3200824]=3240
			Exploit_Needed[3200825]=16200
			Exploit_Needed[3200826]=16200

			if Role_Expoit<Exploit_Needed[ItemID] then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1603)
				msg.AddMsgEvent(MsgID, 9, Exploit_Needed[ItemID])
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return 0
			end
		end
	end

	return 1

end

function SendConfirmMsg(MsgContentID, RoleID, TargetID)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 20, MsgContentID)
	msg.AddMsgEvent(MsgID, 21, 5)
	msg.AddMsgEvent(MsgID, 1, 100001)
	msg.AddMsgEvent(MsgID, 24, TargetID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end

function SendYesNoMsg(MsgContentID, RoleID, TargetID)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 20, MsgContentID)
	msg.AddMsgEvent(MsgID, 21, 4)
	msg.AddMsgEvent(MsgID, 1, 100001)
	msg.AddMsgEvent(MsgID, 21, 5)
	msg.AddMsgEvent(MsgID, 1, 100002)
	msg.AddMsgEvent(MsgID, 24, TargetID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end

function CheckEnterBattleField(godMiracleID, guildID, RoleID, TargetID)
	local stepindex = 0
	--ȡ������Ӫ���ͣ�1��ʾ���أ�2��ʾ������0��ʾû�в�սȨ
	local guildAlliance = guild.GetGuildAllianceType(godMiracleID, guildID)
	if guildAlliance == nil then
		SendConfirmMsg(303005, RoleID, TargetID)				--��ʾ�����Ĺ���û�б���ս���Ĳ���Ȩ��Ŀǰ�޷����롱
	else
		if guildAlliance == 1 or guildAlliance == 2 then		--���ڲ�ս����
			SendYesNoMsg(303003, RoleID, TargetID)				--�Ƿ��������ڣս����
			stepindex = 3
		else
			SendConfirmMsg(303005, RoleID, TargetID)			--��ʾ�����Ĺ���û�б���ս���Ĳ���Ȩ��Ŀǰ�޷����롱
			stepindex = 0
		end
	end
	return stepindex;
end

function BattleFieldTransmitor_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	--����ǵ�һ�ζԻ�
	if TalkIndex == -1 then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["BattleField_TalkIndex"], 1)
	end

	local stepindex = role.GetRoleScriptData(RoleID, 1, RoleDataType["BattleField_TalkIndex"])
	local godMiracleID = GodMiracleTypeID[TargetTypeID].GodMiracleTypeID

	if stepindex == 1 then
		--���жϽ�ɫ�Ƿ�����ĳһ������
		local guildID = guild.GetRoleGuildID(RoleID)
		if guildID == nil or guildID == 4294967295 then
			SendConfirmMsg(303001, RoleID, TargetID)					--���������κι��ᣬ���ܽ���ս����ֻ�й����Ա���ܽ���ս��
			stepindex = 0
		else
			local godCurState = guild.GetGodMiracleCurState(godMiracleID)
			if godCurState == 0 then									--δ��ռ��
				SendYesNoMsg(303002, RoleID, TargetID)					--����ڣս������δ��ռ��״̬���Ƿ���룿
				stepindex = 2
			elseif godCurState == 1 then								--��ս	
				local WeekDay = os.date("%A")
				local Hour = tonumber(os.date("%H"))
				local Min  = tonumber(os.date("%M"))
				
				--ʱ��Ϊ����19��50֮�����в�ս��Ա�ɽ�������������Ҷ�������ս����
				if WeekDay=="Saturday" and Hour == 19 and Min >= 50 then -- and broadCastGlag.flag2 == 1
					--�����Ӫ������ս��
					stepindex = CheckEnterBattleField(godMiracleID, guildID, RoleID, TargetID)
				else															--ʱ��Ϊ����19��50֮ǰ��ֻ��ռ�칫���Ա�ɽ�
				    local defendGuildID = guild.GetDefendGuildID(godMiracleID)
					if guildID == defendGuildID then		                    --������ռ�칫��
						SendYesNoMsg(303003, RoleID, TargetID)					--�Ƿ��������ڣս����
						stepindex = 2
					else
						SendConfirmMsg(303004, RoleID, TargetID)				--��ʾ�����Ĺ���û��ռ���ս����Ŀǰ�޷����롱
						stepindex = 0
					end
				end
			elseif godCurState == 2 then										--������
				stepindex = CheckEnterBattleField(godMiracleID, guildID, RoleID, TargetID)	--�����Ӫ������ս��
			end
		end

	--�����˶����͵��̶��ص�
	elseif stepindex == 2 then
		if TalkIndex ~= 4 then
			return
		end

		local DestMapID = GodMiracleTypeID[TargetTypeID].MapID;

		role.RoleGotoNewMap(MapID, InstanceID, RoleID, DestMapID, RoleEnter_Pos[DestMapID].x2, RoleEnter_Pos[DestMapID].y2, RoleEnter_Pos[DestMapID].z2)

		stepindex = 0

	--������Ӫ���͵�������ͬ�ĵص�
	elseif stepindex == 3 then
		if TalkIndex ~= 4 then
			return
		end

		local guildID = guild.GetRoleGuildID(RoleID);
		if guildID ~= nil and guildID ~= -1 then

			--ȡ������Ӫ���ͣ�1��ʾ���أ�2��ʾ������0��ʾû�в�սȨ
			local guildAlliance = guild.GetGuildAllianceType(godMiracleID, guildID)
			if guildAlliance ~= nil then
				--ȡҪ���͵��ĵ�ͼID
				local DestMapID = GodMiracleTypeID[TargetTypeID].MapID;
				if guildAlliance == 1 then			--�������ڷ�����Ӫ
					role.RoleGotoNewMap(MapID, InstanceID, RoleID, DestMapID, RoleEnter_Pos[DestMapID].x2, RoleEnter_Pos[DestMapID].y2, RoleEnter_Pos[DestMapID].z2)
				elseif guildAlliance == 2 then		--�������ڹ�����Ӫ
					role.RoleGotoNewMap(MapID, InstanceID, RoleID, DestMapID, RoleEnter_Pos[DestMapID].x1, RoleEnter_Pos[DestMapID].y1, RoleEnter_Pos[DestMapID].z1)
				elseif guildAlliance == 0 then		--����û�в�սȨ
					SendConfirmMsg(602004, RoleID, TargetID)					--��ʾ�����Ĺ���û�б���ս���Ĳ���Ȩ��Ŀǰ�޷����롱
				end
			end
			stepindex = 0
		end
	end

	role.SetRoleScriptData(RoleID, 1, RoleDataType["BattleField_TalkIndex"], stepindex)

end

function DZTC_Transmitor_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		SendYesNoMsg(303006, RoleID, TargetID)		--�����Ƿ�Ҫǰ����ս��ǣ���
	elseif TalkIndex == 4 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, MapID_S02, S02_Trans_Pos.x, S02_Trans_Pos.y, S02_Trans_Pos.z)
	end
end

aux.RegisterCreatureEvent(4900504, 16, "GodMiracle_On_Can_Buy")
aux.RegisterCreatureEvent(4900505, 16, "GodMiracle_On_Can_Buy")
aux.RegisterCreatureEvent(4900506, 16, "GodMiracle_On_Can_Buy")
aux.RegisterCreatureEvent(4900507, 16, "GodMiracle_On_Can_Buy")
aux.RegisterCreatureEvent(4900508, 16, "GodMiracle_On_Can_Buy")

aux.RegisterCreatureEvent(5010110, 7, "BattleFieldTransmitor_OnTalk")		-- ����ڣս��ս��������
aux.RegisterCreatureEvent(5010054, 7, "DZTC_Transmitor_OnTalk")			-- ��ս��Ǵ�����
