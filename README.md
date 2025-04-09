# MINIMALISTIC TEXT ADVENTURE GAME CONCEPT

## Install: 
just run `install.sh`, it will load ArduinoCore-API and build from source.

## Run: 
just run `main` executable.

## Grammar:
```
EPISODE <episode>                                 // game is made up from (EPISODE ...) blocks
...                                               // each episode is a free text, except the followings:
MEMO <memo>                                       // the game will remember this "MEMO" when player reaches this point, (game state: each MEMO can be defined / or not)
IF CHOOSE <option> GOTO <episode>                 // player will see this option and goes (GOTO ...) the episode when selects it. (IF CHOOSE ...)
IF <memo> AND CHOOSE <option> GOTO <episode>      // player will see this option only if the game "remember" that MEMO (IF ...)
IF NO <memo> AND CHOOSE <option> GOTO <episode>   // player will see this only when the game did not see that MEMO at this point. (IF NOT ...)
```

## Example game:
See in `main()` function (hardcoded for now but can be externalized, for eg. read from file and pass as parameter...)
