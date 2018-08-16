DROP TABLE IF EXISTS `amas_warning_zone`;
CREATE TABLE `amas_warning_zone` (
  `ZoneID` int(10) NOT NULL DEFAULT '0',
  `Comment` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`ZoneID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;