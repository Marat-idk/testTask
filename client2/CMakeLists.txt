cmake_minimum_required(VERSION 3.10)

project(client2 VERSION 5.0.0)

# Переменная для одиковых названий экспортруемого таргетов
set(EXPORT_TARGETS_NAME ${PROJECT_NAME}Targets)

# Определяем стандарт С++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определяем суфикс для версии Debug
set(CMAKE_DEBUG_POSTFIX _g)

# Определяем флаги для линковки файлов для SO файлов
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pthread -Wl,--no-undefined")

# Определяем список исходных файлов
file(GLOB_RECURSE SOURCES src/*.cpp)

find_package(RdKafka)

# Определяем имя цели (проекта)
add_executable(${PROJECT_NAME} ${SOURCES})

#Постфикс для Debug
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")

# Подключаем библиотеки
target_link_libraries(${PROJECT_NAME} PUBLIC RdKafka::rdkafka++)

# Настройка инсталяции.
# Опция COMPONENT нужна для команды cmake --install build --component <component_name> (доступна из cmake 3.18)
install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION tests/${PROJECT_NAME} COMPONENT tests)