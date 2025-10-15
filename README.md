# templater
A simple C project template generator made in c. By default the project files are generated in the directory its executed from.

**NOTE**: install.sh does not work at the moment.

## Usage

Usage: templater [-d dirname]

### Flags

* **-d** - Generate the project files in a specified directory name

## Template structure

```
build/
libs/
src/
├─ main.c
Makefile
.gitignore
```

## Makefile

The program generates a makefile for the program make that is used for compiling the project. The compiler used is gcc (GNU Compiler Collection).

## Install

To install the program, follow these steps.

* Clone the repositiory. (make sure you have git installed) \
```bash
git clone https://github.com/Samisalami05/templater.git
```
* Move into templater. \
```bash
cd templater
```
* Compile templater. (make sure you have make installed) \
```
make
```

* Copy executable to clipboard. \
```
./install.sh
```

