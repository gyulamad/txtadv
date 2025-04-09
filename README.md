# MINIMALISTIC TEXT ADVENTURE GAME CONCEPT

## Install: 
just run `install.sh`, it will load ArduinoCore-API and build from source.

## Run: 
just run `main` executable.

## Grammar:
```
EPISODE <episode>
...
MEMO <memo>
IF CHOOSE <option> GOTO <episode>
IF <memo> AND CHOOSE <option> GOTO <episode>
IF NO <memo> AND CHOOSE <option> GOTO <episode>
```

## Example game:
See in `main()` function (hardcoded for now but can be externalized, for eg. read from file and pass as parameter...)
