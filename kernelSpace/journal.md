**Ядро операционной системы**

Из bootloader приходит готовая структура со всей информации об аппаратуре (DTB)

kernel\_init производит базовую инициализацию стека, обработчика прерываний, драйверов

Добавлена возможность в ядре переключать SBI, драйвер, находящаяся в include/kernelSpace/config.h на дебаг(debug) #define \_\_DBCN\_\_ (#undef \_\_DBCN\_\_), легаси(legacy) #define \_\_CN\_\_ (#undef \_\_CN\_\_) и UART(TX = 8 pin; RX = 10 pin) #define \_\_UART\_USE0\_\_ (#undef \_\_UART\_USE0\_\_)  в функции console\_printf


**Функции SBI (libsbi/)**
Cледующие описанные функции - интерфйес взаимодействия с SBI. Функции *sbi_debug_console_write* - записать в консоль строку указанного размера, *sbi_debug_console_read* - прочитать из консоли указанное кол-во символов, *sbi_debug_console_write_byte* - записать в консоль символ. Также есть устаревшие функции *sbi_console_putchar* - вывести символ в консоль, *sbi_console_getchar* - считать символ с клавиатуры. 

**Работа с отладкой (debug/)**
Для вывода в консоль используется функция console\_printf()
Для ввода символа используется функция console\_getchar()

TODO:


**Исключение (arch/riscV/panic.S, .../htrap.c, .../trap.S)**
Добавлен макрос PANIC для отслеживания в какой строчке и файле упала программа
Загружен обработчик исключений в регистр stveс, сообщая какой тип исключения произошел

TODO:


**Виртуальная память (arch/riscV/memory/alloc.c,  .../mmu.c)**
В alloc.c происходит выделение физической памяти из пула \_free\_ram\_start под размер страницы 4096 байт
В mmu.c создается таблица виртуальных адресов согласно правилам Sv39 (таблица которая хранит физический адрес, а ключ это виртуальный адрес. Уровень таблицы зависит от SvX. В данном случае таблица состоит из трех уровней), чтобы в дальнейшем передать таблицу в регистр satp, для активации виртуальной памяти. В таблицу записываются все сегменты (которые находятся в линкере main.lds) программы

TODO:
- Будет ли оправдано хранение мета данных в buffFreeRam?


**Процесс (process/process_control.c .../switch_context.S)**
Создание и планирования процесса (планировщик) реализован в process\_contorl.c
Переключение контекса процесса происходит в switch\_context.S
Сохранение текущего процесса в стек, а в дальнейшем в массив процессов происходит в arch/riscV/trap.S, .../htrap.c
Реализовано чтение ELF файла (process/elf.c)

TODO:
- Улучшить планировщик (scheldure) (process\_control.c)


**Системный вызов (arch/riscV/systemCalls/syscall.c)**
Происходит системный вызов с помощью асемблерной инструкции ecall в режиме пользователя
Системные вызовы используют стандарт LINUX(https://syscalls.mebeim.net/?table=riscv/64/rv64/latest)

| Номер          | Системный вызов | Описание |
| -------------- | --------------- | -------- |
| clone          | 220 (0xdc)      | Создает новый процесс | **-**
| exit           | 93 (0x5d)       | Завершает текущий процесс | **+-**
| waitpid        | 95 (0x5f)       | Переводит процесс в режим ожидания до тех пор, пока не завершатся его дочерние процессы (Ждет, пока процесс или поток завершит свою работу) | **-**
| execve         | 221 (0xdd)      | Выполняет новую программу в процессе | **-**
| getpid         | 172 (0xac)      | Получает уникальный идентификатор процесса | **-**
| openat         | 56 (0x38)       | Открыть файл или создать (или устройства) | **-**
| close          | 57 (0x39)       | Закрыть файл (или устройства) | **-**
| read           | 63 (0x3f)       | Выполняет чтение из файла (или устройства) | **-**
| write          | 64 (0x40)       | Выполняет запись в файл (или устройство) | **-**
| lseek          | 62 (0x3e)       | Меняет точку входа чтения/записи в файле | **-**
| unlinkat       | 35 (0x23)       | Удаляет файл или ссылку на файл          | **-**
| renameat2      | 276 (0x114)     | Меняет имя или местоположения файла | **-**
| mkdirat        | 34 (0x22)       | Создает каталог | **-**
|   ?            |    ?            | Удалить каталог | **-**
| chdir          | 49 (0x31)       | Изменяет текущий каталог | **-**
| statfs         | 43 (0x2b)       | Получить статус файла | **-**
| fstatfs        | 44 (0x2c)       | Получить статус открытого файла | **-**
| linkat         | 37 (0x25)       | Создать ссылку на файл | **-**
| symlinkat      | 36 (0x24)       | Получить статус открытого файла | **-**
| brk            | 214 (0xd6)      | Измненение размера сегмента данных | **-**
| mmap           | 222 (0xde)      | Создает новое отображение в виртуальном адресном пространстве процесса вызова | **-**
| clock\_gettime | 113 (0x71)      | Получить текущее время | **-**
| alarm          | ?               | Получить статус открытого файла | **-**
| getuid         | 174 (0xae)      | Получить айди пользовательского процесса | **-**
| getgid         | 176 (0xb0)      | Получить айди группы вызывающего процесса | **-**
| socket         | 198 (0xc6)      | Создать новый сокет | **-**
| bind           | 200 (0xc8)      | Привязать сокет к сетевому адресу | **-**
| listen         | 201 (0xc9)      | Помечается как сокет, который будет использоваться для приема входящих запросов на соединение с помощью accept | **-**
| accept         | 202 (0xca)      | Принимает соединение на сокете | **-**
| connect        | 203 (0xcb)      | Соединение сокета | **-**
| sendto         | 206 (0xce)      | Отправляет сообщение на сокет | **-**
| recvfrom       | 207 (0xcf)      | Получить сообщение от сокета | **-**
| umask          | 166 (0xa6)      | Изменяет права доступа/режима файла | **-**
| fchown         | 55 (0x37)       | Изменяет владельца файла | **-**

TODO:
- Реализовать базовые системные вызовы согласно стандарту LINUX(https://syscalls.mebeim.net/?table=riscv/64/rv64/latest), согласно таблице выше


**Драйвера (drivers/)**

TODO:
- Уход от SBI (Реализовать таймер)

(drivers/uart.c)
Реализован драйвер UART, с его помощью можно читать/записывать(uartRead/uartWrite) в коносоль данные

(drivers/sdmmc.c)
Реализован драйвер sd-карты. Добавлена возможность чтения/запись по секторам через NONDMA, т.е. занимое процессорное время.
Инициализация работает на самом минимальном делители частоты 0x80, передача данных на 0x08

**Файловая система (ФС) (filesystem/)**

(filesystem/fs.c)
Используется Minix 3
Начинаеся с fsInit - ищет раздел 0x83 (Minix3) - записывает смещение относительно всего диска (LBA), после записывает superBlock, минуя секцию boot и если магическое число подходит под Minix3, то успех

imap(inode map) - bitmap, содержит информацию о свободных inode, где 0 - свободен, 1 - занят (imap начинается с 2)

zmap(zone map) - bitmap, содержит информация о свободных zone(data), где 0 - свободен, 1 - занят. Начинается с first_data_zone - 1, до заразервировано под служебные данные(superBlock, imap, zmap, inode table) (zmap начинается с 2 + imap_blocks)

inode table - хранит информацию о файле(файл, диреткория, фифо и т.д) (inode table начинается с 2 + imap_blocks + zmap_blocks)

data - хранит данные которые находятся в файле (файла - его данные, директории - информация содержимого в директории: имя, inode, struct DirEntryMinix3) (data начинается с 2 + imap_blocks + zmap_blocks + ninodes * 64 / block_size или first_data_zone)

struct DirInfo - служебная структура, служит для удаления и создания файлов. Хранит в себе номер директории в которой хрантся ребенок, inode родителя, себе сектор родителя, имя ребенка

struct InodeMinix3 getInodeMinix3(long inode_num) - получение inode table. Чтобы узнать в каком блоке находится информация для inode использую формулу ((inode_num - 1) / (superBlock.block_size / sizeof(struct InodeMinix3)))
Получаю бит внутри этого блока (inode_num - 1) % (superBlock.block_size / sizeof(struct InodeMinix3))
Возвращаю структуру InodeMinix3

void putInodeMinix3(long inode_num, struct InodeMinix3 src) - запись inode table в блок inode суть такая же как и getInodeMinix3 еще добавляеться запись на диск

uint32_t processingZone(struct InodeMinix3 inode, long logBlck, uint32_t* buff) - ищет занятые зоны и возвращает сектор

uint32_t ensureZone(struct InodeMinix3 *inode, long logBlck, uint32_t* buff) - ищет свободные зоны и алоцирует его

void deleteZonesMinix3(struct InodeMinix3 inode, uint32_t* buff) - удаляет все зоны в inode

uint32_t allocZone() - ищет в zmap свободный бит и выделяет zone для данных в файле. Какая zone вычисляется по формуле superBlock.first_data_zone - 1 + i * (uint32_t)superBlock.block_size * 8 + byte * 8 + bit

int deallocZmapMinix3(uint32_t zone) - чистит bitmap zmap

uint32_t allocImap() - ищет в imap свободный бит и выделяет inode для информации о файле. Какой inode вычисляется  по формуле i * (uint32_t)superBlock.block_size * 8 + byte * 8 + bit

int deallocImapMinix3(long inode_num) - чистит bitmap imap

int allocDir(uint32_t chInode, uint32_t pInode, const char* name) - создает директорию/файл. Принимает inode которую хотим выделить(chInode), inode родителя(pInode), в которой будет хранится chInode и имя chInode(name)
Ищет выделенную зону(processingZone), если нету, то выделяет. Когда зона найдет ищет в этой зоне свободную struct DirEntryMinix3 после заполняю её

long creatFileMinix3(const char* name, long parentInode, uint16_t mode) - вспомогательная функция. Создает файл. Аллоцирует imap(allocImap) записывает на диск и вызывает allocDir

long depthMinix3(const char* path, struct DirInfo* di) - парсит path и вызывает searchPath. Начианется от root inode = 1

long searchPath(struct DirInfo* di) - ищет файлы/директории. Подсчет размера директории - считает и пустые inode

long openMinix3(const char* path, uint16_t mode) - ищет файл, если его нету и есть родитель(директория) то создает
Возвращает inode

long mkdirMinix3(const char* path, uint16_t mode) - создает директорию, если есть родитель(директория) и возвращает inode

int rmdirMinix3(const char* path) - удаляет директорию, если она пуста. Размер родителя не убавляется, особенность Minix3 (видимо чтобы поиск пути не считал все зоны)

int deleteDirMinix3(struct DirInfo di) - вспомогательная функция, которая удаляет у родителя inode ребенка.

int unlinkMinix3(const char* path) - удаляет файл. Размер родителя не убавляется, особенность Minix3 (видимо чтобы поиск пути не считал все зоны)

uint64_t readInodeMinix3(long inode_num, uint8_t* buff, uint64_t size, uint64_t offset) - записывает в буфер содержимое файла

uint64_t writeInodeMinix3(long inode_num, uint8_t* buff, uint64_t size, uint64_t offset) - записывает из буфера на файл

TODO:
Добавить attime, mtime, ctime, uid, gid
Сделать файловый дескриптор