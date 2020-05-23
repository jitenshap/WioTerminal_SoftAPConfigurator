void setting_mode(wifi_settings_t* conf)
{
  while(!(conf -> saved))
  {
    WiFiClient client = webserver.available();   // listen for incoming clients
    if (client) 
    {
      String headers = "";
      String method = "";
      String path = "";
      Serial.println("New Client.");
      while (client.connected()) 
      {
        if(headers == "")
        {
          long timeout = 5000 + millis();
          while(!client.available() && (timeout - millis()) > 0) 
          {
            delay(100);
          }
          while(client.available() && (timeout - millis()) > 0) 
          {
            String line = client.readStringUntil('\r');
            if(line.indexOf("GET") == 0)
            {
              method = "GET";
              path = line.substring(4, line.indexOf(" ", 5));
            }
            if(line.indexOf("POST") == 0)
            {
              method = "POST";
              path = line.substring(5, line.indexOf(" ", 6));
            }
            headers += line;
            if(line == "\n")
            {
              break;
            }
          }
          if(headers == "")
          {
            break;
          }
          Serial.println("Headers received");
          Serial.println(headers);      
          Serial.println();
          Serial.println("Method = " + method + ", Path = " + path);
        }
        if(method == "POST")
        {
          long timeout = 5000 + millis();
          while(!client.available() && (timeout - millis()) > 0) 
          {
            Serial.print(".");
            delay(100);
          }
          if(path == "/setting")
          {
            Serial.println();
            String body = "";
            while(client.available() && (timeout - millis()) > 0)
            {
              body += client.readString();
            }
            if(body != "")
            {
              Serial.println("Body received");
              Serial.println(body);
              Serial.println();        
              String ssid = "";
              String pwd = "";
              if(body.indexOf("pwd=") > 0 && body.indexOf("ssid=") > 0)
              {
                if(body.indexOf("&pwd="))
                {
                  ssid = body.substring(body.indexOf("ssid=") + 5, body.indexOf("&"));
                  pwd = body.substring(body.indexOf("pwd=") + 4);            
                }
                else
                {
                  pwd = body.substring(body.indexOf("pwd=") + 4, body.indexOf("&"));
                  ssid = body.substring(body.indexOf("ssid=") + 5);
                }            
                Serial.println("SSID = " + ssid + ", PWD = " + pwd);
                strcpy(conf -> ssid, ssid.c_str());
                strcpy(conf -> password, pwd.c_str());
                conf -> saved = true;
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println();
                client.println("Saved");          
                break;
              }
            }
            else
            {
              client.println("HTTP/1.1 400 BAD REQUEST");
              client.println("Content-type:text/html");
              client.println();
              client.println("Bad request");
              break;
            }
          }
          else
          {
            client.println("HTTP/1.1 404 NOT FOUND");
            client.println("Content-type:text/html");
            client.println();
            client.println("Not Found");
            break;
          }
          break;        
        }
        else if(method == "GET")
        {
          if(path == "/")
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("<form action=\"/setting\" method=\"post\">\n");
            client.print("<div>\n");
            client.print("<label for=\"ssid\">SSID: </label>\n");
            client.print("<input name=\"ssid\" id=\"ssid\" value=\"\">\n");
            client.print("</div>\n");
            client.print("<div>\n");
            client.print("<label for=\"pwd\">Password: </label>\n");
            client.print("<input name=\"pwd\" id=\"pwd\" value=\"\">\n");
            client.print("</div>\n");
            client.print("<div>\n");
            client.print("<button>Save settings</button>\n");
            client.print("</div>\n");
            client.print("</form>");
            client.println();
            break;
          }
          else
          {
            client.println("HTTP/1.1 404 NOT FOUND");
            client.println("Content-type:text/html");
            client.println();
            client.print("Not Found\n");  
            break;                
          }
          break;
        }
      }
      client.stop();
      Serial.println("Client Disconnected.");
    }
  }  
}
