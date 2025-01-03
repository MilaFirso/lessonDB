Домашнее задание к занятию «Работа с PostgreSQL из C++»

Выполнив это задание, вы сможете использовать библиотеку libpq++, чтобы подключаться к базе данных PostgreSQL из С++ и выполнять запросы.
Цель задания

    Научиться делать SELECT-запросы к базе данных PostgreSQL из С++, чтобы получать данные и отображать их в C++ приложении.
    Научиться делать INSERT-, UPDATE- и DELETE-запросы к базе данных PostgreSQL из С++ для манипуляции данными из C++ приложения.

Инструкция для выполнения домашнего задания
Прочитать инструкцию

Задание 1

Создайте программу для управления клиентами на C++.

Нужно хранить персональную информацию о клиентах:

    имя,
    фамилия,
    email,
    телефон.

Сложность в том, что телефон у клиента может быть не один, а два, три и даже больше. А может и не быть — например, если он не захотел его оставлять.

Вам нужно разработать структуру БД для хранения информации и написать класс на С++ для управления данными со следующими методами:

    Метод, создающий структуру БД (таблицы).
    Метод, позволяющий добавить нового клиента.
    Метод, позволяющий добавить телефон для существующего клиента.
    Метод, позволяющий изменить данные о клиенте.
    Метод, позволяющий удалить телефон у существующего клиента.
    Метод, позволяющий удалить существующего клиента.
    Метод, позволяющий найти клиента по его данным — имени, фамилии, email или телефону.

Эти методы обязательны, но это не значит, что должны быть только они. Можно создавать дополнительные методы и классы.

Также предоставьте код, демонстрирующий работу написанного вами класса.
