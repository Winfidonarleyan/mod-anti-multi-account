DROP TABLE IF EXISTS `amas_player_info`;
CREATE TABLE `amas_player_info` (
  `PlayerGUID` int(20) NOT NULL,
  `TotalWarningPoint` float NOT NULL DEFAULT '0',
  `TotalTimeAccount` int(20) NOT NULL DEFAULT '0',
  `AverageItemLevel` int(20) NOT NULL DEFAULT '0',
  `IP` varchar(20) NOT NULL,
  `FriendCount` int(20) NOT NULL DEFAULT '0',
  `Money` int(20) NOT NULL DEFAULT '0',
  `CompletedQuests` int(20) NOT NULL DEFAULT '0',
  `TotalTimePlayed` int(20) NOT NULL DEFAULT '0',
  `Honor` int(20) NOT NULL DEFAULT '0',
  `Kills` int(20) NOT NULL DEFAULT '0',
  `CurrentZone` int(20) NOT NULL DEFAULT '0',
  `MissingSpells` int(20) NOT NULL DEFAULT '0',
  `ProfessionLearned` int(20) NOT NULL DEFAULT '0',
  `FreeTalent` int(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`PlayerGUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;