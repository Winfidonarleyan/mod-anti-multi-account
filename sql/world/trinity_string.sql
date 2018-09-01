SET @FirstID = 40037;

DELETE FROM `trinity_string` WHERE `entry` BETWEEN @FirstID AND @FirstID + 18;
INSERT INTO `trinity_string`(`entry`, `content_default`, `content_loc8`) VALUES 
(@FirstID, 'Player %s have %.2f point warning', 'Игрок %s имеет %.2f очков подозрительности'),
(@FirstID + 1, 'Player %s is offline and don\'t save DB', 'Игрок %s не в сети и ещё не сохранён в БД'),
(@FirstID + 2, 'Player %s\n- All: %.2f\n- Total account time: %.2f\n- Average item level: %.2f\n- Free talent: %.2f\n- Completed quest: %.2f\n- Friend list: %.2f\n- Money: %.2f\n- Honor and kills: %.2f\n- Trainer spells: %.2f\n- Warning zone: %.2f\n- Ip: %.2f\n- Profession: %.2f\n- Join Account: %.2f\n- Join Character: %.2f', '%s\n- Всего: %.2f\n- Игровое время аккаунта: %.2f\n- Средний уровень предметов: %.2f\n- Не использованые таланты: %.2f\n- Завершённые квесты: %.2f\n- Список друзей: %.2f\n- Деньги: %.2f\n- Хонор и килы: %.2f\n- Обучение у тренера: %.2f\n- Подозрительная зона: %.2f\n- Айпи: %.2f\n- Профессии: %.2f\n- Дата создания аккаунта: %.2f\n- Дата создания персонажа: %.2f'),
(@FirstID + 3, 'Player %s\n- Warning Points: %.2f', 'Игрок %s\n- Очков подозрительности: %.2f'),
(@FirstID + 4, 'Warning zone list: ', 'Список подозрительных зон:'),
(@FirstID + 5, '|cff00ff00You must enter a zoneid|r', 'Введите айди зоны'),
(@FirstID + 6, '|cff00ff00You GMLevel is low:|r', '|cff00ff00Ваш уровень ГМ низок:|r'),
(@FirstID + 7, '|cff00ff00You added new warning zone:|r %u - %s', '|cff00ff00Вы добавили подозрительную зону:|r %u - %s'),
(@FirstID + 8, '|cff00ff00Not found in warning zone:|r %u - %s', '|cff00ff00Не найдено среди подозрительных зон:|r %u - %s'),
(@FirstID + 9, '|cff00ff00You delete warning zone:|r %u - %s', '|cff00ff00Вы удалили подозрительную зону:|r %u - %s'),
(@FirstID + 10, '|cff00ff00This warning zone already exists:|r %u - %s', '|cff00ff00Такая подозрительная зона уже существует:|r %u - %s'),
(@FirstID + 11, '|cff00ff00List warning zone is empty', '|cff00ff00Список подозрительных зон пуст'),
(@FirstID + 12, '|cff00ff00Not found zoneid:|r %u', '|cff00ff00Такой зоны не существует:|r %u'),
(@FirstID + 13, 'Player %s. |cffff0000%.2f|r\n|cffff0000%.2f|r. Time played in account: %s\n|cffff0000%.2f|r. Average item level: %u\n|cffff0000%.2f|r. Free talents: %u\n|cffff0000%.2f|r. Quest completed: %u\nTime played in char: %s\n|cffff0000%.2f|r. Friend count: %u\n|cffff0000%.2f|r. Money: %s\n|cffff0000%.2f|r. Honor: %u. Kills: %u\n|cffff0000%.2f|r. IP: %s\n|cffff0000%.2f|r. Missing trainer spells: %u\n|cffff0000%.2f|r. Current zone: %u - %s%s\n|cffff0000%.2f|r. Profession count: %u', 'Игрок %s. |cffff0000%.2f|r\n|cffff0000%.2f|r. Игровое время на аккаунте: %s\n|cffff0000%.2f|r. Средний уровень предметов: %u\n|cffff0000%.2f|r. Свободных талантов: %u\n|cffff0000%.2f|r. Пройденных заданий: %u\nИгровое время на персонаже: %s\n|cffff0000%.2f|r. Количество друзей: %u\n|cffff0000%.2f|r. Деньги: %s\n|cffff0000%.2f|r. Очки чести: %u. Убийства: %u\n|cffff0000%.2f|r. Айпи: %s\n|cffff0000%.2f|r. Не изученных заклинаний: %u\n|cffff0000%.2f|r. Находится в зоне: %u - %s%s\n|cffff0000%.2f|r. Количество профессий: %u'),
(@FirstID + 14, ' (is warning zone)', ' (подозрительная зона)'),
(@FirstID + 15, 'List top players online warning point:', 'Список топа онлайн игроков по очкам подозреваемости:'),
(@FirstID + 16, 'Suck online players is not found', 'Такие онлайн игроки не найдены'),
(@FirstID + 17, 'List top players offline warning point:', 'Список топа оффлайн игроков по очкам подозреваемости:'),
(@FirstID + 18, 'Suck offline players is not found', 'Такие оффлайн игроки не найдены');