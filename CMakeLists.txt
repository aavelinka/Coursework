cmake_minimum_required(VERSION 3.10)
project(Coursework)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core/models/include
    ${CMAKE_CURRENT_SOURCE_DIR}/core/exceptions/include
    ${CMAKE_CURRENT_SOURCE_DIR}/core/utils/include
    ${CMAKE_CURRENT_SOURCE_DIR}/core/repositories/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/core/services/include
)

set(MODEL_SOURCES
    core/models/include/user/BodyMeasurement.h
    core/models/include/user/Person.h
    core/models/include/user/User.h
    core/models/include/workout/Exercise.h
    core/models/include/workout/CardioTraining.h
    core/models/include/workout/StrengthTraining.h        
    core/models/include/workout/UpperBodyWorkout.h
    core/models/include/workout/LowerBodyWorkout.h
    core/models/src/user/BodyMeasurement.cpp
    core/models/src/user/Person.cpp
    core/models/src/user/User.cpp
    core/models/src/workout/Exercise.cpp
    core/models/src/workout/CardioTraining.cpp
    core/models/src/workout/StrengthTraining.cpp        
    core/models/src/workout/UpperBodyWorkout.cpp
    core/models/src/workout/LowerBodyWorkout.cpp
)

set(EXCEPTION_SOURCES
    core/exceptions/include/Exception.h 
    core/exceptions/include/FileException.h 
    core/exceptions/include/InputException.h
    core/exceptions/include/InputValidator.h 
    core/exceptions/src/InputValidator.cpp             
)

set(UTILS_SOURCES
    core/utils/include/Menu.h
    core/utils/src/Menu.cpp
)

set(REPOSITORY_SOURCES
    core/repositories/include/File.h
    core/repositories/include/TextFile.h
    core/repositories/include/TextFile.tpp
)

set(SERVICE_SOURCES
    core/services/include/BMICalculator.h
    core/services/include/WorkoutService.h
    core/services/include/UserService.h
    core/services/include/UndoManager.h
    core/services/src/BMICalculator.cpp
    core/services/src/WorkoutService.cpp
    core/services/src/UserService.cpp
    core/services/src/UndoManager.cpp
)

set(ALL_SOURCES
    src/main.cpp
    ${MODEL_SOURCES}
    ${EXCEPTION_SOURCES}
    ${UTILS_SOURCES}
    ${REPOSITORY_SOURCES}
    ${SERVICE_SOURCES}
)

set(CORE_SOURCES
    ${MODEL_SOURCES}
    ${EXCEPTION_SOURCES}
    ${UTILS_SOURCES}
    ${REPOSITORY_SOURCES}
    ${SERVICE_SOURCES}
)

add_library(coursework_core STATIC ${CORE_SOURCES})

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE coursework_core)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_executable(CourseworkGUI
    gui/gui_main.cpp
    gui/LoginWindow.cpp
    gui/DashboardWindow.cpp
    gui/RegistrationDialog.cpp
    gui/HistoryChartWidget.cpp
    gui/HistoryChartWidget.h
)

target_link_libraries(CourseworkGUI
    PRIVATE
        coursework_core
        Qt6::Widgets
)

set_target_properties(CourseworkGUI PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
