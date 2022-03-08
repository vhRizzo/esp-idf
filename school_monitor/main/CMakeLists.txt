idf_component_register(SRCS "main.cpp" 
                            "misc/global_data.cpp"
                            "misc/wifi_setup.c" 
                            "misc/mqtt_setup.c"
                            "inmp441/inmp441_setup.cpp" 
                            "bme280/bme280_setup.cpp" 
                            "bme280/bme280.c" 
                            "dsm501a/dsm501a_setup.cpp"
                    INCLUDE_DIRS "bme280" "dsm501a" "inmp441" "misc" ".")
