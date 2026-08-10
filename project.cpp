/*
    ZOMBIE SURVIVAL - Simple Version
    A short console game demonstrating basic OOP in C++.
    Uses only <iostream> and <string>.
*/

#include <iostream>
#include <string>
using namespace std;

// This variable is changed a little bit every time we need a
// "random" number. It's an unsigned long because the math below
// can produce large numbers, and unsigned long can hold bigger
// values than a normal int without going negative.
unsigned long seedValue = 12345;

// Returns a number between minVal and maxVal (both included).
// Not truly random, but changes enough each call to feel random.
int randomNumber(int minVal, int maxVal) {
    seedValue = seedValue * 1103515245UL + 12345UL;
    unsigned long value = (seedValue / 65536UL) % 32768UL;
    return minVal + (int)(value % (maxVal - minVal + 1));
}

// ---------------- Player ----------------
class Player {
private:
    string name;
    int health;
    int hunger;
    int bullets;
    int food;

public:
    Player(string n) {
        name = n;
        health = 100;
        hunger = 100;
        bullets = 5;
        food = 2;
    }

    bool isAlive() {
        if (health <= 0) return false;
        if (hunger <= 0) return false;
        return true;
    }

    void eat() {
        if (food <= 0) {
            cout << "No food left!\n";
            return;
        }
        food = food - 1;
        hunger = hunger + 30;
        if (hunger > 100) hunger = 100;
        cout << "You ate. Hunger: " << hunger << "\n";
    }

    void takeDamage(int amount) {
        health = health - amount;
        if (health < 0) health = 0;
    }

    void loseHunger(int amount) {
        hunger = hunger - amount;
        if (hunger < 0) hunger = 0;
    }

    void addFood(int amount) { food = food + amount; }
    void addBullets(int amount) { bullets = bullets + amount; }
    int getBullets() { return bullets; }
    void useBullet() { bullets = bullets - 1; }

    void showStatus(int day) {
        cout << "\n--- Day " << day << " ---\n";
        cout << "Health: " << health << "  Hunger: " << hunger
             << "  Food: " << food << "  Bullets: " << bullets << "\n";
    }
};

// ---------------- Zombie ----------------
class Zombie {
private:
    int health;
    int damage;

public:
    Zombie() {
        health = 50;
        damage = randomNumber(10, 25);
    }

    bool isDead() {
        if (health <= 0) return true;
        return false;
    }

    void takeDamage(int amount) {
        health = health - amount;
    }

    void attack(Player &player) {
        cout << "Zombie hits you for " << damage << " damage!\n";
        player.takeDamage(damage);
    }
};

// ---------------- Game ----------------
class Game {
private:
    Player player;
    int day;

public:
    Game(string name) : player(name) {
        day = 1;
    }

    void run() {
        cout << "=== ZOMBIE SURVIVAL ===\n";
        cout << "Survive as many days as you can!\n";

        bool playing = true;
        while (playing) {
            player.showStatus(day);
            cout << "1. Search  2. Fight  3. Eat  4. Quit\nChoice: ";

            int choice;
            cin >> choice;

            if (choice == 1) {
                search();
            } else if (choice == 2) {
                fight();
            } else if (choice == 3) {
                player.eat();
            } else if (choice == 4) {
                playing = false;
            } else {
                cout << "Invalid choice.\n";
            }

            player.loseHunger(10);
            day = day + 1;

            if (player.isAlive() == false) {
                playing = false;
                cout << "\nGAME OVER. You survived " << day - 1 << " days.\n";
                return;
            }
        }
        cout << "\nThanks for playing! You survived " << day - 1 << " days.\n";
    }

private:
    void search() {
        int roll = randomNumber(1, 4);
        if (roll == 1) {
            player.addFood(1);
            cout << "You found food!\n";
        } else if (roll == 2) {
            player.addBullets(2);
            cout << "You found bullets!\n";
        } else if (roll == 3) {
            cout << "You found nothing.\n";
        } else {
            cout << "A zombie ambushes you!\n";
            Zombie z;
            battle(z);
        }
    }

    void fight() {
        cout << "A zombie appears!\n";
        Zombie z;
        battle(z);
    }

    void battle(Zombie &z) {
        bool fighting = true;

        while (fighting) {
            // If the player has no bullets, the zombie gets a free hit
            if (player.getBullets() <= 0) {
                cout << "No bullets! The zombie attacks.\n";
                z.attack(player);

                if (player.isAlive() == false) {
                    fighting = false;
                }
            } else {
                // Player shoots the zombie
                player.useBullet();
                z.takeDamage(30);
                cout << "You shot the zombie!\n";

                if (z.isDead()) {
                    player.addFood(1);
                    cout << "You defeated the zombie and looted food!\n";
                    fighting = false;
                } else {
                    z.attack(player);
                    if (player.isAlive() == false) {
                        fighting = false;
                    }
                }
            }
        }
    }
};

// ---------------- main ----------------
int main() {
    string name;
    cout << "Enter your name: ";
    cin >> name;

    // Turn the player's name into a starting number for randomNumber().
    // This just adds up the character codes of each letter.
    int total = 0;
    for (int i = 0; i < (int)name.length(); i++) {
        total = total + name[i];
    }
    seedValue = (unsigned long)(total + 1); // +1 so it is never zero

    Game game(name);
    game.run();

    return 0;
}