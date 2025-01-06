#Используем официальный образ Python облегченной версии `slim`:
  FROM python:3.9-slim

#Устанавливаю рабочую директорию:
  WORKDIR /FILES

#Копирую файлы проекта из текущей директории (Linux5_DockerStart) в рабочую директорию контейнера (FILES):
  COPY docker_ini.py .

#Указываю команду по умолчанию:
  CMD ["python", "docker_ini.py"]
  
