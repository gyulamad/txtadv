# MINIMALISTIC TEXT ADVENTURE GAME CONCEPT

## Install: 
just run `install.sh`, it will load ArduinoCore-API and build from source.

## Run: 
just run `main` executable.

## Grammar:
```
EPISODE <episode>  // game is made up from EPISODE blocks
... // each episode is a free text, except the followings:
MEMO <memo>   // the game will remember this "memo", (game state: each memo can be defined / or not)
IF CHOOSE <option> GOTO <episode> // player will see this option and goes the episode when selects
IF <memo> AND CHOOSE <option> GOTO <episode> // player will see this option only if the game "remember" that memo
IF NO <memo> AND CHOOSE <option> GOTO <episode> // player will see this only when the game did not see that memo at this point.
```

## Example game:
See in `main()` function (hardcoded for now but can be externalized, for eg. read from file and pass as parameter...)
