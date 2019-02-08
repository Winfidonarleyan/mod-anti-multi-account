-- ----------------------------
-- Table structure for amas_player_comment
-- ----------------------------
DROP TABLE IF EXISTS `amas_player_comment`;
CREATE TABLE `amas_player_comment`  (
  `CommentID` int(11) NOT NULL AUTO_INCREMENT,
  `PlayerGuid` int(20) NOT NULL,
  `PlayerAccount` int(20) NOT NULL,
  `Comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `Date` TIMESTAMP NULL DEFAULT NULL,
  `CommenterGuid` int(20) NOT NULL DEFAULT 0,
  `CommenterAccount` int(20) NOT NULL,
  PRIMARY KEY (`CommentID`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 10 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;