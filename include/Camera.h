/**!
 * @file Camera.h 
 * @author Aleksandar Dikov (528052)
 * @version 1.0
 * @date 2025-06-04
 * @details This class holds all the functions for working with the Camera of the Elegoo smart car 
 * @copyright GNU Public Licence
 */
#pragma once

const char * WIFI_SSID = " Bruh"; 
const char * WIFI_PSWD = "12345677";  //TODO 

class Camera{

    public: 
    void Camera_Init(); 
    /**
     * @brief 
     * Check the traffic light to see if it's green or red
     * 
     * @return true 
     * The 
     * @return false 
     */
    bool Check_Light(); 



    private: 
}; 