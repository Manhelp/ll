--���鼼���ж�
function S9001001_CanUsediaocha(MapID, InstanceID, SkillID , RoleID, TargetID)
	local bRet, bIgnore = 0, false
	local temp = table.getn(tianjiebaozang)
	if(temp ~= nil and temp > 0) then
		for i=1, temp do  --�ж��Ƿ�����籦��
			if(tianjiebaozang[i] == TargetID) then
				local lastDate = role.GetRoleScriptData(RoleID, 1, RoleDataType["zuihoudezhanyi"])
				local theDays = tonumber(os.date("%j"))
				if lastDate ~= nil and theDays == lastDate then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 207001)--�������Ѿ�������һ����籦���ˣ�
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					bRet = 999
				end
			end
		end
	end
	return bRet, bIgnore
end

aux.RegisterSkillEvent(9001001, 0, "S9001001_CanUsediaocha")



