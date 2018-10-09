DROP TABLE IF EXISTS `amas_player_info`;
CREATE TABLE `amas_player_info` (
  `PlayerGUID` int(15) NOT NULL,
  `TotalWarningPoint` float NOT NULL DEFAULT '0',
  `AverageItemLevel` smallint(10) NOT NULL DEFAULT '0',
  `IP` varchar(15) NOT NULL,
  `CompletedQuests` smallint(10) NOT NULL DEFAULT '0',
  `MissingSpells` tinyint(5) NOT NULL DEFAULT '0',
  `ProfessionLearned` tinyint(5) NOT NULL DEFAULT '0',
  `FreeTalent` tinyint(5) NOT NULL DEFAULT '0',
  `SessionNumber` int(10) NOT NULL DEFAULT '1',
  PRIMARY KEY (`PlayerGUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;