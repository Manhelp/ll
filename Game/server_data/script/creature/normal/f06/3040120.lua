---[0]��¼�Ƿ�ˢ������
---[1]381, 15076, 235����ĵ���  517
---[2]393, 15076, 235����ĵ���  518
---[3]405, 15076, 235����ĵ���  519
---[4]381, 15076, 247����ĵ���  520

---[5]393, 15076, 247����ĵ���  521
---[6]405, 15076, 247����ĵ���  522
---[7]381, 15076, 259����ĵ���  523
---[8]393, 15076, 259����ĵ���  524
---[9]405, 15076, 259����ĵ���  525


--[10]��λ��¼1��9����ĵ���״̬��1��ʾ��buff 0��ʾ��buff


--[11]��¼��ȵ�ˢ�����������������60

--[12]��¼�Ƿ�������


--[[


function c3040120_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	unvisable={}
	unvisable[1]=map.GetMapScriptData(MapID,InstanceID,1,1)
	unvisable[2]=map.GetMapScriptData(MapID,InstanceID,1,2)
	unvisable[3]=map.GetMapScriptData(MapID,InstanceID,1,3)
	unvisable[4]=map.GetMapScriptData(MapID,InstanceID,1,4)
	unvisable[5]=map.GetMapScriptData(MapID,InstanceID,1,5)
	unvisable[6]=map.GetMapScriptData(MapID,InstanceID,1,6)
	unvisable[7]=map.GetMapScriptData(MapID,InstanceID,1,7)
	unvisable[8]=map.GetMapScriptData(MapID,InstanceID,1,8)
	unvisable[9]=map.GetMapScriptData(MapID,InstanceID,1,9)
	

	for i=1,9 do
		map.MapDeleteCreature(MapID, InstanceID, unvisable[i])
	end
	map.SetMapScriptData(MapID,InstanceID,1,0,0)

	
end
aux.RegisterCreatureEvent(3040120, 4, "c3040120_OnDie")
]]--