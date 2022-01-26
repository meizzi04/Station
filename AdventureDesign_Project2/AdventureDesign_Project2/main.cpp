#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

#define Size 100

class Station
{
public:
    int line = 0;     // ȣ��
    int station_num = 0;     // �� ��ȣ
    char station_name[Size] = {};  // �� �̸�
    int transfer = 0; // ȯ�¿�
    int nextCount = 0;  // �������� �� �� �ִ� ���� ����
    int* nextList = new int[10];  // �������� �� �� �ִ� ������ ������ �迭
    bool visited = 0;   // �湮 ���� Ȯ��

    Station(int a, int b, char* c, int d, int e)    // ������
    {
        line = a;
        station_num = b;
        strcpy(station_name, c);
        transfer = d;
        nextCount = e;
    };
};

void Menu(int n);
void Setting();
void ListData();
int* InputData(int startnum, int destnum);
void printpath(vector<Station> path, int currentnum);
void DFS(vector<Station> path, int count, int currentnum, int destnum, vector<Station> visited);

vector <Station> stationList;  // �� ������ ����
vector<Station> path;   // ������ ��� ����

int n = 1;  // ��� ��ȣ

int main()
{
    int menu_num;

    while (1)
    {
        cout << "<����ö �� ã��>" << endl;
        cout << "1. ���α׷� ����" << endl;
        cout << "2. ���α׷� ����" << endl;
        cout << "���� �޴� �� �ϳ��� �����Ͻÿ�. ";
        cin >> menu_num;
        cout << endl;
        system("cls");
        Menu(menu_num);
    }
}

void Menu(int n)
{
    switch (n)
    {
    case 1:
        Setting();
        break;
    case 2:
        cout << endl << "// ���α׷��� �����մϴ�." << endl;
        exit(1);
        break;
    }
}

void Setting()
{
    string area;
    n = 1;

    do
    {
        cout << "���ϴ� ������ �����Ͻÿ�. ";
        cin >> area;

        if (area != "�뱸")
        {
            cout << "// ������ ������ ���� ������Ʈ���� �ʾҽ��ϴ�." << endl << endl;
            cin.clear();
            cin.ignore();
        }
    } while (area != "�뱸");

    cout << endl;
    cout << "< �뱸 ����ö �뼱�� >" << endl;

    cout << "- 1ȣ�� : ��ȭ���, ȭ��, ���, ��õ, ����, ����, ����, ����, �����, ���, ������, "
        << "�����, ���뺴��, ����, ���, �ݿ���, �߾ӷ�, �뱸��, ĥ������, ��õ, ���뱸��, ����û, "
        << "�ƾ米, ����, �ؾ�, ����, ���, ����, �ű�, �ݾ߿�, ����, �Ƚ�" << endl;
    cout << "- 2ȣ�� : ����, �ٻ�, ���, ��â, ����, �����������, �̰�, ���, ����, ����, �η�, ����"
        << "�ݰ�, �ų�, �ݿ���, ��뺴��, �뱸����, ����, ������û, ����, ��Ƽ, ��ȣ, �����, ���, "
        << "�Ÿ�, ���, ����, �Ӵ�, ������" << endl;
    cout << "- 3ȣ�� : ĥ���뺴��, ����, �Ȱ�, ��õ, ĥ����, ����, ����, ��õ, ��õ����"
        << "�ȴ�, ����, ����, �ȴ޽���, ����, �ϱ�û, �޼�����, ��������, �ų�, ����, ���, "
        << "�ǵ����, �����, ��������, �������ο��, ���ȸ��, Ȳ��, ������, ����, ����, ����" << endl;
    cout << endl << "// ���� ��� �� �� ���� �����Ͻÿ�." << endl;
    
    int count = 0;
    int startnum = 0;
    int destnum = 0;
    int* num = new int[2];
    
    ListData();
    num = InputData(startnum, destnum);
    startnum = num[0] - 1;
    destnum = num[1] - 1;
    DFS(path, count, startnum, destnum, stationList);

    int a = n;

    do
    {
        cout << "���� ��� �� ���ϴ� ��θ� �ϳ� �����Ͻÿ�. ";
        cin >> a;
        if (a >= n || a == 0)
        {
            cout << "// ����� ��ȣ�� �ٽ� Ȯ�����ּ���." << endl << endl;
            cin.clear();
            cin.ignore();
        }

    } while (a >= n || a == 0);

    cout << endl;

}

void ListData()  // ������ �����͸� �о �迭�� �����ϴ� �Լ�
{
    FILE* file = NULL;
    file = fopen("station.txt", "r");
    if (file != NULL)
    {
        char arr[Size];
        while (!feof(file))
        {
            fgets(arr, sizeof(arr), file);

            int line;     // ȣ��
            int station_num;     // �� ��ȣ
            char station_name[Size];  // �� �̸�
            int transfer;
            int nextCount;  // �������� �� �� �ִ� ���� ����

            char* stationArr[Size];    // ��ü station�� ������ ��ü �迭

            *stationArr = strtok(arr, " ");
            line = atoi(*stationArr);
            *stationArr = strtok(NULL, " ");
            station_num = atoi(*stationArr);
            *stationArr = strtok(NULL, " ");
            transfer = atoi(*stationArr);
            *stationArr = strtok(NULL, " ");
            nextCount = atoi(*stationArr);
            *stationArr = strtok(NULL, " ");
            strncpy(station_name, *stationArr, strlen(*stationArr));
            station_name[strlen(*stationArr) - 1] = '\0';

            Station station = Station(line, station_num, station_name, transfer, nextCount);

            if (nextCount == 1)
            {
                if (station_num == 1 || station_num == 33 || station_num == 62)
                    station.nextList[0] = station_num;
                else
                    station.nextList[0] = station_num - 2;
            }
            else
            {
                station.nextList[0] = station_num - 2;
                station.nextList[1] = station_num;
            }

            if (transfer != 0)
            {
                station.nextCount += 1;
                station.nextList[nextCount] = transfer - 1;
            }

            stationList.push_back(station);
        }
    }
    else
    {
        fprintf(file, "file open fail");
    }

    fclose(file);
}

int* InputData(int startnum, int destnum)// ����ڷκ��� ��߿��� �������� �Է¹޴� �Լ�
{
    string start;
    string dest;
    //bool s;

    do
    {
        cout << "��߿� : ";
        cin >> start;

        for (int i = 0; i < stationList.size(); i++)
        {
            if (start.compare(stationList[i].station_name) == 0)
            {
                startnum = stationList[i].station_num;
                break;
            }
            else
                startnum = 0;
        }

        if (startnum == 0)
        {
            cout << "// �Է��Ͻ� ��߿��� �������� �ʽ��ϴ�. �ٽ� �Է����ּ���." << endl;
        }
        cout << endl;
    } while (startnum == 0);

    do
    {
        cout << "������ : ";
        cin >> dest;

        for (int i = 0; i < stationList.size(); i++)
        {
            if (dest.compare(stationList[i].station_name) == 0)
            {
                destnum = stationList[i].station_num;
                break;
            }
            else
                destnum = 0;
        }

        if (destnum == 0)
        {
            cout << "// �Է��Ͻ� �������� �������� �ʽ��ϴ�. �ٽ� �Է����ּ���." << endl;
        }

        cout << endl;

    } while (destnum == 0);

    cout << "// ������ ��ο� ���� ������ ���Ͽ��� ã�� ����մϴ�." << endl;
    cout << "< ��߿� : " << start << ", ������ : " << dest << " >" << endl << endl;

    int num[2] = { startnum, destnum };
    return num;
}

void printpath(vector<Station> path, int currentnum)
{
    int count = 0;
    int trans = 0;
    int time = 0;

    cout << n << ". ";

    int i = 0;
    int line = path[0].line;

    for (i = 1; i < path.size(); i++)
    {
        if (line != path[i].line)
        {
            line = path[i].line;
            if (i != path.size() - 1)
                path.erase(path.begin() + i - 1);
            else
                path.erase(path.begin() + i);
        }

    }

    i = 0;
    line = path[0].line;

    for (i = 0; i < path.size() - 1; i++)
    {

        if ((line != path[i].line) || i == 0)
        {
            line = path[i].line;
            count++;
            if (i == 0)
                time += 2;
            else
            {
                trans++;
                time += 8;
            }
            cout << path[i].station_name << "(" << line << "ȣ��) - ";

        }
        else
        {
            count++;
            time += 2;
            cout << path[i].station_name << " - ";
        }
    }

    if ((line != path[i].line) || i == 0)
    {
        line = path[i].line;
        count++;
        if (i == 0)
            time += 2;
        else
        {
            trans++;
            time += 8;
        }
        cout << path[i].station_name << "(" << line << "ȣ��)";
    }
    else
    {
        count++;
        time += 2;
        cout << path[i].station_name;
    }
    cout << " : " << count << "��, " << trans << "ȯ��, " << time << "��" << endl << endl;

    n++;
    count = 0;
    time = 0;
}

void DFS(vector<Station> path, int count, int currentnum, int destnum, vector<Station> visited)  // ��͸� ���� ��߿����� ���������� ���� ��� ��θ� ã�� �Լ�
{
    // ���翪�� �������� ���
    if (currentnum == destnum)
    {
        path.push_back(stationList[destnum]);  // ��ο� ������ �߰�

        printpath(path, currentnum);  // ��� ���

        path.pop_back();  // ��ο��� �������� ����
        return;
    }

    // �����̰ų� �̹� �ѹ� �湮�� ���� ���
    if (stationList[currentnum].nextCount == 1 && count > 0 || visited[currentnum].visited)
    {
        return;
    }

    visited[currentnum].visited = true; // ���翪�� �湮�ߴٰ� ó��
    path.push_back(stationList[currentnum]); // ��ο� ���翪 �߰�


    // ���翪���� �� �� �ִ� ���鿡 ���� dfs ���
    for (int i = 0; i < stationList[currentnum].nextCount; i++)
    {
        DFS(path, count + 1, stationList[currentnum].nextList[i], destnum, visited);
        if (i > 1)
            path.pop_back(); // ��ο��� ����
    }
    visited[currentnum].visited = false;// dfs�� ���� �ڿ� �湮 ó�� �ߴ� �͵��� ���� 
    if (!path.empty())
        path.pop_back(); // ��ο��� ����
}