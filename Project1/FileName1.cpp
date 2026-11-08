#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

class Song
{
public:
	class Duration
	{
	private:
		int minutes;
		int seconds;

	public:
		Duration() = delete;

		Duration(int minutes, int seconds) : minutes(minutes), seconds(seconds)
		{
			if (minutes < 0)
				this->minutes = 0;

			if (seconds < 0)
				this->seconds = 0;

			if (seconds >= 60)
			{
				this->minutes += seconds / 60;
				this->seconds %= 60;
			}
		}

		int getTotalSeconds() const
		{
			return minutes * 60 + seconds;
		}

		Duration operator + (const Duration& other) const
		{
			return Duration{ minutes + other.minutes, seconds + other.seconds };
		}

		void print() const
		{
			cout << minutes << ":";

			if (seconds / 10 == 0)
				cout << "0";

			cout << seconds;
		}
	};

private:
	string title;
	string artist;
	Duration duration;

public:
	Song(const string& title, const string& artist, int minutes, int seconds) :
		title(title), artist(artist), duration{ minutes, seconds } {
	}

	Duration getDuration() const
	{
		return duration;
	}

	void print() const
	{
		cout << artist << " - " << title << " (";
		duration.print();
		cout << ")\n"; 
	}
};

class Playlist
{
private:
	string name;
	Song** songs;
	int count;

public:
	Playlist(const string& name) : songs(nullptr), count(0)
	{
		this->name = name;
	}

	Playlist(const Playlist& other) = delete;
	Playlist& operator = (const Playlist& other) = delete;

	~Playlist()
	{
		delete[] songs;
	}

	bool add(Song* song)
	{
		if (song == nullptr)
			return false;

		for (int i = 0; i < count; i++)
		{
			if (this->songs[i] == song)
				return false;
		}

		int size = count + 1;
		Song** new_songs = new Song * [size];
		for (int i = 0; i < count; i++)
		{
			new_songs[i] = songs[i];
		}
		new_songs[count] = song;
		delete[] songs;
		songs = new_songs;
		count = size;

		return true;
	}

	Song::Duration totalDuration() const
	{
		Song::Duration total{ 0, 0 };

		for (int i = 0; i < count; i++)
		{
			total = total + songs[i]->getDuration();
		}

		return total;
	}

	void show() const
	{
		cout << name << endl;

		for (int i = 0; i < count; i++)
		{
			songs[i]->print();
		}

		cout << "Общая продолжительность: ";
		this->totalDuration().print();
		cout << endl;
	}

	bool remove(Song* song)
	{
		int index = -1;

		for (int i = 0; i < count; i++)
		{
			if (songs[i] == song)
			{
				index = i;
				break;
			}
		}

		if (index == -1)
			return false;

		for (int i = index; i < count - 1; i++)
		{
			songs[i] = songs[i + 1];
		}

		count--;

		return true;
	}
};


int main()
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	Song song1{ "Ранкова кава", "Гурт Світанок", 3, 7 };
	Song song2{ "Нічне місто", "Ліна Вітер", 0, 250 };
	song1.print();
	song2.print();

	Song::Duration total = song1.getDuration() + song2.getDuration();
	cout << "Сума: ";
	total.print();
	cout << endl << endl;

	Song song3{ "Песня 3", "Группа 3", 4, 11 };
	Song song4{ "Песня 4", "Группа 4", 2, 43 };
	Song song5{ "Песня 5", "Группа 5", 3, 26 };

	Playlist playlist1{ "Ранкова пробіжка" };
	Playlist playlist2{ "Вечірній чіл" };

	playlist1.add(&song1);
	playlist1.add(&song3);
	playlist1.add(&song4);

	playlist2.add(&song2);
	playlist2.add(&song5);
	playlist2.add(&song1); 

	if (!playlist1.add(&song3))
		cout << "Не вдалося додати: ця пісня вже є в плейлисті\n";

	if (!playlist1.add(nullptr))
		cout << "Не вдалося додати: nullptr\n";

	cout << endl;
	playlist1.show();
	cout << endl;
	playlist2.show();

		if (!playlist2.remove(&song1))
		cout << "Не вдалося видалити: пісні немає в плейлисті\n";

	cout << endl;
	playlist1.show();
	cout << endl;
	playlist2.show();

	return 0;
}

/*1) Потому что Duration не имеет конструктора по умолчанию. Все поля объекта создаются до выполнения тела конструктора
Song. Если написать duration = Duration(minutes, seconds) в теле, компилятор сначала попытается создать duration
без аргументов, а это запрещено. Поэтому используем список инициализации.*/

/*2) При создании сначала вызывается конструктор Duration, затем выполняется конструктор Song.
При уничтожении: сначала вызывается деструктор Song, затем деструктор Duration.*/

/*3) Потому что Duration объявлен внутри Song. Это вложенный класс, поэтому его полное имя — Song::Duration.
Если объявить его в private-секции, обратиться к нему из main уже нельзя будет — он станет недоступен за пределами Song.*/

/*4) Song и Duration - композиция, Playlist и Song - агрегация*/

/*5) Потому что Playlist владеет массивом указателей, но не самими объектами Song.*/

/*6) Потому что при стандартном копировании скопируются только поля и значения указателей, а не сами массивы.*/