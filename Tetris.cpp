/*Programmu veidoja: Gļebs Dimitrijevs DP2-1*/

/*Importējam visas nepieciešamās bibliotēkas*/
#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>

// Lai katru reizi nerakstīt sf:: un std::
using namespace sf;
using namespace std;

/*Konsoles augstums un platums*/
const int M = 20;
const int N = 10;

int field[M][N] = { 0 };

/*Kvadrātiņu lielums*/
int w = 34;

struct Point
{
    int x, y;
}
a[4], b[4];

/*masīvs ar figūrām, 7 figuras, 4 kvadrati katrā*/
int figures[7][4] = {
    1,3,5,7,
    2,4,5,7,
    3,5,4,6,
    3,5,4,7,
    2,3,5,7,
    3,5,7,6,
    2,3,4,5,
};


bool check() {
    for (int i = 0; i < 4; i++)
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
            return 0;
        else if (field[a[i].y][a[i].x])
            return 0;

    return 1;
}
/*Main funkcija*/
int main()
{
    //Music music;

    /*sick flou*/
    //music.openFromFile("audio/Tetris-Theme-Tetirs-Soundtrack.ogg");
    //music.setVolume(20);

    // nesanak pievienot
    //int points = 0;

    srand(time(0));

    /*Pats logs*/
    RenderWindow window(VideoMode(N * w, M * w), "Tetris!");

    /*Bildes mainīgais*/
    Texture texture;

    /*Nosakam ceļu līdz mums nepiecešamai bildei*/
    //texture.loadFromFile("tiles.png");

    // Gudrāka versija
    if (!texture.loadFromFile("tiles.png")) {
        cout << "Error" << endl; 
        return 0;
    }


    /*Spēles objekts, struktūra atkarīga no importētās bildes*/
    Sprite tiles(texture);


    /*Mainīgie, kas ļauj kustināt figūru horizontāli*/
    int dx = 0, colorNum = 1;
    bool rotate = false;
    float timer = 0, delay = 0.3;
    Clock clock;
    bool ad = true;

    /*Main cikls*/
    while (window.isOpen())
    {

        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event event;
        while (window.pollEvent(event))
        {
            /*Atbild par loga aizvēršanu*/
            if (event.type == Event::Closed)
                window.close();

            /*Pogas, kas atbild par figūras kustību un rotēšanu*/
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Up) {
                    rotate = true;
                }
                else if (event.key.code == Keyboard::Right) {
                    dx = 1;
                }
                else if (event.key.code == Keyboard::Left) {
                    dx = -1;
                }
                else if (event.key.code == Keyboard::Q) {
                    cout << "Spēle tika aizvērta" << endl;
                    return 0;
                }
            }
        }

        /*Paatrina kustību uz leju*/
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            delay = 0.05;
        }

        for (int i = 0; i < 4; i++) {
            b[i] = a[i];
            a[i].x += dx;
        }

        if (!check()) {
            for (int i = 0; i < 4; i++)
                a[i] = b[i];
        }

        /*Rotējam figūru*/
        if (rotate) {
            Point p = a[1];
            for (int i = 0; i < 4; i++) {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;

                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }

            if (!check()) {
                for (int i = 0; i < 4; i++)
                    a[i] = b[i];
            }
        }

        // atbild par kustību lejup
        if (timer > delay) {
            for (int i = 0; i < 4; i++) {
                b[i] = a[i];
                a[i].y += 1;
            }


            if (!check()) {
                for (int i = 0; i < 4; i++)
                    field[b[i].y][b[i].x] = colorNum;
                colorNum = 1 + rand() % 7;
                int n = rand() % 7;
                for (int i = 0; i < 4; i++) {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
            }

            // obligati jābūt 0
            timer = 0;
        }

        // pirmas figuras lielums
        if (ad) {
            int n = rand() % 7;
            if (a[0].x == 0)
                for (int i = 0; i < 4; i++) {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
            ad = false;
        }
        int k = M - 1;
        for (int i = M - 1; i > 0; i--) {
            int count = 0;
            for (int j = 0; j < N; j++) {
                if (field[i][j])
                    count++;
                field[k][j] = field[i][j];
            }
            if (count < N) {
                k--;
            }
        }
        

        // pa labi, pa kreisi
        dx = 0;
        // rotacija notiek tikai viena, nevis bezgalīga
        rotate = false;

        // kustiba notiek ik pēc delay sekundēm
        delay = 0.3;

        /*Uzliekam baltu fonu, notira visu lieko*/
        window.clear(Color::White);

        // figura paliek sava vieta pēc piezemēšanas
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++) {
                if (field[i][j] == 0)
                    continue;
                // pieraksta figuras novietojumu
                tiles.setTextureRect(IntRect(field[i][j] * w, 0, w, w));
                tiles.setPosition(j * w, i * w);
                window.draw(tiles);
            }

        /*For cikls, kas atbild par figūrām un tas izvadīšanu logā*/
        for (int i = 0; i < 4; i++) {
            tiles.setTextureRect(IntRect(colorNum * w, 0, w, w));
            tiles.setPosition(a[i].x * w, a[i].y * w);
            /*Komanda, kura izvada figūru*/
            window.draw(tiles);
        }

        window.display();
    }
    return 0;
}