
--����������----------------------------------------
function AddDriverConfig()
	var = {};
	var.m_uSortID		 			= 0x10;
	var.m_uRunID					= 3000;
	var.m_uExeProb		    = 100;
	var.m_uExeTimes		    = 1;  
	var.m_uLogTimes		    = 1;
	var.m_uTimeOut		    = 255;
	var.m_uFirExeTime	    = 0;
	var.m_uExeInterval    = 0;
	var.m_uWinSysLimit    = 1;
	var.m_uLoadFlag	      = 0x01;	
	var.m_setOS           = {0x13, 0x14, 0x15, 0x16, 0x20, 0x21, 0x22, 0x23, 0x24, 0x30, 0x31, 0x32, 0x33 ,0x40, 0x41, 0x42, 0x43, 0x50, 0x51};   


	
	var.m_pPara = {};
	var.m_pPara.m_nCltDrv = {};
	
	var.m_pPara.m_nCltDrv.uLoadDrv			= 1;					--0: �����ط��������, 1:����
	var.m_pPara.m_nCltDrv.uIsProtcet		= 1;			 		--0: ��������1������
	var.m_pPara.m_nCltDrv.uCallFunc			= 1;					--0: ֱ�ӷ��� 1: ���µ���(uCallFuncΪ0ʱ��uIsProtcetû������)
	var.m_pPara.m_nCltDrv.uOpCallRawFunc	= 1;					--0: OpenProcess����ԭʼ 1: ��ԭʼ
	
	var.m_pPara.m_nCltDrv.uHookUSInStart	= 1;					--0: ��Hook NtUserSendInput��1��Hook
	var.m_pPara.m_nCltDrv.uUSCallRawFunc	= 1;					--0: NtUserSendInput����ԭʼ 1: ��ԭʼ
	var.m_pPara.m_nCltDrv.uHookId_1			= 100;				    --100: ��ЧID�� ��������hook����
	var.m_pPara.m_nCltDrv.uHookId_2		    = 100;				    --100: ��ЧID�� ��������hook����
	
	var.m_pPara.m_nCltDrv.uIsGetApiData		= 0;					--0: ����ȡ�ں�API���ݣ�1����ȡ	
	var.m_pPara.m_nCltDrv.uIsGetNtoskrnlApi = 0;					--0: ����ȡNtoskrnl.exe��API��Ϣ, 1:��ȡ
	var.m_pPara.m_nCltDrv.vectNtoskrnlApi 	= {
																							"KeAttachProcess",	
																							"KeDetachProcess", 
																							"ZwWriteVirtualMemory",												
											};

	var.m_pPara.m_nCltDrv.uIsOpenKernelTickCheck	= 1; 			--0: ������CheckTickCount, 1: ����
	var.m_pPara.m_nCltDrv.uKernelTickCheck_T		= 10000; 		--���Tick����ʱ����
	var.m_pPara.m_nCltDrv.uDelt						= 50;			--����ƫ��
	var.m_pPara.m_nCltDrv.uRate 					= 100;			--TickCount��ֵ
	var.m_pPara.m_nCltDrv.uKernelTickCheckLog		= 0;            --0:API��������,1:������
	
	
	
	
	var.m_pPara.m_nCltDrv.uDrvVer					= 99;		--�����İ汾��,������������޸ģ����һ
	
	var.m_pPara.m_nCltDrv.uKernelTickCheckCltOver 	= 1000;		--���ͻ��˽�����ʱ����
		
	var.m_pPara.m_nCltDrv.uIsUseSendTick 		= 0; 		--1: user�㷢��Tick  0��������(uIsUseSendTickΪ �� uIsGetTickData ����ҲΪ ��)
	var.m_pPara.m_nCltDrv.uIsGetTickData 		= 0;		--1: tickʱ������				0������
	var.m_pPara.m_nCltDrv.uIsKernelTickCheck 	= 0; 				--1: Tickʱ�ں˼�⣬0�������
	var.m_pPara.m_nCltDrv.uIsGetOrigAddress		= 0;				--1: ��ԭʼ��ַ
	var.m_pPara.m_nCltDrv.uIsStopService		= 1;				--1��ֹͣ��������0����ͣ
	var.m_pPara.m_nCltDrv.uIsUnloadDrv			= 0;				--1: ж��������0����ж
	var.m_pPara.m_nCltDrv.uPwdDrvDone			= 0;				--1: ����pwdprotect������0��������
	var.m_pPara.m_nCltDrv.uNewIrpDone			= 0;				--1: �����µ�IRP,0: ������
	var.m_pPara.m_nCltDrv.nbGetRawFunTabAddr	= 1;				--1: ��ԭʼ������ַ��ĵ�ַ	0������
	var.m_pPara.m_nCltDrv.nbGetRawAddr			= 0;				--1: ����������ԭʼ��ַ		0������
	var.m_pPara.m_nCltDrv.uIsStartProcMonitor	= 0;				--1: �������������Ƿ�ж��	0: ������
	
	var.m_pPara.m_nCltDrv.m_vectAllApiIdx = {
																									--m_ApiId<1100 ��NTOSKRNL_API
																									--m_ApiId>=1100 ��Win32k													      
											 };	
																	          
  																						
		var.m_pPara.m_nCltDrv.m_uApiIdx_NtUserSendInput = {	-- NtUserSendInput
																											{ uOSID=0x13,nApiIdx=0x1e1 }, 	--0x10 NT	δ֪��
																											{ uOSID=0x14,nApiIdx=0x1e1 }, 
																											{ uOSID=0x15,nApiIdx=0x1e1 },
																											{ uOSID=0x16,nApiIdx=0x1e1 },
																											{ uOSID=0x20,nApiIdx=0x1e1 }, 	--0x20 WIN2000
																											{ uOSID=0x21,nApiIdx=0x1e1 }, 
																											{ uOSID=0x22,nApiIdx=0x1e1 },
																											{ uOSID=0x23,nApiIdx=0x1e1 },
																											{ uOSID=0x24,nApiIdx=0x1e1 }, 
																											{ uOSID=0x30,nApiIdx=0x1f6 }, 	--0x30 WINXP
																											{ uOSID=0x31,nApiIdx=0x1f6 },
																											{ uOSID=0x32,nApiIdx=0x1f6 },
						                                         	{ uOSID=0x33,nApiIdx=0x1f6 },																	
                                                      { uOSID=0x40,nApiIdx=0x1f4 },	--0x40 WIN2003
																											{ uOSID=0x41,nApiIdx=0x1f4 },
																											{ uOSID=0x42,nApiIdx=0x1f4 },
																											{ uOSID=0x43,nApiIdx=0x1f4 },
																											{ uOSID=0x50,nApiIdx=0x20D },--0x50  vista
																											{ uOSID=0x51,nApiIdx=0x20D },
																									 };	
	var.m_pPara.m_nCltDrv.m_uApiIdx_NtOpenProcess = 	{	-- NtOpenProcess
																											{ uOSID=0x13,nApiIdx=0x54 }, 	--0x10 NT
																											{ uOSID=0x14,nApiIdx=0x54 }, 
																											{ uOSID=0x15,nApiIdx=0x54 },
																											{ uOSID=0x16,nApiIdx=0x54 },
																											{ uOSID=0x20,nApiIdx=0x6a }, 	--0x20 WIN2000
																											{ uOSID=0x21,nApiIdx=0x6a }, 
																											{ uOSID=0x22,nApiIdx=0x6a },
																											{ uOSID=0x23,nApiIdx=0x6a },
																											{ uOSID=0x24,nApiIdx=0x6a }, 
																											{ uOSID=0x30,nApiIdx=0x7a }, 	--0x30 WINXP
																											{ uOSID=0x31,nApiIdx=0x7a },
																											{ uOSID=0x32,nApiIdx=0x7a },
                                                      { uOSID=0x33,nApiIdx=0x7a },
																											{ uOSID=0x40,nApiIdx=0x80 },	--0x40 WIN2003
																											{ uOSID=0x41,nApiIdx=0x80 },
																											{ uOSID=0x42,nApiIdx=0x80 },
																											{ uOSID=0x43,nApiIdx=0x80 },
																											{ uOSID=0x50,nApiIdx=0xc2 },--0x50  vista
																											{ uOSID=0x51,nApiIdx=0xc2 },
																										};
  var.m_pPara.m_nCltDrv.m_uApiIdx_NtReadProcessMemory = {		-- NtReadVirtualMemory
  																										{ uOSID=0x13,nApiIdx=0x89 }, 	--0x10 NT
																											{ uOSID=0x14,nApiIdx=0x89 }, 
																											{ uOSID=0x15,nApiIdx=0x89 },
																											{ uOSID=0x16,nApiIdx=0x89 },
																											{ uOSID=0x20,nApiIdx=0xa4 }, 	--0x20 WIN2000
																											{ uOSID=0x21,nApiIdx=0xa4 }, 
																											{ uOSID=0x22,nApiIdx=0xa4 },
																											{ uOSID=0x23,nApiIdx=0xa4 },
																											{ uOSID=0x24,nApiIdx=0xa4 }, 
																											{ uOSID=0x30,nApiIdx=0xba }, 	--0x30 WINXP
																											{ uOSID=0x31,nApiIdx=0xba },
																											{ uOSID=0x32,nApiIdx=0xba },
                                                      { uOSID=0x33,nApiIdx=0xba },
																											{ uOSID=0x40,nApiIdx=0xc2 },	--0x40 WIN2003
																											{ uOSID=0x41,nApiIdx=0xc2 },
																											{ uOSID=0x42,nApiIdx=0xc2 },
																											{ uOSID=0x43,nApiIdx=0xc2 },
																											{ uOSID=0x50,nApiIdx=0x105 },--0x50  vista
																											{ uOSID=0x51,nApiIdx=0x105 },
																										};
		var.m_pPara.m_nCltDrv.m_uApiIdx_NtUserSetWindowsHookEx = {	-- NtUserSetWindowsHookEx ?????
																											{ uOSID=0x13,nApiIdx=0x1e1 }, 	--0x10 NT		δ֪��
																											{ uOSID=0x14,nApiIdx=0x1e1 }, 
																											{ uOSID=0x15,nApiIdx=0x1e1 },
																											{ uOSID=0x16,nApiIdx=0x1e1 },
																											{ uOSID=0x20,nApiIdx=0x212 }, 	--0x20 WIN2000
																											{ uOSID=0x21,nApiIdx=0x212 }, 
																											{ uOSID=0x22,nApiIdx=0x212 },
																											{ uOSID=0x23,nApiIdx=0x212 },
																											{ uOSID=0x24,nApiIdx=0x212 }, 
																											{ uOSID=0x30,nApiIdx=0x225 }, 	--0x30 WINXP
																											{ uOSID=0x31,nApiIdx=0x225 },
																											{ uOSID=0x32,nApiIdx=0x225 },
                                                      { uOSID=0x33,nApiIdx=0x225 },
																											{ uOSID=0x40,nApiIdx=0x221 },	--0x40 WIN2003
																											{ uOSID=0x41,nApiIdx=0x221 },
																											{ uOSID=0x42,nApiIdx=0x221 },
																											{ uOSID=0x43,nApiIdx=0x221 },
																											{ uOSID=0x50,nApiIdx=0x23D },--0x50  vista
																									 		{ uOSID=0x51,nApiIdx=0x23D },
																									 };		

																									 
		var.m_pPara.m_nCltDrv.m_uApiIdx_NtQuerySystemInformation = {	-- NtQuerySystemInformation  ?????
																											{ uOSID=0x13,nApiIdx=0x97 }, 	--0x10 NT			δ֪��
																											{ uOSID=0x14,nApiIdx=0x97 }, 
																											{ uOSID=0x15,nApiIdx=0x97 },
																											{ uOSID=0x16,nApiIdx=0x97 },
																											{ uOSID=0x20,nApiIdx=0x97 }, 	--0x20 WIN2000
																											{ uOSID=0x21,nApiIdx=0x97 }, 
																											{ uOSID=0x22,nApiIdx=0x97 },
																											{ uOSID=0x23,nApiIdx=0x97 },
																											{ uOSID=0x24,nApiIdx=0x97 }, 
																											{ uOSID=0x30,nApiIdx=0xAD }, 	--0x30 WINXP
																											{ uOSID=0x31,nApiIdx=0xAD },
																											{ uOSID=0x32,nApiIdx=0xAD },
                                                 			{ uOSID=0x33,nApiIdx=0xAD },
																											{ uOSID=0x40,nApiIdx=0xB5 },	--0x40 WIN2003
																											{ uOSID=0x41,nApiIdx=0xB5 },
																											{ uOSID=0x42,nApiIdx=0xB5 },
																											{ uOSID=0x43,nApiIdx=0xB5 },
																											{ uOSID=0x50,nApiIdx=0xF8 },--0x50  vista
																											{ uOSID=0x51,nApiIdx=0xF8 },
																									 };																								
	 var.m_pPara.m_nCltDrv.m_uApiIdx_NtQueryVirtualMemory = {	-- NtQueryVirtualMemory ?????
																											{ uOSID=0x13,nApiIdx=0x9C }, 	--0x10 NT		δ֪��
																											{ uOSID=0x14,nApiIdx=0x9C }, 
																											{ uOSID=0x15,nApiIdx=0x9C },
																											{ uOSID=0x16,nApiIdx=0x9C },
																											{ uOSID=0x20,nApiIdx=0x9C }, 	--0x20 WIN2000
																											{ uOSID=0x21,nApiIdx=0x9C }, 
																											{ uOSID=0x22,nApiIdx=0x9C },
																											{ uOSID=0x23,nApiIdx=0x9C },
																											{ uOSID=0x24,nApiIdx=0x9C }, 
																											{ uOSID=0x30,nApiIdx=0xB2 }, 	--0x30 WINXP
																											{ uOSID=0x31,nApiIdx=0xB2 },
																											{ uOSID=0x32,nApiIdx=0xB2 },
                                                      { uOSID=0x33,nApiIdx=0xB2 },
																											{ uOSID=0x40,nApiIdx=0xBA },	--0x40 WIN2003
																											{ uOSID=0x41,nApiIdx=0xBA },
																											{ uOSID=0x42,nApiIdx=0xBA },
																											{ uOSID=0x43,nApiIdx=0xBA },
																											{ uOSID=0x50,nApiIdx=0xFD },--0x50  vista
																									    { uOSID=0x51,nApiIdx=0xFD },
																									 };
	 var.m_pPara.m_nCltDrv.m_uApiIdx_NtUserBuildHwndList = {	-- NtUserBuildHwndList ?????
																											{ uOSID=0x13,nApiIdx=0x12E }, 	--0x10 NT		δ֪��
																											{ uOSID=0x14,nApiIdx=0x12E }, 
																											{ uOSID=0x15,nApiIdx=0x12E },
																											{ uOSID=0x16,nApiIdx=0x12E },
																											{ uOSID=0x20,nApiIdx=0x12E }, 	--0x20 WIN2000
																											{ uOSID=0x21,nApiIdx=0x12E }, 
																											{ uOSID=0x22,nApiIdx=0x12E },
																											{ uOSID=0x23,nApiIdx=0x12E },
																											{ uOSID=0x24,nApiIdx=0x12E }, 
																											{ uOSID=0x30,nApiIdx=0x138 }, 	--0x30 WINXP
																											{ uOSID=0x31,nApiIdx=0x138 },
																											{ uOSID=0x32,nApiIdx=0x138 },
			                                                { uOSID=0x33,nApiIdx=0x138 },
																											{ uOSID=0x40,nApiIdx=0x137 },	--0x40 WIN2003
																											{ uOSID=0x41,nApiIdx=0x137 },
																											{ uOSID=0x42,nApiIdx=0x137 },
																											{ uOSID=0x43,nApiIdx=0x137 },
																											{ uOSID=0x50,nApiIdx=0x142 },--0x50  vista
																											{ uOSID=0x51,nApiIdx=0x142 },
																									 };	
var.m_pPara.m_nCltDrv.m_uApiIdx_NtUserQueryWindow = {	-- NtUserQueryWindow ?????
										                                 { uOSID=0x13,nApiIdx=0x1D2 }, 	--0x10 NT			δ֪��
										                                 { uOSID=0x14,nApiIdx=0x1D2 }, 
										                                 { uOSID=0x15,nApiIdx=0x1D2 },
										                                 { uOSID=0x16,nApiIdx=0x1D2 },
										                                 { uOSID=0x20,nApiIdx=0x1D2 }, 	--0x20 WIN2000
										                                 { uOSID=0x21,nApiIdx=0x1D2 }, 
										                                 { uOSID=0x22,nApiIdx=0x1D2 },
									                                 	 { uOSID=0x23,nApiIdx=0x1D2 },
										                                 { uOSID=0x24,nApiIdx=0x1D2 }, 
										                                 { uOSID=0x30,nApiIdx=0x1E3 }, 	--0x30 WINXP
										                                 { uOSID=0x31,nApiIdx=0x1E3 },
										                                 { uOSID=0x32,nApiIdx=0x1E3 },
										                                 { uOSID=0x33,nApiIdx=0x1E3 },
										                                 { uOSID=0x40,nApiIdx=0x1E1 },	--0x40 WIN2003
										                                 { uOSID=0x41,nApiIdx=0x1E1 },
										                                 { uOSID=0x42,nApiIdx=0x1E1 },
										                                 { uOSID=0x43,nApiIdx=0x1E1 },
										                                 { uOSID=0x50,nApiIdx=0x1F8 },	--0x50  vista
										                                 { uOSID=0x51,nApiIdx=0x1F8 },
									                               };	
	var.m_pPara.m_nCltDrv.m_uApiIdx_NtUserGetForegroundWindow = {	-- NtUserGetForegroundWindow ?????
																										{ uOSID=0x13,nApiIdx=0x189 }, 	--0x10 NT			δ֪��
																										{ uOSID=0x14,nApiIdx=0x189 }, 
																										{ uOSID=0x15,nApiIdx=0x189 },
																										{ uOSID=0x16,nApiIdx=0x189 },
										                                { uOSID=0x20,nApiIdx=0x189 }, 	--0x20 WIN2000
										                                { uOSID=0x21,nApiIdx=0x189 }, 
										                                { uOSID=0x22,nApiIdx=0x189 },
										                                { uOSID=0x23,nApiIdx=0x189 },
										                                { uOSID=0x24,nApiIdx=0x189 }, 
										                                { uOSID=0x30,nApiIdx=0x194 }, 	--0x30 WINXP
										                                { uOSID=0x31,nApiIdx=0x194 },
										                                { uOSID=0x32,nApiIdx=0x194 },
                                                   	{ uOSID=0x33,nApiIdx=0x194 },
										                                { uOSID=0x40,nApiIdx=0x193 },	--0x40 WIN2003
										                                { uOSID=0x41,nApiIdx=0x193 },
										                                { uOSID=0x42,nApiIdx=0x193 },
										                                { uOSID=0x43,nApiIdx=0x193 },
										                                { uOSID=0x50,nApiIdx=0x1A2 },	--0x50  vista
										                                { uOSID=0x51,nApiIdx=0x1A2 },
									                               };	
																																		 																								
	LuaData("TaskQ", var);
end;
----------------AddPerform------------------------------
AddDriverConfig();

