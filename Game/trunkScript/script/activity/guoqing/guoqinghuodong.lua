--���ʼ
guoqingshizhe_id = 0
guoqinglingtong_id = 0
guoqingshangdian_id = 0
huanleshizhe_id = 0
every_Exp={}
every_Exp[50]=100000
every_Exp[51]=100000
every_Exp[52]=200000
every_Exp[53]=300000
every_Exp[54]=400000
every_Exp[55]=500000
every_Exp[56]=600000
every_Exp[57]=700000
every_Exp[58]=800000
every_Exp[59]=900000
every_Exp[60]=1000000
every_Exp[61]=1100000
every_Exp[62]=1200000
every_Exp[63]=1300000
every_Exp[64]=1400000
every_Exp[65]=1500000
every_Exp[66]=1600000
every_Exp[67]=1700000
every_Exp[68]=1800000
every_Exp[69]=1900000
every_Exp[70]=2000000
every_Exp[71]=2100000
every_Exp[72]=2200000
every_Exp[73]=2300000
every_Exp[74]=2400000
every_Exp[75]=2500000
every_Exp[76]=2600000
every_Exp[77]=2700000
every_Exp[78]=2800000
every_Exp[79]=2900000
every_Exp[80]=3000000
every_Exp[81]=3100000
every_Exp[82]=3200000
every_Exp[83]=3300000
every_Exp[84]=3400000
every_Exp[85]=3500000
every_Exp[86]=3600000
every_Exp[87]=3700000
every_Exp[88]=3800000
every_Exp[89]=3900000
every_Exp[90]=4000000
every_Exp[91]=4100000
every_Exp[92]=4200000
every_Exp[93]=4300000
every_Exp[94]=4400000
every_Exp[95]=4500000
every_Exp[96]=4600000
every_Exp[97]=4700000
every_Exp[98]=4800000
every_Exp[99]=4900000
every_Exp[100]=5000000
function guoqing_OnStart(actID)
		guoqingshizhe_id = map.MapCreateCreature(3424073512, -1, 5610062, 505, 23510, 759)
		guoqinglingtong_id = map.MapCreateCreature(3424073512, -1, 5610063, 505, 23510, 765)
		guoqingshangdian_id = map.MapCreateCreature(3424073512, -1, 5610064, 500, 23510, 765)
		huanleshizhe_id = map.MapCreateCreature(3424073512, -1, 5610065, 402, 23692, 753)

end
aux.RegisterActEvent(45, 2, "guoqing_OnStart")
--�����
function guoqing_OnEnd(actID)
	if guoqinglingtong_id ~= 0 and guoqinglingtong_id ~= nil then
		map.MapDeleteCreature(3424073512, -1, guoqinglingtong_id)
		guoqinglingtong_id=nil
		map.MapDeleteCreature(3424073512, -1, huanleshizhe_id)
		huanleshizhe_id=nil
	end
end
aux.RegisterActEvent(45, 3, "guoqing_OnEnd")

--����ף��ʹ��
function c5610062_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local curmon = tonumber(os.date("%m"))--��
	local curday = tonumber(os.date("%d"))--��
	local curyear = tonumber(os.date("%Y"))--��
	local guoqing_start = false
	local restExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["curExp"])
	if curyear == 2011 and ((curmon == 9 and curday >=27) or (curmon == 10 and curday <= 10))  then
		guoqing_start = true
	else
		guoqing_start = false
	end
	if (guoqing_start == true and TalkIndex == -1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444240)				--����ʮ�£��ٹ�ͬ�죬�ҿ��Ǹ���������˺ܶ�ף�����Ͻ�ѡ����ϲ���İɣ����˿ɾ�û���ˡ���50�����µ�С���Ѿͱ�������ˣ���ȥ�����ɣ�
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)				--
		msg.AddMsgEvent(MsgID, 1, 444236)			--��ÿ�ձ�����
		msg.AddMsgEvent(MsgID, 21, 5)				--
		msg.AddMsgEvent(MsgID, 1, 444237)			--����ȡ������
		msg.AddMsgEvent(MsgID, 21, 6)				--
		msg.AddMsgEvent(MsgID, 1, 444238)			--������ָ���
		msg.AddMsgEvent(MsgID, 21, 30)				--
		msg.AddMsgEvent(MsgID, 1, 100002)			--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif (guoqing_start == false and TalkIndex == -1 and restExp~=0) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444249)				--�����Ѿ������ˣ��Ͻ�����ľ������߰ɣ������ҿ�Ҫ��ȥ�ˡ�
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 5)				--
		msg.AddMsgEvent(MsgID, 1, 444237)			--����ȡ������
		msg.AddMsgEvent(MsgID, 21, 6)				--
		msg.AddMsgEvent(MsgID, 1, 444238)			--������ָ���
		msg.AddMsgEvent(MsgID, 21, 30)				--
		msg.AddMsgEvent(MsgID, 1, 100002)			--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif (guoqing_start == false and TalkIndex == -1 and restExp==0) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444250)				--���ˣ���Ľ����Ѿ�������ˣ����������ټ��ɡ�
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 30)				--
		msg.AddMsgEvent(MsgID, 1, 100002)			--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444242)  			--���㵽��50�������ܲμ��������Ͻ�ȥ�����ɡ�
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444241)				--�������Ѿ���ɱ����ˣ�������ȡ������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 10)				--�鿴�������
			msg.AddMsgEvent(MsgID, 1, 444251)
			msg.AddMsgEvent(MsgID, 21, -1)				--
			msg.AddMsgEvent(MsgID, 1, 100001)			--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			local curExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["curExp"])
			local days = role.GetRoleScriptData(RoleID, 1, RoleDataType["guoqingbaodao"])
			local signDays = days%100
			local lossDays = (days%10000-days%100)/100
			local cursign= (days%100000-days%10000)/10000
			local lastDate = (days%1000000000-days%1000000)/1000000
			local changeDate = (days-days%1000000000)/1000000000
			local curDate = tonumber(os.date("%j"))
			local totalExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["totalExp"])
			local gotExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["gotExp"])
			local lossExp = totalExp - curExp

			if(curDate == lastDate) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444243)				--�����㲻�Ǳ�����һ�����𣿿����������ģ��Դ���ô����ʹ��
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, 10)				--�鿴�������
				msg.AddMsgEvent(MsgID, 1, 444251)
				msg.AddMsgEvent(MsgID, 21, -1)				--
				msg.AddMsgEvent(MsgID, 1, 100001)			--��ȡ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif(curDate - lastDate > 0 and curDate - lastDate < 15 ) then
				if(changeDate == 1) then
					lossDays = lossDays+(curDate - lastDate -1)
					for i=1,(curDate - lastDate -1)  do
						lossExp = lossExp+curExp*0.5
						curExp = curExp * 0.5
					end
				end
				signDays=signDays+1
				cursign =cursign+1
				lastDate = curDate
				changeDate = 1
				totalExp = totalExp+every_Exp[level]*(1+0.2*(cursign-1))
				if level < 60 then
					curExp = totalExp - lossExp
				elseif level <70 then
					curExp = totalExp - lossExp
				elseif level <80 then
					curExp = totalExp - lossExp
				elseif level <90 then
					curExp = totalExp - lossExp
				elseif level <100 then
					curExp = totalExp - lossExp
				end
				role.SetRoleScriptData(RoleID, 1, RoleDataType["curExp"], curExp)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["totalExp"], totalExp)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["guoqingbaodao"], changeDate*1000000000+lastDate*1000000+cursign*10000+lossDays*100+signDays)
			elseif(curDate - lastDate > 14) then
				lastDate = curDate
				changeDate = 1
				lossExp = 0
				signDays=signDays+1
				cursign = cursign +1
				curExp = every_Exp[level]
				totalExp = every_Exp[level]
				role.SetRoleScriptData(RoleID, 1, RoleDataType["curExp"], curExp)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["totalExp"], totalExp)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["guoqingbaodao"], changeDate*1000000000+lastDate*1000000+cursign*10000+lossDays*100+signDays)
			end
		end
	elseif TalkIndex == 5 then
		local curExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["curExp"])
		local days = role.GetRoleScriptData(RoleID, 1, RoleDataType["guoqingbaodao"])
		local signDays = days%100
		local lastDate = (days%1000000000-days%1000000)/1000000
		local gotExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["gotExp"])
		local lossDays = (days%10000-days%100)/100
		local cursign= (days%100000-days%10000)/10000
		local curDate = tonumber(os.date("%j"))
		local changeDate = (days-days%1000000000)/1000000000
		if(changeDate == 1 and curDate ~= lastDate) then
			lossDays = lossDays+(curDate - lastDate -1)
			for i=1,(curDate - lastDate -1)  do
				curExp = curExp * 0.5
			end
			changeDate = 0
			role.SetRoleScriptData(RoleID, 1, RoleDataType["guoqingbaodao"], changeDate*1000000000+lastDate*1000000+cursign*10000+lossDays*100+signDays)
		end
		local totalExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["totalExp"])
		local lossExp = totalExp - curExp
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)

		role.SetRoleScriptData(RoleID, 1, RoleDataType["curExp"], curExp)
		if level < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444242)  			--���㵽��50�������ܲμ��������Ͻ�ȥ�����ɡ�
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		local MsgID = msg.BeginMsgEvent()
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			msg.AddMsgEvent(MsgID, 20, 444244)  			--��ȷ��Ҫ��ȡ��
			msg.AddMsgEvent(MsgID, 9, signDays)
			msg.AddMsgEvent(MsgID, 9, curExp)
			msg.AddMsgEvent(MsgID, 9, lossDays)
			msg.AddMsgEvent(MsgID, 9, lossExp)
			msg.AddMsgEvent(MsgID, 9, gotExp)
			msg.AddMsgEvent(MsgID, 21, 8)
			msg.AddMsgEvent(MsgID, 1, 100001)		--��ȷ����
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 6 then
		local curExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["curExp"])
		local days = role.GetRoleScriptData(RoleID, 1, RoleDataType["guoqingbaodao"])
		local signDays = days%100
		local lastDate = (days%1000000000-days%1000000)/1000000
		local gotExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["gotExp"])
		local lossDays = (days%10000-days%100)/100
		local cursign= (days%100000-days%10000)/10000
		local curDate = tonumber(os.date("%j"))
		local changeDate = (days-days%1000000000)/1000000000
		if(changeDate == 1 and lastDate ~= curDate) then
			lossDays = lossDays+(curDate - lastDate -1)
			for i=1,(curDate - lastDate -1)  do
				curExp = curExp * 0.5
			end
			changeDate = 0
			role.SetRoleScriptData(RoleID, 1, RoleDataType["guoqingbaodao"], changeDate*1000000000+lastDate*1000000+cursign*10000+lossDays*100+signDays)
		end
		local totalExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["totalExp"])
		local lossExp = totalExp - curExp
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)

		role.SetRoleScriptData(RoleID, 1, RoleDataType["curExp"], curExp)
		if level < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444242)  			--���㵽��50�������ܲμ��������Ͻ�ȥ�����ɡ�
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444245)  			--��ȷ��Ҫ�ָ��𣿻ָ�������Ҫ����һ��ħ��ˮ
			msg.AddMsgEvent(MsgID, 9, signDays)
			msg.AddMsgEvent(MsgID, 9, curExp)
			msg.AddMsgEvent(MsgID, 9, lossDays)
			msg.AddMsgEvent(MsgID, 9, lossExp)
			msg.AddMsgEvent(MsgID, 9, gotExp)
			msg.AddMsgEvent(MsgID, 21, 9)
			msg.AddMsgEvent(MsgID, 1, 100001)		--��ȷ����
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100002)		--��ȡ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif(TalkIndex == 8) then
		local curExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["curExp"])
		local days = role.GetRoleScriptData(RoleID, 1, RoleDataType["guoqingbaodao"])
		local signDays = days%100
		local lastDate = (days%1000000000-days%1000000)/1000000
		local gotExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["gotExp"])
		local lossDays = (days%10000-days%100)/100
		local cursign= (days%100000-days%10000)/10000
		local totalExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["totalExp"])
		local lossExp = totalExp - curExp
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		local num = role.GetRoleItemNum(RoleID, 4820063)
		local exp = curExp
		if(curExp == 0) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444252)       --��û�п���ȡ�ľ��飬һ������ȥ��
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100001)		--��ȷ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if(num <= 10) then                      --�о�ħ��ˮ���������轱��
				exp = (1+num * 0.05) *curExp
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4820063, num, 101)
			else
				exp = 1.5*curExp
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4820063, 10, 101)
			end
			role.AddRoleExp(MapID, InstanceID, RoleID, exp)
			gotExp = gotExp+curExp							--����ȡ��������
			curExp = 0                              --����ȡ���������
			totalExp = 0
			cursign = 0								--����ǩ���������
			role.SetRoleScriptData(RoleID, 1, RoleDataType["curExp"], curExp)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["gotExp"], gotExp)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["totalExp"], totalExp)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["guoqingbaodao"], lastDate*1000000+cursign*10000+lossDays*100+signDays)
		end
	elseif(TalkIndex == 9) then
		local curExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["curExp"])
		local days = role.GetRoleScriptData(RoleID, 1, RoleDataType["guoqingbaodao"])
		local signDays = days%100
		local lastDate = (days%1000000000-days%1000000)/1000000
		local gotExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["gotExp"])
		local lossDays = (days%10000-days%100)/100
		local cursign= (days%100000-days%10000)/10000
		local totalExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["totalExp"])
		local lossExp = totalExp - curExp
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		local num = role.GetRoleItemNum(RoleID, 4820063)
		local exp = curExp
		if(lossExp == 0) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444248)       --��û��ʧ���飬��ʲô���֣�û���Һ�æ��
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100001)		--��ȷ����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if(num < 1) then                      --�о�ħ��ˮ���������轱��
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444246)       --�������أ�������û��ħ��ˮ������ʲô���֡�
				msg.AddMsgEvent(MsgID, 21, -1)
				msg.AddMsgEvent(MsgID, 1, 100001)		--��ȷ����
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				curExp = curExp + lossExp
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4820063, 1, 101)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["curExp"], curExp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71, 444253)       --��û��ʧ���飬��ʲô���֣�û���Һ�æ��
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	elseif(TalkIndex == 10) then
		local curExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["curExp"])
		local days = role.GetRoleScriptData(RoleID, 1, RoleDataType["guoqingbaodao"])
		local signDays = days%100
		local lastDate = (days%1000000000-days%1000000)/1000000
		local gotExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["gotExp"])
		local lossDays = (days%10000-days%100)/100
		local cursign= (days%100000-days%10000)/10000
		local totalExp = role.GetRoleScriptData(RoleID, 1, RoleDataType["totalExp"])
		local lossExp = totalExp - curExp
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444247)  			--��ʾ�������
		msg.AddMsgEvent(MsgID, 9, signDays)
		msg.AddMsgEvent(MsgID, 9, curExp)
		msg.AddMsgEvent(MsgID, 9, lossDays)
		msg.AddMsgEvent(MsgID, 9, lossExp)
		msg.AddMsgEvent(MsgID, 9, gotExp)
		msg.AddMsgEvent(MsgID, 21, -1)
		msg.AddMsgEvent(MsgID, 1, 100001)		--��ȡ����
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(5610062, 7, "c5610062_On_Talk")

