--��������˫������Ҳ���ʹ�þ��������

function ShuangBeiFu_canUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local GM_Exp_Eff = aux.GetGMExpRate()
	if(GM_Exp_Eff >= 2) then
		--�������Ѿ������౶�������ʹ�ô�����Ʒ
		bRet = 71
		return bRet, bIgnore
	end
end

aux.RegisterItemEvent(3070024, 0, "ShuangBeiFu_canUse")--3070024	С�����                  
aux.RegisterItemEvent(3070025, 0, "ShuangBeiFu_canUse")--3070025	С�����                  
aux.RegisterItemEvent(3070026, 0, "ShuangBeiFu_canUse")--3070026	�����                      
aux.RegisterItemEvent(3070027, 0, "ShuangBeiFu_canUse")--3070027	�����                      
aux.RegisterItemEvent(3070028, 0, "ShuangBeiFu_canUse")--3070028	���������               
aux.RegisterItemEvent(3070029, 0, "ShuangBeiFu_canUse")--3070029	���������               
aux.RegisterItemEvent(3070061, 0, "ShuangBeiFu_canUse")--3070061	�����                      
aux.RegisterItemEvent(5600001, 0, "ShuangBeiFu_canUse")--5600001	���������[2Сʱ]    
aux.RegisterItemEvent(5600002, 0, "ShuangBeiFu_canUse")--5600002	���������[2Сʱ]    
aux.RegisterItemEvent(5600003, 0, "ShuangBeiFu_canUse")--5600003	���������[6Сʱ]    
aux.RegisterItemEvent(5600004, 0, "ShuangBeiFu_canUse")--5600004	���������[6Сʱ]    
aux.RegisterItemEvent(5600005, 0, "ShuangBeiFu_canUse")--5600005	���������[12Сʱ]  
aux.RegisterItemEvent(5600006, 0, "ShuangBeiFu_canUse")--5600006	���������[12Сʱ]  
aux.RegisterItemEvent(5600007, 0, "ShuangBeiFu_canUse")--5600007	���������[����]      
aux.RegisterItemEvent(5600008, 0, "ShuangBeiFu_canUse")--5600008	���������[����]




