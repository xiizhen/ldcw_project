#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <windows.h>

using namespace std;

// 娓呭睆鍑芥暟
void clearScreen() {
    system("cls");
}

// 灞呬腑鏄剧ず鏂囧瓧
void centerText(const string& text, int width = 80) {
    int padding = (width - text.length()) / 2;
    if (padding > 0) cout << string(padding, ' ');
    cout << text << endl;
}

// 棰樼洰缁撴瀯
struct Question {
    string questionText;
    set<char> correctAnswers;
    string explanation;
};

// 瑙ｆ瀽绛旀
set<char> parseAnswers(const string& answerLine) {
    set<char> correctAnswers;
    size_t pos = answerLine.find("Answer:");
    if (pos != string::npos) {
        string answerPart = answerLine.substr(pos + 7);
        stringstream ss(answerPart);
        string token;
        while (getline(ss, token, ',')) {
            for (char c : token) {
                if (isalpha(c)) {
                    correctAnswers.insert(toupper(c));
                }
            }
        }
    }
    return correctAnswers;
}

// 瑙ｆ瀽瑙ｉ噴
string parseExplanation(const string& line) {
    size_t pos = line.find("Explanation:");
    if (pos != string::npos) {
        return line.substr(pos + 12);
    }
    return "";
}

// 妫€鏌ユ槸鍚︽湁鏁堢瓟妗堬紙鍙厑璁?A-D锛?
bool isValidAnswer(const string& input) {
    for (char c : input) {
        if (isalpha(c)) {
            char upper = toupper(c);
            if (upper < 'A' || upper > 'D') {
                return false;
            }
        }
    }
    return true;
}

// 鑾峰彇鐢ㄦ埛杈撳叆绛旀
set<char> getUserAnswers() {
    string input;
    set<char> userAnswers;

    while (true) {
        cout << "\nYour answer (A/B/C/D or multiple like A,C): ";
        getline(cin, input);
        userAnswers.clear();

        if (!isValidAnswer(input)) {
            cout << "Invalid input. Please enter only A, B, C, or D.\n";
            continue;
        }

        for (char c : input) {
            if (isalpha(c)) {
                userAnswers.insert(toupper(c));
            }
        }

        if (!userAnswers.empty()) break;
        cout << "Please enter at least one valid option.\n";
    }

    return userAnswers;
}

// 鐢ㄦ埛鍙嶉
string getUserFeedback(const string& userName) 
{
	
    string feedback;
    
    clearScreen();
    centerText("========================================");
    centerText("                FEEBACK                 ");
    centerText("========================================\n");
    cout << "\n" << userName << ", please share your feedback about this quiz:\n";
    getline(cin, feedback);
    return feedback;
}

// 璇勫垎鏂囧瓧
string getReview(int score) {
    if (score <= 3) return "Needs Improvement";
    else if (score <= 7) return "Good Job!";
    else return "Excellent! You are a Smart City Transport Champion!";
}

// 淇濆瓨鎴愮哗
void saveUserData(const string& name, int score, const string& review, const string& feedback) {
    ofstream file("user.txt", ios::app);
    if (file.is_open()) {
        file << "Name: " << name << "\n";
        file << "Score: " << score << "/15\n";
        file << "Review: " << review << "\n";
        file << "Feedback: " << feedback << "\n";
        file << "------------------------\n";
        file.close();
        cout << "\nYour data has been saved successfully.\n";
    } else {
        cerr << "Error: Unable to save user data.\n";
    }
}

// 娆㈣繋鍔ㄧ敾
void showIntro() {
    system("COLOR 0E");
    clearScreen();
    SetConsoleCP(437);
    SetConsoleOutputCP(437);

    int bar1 = 219;
    centerText("Welcome to Eco-Friendly Transportation Quiz");
    centerText("Loading, please wait...\n");

    cout << "\t\t\t";

    for (int i = 0; i <= 100; i += 4) {
        cout << " " << i << "%";
        cout << flush;
        Sleep(10);
        cout << "\r\t\t\t";
        for (int j = 0; j < i / 4; j++) {
            cout << (char)bar1;
        }
        Sleep(10);
    }

    cout << "\n";
    system("pause");
    clearScreen();
}

// 涓诲嚱鏁?
int main() {
    showIntro();
    clearScreen();

    ifstream file("question.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open question.txt file.\n";
        return 1;
    }

    string userName;
    centerText("===== Green City, Smart Living Quiz =====");
    centerText("Topic: Eco-Friendly Transportation in Smart Cities\n");
    cout << "\nEnter your name: ";
    getline(cin, userName);

    clearScreen();
    centerText("Hi " + userName + "! You will be asked 15 multiple-choice questions.");
    centerText("Each correct answer gives you 1 point.\n");
    system("pause");
    clearScreen();

    string line;
    int correctCount = 0;
    int totalQuestions = 0;

    while (getline(file, line)) {
        Question q;
        q.questionText = "";

        while (line.find("Answer:") == string::npos && !file.eof()) {
            q.questionText += line + "\n";
            getline(file, line);
        }

        q.correctAnswers = parseAnswers(line);

        if (getline(file, line)) {
            q.explanation = parseExplanation(line);
        }

        clearScreen();

        totalQuestions++;
        centerText("===== Question " + to_string(totalQuestions) + " =====");
        cout << q.questionText << endl;

        set<char> userAnswers = getUserAnswers();
        cout << "\n";

        if (userAnswers == q.correctAnswers) {
            centerText("Correct!\n");
            correctCount++;
        } else {
            string correctAnsText = "Wrong! Correct answer: ";
            for (char ans : q.correctAnswers) {
                correctAnsText += ans;
                correctAnsText += ' ';
            }
            centerText(correctAnsText);
            cout << "\n";

            if (!q.explanation.empty()) {
                cout << "\nExplanation:\n" << q.explanation << "\n";
            }
        }

        system("pause");
    }

    file.close();
    clearScreen();

    string review = getReview(correctCount);
    string feedback = getUserFeedback(userName);
    saveUserData(userName, correctCount, review, feedback);

    clearScreen();
    centerText("========================================");
    centerText("             QUIZ COMPLETED             ");
    centerText("========================================\n");

    centerText("Thank you, " + userName + "!");
    centerText("You scored " + to_string(correctCount) + " out of " + to_string(totalQuestions) + ".");
    centerText("Performance Review: " + review + "\n");

    centerText("Your feedback has been recorded.");
    centerText("We hope you enjoyed the quiz!\n");

    centerText("========================================");

    system("pause");
    return 0;
}
