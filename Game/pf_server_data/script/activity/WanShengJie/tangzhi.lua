
--����ÿ�������϶���¼����ֽ�������������Ҫ������������û��
--��Ҫ�������ϼ�¼��ݣ�ÿ����һ��һ���µ���ֽ�����ж���ݣ�
--�����ݺͼ�¼��һ��������ֽ���������㣬Ȼ����ʾ��
function HuoDeTangZhi(RoleID)
    local i = role.GetRoleScriptData(RoleID, 1, RoleDataType["TangZhi"])
	if i == nil then
	    i = 0
	end
	for q=0,9 do                       --�ݴ����޸�
	    if act.GetActScriptData(30, 1, q) == RoleID then
		    if act.GetActScriptData(30, 1, q+10) >= i then
			    i = act.GetActScriptData(30, 1, q+10)
			end
		end
	end
	i = i + 1
	role.SetRoleScriptData(RoleID, 1, RoleDataType["TangZhi"], i)

	if i ~= nil and i % 100 == 0 and i ~= 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 206218)  --<�����>�Ѿ��ۼ��ռ��ˡ���������ֽ���ٳԵ��ǣ�С��������
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 9, i)
		msg.DispatchWorldMsgEvent(MsgID)
    end

	local arrID = {}
	local arrNum = {}
	local bNew = 1
	for l=0,9 do
		arrID[l] = act.GetActScriptData(30, 1, l)
		if arrID[l] == nil then
		    arrID[l] = 0
		end
		if arrID[l] == RoleID  then
			arrNum[l] = i
			bNew = 0
		else
			arrNum[l] = act.GetActScriptData(30, 1, l+10)
			if arrNum[l] == nil then
		        arrNum[l] = 0
		    end
		end
	end

	if bNew == 1 then
		arrID[10] = RoleID;
		arrNum[10] = i;
	else
		arrID[10] = 0;
		arrNum[10] = 0;
	end

	for m=0, 9 do
		for n=m+1,10 do
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

	for j=0,9 do
		act.SetActScriptData(30, 1, j, arrID[j])
		act.SetActScriptData(30, 1, j+10, arrNum[j])
	end

	act.SaveActScriptData(30)

end

