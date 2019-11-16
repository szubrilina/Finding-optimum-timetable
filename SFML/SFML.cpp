#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp> 
#include <Windows.h> 
#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;
using namespace sf;

vector<vector<int>>tree;
vector<string>doctor;
vector<string>people;
vector<int>used;
vector<int>mt;
vector<vector<int>>ans;
string s;

int n, k, hour = 8, minut = 30, day = 0;
int f(string s)
{
	int ch = 1, ans = 0;
	for (int i = s.size() - 1; i >= 0; --i)
	{
		ans += (s[i] - '0') * ch;
		ch *= 10;
	}
	return ans;
}

bool kuhn(int v)
{
	if (used[v] == true)
		return false;

	used[v] = true;

	for (int i = 0; i < tree[v].size(); i++)
	{
		if (mt[tree[v][i]] == -1 || kuhn(mt[tree[v][i]]))
		{
			mt[tree[v][i]] = v;
			return true;
		}
	}
	return false;
}

void timer(int fl = 0)
{
	if (minut == 30)
	{
		++hour;
		minut = 0;
	}
	else
		minut = 30;

	if (hour == 19)
	{
		hour = 9;
		++day;
	}

	if (fl == 0)
	{
		cout << hour << ":";
		if (minut == 0)
			cout << "00";
		else
			cout << 30;
		if (day >= 1)
			cout << " + " << day << " days";
		cout << endl;
	}
}

int main()
{
	freopen("sample_input.txt", "r", stdin);
	freopen("sample_input.txt", "w", stdout);

	setlocale(LC_ALL, "Russian");
	int nom;
	cin >> n >> k;
	tree.resize(n);
	people.resize(n);
	doctor.resize(k);

	for (int i = 0; i < k; ++i)
	{
		cin >> s >> nom;
		doctor[nom - 1] = s;
	}
	for (int i = 0; i < n; ++i)
	{
		cin >> s >> nom;
		people[nom - 1] = s;
	}

	for (int i = -1; i < n; i++)
	{
		int ch = 0, fl = 0;
		getline(cin, s);
		for (int j = 0; j < s.size(); j++)
		if (s[j] >= '0'&& s[j] <= '9')
		{
			fl = 1;
			ch *= 10;
			ch += s[j] - '0';

			if (j == s.size() - 1)
				tree[i].push_back((ch - 1) + n);
		}
		else
		{
			if (fl == 1)
				tree[i].push_back((ch - 1) + n);
			ch = 0;
		}
	}

	while (1)
	{
		char flag = 0;
		for (int i = 0; i < tree.size(); i++)
		if (tree[i].size() != 0)
		{
			flag = 1;
			break;
		}
		if (!flag)
			break;

		vector<char>used1(n);
		mt.assign(n + k, -1);

		for (int i = 0; i < n; ++i)
		for (int j = 0; j < tree[i].size(); ++j)
		if (mt[tree[i][j]] == -1)
		{
			mt[tree[i][j]] = i;
			used1[i] = true;
			break;
		}

		for (int i = 0; i < n; i++)
		{
			if (used1[i]) continue;
			used.resize(n, false);
			kuhn(i);
		}
		ans.push_back(mt);
		for (int i = 0; i < n + k; ++i)
		if (mt[i] != -1)
			tree[mt[i]].erase(find(tree[mt[i]].begin(), tree[mt[i]].end(), i), find(tree[mt[i]].begin(), tree[mt[i]].end(), i) + 1);
	}

	for (int i = 0; i < ans.size(); ++i)
	{
		timer();
		for (int j = 0; j < ans[i].size(); ++j)
		{
			if (ans[i][j] != -1)
			{
				cout << "The Patient   ";
				for (int l = 0; l < people[ans[i][j]].size(); ++l)
					cout << people[ans[i][j]][l];
				cout << " goes to the doctor  ";
				for (int l = 0; l < doctor[j - n].size(); ++l)
					cout << doctor[j - n][l];
				cout << endl;
			}
		}
		cout << endl;
	}

	timer(1);
	cout << "The number of work days is " << day + 1 << endl;
	cout << "All patients will be satisfied to  " << hour << ":";
	if (minut == 0)
		cout << "00" << endl;
	else
		cout << minut << endl;

	//////////////////////////////////////////////////////////////////////////////Визуализация
	RenderWindow window(VideoMode(1200, 800), "sfmltest", Style::Close);

	vector<pair<int, int>>shape(n + k);
	int num_pik = 5;
	for (int i = n; i < k + n; ++i)
	{
		shape[i].first = 800;
		shape[i].second = num_pik;
		num_pik += 40;
	}
	num_pik = 5;
	for (int i = 0; i < n; ++i)
	{
		shape[i].first = 417;
		shape[i].second = num_pik;
		num_pik += 40;
	}

	string hours = "9", minuts = "00", d = "0";

	while (window.isOpen())
	{
		Font font;
		font.loadFromFile("sansation.ttf");

		for (int i = 0; i < n; ++i)
		{
			CircleShape shapee(8.f);
			shapee.setFillColor(Color(255, 255, 255));
			shapee.setPosition(shape[i].first, shape[i].second);
			window.draw(shapee);

			sf::Text text(people[i], font);
			text.setCharacterSize(18);
			text.setStyle(sf::Text::Regular);
			text.setColor(sf::Color::White);
			text.setPosition(shape[i].first - 15 * people[i].size(), shape[i].second);
			window.draw(text);
		}

		for (int i = n; i < k + n; ++i)
		{
			CircleShape shapee(8.f);
			shapee.setFillColor(Color(255, 255, 255));
			shapee.setPosition(shape[i].first, shape[i].second);

			window.draw(shapee);

			sf::Text text(doctor[i - n], font);
			text.setCharacterSize(18);
			text.setStyle(sf::Text::Regular);
			text.setColor(sf::Color::White);
			text.setPosition(shape[i].first + 100, shape[i].second);
			window.draw(text);
		}

		sf::VertexArray lines(sf::Lines, 2);
		int x = 70, y = 0;

		for (int j = 0; j < ans.size(); ++j)
		{
			Color color;
			for (int i = 0; i < ans[j].size(); ++i)
			{
				if (ans[j][i] != -1)
				{
					lines[0].position = sf::Vector2f(shape[ans[j][i]].first + 6, shape[ans[j][i]].second + 6);
					lines[1].position = sf::Vector2f(shape[i].first + 6, shape[i].second + 6);

					int v = j + 1;
					sf::Color a(90, 90, 90);
					sf::Color b(170, 170, 170);
					if (v == 1)
					{
						lines[0].color = Color::Red;
						lines[1].color = Color::Red;
						color = Color::Red;
					}
					if (v == 2)
					{
						lines[0].color = Color::Green;
						lines[1].color = Color::Green;
						color = Color::Green;
					}
					if (v == 3)
					{
						lines[0].color = Color::White;
						lines[1].color = Color::White;
						color = Color::White;
					}
					if (v == 4)
					{
						lines[0].color = Color::Blue;
						lines[1].color = Color::Blue;
						color = Color::Blue;
					}
					if (v == 5)
					{
						lines[0].color = Color::Yellow;
						lines[1].color = Color::Yellow;
						color = Color::Yellow;
					}
					if (v == 6)
					{
						lines[0].color = Color::Magenta;
						lines[1].color = Color::Magenta;
						color = Color::Magenta;
					}
					if (v == 7)
					{
						lines[0].color = Color::Cyan;
						lines[1].color = Color::Cyan;
						color = Color::Cyan;
					}
					if (v == 8)
					{
						lines[0].color = Color::Red + a;
						lines[1].color = Color::Red + a;
						color = Color::Red + a;
					}
					if (v == 9)
					{
						lines[0].color = Color::Green + a;
						lines[1].color = Color::Green + a;
						color = Color::Green + a;
					}
					if (v == 10)
					{
						lines[0].color = Color::Black + a;
						lines[1].color = Color::Black + a;
						color = Color::Black + a;
					}
					if (v == 11)
					{
						lines[0].color = Color::Blue + a;
						lines[1].color = Color::Blue + a;
						color = Color::Blue + a;
					}
					if (v == 12)
					{
						lines[0].color = Color::Yellow + a;
						lines[1].color = Color::Yellow + a;
						color = Color::Yellow + a;
					}
					if (v == 13)
					{
						lines[0].color = Color::Magenta + a;
						lines[1].color = Color::Magenta + a;
						color = Color::Magenta + a;
					}
					if (v == 14)
					{
						lines[0].color = Color::Cyan + a;
						lines[1].color = Color::Cyan + a;
						color = Color::Cyan + a;
					}
					if (v == 15)
					{
						lines[0].color = Color::White + a;
						lines[1].color = Color::White + a;
						color = Color::White + a;
					}
					if (v == 16)
					{
						lines[0].color = Color::Green + b;
						lines[1].color = Color::Green + b;
						color = Color::Green + b;
					}
					if (v == 17)
					{
						lines[0].color = Color::Black + b;
						lines[1].color = Color::Black + b;
						color = Color::Black + b;
					}
					if (v == 18)
					{
						lines[0].color = Color::Blue + b;
						lines[1].color = Color::Blue + b;
						color = Color::Blue + b;
					}
					if (v == 19)
					{
						lines[0].color = Color::Yellow + b;
						lines[1].color = Color::Yellow + b;
						color = Color::Yellow + b;
					}
					if (v == 20)
					{
						lines[0].color = Color::Magenta + b;
						lines[1].color = Color::Magenta + b;
						color = Color::Magenta + b;
					}
					if (v == 21)
					{
						lines[0].color = Color::Cyan + b;
						lines[1].color = Color::Cyan + b;
						color = Color::Cyan + b;
					}
					if (v == 22)
					{
						lines[0].color = Color::Red + b;
						lines[1].color = Color::Red + b;
						color = Color::Red + b;
					}
					if (v == 23)
					{
						Color c(12, 12, 12);
						lines[0].color = Color::Red + b + c;
						lines[1].color = Color::Red + b + c;
						color = Color::Red + b + c;
					}
					if (v == 24)
					{
						Color c(12, 12, 12);
						lines[0].color = Color::Green + b + c;
						lines[1].color = Color::Green + b + c;
						color = Color::Green + a + b + c;
					}
					if (v >= 24)
					{
						Color c(rand() % 256, rand() % 256, rand() % 256);
						lines[0].color = c;
						lines[1].color = c;
						color = c;
					}
					window.draw(lines);
				}
			}

			sf::Text text(hours, font);
			text.setCharacterSize(18);
			text.setStyle(sf::Text::Regular);
			text.setColor(sf::Color::White);
			text.setPosition(x + 25, y);
			window.draw(text);

			sf::Text text2(":", font);
			text2.setCharacterSize(18);
			text2.setStyle(sf::Text::Regular);
			text2.setColor(sf::Color::White);
			text2.move(x + 50, y);
			window.draw(text2);

			sf::Text text1(minuts, font);
			text1.setCharacterSize(18);
			text1.setStyle(sf::Text::Regular);
			text1.setColor(sf::Color::White);
			text1.move(x + 65, y);
			window.draw(text1);

			if (d != "0")
			{
				sf::Text text4("+", font);
				text4.setCharacterSize(18);
				text4.setStyle(sf::Text::Regular);
				text4.setColor(sf::Color::White);
				text4.setPosition(x + 100, y);
				window.draw(text4);

				sf::Text text3(d, font);
				text3.setCharacterSize(18);
				text3.setStyle(sf::Text::Regular);
				text3.setColor(sf::Color::White);
				text3.setPosition(x + 130, y);
				window.draw(text3);

				sf::Text text5("days", font);
				text5.setCharacterSize(18);
				text5.setStyle(sf::Text::Regular);
				text5.setColor(sf::Color::White);
				text5.setPosition(x + 160, y);
				window.draw(text5);
			}

			RectangleShape rectangle(sf::Vector2f(x, 0));
			rectangle.setSize(sf::Vector2f(50, 20));
			rectangle.setPosition(0, y);
			rectangle.setFillColor(color);
			window.draw(rectangle);

			y += 30;

			if (minuts == "00")
				minuts = "30";
			else
			{
				minuts = "00";
				if (hours == "9")
					hours = "10";
				else if (hours == "10")
					hours = "11";
				else if (hours == "11")
					hours = "12";
				else if (hours == "12")
					hours = "13";
				else if (hours == "13")
					hours = "14";
				else if (hours == "14")
					hours = "15";
				else if (hours == "15")
					hours = "16";
				else if (hours == "16")
					hours = "17";
				else if (hours == "17")
					hours = "18";
				else if (hours == "18")
				{
					hours = "9";
					int val = 0, ch = 1;
					for (int pos = d.size() - 1; pos >= 0; --pos)
					{
						val += (d[pos] - '0') * ch;
						ch *= 10;
					}

					d.clear();

					++val;
					while (val > 0)
					{
						d.push_back((val % 10) + '0');
						val /= 10;
					}
				}
			}
		}
		window.display();
		while (true)
		{
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
				{
					window.close();
					return 0;
				}
			}
		}
	}
	return 0;
}