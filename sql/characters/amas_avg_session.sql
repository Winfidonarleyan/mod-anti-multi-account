DROP TABLE IF EXISTS `amas_avg_session`;
CREATE TABLE `amas_avg_session`  (
  `PlayerGuid` int(11) NOT NULL DEFAULT 0,
  `SessionDurationLast` int(10) NOT NULL DEFAULT 0,
  `SessionDurationAverage` int(10) NOT NULL DEFAULT 0,
  `SessionNumber` int(10) NOT NULL DEFAULT 0,
  PRIMARY KEY (`PlayerGuid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;
