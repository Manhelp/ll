


--ʥ���������¼�
function C6030001_OnInVest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	--�������Ƿ���ʥ���࣬�������۳�

	local monsterHP = cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0)

	local nRet = 0
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(RoleID)
	if FreeSize > 0 then
		nRet = role.RemoveFromRole(MapID, InstanceID, RoleID, 4800024, 1, 1000)
		if 0 == nRet then
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			local i = math.random(100)

			if i <= 5 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 4800004, 1, -1, 8, 102)
			elseif i > 5 and i<=25 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 4800008, 1, -1, 8, 102)
			elseif i>=50 and i<54 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 4800240, 1, -1, 8, 102)
			elseif i>=97 and i<98 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 4800003, 1, -1, 8, 102)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 300024)  --[<P1>]��ʥ�����»����һ�����������ѩ֮�꣬�������ˣ�
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.DispatchBroadcast(MsgID, -1, -1, -1)
			else
				if level >= 30 and level <= 34 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 15000)
				elseif level > 34 and level <= 39 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 15000)
				elseif level >= 40 and level <= 49 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 30000)
				elseif level >= 50 and level <= 59 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 55000)
				elseif level >= 60 and level <= 69 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 80000)
				elseif level >= 70 and level <= 79 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 100000)
				elseif level >= 80 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 120000)
				end
			end



			monsterHP=monsterHP+1
			cre.SetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0, monsterHP)

			if monsterHP%300 == 0 then
				unit.AddBuff(MapID, InstanceID, CreatureID, 9004401, CreatureID)--������СBUFF���
				local MsgID = msg.BeginMsgEvent();
				msg.AddMsgEvent(MsgID,102, 300025)--���ؾ�Ȼʥ������С�ˣ��������������ҲԽ��Խ���ˣ��ٲ���ʥ�����ϵ������Ҫ������������ѽ��
				msg.DispatchBroadcast(MsgID, -1, -1, -1)
			end
			if monsterHP == 1000 then
			unit.CancelBuff(MapID, InstanceID, CreatureID, 6936601)
			unit.CancelBuff(MapID, InstanceID, CreatureID, 6936801)
			end
		else
			unit.ChangeHP(MapID, InstanceID, CreatureID, 1)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 206316)--��ʾû��ʥ���࣬�޷�����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)--��Ҫ�����ռ�
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(6030001, 6, "C6030001_OnInVest")






function c6030001_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

			local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
			yaojing = map.MapCreateCreature(MapID, InstanceID, 1092011, x+1, y, z+1)
			unit.AddBuff(MapID, InstanceID, yaojing, 5932801, yaojing)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 300026)    --����ʥ������ʧ�ˣ�ʥ������������Ѥ���ǳ���3���Ӻ󼴿ɻ�ɱ��ͨ����Լ�ɻ������ʥ��������
			msg.DispatchWorldMsgEvent(MsgID)


end

function c1092011_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

			local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
			map.MapCreateCreature(MapID, InstanceID, 6010071, x+1, y, z+1)
			map.MapCreateCreature(3424073512, InstanceID, 5600020, 627, 23510, 585)
			local MsgID1 = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID1, 102, 300027)  --ʥ�������Ѿ������ڶ�ս��ǵ�<585��600>������ǰȥ��ȡʥ��������ף����������ף����ʹ��þ�������1.5��
			msg.DispatchWorldMsgEvent(MsgID1)
end

aux.RegisterCreatureEvent(6030001,4,"c6030001_OnDie")
aux.RegisterCreatureEvent(1092011,4,"c1092011_OnDie")
