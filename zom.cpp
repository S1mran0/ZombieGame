/*
ZOMBIE SURVIVAL - Simple Version
A short console game demonstrating basic OOP in C++.
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// Returns a random number between minVal and maxVal
int randomNumber(int minVal, int maxVal) {
    return minVal + rand() % (maxVal - minVal + 1);
}

// ---------------- Player ----------------
class Player {
private:
    string name;
    int health;
    int hunger;
    int bullets;
    int food;
    int bandages;

public:
    Player(string n) {
        name = n;
        health = 100;
        hunger = 100;
        bullets = 5;
        food = 2;
        bandages = 0;
    }

    bool isAlive() {
        return health > 0 && hunger > 0;
    }

    void eat() {
        if (food <= 0) {
            cout << "No food left!\n";
            return;
        }

        food--;
        hunger += 30;

        if (hunger > 100)
            hunger = 100;

        cout << "You ate. Hunger: " << hunger << "\n";
    }

    void useBandage() {
        if (bandages <= 0) {
            cout << "No bandages left!\n";
            return;
        }

        bandages--;
        health += 25;

        if (health > 100)
            health = 100;

        cout << "You used a bandage. Health: " << health << "\n";
    }

    void takeDamage(int amount) {
        health -= amount;

        if (health < 0)
            health = 0;
    }

    void loseHunger(int amount) {
        hunger -= amount;

        if (hunger < 0)
            hunger = 0;
    }

    void addFood(int amount) {
        food += amount;
    }

    void addBullets(int amount) {
        bullets += amount;
    }

    void addBandage() {
        bandages++;
    }

    int getBullets() {
        return bullets;
    }

    void useBullet() {
        bullets--;
    }

    void showStatus(int day) {
        cout << "\n--- Day " << day << " ---\n";
        cout << "Health: " << health
             << "  Hunger: " << hunger
             << "  Food: " << food
             << "  Bullets: " << bullets
             << "  Bandages: " << bandages << "\n";
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
        return health <= 0;
    }

    void takeDamage(int amount) {
        health -= amount;
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

    void search() {
        int roll = randomNumber(1, 4);

        if (roll == 1) {
            player.addFood(1);
            cout << "You found food!\n";
        }
        else if (roll == 2) {
            player.addBullets(2);
            cout << "You found bullets!\n";
        }
        else if (roll == 3) {
            cout << "You found nothing.\n";
        }
        else {
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

            if (player.getBullets() <= 0) {
                cout << "No bullets! The zombie attacks.\n";
                z.attack(player);

                if (!player.isAlive())
                    fighting = false;
            }
            else {
                player.useBullet();
                z.takeDamage(30);

                cout << "You shot the zombie!\n";

                if (z.isDead()) {
                    player.addFood(1);
                    cout << "You defeated the zombie and looted food!\n";

                    // 10% chance to find a bandage
                    if (randomNumber(1, 10) == 1) {
                        player.addBandage();
                        cout << "You found a rare bandage!\n";
                    }

                    fighting = false;
                }
                else {
                    z.attack(player);

                    if (!player.isAlive())
                        fighting = false;
                }
            }
        }
    }

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

            cout << "1. Search  2. Fight  3. Eat  4. Bandage  5. Quit\nChoice: ";

            int choice;
            cin >> choice;

            if (choice == 1)
                search();
            else if (choice == 2)
                fight();
            else if (choice == 3)
                player.eat();
            else if (choice == 4)
                player.useBandage();
            else if (choice == 5)
                playing = false;
            else
                cout << "Invalid choice.\n";

            player.loseHunger(10);
            day++;

            if (!player.isAlive()) {
                playing = false;
                cout << "\nGAME OVER. You survived "
                     << day - 1 << " days.\n";
                return;
            }
        }

        cout << "\nThanks for playing! You survived "
             << day - 1 << " days.\n";
    }
};

// ---------------- main ----------------
int main() {

    srand(time(0));

    string name;

    cout << "Enter your name: ";
    cin >> name;

    Game game(name);
    game.run();

    return 0;
}