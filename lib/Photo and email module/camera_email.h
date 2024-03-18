#ifndef camera_email_h
#define camera_email_h

#include "esp_camera.h"
#include "SPI.h"
#include "driver/rtc_io.h"
#include <FS.h>
#include <ESP_Mail_Client.h>

//Email beállítások


struct email_data{
    const char* sender ="";
    const char* password ="";
    const char* smtpServer = "smtp.gmail.com";
    const int serverport = 465;
    const char* subject ="";
    const char* recipient ="";
    const char* name = "User";
    const char* htmlMsg = "<h1>Example text</h1>";
};



//Kamera beállítások
#define FILE_PHOTO "photo.jpg"
#define FILE_PHOTO_PATH "/photo.jpg"

#define CAMERA_MODEL_AI_THINKER

#if defined(CAMERA_MODEL_AI_THINKER)
  #define PWDN_GPIO_NUM     32
  #define RESET_GPIO_NUM    -1
  #define XCLK_GPIO_NUM      0
  #define SIOD_GPIO_NUM     26
  #define SIOC_GPIO_NUM     27
  #define Y9_GPIO_NUM       35
  #define Y8_GPIO_NUM       34
  #define Y7_GPIO_NUM       39
  #define Y6_GPIO_NUM       36
  #define Y5_GPIO_NUM       21
  #define Y4_GPIO_NUM       19
  #define Y3_GPIO_NUM       18
  #define Y2_GPIO_NUM        5
  #define VSYNC_GPIO_NUM    25
  #define HREF_GPIO_NUM     23
  #define PCLK_GPIO_NUM     22
#else
  #error "Camera model not selected"
#endif




void initCamera(void);


void capturePhotoSaveLittleFS(bool flash);
void sendPhoto(email_data& email);
void smtpCallback(SMTP_Status status);
void captureAndSendPhoto(email_data& emaildata, bool flash);










#endif