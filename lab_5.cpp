#include <iostream>
#include <string>
#include <windows.h>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

// Структура для хранения данных о студенте
struct Student {
    string fam;       // Фамилия
    string im;        // Имя
    string otch;      // Отчество
    int group{};      // Номер группы
    int notes[5]{};   // Оценки (5 оценок)
    double average{}; // Средний балл
    int kol2{};       // Количество неудовлетворительных оценок
};

// Структура для хранения статистики по группам
struct GroupStats {
    int group{};     // Номер группы
    int count{};     // Количество студентов в группе
    int badcount{};  // Количество студентов с неудовлетворительными оценками
};

// Функция для вывода разделителя заданной длины и символа
void printSeparator(int length, string symbol) {
    for (int i = 0; i < length; i++)
        cout << symbol;
}

// Функция для ввода данных о студентах
void inputStudents(bool isInteractive, int N, vector<Student>& students) {
    for (int i = 0; i < N; i++) {
        if (isInteractive)
            cout << "Введите данные студента: Фамилия Имя Отчество Группа и 5 оценок:" << endl;

        Student s;
        cin >> s.fam >> s.im >> s.otch >> s.group; // Ввод ФИО и номера группы
        float sum = 0;
        s.kol2 = 0;

        for (int j = 0; j < 5; j++) {
            cin >> s.notes[j]; // Ввод оценок
            sum += s.notes[j];
            if (s.notes[j] == 2)
                s.kol2++; // Подсчёт количества неудовлетворительных оценок
        }

        s.average = round(sum / 5.0 * 10.0) / 10.0; // Расчёт среднего балла
        students.push_back(s); // Добавление студента в вектор
    }
}

// Функция для подсчёта уникальных групп
int countUniqueGroups(const vector<Student>& students) {
    vector<int> groups;
    for (const auto& student : students)
        if (find(groups.begin(), groups.end(), student.group) == groups.end())
            groups.push_back(student.group);

    return groups.size(); // Возвращает количество уникальных групп
}

// Функция для заполнения статистики по группам
void fillGroupStats(const vector<Student>& students, vector<GroupStats>& groupStats) {
    for (const auto& student : students) {
        auto it = find_if(groupStats.begin(), groupStats.end(), [&student](const GroupStats& g) {
            return g.group == student.group;
            });

        if (it != groupStats.end()) {
            it->count++; // Увеличение количества студентов в группе
            if (student.kol2 > 0)
                it->badcount++; // Увеличение количества студентов с неудовлетворительными оценками
        }
        else {
            GroupStats g;
            g.group = student.group;
            g.count = 1;
            g.badcount = student.kol2 > 0 ? 1 : 0;
            groupStats.push_back(g); // Добавление новой группы в статистику
        }
    }
}

// Функция для сортировки студентов и групп
void sortStudentsAndGroups(vector<Student>& students, vector<GroupStats>& groupStats) {
    sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.group < b.group; // Сортировка студентов по номеру группы
        });

    sort(groupStats.begin(), groupStats.end(), [](const GroupStats& a, const GroupStats& b) {
        return a.badcount > b.badcount; // Сортировка групп по количеству "двоечников"
        });
}

// Функция для вывода таблицы студентов
void printStudentTable(const vector<Student>& students) {
    int nameWidth = 30; // Ширина для ФИО

    cout << "____________________________________________________________" << endl;
    cout << "| Группа |               ФИО                | Успеваемость | " << endl;
    cout << "|________|__________________________________|______________|" << endl;

    for (const auto& student : students) {
        string fullName = student.fam + " " + student.im + " " + student.otch;
        cout << "| " << setw(6) << right << student.group << " | "
            << setw(nameWidth) << left << fullName << "   | ";

        for (int j = 0; j < 5; j++) {
            cout << student.notes[j] << (j < 4 ? "," : "    "); // Вывод оценок
        }
        cout << "| " << endl;
    }

    cout << "|________|__________________________________|______________|" << endl;
}

// Функция для вывода статистики по группам
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

// Функция для вывода студентов с высоким средним баллом
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
    SetConsoleCP(1251); // Установка кодировки консоли для ввода
    SetConsoleOutputCP(1251); // Установка кодировки консоли для вывода

    bool isHuman = false; // Флаг режима работы (интерактивный или автоматический)
    if (argc > 1 && strcmp(argv[1], "false") == 0)
        isHuman = false;

    int N;
    if (isHuman) {
        cout << "Введите количество студентов:" << endl;
    }
    cin >> N; // Ввод количества студентов

    vector<Student> students;
    inputStudents(isHuman, N, students); // Ввод данных о студентах

    vector<GroupStats> groupStats;
    fillGroupStats(students, groupStats); // Заполнение статистики по группам

    sortStudentsAndGroups(students, groupStats); // Сортировка студентов и групп

    if (isHuman) {
        cout << "Таблица студентов:" << endl;
        printStudentTable(students); // Вывод таблицы студентов

        cout << "\nСтуденты с средним баллом больше 4:" << endl;
        printHighAverageTable(students); // Вывод студентов с высоким средним баллом

        cout << "\nТаблица статистики групп:" << endl;
        printGroupStatsTable(groupStats); // Вывод статистики по группам
    }
    else {
        printStudentTable(students); // Вывод таблицы студентов

        printHighAverageTable(students); // Вывод студентов с высоким средним баллом

        printGroupStatsTable(groupStats); // Вывод статистики по группам

        for (const auto& student : students) {
            cout << student.group << " - " << student.fam << " " << student.im << " " << student.otch
                << " (Средний балл: " << student.average << ")\n";
        }
    }
}
