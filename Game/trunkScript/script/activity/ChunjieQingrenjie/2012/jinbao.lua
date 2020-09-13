--ҡǮ����ÿ�������Ӻ����Ի�һ�Σ����ӡ����硢�����������������5�֣������ʱ��ˢ���Ʊ�����
function i4830012_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false

	--�������s03����������ʹ��
	if MapID ~= 3424073256 then
	    bRet = 43--�����ڸ�λ��ʹ�ø���Ʒ
	else
		local m = map.GetMapScriptData(MapID,InstanceID,1,1)
		if m == nil then
			m = 0
		end
		if m > 150 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 444401)--ҡǮ���������Ѵﵽ���ޣ���һ��ʱ��������ֲ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			bRet = 102
		end
	end
	--����
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830012, 0, "i4830012_CanUse")

function i4830012_OnUse(MapID, InstanceID, TypeID, RoleID)
	local m = map.GetMapScriptData(MapID,InstanceID,1,1)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	local creatureID = map.MapCreateCreature(MapID, InstanceID, 6040017, x, y, z)--ҡǮ������
	cre.SetCreatureScriptData(MapID, InstanceID, creatureID, 1, 1, RoleID)--��¼��ֲ
	map.SetMapScriptData(MapID,InstanceID,1,1,m+1)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 1, 444402)--ҡǮ�������Ѿ������������Ի���֪����ʱ�չ�����
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
aux.RegisterItemEvent(4830012, 1, "i4830012_OnUse")




function n6040017_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local m = map.GetMapScriptData(MapID,InstanceID,1,1)
	map.SetMapScriptData(MapID,InstanceID,1,1,m-1)

end
aux.RegisterCreatureEvent(6040017, 13, "n6040017_OnDie")


function n6040017_OnIalk0(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local m = map.GetMapScriptData(MapID,InstanceID,1,1)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--������ID
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if RoleID == seedowner then
		if TalkIndex == -1 then
		    if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444457)	-- 2����֮���Ҿͳ����ˣ���ʱ����Ҫ������չ���һ�¡�
				msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 405014)	-- "�õģ���֪���ˡ�"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
		    else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444462)	-- �����������ˣ���Ҫ��2���Ӳ��ܳ����أ���Ҫ̫�ż�����
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
				if s > 60 then  --�������1����
					map.MapDeleteCreature(MapID, InstanceID, TargetID)
					local creatureID1 = map.MapCreateCreature(MapID, InstanceID, 6040026, x, y, z)--ҡǮ������
					cre.SetCreatureScriptData(MapID, InstanceID, creatureID1, 1, 1, RoleID)
					map.SetMapScriptData(MapID,InstanceID,1,1,m-1)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 444458)--�����ĵȴ���2���ӻ�û�е���
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 1, 405013)--�����������ģ����ܶԻ�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(6040017, 7, "n6040017_OnIalk0")

function n6040026_OnIalk1(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--������ID
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if RoleID == seedowner then
		if TalkIndex == -1 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444459)	-- �ܲ����ײų��󰡡�2����֮���ҾͿ��Կ����ˣ�������ֻ�ܴ�������ӣ���ʱ�ǵù������ҡ�
				msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 405014)	-- "�õģ���֪���ˡ�"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444462)	-- �����������ˣ���Ҫ��2���Ӳ��ܳ����أ���Ҫ̫�ż�����
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
				if s > 60 then  --�������1����
					map.MapDeleteCreature(MapID, InstanceID, TargetID)
					local creatureID2 = map.MapCreateCreature(MapID, InstanceID, 6040027, x, y, z)--ҡǮ������
					cre.SetCreatureScriptData(MapID, InstanceID, creatureID2, 1, 1, RoleID)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 1, 444458)--��û�е������ӣ����ܺ�ҡǮ���Ի�
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 405013)--�����������ģ����ܶԻ�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end
aux.RegisterCreatureEvent(6040026, 7, "n6040026_OnIalk1")

function n6040027_OnIalk2(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--������ID
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if RoleID == seedowner then
		if TalkIndex == -1 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444460)	-- �������ˣ���ô��������2���Ӻ�Ϳ��Խ���ˣ���ʱ������ժŶ��
				msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 405014)	-- "�õģ���֪���ˡ�"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444462)	-- �����������ˣ���Ҫ��2���Ӳ��ܳ����أ���Ҫ̫�ż�����
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
				if s > 60 then  --�������1����
					map.MapDeleteCreature(MapID, InstanceID, TargetID)
					local creatureID3 = map.MapCreateCreature(MapID, InstanceID, 6040028, x, y, z)--ҡǮ�����
					cre.SetCreatureScriptData(MapID, InstanceID, creatureID3, 1, 1, RoleID)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 1, 444458)--��û�е������ӣ����ܺ�ҡǮ���Ի�
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 1, 405013)--�����������ģ����ܶԻ�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(6040027, 7, "n6040027_OnIalk2")

function n6040028_OnIalk3(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--������ID
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if RoleID == seedowner then
		if TalkIndex == -1 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444461)	-- ������ժ�ˣ�������֪����ʲôԭ����2���Ӻ��ٹ��������ɡ���Ҫ��ס��ֻ�ܴ�������ӣ�
				msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 405014)	-- "�õģ���֪���ˡ�"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444462)	-- �����������ˣ���Ҫ��2���Ӳ��ܳ����أ���Ҫ̫�ż�����
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
				if s > 60 then  --�������1����--60153-60157--60158-60162--60163-60167
					for i=0,3 do
						if role.IsRoleHaveQuest(MapID, InstanceID, RoleID,30863+i) == true then
							map.MapCreateCreature(MapID, InstanceID,3040339+i,x, y, z)
							map.MapDeleteCreature(MapID, InstanceID, TargetID)
							break
						end
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 1, 444458)--��û�е������ӣ����ܺ�ҡǮ���Ի�
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 1, 405013)--�����������ģ����ܶԻ�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(6040028, 7, "n6040028_OnIalk3")
