SET 
@FirstID = 40037,
@r = "|r", -- Reset color
@red = "|cFFFF0000",
@green = "|cFF00FF00",
@n = "\n"; -- New line

SET
@separator = "--------- \n", -- Line separator
@logo = CONCAT(@red, "#", @r, " ", @green), -- Winfi logo
@sytemlogo = CONCAT("[", @red, "AMAS", @r, "]", @green, " "); -- AMAS logo

DELETE FROM `trinity_string` WHERE `entry` BETWEEN @FirstID AND @FirstID + 31;
INSERT INTO `trinity_string`(`entry`, `content_default`, `content_loc8`) VALUES

-- AMAS general
(@FirstID, CONCAT(@sytemlogo, "System disabled"), CONCAT(@sytemlogo, "Система выключена")),

-- Announce GM
(@FirstID + 1, CONCAT(@sytemlogo, @green, "Character", @r, " %s ", @green, "just logged in and has", @r, " %.2f ", @green, "AMAS points.", @r), CONCAT(@sytemlogo, @green, "Игрок", @r, " %s ", @green , "имеет", @r, " %.2f ", @green, "очков подозрительности", @r)),

-- AMAS info
(@FirstID + 2, CONCAT(
"Character %s", @n, "-------", @n, "Total points: ", @red, "%.2f", @r, @n,
@red, "%.2f", @r, " - Time played on account: %s", @n,
@red, "%.2f", @r, " - Average item level: %u", @n,
@red, "%.2f", @r, " - Unused talent points: %u", @n,
@red, "%.2f", @r, " - Completed quests count: %u", @n,
@red, "%.2f", @r, " - Friend count: %u", @n,
@red, "%.2f", @r, " - Money: %s", @n,
@red, "%.2f", @r, " - Honor: %u. Kills: %u", @n,
@red, "%.2f", @r, " - IP: %s. Unique? (%s).%s%s", @n,
@red, "%.2f", @r, " - Missing trainer spells: %u", @n,
@red, "%.2f", @r, " - Current zone: %u - %s%s", @n,
@red, "%.2f", @r, " - Profession count: %u", @n,
@red, "%.2f", @r, " - Session count: %u. Avg session duration: %s", @n,
"Time played on character: %s", @n,
"Comment count: %i"), CONCAT("Игрок %s. |cffff0000%.2f|r", @n,
"-------", @n,
@red, "%.2f", @r, " - Игровое время на аккаунте: %s", @n,
@red, "%.2f", @r, " - Средний уровень предметов: %u", @n,
@red, "%.2f", @r, " - Свободных талантов: %u", @n,
@red, "%.2f", @r, " - Пройденных заданий: %u", @n,
@red, "%.2f", @r, " - Количество друзей: %u", @n,
@red, "%.2f", @r, " - Деньги: %s", @n,
@red, "%.2f", @r, " - Очки чести: %u. Убийства: %u", @n,
@red, "%.2f", @r, " - Айпи: %s. Унивальное? (%s).%s%s", @n,
@red, "%.2f", @r, " - Не изученных заклинаний: %u", @n,
@red, "%.2f", @r, " - Находится в зоне: %u - %s%s", @n,
@red, "%.2f", @r, " - Количество профессий: %u", @n,
@red, "%.2f", @r, " - Сессия. Номер: %u. В среднем: %s", @n,
"Игровое время на персонаже: %s", @n,
"Количество комментариев: %i")),
(@FirstID + 3, "Character %s is offline and has no entry in the Database.", "Игрок %s не в сети и ещё не сохранён в БД"),

-- AMAS warning zone
(@FirstID + 4, CONCAT(@green, "Warning zone list:"), CONCAT(@green, "Список подозрительных зон:")),
(@FirstID + 5, CONCAT(@green, "You added a new warning zone:", @r, " %u - %s"), CONCAT(@green, "Вы добавили подозрительную зону", @r, " %u - %s")),
(@FirstID + 6, CONCAT(@green, "Not found in warning zone", @r, " %u - %s"), CONCAT(@green, "Не найдено среди подозрительных зон", @r, " %u - %s")),
(@FirstID + 7, CONCAT(@green, "You have deleted warning zone", @r, " %u - %s"), CONCAT(@green, "Вы удалили подозрительную зону", @r, " %u - %s")),
(@FirstID + 8, CONCAT(@green, "This warning zone already exists", @r, " %u - %s"), CONCAT(@green, "Такая подозрительная зона уже существует",   @r, " %u - %s")),
(@FirstID + 9, CONCAT(@green, "List warning zone is empty."), CONCAT(@green, "Список подозрительных зон пуст")),
(@FirstID + 10, " (is warning zone).", " (подозрительная зона)"),
(@FirstID + 11, CONCAT(@green, "Not found zoneid:", @r, " %u"), CONCAT(@green, "Такой зоны не существует:", @r, " %u")),

-- AMAS list
(@FirstID + 12, CONCAT(@logo, "Suspicious online players:", @r), CONCAT(@logo, "Список топа онлайн игроков по очкам подозреваемости", @r)),
(@FirstID + 13, CONCAT(@logo, "There is no suspicious online character.", @r), CONCAT(@logo, "Такие онлайн игроки не найдены.", @r)),
(@FirstID + 14, CONCAT(@logo, "Suspicious offline players:", @r), CONCAT(@logo, "Список топа оффлайн игроков по очкам подозреваемости", @r)),
(@FirstID + 15, CONCAT(@logo, "There is no suspicious offline character.", @r), CONCAT(@logo, "Такие оффлайн игроки не найдены.", @r)),

-- AMAS comment
(@FirstID + 16, CONCAT(@logo, "Enter character's name and type your comment."), CONCAT(@logo, "Введите имя персонажа и комментарий")),
(@FirstID + 17, CONCAT(@logo, "Type your comment after the character's name."), CONCAT(@logo, "Введите комментарий")),
(@FirstID + 18, CONCAT(@logo, "Comment ID required."), CONCAT(@logo, "Укажите номер комментария")),
(@FirstID + 19, CONCAT(@logo, "Type the comment ID and then your comment."), CONCAT(@logo, "Укажите номер комментария и сообщение")),
(@FirstID + 20, CONCAT(@logo, "Comment ID not found:", @r, " %u."), CONCAT(@logo, "Не найден комментарий под номером:", @r, " %u.")),
(@FirstID + 21, CONCAT(@logo, "No comment found for character:", @r, " %s."), CONCAT(@logo, "Не найдены комментарии для игрока:", @r, " %s.")),
(@FirstID + 22, CONCAT(@logo, "Entered invalid comment ID."), CONCAT(@logo, "Неправильно введён номер комментария")),
(@FirstID + 23, CONCAT(@logo, "Comment added.", @n, @logo, "Character:", @r, " %s", @n, @logo, "Comment:", @r, @red, " %s", @r), CONCAT(@logo, "Вы добавили комментарий игроку", @r, " %s ", @green, "-", @r, @red, "%s", @r)),
(@FirstID + 24, CONCAT(@logo, "Comment deleted.", @n, @logo, "CommentID:", @r, " %u.", @n, @logo, "Comment:", @r, @red, "%s", @r, "."), CONCAT(@logo, "Удалён комментарий. Номер:", @r, " %u. ", @green, "Сообщение:", @r, @red, "%s", @r)),
(@FirstID + 25, CONCAT(@logo, "Edited comment. CommentID:", @r, " %u. ", @green, "CommentTexts: ", @r, @red, "%s", @r, " -> ", @red, "%s", @r), CONCAT(@logo, "Замена комментария. Номер:", @r, " %u. ", @green, "Сообщение:", @r, @red, "%s", @r)),
(@FirstID + 26, CONCAT(@logo, "List comments for character", @r, " %s."), CONCAT(@logo, "Список комментариев для игрока", @r, "%s")),
(@FirstID + 27, CONCAT(@logo, "CommentID:", @r, " %u | ", @green, "Commenter:", @r, " %s | ", @green, "Date:", @r, " %s | ", @green, "Comment: ", @r, " %s"), CONCAT(@logo, "Номер:", @r, " %u. ", @green, "Автор:", @r, " %s. ", @green, "Дата:", @r, " %s. ", @green, "Комментарий:", @r, "%s.")),

-- AMAS same ip
(@FirstID + 28, " PERFECT MATCH with account ", " Совпадение с акками "),
(@FirstID + 29, " Same first byte ip ", " Cовпадение первых цифр айпи"),

-- AMAS confirmed
(@FirstID + 30, CONCAT(
@separator,
@sytemlogo, @red, "MULTI-ACCOUNT USER CONFIRMED! \n",
@sytemlogo, @green, "Character Name:", @r, " %s \n",
@sytemlogo, @green, "Character Points: ", @r, "%.2f \n",
@sytemlogo, @green, "AMAS maximum points before confirmation: ", @r, " %u \n"),
CONCAT(@sytemlogo, @green, "Подтверждённый игрок", @r, " %s ", @green, "имеет", @r, " %.2f ", @green, "очков подозрительности. Это больше минимума", @r, " %u")),

-- AMAS mail
(@FirstID + 31, CONCAT("Recipient %s", @n, "- Money: %u", @n, "- Item: %u", @n, "- Only Text: %u", @n, "- Auction House: %u", @n, "- Account: %s - %u"), CONCAT("Получатель %s", @n, "- Деньги: %u", @n, "- Итемы: %u", @n, "- Только текст: %u", @n, "- Аукционный дом: %u", @n, "- Аккаунт: %s - %u"));
