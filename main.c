#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STRING_LENGTH 100 //deklaruje dwie stale do podmianki dla preprocesora
#define GRADES_SIZE 10 //obie to wielkosc tablic

struct Subject { //tworze "foremki" na obiekty typu przedmiot
   char name[STRING_LENGTH]; //przechowujace dwie wartosci, nazwe przedmiotu i oceny ucznia
   int grades[GRADES_SIZE];
};

struct Student { //tworze rowniez strukture typu student z czterema polami
   char surname[STRING_LENGTH]; //nazwisko
   int birthDate[3]; //tablice reprezentujaca daty, moglem uzyc gotowej struktury tego typu lub tez wykorzystac do tego
   //stringi ale chcialem zrobic cos swojego i nie chcialo mi sie babrac z dwuwymiarowymi tablicami w wypadku stringow
   int indexNumber; //index ucznia
   struct Subject subjects[3]; //przedmioty ucznia
};

void clearBuffer() { //funkcja do czyzczenia zagubionych znakow nowej lini
    int c; //oczywiscie nieautorska
    while ((c = getchar()) != '\n' && c != EOF);
}

void copyArray(int *source, int *destination, int n) { //funkcja do kopiowania wartosci z jednej tablicy do drugiej
   for (int i = 0; i < n; i++) { //stworzylem ja zeby przenosic oceny
      destination[i] = source[i];
   }
}

//funkcja sluzaca do tworzenia studentow z przekazanych argumentow
//do tego wskaznik na wskaznik tablicy typu student zeby moc zaalokowac ponownie miejsce w pamieci
void addStudent(struct Student **students, int *size, char surname[], int birthDay, int birthMonth, int birthYear, int grades[3][10]) {
   (*size)++; //powiekszamy wielkosc
   struct Student *temp = realloc(*students, (*size) * sizeof(struct Student)); //zajmujemy nowe miejsce
   if (temp == NULL) { //sprawdzamy czy udalo sie przydzielic pamiecw
      printf("Out of memory\n");
      exit(1);
   }
   *students = temp; //jesli sie udalo to przypisujemy do naszego wskaznika nowy wskaznik z zajeta pamiecia

   struct Student *newStudent = &(*students)[*size - 1]; //tutaj pobieramy studenta z tablicy (wskaznik na studenta
   //pobieramy wyluskujac go z tablicy przekazanej jako argument)
   strcpy(newStudent->surname, surname); //wklepujemy nowe dane dla studenta recznie badz tez korzystajac z funkcji
   newStudent->birthDate[0] = birthDay;
   newStudent->birthDate[1] = birthMonth;
   newStudent->birthDate[2] = birthYear;
   newStudent->indexNumber = *size - 1;
   strcpy(newStudent->subjects[0].name, "Matematyka"); //ustawiamy tutaj recznie nazwy przedmiotow zeby kazdy
   strcpy(newStudent->subjects[1].name, "Jezyk angielski"); //uczen mial takie same
   strcpy(newStudent->subjects[2].name, "Biologia");
   copyArray(grades[0], newStudent->subjects[0].grades, GRADES_SIZE);
   copyArray(grades[1], newStudent->subjects[1].grades, GRADES_SIZE);
   copyArray(grades[2], newStudent->subjects[2].grades, GRADES_SIZE);
}

//funkcja sluzaca do pobierania ocen od uzytkownika
void insertGrades(int *grades) {
   for (int i = 0; i < GRADES_SIZE; i++) { //maksymalnie 10 ocen
      int grade;
      printf("Enter grade (0 to stop):"); //jesli przekazana liczba bedzie rowna 0 badz tez nie bedzie liczba calkowita pobieranie ocen
      if (scanf("%d", &grade) != 1 || grade == 0) { //zostaje przerwane
         clearBuffer();
         break;
      }
      if (grade < 1 || grade > 6) { //jesli ocena wychodzi z zakresu to cofamy zmienna i o 1 i pobieramy dane jeszcze raz
         printf("Invalid grade\n");
         i--;
         clearBuffer();
         continue;
      }
      grades[i] = grade; //zapisujemy nowa oceny pod odpowiednim indexem tablicy
   }
   for (int j = 0; j < GRADES_SIZE; j++) { //dla reszty ocen (po przerwaniu) ustawiamy wartosc 0
      if (grades[j] < 1 || grades[j] > 6) {
         grades[j] = 0;
      }
   }
}

//funkcja pobierajaca dane od uzytkownika i doceolowo tworzaca kopie studenta korzystajac z wyzej zadeklarowanej funkcji
void addStudentHandler(struct Student **students, int *size) {
   char surname[STRING_LENGTH]; //deklaracja zmiennych do ktorych bedziemy pobierac dane
   int birthDate[3];
   int grades[3][GRADES_SIZE];

   printf("Insert surname: ");
   fgets(surname, STRING_LENGTH, stdin);
   surname[strcspn(surname, "\n")] = '\0'; //usuwamy znak nowej linii

   printf("Insert birth date:\n"); //tutaj kolejna pobieramy dzien, miesiac i rok urodzen (niestety 0 na poczatku beda ignorowane
   printf("    day:"); //przez fakt ze to zmienne calkowite wiec miesiac np 09 bedzie wygladal 9 :((
   scanf("%d", &birthDate[0]);
   printf("    month:");
   scanf("%d", &birthDate[1]);
   printf("    year:");
   scanf("%d", &birthDate[2]);
   clearBuffer(); //usuwamy znak nowej lini

   printf("Insert grades for subject - Matematyka:\n"); //tutaj po kolei pobieramy oceny z danych przedmiotow
   insertGrades(grades[0]);
   printf("Insert grades for subject - Jezyk angielski:\n");
   insertGrades(grades[1]);
   printf("Insert grades for subject - Biologia:\n");
   insertGrades(grades[2]);
   //i na koniec wywolujemy funkcje tworzaca studenta
   addStudent(students, size, surname, birthDate[0], birthDate[1], birthDate[2], grades);
}

//funckaj do wyswietlania ocen, zwyczajna funkcja ktora iteruje po kolejnych elementach tablicy
void printGrades(int grades[], char subjectName[]) {
   printf("    %s:\n", subjectName);
   printf("       ");
   for (int i = 0; i < GRADES_SIZE; i++) {
      if (grades[i] == 0) {
         break;
      }
      printf("%d, ", grades[i]);
   }
   printf("\n");
}

//funkcja do wyswietlanai wszystkich danych o studencie
void printStudentData(struct Student *students, int index) {
   printf("Surname: %s\n", students[index].surname);
   printf("Index: %d\n", students[index].indexNumber);
   printf("Birth date: %d-%d-%d\n", students[index].birthDate[0], students[index].birthDate[1], students[index].birthDate[2]);
   printf("Subjects:\n");
   for (int i = 0; i < 3; i++) {
      printGrades(students[index].subjects[i].grades, students[index].subjects[i].name);
   }
}

//funkcja ktora drukuje menu dla uzytkownika
void printMenu() {
   printf("1 - Add Student\n");
   printf("2 - Read student data\n");
   printf("0 - Exit\n");
}

//funkcja gdzie wszystko sie dzieje
int main(void) {
   struct Student *students = NULL; //deklarujemy tablice studentow
   int size = 0; //deklarujemy rozmiar tablicy ktory przekazujemy do funckji
   bool work = true; //flaga dla petli while

   while (work) {
      printMenu(); //po kazdej iteracji drukujemy menu dla uzytkownika
      int option;
      printf("Enter option: ");
      if (scanf("%d", &option) != 1) { //sprawdzamy czy uzytkownik wporadzil zmienna calkowita i w razie czego go poprawiamy
         clearBuffer();
         printf("Invalid input. Try again.\n");
         continue;
      }
      clearBuffer();

      switch (option) { //petla switch wykonujaca odpowiednie do menu operacje
         case 1:
            addStudentHandler(&students, &size); //dodawanie studenta
            break;
         case 2:
            printf("Enter student index: "); //wyswietla danych o studencie po indexie
            int studentIndex;
            if (scanf("%d", &studentIndex) != 1 || studentIndex < 0 || studentIndex >= size) {
               clearBuffer();
               printf("Invalid index.\n");
               break;
            }
            clearBuffer();
            printStudentData(students, studentIndex);
            break;
         case 0: //koniec pracy programu
            work = false;
            break;
         default:
            printf("Invalid option.\n");
      }
   }

   free(students); //zwalnianie miejsca
   return 0;
}
