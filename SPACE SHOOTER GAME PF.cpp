#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/Network.hpp>
#include<SFML/Audio.hpp>
#include<ctime>
#include<fstream>
#include<string.h>
#include<cstdlib>


using namespace sf;
using namespace std;

RenderWindow* window;
VideoMode video;
const int num_enemies = 100;
const int num_bullets = 100;
const int max_level = 5;
const int height = 768;
const int width = 768;
const int bullet_radius = 5;
int S_P_L = 100;
int i = 0;

struct Rocket
{
    Vector2f Rocket_Position;
    int health;
    bool isAlive;
    int Score;
    RectangleShape player;
    float rocket_speed;
};

struct Enemy
{
    Vector2f Enemy_Position;
    bool Alive;
    RectangleShape enemy;
    float enemyspeed;
};

struct Bullet
{
    Vector2f Bullet_position;
    CircleShape bullet;
    bool active;
    float bulletspeed;
};

struct Gamestate
{
    Enemy enemies[num_enemies];
    Bullet bullets[num_bullets];
    Rocket rocket;
    bool gamestart;
    bool gameover;
    int level;
    int highscore;
    int enemyperlvl;
    int noofenemy;
    int framecount;
    int spawndelayframes;
};

void initgamestate(Gamestate& gamestate)
{
    gamestate.rocket.player.setFillColor(Color::Yellow);
    gamestate.rocket.player.setSize(Vector2f(80.f, 50.f));
    gamestate.rocket.Rocket_Position = Vector2f(width / 2.f - 25.f, height - 50.f);
    gamestate.rocket.player.setPosition(gamestate.rocket.Rocket_Position);
    gamestate.rocket.isAlive = true;
    gamestate.rocket.health = 3;
    gamestate.rocket.Score = 0;
    gamestate.rocket.rocket_speed = 10.f;

    for (int i = 0; i < num_bullets; i++)
    {
        gamestate.bullets[i].active = false;
        gamestate.bullets[i].bullet.setFillColor(Color::Red);
        gamestate.bullets[i].bullet.setRadius(bullet_radius);
    }

    gamestate.level = 1;
    gamestate.highscore = 0;
    gamestate.noofenemy = 0;
    gamestate.spawndelayframes = 60;
    gamestate.framecount = 0;
}
void gameoverdisplay(Gamestate& gamestate)
{

    Font font4;
    if (!font4.loadFromFile("Fonts/ByteBounce.ttf"))
        cout << "Could not load font";

    Text text4;
    text4.setFont(font4);
    text4.setCharacterSize(60);
    text4.setScale(1.f, 1.f);
    text4.setFillColor(Color::Magenta);
    text4.setStyle(Text::Bold);
    text4.setString("GAME OVER!\n\n");

    Vector2u windowSize = window->getSize();
    FloatRect textSize = text4.getLocalBounds();
    float x = (windowSize.x - textSize.width) / 2;
    float y = (windowSize.y - textSize.height) / 2;
    text4.setPosition(x, y);
    window->draw(text4);
}
void finaldisplay(Gamestate& gamestate)
{
    Font font5;
    if (!font5.loadFromFile("Fonts/ByteBounce.ttf"))
        cout << "Could not load font";

    Text text5;
    text5.setFont(font5);
    text5.setCharacterSize(39);
    text5.setScale(1.f, 1.f);
    text5.setFillColor(Color::Magenta);
    text5.setStyle(Text::Bold);
    text5.setString("CONGRATULATIONS ! You have finished the game\n\n");



    Vector2u windowSize = window->getSize();
    FloatRect textSize = text5.getLocalBounds();
    float x = (windowSize.x - textSize.width) / 2;
    float y = (windowSize.y - textSize.height) / 2;
    text5.setPosition(x, y);
    window->draw(text5);
}
void HS(Gamestate& gamestate)
{
    Font font6;
    if (!font6.loadFromFile("Fonts/ARIBLK.ttf"))
        cout << "Could not load font";

    Text text6;
    text6.setFont(font6);
    text6.setCharacterSize(45);
    text6.setScale(1.f, 1.f);
    text6.setFillColor(Color::Magenta);
    text6.setStyle(Text::Bold);
    text6.setString("\n\n\nHigh Score : " + to_string(gamestate.highscore));



    Vector2u windowSize = window->getSize();
    FloatRect textSize = text6.getLocalBounds();
    float x = (windowSize.x - textSize.width) / 2;
    float y = (windowSize.y - textSize.height) / 2;
    text6.setPosition(x, y);
    window->draw(text6);
}
void createfile(const string& highscore)
{
    ofstream file(highscore);
    file << 0;
    file.close();
}
int get_highscore(const string& highscore)
{
    ifstream file(highscore);
    int H_S = 0;
    if (file.is_open())
    {
        file >> H_S;
        file.close();
    }
    return H_S;

}
void SaveHighScore(const string& highscore, int newH_S)
{
    ofstream file(highscore);
    if (file)
    {
        file << newH_S;
        file.close();
    }
}
void HighScore(Gamestate& gamestate)
{

    Font font4;
    if (!font4.loadFromFile("Fonts/ByteBounce.ttf"))
        cout << "Could not load font";

    Text text4;
    text4.setFont(font4);
    text4.setCharacterSize(95);
    text4.setScale(1.f, 1.f);
    text4.setFillColor(Color::Green);
    text4.setStyle(Text::Bold);
    text4.setString("Score : " + to_string(gamestate.rocket.Score));

    Vector2u windowSize = window->getSize();
    FloatRect textSize = text4.getLocalBounds();
    float x = (windowSize.x - textSize.width) / 2;
    float y = (windowSize.y - textSize.height) / 2;
    text4.setPosition(x, y);
    window->draw(text4);
}
int rang;
void manageenemies(Gamestate& gamestate)
{

    gamestate.framecount++;
    if (gamestate.framecount >= gamestate.spawndelayframes && gamestate.noofenemy < 100)
    {
        float random_x = rand() % 700;
        rang = rand() % 3;
        if (rang == 0)
        {
            gamestate.enemies[gamestate.noofenemy].enemy.setFillColor(Color::Green);
            gamestate.enemies[gamestate.noofenemy].Alive = true;
        }
        else if (rang == 1)
        {
            gamestate.enemies[gamestate.noofenemy].enemy.setFillColor(Color::Blue);
            gamestate.enemies[gamestate.noofenemy].Alive = true;
        }
        else if (rang == 2)
        {
            gamestate.enemies[gamestate.noofenemy].enemy.setFillColor(Color::Red);
            gamestate.enemies[gamestate.noofenemy].Alive = true;
        }
        gamestate.enemies[gamestate.noofenemy].enemy.setSize(Vector2f(50.f, 50.f));
        gamestate.enemies[gamestate.noofenemy].Enemy_Position = Vector2f(random_x, 0.f);
        gamestate.enemies[gamestate.noofenemy].enemy.setPosition(gamestate.enemies[gamestate.noofenemy].Enemy_Position);
        if (gamestate.level == 1)
        {
            gamestate.enemies[gamestate.noofenemy].enemyspeed = 1.f;
        }
        if (gamestate.level == 2)
        {
            gamestate.enemies[gamestate.noofenemy].enemyspeed = 1.7f;
        }
        if (gamestate.level == 3)
        {
            gamestate.enemies[gamestate.noofenemy].enemyspeed = 2.2f;
        }
        if (gamestate.level == 4)
        {
            gamestate.enemies[gamestate.noofenemy].enemyspeed = 2.9f;
        }
        if (gamestate.level == 5)
        {
            gamestate.enemies[gamestate.noofenemy].enemyspeed = 3.3f;
        }

        gamestate.noofenemy++;
        gamestate.framecount = 0;


    }

}
void enemymovement(Gamestate& gamestate)
{
    for (int i = 0; i < num_enemies; i++)
    {
        if (gamestate.rocket.health == 0)
        {
            gameoverdisplay(gamestate);
            HighScore(gamestate);
            break;
        }
        if (gamestate.enemies[i].Alive == true)
        {
            gamestate.enemies[i].enemy.move(0.f, gamestate.enemies[i].enemyspeed);
            if (gamestate.enemies[i].enemy.getPosition().y > height)
            {
                gamestate.enemies[i].Alive = false;
                gamestate.rocket.health--;

            }
        }
    }
}
void CollisionChecking(Gamestate& gamestate)
{

    for (int i = 0;i < num_enemies;i++)
    {

        for (int j = 0;j < num_bullets;j++)
        {
            if (gamestate.bullets[j].active)
            {
                if (gamestate.enemies[i].Alive && (gamestate.bullets[j].bullet.getGlobalBounds().intersects(gamestate.enemies[i].enemy.getGlobalBounds())))
                {

                    gamestate.bullets[j].active = false;
                    gamestate.enemies[i].Alive = false;
                    if (rang == 0)
                    {
                        gamestate.rocket.Score += 7;

                    }
                    else if (rang == 1)
                    {
                        gamestate.rocket.Score += 5;

                    }
                    else if (rang == 2)
                    {
                        gamestate.rocket.Score += 3;
                    }
                    if (gamestate.rocket.Score >= S_P_L)
                    {
                        if (gamestate.level < 5)
                        {
                            S_P_L += 100;
                            gamestate.level++;
                        }
                    }
                }

            }

        }

        if (gamestate.enemies[i].Alive && gamestate.rocket.player.getGlobalBounds().intersects(gamestate.enemies[i].enemy.getGlobalBounds()))
        {
            gamestate.enemies[i].Alive = false;
            gamestate.rocket.health--;
        }
    }
}


void Shootingbullets(Gamestate& gamestate)
{
    for (int i = 0; i < num_bullets; i++)
    {
        if (!gamestate.bullets[i].active)
        {
            gamestate.bullets[i].active = true;
            gamestate.bullets[i].bullet.setPosition(gamestate.rocket.Rocket_Position.x + gamestate.rocket.player.getSize().x / 2.f - bullet_radius, gamestate.rocket.Rocket_Position.y);
            gamestate.bullets[i].bulletspeed = 10.f;
            break;
        }
    }
}

void updatebullets(Gamestate& gamestate)
{
    for (int i = 0; i < num_bullets; i++)
    {
        if (gamestate.bullets[i].active)
        {
            gamestate.bullets[i].bullet.move(0, -gamestate.bullets[i].bulletspeed);

            if (gamestate.bullets[i].bullet.getPosition().y < 0)
            {
                gamestate.bullets[i].active = false;
            }
        }
    }
}

void RocketMovement(Gamestate& gamestate)
{
    if (Keyboard::isKeyPressed(Keyboard::A))
    {
        if (gamestate.rocket.Rocket_Position.x > 0)
            gamestate.rocket.Rocket_Position.x -= 4;
        gamestate.rocket.player.move(-gamestate.rocket.rocket_speed, 0);
    }

    if (Keyboard::isKeyPressed(Keyboard::D))
    {
        if (gamestate.rocket.Rocket_Position.x < width - gamestate.rocket.player.getSize().x)
            gamestate.rocket.Rocket_Position.x += 4;
        gamestate.rocket.player.move(gamestate.rocket.rocket_speed, 0);
    }

    gamestate.rocket.player.setPosition(gamestate.rocket.Rocket_Position);
}
void gamescreencontent(Gamestate& gamestate)
{
    Font font3;
    if (!font3.loadFromFile("Fonts/ARIAl.ttf"))
        cout << "Could not load font";

    Text text3;
    text3.setFont(font3);
    text3.setCharacterSize(30);
    text3.setScale(1.f, 1.f);
    text3.setFillColor(Color::Cyan);
    text3.setStyle(Text::Bold);
    text3.setString("Score: " + to_string(gamestate.rocket.Score) +
        "\nHealth: " + to_string(gamestate.rocket.health) +
        "\nLevel: " + to_string(gamestate.level));

    text3.setPosition(0.f, 0.f);
    window->draw(text3);

}
void displaymainscreen()
{
    Font font;
    if (!font.loadFromFile("Fonts/ByteBounce.ttf"))
        cout << "Could not load font";

    Text text;
    text.setFont(font);
    text.setCharacterSize(60);
    text.setScale(1.f, 1.f);
    text.setFillColor(Color::Magenta);
    text.setStyle(Text::Bold);
    text.setString("SPACE SHOOTER GAME");

    Vector2u windowSize = window->getSize();
    FloatRect textSize = text.getLocalBounds();
    float x = (windowSize.x - textSize.width) / 2;
    float y = 0.f;
    text.setPosition(x, y);

    // Instructions
    Font font1;
    if (!font1.loadFromFile("Fonts/ARIAl.ttf"))
        cout << "Could not load font";

    Text text1;
    text1.setFont(font1);
    text1.setCharacterSize(30);
    text1.setScale(1.f, 1.f);
    text1.setFillColor(Color::White);
    text1.setStyle(Text::Bold);
    text1.setString("\n\nI N S T R U C T I O N S");

    Vector2u windowSize1 = window->getSize();
    FloatRect textSize1 = text1.getLocalBounds();
    float x1 = (windowSize1.x - textSize1.width) / 2;
    float y1 = 0.f;
    text1.setPosition(x1, 0.f);

    // Instruction details
    Font font2;
    if (!font2.loadFromFile("Fonts/ARIAl.ttf"))
        cout << "Could not load font";

    Text text2;
    text2.setFont(font2);
    text2.setCharacterSize(30);
    text2.setScale(1.f, 1.f);
    text2.setFillColor(Color::Green);
    text2.setStyle(Text::Bold);
    text2.setString("\n\n\n\n1 - Use (A or D) to move player \n\n2 - Press space to fire bullets.\n\n3 - avoid hitting space debris\n\n4 - different colored debris gives different scores\n\n5 - Press R to restart from level 1 after completion \n\n\n\t\t\t\tPress space to start level 1\n\t\t\t\tPress esc to exit the game");
    text2.setPosition(5.f, 0.f);

    window->draw(text);
    window->draw(text1);
    window->draw(text2);
}
bool areAllEnemiesDefeated(Gamestate& gamestate)
{

    for (int d = 0; d < num_enemies; d++)
    {
        if (gamestate.enemies[d].Alive)
        {
            return false;  // If any enemy is alive, return false
        }
    }

    return true;  // If all enemies are defeated, return true
}

void render(Gamestate& gamestate)
{
    window->clear();

    if (i == 0)
        displaymainscreen();

    if (i == 1)
    {
        // Draw player
        if (gamestate.rocket.health != 0)
        {
            gamescreencontent(gamestate);
            window->draw(gamestate.rocket.player);
            RocketMovement(gamestate);
        }

        //draw enemies

        for (int a = 0;a < num_enemies;a++)
        {
            if (gamestate.enemies[a].Alive == true && gamestate.rocket.health != 0)
                window->draw(gamestate.enemies[a].enemy);

        }
        manageenemies(gamestate);
        enemymovement(gamestate);

        // Draw bullets
        for (int l = 0; l < num_bullets; l++)
        {
            if (gamestate.bullets[l].active == true && gamestate.rocket.health != 0)
                window->draw(gamestate.bullets[l].bullet);
        }

        updatebullets(gamestate);

    }

    if (i == 2)
    {
        finaldisplay(gamestate);
        HS(gamestate);
        HighScore(gamestate);

    }




    window->display();
}

int main()
{
    string highscore = "C:\\Users\\Hp\\Desktop\\project\\PF PROJECT\\highscore.txt";
    srand(static_cast<unsigned>(time(0)));
    video = VideoMode(height, width);
    window = new RenderWindow(video, "SPACE SHOOTER!", Style::Close | Style::Titlebar | Style::Resize);
    window->setFramerateLimit(60);
    Gamestate gamestate;
    initgamestate(gamestate);

    while (window->isOpen())
    {
        Event event;
        while (window->pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window->close();
            }

            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Escape)
                {
                    window->close();
                }

                if (i == 0)
                {
                    gamestate.rocket.Score = 0;
                    if (event.key.code == Keyboard::Space)
                    {
                        i = 1;
                    }

                }

                else if (i == 1)
                {
                    if (event.key.code == Keyboard::Space)
                    {
                        Shootingbullets(gamestate);
                    }


                    if (gamestate.rocket.health == 0)
                    {
                        gamestate.rocket.player.setSize(Vector2f(80.f, 50.f));
                        gamestate.rocket.Rocket_Position = Vector2f(width / 2.f - 25.f, height - 50.f);
                        gamestate.rocket.player.setPosition(gamestate.rocket.Rocket_Position);
                        if (event.key.code == Keyboard::Space)
                        {
                            i = 0;
                            gamestate.rocket.health = 3;
                            gamestate.level = 1;
                            for (int h = 0;h < gamestate.noofenemy;h++)
                            {
                                gamestate.enemies[h].Alive = false;
                            }
                            for (int i = 0; i < num_bullets; i++)
                            {
                                gamestate.bullets[i].active = false;
                                gamestate.bullets[i].bullet.setFillColor(Color::Red);
                                gamestate.bullets[i].bullet.setRadius(bullet_radius);
                            }
                            S_P_L = 20;
                            gamestate.noofenemy = 0;
                        }

                    }
                    if (gamestate.rocket.health != 0 && gamestate.level == 5)
                    {
                        if (areAllEnemiesDefeated(gamestate))
                        {
                            i = 2;
                        }
                    }


                    gamestate.highscore = get_highscore(highscore);
                    if (gamestate.rocket.Score >= gamestate.highscore)
                    {
                        gamestate.highscore = gamestate.rocket.Score;
                        SaveHighScore(highscore, gamestate.rocket.Score);
                    }

                }

                if (i == 2)
                {

                    if (event.key.code == Keyboard::R)
                    {
                        gamestate.rocket.player.setSize(Vector2f(80.f, 50.f));
                        gamestate.rocket.Rocket_Position = Vector2f(width / 2.f - 25.f, height - 50.f);
                        gamestate.rocket.player.setPosition(gamestate.rocket.Rocket_Position);
                        gamestate.rocket.health = 3;
                        gamestate.level = 1;
                        for (int h = 0;h < gamestate.noofenemy;h++)
                        {
                            gamestate.enemies[h].Alive = false;
                        }
                        for (int i = 0; i < num_bullets; i++)
                        {
                            gamestate.bullets[i].active = false;
                            gamestate.bullets[i].bullet.setFillColor(Color::Red);
                            gamestate.bullets[i].bullet.setRadius(bullet_radius);
                        }
                        S_P_L = 100;
                        gamestate.noofenemy = 0;
                        gamestate.spawndelayframes = 60;
                        gamestate.framecount = 0;
                        i = 0;



                    }
                }


            }
        }

        CollisionChecking(gamestate);

        render(gamestate);
    }

    return 0;
}


