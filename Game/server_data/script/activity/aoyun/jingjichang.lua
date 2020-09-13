function n6040103_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curhour = tonumber(os.date("%H"))
	local curmin = tonumber(os.date("%M"))

	local time1 = curmon*10000 + curday *100 + curhour--����ʱ
	if time1 ~=role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_time"]) then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_baoming"], 0)--����0û���б�����1��ͨ��2��רҵ��
		role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_time"], time1)--
		role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_jifen"],0)
	end
	local x = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_baoming"])

	local money = role.GetRoleSilver(MapID, InstanceID,RoleID)

	local lev = role.GetRoleLevel(MapID, InstanceID,RoleID)

	local juan = role.GetRoleItemNum(RoleID, 4810040)

	if TalkIndex == -1 then
		if curhour == 20 or (curhour == 21 and curmin < 40)then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211162) -- ��������Ŀ�����ˣ�\n    ѡ�����������������ͨ�鱨����Ҫ20��רҵ�鱨����Ҫ50���һ��רҵ�����������ֻ�ܲμ�һ�����ÿ��20:00����������������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 4) 
			msg.AddMsgEvent(MsgID, 1, 211124) -- �μ���ͨ�����
			msg.AddMsgEvent(MsgID, 21, 5)
			msg.AddMsgEvent(MsgID, 1, 211125) -- �μ�רҵ�����
			msg.AddMsgEvent(MsgID, 21, 8)
			msg.AddMsgEvent(MsgID, 1, 211126) --��������׼����
			msg.DispatchRoleMsgEvent(RoleID, MsgID) 
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211162) -- ��������Ŀ�����ˣ�\n    ѡ�����������������ͨ�鱨����Ҫ20��רҵ�鱨����Ҫ50���һ��רҵ�����������ֻ�ܲμ�һ�����ÿ��20:00����������������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 8)
			msg.AddMsgEvent(MsgID, 1, 211126) --��������׼����
			msg.DispatchRoleMsgEvent(RoleID, MsgID) 
		end
		 
	elseif TalkIndex == 4 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 211163) -- ��������Ŀ�����ˣ�\n    ��ȷ��Ҫ������ͨ������𣿲μ���ͨ��֮��Ͳ��ܲμ�רҵ������ˡ�
		msg.AddMsgEvent(MsgID, 24, TargetID) 
		msg.AddMsgEvent(MsgID, 21, 6) 
		msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
		msg.AddMsgEvent(MsgID, 21, 99)
		msg.AddMsgEvent(MsgID, 1, 100002) -- ȡ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 211163) -- ��������Ŀ�����ˣ�\n    ��ȷ��Ҫ����רҵ������𣿲μ���ͨ��֮��Ͳ��ܲμ���ͨ������ˡ�
		msg.AddMsgEvent(MsgID, 24, TargetID) --
		msg.AddMsgEvent(MsgID, 21, 7) 
		msg.AddMsgEvent(MsgID, 1, 100001) -- ȷ��
		msg.AddMsgEvent(MsgID, 21, 99)
		msg.AddMsgEvent(MsgID, 1, 100002) -- ȡ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 6 then
		if x == 1 then--ҵ����
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211165) -- ��������Ŀ�����ˣ�\n    ���Ѿ�������ͨ������������ٱ����ˡ�
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif x == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211166) -- ��������Ŀ�����ˣ�\n    רҵ�����ͨ�������ֻ�ܲμ�һ��
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif lev < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211167) -- ��������Ŀ�����ˣ�\n    ��ĵȼ�����50�������ܱ���������
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif money < 200000 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211168) -- ��������Ŀ�����ˣ�\n    ��������Ҫ���ɱ����ѣ������Ϻ���û��ô��Ǯ��
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)	
		else
			role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_baoming"], 1)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_got"], 0)
			role.DecRoleSilver(MapID, InstanceID,RoleID,200000, 400)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211169) -- ��������Ŀ�����ˣ�\n    ������������ͨ��������Ѿ������ɹ���������<A>21:40</A>��ʼ����ȥ׼���μӱ�����.\n    �������ڻ�ˢ�¸���ʯ������ը��������ɱ�����÷�����<A>21:55</A>����ʱ��������߻�ùھ���
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 7 then
		if x == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211166) -- ��������Ŀ�����ˣ�\n    רҵ�����ͨ�������ֻ�ܲμ�һ��
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif x == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211170) -- ��������Ŀ�����ˣ�\n    ���Ѿ�����רҵ������������ٱ����ˡ�
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif lev < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211167) -- ��������Ŀ�����ˣ�\n    ��ĵȼ�����50�������ܱ���������
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif money < 500000 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211168) -- ��������Ŀ�����ˣ�\n    ��������Ҫ���ɱ����ѣ������Ϻ���û��ô��Ǯ��
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)	
		elseif juan < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211171) -- ��������Ŀ�����ˣ�\n    ����רҵ�����Ҫ����רҵ������
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)	
		else
			
			
				role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_baoming"], 2)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_got"], 0)
				role.RemoveFromRole(MapID, InstanceID,RoleID,4810040, 1, 400)	--ɾ��һ��������
				role.DecRoleSilver(MapID, InstanceID,RoleID,500000, 420)

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211172) -- ��������Ŀ�����ˣ�\n    ����������רҵ��������Ѿ������ɹ���������<A>21:40</A>��ʼ����ȥ׼���μӱ�����.\n    �������ڻ�ˢ�¸���ʯ������ը��������ɱ�����÷�����<A>21:55</A>����ʱ��������߻�ùھ���
				msg.AddMsgEvent(MsgID, 24, TargetID) 
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100001) -- ȡ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			
		end
	elseif TalkIndex == 8 then

		role.RoleGotoNewMap(MapID, InstanceID, RoleID, MapID, 240,3087,633)--

	end
end
aux.RegisterCreatureEvent(6040103, 7, "n6040103_OnTalk")

--10�������ʧ6040138
function chihongjuxiang_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
	local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curhour = tonumber(os.date("%H"))
	local curmin = tonumber(os.date("%M"))
	local curtime = curhour*100 + curmin
	if curtime >= 2140 and curtime <= 2155 then 
		local chihongjuxiang ={}
		chihongjuxiang[1]={157,3215,780}
		chihongjuxiang[2]={170,3220,778}
		chihongjuxiang[3]={197,3222,791}
		chihongjuxiang[4]={185,3229,809}
		chihongjuxiang[5]={169,3229,807}
		chihongjuxiang[6]={174,3222,796}
		chihongjuxiang[7]={152,3210,787}
		chihongjuxiang[8]={146,3213,770}
		chihongjuxiang[9]={139,3213,749}
		chihongjuxiang[10]={128,3204,726}
		
		for i = 1,10 do
			local chihongjuxiang1 = map.GetMapScriptData(MapID,InstanceID,1,20)
			if chihongjuxiang1 == nil or chihongjuxiang1 < 0 then 
				chihongjuxiang1 = 0
			end
			local num = 30 - chihongjuxiang1
			if num > 0 then
				map.MapCreateColCreature(MapID, InstanceID,6040127, chihongjuxiang[i][1],chihongjuxiang[i][2],chihongjuxiang[i][3], 1)
				map.SetMapScriptData(MapID,InstanceID,1,20,chihongjuxiang1+1)
			else
				break
			end
		end
		
		map.MapCreateColCreature(MapID, InstanceID,6040138, 1,1,1, 1)
		
		local weilanjuxiang = {}
		weilanjuxiang[1]={167,3214,765}
		weilanjuxiang[2]={185,3222,773}
		weilanjuxiang[3]={199,3222,743}
		
		for q = 1,3 do
			local weilanjuxiang1 = map.GetMapScriptData(MapID,InstanceID,1,25)
			if weilanjuxiang1 == nil then 
				weilanjuxiang1 = 0
			end
			local num1 = 3 - weilanjuxiang1
			if num1 > 0 then
				map.MapCreateColCreature(MapID, InstanceID,6040130, weilanjuxiang[q][1],weilanjuxiang[q][2],weilanjuxiang[q][3], 1)
				map.SetMapScriptData(MapID,InstanceID,1,25,weilanjuxiang1+1)
			else
				break
			end
		end
	end

end
aux.RegisterCreatureEvent(6040138, 13, "chihongjuxiang_OnDisappear")


function G6040127_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local chihongjuxiang = map.GetMapScriptData(MapID,InstanceID,1,20)
	local shenlvjuxiang = map.GetMapScriptData(MapID,InstanceID,1,23)
	map.SetMapScriptData(MapID,InstanceID,1,20,chihongjuxiang-1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, RoleID)
	local r = math.random(100)
	if shenlvjuxiang < 10 then
		if r <= 10 then
			map.MapCreateColCreature(MapID, InstanceID,6040131, x, y, z, 1)
			map.SetMapScriptData(MapID,InstanceID,1,23,shenlvjuxiang+1)
		end
	end
	if role.IsRoleHaveBuff(MapID, InstanceID,RoleID,59463) then
		jingjichangjifen(RoleID,10*2)
	else
		jingjichangjifen(RoleID,10)
	end
end
aux.RegisterCreatureEvent(6040127, 4, "G6040127_OnDie")

function G6040127_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
	local chihongjuxiang = map.GetMapScriptData(MapID,InstanceID,1,20)
	map.SetMapScriptData(MapID,InstanceID,1,20,chihongjuxiang-1)
end
aux.RegisterCreatureEvent(6040127, 13, "G6040127_OnDisappear")

--20�������ʧ6040138
function shenheijuxiang_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
	local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curhour = tonumber(os.date("%H"))
	local curmin = tonumber(os.date("%M"))
	local curtime = curhour*100 + curmin
	if curtime >= 2140 and curtime <= 2155 then
		local shenheijuxiang ={}
		shenheijuxiang[1] ={112,3193,740}
		shenheijuxiang[2] ={105,3194,733}
		shenheijuxiang[3] ={100,3216,779}
		shenheijuxiang[4] ={114,3216,790}
		shenheijuxiang[5] ={130,3200,795}
		shenheijuxiang[6] ={142,3204,793}
		shenheijuxiang[7] ={177,3219,733}

		for i = 1,7 do
			local shenheijuxiang1 = map.GetMapScriptData(MapID,InstanceID,1,21)
			if shenheijuxiang1 == nil or shenheijuxiang1 < 0 then
				shenheijuxiang1 = 0
			end
			local num = 15 - shenheijuxiang1
			if num > 0 then
				map.MapCreateColCreature(MapID, InstanceID,6040128, shenheijuxiang[i][1],shenheijuxiang[i][2],shenheijuxiang[i][3], 1)
				map.SetMapScriptData(MapID,InstanceID,1,21,shenheijuxiang1+1)
			else
				break
			end
		end
		map.MapCreateColCreature(MapID, InstanceID,6040139, 1,1,1, 1)
	end
end
aux.RegisterCreatureEvent(6040139, 13, "shenheijuxiang_OnDisappear")
function G6040128_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local shenheijuxiang = map.GetMapScriptData(MapID,InstanceID,1,21)
	local shenlvjuxiang = map.GetMapScriptData(MapID,InstanceID,1,23)
	map.SetMapScriptData(MapID,InstanceID,1,21,shenheijuxiang-1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, RoleID)
	local r = math.random(100)
	if shenlvjuxiang < 10 then
		if r <= 3 then
			map.MapCreateColCreature(MapID, InstanceID,6040131, x, y, z, 1)
			map.SetMapScriptData(MapID,InstanceID,1,23,shenlvjuxiang+1)
		end
	end
	if role.IsRoleHaveBuff(MapID, InstanceID,RoleID,59463) then
		jingjichangjifen(RoleID,20*2)
	else
		jingjichangjifen(RoleID,20)
	end
end
aux.RegisterCreatureEvent(6040128, 4, "G6040128_OnDie")
function G6040128_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
	local shenheijuxiang = map.GetMapScriptData(MapID,InstanceID,1,21)	
	map.SetMapScriptData(MapID,InstanceID,1,21,shenheijuxiang-1)
end
aux.RegisterCreatureEvent(6040128, 13, "G6040128_OnDisappear")
--30�����ʧ
function huanyanjuxiang_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
	local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curhour = tonumber(os.date("%H"))
	local curmin = tonumber(os.date("%M"))
	local curtime = curhour*100 + curmin
	if curtime >= 2140 and curtime <= 2155 then
		local huangyanjuxiang ={}
		huangyanjuxiang[1] ={163,3257,725}
		huangyanjuxiang[2] ={167,3202,820}
		
		for i = 1,2 do
			local huangyanjuxiang1 = map.GetMapScriptData(MapID,InstanceID,1,22)
			if huangyanjuxiang1 == nil or huangyanjuxiang1 < 0 then
				huangyanjuxiang1 = 0 
			end
			local num = 5 - huangyanjuxiang1
			if num > 0 then
				map.MapCreateColCreature(MapID, InstanceID,6040129, huangyanjuxiang[i][1],huangyanjuxiang[i][2],huangyanjuxiang[i][3], 1)
				map.SetMapScriptData(MapID,InstanceID,1,22,huangyanjuxiang1+1)
			else 
				break
			end
		end
		map.MapCreateColCreature(MapID, InstanceID,6040140, 1,1,1, 1)
	end
end
aux.RegisterCreatureEvent(6040140, 13, "huanyanjuxiang_OnDisappear")

function G6040129_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local huanyanjuxiang = map.GetMapScriptData(MapID,InstanceID,1,22)
	map.SetMapScriptData(MapID,InstanceID,1,22,huanyanjuxiang-1)
	if role.IsRoleHaveBuff(MapID, InstanceID,RoleID,59463) then
		jingjichangjifen(RoleID,30*2)
	else
		jingjichangjifen(RoleID,30)
	end
end
aux.RegisterCreatureEvent(6040129, 4, "G6040129_OnDie")
function G6040129_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
	local huanyanjuxiang = map.GetMapScriptData(MapID,InstanceID,1,22)	
	map.SetMapScriptData(MapID,InstanceID,1,22,huanyanjuxiang-1)
end
aux.RegisterCreatureEvent(6040129, 13, "G6040129_OnDisappear")
--ε����������ʧ
function G6040130_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local weilanjuxiang = map.GetMapScriptData(MapID,InstanceID,1,25)
	map.SetMapScriptData(MapID,InstanceID,1,23,weilanjuxiang-1)
	unit.AddBuff(MapID, InstanceID, RoleID,5946301,TargetTypeID)
end
aux.RegisterCreatureEvent(6040130, 4, "G6040130_OnDie")
function G6040130_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
	local weilanjuxiang = map.GetMapScriptData(MapID,InstanceID,1,25)
	map.SetMapScriptData(MapID,InstanceID,1,20,weilanjuxiang-1)
end
aux.RegisterCreatureEvent(6040130, 13, "G6040130_OnDisappear")
--���̹�������ʧ
function G6040131_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local shenlvjuxiang = map.GetMapScriptData(MapID,InstanceID,1,23)
	map.SetMapScriptData(MapID,InstanceID,1,23,shenlvjuxiang-1)
	if role.IsRoleHaveBuff(MapID, InstanceID,RoleID,59463) then
		jingjichangjifen(RoleID,100*2)
	else
		jingjichangjifen(RoleID,100)
	end
end
aux.RegisterCreatureEvent(6040131, 4, "G6040131_OnDie")
function G6040130_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
	local shenlvjuxiang = map.GetMapScriptData(MapID,InstanceID,1,23)
	map.SetMapScriptData(MapID,InstanceID,1,20,shenlvjuxiang-1)
end
aux.RegisterCreatureEvent(6040131, 13, "G6040131_OnDisappear")

function jingjichangjifen(RoleID,num)
	local curhour = tonumber(os.date("%H"))
	local curmin = tonumber(os.date("%M"))
	local curtime = curhour*100 + curmin
	if curtime >=2140 and curtime <= 2200 then
		local MapID = 3390517288
		local InstanceID = -1
		local i = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_jifen"])
		if i == nil then 
		    i = 0
		end
		for q=30,32 do                       --�ݴ����޸�
		    if map.GetMapScriptData(MapID,InstanceID,1,q) == RoleID then
			    if map.GetMapScriptData(MapID,InstanceID,1,q+3) >= i then
				    i = map.GetMapScriptData(MapID,InstanceID,1,q+3)
				end
			end
		end
		
		i = i + num
		role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunjinjichang_jifen"], i)
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 211205)--��ǰ����Ϊ
		msg.AddMsgEvent(QuestID1, 9, i)--��ǰ����Ϊ
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)

		local arrID = {}
		local arrNum = {}
		local bNew = 1
		for l=30,32 do
			arrID[l] = map.GetMapScriptData(MapID,InstanceID,1,l)
			if arrID[l] == nil then
			    arrID[l] = 0
			end
			if arrID[l] == RoleID  then
				arrNum[l] = i
				bNew = 0
			else
				arrNum[l] = map.GetMapScriptData(MapID,InstanceID,1,l+3)
				if arrNum[l] == nil then
				arrNum[l] = 0
			    end
			end
		end

		if bNew == 1 then
			arrID[33] = RoleID;
			arrNum[33] = i;
		else
			arrID[33] = 0;
			arrNum[33] = 0;
		end

		for m=30, 32 do
			for n=m+1,33 do
				if arrNum[n] > arrNum[m] then
					local temp = arrNum[m]
					arrNum[m] = arrNum[n]
					arrNum[n] = temp
					temp = arrID[m]
					arrID[m] = arrID[n]
					arrID[n] = temp
				end
			end
		end
		for j=30,32 do
			map.SetMapScriptData(MapID,InstanceID,1,j,arrID[j])
			map.SetMapScriptData(MapID,InstanceID,1,j+3,arrNum[j])
			
		end
	end
end