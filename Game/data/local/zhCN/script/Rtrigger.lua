function OnTrigger(id)--�������������к�

	if( id==1 ) then--�۲�仯
		if( Quest.QuestIsAccepted(20118) and (not Quest.QuestCanComplete(20118)) )then
			Quest.ShowScreenInfo("��վ�ڻ���֮���ĸߴ����е���һ��а����������ڸ�����");
	elseif( id==2 ) then--̤ѩ��۲�仯
		if( Quest.QuestIsAccepted(20350) and (not Quest.QuestCanComplete(20350)) )then
			Quest.ShowScreenInfo("������̤ѩ��֮�ϣ��쿴����һЩ�µ������");
		end
end