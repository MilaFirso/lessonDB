#include <iostream>
#include "CreateDB.h"

#pragma execution_character_set("utf-8")

using namespace std;

int main()
{
  // setlocale(LC_ALL, "rus");
  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);

  try
  {
    string connectionString =
      "host=localhost "
      "port=5432 "
      "dbname=postgres "
      "user=postgres "
      "password=Firsova2010";
    DataBase db(connectionString);

    db.createDatabaseTables();

    db.addClient("Mila", "Milk", "Mila@ma.r", "11892752589");
    db.show();


    db.addClient("Dim", "Tera", "Tee@mail.ru", "11892765589");
    db.show();

    
    try
    {
      db.addPhone("Mila", "89658756551");
      db.show();

      db.addPhone("Dim", "89658556541");
      db.show();
    }
    catch (const std::exception &ex)
    {
      SetConsoleCP(CP_UTF8);
      SetConsoleOutputCP(CP_UTF8);
      std::cout << "Exception happened: " << ex.what() << std::endl;
    }

    try
    {
      db.updateClient("Tee@mail.ru", "Ser", "Petrov", "Serg@mail.ru");
      db.show();

      
    }
    catch (const std::exception &ex)
    {
      SetConsoleCP(CP_UTF8);
      SetConsoleOutputCP(CP_UTF8);
      std::cout << "Exception happened: " << ex.what() << std::endl;
    }

    db.removePhone("Tee@mail.ru", "89658755541");

    db.removeClient("Di@ma.r");

    std::vector<Client> foundClients = db.findClients("Dim");
    for (const auto &client : foundClients)
    {
      std::cout << "Found client: " << client.name_ << " " << client.surname_
                << " " << client.email_ << " " << std::endl;
    }
  }
  catch (const std::exception &ex)
  {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "Exception happened: " << ex.what() << std::endl;
  }

  std::cout << std::endl << std::endl << "Goodbay, World";
};