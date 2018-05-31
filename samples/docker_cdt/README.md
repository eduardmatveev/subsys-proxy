./project is Eclipse CDT project example proxy make, gcc, g++, gdb from docker container

Generate proxy exe
sh create-exe.sh

Build docker container and copy /usr/include, /usr/lib
sh container-build.sh

Run docker container and exec sh console
sh container-run.sh

Rename ECLIPSE_WORKSPASE\.metadata\.plugins\org.eclipse.cdt.managedbuilder.core\spec.c to ECLIPSE_WORKSPASE\.metadata\.plugins\org.eclipse.cdt.managedbuilder.core\spec.C

open ecplise project 
./project