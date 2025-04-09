#include <string>
#include <vector>
#include <iostream>
#include <cassert>

#include <stdlib.h>
#include <stddef.h>

#include "String.h"

using namespace std;



#define NO_INPUT 0

// // Emulate Arduino String library
// class String: public string {
// public:
//     using string::string;
//     virtual ~String() = default;
// };

// Interface for different I/O devices
class IO {
public:
    IO() = default;
    virtual ~IO() = default;
    virtual void print(const String& text) = 0;
    virtual int input() = 0;
};

// Linux terminal implementation for I/O (EinkPDA needs it's own interpretation)
class IOTerminal: public IO {
public:
    void print(const String& text) override {
        const size_t bsize = 200;
        char buff[bsize];
        text.toCharArray(buff, bsize);
        cout << buff << endl;
    }

    int input() {
        int i;
        cin >> i;
        return i;
    }
};

vector<String> split(String input, String delimiter) {
    vector<String> result;
    int startIndex = 0;
    int endIndex = input.indexOf(delimiter);
    
    while (endIndex >= 0) {
        result.push_back(input.substring(startIndex, endIndex));
        startIndex = endIndex + delimiter.length();  // Move past the delimiter
        endIndex = input.indexOf(delimiter, startIndex);
    }
    
    // Add the last part
    if (startIndex < input.length()) {
        result.push_back(input.substring(startIndex));
    }
    
    return result;
}

class TextAdventureGame {
public:
    TextAdventureGame(IO& io, const String& script): io(io), lines(split(script, "\n")) {}
    virtual ~TextAdventureGame() = default;

    bool go(int i) {
        i--;
        if (i < 0 || i >= choices.size()) {
            io.print("Invalid");
            return false;
        }
        String episode = choices[i].episode;
        String line = "EPISODE ";
        line.concat(episode);
        for (next = 0; next < lines.size(); next++)
            if (lines[next] == line) break;
        next++;        
        return next < lines.size();
    }

    void read() {
        choices.clear();
        for (; next < lines.size(); next++) {
            if (lines[next].startsWith("EPISODE ")) break; // next episode reached?
            if (lines[next].startsWith("MEMO ")) {
                memos.push_back(split(lines[next], "MEMO ")[1]);
                continue;
            }
            
            // is it a choice?
            if (lines[next].startsWith("IF CHOOSE ")) { 
                choices.push_back(choice(lines[next]));
                continue;
            }

            // Is it a choice when we don't have that MEMO?
            if (lines[next].startsWith("IF NO ")) {
                int found = false;
                for (int i = 0; i < memos.size(); i++)
                    if (found = lines[next].startsWith("IF NO " + memos[i] + " AND CHOOSE "))
                        break;
                if (!found) choices.push_back(choice(lines[next]));
                continue;
            }

            // Do we have this MEMO?
            if (lines[next].startsWith("IF ")) {
                bool found = false;
                for (int i = 0; i < memos.size(); i++)
                    if (found = lines[next].startsWith("IF " + memos[i] + " AND CHOOSE ")) {
                        choices.push_back(choice(lines[next]));
                        break;
                    }
                //if (found) 
                continue;
            }

            // Print the line only when it's not choice nor memo. (only texts)
            // if (!lines[next].startsWith("IF ")) 
            io.print(lines[next]);
        }
        next++;
    }

    void choose() {
        for (int i = 0; i < choices.size(); i++) {
            String output(i+1);
            output += ": ";
            output += choices[i].text;
            io.print(output);
        }
    }

    bool play() {
        choose();
        int inp = io.input();
        if (inp != NO_INPUT && go(inp)) read();
        return choices.empty(); // no more choice: game is over!
    }

// private:

    struct Choice {
        String text;
        String episode;
    };

    Choice choice(const String& line) {
        String choice = split(line, " CHOOSE ")[1];
        vector<String> splits = split(choice, " GOTO ");
        return { splits[0], splits[1] };
    }

    vector<String> lines;
    vector<Choice> choices;
    vector<String> memos = {};
    int next = 0;
    // bool over = false;
    IO& io;
};


// ========[ TESTS ]========

class IOFake: public IO {
public:
    IOFake(): IO() {}
    virtual ~IOFake() = default;
    
    void print(const String& text) override {
        outputs.push_back(text);
    }
    
    int input() override {
        if (inputs.empty()) return NO_INPUT;
        int i = inputs.front();
        inputs.erase(inputs.begin());
        return i;
    }
    
    vector<int> inputs;
    vector<String> outputs;
};


void test_TextAdventureGame_go_goes_to_episode() {
    IOFake io;
    String script = R"(Not an episod
EPISODE Not that one!
EPISODE This one!
EPISODE Wrong one!
)";
    TextAdventureGame game(io, script);
    game.choices.push_back({"Test choice", "This one!"});
    assert(game.go(1) && "Episode found");
    assert(game.next == 3 && "Episode found in the right line");
}

void test_TextAdventureGame_read_prints_episod() {
    IOFake io;
    String script = R"(It should be printed,
until we reach the next episod
EPISODE Next episode
...
)";
    TextAdventureGame game(io, script);
    game.read();
    assert(io.outputs.size() == 2 && "Episode should be printed until we reached the next one");
    assert(io.outputs[0] == "It should be printed,");
    assert(io.outputs[1] == "until we reach the next episod");
}

void test_TextAdventureGame_read_finds_choices() {
    IOFake io;
    String script = R"(It should be printed.
IF CHOOSE I am going over here GOTO Here we go!
IF CHOOSE I am doing nothing GOTO Waiting...
EPISODE Next episode
...
)";
    TextAdventureGame game(io, script);
    game.read();
    assert(io.outputs.size() == 1 && "Episode should be printed until we reached the next one (except the choices...)");
    assert(io.outputs[0] == "It should be printed.");
    assert(game.choices.size() == 2 && "Should read and find each choice(s)");
    assert(game.choices[0].text == "I am going over here");
    assert(game.choices[0].episode == "Here we go!");
    assert(game.choices[1].text == "I am doing nothing");
    assert(game.choices[1].episode == "Waiting...");
}


void test_TextAdventureGame_read_finds_choices_when_memo() {
    IOFake io;
    String script = R"(It should be printed.
IF key AND CHOOSE I am going over here GOTO Here we go!
IF NO key AND CHOOSE This is not a choice GOTO Nothing
IF CHOOSE I am doing nothing GOTO Waiting...
EPISODE Next episode
...
)";
    TextAdventureGame game(io, script);
    game.memos.push_back("key");
    game.read();
    assert(io.outputs.size() == 1 && "Episode should be printed until we reached the next one (except the choices...)");
    assert(io.outputs[0] == "It should be printed.");
    assert(game.choices.size() == 2 && "Should read and find each choice(s)");
    assert(game.choices[0].text == "I am going over here");
    assert(game.choices[0].episode == "Here we go!");
    assert(game.choices[1].text == "I am doing nothing");
    assert(game.choices[1].episode == "Waiting...");
}

void test_TextAdventureGame_read_memos() {
    IOFake io;
    String script = R"(You pick up a key.
MEMO key
And you take an apple.
MEMO apple
)";
    TextAdventureGame game(io, script);
    game.read();
    assert(io.outputs.size() == 2 && "Episode should be printed until we reached the next one (except the choices and memos...)");
    assert(io.outputs[0] == "You pick up a key.");
    assert(io.outputs[1] == "And you take an apple.");
    assert(game.choices.size() == 0); // no more choice
    assert(game.memos.size() == 2);
    assert(game.memos[0] == "key");
    assert(game.memos[1] == "apple");
}

void run_tests() {
    test_TextAdventureGame_go_goes_to_episode();
    test_TextAdventureGame_read_prints_episod();
    test_TextAdventureGame_read_finds_choices();
    test_TextAdventureGame_read_finds_choices_when_memo();
    test_TextAdventureGame_read_memos();
}

int main() {
    run_tests();
    cout << "Tests OK" << endl;

    // ===== IOTerminal example: =====
    // IOTerminal io;
    // io.print("Hello world!");
    // int i = io.input();
    // cout << "[" << to_string(i) << "]" << endl;
    // ===============================

    // ========== Test game: =========
    IOTerminal io;
    String script = R"(This is a test game!
IF CHOOSE Start GOTO Room

EPISODE Room
You are in a dark room.
There is a window and a door.
IF key AND CHOOSE I am going to the door GOTO Door opens
IF NO key AND CHOOSE I am going to the door GOTO Door closed
IF CHOOSE I am going to the window GOTO Window
IF NO key AND CHOOSE Look around GOTO Key
IF key AND CHOOSE Look around GOTO Nothing

EPISODE Door opens
The key opens the door. Great!
You won!

EPISODE Door closed
The door is closed.
IF CHOOSE Back GOTO Room

EPISODE Window
The dark night out there,
nothing to see...
IF CHOOSE Back GOTO Room

EPISODE Key
Wow! you just found a key!
MEMO key
IF CHOOSE Back GOTO Room

EPISODE Nothing
This is just a room,
nothing is here...
IF CHOOSE Back GOTO Room

)";

    TextAdventureGame game(io, script);
    game.read();
    while(!game.play());

}