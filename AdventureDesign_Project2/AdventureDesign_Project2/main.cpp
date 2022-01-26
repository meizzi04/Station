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
    int line = 0;     // 호선
    int station_num = 0;     // 역 번호
    char station_name[Size] = {};  // 역 이름
    int transfer = 0; // 환승역
    int nextCount = 0;  // 다음으로 갈 수 있는 역의 개수
    int* nextList = new int[10];  // 다음으로 갈 수 있는 역들을 저장한 배열
    bool visited = 0;   // 방문 여부 확인

    Station(int a, int b, char* c, int d, int e)    // 생성자
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

vector <Station> stationList;  // 역 정보들 저장
vector<Station> path;   // 지나온 경로 저장

int n = 1;  // 경로 번호

int main()
{
    int menu_num;

    while (1)
    {
        cout << "<지하철 길 찾기>" << endl;
        cout << "1. 프로그램 실행" << endl;
        cout << "2. 프로그램 종료" << endl;
        cout << "위의 메뉴 중 하나를 선택하시오. ";
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
        cout << endl << "// 프로그램을 종료합니다." << endl;
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
        cout << "원하는 지역을 선택하시오. ";
        cin >> area;

        if (area != "대구")
        {
            cout << "// 선택한 지역은 아직 업데이트되지 않았습니다." << endl << endl;
            cin.clear();
            cin.ignore();
        }
    } while (area != "대구");

    cout << endl;
    cout << "< 대구 지하철 노선도 >" << endl;

    cout << "- 1호선 : 설화명곡, 화원, 대곡, 진천, 월배, 상인, 월촌, 송현, 성당못, 대명, 안지랑, "
        << "현충로, 영대병원, 교대, 명덕, 반월당, 중앙로, 대구역, 칠성시장, 신천, 동대구역, 동구청, "
        << "아양교, 동촌, 해안, 방촌, 용계, 율하, 신기, 반야월, 각산, 안심" << endl;
    cout << "- 2호선 : 문양, 다사, 대실, 강창, 계명대, 성서산업단지, 이곡, 용산, 죽전, 감삼, 두류, 내당"
        << "반고개, 신남, 반월당, 경대병원, 대구은행, 범어, 수성구청, 만촌, 담티, 연호, 대공원, 고산, "
        << "신매, 사월, 정평, 임당, 영남대" << endl;
    cout << "- 3호선 : 칠곡경대병원, 학정, 팔거, 동천, 칠곡운암, 구암, 태전, 매천, 매천시장"
        << "팔달, 공단, 만평, 팔달시장, 원대, 북구청, 달성공원, 서문시장, 신남, 남산, 명덕, "
        << "건들바위, 대봉교, 수성시장, 수성구민운동장, 어린이회관, 황금, 수성못, 지산, 범물, 용지" << endl;
    cout << endl << "// 위의 장소 중 한 곳을 선택하시오." << endl;
    
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
        cout << "위의 경로 중 원하는 경로를 하나 선택하시오. ";
        cin >> a;
        if (a >= n || a == 0)
        {
            cout << "// 경로의 번호를 다시 확인해주세요." << endl << endl;
            cin.clear();
            cin.ignore();
        }

    } while (a >= n || a == 0);

    cout << endl;

}

void ListData()  // 파일의 데이터를 읽어서 배열에 저장하는 함수
{
    FILE* file = NULL;
    file = fopen("station.txt", "r");
    if (file != NULL)
    {
        char arr[Size];
        while (!feof(file))
        {
            fgets(arr, sizeof(arr), file);

            int line;     // 호선
            int station_num;     // 역 번호
            char station_name[Size];  // 역 이름
            int transfer;
            int nextCount;  // 다음으로 갈 수 있는 역의 개수

            char* stationArr[Size];    // 전체 station을 저장할 객체 배열

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

int* InputData(int startnum, int destnum)// 사용자로부터 출발역과 도착역을 입력받는 함수
{
    string start;
    string dest;
    //bool s;

    do
    {
        cout << "출발역 : ";
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
            cout << "// 입력하신 출발역은 존재하지 않습니다. 다시 입력해주세요." << endl;
        }
        cout << endl;
    } while (startnum == 0);

    do
    {
        cout << "도착역 : ";
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
            cout << "// 입력하신 도착역은 존재하지 않습니다. 다시 입력해주세요." << endl;
        }

        cout << endl;

    } while (destnum == 0);

    cout << "// 선택한 경로에 대한 정보를 파일에서 찾아 출력합니다." << endl;
    cout << "< 출발역 : " << start << ", 도착역 : " << dest << " >" << endl << endl;

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
            cout << path[i].station_name << "(" << line << "호선) - ";

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
        cout << path[i].station_name << "(" << line << "호선)";
    }
    else
    {
        count++;
        time += 2;
        cout << path[i].station_name;
    }
    cout << " : " << count << "역, " << trans << "환승, " << time << "분" << endl << endl;

    n++;
    count = 0;
    time = 0;
}

void DFS(vector<Station> path, int count, int currentnum, int destnum, vector<Station> visited)  // 재귀를 통해 출발역부터 도착역까지 가는 모든 경로를 찾는 함수
{
    // 현재역이 도착역인 경우
    if (currentnum == destnum)
    {
        path.push_back(stationList[destnum]);  // 경로에 도착역 추가

        printpath(path, currentnum);  // 경로 출력

        path.pop_back();  // 경로에서 마지막역 제거
        return;
    }

    // 종점이거나 이미 한번 방문한 역인 경우
    if (stationList[currentnum].nextCount == 1 && count > 0 || visited[currentnum].visited)
    {
        return;
    }

    visited[currentnum].visited = true; // 현재역을 방문했다고 처리
    path.push_back(stationList[currentnum]); // 경로에 현재역 추가


    // 현재역에서 갈 수 있는 역들에 대해 dfs 재귀
    for (int i = 0; i < stationList[currentnum].nextCount; i++)
    {
        DFS(path, count + 1, stationList[currentnum].nextList[i], destnum, visited);
        if (i > 1)
            path.pop_back(); // 경로에서 삭제
    }
    visited[currentnum].visited = false;// dfs가 끝난 뒤에 방문 처리 했던 것들을 해제 
    if (!path.empty())
        path.pop_back(); // 경로에서 삭제
}