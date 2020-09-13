--1��Ա��������
function I4700038_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 19, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070058, 25, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700039, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3051607, 99, -1, 8, 420)  --�����ӹ��ĵã��󶨣�
	role.AddRoleItem(MapID, InstanceID, TargetID, 3051607, 99, -1, 8, 420)  --�����ӹ��ĵã��󶨣�
	role.AddRoleItem(MapID, InstanceID, TargetID, 3051608, 99, -1, 8, 420)  --���������ĵã��󶨣�
	role.AddRoleItem(MapID, InstanceID, TargetID, 3051608, 99, -1, 8, 420)  --���������ĵã��󶨣�
	role.AddRoleItem(MapID, InstanceID, TargetID, 3051609, 99, -1, 8, 420)  --�����÷��ĵã��󶨣�
	role.AddRoleItem(MapID, InstanceID, TargetID, 3051609, 99, -1, 8, 420)  --�����÷��ĵã��󶨣�
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050001, 100, -1, 8, 420)  --ԭ��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050002, 999, -1, 8, 420)  --����
end

function I4700038_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 11) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700038, 1, "I4700038_GiftBag")
aux.RegisterItemEvent(4700038, 0, "I4700038_CanUseGiftBag")

--30��Ա��������
function I4700039_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700040, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020116, 2, -1, 8, 420)   --��+10
	role.AddRoleItem(MapID, InstanceID, TargetID, 4222004, 1, -1, 8, 420)	--��è�ɳ�������
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211403, 1, -1, 8, 420)	--����
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070008, 1, -1, 8, 420)   --����ֿ�
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600007, 30, -1, 8, 420)	--��������������죩
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 999, -1, 8, 420)	--������
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070085, 1, -1, 8, 420)	--5000��ȯ
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)	--ս�����[����]
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070022, 20, -1, 8, 420)	--��������ҩ
	role.AddRoleItem(MapID, InstanceID, TargetID, 4622106, 1, -1, 8, 420)   --����6��������
	role.AddRoleItem(MapID, InstanceID, TargetID, 4622210, 1, -1, 8, 420)	--������ͨ6��������
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630001, 1, -1, 8, 420)	--����ʹ��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630101, 1, -1, 8, 420)	--�ռ�����
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630201, 1, -1, 8, 420)	--�����ػ���
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700278, 10, -1, 8, 420)  --���鱦ƿ���󶨣�
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050002, 999, -1, 8, 420)  --����
	role.AddRoleItem(MapID, InstanceID, TargetID, 4080079, 999, -1, 8, 420)  --����֮ѩ
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820087, 999, -1, 8, 420)  --����������󶨣�

	local Vocation = role.GetRoleClass(TargetID)
	if Vocation == 1 or	Vocation == 2 or Vocation == 3 or Vocation == 6 or Vocation == 7 or Vocation == 8 or Vocation == 9	then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8160020, 1, 5, 8, 420)
	elseif Vocation == 4 or Vocation == 5 or Vocation == 10 or Vocation == 11 or Vocation == 12 or Vocation == 13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8170020, 1, 5, 8, 420)
	end
end

function I4700039_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 14) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700039, 1, "I4700039_GiftBag")
aux.RegisterItemEvent(4700039, 0, "I4700039_CanUseGiftBag")

--50��Ա��������
function I4700040_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700041, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020116, 8, -1, 8, 420) 	--��+10
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020118, 2, -1, 8, 420) 	--��+12
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400031, 20, -1, 8, 420)	--����������ҩ
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700278, 50, -1, 8, 420)  --���鱦ƿ���󶨣�
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 999, -1, 8, 420)  --�������
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820061, 999, -1, 8, 420)  --���ֱ�
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050002, 999, -1, 8, 420)  --����
    local Vocation = role.GetRoleClass(TargetID)
	if Vocation == 1 or	Vocation == 2 or Vocation == 3 or Vocation == 6 or Vocation == 7 or Vocation == 8 or Vocation == 9	then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8010021, 1, 5, 8, 420)  --����֮��
		role.AddRoleItem(MapID, InstanceID, TargetID, 8020021, 1, 5, 8, 420)	--����֮��
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030121, 10, -1, 8, 420)	--������̫��ʯ
	elseif Vocation == 4 or Vocation == 5 or Vocation == 10 or Vocation == 11 or Vocation == 12 or Vocation == 13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8030021, 1, 5, 8, 420)	--����֮��
		role.AddRoleItem(MapID, InstanceID, TargetID, 8040021, 1, 5, 8, 420)	--����֮��
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030123, 10, -1, 8, 420)	--�������¹�ʯ
	end
end

function I4700040_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 11) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700040, 1, "I4700040_GiftBag")
aux.RegisterItemEvent(4700040, 0, "I4700040_CanUseGiftBag")

--60��Ա��������
function I4700041_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700042, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600007, 30, -1, 8, 420)  --��������������죩
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070085, 1, -1, 8, 420)	--5000��ȯ
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010020, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010025, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700278, 50, -1, 8, 420)  --���鱦ƿ����
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800039, 20, -1, 8, 420)  --60αħ�һ�ȯ
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820061, 999, -1, 8, 420)  --���ֱ�
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820035, 10, -1, 8, 420)  --�Ϲŵ伮
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050002, 999, -1, 8, 420)  --����
	 local Vocation = role.GetRoleClass(TargetID)
	if Vocation == 1 or	Vocation == 2 or Vocation == 3 or Vocation == 6 or Vocation == 7 or Vocation == 8 or Vocation == 9	then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8013007, 1, 5, 8, 420)  --ħ������
		role.AddRoleItem(MapID, InstanceID, TargetID, 8023007, 1, 5, 8, 420)
	elseif Vocation == 4 or Vocation == 5 or Vocation == 10 or Vocation == 11 or Vocation == 12 or Vocation == 13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8033007, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8043007, 1, 5, 8, 420)
	end
end

function I4700041_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700041, 1, "I4700041_GiftBag")
aux.RegisterItemEvent(4700041, 0, "I4700041_CanUseGiftBag")

--70��Ա��������
function I4700042_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700043, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020121, 10, -1, 8, 420)  --��+15
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020061, 20, -1, 8, 420)	--�м�����ʯ
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700278, 50, -1, 8, 420)  --���鱦ƿ����
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800131, 999, -1, 8, 420) --��������Ʊ����
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820061, 999, -1, 8, 420)  --���ֱ�
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820035, 30, -1, 8, 420)  --�Ϲŵ伮
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050002, 999, -1, 8, 420)  --����
--[[	 local Vocation = role.GetRoleClass(TargetID)
	if Vocation == 1 or	Vocation == 2 or Vocation == 3 or Vocation == 6 or Vocation == 7 or Vocation == 8 or Vocation == 9	then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8013007, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8023007, 1, 5, 8, 420)
	elseif Vocation == 4 or Vocation == 5 or Vocation == 10 or Vocation == 11 or Vocation == 12 or Vocation == 13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8033007, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8043007, 1, 5, 8, 420)
	end]]
end

function I4700042_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700042, 1, "I4700042_GiftBag")
aux.RegisterItemEvent(4700042, 0, "I4700042_CanUseGiftBag")

--80��Ա��������
function I4700043_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020061, 20, -1, 8, 420)  --�м�����ʯ
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020016, 10, -1, 8, 420)  --�߼�����ʯ
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020124, 5, -1, 8, 420)  --��+18
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020126, 2, -1, 8, 420)  --��+20
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400031, 50, -1, 8, 420)  --����������ҩ
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 999, -1, 8, 420)	--������[���]
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600007, 100, -1, 8, 420)	--��������������죩
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)	--ս����������ܣ�
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010021, 1, -1, 8, 420)	--����ʥҩ
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010026, 1, -1, 8, 420)	--����ʥˮ
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700278, 99, -1, 8, 420)  --���鱦ƿ����
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800041, 20, -1, 8, 420)  --80αħ�һ��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820061, 999, -1, 8, 420)  --���ֱү
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700230, 15, -1, 8, 420)   --9���ɳ�װ����
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820255, 200, -1, 8, 420)  --�ɳ�֮�飨�󶨣�
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820035, 50, -1, 8, 420)  --�Ϲŵ伮
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060062, 2, -1, 8, 420)  --����Ǳ����ҩ
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060058, 10, -1, 8, 420)  --�ȹ�������ҩ
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060059, 20, -1, 8, 420)  --�ߵ�������ҩ
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060061, 1, -1, 8, 420)  --������ҩ��Ա��ר�ã�δ������ߣ�
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050002, 999, -1, 8, 420)  --����
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030115, 50, -1, 8, 420)  --4����ʯ�����
	local Vocation = role.GetRoleClass(TargetID)
	if Vocation == 1 or	Vocation == 2 or Vocation == 3 or Vocation == 6 or Vocation == 7 or Vocation == 8 or Vocation == 9	then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8013009, 1, 5, 8, 420)  --80ħ������
		role.AddRoleItem(MapID, InstanceID, TargetID, 8023009, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030306, 10, -1, 8, 420)--����̫��ʯ[4��]

	elseif Vocation == 4 or Vocation == 5 or Vocation == 10 or Vocation == 11 or Vocation == 12 or Vocation == 13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8033009, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8043009, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030326, 10, -1, 8, 420)--���¹�ʯ[4��]
	end
end

function I4700043_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 34) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700043, 1, "I4700043_GiftBag")
aux.RegisterItemEvent(4700043, 0, "I4700043_CanUseGiftBag")

