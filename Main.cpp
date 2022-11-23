
#include <time.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
using namespace sf;

const int height = 20;
const int width = 10;

int field[height][width] = { 0 };

struct Point
{
    int x, y;
} a[4], b[4];

int figures[7][4] =
{
    1,3,5,7, 
    2,4,5,7, 
    3,5,4,6, 
    3,5,4,7, 
    2,3,5,7, 
    3,5,7,6, 
    2,3,4,5, 
};

bool check()
{
    for (int i = 0; i < 4; i++)
    {
        if (a[i].x < 0 || a[i].x >= width || a[i].y >= height) 
            return 0;
        else if (field[a[i].y][a[i].x]) 
            return 0;
    }
    return 1;
};


int main()
{
    srand((int)time(0));

    RenderWindow window(VideoMode(320, 480), "Tetris");

    Texture t1, t2, t3, t4;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/background.png");
    t3.loadFromFile("images/frame.png");
    t4.loadFromFile("images/GameOver.jpg");

    Sprite s(t1), background(t2), frame(t3), end(t4);

    int dx = 0; bool rotate = 0; int colorNum = 1;
    float timer = 0.0f, delay = 0.3f;
    bool endGame = false;

    Clock clock;
    int t = 0;

    while (window.isOpen())
    {
        float time;
        time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;

        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Up) 
                    rotate = true;
                else if (e.key.code == Keyboard::Left)
                    dx = -1;
                else if (e.key.code == Keyboard::Right) 
                    dx = 1;
        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) 
            delay = 0.05f;

        for (int i = 0; i < 4; i++) 
        { 
            b[i] = a[i]; a[i].x += dx;
        }

        if (!check()) 
            for (int i = 0; i < 4; i++) 
                a[i] = b[i];

        if (rotate)
        {
            Point p = a[1];
            for (int i = 0; i < 4; i++)
            {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
            if (!check())
                for (int i = 0; i < 4; i++)
                    a[i] = b[i];
        }

        for (int j = 0; j < width; j++)
        {
            for (int i = 0; i < 4; i++) {

                if (field[0][j] != 0 && a[i].x == 0) {
                    endGame = true;
                }
            }
        }

        if (timer > delay && !endGame)
        {
            for (int i = 0; i < 4; i++) 
            { 
                b[i] = a[i]; 
                a[i].y += 1; 
            }

            if (!check())
            {
                for (int i = 0; i < 4; i++) 
                    field[b[i].y][b[i].x] = colorNum;

                colorNum = 1 + rand() % 7;
                int n = rand() % 7;

                for (int i = 0; i < 4; i++)
                {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
            }
            timer = 0;
        }

        int k = height - 1;
        for (int i = height - 1; i > 0; i--)
        {
            int count = 0;
            for (int j = 0; j < width; j++)
            {
                if (field[i][j]) count++;
                field[k][j] = field[i][j];
            }
            if (count < width) 
                k--;
        }

        dx = 0; rotate = 0; delay = 0.3f;
        window.draw(background);

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
            {
                if (field[i][j] == 0)
                    continue;
                s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
                s.setPosition(j * 18.0f, i * 18.0f);
                s.move(28.0f, 31.0f);
                window.draw(s);
            }

        for (int i = 0; i < 4; i++)
        {
            s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
            s.setPosition(a[i].x * 18.0f, a[i].y * 18.0f);
            s.move(28.0f, 31.0f);
            window.draw(s);
        }

        window.draw(frame);

        if (endGame) {
            end.setPosition(-102.0f, 100.0f);
            end.setScale(0.8f, 0.8f);
            window.draw(end);
        }

        window.display();
    }
    return 0;
}