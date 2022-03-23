
#include <math.h>
#include <stdio.h>

// Do³¹cz definicje biblioteki Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

const float FPS = 60;		//obraz bêdzie aktualizowany co 1/FPS sekundy
const int SCREEN_W = 640;	//szerokoœæ okna
const int SCREEN_H = 480;	//wysokoœæ okna

// Funkcja g³ówna
int main()
{
	ALLEGRO_DISPLAY* display = NULL;			//okno
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;	//kolejka zdarzen
	ALLEGRO_TIMER* timer = NULL;				//timer, od ktorego bêdziemy odbierac zdarzenia (potrzebny do animacji)
	bool redraw = true;

	if (!al_init()) {							//inicjalizacja biblioteki Allegro
		fprintf(stderr, "Nie zainicjalizowano allegro!\n");
		return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);	//utworznie okna
	timer = al_create_timer(1.0 / FPS);					//utworzenie timera
	al_install_keyboard();								//inicjalizacja obs³ugi klawiatury
	event_queue = al_create_event_queue();				//utworzenie kolejki zdarzeñ

	al_init_primitives_addon();							//inicjalizacja obs³ugi prostych elementów (punkty, linie, prostok¹ty, elipsy itd.)

	//Rejestracja ¿róde³ zdarzeñ (okno, timer, klawiatura ...)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	//Kolory rysowania
	ALLEGRO_COLOR yellow = al_map_rgb(255, 255, 0);
	ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255);
	ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);

	//Definicja wielok¹ta
	const int N = 5;
	float dx[N];
	float dy[N];
	int xm = SCREEN_W / 2;
	int ym = SCREEN_H / 2;
	float xx,yy;
	float fibis = 0.0;
	float rfactor = 1;
	int r = 50;
	xx = xm;
	yy = ym-r-100;


	//Tablice na przetworzone wspó³rzêdna punktów
	float points[2 * N];

	//Zmienne na potrzeby obracania figury
	double fi = 0.0, dfi = 0.02, sinfi, cosfi,sinbis,cosbis;

	//Uruchamiamy timer, który bêdzie z zadan¹ czêstotliwoœci¹ wysy³a³ zdarzenia
	al_start_timer(timer);

	//Pêtla g³ówna programu - obs³uga zdarzeñ.
	//Dzia³amy, dopóki u¿ytkownik nie wciœnie Esc.
	while (true)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER) {	//zdarzenie timera -> odœwie¿enie obrazu 
			redraw = true;
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {	//zdarzenie klawiatury -> jeœli Esc to koñczymy
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				break;
		}
		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { //zdarzenie zamkniêcia okna
			break;
		}

		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;
			al_clear_to_color(black); //czyszczenie okna na zadany kolor

			//Wyznacz œrodek ekranu
			xm = SCREEN_W / 2;
			ym = SCREEN_H / 2;
			for (int i = 0; i < N; i++)
			{
				dx[i] = r * cos(2 * ALLEGRO_PI * i / N);
				dy[i] = r * sin(2 * ALLEGRO_PI * i / N);
			}
			//Obrót figury
			sinfi = sin(fi);
			cosfi = cos(fi);
			sinbis = sin(fibis);
			cosbis = cos(fibis);
			xx = (r + 100) * cosbis +xm;
			yy = (r + 100) * sinbis + ym;
			for (int i = 0; i < N; i++)
			{
				points[2 * i] = (dx[i] * cosfi - dy[i] * sinfi + 0.5) + xx;
				points[2 * i + 1] = (dx[i] * sinfi + dy[i] * cosfi + 0.5) + yy;
			}
			fi -= dfi;
			fibis +=dfi;
			//Narysuj wype³niony okr¹g
			/*al_draw_filled_circle(xm, ym, 100, blue);*/
			al_draw_circle(xx, yy, r, white, 2);
			al_draw_circle(xm, ym, 100, white, 2);

			//Narysuj wype³niony wielokat
			al_draw_polygon(points, N, 2,white,2,2);
			if (r > 200 || r < 50)
			{
				rfactor *= -1;
			}
			r += rfactor;


			//Wyœwietl w oknie to, co narysowano w buforze
			al_flip_display();
		}
	}

	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	return 0;
}