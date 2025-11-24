cmake_minimum_required(VERSION 3.10)
project(Coursework)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Include directories (Пути для поиска заголовочных файлов)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/models/include
    ${CMAKE_CURRENT_SOURCE_DIR}/exceptions/include
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/include
    ${CMAKE_CURRENT_SOURCE_DIR}/repositories/include # Путь для TextFile.h и TextFile.tpp
    ${CMAKE_CURRENT_SOURCE_DIR}/services/include
)

# 2. Model sources
set(MODEL_SOURCES
    models/src/user/BodyMeasurement.cpp
    models/src/user/Person.cpp
    models/src/user/User.cpp
    models/src/workout/Exercise.cpp
    models/src/workout/CardioTraining.cpp
    models/src/workout/StrenthTraining.cpp        # <--- ВАЖНО: Вы используете StrenthTraining.cpp (с опечаткой)
    models/src/workout/UpperBodyWorkout.cpp
    models/src/workout/LowerBodyWorkout.cpp
)

# 3. Exception sources
set(EXCEPTION_SOURCES
    exceptions/src/Exception.cpp
    exceptions/src/FileException.cpp
    exceptions/src/InputException.cpp
    exceptions/src/InputValidator.cpp             # <--- ИСПРАВЛЕНО: Этот файл находится в exceptions/src
)

# 4. Utils sources (Оставим пустым, так как InputValidator.cpp в exceptions/src)
set(UTILS_SOURCES
    # utils/src/InputValidator.cpp (Здесь пусто, так как он в exceptions/src)
)

# 5. Repository sources (Оставляем пустым, так как TextFile.tpp — это шаблон, а src пуст)
set(REPOSITORY_SOURCES
)

# 6. Service sources
set(SERVICE_SOURCES
    services/src/BMICalculator.cpp
    services/src/WorkoutService.cpp
    services/src/UserService.cpp
    services/src/UndoManager.cpp
)

# 7. All sources
set(ALL_SOURCES
    main.cpp
    ${MODEL_SOURCES}
    ${EXCEPTION_SOURCES}
    ${UTILS_SOURCES}
    ${REPOSITORY_SOURCES}
    ${SERVICE_SOURCES}
)

# Create executable
add_executable(${PROJECT_NAME} ${ALL_SOURCES})

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)