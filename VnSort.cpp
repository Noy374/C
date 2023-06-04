#include <fstream>
#include <iostream>
#include <Windows.h>
#include <string>
#include <map>

const int Npath = 4;

struct Elem
{
	int num;
	char ch;
};

struct MyAdapter
{
	std::fstream file;
	Elem elem;
	bool eof, eor;
	void ReadNext();
	void StartRead(std::string filename);
	void StartWrite(std::string filename);
	void Close();
	void Copy(MyAdapter& x);
	void CopyRun(MyAdapter& x);
};

void InPuth(std::string filename, MyAdapter f[Npath])
{
	MyAdapter f0;
	std::string name;
	f0.StartRead(filename);
	for (int i = 0; i < Npath / 2; i++)
	{
		name = "f" + std::to_string(i) + ".txt";
		f[i].StartWrite(name);
	}
	bool flag;
	int i = 0;
	while (!f0.eof)
	{
		/*do
		{
			flag = f[i].elem.num < f0.elem.num && ('a' <= f[i].elem.ch && f[i].elem.ch < 'z');
			f[i].CopyRun(f0);
		} while (flag && !f0.eof);*/
		f[i].CopyRun(f0);
		i = (i + 1) % (Npath / 2);
	}
	f0.Close();
	for (int i = 0; i < Npath / 2; i++)
		f[i].Close();
}

void OutPuth(std::string filename, MyAdapter f[Npath], int in)
{
	MyAdapter f0;
	std::string str = "f" + std::to_string(in) + ".txt";
	f[in].StartRead(str);
	if (!f[in].eof)
	{
		f0.StartWrite(filename);
		Elem el;
		while (!f[in].eof)
			f0.CopyRun(f[in]);
		f0.Close();
	}
	f[in].Close();
}

void print(std::string str)
{
	MyAdapter file;
	file.StartRead(str);
	Elem el;
	std::cout << str << " ";
	while (!file.eof)
	{
		el = file.elem;
		std::cout << el.num << ' ';
		file.ReadNext();
	}
	file.Close();
	std::cout << "\n";
}

void printall()
{
	for (int i = 0; i < Npath; i++)
	{
		MyAdapter file;
		std::string name = "f" + std::to_string(i) + ".txt";
		print(name);
	}
	std::cout << "\n";
}


void Sorting(std::string filename)
{
	MyAdapter f[Npath];
	InPuth(filename, f);
	int count(0);
	bool step = 0;
	int cnt = Npath / 2;
	int cnt1;
	int Min;
	int minIn;
	std::string name;
	int ord;
	int lcf;
	std::map <int, int> PrOrder;
	for (int i = 0; i < Npath; i++)
		PrOrder[i] = i;
	do
	{
		count = 0;

		for (int i = cnt; i < Npath; i++)
		{
			name = "f" + std::to_string(PrOrder[i]) + ".txt";
			f[PrOrder[i]].StartWrite(name);
		}
		printall();
		for (int i = 0; i < cnt; i++)
		{
			name = "f" + std::to_string(PrOrder[i]) + ".txt";
			f[PrOrder[i]].StartRead(name);
		}

		ord = cnt;

		for (int i = 0; i < cnt;)
		{
			if (f[PrOrder[i]].eof)
			{
				std::swap(PrOrder[i], PrOrder[cnt - 1]);
				cnt--;
			}
			else
				i++;
		}

		while (cnt > 0)
		{
			Min = f[PrOrder[0]].elem.num;
			for (int i = 0; i < cnt; i++)
				Min = min(f[PrOrder[i]].elem.num, Min);
			

			cnt1 = cnt;

			while (cnt1 > 1)
			{
				minIn = 0;
				for (int i = 0; i < cnt1; i++)
					if (f[PrOrder[minIn]].elem.num > f[PrOrder[i]].elem.num)
						minIn = i;

				f[PrOrder[ord]].Copy(f[PrOrder[minIn]]);
				lcf = PrOrder[ord];

				if (f[PrOrder[minIn]].eor)
				{
					std::swap(PrOrder[minIn], PrOrder[cnt1 - 1]);
					cnt1--;
				}
			}

			f[PrOrder[ord]].CopyRun(f[PrOrder[0]]);

			for (int i = 0; i < cnt;)
			{
				if (f[PrOrder[i]].eof)
				{
					std::swap(PrOrder[i], PrOrder[cnt - 1]);
					cnt--;
				}
				else
					i++;
			}

			for (int i = 0; i < cnt; i++)
				f[PrOrder[i]].eor = 0;
			count++;
		
				ord = max(Npath / 2 + step, (ord + 1) % Npath);
		}


		for (int i = 0; i < Npath; i++)
			f[i].Close();
		step = !step;
		cnt = Npath / 2 + step;

		for (int i = 0; i < Npath / 2; i++)
			std::swap(PrOrder[i], PrOrder[Npath - i - 1]);
	} while (count > 1);
	OutPuth(filename, f, lcf);
	for (int i = 0; i < Npath; i++)
	{
		name = "f" + std::to_string(i) + ".txt";
		const char* file = name.c_str();
		remove(file);
	}

}

void Create_File(std::string filename, int cnt)
{
	std::ofstream file(filename, std::ios::binary);
	srand(GetTickCount64());
	Elem el;
	for (int i = 0; i < cnt; ++i)
	{
		el.num = rand() % 100;
		el.ch = char('a' + rand() % 26);
		file.write((char*)&el, sizeof(el));
	}
	file.close();

}

bool  CheckFile(std::string filename)
{
	std::ifstream file(filename, std::ios::binary);
	Elem x, y;
	bool check(true);
	file.read((char*)&y, sizeof(y));
	while (!file.eof() && check)
	{
		x = y;
		file.read((char*)&y, sizeof(y));
		check = x.num <= y.num;
	}
	file.close();
	return check;
}

int main()
{
	Create_File("data.txt", 10);

	print("data.txt");
	std::cout << "\n";
	Sorting("data.txt");

	if (CheckFile("data.txt"))
		std::cout << "File is ordered";
	else
		std::cout << "Not ordered";
	std::cout << "\n";

	std::ifstream file("data.txt", std::ios::binary);
	Elem el;
	print("data.txt");

	std::cin.get();
	return 0;
}

void MyAdapter::ReadNext()
{
	if (!file.read((char*)&elem, sizeof(elem)))
		eof = true;
	else
		eof = false;
}

void MyAdapter::StartRead(std::string filename)
{
	file.open(filename, std::ios::in | std::ios::binary);
	ReadNext();
	eor = eof;
}

void MyAdapter::StartWrite(std::string filename)
{
	file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
}

void MyAdapter::Close()
{
	file.close();
}

void MyAdapter::Copy(MyAdapter& x)
{
	elem = x.elem;
	file.write((char*)&elem, sizeof(elem));
	x.ReadNext();
	x.eor = x.eof || (x.elem.num < elem.num);
}

void MyAdapter::CopyRun(MyAdapter& x)
{
	do
	{
		Copy(x);
	} while (!x.eor);
}

Функция InPuth принимает название файла и массив объектов MyAdapter.
Она открывает файл с помощью StartRead у объекта f0,
а затем записывает содержимое файла в порции по Npath / 2 элементов в объекты f[i] с помощью CopyRun, 
используя сортировку по колонке num.После окончания чтения файла, все объекты закрываются с помощью Close.

Функция OutPuth принимает название файла, 
массив объектов MyAdapter и индекс объекта f[in], 
содержимое которого нужно записать в файл.Она открывает объект f[in] с помощью StartRead,
а затем записывает его содержимое в файл с помощью CopyRun.После окончания записи файл закрывается с помощью Close.

Функция print принимает название файла и выводит его содержимое в консоль с помощью метода ReadNext и объекта elem.

Функция printall выводит содержимое всех файлов с помощью вызова print для каждого объекта f[i].

Функция Sorting принимает название файла.
Она создает Npath объектов MyAdapter,
затем поочередно записывает порции содержимого файла из объектов f[i],
начиная с f[Npath / 2] и заканчивая f[Npath - 1], используя StartWrite.
Затем она поочередно читает содержимое файла из первых Npath / 2 объектов, 
используя метод StartRead.Затем она удаляет из списка объектов те, которые достигли конца файла,
затем записывает наименьший элемент в объект f[ord] и переходит к следующему объекту f[i], где i = (i + 1) mod Npath.Она продолжает эти действия, пока остается более одного объекта.Наконец, она записывает содержимое последнего объекта в файл с помощью OutPuth, затем закрывает все объекты и удаляет временные файлы.

Функция Create_File создает файл с заданным именем и количеством элементов.Элементы файла генерируются случайным образом.

Функция CheckFile принимает название файла и проверяет, отсортирован ли он по возрастанию поля num.

main создает файл, а затем вызывает функцию Sorting.После завершения сортировки проверяется, отсортирован ли файл, а также выводит содержимое файла.
