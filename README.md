# Subsys-Proxy
Появилась необхоимость запускать приложения из подсистемы (WSL/Docker и др) под видом приложений Windows (собирать в подсистеме, а разрабатывать на хост машине, где различные IDE требуют исполняемых файлов)<br>
В директории samples находятся примеры использования для проектов Eclipse CDT, но можно так же использовать для любых других программ<br><br>

## Как работает:
proxy.exe - программа проксирования вывода/ввода и замены текста, используется в качестве основы для создания исполняемого файла<br>
creator.exe - программа для создания исполняемого файла и записи параметров в конец файла<br>

```
./creator.exe ./proxy.exe [dst] [cmd] [options]
[dst] - output exe
[cmd] - command execute from host, #PWD# and #ARGS# placeholders
-q - quiet
-i - use input replace
-l <log-file>
-f <replace-from-host replacement-from-host>
-t <replace-to-host replacement-to-host>
```

## Пример:
```
./creator.exe ./proxy.exe ./tartest.exe "/c/Windows/System32/bash.exe -c 'cd #PWD# && exec /bin/tar #ARGS#'" -i -f "\\" "/" -f "/c/" "/mnt/c/" -f "C:" "/mnt/c" -l ./log.txt
```
на выходе получается tartest.exe который работает так же как /bin/tar в WSL<br><br>

## Build:<br>
### For Windows:
Install MSYS2x86_64 http://repo.msys2.org/distrib/x86_64/<br>
Add to Path environment variable [msys64_dir]\usr\bin<br>
Install utils ```pacman -S msys/cmake msys/make msys/gcc```<br>
Build ```sh build.sh```<br>

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

