#include <iostream>
#include <iomanip>
#include <string>
#include <bitset>
#include <fstream>
#include <locale.h>

using namespace std;

void encryption(string file_name)
{
	string start, result, buf, add;
	int password, i, y, c, f, h, n;
	bool e;

	cout << "Enter the string: ";
	getline(cin, start, '\n');

	cout << "Enter the password: ";
	cin >> password;

	srand(password);
	bitset<32> gamma(rand());

	string w(gamma.to_string());

	while ((start.size() % 4) != 0)
	{
		start.push_back(' ');
	}

	fstream enfile(file_name, ios::out);
	bitset<32> d;

	for (i = 0; i < (start.size() / 4); i++)
	{
		buf = start.substr(4 * i, 4);

		bitset<8> d1(buf[0]);
		bitset<8> d2(buf[1]);
		bitset<8> d3(buf[2]);
		bitset<8> d4(buf[3]);

		for (y = 0; y < 8; y++)
		{
			d[y + 24] = d1[y];
			d[y + 16] = d2[y];
			d[y + 8] = d3[y];
			d[y] = d4[y];
		}

		for (y = 0; y < 32; y++)
		{
			d[y] = d[y] ^ gamma[y];
		}

		for (y = 0; y < 4; y++)
		{
			e = d[0];
			d = d >> 1;
			d[31] = e;
		}

		for (y = 0; y < 8; y++)
		{
			d1[y] = d[y + 24];
			d2[y] = d[y + 16];
			d3[y] = d[y + 8];
			d4[y] = d[y];
		}

		char c1(d1.to_ulong());
		char c2(d2.to_ulong());
		char c3(d3.to_ulong());
		char c4(d4.to_ulong());

		enfile << c1;
		enfile << c2;
		enfile << c3;
		enfile << c4;
	}

	enfile.close();

	cout << "Encrypted! " << endl;
	cout << "Former line length: " << start.size() << " symbols " << endl;
}

void decryption(string file_name)
{
	string end, data;
	char a[500];
	char c = '\n';
	int password, i, y, z, m = 0;
	bool e;

	cout << "Enter the password: ";
	cin >> password;

	srand(password);
	bitset<32> gamma(rand());

	string w(gamma.to_string());

	fstream defile(file_name, ios::in);

	if (defile.is_open())
	{
		int x = 0;
		bitset<32> d;
		cin.ignore();

		do
		{
			getline(defile, data);

			for (i = 0; i < data.size(); i++)
			{
				if (x == 499)
				{
					for (int n = 0; n < 497; n += 4)
					{
						bitset<8> s1(a[n]);
						bitset<8> s2(a[n + 1]);
						bitset<8> s3(a[n + 2]);
						bitset<8> s4(a[n + 3]);

						for (y = 0; y < 8; y++)
						{
							d[y + 24] = s1[y];
							d[y + 16] = s2[y];
							d[y + 8] = s3[y];
							d[y] = s4[y];
						}

						for (y = 0; y < 4; y++)
						{
							e = d[31];
							d = d << 1;
							d[0] = e;
						}

						for (y = 0; y < 16; y++)
						{
							d[y] = d[y] ^ gamma[y];
						}

						for (y = 0; y < 8; y++)
						{
							s1[y] = d[y + 24];
							s2[y] = d[y + 16];
							s2[y] = d[y + 8];
							s2[y] = d[y];
						}

						char c1(s1.to_ulong());
						char c2(s2.to_ulong());
						char c3(s3.to_ulong());
						char c4(s4.to_ulong());

						end += c1;
						end += c2;
						end += c3;
						end += c4;
					}
					x = 0;
					m++;
				}

				a[x] = data[i];
				x++;

				if ((i == (data.size() - 1)) && (!defile.eof()))
				{
					a[x] = c;
					x++;
				}
			}
		} while (!defile.eof());

		for (int n = 0; n < x - 3; n += 4)
		{
			bitset<8> s1(a[n]);
			bitset<8> s2(a[n + 1]);
			bitset<8> s3(a[n + 2]);
			bitset<8> s4(a[n + 3]);

			for (y = 0; y < 8; y++)
			{
				d[y + 24] = s1[y];
				d[y + 16] = s2[y];
				d[y + 8] = s3[y];
				d[y] = s4[y];
			}

			for (y = 0; y < 4; y++)
			{
				e = d[31];
				d = d << 1;
				d[0] = e;
			}

			for (y = 0; y < 16; y++)
			{
				d[y] = d[y] ^ gamma[y];
			}

			for (y = 0; y < 8; y++)
			{
				s1[y] = d[y + 24];
				s2[y] = d[y + 16];
				s3[y] = d[y + 8];
				s4[y] = d[y];
			}

			char c1(s1.to_ulong());
			char c2(s2.to_ulong());
			char c3(s3.to_ulong());
			char c4(s4.to_ulong());

			end += c1;
			end += c2;
			end += c3;
			end += c4;
		}

		defile.close();

		cout << "The message is: " << end << endl << endl;
		cout << "Length of the decrypted line is: " << end.size() << endl;
		cout << "Overall symbols read: " << m * 500 + x << endl;
		defile.close();
	}
	else
	{
		cout << "The file has not been found... " << endl;
	}
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");

	if (argc != 3)
	{
		cout << "Error! Three parameters are to be used... " << endl << endl;
		system("pause");
		return 0;
	}
	else
	{
		const string mode(argv[1]);
		const string file_name(argv[2]);

		if ((mode == "encryption") || (mode == "encrypt"))
		{
			encryption(file_name);
		}
		else
		{
			if ((mode == "decryption") || (mode == "decrypt"))
			{
				decryption(file_name);
			}
			else
			{
				cout << "Error! Unknown command... " << endl;
				system("pause");
				return 0;
			}
		}
	}
}