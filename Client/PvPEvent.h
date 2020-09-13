#pragma once

struct tagRoomListChange : public tagGameEvent
{
	E_PvP_RoomType				eHallType;
	E_PvP_RoomLevel					eLevel;
	tagRoomListChange():tagGameEvent(_T("tagRoomListChange"),NULL)
	{}
};

struct tagRoomChange : public tagGameEvent
{
	E_PvP_RoomType				eHallType;
	E_PvP_RoomLevel					eLevel;
	INT16	                    nRoomID ;	//�����
	tagRoomChange():tagGameEvent(_T("tagRoomChange"),NULL)
	{}
};


struct tagInvalidRoomEvent : public tagGameEvent
{
	E_PvP_RoomType				eHallType;
	E_PvP_RoomLevel					eLevel;
	std::list<INT16>	            nRoomIDs ;	//�����
	tagInvalidRoomEvent():tagGameEvent(_T("tagInvalidRoomEvent"),NULL)
	{}
};

struct tagEnterHallEvent : public tagGameEvent
{
	E_PvP_RoomType				eHallType;
	tagEnterHallEvent():tagGameEvent(_T("tagEnterHallEvent"),NULL)
	{}
};
