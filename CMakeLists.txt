cmake_minimum_required(VERSION 3.10)
project(Coursework)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/models/include
    ${CMAKE_CURRENT_SOURCE_DIR}/exceptions/include
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/include
    ${CMAKE_CURRENT_SOURCE_DIR}/repositories/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/services/include
)

set(MODEL_SOURCES
    models/include/user/BodyMeasurement.h
    models/include/user/Person.h
    models/include/user/User.h
    models/include/workout/Exercise.h
    models/include/workout/CardioTraining.h
    models/include/workout/StrengthTraining.h        
    models/include/workout/UpperBodyWorkout.h
    models/include/workout/LowerBodyWorkout.h
    models/src/user/BodyMeasurement.cpp
    models/src/user/Person.cpp
    models/src/user/User.cpp
    models/src/workout/Exercise.cpp
    models/src/workout/CardioTraining.cpp
    models/src/workout/StrengthTraining.cpp        
    models/src/workout/UpperBodyWorkout.cpp
    models/src/workout/LowerBodyWorkout.cpp
)

set(EXCEPTION_SOURCES
    exceptions/include/Exception.h 
    exceptions/include/FileException.h 
    exceptions/include/InputException.h
    exceptions/include/InputValidator.h 
    exceptions/src/InputValidator.cpp             
)

set(UTILS_SOURCES
    utils/include/Menu.h
    utils/src/Menu.cpp
)

set(REPOSITORY_SOURCES
    repositories/include/File.h
    repositories/include/TextFile.h
    repositories/include/TextFile.tpp
)

set(SERVICE_SOURCES
    services/include/BMICalculator.h
    services/include/WorkoutService.h
    services/include/UserService.h
    services/include/UndoManager.h
    services/src/BMICalculator.cpp
    services/src/WorkoutService.cpp
    services/src/UserService.cpp
    services/src/UndoManager.cpp
)

set(ALL_SOURCES
    main.cpp
    ${MODEL_SOURCES}
    ${EXCEPTION_SOURCES}
    ${UTILS_SOURCES}
    ${REPOSITORY_SOURCES}
    ${SERVICE_SOURCES}
)

add_executable(${PROJECT_NAME} ${ALL_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)