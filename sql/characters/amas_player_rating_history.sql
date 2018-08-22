DROP TABLE IF EXISTS `amas_player_rating_history`;
CREATE TABLE `amas_player_rating_history`  (
  `PlayerGUID` int(11) NOT NULL,
  `WarningPointAll` float NOT NULL DEFAULT 0,
  `WarningPointTimeAcc` float NOT NULL,
  `WarningPointAverageIlvl` float NOT NULL,
  `WarningPointFreeTalent` float NOT NULL,
  `WarningPointCompletedQuest` float NOT NULL,
  `WarningPointFriend` float NOT NULL,
  `WarningPointMoney` float NOT NULL,
  `WarningPointHonorAndKills` float NOT NULL,
  `WarningPointTrainerSpells` float NOT NULL,
  `WarningPointIp` float NOT NULL,
  `WarningPointWarningZone` float NOT NULL,
  `WarningPointProfession` float NOT NULL,
  `WarningPointJoinAccount` float NOT NULL,
  `WarningPointJoinCharacter` float NOT NULL,
  `Date` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' ON UPDATE CURRENT_TIMESTAMP
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;
