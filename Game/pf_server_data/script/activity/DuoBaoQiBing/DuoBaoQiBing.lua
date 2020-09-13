----------------------------------------------------------------------------------------
-------------------------------- �ᱦ���� ------------------------------------------
----------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------
-- һЩ����
--------------------------------------------------------------------------------------
BaoXiang_ID						= 4500028						-- ������ƷID
DuoBaoQiBing_Active_ID	= 27									-- �ᱦ����ID
ERS_HoldChest					= 16777216						-- �ᱦ����Я��ģʽ0x1000000
HoldChest_BuffID				= 9003301						-- �ᱦ����Я��ģʽ0x1000000
ERS_SafeArea						=	16								-- 0x0010,	// ��ȫ��

-- ��C++ע��Ľű��¼�
ESRE_OnChaseHolderDie				= 44							-- �ᱦ����Я�������߱�ɱ��ʱ�����¼�
ESRE_ForceLootChase						= 45							-- ����ǿ�Ƶ���
ESIE_CanUse									=	0							-- ��Ʒ�Ƿ����
ESIE_CanDelGroundItem					=	2							-- ���ڵ��ϵ���Ʒ�Ƿ����ɾ��
ESIE_PutDownItem							=	3							-- ������Ʒ
ESIE_CanPickUpItem						=	4							-- �ж��Ƿ���Լ�����Ʒ
ESIE_PickUpItem								=	5							-- ������Ʒʱ����
ESIE_CanPutDown							= 6							-- �ж���Ʒ�Ƿ���Զ���
ESIE_CanDelItemOnRoleInit			= 7							-- ��ɫ��ʼ��ʱ�򣬼�鱳������Ʒ��Ҫɾ����0��ʾ��Ҫɾ���� 1��ʾ����Ҫ��
ESAE_OnTimer									=	1							-- ��ʱ����
ESAE_OnStart									=	2							-- ���ʼ
ESAE_OnEnd									=	3							-- �����
ESAE_OnTimerMin							=	4							-- �ÿ���Ӹ���
ESAE_OnActOpenClose					= 9							-- ����̨������رջ

-- ��ɫ����״̬ö��
EMS_Swim							= 6					-- ��Ӿ
EMS_SwimStand					= 7					-- ��ӾƯ��״̬

-- ������
E_Loot_Swim_Can_Not_Pickup_BaoXiang = 11			-- ��Ӿʱ����ʰȡ����
E_Loot_Role_Level_Limit = 12									-- ��ɫ�ȼ�����20������ʰȡ����
E_Loot_Role_SafeArea_Limit = 13								-- ��ɫ�ڰ�ȫ���ڲ���ʰȡ����

EWeekDay = {["Sunday"] = 1, ["Monday"] = 2, ["Tuesday"] = 3, ["Wednesday"] = 4, ["Thursday"] = 5, ["Friday"] = 6, ["Saturday"] = 7}

--------------------------------------------------------------------------------------
-- �ű��ֲ�����
--------------------------------------------------------------------------------------
BaoXiang_SerialID_High	= 0								-- ������Ʒ���кţ���λ��
BaoXiang_SerialID_Low	= 0								-- ������Ʒ���кţ���λ��
BaoXiang_GroupID_High = 0								-- �������ID����λ��
BaoXiang_GroupID_Low = 0								-- �������ID����λ��
BaoXiang_MapID				= 0							-- �������ڳ���ID
BaoXiang_X					= 0								-- ����X����
BaoXiang_Z					= 0								-- ����Z����
BaoXiang_Holder				= 0								-- �õ��������ҵĽ�ɫID
Min_Counter					= 0								-- ���Ӽ�����
Active_Started				= 0								-- ��Ƿ�ʼ
Active_End					= 0								-- ��Ƿ����
BaoXiang_OpenRemainTime		= BaoXiang_CDTime				-- ����ɿ���ʣ��ʱ�䣨��λΪ�룩
Tick_Count					= 0								-- tick��ʱ
BaoXiang_UpdateRemainTime	= BaoXiang_Update_Interval	    -- ����ˢ�±���ĵ���ʱ����λΪ�� ��
WeekDay_OpenState = {0, 0, 0, 0, 0, 0, 0}	 -- �ᱦ�������״̬�������տ�ʼ

--------------------------------------------------------------------------------------
-- �ű��ֲ����ߺ���
--------------------------------------------------------------------------------------
-- ���û��ر���
function ResetActiveData()
	BaoXiang_MapID				= 0								-- �������ڳ���ID
	BaoXiang_X					= 0								-- ����X����
	BaoXiang_Z					= 0								-- ����Z����
	BaoXiang_Holder				= 0							-- �õ��������ҵĽ�ɫID
	Min_Counter					= 0								-- ���Ӽ�����
	BaoXiang_OpenRemainTime		= BaoXiang_CDTime				-- ����ɿ���ʣ��ʱ�䣨��λΪ�룩
	Tick_Count					= 0								-- tick��ʱ
	BaoXiang_SerialID_High	= 0							-- ������Ʒ���кţ���λ��
	BaoXiang_SerialID_Low	= 0							-- ������Ʒ���кţ���λ��
	BaoXiang_GroupID_High = 0								-- �������ID����λ��
	BaoXiang_GroupID_Low = 0								-- �������ID����λ��
	BaoXiang_UpdateRemainTime	= BaoXiang_Update_Interval	    -- ����ˢ�±���ĵ���ʱ����λΪ�� ��
end

-- �㲥��XX�����XX����XX�����Ѿ�Я���˱�������ץ��ʱ��ȥ����ɣ���
function SendPickUpBaoXiangMsg()
	if BaoXiang_MapID == nil or BaoXiang_Holder == nil then
		return
	end
	if BaoXiang_MapID == 0 or BaoXiang_Holder == nil then
		return
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100073)
	msg.AddMsgEvent(MsgID, 2, BaoXiang_Holder)
	msg.AddMsgEvent(MsgID, 6, BaoXiang_MapID)
	msg.AddMsgEvent(MsgID, 9, BaoXiang_X)
	msg.AddMsgEvent(MsgID, 9, BaoXiang_Z)
	msg.DispatchWorldMsgEvent(MsgID)
end

--�㲥���ʼ(���������XX������XX����㣬��ҸϿ�ȥ���ᱦ���)
function SendChaseUpdateMsg()
	if BaoXiang_MapID == nil then
		return
	end
	if BaoXiang_MapID == 0 then
		return
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100072)
	msg.AddMsgEvent(MsgID, 6, BaoXiang_MapID)
	msg.AddMsgEvent(MsgID, 9, BaoXiang_X)
	msg.AddMsgEvent(MsgID, 9, BaoXiang_Z)
	msg.DispatchWorldMsgEvent(MsgID)
end

-- ��������߱����ɱ��
function SendChaseHolderBeKilledByRole(KillerID)
	if BaoXiang_MapID == nil then
		return
	end

	if BaoXiang_MapID == 0 then
		return
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100074)
	msg.AddMsgEvent(MsgID, 2, KillerID)
	msg.AddMsgEvent(MsgID, 2, BaoXiang_Holder)
	msg.AddMsgEvent(MsgID, 6, BaoXiang_MapID)
	msg.AddMsgEvent(MsgID, 9, BaoXiang_X)
	msg.AddMsgEvent(MsgID, 9, BaoXiang_Z)
	msg.DispatchWorldMsgEvent(MsgID)
end

-- ͬ�����俪��ʣ��ʱ��
function SyncOpenChaseRemainTime()
	if BaoXiang_Holder ~= 0 then
		role.SyncOpenChaseRemainTime(BaoXiang_Holder, BaoXiang_OpenRemainTime)
	end
end

-- ���±��䵱ǰ����()
function UpdateBaoXiangPos(RoleID)
	if BaoXiang_MapID == nil then
		return
	end
	if BaoXiang_MapID == 0 then
		return
	end

	if RoleID ~= 0 then
		local BaoXiang_Y = 0
		BaoXiang_X, BaoXiang_Y, BaoXiang_Z = unit.GetPosition(BaoXiang_MapID, 0, RoleID)
	end
end

-- ȥ����ɫ���ϵ�Я��ģʽ״̬
function UnSetHoldChestState(RoleID)
	role.UnSetRoleState(RoleID, ERS_HoldChest, 1)
	unit.CancelBuff(BaoXiang_MapID, 0, RoleID, HoldChest_BuffID)
end

-- ����ɫ���öᱦ����Я��ģʽ״̬
function SetHoldChestState(RoleID)
	role.SetRoleState(RoleID, ERS_HoldChest, 1)
	unit.AddBuff(BaoXiang_MapID, 0, RoleID, HoldChest_BuffID, RoleID)
end

-- �����õ��������ҵĽ�ɫID
function SetBaoXiangHolderRoleID(RoleID)
	if RoleID == 0 then
		-- ȥ����ɫ���ϵ�Я��ģʽ״̬
		UnSetHoldChestState(BaoXiang_Holder)

		-- ������䱻���������ñ��俪��ʣ��ʱ��
		if BaoXiang_Hold_Time_Mod == 0 then
			BaoXiang_OpenRemainTime = BaoXiang_CDTime
		end

		-- ���±��䵱ǰ����
		UpdateBaoXiangPos(BaoXiang_Holder)
	else
		-- ����ɫ���öᱦ����Я��ģʽ״̬
		SetHoldChestState(RoleID)

		-- ����ˢ�±���ĵ���ʱ����λΪ�� ��
		BaoXiang_UpdateRemainTime = BaoXiang_Update_Interval

		-- ���±��䵱ǰ����
		UpdateBaoXiangPos(RoleID)
	end

	-- �����õ��������ҵĽ�ɫID
	BaoXiang_Holder = RoleID

	-- ͬ�����俪��ʣ��ʱ��
	SyncOpenChaseRemainTime()
end

-- ɾ�������
function RemoveBaoXiang()
	if BaoXiang_Holder ~= 0 then
		role.RemoveFromRole(BaoXiang_MapID, 0, BaoXiang_Holder, BaoXiang_ID, 1, 310)
	else
		map.RemoveGroundItem(BaoXiang_MapID, BaoXiang_GroupID_High, BaoXiang_GroupID_Low)
	end
end

-- ÿ�ջ��ʼ
function ActiveStart()
	-- ���ǰһ��Ļû�н��������ұ�����������ϣ�ɾ������
	if Active_End == 0 then
		-- ɾ�������
		RemoveBaoXiang()
	end

	-- ȥ����ɫ���ϵ�Я��ģʽ״̬
	UnSetHoldChestState(BaoXiang_Holder)

	-- ���û��ر���
	ResetActiveData()

	-- �㲥�����Я�������߲��ɽ���ʹ��"���͵��ߣ����ߡ��������ִ塢�����Լ���������ˮ"�����ҽ������ϲ�����ʹ���˹涨�еĵ��ߣ����ｫ�Զ������ڽ��в�����ʹ�õ���ǰ�ĵص㣬��������أ�������
	local RuleMsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(RuleMsgID, 102, 100075)
	msg.DispatchWorldMsgEvent(RuleMsgID)

	-- ������ˢ�������ŵ������ͼ��������������Ա�
	-- �㲥���ʼ(���������XX������XX����㣬��ҸϿ�ȥ���ᱦ���)
	ActiveStartAndStartBroadcast()

	-- �״̬���
	Active_Started = 1
	Active_End = 0
end

-- �ᱦ��������
function ActiveEnd()
	Active_End = 1

	-- ��������㲥
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100083)
	msg.DispatchWorldMsgEvent(MsgID)

	-- ȥ����ɫ���ϵ�Я��ģʽ״̬
	UnSetHoldChestState(BaoXiang_Holder)
end

-- �ᱦ������ʼ��ˢ������������㲥
function ActiveStartAndStartBroadcast()
	--������ˢ�������ŵ������ͼ��������������Ա�
	BaoXiang_MapID, BaoXiang_X, BaoXiang_Z, BaoXiang_SerialID_High, BaoXiang_SerialID_Low, BaoXiang_GroupID_High, BaoXiang_GroupID_Low = map.PutItemNearRandCreature(BaoXiang_ID)
	if BaoXiang_MapID == nil or BaoXiang_X == nil or BaoXiang_Z == nil or BaoXiang_SerialID_High==nil or BaoXiang_SerialID_Low == nil or BaoXiang_GroupID_High == nil or BaoXiang_GroupID_Low == nil then
		return
	end

	--�㲥ˢ�±���(���������XX������XX����㣬��ҸϿ�ȥ���ᱦ���)
	SendChaseUpdateMsg()
end

-- �ж϶ᱦ�����Ƿ�ʼ�ˣ����һ�û�н���
function IsActiveEnd()
	return Active_End
end

-- ���±��俪��ʣ��ʱ��
function UpdateOpenRemainTime()
	if BaoXiang_OpenRemainTime == nil then
		BaoXiang_OpenRemainTime	= BaoXiang_CDTime				-- ����ɿ���ʣ��ʱ�䣨��λΪ�룩
	end

	if IsActiveEnd() == 1 then
		return
	end

	if BaoXiang_Holder ~= 0  then
		BaoXiang_OpenRemainTime = BaoXiang_OpenRemainTime-1

		-- �������ɿ���ʣ��ʱ�䣨��λΪ�룩С��0���ᱦ��������
		if BaoXiang_OpenRemainTime < 0 then
			ActiveEnd()
		end
	end
end

-- ��������ˢ�±���ʣ��ʱ��
function BaoXiangUpdateRemainTime()
	if BaoXiang_UpdateRemainTime == nil then
		BaoXiang_UpdateRemainTime = BaoXiang_Update_Interval	    -- ����ˢ�±���ĵ���ʱ����λΪ�� ��
	end
	if IsActiveEnd() == 1 then
		return
	end

	if BaoXiang_Holder == 0 then
		BaoXiang_UpdateRemainTime = BaoXiang_UpdateRemainTime-1

		-- ���ˢ�±���ʣ��ʱ�䣨��λΪ�룩С��0������ˢ�±���
		if BaoXiang_UpdateRemainTime < 0 then
			map.RemoveGroundItem(BaoXiang_MapID, BaoXiang_GroupID_High, BaoXiang_GroupID_Low)

			-- �ᱦ������ʼ��ˢ������������㲥
			ActiveStartAndStartBroadcast()

			-- ����ˢ�±���ĵ���ʱ����λΪ�� ��
			BaoXiang_UpdateRemainTime	= BaoXiang_Update_Interval

		end
	end
end

-- ����ǽ����Ƿ��жᱦ����
function IsTodayHaveActive()
	return WeekDay_OpenState[EWeekDay[os.date("%A")]]
end

--------------------------------------------------------------------------------------
-- �ű���C++��������ע����¼�����
--------------------------------------------------------------------------------------

-- �ᱦ������ʼ
function DuoBaoQiBing_OnActiveStart(ActiveID)
	if DuoBaoQiBing_trigger == 0 then
		return
	end

	WeekDay_OpenState[1], WeekDay_OpenState[2], WeekDay_OpenState[3], WeekDay_OpenState[4], WeekDay_OpenState[5], WeekDay_OpenState[6], WeekDay_OpenState[7] = act.GetDuoBaoQiBingOpenState()

	-- ���û��ر���
	ResetActiveData()

	Active_Started				= 0								-- ��Ƿ�ʼ
	Active_End					= 1								-- ��Ƿ����
end

-- ÿtick����
function DuoBaoQiBing_OnActiveTimer(ActiveID, Sec)

	if DuoBaoQiBing_trigger == 0 then
		return
	end

	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	local Sec   = tonumber(os.date("%S"))

	-- tick��ʱ�����㱦��CDʱ�䣬��λΪ�룩
	Tick_Count = Tick_Count+1
	if Tick_Count == 6 then

		-- ���±��俪��ʣ��ʱ��
		UpdateOpenRemainTime()

		-- ��������ˢ�±���ʣ��ʱ��
		BaoXiangUpdateRemainTime()

		Tick_Count = 0
	end

	-- ÿ�����ϰ˵���ʼ���ߵ���ʮ�ŷֵ�ʱ������Active_Started
	if IsTodayHaveActive() == 1 then
		if Hour == 19 and Min == 59 and Active_Started == 1 then
			Active_Started = 0
		end

		if Hour == 20 and Min == 0 and Active_Started == 0 then
			ActiveStart()
		end
	end
end

-- ÿ���Ӹ���
function DuoBaoQiBing_OnActiveTimeMin(ActiveID)
	if DuoBaoQiBing_trigger == 0 then
		return
	end

	if IsActiveEnd() == 1 then
		return
	end

	-- ������仹û�б���Ҽ���
	if BaoXiang_Holder ~= 0 then
		-- ���±��䵱ǰ����
		UpdateBaoXiangPos(BaoXiang_Holder)

		-- ͬ�����俪��ʣ��ʱ��
		SyncOpenChaseRemainTime()
	end

	Min_Counter = Min_Counter+1
	if Min_Counter > BaoXiang_Pos_Broadcast_Interval then

		-- ������仹û�б���Ҽ���
		if BaoXiang_Holder ~= 0 then
			-- �㲥��XX�����XX����XX�����Ѿ�Я���˱�������ץ��ʱ��ȥ����ɣ���
			SendPickUpBaoXiangMsg()
		else
			-- �㲥(���������XX������XX����㣬��ҸϿ�ȥ���ᱦ���)
			SendChaseUpdateMsg()
		end

		Min_Counter = 0
	end

end

-- �����
function DuoBaoQiBing_OnActiveEnd(ActiveID)

end

--��Ҽ����䣬���㲥
function DuoBaoQiBing_OnPickUpBaoXiang(ItemID, RoleID)
	if DuoBaoQiBing_trigger == 0 then
		return
	end

	if IsActiveEnd() == 1 then
		return
	end

	if ItemID~=BaoXiang_ID then
		return
	end

	-- �����õ��������ҵĽ�ɫID
	SetBaoXiangHolderRoleID(RoleID)

	BaoXiang_GroupID_High = 0
	BaoXiang_GroupID_Low = 0

	-- �㲥��XX�����XX����XX�����Ѿ�Я���˱�������ץ��ʱ��ȥ����ɣ���
	SendPickUpBaoXiangMsg()

end

--�ж�����Ƿ��ܼ�����
function DuoBaoQiBing_CanPickUpBaoXiang(ItemID, RoleID)
	if DuoBaoQiBing_trigger == 0 then
		return 0
	end

	-- ���ȼ�
	local RoleLvl = role.GetRoleLevel(BaoXiang_MapID, 0, RoleID)
	if RoleLvl == nil then
		return 0		--E_Success
	end
	if BaoXiang_ID == ItemID and  (RoleLvl < Active_Min_Level or RoleLvl > Active_Max_Level) then
		return E_Loot_Role_Level_Limit		-- �ȼ�������Ҫ��
	end

	-- ����ɫ��Ӿ״̬
	local CurMoveState = role.GetCurMoveState(RoleID)
	if CurMoveState == EMS_Swim	or CurMoveState == EMS_SwimStand then
		return E_Loot_Swim_Can_Not_Pickup_BaoXiang
	end

	-- ����ɫ�ǲ�������ȫ��
	if role.IsRoleInStatus(RoleID, ERS_SafeArea) then
		return E_Loot_Role_SafeArea_Limit
	end

	return 0			--E_Success
end

-- ���б������ұ�ɱ��
function DuoBaoQiBing_OnChaseHolderDie(RoleID, KillerID)
	if DuoBaoQiBing_trigger == 0 then
		return
	end

	if IsActiveEnd() == 1 then
		return
	end

	if BaoXiang_Holder ~= RoleID then
		return
	end

	-- ���±��䵱ǰ����
	UpdateBaoXiangPos(BaoXiang_Holder)

	if unit.IsPlayer(KillerID) then
		-- ���㲥��<Msg id="100074" type="1" channel="5" value="���[&lt;p1&gt;]�Ѿ���Я����������[&lt;p2&gt;]ɱ�������������&lt;p3&gt;(&lt;p4&gt;,&lt;p5&gt;)���꣬��ҸϿ�ȥ����ɣ�"/>��
		SendChaseHolderBeKilledByRole(KillerID)
	else
		--�㲥ˢ�±���(���������XX������XX����㣬��ҸϿ�ȥ���ᱦ���)
		SendChaseUpdateMsg()
	end

	-- ��������ϵı��䶪��������
	role.PutDownItem(RoleID, BaoXiang_ID)

	-- ���ñ���Я����ID
	SetBaoXiangHolderRoleID(0)

end

-- �����Ƿ����ɾ��(0��ʾ����ɾ���� 1��ʾ������)
function DuoBaoQiBing_CanDelBaoXiangOnGround(TypeID, SerialID_High, SerialID_Low)
	if DuoBaoQiBing_trigger == 0 then
		return 0
	end

	if IsActiveEnd() == 1 then
		return 0
	end

	if TypeID == BaoXiang_ID and BaoXiang_SerialID_High == SerialID_High and BaoXiang_SerialID_Low == SerialID_Low then
		return 1
	else
		return 0
	end
end

-- ����ǿ�Ƶ���
function DuoBaoQiBing_ForceLootChase(RoleID)
	if DuoBaoQiBing_trigger == 0 then
		return
	end

	if IsActiveEnd() == 1 then
		return
	end

	if BaoXiang_Holder == RoleID then
		-- ���±��䵱ǰ����
		UpdateBaoXiangPos(BaoXiang_Holder)

		-- ��������ϵı��䶪��������
		role.PutDownItem(RoleID, BaoXiang_ID)

		-- ֪ͨ��ұ������
		role.ForceLootChase(BaoXiang_Holder)

		-- ���ñ���Я����ID
		SetBaoXiangHolderRoleID(0)

		--�㲥ˢ�±���(���������XX������XX����㣬��ҸϿ�ȥ���ᱦ���)
		SendChaseUpdateMsg()
	end
end


--[[-- ���ˣ����䲻�ܶ�����
function DuoBaoQiBing_PutDownItem(TypeID)
	if IsActiveEnd() == 1 then
		return
	end

	-- ���ñ���Я����ID
	SetBaoXiangHolderRoleID(0)

	--�㲥ˢ�±���(���������XX������XX����㣬��ҸϿ�ȥ���ᱦ���)
	SendChaseUpdateMsg()
end
--]]

-- ���ˣ����䲻�ܶ�����(0��ʾ���ԣ� 1��ʾ������)
function DuoBaoQiBing_CanPutDownBaoXiang(TypeID, SerialID_High, SerialID_Low)
	if DuoBaoQiBing_trigger == 0 then
		return 0
	end

	if IsActiveEnd() == 1 then
		return 0
	end

	if TypeID == BaoXiang_ID and BaoXiang_SerialID_High == SerialID_High and BaoXiang_SerialID_Low == SerialID_Low then
		return 1
	else
		return 0
	end
end

-- �����Ƿ��ܿ���
function DuoBaoQiBing_CanOpenBaoXiang(MapID, InstanceID, TypeID, TargetID)
	if DuoBaoQiBing_trigger == 0 then
		return 0, 0
	end

	if IsActiveEnd() == 1 then
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 8) then
			--��ʾ��ұ����ռ䲻��
			return 40, false
		end
		return 0, 0
	end

	-- E_UseItem_TreasureInTime		=	60,		// �ᱦ�������δ��������ʱ��
	return 60, 1
end

--��ɫ��ʼ��ʱ�򣬼�鱳������Ʒ��Ҫɾ����0��ʾ��Ҫɾ���� 1��ʾ����Ҫ��
function DuoBaoQiBing_CanDeleteBaoXiang(TypeID, SerialID_High, SerialID_Low)
	if TypeID == BaoXiang_ID  then
		if BaoXiang_SerialID_High == SerialID_High and BaoXiang_SerialID_Low == SerialID_Low and IsActiveEnd() == 1 then
			return 1
		else
			return 0
		end
	else
		return 1
	end
end

--�����̨����
function DuoBaoQiBing_OnActOpenClose(ActiveID, IsOpen)
	if DuoBaoQiBing_Active_ID ~= ActiveID then
		return
	end

	if DuoBaoQiBing_trigger == 0 and IsActiveEnd() ~= 1 then
		-- ɾ�������
		RemoveBaoXiang()

		-- �ᱦ��������
		ActiveEnd()
	end

	DuoBaoQiBing_trigger = IsOpen
end

-- ��ע���ʼ������ʱ���²���
aux.RegisterActEvent(DuoBaoQiBing_Active_ID, ESAE_OnTimer, "DuoBaoQiBing_OnActiveTimer")
aux.RegisterActEvent(DuoBaoQiBing_Active_ID, ESAE_OnStart, "DuoBaoQiBing_OnActiveStart")
aux.RegisterActEvent(DuoBaoQiBing_Active_ID, ESAE_OnEnd, "DuoBaoQiBing_OnActiveEnd")
aux.RegisterActEvent(DuoBaoQiBing_Active_ID, ESAE_OnTimerMin, "DuoBaoQiBing_OnActiveTimeMin")
aux.RegisterActEvent(DuoBaoQiBing_Active_ID, ESAE_OnActOpenClose, "DuoBaoQiBing_OnActOpenClose")

aux.RegisterRoleEvent(ESRE_OnChaseHolderDie,	"DuoBaoQiBing_OnChaseHolderDie")
aux.RegisterRoleEvent(ESRE_ForceLootChase,		"DuoBaoQiBing_ForceLootChase")

aux.RegisterItemEvent(BaoXiang_ID, ESIE_CanPickUpItem,					"DuoBaoQiBing_CanPickUpBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID, ESIE_PickUpItem,							"DuoBaoQiBing_OnPickUpBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID, ESIE_CanDelGroundItem,				"DuoBaoQiBing_CanDelBaoXiangOnGround")
--aux.RegisterItemEvent(BaoXiang_ID, ESIE_PutDownItem,					"DuoBaoQiBing_PutDownItem")
aux.RegisterItemEvent(BaoXiang_ID, ESIE_CanPutDown,						"DuoBaoQiBing_CanPutDownBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID, ESIE_CanUse,								"DuoBaoQiBing_CanOpenBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID, ESIE_CanDelItemOnRoleInit,		"DuoBaoQiBing_CanDeleteBaoXiang")

