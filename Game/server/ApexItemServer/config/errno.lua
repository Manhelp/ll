
--Apex2ϵͳ�е�����ʱ�Ĵ�����б�.

function AddErrNo()	

				AddErrAction( 61039, 1 );	
				AddErrAction( 61061, 1 );
				AddErrAction( 61099, 1 );
				
				--AddErrAction( 47200, 1 ); --����δ����
				AddErrAction( 63012, 1 );
				--AddErrAction( 61113, 1 );
				--AddErrAction( 61112, 1 );
				AddErrAction( 61096, 1 );
				AddErrAction( 61097, 1 );
				AddErrAction( 61086, 1 );
				AddErrAction( 61087, 1 );
				--AddErrAction( 61073, 1 );
				AddErrAction( 61084, 1 );
				--AddErrAction( 61090, 1 );
				AddErrAction( 49001, 1 );
				AddErrAction( 49002, 1 );
				AddErrAction( 49003, 1 );
				--AddErrAction( 63002, 1 );
				--AddErrAction( 63001, 1 );
	
			    AddErrActionRange( 41100, 41150, 1 );
			    AddErrActionRange( 41300, 41350, 1 );
			    AddErrActionRange( 41500, 41510, 1 );
				AddErrActionRange( 43500, 43600, 1 );
				AddErrActionRange( 47000, 47050, 1 );
				AddErrActionRange( 47500, 47530, 1 );
				AddErrActionRange( 47600, 47631, 1 );
				AddErrActionRange( 42500, 42600, 1 );
				
			    DelErrNo(41101);
			    DelErrNo(41302);
			    DelErrNo(47031);
			    DelErrNo(47032);
			    
				AddKillTimes_Every(1000);     
				--AddKillTimes_One(47200,10000);
				AddKillTimes_One(61087,10000);

				SetKillSpeed_Every(30);          --��λʱ��������ÿ����������˵Ĵ���
				--SetKillSpeed_One(61039,4);			 --��λʱ���ڵ������ô�������˵Ĵ���
				--SetKillSpeed_One(47200,100);
				SetKillSpeed_One(61087,100);
				SetKillSpeed_UnitTime(1*60*1000);--�������˵�ʱ�䳤��

				
				--AddKillTimes_One(61062,400);         --�˴���ŵ����ξͲ�����,���������ʵ������趨
				--AddDelayTime(120000,3);              --��2�������д�������˴������ڵ���3�Ķ�������,����
				--SetDelayKill( 0, 3 );                --��һ����������ʱ���أ��ڶ�����������ʱƫ��
				--AddKillDelay( 61081, 10000);         --��һ����������ţ��ڶ�������������ʱʱ�������ڲ鵽��ֱ��10s����
end

AddErrNo();


