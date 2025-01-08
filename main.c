#include <ctype.h>
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

//funkcja do drukowania listy studentow
void printStudentList(struct Student *students, int size) {
   printf("Student list:\n");
   for (int i = 0; i < size; i++) {
      printf("%d. %s\n",students->indexNumber, students->surname);
      //operator -> sluzy do dostepu do skladowych struktury za posrednictwem wskaznika
      //jest skrotowym zapisem dereferencji i pobrania pola
      //rownoznaczy z: (*students).indexNumber
      students++; //dzieki temu ze do funkcji przekazujemy kopie do wskaznika
      //moge smialo iterowac w ten sposob nie psujac nic w oryginalnym wskazniku :>
   }
}

//funckaj do usuwania studenta pod danym indexem
void deleteStudent(struct Student **students, int *size, int index) {
   if (*size == 0) { // Sprawdzamy, czy tablica nie jest pusta
      printf("Student list is empty\n");
      return;
   }

   for (int i = index; i < *size - 1; i++) {//przesuwamy elementy w tablicy, aby usunąć dziurę
      (*students)[i] = (*students)[i + 1];
      (*students)[i].indexNumber = index;//wiem ze to sie moze mija z sensem w prawdziwym zyciu
      //ale index studenta jest u mnie powiazany z indexem w tablicy wiec go tez musze zaktualizowac :((
   }

   *size -= 1;
   *students = realloc(*students, (*size) * sizeof(struct Student));//realokujemy pamięć, aby zmniejszyć tablicę

   if (*students == NULL && *size > 0) {//sprawdzamy, czy realloc się nie powiódł
      perror("Realloc failed");
      exit(1);
   }

   printf("Student at index %d removed\n", index);
}

//funkcja do szukania studenta po nazwisku
void findStudentBySurname(struct Student **students, int *size, char surname[]) {
   if (*size == 0) { //sprawdzamy, czy lista studentów jest pusta
      printf("Student list is empty\n");
      return;
   }

   //zmieniamy całe nazwisko na wielkie litery
   for (int i = 0; surname[i] != '\0'; i++) {
      surname[i] = toupper(surname[i]);//zmieniamy wszystkie litery w surname
   }

   for (int i = 0; i < *size; i++) {
      char *studentSurname = students[i]->surname;
      //tworzymy kopię nazwiska studenta, zmieniając wszystkie litery na duże
      char studentSurnameUpper[strlen(studentSurname) + 1];//tworzymy nową tablicę dla przekształconego nazwiska
      for (int j = 0; studentSurname[j] != '\0'; j++) {
         studentSurnameUpper[j] = toupper(studentSurname[j]);//zmieniamy wszystkie litery na wielkie
      }
      studentSurnameUpper[strlen(studentSurname)] = '\0';//dodajemy znak końca łańcucha

      if (strcmp(studentSurnameUpper, surname) == 0) {
         printf("Student with this surname has been found!\n");
         printStudentData(*students, i);
         return;
      }
   }
   printf("Student with this surname has not been found :(\n");
}

//funkcja porownujaca
int compareStudentsBySurname(const void *a, const void *b) {
   struct Student *studentA = (struct Student *)a;
   struct Student *studentB = (struct Student *)b;

   // Porównujemy nazwiska za pomocą strcmp
   return strcmp(studentA->surname, studentB->surname);
}
void copyStudentArray(struct Student *destination, struct Student *source, int size) {
   for (int i = 0; i < size; i++) {
      destination[i] = source[i];
   }
}

void sortStudents(struct Student **students, int size) {
   struct Student *studentsTemp = (struct Student *)malloc(sizeof(struct Student) * size); //tworze tymczasowa tablice
   copyStudentArray(studentsTemp, *students, size); //ktora zostanie posrotowana zeby nie zaburzyc glownego porzadku
   qsort(studentsTemp, size, sizeof(struct Student), compareStudentsBySurname);//i uniknac mieszaniny z indexami
   printStudentList(studentsTemp, size);
   free(studentsTemp);
}

//funkcja do szukania najlepszej oceny
int getTheBiggestGrade(struct Subject subject) {
   int biggestGrade = 0;
   int i = 0;
   while(subject.grades[i] != 0) { //szukamy do momentu napotkania 0 co oznacza koniec ocen
      if(subject.grades[i] > biggestGrade) {
         biggestGrade = subject.grades[i];
      }
      i++;
   }
   return biggestGrade;
}

void printStudentWithTheBestGrade(struct Student *student, int size) {
   if(size < 1) { //sprawdzamy czy jest jest z kim porownywac
      if(size <= 0) {
         printf("Student list is empty\n");
         return;
      }
      printf("The best grades in each subject have %s bcs he is the only student", student->surname);
      return;
   }
   //Deklarujemy zmienne do przechowadsania indexow uczniow z najlepsza ocena
   int mathIndex = 0;
   int englishIndex = 0;
   int biologyIndex = 0;
   for (int i = 1; i < size; i++) { //iterujemy od jedynki poniweaz na poczatku zakladamy ze najlepsze oceny ma index 0
      //tutaj porownuje ze soba najwieksze oceny studentow pod danymi indexami
      //wiem ze moglbym to wcisnac w osoba funkcje, badz tez zmniejszyc ilosc porownan
      //ale czas nagli a ja coraz bardziej zmeczony
      if(getTheBiggestGrade(student[i].subjects[0]) > getTheBiggestGrade(student[mathIndex].subjects[0])) {
         mathIndex = i;
      }
      if(getTheBiggestGrade(student[i].subjects[1]) > getTheBiggestGrade(student[mathIndex].subjects[1])) {
         englishIndex = i;
      }
      if(getTheBiggestGrade(student[i].subjects[2]) > getTheBiggestGrade(student[mathIndex].subjects[2])) {
         biologyIndex = i;
      }
   }
   printf("Biggest grade from %s have %s\n","math",student[mathIndex].surname);
   printf("Biggest grade from %s have %s\n","english",student[englishIndex].surname);
   printf("Biggest grade from %s have %s\n","biology",student[biologyIndex].surname);
}

//funkcja liczaca srednia ocen
double getAverageGrade(struct Subject subject) {
   int sum = 0;
   int count = 0;
   while (subject.grades[count] != 0) { //0 oznacza koniec ocen
      sum += subject.grades[count];
      count++;
   }
   if (count == 0) return 0; //Zapobiega dzieleniu przez zero, jeśli brak ocen
   return (double)sum / count; //Zwracamy średnią
}

void printStudentWithTheBestAverage(struct Student *students, int size) {
   if (size < 1) { // Sprawdzamy, czy lista studentów jest pusta
      printf("Student list is empty\n");
      return;
   }
   //Deklarujemy zmienne do przechowywania indeksów studentów z najlepszą średnią
   int mathIndex = 0;
   int englishIndex = 0;
   int biologyIndex = 0;
   double bestMathAverage = getAverageGrade(students[0].subjects[0]);
   double bestEnglishAverage = getAverageGrade(students[0].subjects[1]);
   double bestBiologyAverage = getAverageGrade(students[0].subjects[2]);

   // Iteracja po wszystkich studentach
   for (int i = 1; i < size; i++) { // Zakładamy, że najlepszy student to na początku student o indeksie 0
      double mathAverage = getAverageGrade(students[i].subjects[0]); //korzystajac z funkcji zapisujemy srednie z kazdego przedmiotu
      double englishAverage = getAverageGrade(students[i].subjects[1]);
      double biologyAverage = getAverageGrade(students[i].subjects[2]);

      if (mathAverage > bestMathAverage) { //porownujemy srednie z aktualnie najlepsza srednia i w razie potrzeby
         bestMathAverage = mathAverage; //przypisujemy ja jako nowa nejlspza
         mathIndex = i;
      }
      if (englishAverage > bestEnglishAverage) {
         bestEnglishAverage = englishAverage;
         englishIndex = i;
      }
      if (biologyAverage > bestBiologyAverage) {
         bestBiologyAverage = biologyAverage;
         biologyIndex = i;
      }
   }
   printf("Student with the best average in Math: %s with average %.2f\n", students[mathIndex].surname, bestMathAverage);
   printf("Student with the best average in English: %s with average %.2f\n", students[englishIndex].surname, bestEnglishAverage);
   printf("Student with the best average in Biology: %s with average %.2f\n", students[biologyIndex].surname, bestBiologyAverage);
}

//funkcja ktora drukuje menu dla uzytkownika
void printMenu() {
   printf("\n");
   printf("1 - Add Student\n");
   printf("2 - Read student data\n");
   printf("3 - Print student list\n");
   printf("4 - Delete Student\n");
   printf("5 - Find Student by surname\n");
   printf("6 - Sort students by alfabeth\n");
   printf("7 - Print students with best grades from each subject\n");
   printf("8 - Print best average grades from each subject\n");
   printf("0 - Exit\n");
   printf("\n");
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
         case 2: {
            printf("Enter student index: "); //wyswietla dane o studencie po indexie
            int studentIndex;
            if (scanf("%d", &studentIndex) != 1 || studentIndex < 0 || studentIndex >= size) { //sprawdzamy czy index jest poprawny
               clearBuffer();
               printf("Invalid index.\n");
               break;
            }
            clearBuffer();
            printStudentData(students, studentIndex);
            break;
         }
         case 3:
            printStudentList(students, size);
            break;
         case 4: {
            printf("Enter student index which you want to delete:"); //wyswietla dane o studencie po indexie
            int studentIndex;
            if (scanf("%d", &studentIndex) != 1 || studentIndex < 0 || studentIndex >= size) { //sprawdzamy czy index jest poprawny
               clearBuffer();
               printf("Invalid index.\n");
               break;
            }
            clearBuffer();
            deleteStudent(&students, &size, studentIndex);
            break;
         }
         case 5: {
            printf("Insert student surname:");
            char surname[STRING_LENGTH];
            fgets(surname, STRING_LENGTH, stdin);
            surname[strcspn(surname, "\n")] = '\0';
            findStudentBySurname(&students, &size, surname);
            break;
         }
         case 6: {
            printf("Sorting students...");
            sortStudents(&students, size);
            break;
         }
         case 7: {
            printStudentWithTheBestGrade(students, size);
            break;
         }
         case 8: {
            printStudentWithTheBestAverage(students, size);
            break;
         }
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
