--�ڶ�ս���ˢ��NPC�������ܼң�������ṩӦ����14:00��17:00��21:00��ˢ������ʱ��Ϊ2Сʱ
--ʥ���ڻ�ڼ䣨2010-12-14��2011-1-14��  ʥ����
--ÿ�յ�12��30��16��30��20��30�Ὺʼ��6030001��

Xinniandayan_start = 0
sheng_dan_shu = 0 --��¼����ʥ����
shengdan_yanhuishang = 0
shengdan_shu = 0

function yuandan_OnTimerMin(actID)
	--local curmon = tonumber(os.date("%m"))
	--local curday = tonumber(os.date("%d"))
	local curmin = tonumber(os.date("%M"))
	local curhour = tonumber(os.date("%H"))
	--local curweekyear = tonumber(os.date("%Y"))

--ˢ��ʥ����
	if sheng_dan_shu == 0 then
		sheng_dan_shu = map.MapCreateCreature(3424073512, -1, 5600019, 616, 22900, 534)-- 28163   23510   29803
	end

    if curmin == 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 300019)    --ʥ���������ڶ�ս��ǵĵ�<563��596>��30�����ϵ���ҿ��Ե�ʥ��������ȡʥ�������ʥ��ף��
		msg.DispatchWorldMsgEvent(MsgID)
	end

--����ʥ����ˢ��
	if ((curhour == 12 or curhour == 16 or curhour == 20) and curmin >= 30) or (curhour == 13 or curhour == 17 or curhour == 21) or ((curhour == 14 or curhour == 18 or curhour == 22) and curmin < 30) then
		if shengdan_shu == 0 then
				local a = math.random(1,3)
				if a == 1 then
					shengdanshu = map.MapCreateColCreature(3424073256, -1, 6030001, 894, 2872, 1237)--�߻�֮��s03  44741   2873   61871
					unit.AddBuff(3424073256, -1, shengdanshu, 6936601, shengdanshu)
					unit.AddBuff(3424073256, -1, shengdanshu, 6936801, shengdanshu)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 300020)    --����ʥ�����Ѿ��������߻�֮����<894, 1237>�����������е�ʥ����ȥȡ����ʥ������ɣ�6030001��ȡ1000����Ʒ����ȡ���Ժ󣬻����и��������¼�������ʥ����ֻ����2Сʱ�������ץ����
					msg.DispatchWorldMsgEvent(MsgID)
				elseif a == 2 then
					shengdanshu1 = map.MapCreateColCreature(3424071976, -1, 6030001, 646, 14111, 1027)--�����ѹ�s04   32337   14170  50737
					unit.AddBuff(3424071976, -1, shengdanshu1, 6936601, shengdanshu1)
					unit.AddBuff(3424071976, -1, shengdanshu1, 6936801, shengdanshu1)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 300021)    --����ʥ�����Ѿ������ڱ����ѹȵ�<646, 1027>�����������е�ʥ����ȥȡ����ʥ������ɣ�6030001��ȡ1000����Ʒ����ȡ���Ժ󣬻����и��������¼�������ʥ����ֻ����2Сʱ�������ץ����
					msg.DispatchWorldMsgEvent(MsgID)
				elseif a == 3 then
					shengdanshu2 = map.MapCreateColCreature(3424071720, -1, 6030001, 1201, 10476, 1236)--������ѩs05    60055   10475   61821
					unit.AddBuff(3424071720, -1, shengdanshu2, 6936601, shengdanshu2)
					unit.AddBuff(3424071720, -1, shengdanshu2, 6936801, shengdanshu2)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 300022)    --����ʥ�����Ѿ������ڱ�����ѩ��<1201, 1236>�����������е�ʥ����ȥȡ����ʥ������ɣ�6030001��ȡ1000����Ʒ����ȡ���Ժ󣬻����и��������¼�������ʥ����ֻ����2Сʱ�������ץ����
					msg.DispatchWorldMsgEvent(MsgID)
				end
			shengdan_shu = 1
			end
	elseif (curhour == 14 or curhour == 18 or curhour == 22) and curmin == 30 then
	    if shengdan_shu ~= 0 then
			map.MapDeleteCreature(shengdan_shu_Map, -1, shengdan_shu)
			shengdan_shu = 0
		end
	elseif (curhour == 12 or curhour == 16 or curhour == 20) and curmin == 25 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 300023)    --����ʥ���������������߻�֮���������ѹȻ��߱�����ѩ���������е�ʥ����ȥȡ����ʥ������ɣ�����ʥ������ȡ1000����Ʒ����ȡ���Ժ󣬻����и��������¼�������ʥ����ֻ����2Сʱ�������ץ����
		msg.DispatchWorldMsgEvent(MsgID)
	else
	    shengdan_shu = 0
	end
end

--����
aux.RegisterActEvent(34, 4, "yuandan_OnTimerMin")




function Xinnian_OnTimerMin(actID)

	local curmin = tonumber(os.date("%M"))
	local curhour = tonumber(os.date("%H"))




--ˢ��NPC ��������
	if (curhour == 14 or curhour == 17 or curhour == 21) and curmin < 31 then
		if Xinniandayan_start == 0 then

			Xinniandayan_start = 1
			for q=0,14 do                       --�ݴ����޸�
				act.SetActScriptData(36, 1, q, 0)
				act.SaveActScriptData(36)
			end
		end
		--local CompNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin"])
		--role.SetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin"], 0)
        --shengdan_yanhuishang = map.MapCreateCreature(3424073512, -1, 5600015, 531, 23510, 621)-- 26583  23510  31082


	else
	    Xinniandayan_start = 0
	    if (curhour ==13 or curhour == 16 or curhour == 20 ) and curmin == 55 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 300018)    --������ṩӦ�̼��������ڶ�ս��ǵ�<531��621>��ֻҪ���������;Ϳɻ�ø��ֽ�������������ǰ��λ��һ��л������������
			msg.DispatchWorldMsgEvent(MsgID)
		elseif (curhour == 14 or curhour == 17 or curhour == 21) and curmin == 31 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 300028)    --��������Ѿ��ռ������㹻��ʳ�����׼���������͵ȴ��¸�ʱ��ε��������ɣ�
			msg.DispatchWorldMsgEvent(MsgID)
		end
	end

end
aux.RegisterActEvent(36, 4, "Xinnian_OnTimerMin")

--[[��������
function Xinniandayan_OnEnd(actID)
	local CompNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin"])
	role.SetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin"], 0)

end
aux.RegisterActEvent(38, 3, "Xinniandayan_OnEnd")
aux.RegisterActEvent(39, 3, "Xinniandayan_OnEnd")
aux.RegisterActEvent(40, 3, "Xinniandayan_OnEnd")]]

--ʥ�������
function shengdan_OnEnd(actID)
	map.MapDeleteCreature(3424073512, -1, sheng_dan_shu)--ɾ������ʥ����
end
aux.RegisterActEvent(34, 3, "shengdan_OnEnd")

--ʥ�������
function yuandan_OnEnd(actID)
    map.MapDeleteCreature(3424073512, -1, shengdan_yanhuishang)
	map.MapDeleteCreature(3424073512, -1, shixian_hecheng)--ɾ������ʥ����
	for q=0,14 do                       --�ݴ����޸�
		act.SetActScriptData(36, 1, q, 0)
		act.SaveActScriptData(36)
	end
end
aux.RegisterActEvent(36, 3, "yuandan_OnEnd")

