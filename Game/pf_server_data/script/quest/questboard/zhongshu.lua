--ħ����ÿ�������Ӻ����Ի�һ�Σ����ӡ����硢�����������������5�֣������ʱ��ˢ����ħ

function zhongshu_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	local MsgID = msg.BeginMsgEvent()
	--�������s06����������ʹ��
	if MapID ~= 3424072488 then
	    bRet = 43
	else
		local m = map.GetMapScriptData(MapID,InstanceID,1,1)
		if m == nil then
			m = 0
		end
		if m > 150 then
			msg.AddMsgEvent(MsgID, 13, 0)
			msg.AddMsgEvent(MsgID, 1, 405024)--ħ���������Ѵﵽ���ޣ���һ��ʱ��������ֲ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			bRet = 32
		end

	end
	--����
	return bRet, bIgnore
end
aux.RegisterItemEvent(4010209, 0, "zhongshu_CanUse")

function zhongshu_ONUse(MapID, InstanceID, TypeID, RoleID)
	local m = map.GetMapScriptData(MapID,InstanceID,1,1)
	local MsgID = msg.BeginMsgEvent()
	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	local creatureID = map.MapCreateCreature(MapID, InstanceID, 5511019, x, y, z)--ħ������
	cre.SetCreatureScriptData(MapID, InstanceID, creatureID, 1, 1, RoleID)--��¼��ֲ
	map.SetMapScriptData(MapID,InstanceID,1,1,m+1)
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 405010)--ħ�������Ѿ������������Ի���֪����ʱ�չ�����
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
aux.RegisterItemEvent(4010209, 1, "zhongshu_ONUse")




function zhongshu_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local m = map.GetMapScriptData(MapID,InstanceID,1,1)
	map.SetMapScriptData(MapID,InstanceID,1,1,m-1)

end
aux.RegisterCreatureEvent(5511019, 13, "zhongshu_OnDie")


function zhongshu_OnIalk0(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local m = map.GetMapScriptData(MapID,InstanceID,1,1)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--������ID
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local MsgID = msg.BeginMsgEvent()
	if RoleID == seedowner then
		if TalkIndex == -1 then
		    if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				msg.AddMsgEvent(MsgID, 20, 405011)	-- 2����֮���Ҿͳ����ˣ���ʱ����Ҫ������չ���һ�¡�
				msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 405014)	-- "�õģ���֪���ˡ�"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
		    else
				msg.AddMsgEvent(MsgID, 20, 405028)	-- �����������ˣ���Ҫ��2���Ӳ��ܳ����أ���Ҫ̫�ż�����
				msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 405029)	-- "�����ڳ�������"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
		    end

		elseif TalkIndex == 4 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				local lTime = os.time()
				cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3,lTime)
			else
				local cTime = os.time() --��ǰʱ��
				local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3)
				local s = os.difftime(cTime,lTime) --������һ�ζԻ�ʱ�䰴
				if s > 120 then  --�������2����
					map.MapDeleteCreature(MapID, InstanceID, TargetID)
					local creatureID1 = map.MapCreateCreature(MapID, InstanceID, 5511020, x, y, z)--ħ������
					cre.SetCreatureScriptData(MapID, InstanceID, creatureID1, 1, 1, RoleID)
					map.SetMapScriptData(MapID,InstanceID,1,1,m-1)
				else
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 405012)--�����ĵȴ���2���ӻ�û�е���
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 405013)--�����������ģ����ܶԻ�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(5511019, 7, "zhongshu_OnIalk0")

function zhongshu_OnIalk1(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--������ID
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local MsgID = msg.BeginMsgEvent()
	if RoleID == seedowner then
		if TalkIndex == -1 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				msg.AddMsgEvent(MsgID, 20, 405015)	-- �ܲ����ײų��󰡡�2����֮���ҾͿ��Կ����ˣ�������ֻ�ܴ�������ӣ���ʱ�ǵù������ҡ�
				msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 405014)	-- "�õģ���֪���ˡ�"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			 else
				msg.AddMsgEvent(MsgID, 20, 405028)	-- �����������ˣ���Ҫ��2���Ӳ��ܳ����أ���Ҫ̫�ż�����
				msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 405029)	-- "�����ڳ�������"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 4 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				local lTime = os.time()
				cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3,lTime)
			else
				local cTime = os.time() --��ǰʱ��
				local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3)
				local s = os.difftime(cTime,lTime) --������һ�ζԻ�ʱ�䰴
				if s > 120 then  --�������2����
					map.MapDeleteCreature(MapID, InstanceID, TargetID)
					local creatureID2 = map.MapCreateCreature(MapID, InstanceID, 5511021, x, y, z)--ħ������
					cre.SetCreatureScriptData(MapID, InstanceID, creatureID2, 1, 1, RoleID)
				else
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 405012)--��û�е������ӣ����ܺ�ħ���Ի�
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 405013)--�����������ģ����ܶԻ�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(5511020, 7, "zhongshu_OnIalk1")

function zhongshu_OnIalk2(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--������ID
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local MsgID = msg.BeginMsgEvent()
	if RoleID == seedowner then
		if TalkIndex == -1 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				msg.AddMsgEvent(MsgID, 20, 405016)	-- �������ˣ���ô��������2���Ӻ�Ϳ��Խ���ˣ���ʱ������ժŶ��
				msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 405014)	-- "�õģ���֪���ˡ�"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				msg.AddMsgEvent(MsgID, 20, 405028)	-- �����������ˣ���Ҫ��2���Ӳ��ܳ����أ���Ҫ̫�ż�����
				msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 405029)	-- "�����ڳ�������"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end

		elseif TalkIndex == 4 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				local lTime = os.time()
				cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3,lTime)
			else
				local cTime = os.time() --��ǰʱ��
				local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3)
				local s = os.difftime(cTime,lTime) --������һ�ζԻ�ʱ�䰴
				if s > 120 then  --�������2����
					map.MapDeleteCreature(MapID, InstanceID, TargetID)
					local creatureID3 = map.MapCreateCreature(MapID, InstanceID, 5511022, x, y, z)--ħ�����
					cre.SetCreatureScriptData(MapID, InstanceID, creatureID3, 1, 1, RoleID)
				else
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 405012)--��û�е������ӣ����ܺ�ħ���Ի�
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 405013)--�����������ģ����ܶԻ�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(5511021, 7, "zhongshu_OnIalk2")

function zhongshu_OnIalk3(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--������ID
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local MsgID = msg.BeginMsgEvent()
	if RoleID == seedowner then
		if TalkIndex == -1 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				msg.AddMsgEvent(MsgID, 20, 405017)	-- ������ժ�ˣ�������֪����ʲôԭ����2���Ӻ��ٹ��������ɡ���Ҫ��ס��ֻ�ܴ�������ӣ�
				msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 405014)	-- "�õģ���֪���ˡ�"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				msg.AddMsgEvent(MsgID, 20, 405028)	-- �����������ˣ���Ҫ��2���Ӳ��ܳ����أ���Ҫ̫�ż�����
				msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 405029)	-- "�����ڳ�������"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 4 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				local lTime = os.time()
				cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3,lTime)
			else
				local cTime = os.time() --��ǰʱ��
				local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3)
				local s = os.difftime(cTime,lTime) --������һ�ζԻ�ʱ�䰴
				if s > 120 then  --�������2����--60153-60157--60158-60162--60163-60167
						local a = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60153) 
						local b = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60158) 
						local c = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60163) 
						local d = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60154) 
						local e = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60159) 
						local f = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60164) 
						local g = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60155) 
						local h = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60160) 
						local i = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60165) 
						local j = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60156) 
						local k = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60161) 
						local l = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60166) 
						local m = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60157) 
						local n = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60162) 
						local o = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60167) 
						if a == true or b == true or c == true then
							map.MapCreateCreature(MapID, InstanceID, 1093001, x, y, z)--��ħ1093001�ڵ������ħ���Ի���ˢ����51����ͨ����
						end
						if d == true or e == true or f == true then
							map.MapCreateCreature(MapID, InstanceID, 1093002, x, y, z)--��ħ1093002	�ڵ������ħ���Ի���ˢ����54����ͨ����
						end
						if g == true or h == true or i == true then
							map.MapCreateCreature(MapID, InstanceID, 1093003, x, y, z)--��ħ1093003	�ڵ������ħ���Ի���ˢ����57����ͨ����
						end
						if j == true or k == true or l == true then
							map.MapCreateCreature(MapID, InstanceID, 1093004, x, y, z)--��ħ1093004	�ڵ������ħ���Ի���ˢ����60����ͨ����
						end
						if m == true or n == true or o == true then
							map.MapCreateCreature(MapID, InstanceID, 1093005, x, y, z)--��ħ1093005	�ڵ������ħ���Ի���ˢ����63����ͨ����
						end
						map.MapDeleteCreature(MapID, InstanceID, TargetID)
				else
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 405012)--��û�е������ӣ����ܺ�ħ���Ի�
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 405013)--�����������ģ����ܶԻ�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(5511022, 7, "zhongshu_OnIalk3")
