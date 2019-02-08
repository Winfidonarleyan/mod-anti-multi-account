DROP TABLE IF EXISTS `account_rnd_ban`;
CREATE TABLE `account_rnd_ban`  (
  `AccountID` int(11) NOT NULL,
  `Date` TIMESTAMP NULL DEFAULT NULL,
  `Duration` varchar(10) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '0',
  `BanReason` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `Author` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`AccountID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;
