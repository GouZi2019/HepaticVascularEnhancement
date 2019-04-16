#����cmake�汾��
cmake_minimum_required(VERSION 3.8.0)

#���ù�������
project(HepaticVascularPreproccess)

#���ù��̰�����ǰĿ¼���Ǳ���
set(CMAKE_INCLUDE_CURRENT_DIR ON) 

#�����Զ�����moc�ļ�����QT�Ļ�һ��Ҫ����
#set(CMAKE_AUTOMOC ON) 

#������Ҫ��Qt���ļ������ÿһ���ⶼҪд��QtҲ�����������ϵ�Զ�����
#find_package(Qt5Widgets)  
#find_package(Qt5Core)
#find_package(Qt5Gui)
#find_package(OpenCV REQUIRED)
find_package(ITK REQUIRED)

include(${ITK_USE_FILE})
#include_directories( ${OPENCV_INCLUDE_DIR} )


#���ҵ�ǰ�ļ����е�����Դ�����ļ���Ҳ����ͨ��Set��������ļ�����Ϊһ������
FILE(GLOB SRC_FILES "./*.cpp") 
#ͬ�������ҵ�ǰ�ļ��������е�ͷ�ļ�
FILE(GLOB HEAD_FILES "./*.h") 
#ͬ�������ҵ�ǰ�ļ��������е�ui�ļ�
#FILE(GLOB UI_FILES "./*.ui") 

#ͨ��Ui�ļ����ɶ�Ӧ��ͷ�ļ���һ��Ҫ����
#qt5_wrap_ui(WRAP_FILES ${UI_FILES})



#������Դ�ļ�
#set(RCC_FILES resources.qrc)

#��ui�ļ��������ļ�������һ���ļ����У��Ǳ���
#source_group("Ui" FILES ${UI_FILES} ${WRAP_FILES} )

#���������ļ�
#add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEAD_FILES} ${RCC_FILES} ${WRAP_FILES})
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEAD_FILES})

#����Qt5������
#target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} Qt5::Widgets Qt5::Core Qt5::Gui)
target_link_libraries(${PROJECT_NAME} ${ITK_LIBRARIES})