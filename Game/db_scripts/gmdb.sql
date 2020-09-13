/*
gmserver数据库
*/


/*create database `gmdb`;*/

/*账号表结构 */
CREATE TABLE `account` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `name` char(32) NOT NULL,
  `psd` int(11) unsigned NOT NULL,
  `privilege` int(10) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

/*权限表 */
CREATE TABLE `privilege` (              
 `MsgName` varchar(32) NOT NULL,       
 `pri_1` int(4) NOT NULL DEFAULT '0',  
 `pri_2` int(4) NOT NULL DEFAULT '0',  
 `pri_3` int(4) NOT NULL DEFAULT '0',  
 `pri_4` int(4) NOT NULL DEFAULT '0',  
 `pri_5` int(4) NOT NULL DEFAULT '0',  
 `pri_6` int(4) NOT NULL DEFAULT '0',  
 `pri_7` int(4) NOT NULL DEFAULT '0',
 `pri_8` int(4) NOT NULL DEFAULT '0',
 `pri_9` int(4) NOT NULL DEFAULT '0',
 PRIMARY KEY (`MsgName`)               
) ENGINE=InnoDB DEFAULT CHARSET=utf8; 

/*运维客户端ip绑定表 */
CREATE TABLE `rt_iptable` (                   
	`name` char(32) NOT NULL COMMENT '账号名',  
	`ip` char(30) NOT NULL COMMENT '绑定ip',    
	PRIMARY KEY (`name`)                        
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*命令权限 */
insert into `privilege` values 

('NGMTC_AccountForbid',0,1,1,1,1,0,0,0,0),
('NGMTC_AutoNotice',1,1,1,1,1,0,0,0,0),
('NGMTC_Blacklist',0,0,1,1,1,0,0,0,0),
('NGMTC_Double',0,1,1,1,1,0,0,0,0),
('NGMTC_GMCreate',0,0,0,0,1,0,1,0,0),
('NGMTC_GMDelete',0,0,0,0,1,0,1,0,0),
('NGMTC_GMModify',0,0,0,0,1,0,1,0,0),
('NGMTC_ItemCreate',0,0,0,1,1,0,0,0,0),
('NGMTC_ItemDelete',0,0,1,1,1,0,0,0,0),
('NGMTC_ItemResume',0,0,0,1,1,0,0,0,0),
('NGMTC_ProModify',0,0,0,1,1,0,0,0,0),
('NGMTC_RightNotice',1,1,1,1,1,0,0,0,0),
('NGMTC_RoleResume',0,0,1,1,1,0,0,0,0),
('NRTC_BindIP',0,0,0,0,0,0,1,0,0),
('NRTC_Challenge',1,1,1,1,1,1,1,1,1),
('NRTC_Close',0,0,0,0,0,1,1,0,0),
('NRTC_MaxNum',0,0,0,0,0,0,0,1,0),
('NRTC_Start',0,0,0,0,0,1,1,0,0),
('NRTC_SystemCmd',0,0,0,0,0,0,1,0,0),
('NRTC_UpdateMall',0,0,0,0,0,0,0,0,1),
('NGMTC_AutoChatNotice',0,1,1,1,1,0,0,0,0),
('NGMTC_CancelAutoChatNotice',0,1,1,1,1,0,0,0,0),
('NGMTC_CancelRightNotice',1,1,1,1,1,0,0,0,0),
('NGMTS_GMSetFackNumer',0,0,0,0,0,0,0,1,0),
('NGMTC_AddFilterWords',1,1,1,1,1,1,1,1,1),
('NGMTC_DelFilterWords',1,1,1,1,1,1,1,1,1);

CREATE TABLE `gm_log` (                                                                                                                                                                                                                                                                                                                                              
          `AccountID` int(10) unsigned NOT NULL COMMENT '操作人账号ID',
          `Name` char(32) NOT NULL COMMENT '操作人账号名称',
          `Time` datetime NOT NULL COMMENT 'Log记录时间',
          `OperIP` char(32) NOT NULL COMMENT '操作人IP',
          `Action` enum('create GM account','modify GM account','delete GM account','recover role','modify role attribute','create object','delete object','double','cancel double','auto notice','cancel auto notice','right notice','cancel right notice','auto chat notice','cancel auto chat notice','start server','close server','close server forced','IP bind','send system cmd') COMMENT '操作动作',
          `Details` varchar(512) NOT NULL COMMENT '操作描述'                                                                                                                                                                                                                                                                                                             
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8;
   
drop table if exists gm_server_data;      
create table gm_server_data
(
	`data_name` char(32) not null,
	`data_value` bigint(20) default 0,
	key(`data_name`) 
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
insert into gm_server_data value ('MinItemSerial',0);	

insert into `privilege` values ('NRTC_ReloadItemNeedLog',1,1,1,1,1,1,1,1,1);

############################## 1.5.14 start #############################################
insert into `privilege` values ('NGMTC_GuildGodMiracleModify',0,0,0,0,1,0,0,0,0);
############################## 1.5.14 end #############################################

############################## 1.6.1 start #############################################
##由于前面gmdb的sql更新不规范，这里统一更新
##gmtool，根据客服部要求，重新划分gmtool权限
replace into `privilege` values ('NGMTC_AccountForbid',1,1,1,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_AddFilterWords',0,0,0,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_AddNeedPKLogRole',0,0,0,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_AutoChatNotice',1,1,1,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_AutoNotice',1,1,1,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_Blacklist',0,0,0,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_CancelAutoChatNotice',0,1,1,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_CancelAutoNotice',0,1,1,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_CancelRightNotice',0,1,1,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_ClearAttPoint',0,0,0,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_DelFilterWords',0,0,0,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_DelTitle',0,0,0,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_Double',0,1,1,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_GMCreate',0,0,0,0,1,0,0,0,0);
replace into `privilege` values ('NGMTC_GMDelete',0,0,0,0,1,0,0,0,0);
replace into `privilege` values ('NGMTC_GMModify',0,0,0,0,1,0,0,0,0);
replace into `privilege` values ('NGMTC_GuildChgFund',0,0,0,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_GuildChgMemLvl',0,0,0,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_GuildChgState',0,0,0,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_IssueTitle',0,0,0,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_ItemCreate',0,0,0,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_ItemDelete',0,0,0,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_ItemResume',0,0,0,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_KickLockedRole',1,1,1,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_MoveRole',0,0,1,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_ProModify',0,0,0,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_RightNotice',1,1,1,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_RoleResume',0,0,1,1,1,0,0,0,0);
replace into `privilege` values ('NGMTC_RoleSpeak',1,1,1,1,1,0,0,0,0);

##rtclient
replace into `privilege` values ('NRTC_BindIP',0,0,0,0,0,0,0,0,1);
replace into `privilege` values ('NRTC_Challenge',0,0,0,0,0,0,0,0,1);
replace into `privilege` values ('NRTC_Close',0,0,0,0,0,0,0,0,1);
replace into `privilege` values ('NRTC_MaxNum',0,0,0,0,0,0,0,0,1);
replace into `privilege` values ('NRTC_ReloadItemNeedLog',0,0,0,0,0,0,0,0,1);
replace into `privilege` values ('NRTC_Start',0,0,0,0,0,0,0,0,1);
replace into `privilege` values ('NRTC_SystemCmd',0,0,0,0,0,0,0,0,1);
insert into `privilege` values ('NRTC_UpdateGodBid',0,0,0,0,0,0,0,0,1);
replace into `privilege` values ('NRTC_UpdateMall',0,0,0,0,0,0,0,0,1);
insert into `privilege` values ('NRTC_UpdateZaDan',0,0,0,0,0,0,0,0,1);
############################## 1.6.1 end #############################################


############################## 1.6.1 start #############################################
insert into `privilege` values 
('NGMTC_ClearSafeCode',0,0,1,0,0,0,0,0,0);
insert into `privilege` values 
('NGMTC_CloseGuard',0,0,1,0,0,0,0,0,0);
############################## 1.6.1 end #############################################