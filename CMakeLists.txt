cmake_minimum_required(VERSION 3.0)

option( INSTALL_QRIMAC_PLUGIN "Check to install qRIMAC plugin" OFF )

# CloudCompare 'RIMAC' plugin
if (INSTALL_QRIMAC_PLUGIN)
    project( QRIMAC_PLUGIN )

    set(CloudCompare_QCC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../qCC)

    include_directories( ${CloudCompare_QCC_DIR})
    
    include( ${CMAKE_CURRENT_SOURCE_DIR}/../../plugins/CMakePluginTpl.cmake)

    file( GLOB CC_PLUGIN_CUSTOM_HEADER_LIST ${CloudCompare_QCC_DIR}/ccItemSelectionDlg*.h)
    file( GLOB CC_PLUGIN_CUSTOM_SOURCE_LIST ${CloudCompare_QCC_DIR}/ccItemSelectionDlg*.cpp)
    set( CC_PLUGIN_CUSTOM_UI_LIST ${CloudCompare_QCC_DIR}/ui_templates/itemSelectionDlg.ui )


    file( GLOB CC_PLUGIN_CUSTOM_HEADER_LIST ${CloudCompare_QCC_DIR}/ccInterpolationDlg*.h )
    file( GLOB CC_PLUGIN_CUSTOM_SOURCE_LIST ${CloudCompare_QCC_DIR}/ccInterpolationDlg*.cpp )
    set( CC_PLUGIN_CUSTOM_UI_LIST ${CloudCompare_QCC_DIR}/ui_templates/interpolationDlg.ui )

  file( GLOB CC_PLUGIN_CUSTOM_HEADER_LIST ${CloudCompare_QCC_DIR}/ccPointListPickingDlg*.h )
    file( GLOB CC_PLUGIN_CUSTOM_SOURCE_LIST ${CloudCompare_QCC_DIR}/ccPointListPickingDlg*.cpp )
    set( CC_PLUGIN_CUSTOM_UI_LIST ${CloudCompare_QCC_DIR}/ui_templates/pointListPickingDlg.ui )

 file( GLOB CC_PLUGIN_CUSTOM_HEADER_LIST ${CloudCompare_QCC_DIR}/ccPointPropertiesDlg*.h )
    file( GLOB CC_PLUGIN_CUSTOM_SOURCE_LIST ${CloudCompare_QCC_DIR}/ccPointPropertiesDlg*.cpp )
    set( CC_PLUGIN_CUSTOM_UI_LIST ${CloudCompare_QCC_DIR}/ui_templates/pointPropertiesDlg.ui )

   
          
    #add IO support
    include_directories( ${QCC_IO_LIB_SOURCE_DIR} )
    target_link_libraries( ${PROJECT_NAME} QCC_IO_LIB )
  
    #add shapelib support
    #target_link_libraries( ${PROJECT_NAME} SHAPELIB )
   # include_directories( ${SHAPELIB_SOURCE_DIR} )
   # set_property( TARGET ${PROJECT_NAME} APPEND PROPERTY COMPILE_DEFINITIONS CC_SHP_SUPPORT )
endif()