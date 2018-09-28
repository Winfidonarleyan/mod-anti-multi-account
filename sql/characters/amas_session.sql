DROP TABLE IF EXISTS `amas_session`;
CREATE TABLE `amas_session` (
  `PlayerGuid` int(11) NOT NULL DEFAULT '0',
  `SessionNumber` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`PlayerGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;