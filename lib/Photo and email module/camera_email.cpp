#include "camera_email.h"

SMTPSession smtp;


void initCamera(void){   //Kamera inicializálása
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_LATEST;

  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 1;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

void capturePhotoSaveLittleFS( void ) {
  // Dispose first pictures because of bad quality
  camera_fb_t* fb = NULL;
  // Skip first 3 frames (increase/decrease number as needed).
  for (int i = 0; i < 3; i++) {
    fb = esp_camera_fb_get();
    esp_camera_fb_return(fb);
    fb = NULL;
  }
    
  // Take a new photo
  fb = NULL;  
  fb = esp_camera_fb_get();  
  if(!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
  }  

  // Photo file name
  Serial.printf("Picture file name: %s\n", FILE_PHOTO_PATH);
  File file = LittleFS.open(FILE_PHOTO_PATH, FILE_WRITE);

  // Insert the data in the photo file
  if (!file) {
    Serial.println("Failed to open file in writing mode");
  }
  else {
    file.write(fb->buf, fb->len); // payload (image), payload length
    Serial.print("The picture has been saved in ");
    Serial.print(FILE_PHOTO_PATH);
    Serial.print(" - Size: ");
    Serial.print(fb->len);
    Serial.println(" bytes");
  }
  // Close the file
  file.close();
  esp_camera_fb_return(fb);
}

void sendPhoto(email_data email) {
  
  /** Enable the debug via Serial port
   * none debug or 0
   * basic debug or 1
  */
  smtp.debug(1);

  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);

  /* Declare the session config data */
  Session_Config config;
  
  /*Set the NTP config time
  For times east of the Prime Meridian use 0-12
  For times west of the Prime Meridian add 12 to the offset.
  Ex. American/Denver GMT would be -6. 6 + 12 = 18
  See https://en.wikipedia.org/wiki/Time_zone for a list of the GMT/UTC timezone offsets
  */
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 0;
  config.time.day_light_offset = 1;

  /* Set the session config */
  config.server.host_name = email.smtpServer;
  config.server.port = email.serverport;
  config.login.email = email.sender;
  config.login.password = email.password;
  config.login.user_domain = "";

  Serial.println(email.sender);
  Serial.println(email.password);


  /* Declare the message class */
  SMTP_Message message;

  /* Enable the chunked data transfer with pipelining for large message if server supported */
  message.enable.chunking = true;

  /* Set the message headers */
  message.sender.name = "ESP32-CAM";
  message.sender.email = email.sender;


  message.subject = email.subject;
  message.addRecipient(email.name, email.recipient);

  message.html.content = email.htmlMsg;
  message.html.charSet = "utf-8";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_qp;

  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  /* The attachment data item */
  SMTP_Attachment att;

  /** Set the attachment info e.g. 
   * file name, MIME type, file path, file storage type,
   * transfer encoding and content encoding
  */
  att.descr.filename = FILE_PHOTO;
  att.descr.mime = "image/png"; 
  att.file.path = FILE_PHOTO_PATH;
  att.file.storage_type = esp_mail_file_storage_type_flash;
  att.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;

  /* Add attachment to the message */
  message.addAttachment(att);

  /* Connect to server with the session config */
  if (!smtp.connect(&config))
    return;

  

  /* Start sending the Email and close the session */
  if (!MailClient.sendMail(&smtp, &message, true)) Serial.print("Error sending Email, ");
  Serial.println(smtp.errorReason());
}

void smtpCallback(SMTP_Status status){
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success())
  {
    Serial.println("----------------");
    Serial.printf("Message sent success: %d\n", status.completedCount());
    Serial.printf("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++){
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");

  }
}

void captureAndSendPhoto(email_data emaildata){
  capturePhotoSaveLittleFS();
  sendPhoto(emaildata);
}

