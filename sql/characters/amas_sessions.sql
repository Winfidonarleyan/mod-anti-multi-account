DROP TABLE IF EXISTS `amas_session`;
CREATE TABLE `amas_session` (
  `PlayerGUID` int(11) NOT NULL DEFAULT '0',
  `SessionCount` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`PlayerGUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;