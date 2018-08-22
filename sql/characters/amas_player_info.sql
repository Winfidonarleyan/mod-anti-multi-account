DROP TABLE IF EXISTS `amas_player_info`;
CREATE TABLE `amas_player_info`  (
  `PlayerGUID` int(20) NOT NULL,
  `TotalTimeAccount` int(20) NULL DEFAULT NULL,
  `AverageItemLevel` int(20) NOT NULL DEFAULT 0,
  `IP` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `FriendCount` int(20) NOT NULL DEFAULT 0,
  `Money` int(20) NOT NULL DEFAULT 0,
  `CompletedQuests` int(20) NOT NULL DEFAULT 0,
  `TotalTimePlayed` int(20) NOT NULL DEFAULT 0,
  `Honor` int(20) NOT NULL DEFAULT 0,
  `Kills` int(20) NOT NULL DEFAULT 0,
  `CurrentZone` int(20) NOT NULL DEFAULT 0,
  `MissingSpells` int(20) NOT NULL DEFAULT 0,
  `ProfessionLearned` int(20) NOT NULL DEFAULT 0,
  `FreeTalent` int(20) NOT NULL DEFAULT 0,
  `DateCheck` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`PlayerGUID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;