--[[function Buff_OnDestroy ( MapID, InstanceID, RoleID, BuffID )	--buff��ʧ

--��Сʱ��������һ��buff��ʧ����ӵڶ���buff
	if BuffID == 9700101 then
		unit.AddBuff(MapID, InstanceID, RoleID, 9700201, RoleID)

--��Сʱ�������ڶ���buff��ʧ����ӵ�����buff
	elseif BuffID == 9700201 then
		unit.AddBuff(MapID, InstanceID, RoleID, 9700301, RoleID)
------������Сʱ����
		role.BaibaoAddRoleItem(MapID, InstanceID, RoleID, 6020001, 2, -1, 12, 1200)
--��Сʱ������������buff��ʧ����ӵ��ĸ�buff
	elseif BuffID == 9700301 then
		unit.AddBuff(MapID, InstanceID, RoleID, 9700401, RoleID)
------������Сʱ����
		role.BaibaoAddRoleItem(MapID, InstanceID, RoleID, 3070010, 1, -1, 12, 1200)
	end
end

aux.RegisterBuffEvent( 0, "Buff_OnDestroy" )]]
