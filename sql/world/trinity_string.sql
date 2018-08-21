DELETE FROM `trinity_string` WHERE `entry` BETWEEN 40037 AND 40046;
INSERT INTO `trinity_string`(`entry`, `content_default`, `content_loc8`) VALUES 
(40037, 'Player %s have %.2f point warning', 'Игрок %s имеет %.2f очков подозрительности'),
(40038, 'Player %s is offline', 'Игрок %s не в сети'),
(40039, '%s\n- All: %.2f\n- Total account time: %.2f\n- Average item level: %.2f\n- Free talent: %.2f\n- Completed quest: %.2f\n- Friend list: %.2f\n- Money: %.2f\n- Honor and kills: %.2f\n- Trainer spells: %.2f\n- Warning zone: %.2f\n- Ip: %.2f\n- Profession: %.2f\n- Join Account: %.2f\n- Join Character: %.2f', '%s\n- Всего: %.2f\n- Игровое время аккаунта: %.2f\n- Средний уровень предметов: %.2f\n- Не использованые таланты: %.2f\n- Завершённые квесты: %.2f\n- Список друзей: %.2f\n- Деньги: %.2f\n- Хонор и килы: %.2f\n- Обучение у тренера: %.2f\n- Подозрительная зона: %.2f\n- Айпи: %.2f\n- Профессии: %.2f\n- Дата создания аккаунта: %.2f\n- Дата создания персонажа: %.2f'),
(40040, 'Warning zone list: ', 'Список подозрительных зон:'),
(40041, '|cff00ff00You must enter a zoneid|r', 'Введите айди зоны'),
(40042, '|cff00ff00Entered not correct zoneid:|r %u', '|cff00ff00Введён неверный айди зоны:|r %u'),
(40043, '|cff00ff00You added new warning zone:|r %u - %s', '|cff00ff00Вы добавили подозрительную зону:|r %u - %s'),
(40044, '|cff00ff00Not found in warning zone:|r %u - %s', '|cff00ff00Не найдено среди подозрительных зон:|r %u - %s'),
(40045, '|cff00ff00You delete warning zone:|r %u - %s', '|cff00ff00Вы удалили подозрительную зону:|r %u - %s'),
(40046, '|cff00ff00This warning zone already exists:|r %u - %s', '|cff00ff00Такая подозрительная зона уже существует:|r %u - %s');