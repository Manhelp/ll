---mapscriptdata[0] ��¼���������Ƿ���ˢ������
---mapscriptdata[1] ��¼��һ��boss��id

---mapscriptdata[2] ��¼�ڼ�������
---mapscriptdata[3] ��¼boss1�Ƿ�ɱ��

---mapscriptdata[11] --��¼�����Ƿ�����
---mapscriptdata[12] --��¼�����Ƿ�����
---mapscriptdata[13] --��¼���boss�Ƿ�����
---mapscriptdata[14] --��¼ڤ��boss�Ƿ�����
---mapscriptdata[23] --��¼���boss��ID
---mapscriptdata[24] --��¼ڤ��boss��ID
tianjiebaozang = {}  --��¼��籦���ID
function f08_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end


	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100121)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	-------------Ӣ�µ�̽����������������ħ���������������Ե��ǲ���а������ս�ۣ�Ϊ�������ƽ��ս���ɣ�

	local monsterhave = map.GetMapScriptData(MapID,InstanceID,1,0)
	if monsterhave==0 then


		--map.MapCreateCreature(MapID, InstanceID, 3040200, 893, 124, 238)

		local bossid = map.MapCreateCreature(MapID, InstanceID, 3040208, 857, 63, 326)
		--------��һ��boss


		map.SetMapScriptData(MapID,InstanceID,1,0,1)
		map.SetMapScriptData(MapID,InstanceID,1,1,bossid)
		map.SetMapScriptData(MapID,InstanceID,1,11,0)  --��¼�����Ƿ�����
		map.SetMapScriptData(MapID,InstanceID,1,12,0)  --��¼�����Ƿ�����
		map.SetMapScriptData(MapID,InstanceID,1,13,0)  --��¼���boss�Ƿ�����
		map.SetMapScriptData(MapID,InstanceID,1,14,0)  --��¼ڤ��boss�Ƿ�����

	end

end

--ע��
aux.RegisterMapEvent("f08", 2, "f08_OnPlayerEnter")




