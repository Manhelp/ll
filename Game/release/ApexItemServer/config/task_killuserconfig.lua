--������������������
--�������ߣ�szj
--�����ܣ���ĳ�������ڹ涨��ʱ����δ����ɣ��ͽ����ߵ�
--ʹ��Ƶ�ȣ�����
----------------------------- KillUser_szj-------------------------------------------
function AddTask_KillUser( nRunID, nKillNo, nFirExeTime )
	var = {};
	var.m_uSortID	 	  = 0x09;						--���ܸģ�Ҫ�ľ�Ҫ��Ӧ�ĸĳ���
	var.m_uRunID		  = nRunID; 				--1;		--���������ڼ�Ψһ��������ʱע���޸�
	var.m_uExeProb		= 100;
	var.m_uExeTimes		= 1;
	var.m_uLogTimes		= 1;
	var.m_uTimeOut		= 255;		  			--255˵��������������ĳ�ʱ��Ҳ����û�з���	
	var.m_uFirExeTime	= nFirExeTime;		--200;		--��һ��ִ��ʱ�䣬�������������һ����Χ���Գ�һ�㣬
									                    --�Է������ͻ������п���ԭ�򣬵���ʱ��ܳ���
	var.m_uExeInterval= 0;
	var.m_uWinSysLimit= 0x00;
	var.m_uLoadFlag		= 0x01;
	var.m_setOS       = {0x23,0x56,0x78}; 

	--���˵Ĵ����
	--�Ժ�����ӱ�Ĳ���������	
	var.m_pPara = {};
	var.m_pPara.m_nKillNo = nKillNo; 		--63011;
	
	LuaData( "TaskQ", var );	
end;
------------------------------------------------------------
   AddTask_KillUser( 1005, 63012, 200000 );	--��ռ������