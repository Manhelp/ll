--���ڳ�ʼͳ����Ϣ����ͳ�������û��������޶����˶�����
function AddCountInfo()
var = {};
	  var.uPersentUser    	= 100; --������˵İٷֱ�
	  var.uNumUserKillMin 	= 0; --���������Ŀ
	  var.uNumUserLoginMin  = 0; --���ٵ�¼������
	  var.uTimeInterval   	= 60000;--�޶���ʱ�䣬��ʱ����ִ�иù���,�������ʱ�����������
	  LuaData( "CountInfo", var );
end;
----------------------------------------
AddCountInfo();
