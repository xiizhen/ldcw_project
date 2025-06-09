#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <sstream>
#include <algorithm>
#include <cctype>     // for toupper
#include <windows.h>  // for system("cls") on Windows

using namespace std;

// 屏幕清除函数
void clearScreen() {
    system("cls");  // Windows 系统
}

// 问题数据结构
struct Question {
    string questionText;
    set<char> correctAnswers;
    string explanation;
};

// 解析答案行
set<char> parseAnswers(const string& answerLine) {
    set<char> correctAnswers;
    size_t pos = answerLine.find("Answer:");
    if (pos != string::npos) {
        string answerPart = answerLine.substr(pos + 7);
        stringstream ss(answerPart);
        string token;
        while (getline(ss, token, ',')) {
            for (size_t i = 0; i < token.size(); ++i) {
                char c = token[i];
                if (isalpha(c)) {
                    correctAnswers.insert(toupper(c));
                }
            }
        }
    }
    return correctAnswers;
}

// 解析解释行
string parseExplanation(const string& line) {
    size_t pos = line.find("Explanation:");
    if (pos != string::npos) {
        return line.substr(pos + 12);
    }
    return "";
}

// 获取用户答案
set<char> getUserAnswers() {
    string input;
    cout << "Your answer (A/B/C/D or multiple like A,C): ";
    cin >> input;
    cin.ignore(); // 清除输入缓冲区

    set<char> userAnswers;
    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        if (isalpha(c)) {
            userAnswers.insert(toupper(c));
        }
    }
    return userAnswers;
}

// 获取用户反馈
string getUserFeedback() {
    string feedback;
    cout << "\nPlease share your feedback about this quiz:\n";
    getline(cin, feedback);
    return feedback;
}

// 获取评分评语
string getReview(int score) {
    if (score <= 3) return "Needs Improvement";
    else if (score <= 7) return "Good Job!";
    else return "Excellent! You’re a Smart City Transport Champion!";
}

// 保存用户数据
void saveUserData(const string& name, int score, const string& review, const string& feedback) {
    ofstream file("user.txt", ios::app); // 追加模式
    if (file.is_open()) {
        file << "Name: " << name << "\n";
        file << "Score: " << score << "/15\n";
        file << "Review: " << review << "\n";
        file << "Feedback: " << feedback << "\n";
        file << "------------------------\n";
        file.close();
        cout << "Your data has been saved successfully.\n";
    } else {
        cerr << "Unable to save user data.\n";
    }
}

// 测验系统类
class EcoTransportQuizSystem {
public:
    // 构造函数 - 显示加载动画
    EcoTransportQuizSystem() {
        system("COLOR 0e");
        system("cls");

        SetConsoleCP(437);
        SetConsoleOutputCP(437);

        int bar1 = 219;

        cout << "\n\n\n\t\t\t\tWelcome to Eco-Friendly Transportation Quiz :)";
        cout << "\n\n\n\t\t\t\t";

        cout << "\r\t\t\t\t";

        for (int i = 0; i <= 100; i += 4) {
            cout << " " << i << "%";
            cout << flush;
            Sleep(15);
            cout << "\r\t\t\t\t";
            for (int j = 0; j < i / 4; j++) {
                cout << (char)bar1;
            }
        }
        cout << "\n\t\t\t\t" << (char)1 << "!";
        cout << endl;
        system("pause");
        system("cls");
    }

    // 析构函数
    ~EcoTransportQuizSystem() {
        cout << "Thank you for using our Eco-Friendly Transportation Quiz. Goodbye!\n";
    }
};

int main() {
    // 初始化测验系统
    EcoTransportQuizSystem quizSystem;

    // 打开问题文件
    ifstream file("question.txt");
    if (!file) {
        cout << "Error: Could not open question.txt file." << endl;
        return 1;
    }

    string userName;
    int correctCount = 0;
    int totalQuestions = 0;

    // 获取用户姓名
    clearScreen();
    cout << "===== Welcome to the Green City, Smart Living Quiz! =====" << endl;
    cout << "Topic: Eco-Friendly Transportation in Smart Cities" << endl;
    cout << "Please enter your name: ";
    getline(cin, userName);

    // 显示测验说明
    clearScreen();
    cout << "\nHi " << userName << "! You will be asked 15 multiple-choice questions." << endl;
    cout << "Each correct answer gives you 1 point. Select all correct options for multiple-answer questions.\n\nLet's begin!\n\n";

    // 读取问题文件
    string line;
    while (getline(file, line)) {
        Question currentQuestion;
        currentQuestion.questionText = "";

        // 读取问题文本
        while (line.find("Answer:") == string::npos && !file.eof()) {
            currentQuestion.questionText += line + "\n";
            getline(file, line);
        }

        // 解析正确答案
        currentQuestion.correctAnswers = parseAnswers(line);

        // 读取解释
        if (getline(file, line)) {
            currentQuestion.explanation = parseExplanation(line);
        }

        // 显示问题
        totalQuestions++;
        cout << "===== Question " << totalQuestions << " =====" << endl;
        cout << currentQuestion.questionText << endl;

        // 获取用户答案
        set<char> userAnswers = getUserAnswers();

        // 检查答案
        if (userAnswers == currentQuestion.correctAnswers) {
            cout << "Correct!\n\n";
            correctCount++;
        } else {
            cout << "Wrong! The correct answer was: ";
            set<char>::iterator it;
            for (it = currentQuestion.correctAnswers.begin(); it != currentQuestion.correctAnswers.end(); ++it) {
                cout << *it << " ";
            }
            cout << "\n\n";

            // 显示解释
            if (!currentQuestion.explanation.empty()) {
                cout << "Explanation:\n" << currentQuestion.explanation << "\n\n";
            }
        }
    }

    file.close();

    // 计算评分和评语
    string review = getReview(correctCount);

    // 获取用户反馈
    string feedback = getUserFeedback();

    // 保存用户数据
    saveUserData(userName, correctCount, review, feedback);

    // 显示结果
    clearScreen();
    cout << "===== Quiz Complete! =====" << endl;
    cout << userName << ", you scored " << correctCount << " out of " << totalQuestions << "." << endl;
    cout << "Review: " << review << endl;

    system("pause"); // 保持窗口打开
    return 0;
}
