
function IShengChan_Use(MapID, InstanceID, TypeID, TargetID)
	if TypeID >= 3050062 and TypeID <= 3050064 then
		role.ChangeSkillExp(TargetID, 91000+TypeID-3050061, 200)
	elseif TypeID >= 3050065 and TypeID <= 3050067 then
		role.ChangeSkillExp(TargetID, 91000+TypeID-3050064, 800)
	elseif TypeID >= 3050068 and TypeID <= 3050070 then
		role.ChangeSkillExp(TargetID, 91000+TypeID-3050067, 1800)
	end
end
aux.RegisterItemEvent(3050062, 1, "IShengChan_Use")
aux.RegisterItemEvent(3050063, 1, "IShengChan_Use")
aux.RegisterItemEvent(3050064, 1, "IShengChan_Use")
aux.RegisterItemEvent(3050065, 1, "IShengChan_Use")
aux.RegisterItemEvent(3050066, 1, "IShengChan_Use")
aux.RegisterItemEvent(3050067, 1, "IShengChan_Use")
aux.RegisterItemEvent(3050068, 1, "IShengChan_Use")
aux.RegisterItemEvent(3050069, 1, "IShengChan_Use")
aux.RegisterItemEvent(3050070, 1, "IShengChan_Use")

--[[
3050062	�����ӹ��ĵ�	ʹ�ú�����Ӽӹ�����������200
3050063	���������ĵ�	ʹ�ú���������켼��������200
3050064	�����÷��ĵ�	ʹ�ú�����Ӳ÷켼��������200
3050065	�м��ӹ��ĵ�	ʹ�ú�����Ӽӹ�����������800
3050066	�м������ĵ�	ʹ�ú���������켼��������800
3050067	�м��÷��ĵ�	ʹ�ú�����Ӳ÷켼��������800
3050068	�߼��ӹ��ĵ�	ʹ�ú�����Ӽӹ�����������1800
3050069	�߼������ĵ�	ʹ�ú���������켼��������1800
3050070	�߼��÷��ĵ�	ʹ�ú�����Ӳ÷켼��������1800
skillid
�ӹ� 91001
���� 91002
�÷� 91003
]]
function IShengChan_CanUse(MapID, InstanceID, TypeID, TargetID)
    local ID = 0
    if TypeID >= 3050062 and TypeID <= 3050064 then
		ID = 91000+TypeID-3050061
	elseif TypeID >= 3050065 and TypeID <= 3050067 then
		ID = 91000+TypeID-3050064
	elseif TypeID >= 3050068 and TypeID <= 3050070 then
		ID = 91000+TypeID-3050067
	end
    if role.IsLearnedSkill(TargetID,ID) ~= nil and role.IsLearnedSkill(TargetID,ID) >= 5  then
		return 101,false
	else
	    return 0,false
	end
end
aux.RegisterItemEvent(3050062, 0, "IShengChan_CanUse")
aux.RegisterItemEvent(3050063, 0, "IShengChan_CanUse")
aux.RegisterItemEvent(3050064, 0, "IShengChan_CanUse")
aux.RegisterItemEvent(3050065, 0, "IShengChan_CanUse")
aux.RegisterItemEvent(3050066, 0, "IShengChan_CanUse")
aux.RegisterItemEvent(3050067, 0, "IShengChan_CanUse")
aux.RegisterItemEvent(3050068, 0, "IShengChan_CanUse")
aux.RegisterItemEvent(3050069, 0, "IShengChan_CanUse")
aux.RegisterItemEvent(3050070, 0, "IShengChan_CanUse")





--�����ĵð�
function IShengChanbangding_Use(MapID, InstanceID, TypeID, TargetID)
	role.ChangeSkillExp(TargetID, 91000+TypeID-3051606, 200)
end
aux.RegisterItemEvent(3051607, 1, "IShengChanbangding_Use")
aux.RegisterItemEvent(3051608, 1, "IShengChanbangding_Use")
aux.RegisterItemEvent(3051609, 1, "IShengChanbangding_Use")

--[[
3051607	�����ӹ��ĵ�	ʹ�ú�����Ӽӹ�����������200
3051607	���������ĵ�	ʹ�ú���������켼��������200
3051607	�����÷��ĵ�	ʹ�ú�����Ӳ÷켼��������200
skillid
�ӹ� 91001
���� 91002
�÷� 91003
]]
function IShengChanbangding_CanUse(MapID, InstanceID, TypeID, TargetID)
    local ID = 0
	ID = 91000+TypeID-3051606
    if role.IsLearnedSkill(TargetID,ID) ~= nil and role.IsLearnedSkill(TargetID,ID) >= 5  then
		return 101,false
	else
	    return 0,false
	end
end
aux.RegisterItemEvent(3051607, 0, "IShengChanbangding_CanUse")
aux.RegisterItemEvent(3051608, 0, "IShengChanbangding_CanUse")
aux.RegisterItemEvent(3051609, 0, "IShengChanbangding_CanUse")

