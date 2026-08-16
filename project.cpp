#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// PLAYER CLASS
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

    void showStatus(int day) {
        cout << "\n--- Day " << day << " ---\n";
        cout << "Health: " << health << endl;
        cout << "Hunger: " << hunger << endl;
        cout << "Food: " << food << endl;
        cout << "Bullets: " << bullets << endl;
    }

    void eat() {
        if (food > 0) {
            food--;
            hunger = hunger + 30;

            if (hunger > 100)
                hunger = 100;

            cout << "You ate food.\n";
        }
        else {
            cout << "You have no food.\n";
        }
    }

    void damage(int amount) {
        health = health - amount;

        if (health < 0)
            health = 0;
    }

    void loseHunger() {
        hunger = hunger - 10;

        if (hunger < 0)
            hunger = 0;
    }

    void addFood() {
        food++;
    }

    void addBullets() {
        bullets = bullets + 2;
    }

    int getBullets() {
        return bullets;
    }

    void shoot() {
        bullets--;
    }

    int getHealth() {
        return health;
    }

    int getHunger() {
        return hunger;
    }
};


// ZOMBIE CLASS
class Zombie {
private:
    int health;
    int damage;

public:
    Zombie() {
        health = 50;

        // Zombie damage will be between 10 and 25
        damage = rand() % 16 + 10;
    }

    void damageTaken() {
        health = health - 30;
    }

    void attack(Player &player) {
        cout << "Zombie attacks you for "
             << damage << " damage!\n";

        player.damage(damage);
    }

    int getHealth() {
        return health;
    }
};


// GAME CLASS
class Game {
private:
    Player player;
    int day;

public:
    Game(string name) : player(name) {
        day = 1;
    }

    void start() {

        cout << "\n-----------------------\n";
        cout << "     ZOMBIE SURVIVAL\n";
        cout << "-----------------------\n";

        int playing = 1;

        while (playing == 1) {

            player.showStatus(day);

            cout << "\n1. Search";
            cout << "\n2. Fight Zombie";
            cout << "\n3. Eat Food";
            cout << "\n4. Quit";
            cout << "\nChoose: ";

            int choice;
            cin >> choice;

            if (choice == 1) {
                search();
            }

            else if (choice == 2) {
                fight();
            }

            else if (choice == 3) {
                player.eat();
            }

            else if (choice == 4) {
                playing = 0;
                cout << "\nYou left the game.\n";
            }

            else {
                cout << "Invalid choice.\n";
            }

            // One day passes after each action
            if (choice != 4) {

                player.loseHunger();
                day++;

                if (player.getHealth() <= 0 ||
                    player.getHunger() <= 0) {

                    cout << "\nGAME OVER!\n";
                    cout << "You survived "
                         << day - 1 << " days.\n";

                    playing = 0;
                }
            }
        }
    }

private:

    void search() {

        // Gives a number from 1 to 4
        int result = rand() % 4 + 1;

        if (result == 1) {

            player.addFood();

            cout << "You found food!\n";
        }

        else if (result == 2) {

            player.addBullets();

            cout << "You found 2 bullets!\n";
        }

        else if (result == 3) {

            cout << "You found nothing.\n";
        }

        else {

            cout << "Oh no! A zombie found you!\n";

            Zombie zombie;
            fightZombie(zombie);
        }
    }


    void fight() {

        cout << "\nA zombie appears!\n";

        Zombie zombie;
        fightZombie(zombie);
    }


    void fightZombie(Zombie &zombie) {

        int fighting = 1;

        while (fighting == 1) {

            if (player.getHealth() <= 0 ||
                player.getHunger() <= 0) {

                fighting = 0;
            }

            else if (zombie.getHealth() <= 0) {

                fighting = 0;
            }

            else if (player.getBullets() <= 0) {

                cout << "You have no bullets!\n";

                zombie.attack(player);
            }

            else {

                // Player shoots zombie
                player.shoot();
                zombie.damageTaken();

                cout << "You shot the zombie!\n";

                // Check if zombie is defeated
                if (zombie.getHealth() <= 0) {

                    cout << "You defeated the zombie!\n";

                    // Get food after defeating zombie
                    player.addFood();

                    cout << "You found 1 food!\n";
                }

                else {

                    // Zombie attacks back
                    zombie.attack(player);
                }
            }
        }
    }
};


// MAIN FUNCTION
int main() {

    // Start random numbers
    srand(time(0));

    string name;

    cout << "Enter your name: ";
    cin >> name;

    Game game(name);

    game.start();

    return 0;
}