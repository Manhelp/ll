----------------------------------------------------------------------------------
--����Ƿ���װ������ͷ�	0 -- �޳ͷ�  1 -- �гͷ�
function r_RoleIsEquipLootPenalty(MapID, InstanceID, RoleID)
	if role.IsRoleInStatus(ERS_HoldChest) then
		return false
	end
	if MapID == 3618849499 then--test02
		return true
	elseif MapID == 3424073512 then--s02
		return true
	elseif MapID == 3424073256 then--s03
		return true
	elseif MapID == 3424071976 then--s04
		return true
	elseif MapID == 3424071720 then--s05
		return true
	elseif MapID == 3424072488 then--s06
		return true
	elseif MapID == 3424072232 then--s07
		return true
	elseif MapID == 3424075048 then--s08
		return true
	end

	return false
end

----------------------------------------------------------------------------------
--����Ƿ��о���ͷ�	0 -- �޳ͷ�  1 -- �гͷ�
function r_RoleIsExpPenalty(MapID, InstanceID, RoleID)
	if role.IsRoleInStatus(ERS_HoldChest) then
		return false
	end
	if MapID == 3618849499 then--test02
		return true
	elseif MapID == 3424073512 then--s02
		return true
	elseif MapID == 3424073256 then--s03
		return true
	elseif MapID == 3424071976 then--s04
		return true
	elseif MapID == 3424071720 then--s05
		return true
	elseif MapID == 3424072488 then--s06
		return true
	elseif MapID == 3424072232 then--s07
		return true
	elseif MapID == 3424075048 then--s08
		return true
	end
	return false
end
----------------------------------------------------------------------------------
--����Ƿ������˳ͷ�	0 -- �޳ͷ�  1 -- �гͷ�
function r_RoleIsInjuryPenalty(MapID, InstanceID, RoleID)
	if role.IsRoleInStatus(ERS_HoldChest) then
		return false
	end

	-- ս����û�����˳ͷ�
	if MapID == MapID_FX or MapID == MapID_ZL or MapID == MapID_WS then
		return false
	end

	if MapID == 3618849499 or MapID == 2092183860 or MapID == 2092184116 or MapID == 2092184372 or MapID == S59_MapID then--test02
		return false
	--[[elseif MapID == 3017298383 or MapID == 3017299663 or MapID == 3017299919 or  MapID == 3017299151 or  MapID == 3017299407 then
		local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
		if godCurState == 2 then
			if  map.IsInArea(MapID, InstanceID, RoleID, GetAreaID(MapID)) ~= 0 then
				local GuildID = guild.GetRoleGuildID(RoleID)
				if GuildID ~= 4294967295 then
					if GetDefendGuildID(MapID) == GuildID or GetCaptureGuildID(MapID) == GuildID then
						return false
					end
				end
			end
		else
			return true
		end]]
	end
	return true
end


----------------------------------------------------------------------------------
--����Ƿ��е��³ͷ�	0 -- �޳ͷ�  1 -- �гͷ�
function r_RoleIsMoralityPenalty(MapID, InstanceID, RoleID)
	if role.IsRoleInStatus(ERS_HoldChest) then
		return false
	end

	-- ս����û�����˳ͷ�
	if MapID == MapID_FX or MapID == MapID_ZL or MapID == MapID_WS or MapID == S59_MapID then
		return false
	end

	if MapID == 3618849499 then--test02
		return true
	--[[elseif MapID == 3017298383 or MapID == 3017299663 or MapID == 3017299919 or MapID == 3017299151 or MapID == 3017299407 then
		local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
		if godCurState == 2 then
			if  map.IsInArea(MapID, InstanceID, RoleID, GetAreaID(MapID)) ~= 0 then
				local GuildID = guild.GetRoleGuildID(RoleID)
				if GuildID ~= 4294967295 then
					if GetDefendGuildID(MapID) == GuildID or GetCaptureGuildID(MapID) == GuildID then
						return false
					end
				end
			end
		else
			return true
		end]]
	end
	if MapID == 2766543506 or MapID == 2766543250 or MapID == 2766542994 or MapID == 2092183860 or MapID == 2092184116 or MapID == 2092184372 then
	    return false
	end

	return true
end

-- breif: ��������,����������қ�����ֵ����
function IsMinusHostilityPenalty(MapID, RoleID)
	if  map.IsInArea(MapID, InstanceID, RoleID, GetAreaID(MapID)) ~= 0 then
		local GuildID = guild.GetRoleGuildID(RoleID)
		if GuildID ~= 4294967295 then
			if IsInGodCaptureGuild(GuildID) == 1 then
				return false
			end
		end
	end
	return true
end

----------------------------------------------------------------------------------
--����Ƿ�������ֵ�ͷ�	0 -- �޳ͷ�  1 -- �гͷ�
function r_RoleIsHostilityPenalty(MapID, InstanceID, RoleID)
	return false
end
----------------------------------------------------------------------------------
--����Ƿ������������ͷ�	0 -- �޳ͷ�  1 -- �гͷ�
function r_RoleIsCommercePenalty(MapID, InstanceID, RoleID)
	return false
end
--����Ƿ�����������ͷ�	����ֵ 1 - װ������  2 - ����  3 - ����  4 -  ����  5 - ��������
function r_RoleCanCancelPenalty(MapID, InstanceID, RoleID, KillerID)

	if role.IsRoleInStatus(ERS_HoldChest) then
		return true, true, true, true, true
	end

	local morality = role.GetRoleAttValue(MapID, InstanceID, RoleID, 74)
	-- ��ҵ���ֵС�ڵ���-30���ұ������������ʱ���������װ������ͷ�
	if morality <= -30 then
                --����ʹ�ð󶨵���
		local num = role.GetRoleItemNum(RoleID, 3070020)
		if num > 0 then

			role.RemoveFromRole(MapID, InstanceID, RoleID, 3070020, 1, 300)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 11001)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return true, false, false, false, false

                else
                        local num_unbind = role.GetRoleItemNum(RoleID, 3070021)

                        if num_unbind > 0 then

   				role.RemoveFromRole(MapID, InstanceID, RoleID, 3070021, 1, 300)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 11001)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return true, false, false, false, false
                        end

                end
	end

	return false, false, false, false, false

end

----------------------------------------------------------------------------------
--����Ƿ���������	0 -- �޳ͷ�  1 -- �гͷ�
function r_RoleWillLoseField(MapID, InstanceID, RoleID)
	if role.IsRoleInStatus(ERS_HoldChest) then
		return false
	end

	-- ս����û�����˳ͷ�
	if MapID == MapID_FX or MapID == MapID_ZL or MapID == MapID_WS or MapID == S59_MapID then
		return false
	end

	if MapID == 3618849499 or MapID == 2092183860 or MapID == 2092184116 or MapID == 2092184372 then--test02
		return false

	end
	return true
end


----------------------------------------------------------------------------------
aux.RegisterRoleEvent(15, "r_RoleIsEquipLootPenalty")
aux.RegisterRoleEvent(16, "r_RoleIsExpPenalty")
aux.RegisterRoleEvent(17, "r_RoleIsInjuryPenalty")
aux.RegisterRoleEvent(18, "r_RoleIsMoralityPenalty")
aux.RegisterRoleEvent(22, "r_RoleCanCancelPenalty")
aux.RegisterRoleEvent(66, "r_RoleWillLoseField")
