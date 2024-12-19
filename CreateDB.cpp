#include "CreateDB.h"

DataBase::DataBase(const std::string &connection) : connection_{connection}
{
  std::cout << "Connected to database successfully!" << std::endl;
};

void DataBase::createDatabaseTables() // Creating a database structure (tables)
{
    pqxx::work txn(connection_);
    txn.exec(
      R"(CREATE TABLE IF NOT EXISTS clients(id SERIAL PRIMARY KEY, name TEXT, surname TEXT, email TEXT UNIQUE);  
            CREATE TABLE IF NOT EXISTS phones(id SERIAL PRIMARY KEY, client_id INT REFERENCES clients(id), phone TEXT UNIQUE);)");
    txn.commit();
    std::cout << "A database structure (tables) created" << std::endl;
}
  
void DataBase::addClient(
  const std::string &name, const std::string &surname,
  const std::string &email,
  const std::string &phone) // Method for adding a new client
{
  
    pqxx::work txn(connection_);
    connection_.set_client_encoding("UTF8");
    pqxx::result res = txn.exec_params(
      "INSERT INTO clients (name, surname, email) VALUES ($1, $2, $3) "
      "RETURNING id",
      name, surname, email);
    if (!res.empty())
    {
      int client_id = res[0][0].as<int>();
      txn.exec_params(
        "INSERT INTO phones (client_id, phone) VALUES ($1, $2);", client_id,
        phone);
      txn.commit();
      std::cout << "Client added with ID: " << client_id << std::endl;
    }
    else
    {
      txn.abort();
      std::cerr << "Failed to add client" << std::endl;
    }
   }



void DataBase::addPhone(
  const std::string &name,
  const std::string &phone) // Adding a Phone for an Existing Customer
{
  pqxx::work txn(connection_);
  pqxx::result res =
    txn.exec_params("SELECT id, surname FROM clients WHERE name = $1", name);
  if (!res.empty())
  {
    int client_id = res[0][0].as<int>();
    txn.exec_params(
      "INSERT INTO phones (client_id, phone) VALUES ($1, $2)", client_id,
      phone);

    std::string client_surname = res[0][1].as<std::string>();
    txn.commit();
    std::cout << "Phone added for client: " << client_id << " " << name << " "
              << client_surname << std::endl;
  }
  else
  {
    txn.abort();
    std::cerr << "Client not found: " << name << std::endl;
  }
}

void DataBase::updateClient(
  const std::string &email, const std::string &newName,
  const std::string &newSurname,
  const std::string &newEmail) // Changing customer data
{
  pqxx::work txn(connection_);
  pqxx::result res =
    txn.exec_params("SELECT id FROM clients WHERE email = $1", email);
  if (!res.empty())
  {
    int client_id = res[0][0].as<int>();
    txn.exec_params(
      R"(UPDATE clients SET name = $1, surname = $2, email = $3 WHERE id = $4)",
      newName, newSurname, newEmail, client_id);
    txn.commit();
    std::cout << "Client updated: " << client_id << " " << newName << " "
              << newSurname << newEmail << std::endl;
  }
  else
  {
    txn.abort();
    std::cerr << "Client not found: " << newName << std::endl;
  }
}

void DataBase::removePhone(
  const std::string &email,
  const std::string &phone) // Deleting a phone for an existing client
{
  pqxx::work txn(connection_);
  pqxx::result res = txn.exec_params(
    "SELECT id, name, surname FROM clients WHERE email = $1", email);
  if (!res.empty())
  {
    int client_id = res[0][0].as<int>();
    txn.exec_params(
      "DELETE FROM phones WHERE client_id = $1 AND phone = $2", client_id,
      phone);
    txn.commit();
    std::string client_name = res[0][1].as<std::string>();
    std::string client_surname = res[0][2].as<std::string>();
    std::cout << "Phone removed for client: " << client_id << " "
              << client_name << " " << client_surname << std::endl;
  }
  else
  {
    txn.abort();
    std::cerr << "Client not found: " << email << std::endl;
  }
}

void DataBase::removeClient(
  const std::string &email) // Deleting an existing client
{
  pqxx::work txn(connection_);
  pqxx::result res = txn.exec_params(
    "SELECT id, name, surname FROM clients WHERE email = $1", email);
  if (!res.empty())
  {
    int client_id = res[0][0].as<int>();
    std::string client_name = res[0][1].as<std::string>();
    std::string client_surname = res[0][2].as<std::string>();
    txn.exec_params("DELETE FROM phones WHERE client_id = $1", client_id);
    txn.exec_params("DELETE FROM clients WHERE id = $1", client_id);
    txn.commit();
    std::cout << "Client removed: " << client_id << " " << client_name << " "
              << client_surname << std::endl;
  }
  else
  {
    txn.abort();
    std::cerr << "Client not found: " << email << std::endl;
  }
}

std::vector<Client> DataBase::findClients(const std::string &query)
{
  // Íàéòè êëèåíòà ïî åãî äàííûì (èìåíè, ôàìèëèè, email-ó èëè òåëåôîíó)
  std::vector<Client> result;
  pqxx::work txn(connection_);
  pqxx::result res = txn.exec_params(
    "SELECT * "
    "FROM clients "
    "LEFT JOIN public.phones on public.clients.id = public.phones.client_id "
    "WHERE clients.name = $1 OR clients.surname = $1 OR clients.email = $1 OR "
    "phones.phone = $1;",
    query);
  txn.commit();
  for (auto row : res)
  {
    Client client;
    client.name_ = row["name"].as<std::string>();
    client.surname_ = row["surname"].as<std::string>();
    client.email_ = row["email"].as<std::string>();
    result.push_back(client);
  }
  return result;
}


void DataBase::show()
{
  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);

  pqxx::work txn(connection_);

  // Ïîëó÷àåì ñïèñîê òàáëèö â áàçå äàííûõ
  pqxx::result tables = txn.exec(
    "SELECT table_name FROM information_schema.tables WHERE "
    "table_schema='public'");

  // Äëÿ êàæäîé òàáëèöû âûâîäèì âñå åå äàííûå
  for (auto row : tables)
  {
    std::string table_name = row[0].as<std::string>();

    // Âûïîëíÿåì SELECT-çàïðîñ äëÿ âûáîðêè äàííûõ èç òàáëèöû
    std::string query = "SELECT * FROM " + table_name;
    pqxx::result data = txn.exec(query);

    // Âûâîäèì äàííûå èç òàáëèöû â êîíñîëü
    std::cout << "Table: " << table_name << std::endl;
    for (auto &row : data)
    {
      for (auto &field : row) { std::cout << field.c_str() << "  "; }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
  txn.commit();
  std::cout << "Done" << std::endl;
};
