# Как сдавать домашние задания

На курсе есть несколько видов активностей, которые влияют на вашу оценку:
* задачи, сданные в [Контест](https://contest.yandex.ru), в том числе задачи, разобранные на семинарах,
* задачи, по которым нужно проходить код-ревью,
* теоретические домашние задания.

## Домашние задания в контесте
Код программ, которые вы будете сдавать в рамках нашего курса, будет проверяться автоматически в системе [Яндекс.Контест](https://contest.yandex.ru).

### Требования к вводу-выводу 
Программа должна читать из стандартного входа и писать в стандартный выход. Входной формат гарантируется: будет так, как описано в условии. Строго придерживайтесь выходного формата. Лишние пробелы и лишние переводы строк &mdash; не страшно, а вот пропустить пробел, вывести не в том порядке, пропустить перевод строки или вывести что-то лишнее &mdash; будет являться ошибкой.

### Вердикты по задаче

Система выдает один из следующих результатов на каждый запуск.

* **OK** &mdash; задача прошла успешно.
* **PCF** &mdash; Precompile check failed &mdash; code style violation, вы нарушили какие-то из требований по оформлению кода. Чтобы узнать, какие именно, нажмите на ссылку отчет и почитайте. За эту ошибку не снимаются баллы. Также вы можете получить такую ошибку, если использовали при реализации какую-нибудь стандартную структуру данных или какой-нибудь стандартный алгоритм из библиотеки STL, который в данной задаче было использовать запрещено в условии задачи.
* **CE** &mdash; Compilation Error &mdash; ваша программа не компилируется на сервере. Чтобы узнать, почему, нажмите на ссылку «отчет» (она появляется при наведении курсора на строку посылки) и посмотрите, какие ошибки нашел компилятор.
* **WA** &mdash; Wrong Answer &mdash; на некотором тесте программа выдала неверный ответ. Вам не предоставляется возможность увидеть, что это был за тест. Внимательно посмотрите на ограничения в задаче, попробуйте придумать свой тест, на котором ответ будет неправильный. **-1 балл**.
* **PE** &mdash; Presentation Error &mdash; ошибка представления. Например, просили вывести число, а выведена строка. В этом случае вы можете получить **Wrong Answer** либо **Presentation error**, это не гарантируется заранее. **-1 балл**.
* **TL** &mdash; Time Limit exceeded &mdash; ваша программа работает слишком долго. Значит, у вас неправильное асимптотически решение, так как таймлимиты будут выставляться в 2&ndash;3 раза больше, чем время работы авторского решения на максимальном тесте, и этого должно хватать любому правильному решению. **-2 балла**.
* **ML** &mdash; Memory Limit exceeded &mdash; ваша программа использует слишком много памяти. **-2 балла**.
* **RE** &mdash; Runtime Error &mdash; произошла ошибка выполнения. Неочевидные возможные причины: чтение из файла вместо стандартного ввода; запись в файл вместо стандартного вывода; переполнение стека. Очевидные причины: выход за границы массива, деление на ноль. **-2 балла**.

**За тесты из условия баллы не снимаются.**


### Теоретические домашние задания
Тут всё просто &mdash; присылайте pdf файл с решением. Рукописные решения не принимаются, можно использовать [шаблон](https://www.overleaf.com/read/rrdzfpprjmpt). 
Можно присылать исправления ДО дедлайна.


## Ревью

Проверка задачи на ревью состоит из двух этапов:
1. Проверка теоретического решения задачи
2. Проверка кода программы

### Проверка теоретического решения
Присылайте pdf файл с описанием вашего решения:
* алгоритм решения
* доказательство правильности алгоритма
* временная сложность &mdash; асимптотика
* затраты памяти &mdash; асимптотика

Все пункты обязательны!
Если можете доказать, что быстрее невозможно или меньше памяти использовать невозможно, тоже пишите, это полезно. Рукописные решения не принимаются, можно использовать [шаблон](https://www.overleaf.com/read/rrdzfpprjmpt).

Ваше решение должно быть написано понятным русским языком, использовать (псевдо)код либо выражения в духе «этой переменной присваиваем то-то» не надо. Излишне подробно расписывать тоже не нужно: решение любой задачи укладывается на одну страницу A4, максимум полторы. **Ссылаться на факты, доказанные на лекции, можно и нужно (т.е. не надо заново доказывать, какая сложность у вашей сортировки).**

### Code Review

После того как сдадите код в систему, нужно будет отправить его на проверку нам.
Подробнее о том, как сдавать задания на код-ревью можно посмотреть [тут](https://docs.google.com/document/d/1-Ju9byCX3aYSUTITntxiiTDlgSPxD1brNi1-HWCp6jM/edit)

**О сроках.** На сдачу теории дается 1 неделя с момента выдачи задачи, и еще 3 недели на то, чтобы пройти все тесты в системе и прислать интерфейс работающей программы на ревью. Затем Code Review идет до тех пор, пока все замечания не будут исправлены, либо не закончится семестр. Рассчитывайте на то, что время ответа ревьюера в среднем составляет 5 рабочих дней, поэтому не затягивайте с отправкой кода и соблюдайте [стайлгайд](./styleguide.md).
Все дедлайны будут прописаны на странице курса в lms.
