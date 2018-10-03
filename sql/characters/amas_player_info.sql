DROP TABLE IF EXISTS `amas_player_info`;
CREATE TABLE `amas_player_info` (
  `PlayerGUID` int(15) NOT NULL,
  `TotalWarningPoint` float NOT NULL DEFAULT '0',
  `TotalTimeAccount` int(10) NOT NULL DEFAULT '0',
  `AverageItemLevel` smallint(10) NOT NULL DEFAULT '0',
  `IP` varchar(15) NOT NULL,
  `FriendCount` tinyint(5) NOT NULL DEFAULT '0',
  `Money` int(10) NOT NULL DEFAULT '0',
  `CompletedQuests` smallint(10) NOT NULL DEFAULT '0',
  `TotalTimePlayed` int(10) NOT NULL DEFAULT '0',
  `Honor` int(10) NOT NULL DEFAULT '0',
  `Kills` int(10) NOT NULL DEFAULT '0',
  `CurrentZone` tinyint(5) NOT NULL DEFAULT '0',
  `MissingSpells` tinyint(5) NOT NULL DEFAULT '0',
  `ProfessionLearned` tinyint(5) NOT NULL DEFAULT '0',
  `FreeTalent` tinyint(5) NOT NULL DEFAULT '0',
  PRIMARY KEY (`PlayerGUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;