--
--Apex2 Server Test 

--ɨ������һЩ����------------------------------------------------------------------------------------------
function AddScannerConfig()
		var = {};
		var.strLibFName 	= "./scannerlib.SL";
		var.strLibKey			= "39061Ecc9FA549E6A4DA2F4F445F5ECB";
		var.strMulLib     = "./multilib.SL";
		var.nOpenSwapLib  = 0;
		var.nSwapInterval = 3600000;
		var.nDefaultMsg		= 1;
		var.nStartScanner	= 1;			--��0 ������ɨ����
		var.uTryExit			= 0;			--��0 : ���ɨ���������쳣�����˳�ɨ����
		var.uSendTry			= 1;			--��0 : ���ɨ���������쳣���������������
		var.uUnlimitRet   = 1;      --��0 : ����ͻ��������Ƶķ���Ϣ��������ߵ�����ҪΪ�˲���ʹ��
		var.uScanTimeOut  = 170000; --������ʱʱ�ޣ������ڸ�ʱ�ޱ��߳�
		LuaData( "ScannerConfig", var );
end;

AddScannerConfig();

--���ɨ���������߼�--------------------------------------------------------------------------------------
function AddRetMan()
		var = {};
		var.m_uMaxSendRetTimes = 4;
		var.m_vecRet = {};
		--uDealFlag ֧��1��2
		var.m_vecRet[21] = { uScanRetID = 202, uDealFlag = 2, uKillNo = 63012 };			--jtw 4.8 wnd
		var.m_vecRet[22] = { uScanRetID = 112, uDealFlag = 1, uKillNo = 63014 };			--jtw 4.8 wnd
		var.m_vecRet[23] = { uScanRetID = 113, uDealFlag = 1, uKillNo = 63013 };			--jtw 4.8 wnd
		
		var.m_vecRet[2] = { uScanRetID = 221, uDealFlag = 1, uKillNo = 63021 };			--jtlh 3.6
		var.m_vecRet[3] = { uScanRetID = 222, uDealFlag = 1, uKillNo = 63022 };			--jqm 3.61
		var.m_vecRet[4] = { uScanRetID = 223, uDealFlag = 1, uKillNo = 63023 };			--����޸İ棺ollydbg
		--var.m_vecRet[5] = { uScanRetID = 224, uDealFlag = 1, uKillNo = 63024 };			--jtbl 3.6.3
		
		var.m_vecRet[11] = { uScanRetID = 331, uDealFlag = 1, uKillNo = 63031 };			--jqm 3.61
		var.m_vecRet[12] = { uScanRetID = 332, uDealFlag = 1, uKillNo = 63032 };			--jtbl
		
		--���VMִ��ɨ�跽ʽʧ�ܣ����������ҵ��������ߵ�
		var.m_vecVMErr = {};
		--uDealFlag֧��1
		--var.m_vecVMErr[1] = { nOpID = 1, nVMErr = 0x01, uDealFlag = 1, uKillNo = 62002 };
		
		LuaData( "RetMan", var );
end;

AddRetMan();
