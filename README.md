# ConfigReader - static library libconfig.a для чтения и интерпритации конфигов.<br>
(shared библиотека будет позже)<br>
В папке test исходный текст тестовой программы для использования библиотеки<br>

## Структура папок:<br>
```
├── bin                  // для бинарников (библиотеки и тесты)<br>
│   ├── conf2.conf           // тестируемы конфиг (для удобства запуска находится здесь)<br>
│   ├── libconfig.a          // собранная библиотека<br>
│   ├── test1                // тестовая программа для теста библиотеки<br>
├── build                // для сборки всего проекта <br>
├── CMakeLists.txt<br>
├── include              // все инклюды здесь<br>
│   ├── conf.h<br>
│   ├── functions.h<br>
│   └── mem.h<br>
├── src                  // все исходники библиотеки здксь<br>
│   ├── conf.c<br>
│   ├── functions.c<br>
│   └── mem.c<br>
└── test                // исходник тестовой программы<br>
    └── main.c<br>
```

## Сборка<br>
в директории проекта<br>
```
mkdir build bin
cd build
cmake ..
make
```

В директории bin будут сгенерированы libconfig.a и test1<br>

## Run<br>
В директории проекта<br>
```cd bin
./test1 config_file.conf
```

### Формат config-файла<br>
После символа '#' текст игнорируется (можно использовать как комментарий)<br>
<br>
Между '[' и ']' находится название секции (должно быть единственное в строке)<br>
Например: [Name Section] <br>
Начало секции с именем 'Name Section' (пробелы разрешены)<br>
<br>
Имя параметра и значение разделяется знаком '='<br>
Например: param1 = value_param<br>
Название параметра param1<br>
Значение value_param<br>
Пробелы разрешены<br>

Параметры без секции попадают в глобальную секцию с именем GLOBAL<br>
Параметр с именем 'Include' читает и добавляет в конфиг содержимое файла указанного ы значении параметра<br>
Пример config-файла:<br>
```
countInputs = 16
LAYER 16 = 	relu2
LAYER 10  = relu2
LAYER 5   = relu2
learn = 1
epoch = 10

[SECTION 2]
learnDataSet = /path/
LR = 0.1
min_error = 10.321
epoch = 10000

# парсится и добавляется содержимое conf2.txt
Include = conf2.txt

write_log = 1
name_weights = w_lines4
path_weights = /path/NN/
use_progress_ba =  0

```
Получается такой конфиг
```
[GLOBAL]
	countInputs = 16
	LAYER 16 = relu2
	LAYER 10 = relu2
	LAYER 5 = relu2
	learn = 1
	epoch = 10
[SECTION 2]
	learnDataSet = /home/kost/programming/
	LR = 0.1
	min_error = 10.321
	epoch = 10000
	param_subsection = BBB
	write_log = 1
	name_weights = w_lines4
	path_weights = /home/kost/programming/NN/
	use_progress_ba = 0
[subsection]
	param_subsection = aaa

```
