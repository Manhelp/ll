
--����ҩˮ
function I3010043_onUse(MapID, InstanceID, TypeID, TargetID)
--�õ���ҵȼ�
	local n = role.GetRoleLevel( MapID , InstanceID , TargetID)
	if n > 100 then
		n = 100
	end
--Ϊ�����Ӷ�Ӧbuff
	unit.AddBuff( MapID , InstanceID , TargetID , 2004101+100*(n-1) , TargetID )
end

aux.RegisterItemEvent(3010043, 1 , "I3010043_onUse")


--������ҩˮ
function I3010044_onUse(MapID, InstanceID, TypeID, TargetID)
--�õ���ҵȼ�
	local n = role.GetRoleLevel( MapID , InstanceID , TargetID)
	if n > 100 then
		n = 100
	end
--Ϊ�����Ӷ�Ӧbuff
	unit.AddBuff( MapID , InstanceID , TargetID , 2014101+100*(n-1) , TargetID )
end

aux.RegisterItemEvent(3010044, 1 , "I3010044_onUse")


--��������ҩˮ
function I3010045_onUse(MapID, InstanceID, TypeID, TargetID)
--�õ���ҵȼ�
	local n = role.GetRoleLevel( MapID , InstanceID , TargetID)
	if n > 100 then
		n = 100
	end
--Ϊ�����Ӷ�Ӧbuff
	unit.AddBuff( MapID , InstanceID , TargetID , 2024101+100*(n-1) , TargetID )
end

aux.RegisterItemEvent(3010045, 1 , "I3010045_onUse")


--��������ҩˮ
function I3010046_onUse(MapID, InstanceID, TypeID, TargetID)
--�õ���ҵȼ�
	local n = role.GetRoleLevel( MapID , InstanceID , TargetID)
	if n > 100 then
		n = 100
	end
--Ϊ�����Ӷ�Ӧbuff
	unit.AddBuff( MapID , InstanceID , TargetID , 2034101+100*(n-1) , TargetID )
end

aux.RegisterItemEvent(3010046, 1 , "I3010046_onUse")
