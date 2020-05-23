void get_icon()
{ 
  WiFiClient client;
  Serial.println("\nStarting connection to server...");
  if (!client.connect("twemoji.maxcdn.com", 80)) 
  {
    Serial.println("Connection failed!");
  } 
  else 
  {
    Serial.println("Connected to server!");
    client.println("GET http://twemoji.maxcdn.com/v/latest/72x72/1f607.png HTTP/1.0");
    client.println("Host: twemoji.maxcdn.com");
    client.println("Connection: close");
    client.println();
    int len = 0;
    while (client.connected()) 
    {
      String line = client.readStringUntil('\r');
      if (line == "\n") 
      {
        Serial.println("headers received");
        break;
      }
      Serial.print(line);
      int pos = line.indexOf("Content-Length: ");
      if(pos > -1)
      {
        pos += 16;
        len = line.substring(pos).toInt();
      }
    }
    Serial.println("Icon len = " + (String)len);
    byte icon[len + 200];
    int icon_len = 0;
    while (!client.available()) 
    {
      delay(100);
    }
    client.read();
    while (!client.available()) 
    {
      delay(100);
    }
    while (client.available()) 
    {
      icon[icon_len] = (byte)client.read();
      //Serial.println(String(icon[icon_len], HEX));
      icon_len ++;
      if(!client.available())
      {
        delay(1000);
      }
    }
    client.stop();
    
    Serial.println("Icon download complete. len = " + (String)icon_len);
    lcd.drawPng(icon, icon_len, 100, 50, 144, 144 ,0, 0, 2.0);
  }
}
