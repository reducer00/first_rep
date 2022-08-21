// Course_work_white_2nd_attempt.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <exception>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <set>
#include <iomanip>

using namespace std;

class Date {
public:
    Date() {
        day = 0;
        month = 0;
        year = 0;
    }

    Date(int new_day, int new_month, int new_year) {

        if ((new_month <= 12) && (new_month > 0)) {
            month = new_month;
        }
        else {
            stringstream ss;
            string error = "Month value is invalid: ";
            error += to_string(new_month);
            ss << error;
            throw runtime_error(ss.str());
        }

        if ((new_day <= 31) && (new_day > 0)) {
            day = new_day;
        }
        else {
            stringstream ss;
            string error = "Day value is invalid: ";
            error += to_string(new_day);
            ss << error;
            throw runtime_error(ss.str());
        }
        year = new_year;
    }

    int GetYear() const {
        return year;
    };
    int GetMonth() const {
        return month;
    };
    int GetDay() const {
        return day;
    };
private:
    int year;
    int month;
    int day;
};

stringstream& Send_WDF(const string& str, stringstream& sstr) {
    string error = "Wrong date format: ";
    error += str;
    sstr << error;
    return sstr;
}

istream& operator >> (istream& is, Date& new_date) {
    string str;
    int year, month, day = -1;
    char c;
    stringstream loc_is;
    getline(is, str, ' ');
    loc_is << str;

    if (loc_is) {
        
        loc_is >> year >> c >> month;
        if (loc_is) {
            if (c == '-') {
                loc_is >> c >> day;
                if (c == '-') {
                    if ((loc_is.eof()) && (day == -1)) {
                        stringstream ss;
                        Send_WDF(str, ss);
                        throw runtime_error(ss.str());
                    }
                    else if (!loc_is.eof()) {
                        stringstream ss;
                        Send_WDF(str, ss);
                        throw runtime_error(ss.str());
                    }
                    new_date = Date(day, month, year);
                }
                else if (!loc_is.eof()) {
                    stringstream ss;
                    Send_WDF(str, ss);
                    throw runtime_error(ss.str());
                }
                else {
                    stringstream ss;
                    Send_WDF(str, ss);
                    throw runtime_error(ss.str());
                }
            }
            else {
                stringstream ss;
                Send_WDF(str, ss);
                throw runtime_error(ss.str());
            }
        }
        else {
            stringstream ss;
                    Send_WDF(str, ss);
                    throw runtime_error(ss.str());
        }
    }

    return is;
}

ostream& operator << (ostream& os, const Date& d) {
    return os << setfill('0') << setw(4) << d.GetYear()
        << "-" << setw(2) << d.GetMonth()
        << "-" << setw(2) << d.GetDay();
}

bool operator<(const Date& lhs, const Date& rhs) {

    if (lhs.GetYear() == rhs.GetYear()) {
        if (lhs.GetMonth() == rhs.GetMonth()) {
            if (lhs.GetDay() < rhs.GetDay()) {
                return true;
            }
            else {
                return false;
            }
        }
        else if (lhs.GetMonth() < rhs.GetMonth()) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (lhs.GetYear() < rhs.GetYear()) {
        return true;
    }
    else {
        return false;
    }
}

class Database {
public:
    void AddEvent(const Date& date, const string& new_event) {

        database[date].insert(new_event);
    }
    bool DeleteEvent(const Date& date, const string& magic) {

        if (database[date].count(magic) == 1) {
            database[date].erase(magic);
            return true;
        }
        else {
            return false;
        }

    }
    int  DeleteDate(const Date& date) {

        if (database.empty()) {
            return 0;
        }
        else {
            if (database.at(date).empty()) {
                return 0;
            }
            else {
                int result = database.at(date).size();      //find the quality of events 

                database.erase(date);

                return result;
            }
        }
    }

    void Find(const Date& date) const {

        for (string c : database.at(date)) {
            cout << c << endl;
        }
    }

    void Print() const {

        for (const auto& item : database) {
            for (string c : item.second) {
                cout << item.first << " "
                    << c << endl;
            }
        }
    }
private:
    map<Date, set<string>> database;
};

int main() {
    /*Database db;

    string command;
    while (getline(cin, command)) {
        // Считайте команды с потока ввода и обработайте каждую
    }*/

    string input;

    Date date;

    Database db;

    int quality;
    try {

        while (getline(cin, input)) {

            stringstream sstr;

            sstr << input;

            string command = "", new_event = "", check;

            getline(sstr, command, ' ');

            if (command == "Add") {
                sstr >> date;
                sstr >> new_event;
                db.AddEvent(date, new_event);
            }
            else if (command == "Del") {
                sstr >> date >> new_event;
                if (new_event == "") {                                              //обработка удлаения чисто даты
                    try {                                                           //ловим исключение out_of_range - выдает 
                        quality = db.DeleteDate(date);                              //при отсутствии такого контейнера из методов size и empty
                        cout << "Deleted " << quality << " events" << endl;
                    }
                    catch (out_of_range&) {
                        cout << "Deleted 0 events" << endl;
                    }
                }
                else {                                                              //обработка удаления события
                    if (db.DeleteEvent(date, new_event)) {
                        cout << "Deleted successfully" << endl;
                    }
                    else {
                        cout << "Event not found" << endl;
                    }
                }
            }
            else if (command == "Find") {
                sstr >> date;
                try {
                    db.Find(date);
                }
                catch(out_of_range&){
                }
            }
            else if (command == "Print") {
                db.Print();
            }
            else if (command == "") {
                continue;
            }
            else {
                stringstream ss;
                string error = "Unknown command: ";
                error += command;
                ss << error;
                throw runtime_error(ss.str());
            }
        }
    }
    catch (runtime_error& e) {
        cout << e.what() << endl;
    }


    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
