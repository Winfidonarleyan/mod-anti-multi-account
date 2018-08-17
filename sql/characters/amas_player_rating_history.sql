/*
 Navicat Premium Data Transfer

 Source Server         : Local
 Source Server Type    : MySQL
 Source Server Version : 50525
 Source Host           : localhost:3306
 Source Schema         : characters

 Target Server Type    : MySQL
 Target Server Version : 50525
 File Encoding         : 65001

 Date: 18/08/2018 03:35:47
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for amas_player_rating_history
-- ----------------------------
DROP TABLE IF EXISTS `amas_player_rating_history`;
CREATE TABLE `amas_player_rating_history`  (
  `PlayerGUID` int(20) NOT NULL,
  `WarningPointAll` float(20, 0) NOT NULL DEFAULT 0,
  `WarningPointTimeAcc` float(20, 0) NOT NULL,
  `WarningPointAverageIlvl` float(20, 0) NOT NULL,
  `WarningPointFreeTalent` float(20, 0) NOT NULL,
  `WarningPointCompletedQuest` float(20, 0) NOT NULL,
  `WarningPointFriend` float(20, 0) NOT NULL,
  `WarningPointMoney` float(20, 0) NOT NULL,
  `WarningPointHonorAndKills` float(20, 0) NOT NULL,
  `WarningPointTrainerSpells` float(20, 0) NOT NULL,
  `WarningPointIp` float(20, 0) NOT NULL,
  `WarningPointWarningZone` float(20, 0) NOT NULL,
  `Date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

SET FOREIGN_KEY_CHECKS = 1;
