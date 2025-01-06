# Пример выполнения задания

## Шаг 1. Установка Docker и Docker Compose

### Установка Docker

Bash

sudo apt update
sudo apt install docker.io

### Установка Docker Compose

Bash

sudo apt install docker-compose

### Добавление текущего пользователя в группу Docker без прав суперпользователя:

Bash

sudo usermod -aG docker $USER
newgrp docker

### Проверка установки:

Bash

docker --version
docker-compose --version

Результат:

plaintext

max@max-Vivobook-ASUSLaptop-M6500QC-M6500QC:~$ docker --version  
Docker version 24.0.7, build 24.0.7-0ubuntu2~20.04.1  
max@max-Vivobook-ASUSLaptop-M6500QC-M6500QC:~$ docker-compose --version  
docker-compose version 1.25.0, build unknown


---

## Шаг 2. Создаем репозиторий

Создана папка для проекта под именем Linux5_DockerStart внутри репозитория на GitHub - Drone-Programming.

---

## Шаг 3. Разработка простой программы

Создан файл docker_ini.py на языке Python, который выводит сообщение и текущее время:

Python

import datetime

def main():
    print("Привет, это контейнер с Docker!")
    print("Текущее время:", datetime.datetime.now())

if __name__ == "__main__":
    main()


---

## Шаг 4. Перенос папки в терминал

Клонируем папку на локальную машину:

Bash

git clone https://github.com/Gibrin/Drone-Programming/tree/main/Linux5_DockerStart.git
cd Linux5_DockerStart


---

## Шаг 5. Создание Docker-образа

### Создание файла Dockerfile:

Bash

touch Dockerfile
nano Dockerfile


Содержимое Dockerfile:

dockerfile

FROM python:3.9-slim
WORKDIR /FILES
COPY docker_ini.py .
CMD ["python", "docker_ini.py"]


Собираем Docker-образ и запускаем контейнер:

Bash

docker build -t my_docker_ini .
docker run --rm my_docker_ini


---

## Шаг 6. Работа с Docker Compose

### Создание файла docker-compose.yml:

Bash

touch docker-compose.yml
nano docker-compose.yml


Содержимое docker-compose.yml:

YAML

version: '3.8'

services:
  app:
    image: my_docker_ini  # Используем предыдущий образ
    build:
      context: .
      dockerfile: Dockerfile
    container_name: my_docker_ini_container


### Запуск через Docker Compose:

Bash

docker-compose up --build


Обнаружена неактуальная версия Docker Compose. Решение:

Bash

# Удаление старой версии
sudo apt remove docker-compose

# Установка новой версии
Docker Compose version v2.32.1


Редактирование файла docker-compose.yml:

Bash

docker compose up --build


Выход программы:

plaintext

Привет, это контейнер с Docker!
Текущее время: <текущее время>


---

## Шаг 7. Оформление проекта на GitHub

### Перенос файлов в репозиторий:

Bash

git add .
git commit -m "Initial commit"
git push origin main


Решение проблем с доступом и несовпадением версий через ребейз.

---

## Шаг 8. Создание файла README.md

Создано данное описание, соответствующее требованиям разметки Markdown.
Загрузил его обратно на локальный компьютер.
git pull origin main
---

## Справочная информация

Ссылка на решение: https://github.com/Gibrin/Drone-Programming/

Репозиторий содержит:
1. docker_ini.py
2. Dockerfile
3. docker-compose.yml
4. README.md
