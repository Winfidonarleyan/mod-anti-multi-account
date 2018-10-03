DROP TABLE IF EXISTS `amas_sessions`;
CREATE TABLE `amas_sessions` (
  `CharacterGUID` int(11) NOT NULL DEFAULT '0',
  `SessionCount` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`CharacterGUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;