# ConfigReader - static library libconfig.a для чтения и интерпритации конфигов.<br>
(shared библиотека будет позже)<br>
В папке test исходный текст тестовой программы для использования библиотеки<br>

## Структура папок:<br>
```
├── bin                  // для бинарников (библиотеки и тесты)
│   ├── conf2.conf           // тестируемы конфиг (для удобства запуска находится здесь)
│   ├── libconfig.a          // собранная библиотека
│   └── test1                // тестовая программа для теста библиотеки
├── build                // для сборки всего проекта 
├── CMakeLists.txt
├── include              // все инклюды здесь
│   ├── conf.h
│   └── functions.h
├── src                  // все исходники библиотеки здксь
│   ├── conf.cpp
│   └── functions.cpp
└── test                // исходник тестовой программы
    └── main.cpp
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
Между '[' и ']' находится название секции (все что за скобкой ] игнорируется)<br>
Например: [Name Section] <br>
Начало секции с именем 'Name Section' (пробелы разрешены)<br>
<br>
Имя параметра и значение разделяется знаком '='<br>
Например: param1 = value_param<br>
Название параметра param1<br>
Значение value_param<br>
Пробелы разрешены<br>

Параметры без секции попадают в глобальную секцию с именем GLOBAL<br>
Параметр с именем 'Include' читает и добавляет в конфиг содержимое файла и значении параметра<br>
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
conf2.txt:
```
param_subsection = BBB

[subsection]
	param_subsection = aaa

```
Получается такое представление конфигурации
```
Section name: [GLOBAL]
	countInputs = 16
	LAYER 16 = relu2
	LAYER 10 = relu2
	LAYER 5 = relu2
	learn = 1
	epoch = 10
	param_subsection = BBB
Section name: [SECTION 2]
	learnDataSet = /path/
	LR = 0.1
	min_error = 10.321
	epoch = 10000
	write_log = 1
	name_weights = w_lines4
	path_weights = /path/NN/
	use_progress_ba = 0
Section name: [subsection]
	param_subsection = aaa

```
##Использование в коде (пример)

```
#include <stdlib.h>
#include <iostream>
#include "conf.h"

int main(int argc, char* argv[]){

	Conf conf;      // Создаем объект 'конфигурации'

	if(!conf.read_conf("file_conf.conf")){      // Читаем и парсим конфиг. Возвращает false, если ошибка
		std::cerr << "Error open file: file_conf.conf" << std::endl;
		return EXIT_FAILURE;
	}

	conf.print_conf();              // Печатает конфигурацию на stdout
	const std::string sec = "ИмяСекции"
	const std::string par = "ИмяПараметра"

    // Ищем параметр и возвращаем как строку
    // Еще варианты: (смотри в conf.h)

	std::string ansv; // Результат (val)
    if(conf.get_val_as_str(sec, par, val) // Есди не найдено, возврат false
        printf("\n---------\nsection: [%s] %s = %s\n---------\n", sec.c_str(), par.c_str(), ansv.c_str());
    else
        printf("Not Found\n");

	return 0;
}

```

