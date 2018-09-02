SET @FirstID = 40037;

DELETE FROM `trinity_string` WHERE `entry` BETWEEN @FirstID AND @FirstID + 31;
INSERT INTO `trinity_string`(`entry`, `content_default`, `content_loc8`) VALUES 
(@FirstID, 'Player %s have %.2f point warning.', 'Игрок %s имеет %.2f очков подозрительности'),
(@FirstID + 1, 'Player %s is offline and don\'t save DB.', 'Игрок %s не в сети и ещё не сохранён в БД'),
(@FirstID + 2, 'Player %s\n- All: %.2f\n- Total account time: %.2f\n- Average item level: %.2f\n- Free talent: %.2f\n- Completed quest: %.2f\n- Friend list: %.2f\n- Money: %.2f\n- Honor and kills: %.2f\n- Trainer spells: %.2f\n- Warning zone: %.2f\n- Ip: %.2f\n- Profession: %.2f\n- Join Account: %.2f\n- Join Character: %.2f.', '%s\n- Всего: %.2f\n- Игровое время аккаунта: %.2f\n- Средний уровень предметов: %.2f\n- Не использованые таланты: %.2f\n- Завершённые квесты: %.2f\n- Список друзей: %.2f\n- Деньги: %.2f\n- Хонор и килы: %.2f\n- Обучение у тренера: %.2f\n- Подозрительная зона: %.2f\n- Айпи: %.2f\n- Профессии: %.2f\n- Дата создания аккаунта: %.2f\n- Дата создания персонажа: %.2f'),
(@FirstID + 3, 'Player %s\n- Warning Points: %.2f.', 'Игрок %s\n- Очков подозрительности: %.2f'),
(@FirstID + 4, 'Warning zone list:', 'Список подозрительных зон:'),
(@FirstID + 5, '|cff00ff00You must enter a zoneID|r.', 'Введите айди зоны'),
(@FirstID + 6, '|cff00ff00Your GM level is too low:|r.', '|cff00ff00Ваш уровень ГМ низок:|r'),
(@FirstID + 7, '|cff00ff00You added a new warning zone:|r %u - %s', '|cff00ff00Вы добавили подозрительную зону:|r %u - %s'),
(@FirstID + 8, '|cff00ff00Not found in warning zone:|r %u - %s', '|cff00ff00Не найдено среди подозрительных зон:|r %u - %s'),
(@FirstID + 9, '|cff00ff00You have deleted warning zone:|r %u - %s', '|cff00ff00Вы удалили подозрительную зону:|r %u - %s'),
(@FirstID + 10, '|cff00ff00This warning zone already exists:|r %u - %s', '|cff00ff00Такая подозрительная зона уже существует:|r %u - %s'),
(@FirstID + 11, '|cff00ff00List warning zone is empty.', '|cff00ff00Список подозрительных зон пуст'),
(@FirstID + 12, '|cff00ff00Not found zoneid:|r %u', '|cff00ff00Такой зоны не существует:|r %u'),
(@FirstID + 13, 'Player %s. |cffff0000%.2f|r\n|cffff0000%.2f|r. Time played on account: %s\n|cffff0000%.2f|r. Average item level: %u\n|cffff0000%.2f|r. Unused talent points: %u\n|cffff0000%.2f|r. Completed quests count: %u\nTime played in char: %s\n|cffff0000%.2f|r. Friend count: %u\n|cffff0000%.2f|r. Money: %s\n|cffff0000%.2f|r. Honor: %u. Kills: %u\n|cffff0000%.2f|r. IP: %s\n|cffff0000%.2f|r. Missing trainer spells: %u\n|cffff0000%.2f|r. Current zone: %u - %s%s\n|cffff0000%.2f|r. Profession count: %u.', 'Игрок %s. |cffff0000%.2f|r\n|cffff0000%.2f|r. Игровое время на аккаунте: %s\n|cffff0000%.2f|r. Средний уровень предметов: %u\n|cffff0000%.2f|r. Свободных талантов: %u\n|cffff0000%.2f|r. Пройденных заданий: %u\nИгровое время на персонаже: %s\n|cffff0000%.2f|r. Количество друзей: %u\n|cffff0000%.2f|r. Деньги: %s\n|cffff0000%.2f|r. Очки чести: %u. Убийства: %u\n|cffff0000%.2f|r. Айпи: %s\n|cffff0000%.2f|r. Не изученных заклинаний: %u\n|cffff0000%.2f|r. Находится в зоне: %u - %s%s\n|cffff0000%.2f|r. Количество профессий: %u'),
(@FirstID + 14, ' (is warning zone).', ' (подозрительная зона)'),
(@FirstID + 15, 'Suspicious online players:', 'Список топа онлайн игроков по очкам подозреваемости:'),
(@FirstID + 16, 'There is no suspicious online player.', 'Такие онлайн игроки не найдены'),
(@FirstID + 17, 'Suspicious offline players:', 'Список топа оффлайн игроков по очкам подозреваемости:'),
(@FirstID + 18, 'There is no suspicious offline player.', 'Такие оффлайн игроки не найдены'),
(@FirstID + 19, '|cFFFF0000#|r |cff00ff00Need enter player name and comment.', '|cFFFF0000#|r |cff00ff00Введите имя персонажа и комментарий'),
(@FirstID + 20, '|cFFFF0000#|r |cff00ff00Type your comment in double quotes \"blabla\" .', '|cFFFF0000#|r |cff00ff00Введите комментарий'),
(@FirstID + 21, '|cFFFF0000#|r |cff00ff00Comment ID required.', '|cFFFF0000#|r |cff00ff00Укажите номер комментария'),
(@FirstID + 22, '|cFFFF0000#|r |cff00ff00Type the comment ID and then the comment itself in double quotes \"blabla\". text.', '|cFFFF0000#|r |cff00ff00Укажите номер комментария и сообщение'),
(@FirstID + 23, '|cFFFF0000#|r |cff00ff00Player name required.', '|cFFFF0000#|r |cff00ff00Введите имя персонажа'),
(@FirstID + 24, '|cFFFF0000#|r |cff00ff00Comment ID not found:|r %u.', '|cFFFF0000#|r |cff00ff00Не найден комментарий под номером:|r %u'),
(@FirstID + 25, '|cFFFF0000#|r |cff00ff00No comment found for player:|r %s.', '|cFFFF0000#|r |cff00ff00Не найдены комментарии для игрока:|r %s'),
(@FirstID + 26, '|cFFFF0000#|r |cff00ff00Entered invalid comment ID.', '|cFFFF0000#|r |cff00ff00Неправильно введён номер комментария'),
(@FirstID + 27, '|cFFFF0000#|r |cff00ff00Comment added. \n|cFFFF0000#|r |cff00ff00Character:|r %s |cff00ff00 \n|cFFFF0000#|r |cff00ff00Comment:|r |cFFFF0000%s|r.', '|cFFFF0000#|r |cff00ff00Вы добавили комментарий игроку|r %s |cff00ff00-|r |cFFFF0000%s|r'),
(@FirstID + 28, '|cFFFF0000#|r |cff00ff00Comment deleted. \n|cFFFF0000# |cff00ff00CommentID:|r %u. \n|cFFFF0000# |cff00ff00Comment:|r |cFFFF0000%s|r.', '|cFFFF0000#|r |cff00ff00Удалён комментарий. Номер:|r %u. |cff00ff00Сообщение|r |cFFFF0000%s|r'),
(@FirstID + 29, '|cFFFF0000#|r |cff00ff00Edited comment. CommentID:|r %u. |cff00ff00CommentTexts:|r |cFFFF0000%s|r -> |cFFFF0000%s|r.', '|cFFFF0000#|r |cff00ff00Замена комментария. Номер:|r %u. |cff00ff00Сообщения:|r |cFFFF0000%s|r -> |cFFFF0000%s|r'),
(@FirstID + 30, '|cFFFF0000#|r |cff00ff00List comments for player|r %s.', '|cFFFF0000#|r |cff00ff00Список комментариев для игрока|r %s'),
(@FirstID + 31, '|cFFFF0000#|r |cff00ff00CommentID:|r %u | |cff00ff00Commenter:|r %s | |cff00ff00Date:|r %s | |cff00ff00Comment:|r %s', '|cFFFF0000#|r |cff00ff00Номер:|r %u. |cff00ff00Автор:|r %s. |cff00ff00Дата:|r %s. |cff00ff00Комментарий:|r %s.');
