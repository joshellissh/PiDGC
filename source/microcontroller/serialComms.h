void handleComms(SDCard &sdCard, Values &values) {
  
  // Check for commands from Pi
  char *serialMessage = getSerialMessage();
  
  if (serialMessage != NULL) {
    // Pi requesting odometer values  
    if (strcmp(serialMessage, "so") == 0) {
      float tripOdometer = sdCard.readFloat(TRIP_FILE, 0.0f);
      float odometer = sdCard.readFloat(ODOMETER_FILE, 0.0f);
      
      char output[512] = {0};
      sprintf(output, "odo:%f,%f", tripOdometer, odometer);
      Serial.println(output);
      
      sprintf(
        output, 
        "log:Odometer values requested. Returning (trip %f / odometer %f)",
        tripOdometer,
        odometer
      );
      Serial.println(output);
    }

    // Pi requesting config
    else if (strcmp(serialMessage, "sconfig") == 0) {
      int ppm = sdCard.readInt(PPM_FILE, 10500);
      bool blinkerSound = sdCard.readBool(BLINKER_FILE, true);
      bool chimeSound = sdCard.readBool(CHIME_FILE, true);
      int screenDimming = sdCard.readInt(DIMMING_FILE, 20);
    
      char output[512] = {0};
      sprintf(output, "config:%d,%d,%d,%d", ppm, blinkerSound, chimeSound, screenDimming);
      Serial.println(output);
      
      sprintf(
        output, 
        "log:Config value requested. Returning PPM %d, Blinker Sound %d, Chime Sound %d, Screen Dimming %d",
        ppm,
        blinkerSound,
        chimeSound,
        screenDimming
      );
      Serial.println(output);
    }

    // Pi writing odometer values
    else if(strstr(serialMessage, "wo:") != NULL) {
      char *stringValues = serialMessage + 3;

      // Write trip odometer
      char *token = strtok(stringValues, ",");
      values.tripOdometer = atof(token);
      sdCard.writeValue(TRIP_FILE, values.tripOdometer);

      // Write mileage odometer
      token = strtok(NULL, ",");
      values.odometer = atof(token);
      sdCard.writeValue(ODOMETER_FILE, values.odometer);

      char output[512] = {0};
      sprintf(
        output, 
        "log:Wrote mileage (trip %f, odo %f) to SD card.",
        values.tripOdometer,
        values.odometer
      );
      Serial.println(output);
    }

    // Pi resetting trip odometer
    else if(strstr(serialMessage, "trip") != NULL) {
      // Write trip odometer
      sdCard.writeValue(TRIP_FILE, 0.0f);
      Serial.println("log:Reset trip odometer.");
    }

     // Pi writing config
    else if(strstr(serialMessage, "wconfig:") != NULL) {
      char *values = serialMessage + strlen("wconfig:");
      int ppm = 8000;
      bool blinkerSound = true;
      bool chimeSound = true;
      int screenDimming = 20;

      char* value = strtok(values, ",");
      int part = 0;
      while (value != 0) {        
        if (part == 0)
          ppm = atoi(value);
        else if (part == 1)
          blinkerSound = atoi(value);
        else if (part == 2)
          chimeSound = atoi(value);
        else if (part == 3)
          screenDimming = atoi(value);
        
        value = strtok(0, ",");
        part++;
      }
      
      sdCard.writeValue(PPM_FILE, ppm);
      sdCard.writeValue(BLINKER_FILE, blinkerSound);
      sdCard.writeValue(CHIME_FILE, chimeSound);
      sdCard.writeValue(DIMMING_FILE, screenDimming);

      char output[512] = {0};
      sprintf(
        output, 
        "log:Wrote config values (PPM %d, Blinker Sound %d, Chime Sound %d, Screen Dimming %d) to SD card.",
        ppm,
        blinkerSound,
        chimeSound,
        screenDimming
      );
      Serial.println(output);
    }
  }
}
