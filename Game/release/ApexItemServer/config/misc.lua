
--�йط���ҷ�������һЩ���ù�������ݶ����ڴ˴�
-- AddSRID2RIDMap() ���ɨ�践��ID(opid * 100 + sendid)��Ҫ�ߵ�������RunID
function AddSRID2RIDMap()
	var = {};
	var.m_vecSR2TMap = {};
	--var.m_vecSR2TMap[0] = { uSRID = 202, uRunID = 1005 };
		
	LuaData( "SR2RID", var );
end;

AddSRID2RIDMap();