#include <iostream>
#include <string>
#include <windows.h>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

struct Student {
    string fam;
    string im;
    string otch;
    int group{};
    int notes[5]{};
    double average{};
    int kol2{}; // Количество неудовлетворительных оценок
};

struct GroupStats {
    int group{};
    int count{}; // Количество студентов в группе
    int badcount{}; // Количество студентов с "2"
};

void printSeparator(int length, string symbol) {
    for (int i = 0; i < length; i++)
        cout << symbol;
}

void inputStudents(bool isInteractive, int N, vector<Student>& students) {
    for (int i = 0; i < N; i++) {
        if (isInteractive)
            cout << "Введите данные студента: Фамилия Имя Отчество Группа и 5 оценок:" << endl;

        Student s;
        cin >> s.fam >> s.im >> s.otch >> s.group;
        float sum = 0;
        s.kol2 = 0;

        for (int j = 0; j < 5; j++) {
            cin >> s.notes[j];
            sum += s.notes[j];
            if (s.notes[j] == 2)
                s.kol2++;
        }

        s.average = round(sum / 5.0 * 10.0) / 10.0;
        students.push_back(s);
    }
}

int countUniqueGroups(const vector<Student>& students) {
    vector<int> groups;
    for (const auto& student : students)
        if (find(groups.begin(), groups.end(), student.group) == groups.end())
            groups.push_back(student.group);

    return groups.size();
}

void fillGroupStats(const vector<Student>& students, vector<GroupStats>& groupStats) {
    for (const auto& student : students) {
        auto it = find_if(groupStats.begin(), groupStats.end(), [&student](const GroupStats& g) {
            return g.group == student.group;
            });

        if (it != groupStats.end()) {
            it->count++;
            if (student.kol2 > 0)
                it->badcount++;
        }
        else {
            GroupStats g;
            g.group = student.group;
            g.count = 1;
            g.badcount = student.kol2 > 0 ? 1 : 0;
            groupStats.push_back(g);
        }
    }
}

void sortStudentsAndGroups(vector<Student>& students, vector<GroupStats>& groupStats) {
    sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.group < b.group;
        });

    sort(groupStats.begin(), groupStats.end(), [](const GroupStats& a, const GroupStats& b) {
        return a.badcount > b.badcount;
        });
}

void printStudentTable(const vector<Student>& students) {
    int nameWidth = 30; // Ширина для ФИО

    cout << "_____________________________________________________________________________" << endl;
    cout << "| Группа |               ФИО                | Средний балл | Неудовл. оценки |" << endl;
    cout << "|________|__________________________________|______________|_________________|" << endl;

    for (const auto& student : students) {
        string fullName = student.fam + " " + student.im + " " + student.otch;
        cout << "| " << setw(6) << right << student.group << " | "
            << setw(nameWidth) << left << fullName << "   | ";

        for (int j = 0; j < 5; j++) {
            cout << student.notes[j] << (j < 4 ? "," : "    ");
        }

        cout << "| " << setw(15) << student.kol2 << " |" << endl;
    }

    cout << "|________|__________________________________|______________|_________________|" << endl;
}

void printGroupStatsTable(const vector<GroupStats>& groupStats) {
    cout << "____________________________________________" << endl;
    cout << "| Группа | Кол-во студентов | Студентов с 2 |" << endl;
    cout << "|________|__________________|_______________|" << endl;

    for (const auto& g : groupStats) {
        cout << "| " << setw(6) << g.group << " | "
            << setw(16) << g.count << " | "
            << setw(13) << g.badcount << " |" << endl;
    }

    cout << "|________|__________________|_______________|" << endl;
}

void printHighAverageTable(const vector<Student>& students) {
    int nameWidth = 30; // Ширина для ФИО

    cout << "___________________________________________________________" << endl;
    cout << "| Группа |               ФИО                | Средний балл |" << endl;
    cout << "|________|__________________________________|______________|" << endl;

    for (const auto& student : students) {
        if (student.average > 4) {
            string fullName = student.fam + " " + student.im + " " + student.otch;
            cout << "| " << setw(6) << right << student.group << " | "
                << setw(nameWidth) << left << fullName << "   | "
                << setw(12) << student.average << " |" << endl;
        }
    }

    cout << "|________|__________________________________|______________|" << endl;
}

int main(int argc, char* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    bool isInteractive = true;
    if (argc > 1 && strcmp(argv[1], "false") == 0)
        isInteractive = false;

    int N;
    if (isInteractive) {
        cout << "Введите количество студентов:" << endl;
    }
    cin >> N;

    vector<Student> students;
    inputStudents(isInteractive, N, students);

    vector<GroupStats> groupStats;
    fillGroupStats(students, groupStats);

    sortStudentsAndGroups(students, groupStats);

    if (isInteractive) {
        cout << "Таблица студентов:" << endl;
        printStudentTable(students);

        cout << "\nСтуденты с средним баллом больше 4:" << endl;
        printHighAverageTable(students);

        cout << "\nТаблица статистики групп:" << endl;
        printGroupStatsTable(groupStats);
    }
    else {
        for (const auto& student : students) {
            cout << student.group << " - " << student.fam << " " << student.im << " " << student.otch
                << " (Средний балл: " << student.average << ")\n";
        }
    }

    return 0;
}
