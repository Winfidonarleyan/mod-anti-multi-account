SET @FirstID = 40037;

DELETE FROM `trinity_string` WHERE `entry` BETWEEN @FirstID AND @FirstID + 29;
INSERT INTO `trinity_string`(`entry`, `content_default`, `content_loc8`) VALUES

-- Amas general
(@FirstID, '|cFFFF0000#|r |cff00ff00AMAS disabled', '|cFFFF0000#|r |cff00ff00АМАС выключена'),

-- Announce GM
(@FirstID + 1, 'Character %s just logged in and has %.2f AMAS points.', 'Игрок %s имеет %.2f очков подозрительности'),

-- AMAS info
(@FirstID + 2, 'Character %s\n-------\nTotal points: |cffff0000%.2f|r\n|cffff0000%.2f|r - Time played on account: %s\n|cffff0000%.2f|r - Average item level: %u\n|cffff0000%.2f|r - Unused talent points: %u\n|cffff0000%.2f|r - Completed quests count: %u\n|cffff0000%.2f|r - Friend count: %u\n|cffff0000%.2f|r - Money: %s\n|cffff0000%.2f|r - Honor: %u. Kills: %u\n|cffff0000%.2f|r - IP: %s. Unique? (%s).%s%s\n|cffff0000%.2f|r - Missing trainer spells: %u\n|cffff0000%.2f|r - Current zone: %u - %s%s\n|cffff0000%.2f|r - Profession count: %u.\nTime played on character: %s\nComment count: %i', 'Игрок %s. |cffff0000%.2f|r\n|cffff0000%.2f|r - Игровое время на аккаунте: %s\n|cffff0000%.2f|r - Средний уровень предметов: %u\n|cffff0000%.2f|r - Свободных талантов: %u\n|cffff0000%.2f|r - Пройденных заданий: %u\n|cffff0000%.2f|r - Количество друзей: %u\n|cffff0000%.2f|r - Деньги: %s\n|cffff0000%.2f|r - Очки чести: %u. Убийства: %u\n|cffff0000%.2f|r - Айпи: %s. Унивальное? (%s).%s%s\n|cffff0000%.2f|r - Не изученных заклинаний: %u\n|cffff0000%.2f|r - Находится в зоне: %u - %s%s\n|cffff0000%.2f|r - Количество профессий: %u\nИгровое время на персонаже: %s\nКоличество комментариев: %i'),
(@FirstID + 3, 'Character %s is offline and has no entry in the Database.', 'Игрок %s не в сети и ещё не сохранён в БД'),

-- AMAS warning zone
(@FirstID + 4, 'Warning zone list:', 'Список подозрительных зон:'),
(@FirstID + 5, '|cff00ff00You added a new warning zone:|r %u - %s', '|cff00ff00Вы добавили подозрительную зону:|r %u - %s'),
(@FirstID + 6, '|cff00ff00Not found in warning zone:|r %u - %s', '|cff00ff00Не найдено среди подозрительных зон:|r %u - %s'),
(@FirstID + 7, '|cff00ff00You have deleted warning zone:|r %u - %s', '|cff00ff00Вы удалили подозрительную зону:|r %u - %s'),
(@FirstID + 8, '|cff00ff00This warning zone already exists:|r %u - %s', '|cff00ff00Такая подозрительная зона уже существует:|r %u - %s'),
(@FirstID + 9, '|cff00ff00List warning zone is empty.', '|cff00ff00Список подозрительных зон пуст'),
(@FirstID + 10, ' (is warning zone).', ' (подозрительная зона)'),
(@FirstID + 11, '|cff00ff00Not found zoneid:|r %u', '|cff00ff00Такой зоны не существует:|r %u'),

-- AMAS list
(@FirstID + 12, '|cFFFF0000#|r |cff00ff00Suspicious online players:|r', '|cFFFF0000#|r |cff00ff00Список топа онлайн игроков по очкам подозреваемости:|r'),
(@FirstID + 13, '|cFFFF0000#|r |cff00ff00There is no suspicious online character.|r', '|cFFFF0000#|r |cff00ff00Такие онлайн игроки не найдены.|r'),
(@FirstID + 14, '|cFFFF0000#|r |cff00ff00Suspicious offline players:|r', '|cFFFF0000#|r |cff00ff00Список топа оффлайн игроков по очкам подозреваемости:|r'),
(@FirstID + 15, '|cFFFF0000#|r |cff00ff00There is no suspicious offline character.|r', '|cFFFF0000#|r |cff00ff00Такие оффлайн игроки не найдены.|r'),

-- AMAS comment
(@FirstID + 16, '|cFFFF0000#|r |cff00ff00Enter character\'s name and type your comment.', '|cFFFF0000#|r |cff00ff00Введите имя персонажа и комментарий'),
(@FirstID + 17, '|cFFFF0000#|r |cff00ff00Type your comment after the character\'s name.', '|cFFFF0000#|r |cff00ff00Введите комментарий'),
(@FirstID + 18, '|cFFFF0000#|r |cff00ff00Comment ID required.', '|cFFFF0000#|r |cff00ff00Укажите номер комментария'),
(@FirstID + 19, '|cFFFF0000#|r |cff00ff00Type the comment ID and then your comment.', '|cFFFF0000#|r |cff00ff00Укажите номер комментария и сообщение'),
(@FirstID + 20, '|cFFFF0000#|r |cff00ff00Comment ID not found:|r %u.', '|cFFFF0000#|r |cff00ff00Не найден комментарий под номером:|r %u'),
(@FirstID + 21, '|cFFFF0000#|r |cff00ff00No comment found for character:|r %s.', '|cFFFF0000#|r |cff00ff00Не найдены комментарии для игрока:|r %s'),
(@FirstID + 22, '|cFFFF0000#|r |cff00ff00Entered invalid comment ID.', '|cFFFF0000#|r |cff00ff00Неправильно введён номер комментария'),
(@FirstID + 23, '|cFFFF0000#|r |cff00ff00Comment added. \n|cFFFF0000#|r |cff00ff00Character:|r %s |cff00ff00 \n|cFFFF0000#|r |cff00ff00Comment:|r |cFFFF0000%s|r.', '|cFFFF0000#|r |cff00ff00Вы добавили комментарий игроку|r %s |cff00ff00-|r |cFFFF0000%s|r'),
(@FirstID + 24, '|cFFFF0000#|r |cff00ff00Comment deleted. \n|cFFFF0000# |cff00ff00CommentID:|r %u. \n|cFFFF0000# |cff00ff00Comment:|r |cFFFF0000%s|r.', '|cFFFF0000#|r |cff00ff00Удалён комментарий. Номер:|r %u. |cff00ff00Сообщение|r |cFFFF0000%s|r'),
(@FirstID + 25, '|cFFFF0000#|r |cff00ff00Edited comment. CommentID:|r %u. |cff00ff00CommentTexts:|r |cFFFF0000%s|r -> |cFFFF0000%s|r.', '|cFFFF0000#|r |cff00ff00Замена комментария. Номер:|r %u. |cff00ff00Сообщения:|r |cFFFF0000%s|r -> |cFFFF0000%s|r'),
(@FirstID + 26, '|cFFFF0000#|r |cff00ff00List comments for character|r %s.', '|cFFFF0000#|r |cff00ff00Список комментариев для игрока|r %s'),
(@FirstID + 27, '|cFFFF0000#|r |cff00ff00CommentID:|r %u | |cff00ff00Commenter:|r %s | |cff00ff00Date:|r %s | |cff00ff00Comment:|r %s', '|cFFFF0000#|r |cff00ff00Номер:|r %u. |cff00ff00Автор:|r %s. |cff00ff00Дата:|r %s. |cff00ff00Комментарий:|r %s.'),

-- Amas same ip
(@FirstID + 28, ' PERFECT MATCH with account ', ' Совпадение с акками '),
(@FirstID + 29, ' Same first byte ip ', ' Cовпадение первых цифр айпи');
