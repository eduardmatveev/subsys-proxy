# Subsys-Proxy
Появилась необхоимость запускать приложения из подсистемы (WSL/Docker и др) под видом приложений Windows (например если хочется собирать в подсистеме а разрабатывать на хоста машине и различные IDE требуют исполняемых файлов)<br>
В директории находятся примеры использования для проектов Eclipse CDT (все shell скрипты запускаются через sh в msys64)<br><br>

## Как работает:
proxy.exe - программа проксирования вывода/ввода и замены текста, используется после создания исполняемого файла<br>
creator.exe - программа для создания исполняемого файла на основе proxy.exe и записи параметров в конец файла<br>

```
./creator.exe ./proxy.exe dst cmd [options]
-e - exec
-q - quiet
-i - use input replace
-l log
-f replace-from-host replacement-from-host
-t replace-to-host replacement-to-host
```

## Пример:
```
./creator.exe ./proxy.exe ./tartest.exe "/c/Windows/System32/bash.exe -c 'cd #PWD# && exec /bin/tar #ARGS#'" -i -f "\\" "/" -f "/c/" "/mnt/c/" -f "C:" "/mnt/c" -l ./log.txt
```
на выходе получается tartest.exe который работает так же как /bin/tar в WSL<br><br>

## Build:<br>
### For Windows:
Install MSYS2x86_64<br>
From http://repo.msys2.org/distrib/x86_64/<br>
Add to Path environment [msys64_dir]\usr\bin<br>
From cmd<br>
```pacman -S msys/cmake msys/make msys/gcc```<br>
```sh build.sh```

### For MacOSX:
```
brew install cmake
brew install g++
sh build.sh
```

## Планы:
-Под Windows избавиться от msys (собрать через MSVC)<br>
-Разобраться с кодировкой<br>
-Разобраться с tty<br>
-Разобраться с сигналами<br>

