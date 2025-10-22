#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

const int MAX_SONGS = 100;
const int MAX_NAME  = 35;
const int MAX_LINES = 100;
const int MAX_TEXT  = 150;

struct Song {
    int  id;
    char name[MAX_NAME];
char author[MAX_NAME];
    int  year;
    int  lineCount;
    char text[MAX_LINES][MAX_TEXT];
};

Song listSongs[MAX_SONGS];
int  songTotal = 0;
int  nextId    = 1;

void clearInput() {
    cin.clear();
cin.ignore(10000, '\n');
}

 void krt(char* s) {
        s[strcspn(s, "\r\n")] = '\0';
}

//  імпорт тексту з файлу
void addFile(Song &s) {
    clearInput();
    
    cout << "Ім'я файлу: ";
        char file[100];
    cin.getline(file, 100);

FILE* f = fopen(file, "r");
    if (!f) {
        cout << "Не вдалось відкрити файл.\n";
        s.lineCount = 0;
        return;
    }

    s.lineCount = 0;
    while (s.lineCount < MAX_LINES && fgets(s.text[s.lineCount], MAX_TEXT, f)) {
            krt(s.text[s.lineCount]);
        s.lineCount++;
    }
    fclose(f);
    cout << "Текст завантажено.\n";
}

//  ручний ввід тексту
void TextHand(Song &s) {
    cout << "Введи текст (порожній рядок = кінець):\n";
    s.lineCount = 0;
    while (s.lineCount < MAX_LINES) {
        char line[MAX_TEXT];
            cin.getline(line, MAX_TEXT);
        if (strlen(line) == 0) break;
            strncpy(s.text[s.lineCount], line, MAX_TEXT - 1);
            s.text[s.lineCount][MAX_TEXT - 1] = '\0';
            s.lineCount++;
    }
}

// додати пісню
void addSong() {
    if (songTotal >= MAX_SONGS) { cout << "Нема місця!\n"; return; }

    Song s{};
    s.id = nextId++;

    clearInput();
        cout << "Назва: ";
    cin.getline(s.name, MAX_NAME);

            cout << "Автор: ";
    cin.getline(s.author, MAX_NAME);

    cout << "Рік (-1 якщо невідомо): ";
    if (!(cin >> s.year)) { s.year = -1; }
        clearInput();

    cout << "Текст: 1 - вручну, 2 - з файлу: ";
        int choice = 1;
    if (!(cin >> choice)) { clearInput(); choice = 1; }
        clearInput();

    if (choice == 2) addFile(s);
    
    else             TextHand(s);

    listSongs[songTotal++] = s;
    cout << "Пісня додана.\n";
}

// список
void showAll() {
        if (songTotal == 0) { cout << "(порожньо)\n"; return; }
    for (int i = 0; i < songTotal; i++) {
        
        
        cout << listSongs[i].id << " | " << listSongs[i].name
        << " — " << listSongs[i].author
        
             << " (" << listSongs[i].year << ")\n";
    }
}

// показати текст за ID
void showText() {
        cout << "ID пісні: ";
    int id;
        if (!(cin >> id)) { clearInput(); cout << "Невірний ID.\n"; return; }
    clearInput();

    for (int i = 0; i < songTotal; i++) {
            if (listSongs[i].id == id) {
                
                
            cout << "\n=== " << listSongs[i].name << " ===\n";
            for (int j = 0; j < listSongs[i].lineCount; j++) {
                
                cout << listSongs[i].text[j] << "\n";
            }
            return;
        }
    }
    cout << "Не знайдено.\n";
}

// пошук по автору
void findAuthor() {
clearInput();
    cout << "Автор (частина): ";
        char q[MAX_NAME];
    cin.getline(q, MAX_NAME);

    bool any = false;
    for (int i = 0; i < songTotal; i++) {
        
        
        if (strstr(listSongs[i].author, q)) {
            cout << listSongs[i].id << " | " << listSongs[i].name
                 << " — " << listSongs[i].author << "\n";
            any = true;
        }
    }
    if (!any) cout << "Нічого не знайдено.\n";
}

// збереження
void saveFile() {
    FILE* f = fopen("songs.txt", "w");
    
    if (!f) { cout << "Помилка збереження.\n"; return; }

    fprintf(f, "%d\n", songTotal);
    for (int i = 0; i < songTotal; i++) {
        fprintf(f, "%d\n", listSongs[i].id);
    fprintf(f, "%s\n", listSongs[i].name);
        
            fprintf(f, "%s\n", listSongs[i].author);
        fprintf(f, "%d\n", listSongs[i].year);
            fprintf(f, "%d\n", listSongs[i].lineCount);
        for (int j = 0; j < listSongs[i].lineCount; j++) {
            fprintf(f, "%s\n", listSongs[i].text[j]);
        }
    }
    fclose(f);
    cout << "Збережено у songs.txt\n";
    
    
}

//  завантаження
void loadFile() {
    FILE* f = fopen("songs.txt", "r");
    if (!f) return;

    
    
    int cnt = 0;
if (fscanf(f, "%d\n", &cnt) != 1 || cnt < 0) { fclose(f); return; }
    songTotal = (cnt > MAX_SONGS) ? MAX_SONGS : cnt;

    for (int i = 0; i < songTotal; i++) {
        if (fscanf(f, "%d\n", &listSongs[i].id) != 1) { songTotal = i; break; }
        
        

if (!fgets(listSongs[i].name, MAX_NAME, f))  { songTotal = i; break; }
        
        krt(listSongs[i].name);

        if (!fgets(listSongs[i].author, MAX_NAME, f)){ songTotal = i; break; }
        krt(listSongs[i].author);

    if (fscanf(f, "%d\n", &listSongs[i].year) != 1) { songTotal = i; break; }

        int lc = 0;
        if (fscanf(f, "%d\n", &lc) != 1 || lc < 0) { songTotal = i; break; }
        listSongs[i].lineCount = (lc > MAX_LINES) ? MAX_LINES : lc;

for (int j = 0; j < listSongs[i].lineCount; j++) {
            if (!fgets(listSongs[i].text[j], MAX_TEXT, f)) {
                
                listSongs[i].lineCount = j; break; }
            krt(listSongs[i].text[j]);
        }
        
        for (int j = listSongs[i].lineCount; j < lc; j++) {
            char dummy[MAX_TEXT];
            if (!fgets(dummy, MAX_TEXT, f)) break;
            
            
        }
    }
    fclose(f);
    if (songTotal > 0) nextId = listSongs[songTotal - 1].id + 1;
}

int main() {
    loadFile();

    while (true) {
        cout << "\n=== Каталог пісень  ===\n";
        cout << "1. Додати пісню\n";
        cout << "2. Список\n";
        cout << "3. Показати текст\n";
        cout << "4. Пошук по автору\n";
        cout << "5. Зберегти у файл\n";
        cout << "0. Вихід (автозбереження)\n> ";

        int c;
        if (!(cin >> c)) { clearInput(); cout << "Невірний вибір.\n"; continue; }

        if      (c == 1) addSong();
        else if (c == 2) showAll();
        else if (c == 3) showText();
        else if (c == 4) findAuthor();
        else if (c == 5) saveFile();
        else if (c == 0) { saveFile(); break; }
        else cout << "Невідомий пункт.\n";
    }
    return 0;
}
