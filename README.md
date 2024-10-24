# client-server-app
Клиент-серверное приложение для передачи данных
C++, Qt 6, TCP/IP
## Структура репозитория

- **Client**: Ветка, содержащая код клиентского приложения.
- **Server**: Ветка, содержащая код серверного приложения.

# Клиент-серверное приложение

## Описание
Это клиент-серверное приложение, написанное на C++ с использованием Qt, которое позволяет пользователям обмениваться текстовыми сообщениями через TCP-соединение. 
Оно состоит из клиента и сервера, где клиент может подключаться к серверу и отправлять сообщения, которые затем отображаются в окне приложения.

## Функциональные возможности
- **Подключение к серверу**: Клиент может подключаться к серверу по указанному IP-адресу и порту (по умолчанию `127.0.0.1:2323`).
- **Отправка сообщений**: Пользователь может вводить текстовые сообщения в текстовое поле и отправлять их на сервер, нажав кнопку.
- **Получение сообщений**: Все сообщения, полученные от сервера, отображаются в текстовом браузере с временной меткой.
- **Обработка сообщений**: Приложение использует QDataStream для передачи и получения данных между клиентом и сервером, что позволяет передавать сообщения вместе с временными метками.
![image](https://github.com/user-attachments/assets/91c82ee9-ee7e-4b68-bc70-4010dc2d7306)
