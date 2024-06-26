
void HTTP_init(void) {
	HTTP.on("/configs.json", handle_ConfigJSON); // формирование configs.json страницы для передачи данных в web интерфейс
	// API для устройства
	HTTP.on("/setname", handle_Set_Ssdp);     // Установить имя SSDP устройства по запросу вида /ssdp?ssdp=proba
	HTTP.on("/ssid", handle_Set_Ssid);     // Установить имя и пароль роутера по запросу вида /ssid?ssid=home2&password=12345678
	HTTP.on("/ssidap", handle_Set_Ssidap); // Установить имя и пароль для точки доступа по запросу вида /ssidap?ssidAP=home1&passwordAP=8765439
	HTTP.on("/login", handle_Set_Login);
	HTTP.on("/restart", handle_Restart);   // Перезагрузка модуля по запросу вида /restart?device=ok
	HTTP.on("/div", handle_Set_Div);     // Установка делителя значения счетчика
	// Добавляем функцию Update для перезаписи прошивки по WiFi при 1М(256K SPIFFS) и выше
	httpUpdater.setup(&HTTP);
	// Запускаем HTTP сервер

	//here the list of headers to be recorded
	const char * headerkeys[] = {"User-Agent","Cookie"} ;
	size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
	//ask server to track these headers
	HTTP.collectHeaders(headerkeys, headerkeyssize );
	
	HTTP.begin();
}

void HTTP_loop(void) {
	HTTP.handleClient();
}

// Функции API-Set
// Установка SSDP имени по запросу вида http://192.168.0.101/ssdp?ssdp=proba
void handle_Set_Ssdp() {
	if(!HTTP.authenticate(_http_user.c_str(), _http_password.c_str())) return HTTP.requestAuthentication();
	ESP_Name = HTTP.arg("ESPName"); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
	saveConfig();                 // Функция сохранения данных во Flash
	HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}

// Установка параметров для подключения к внешней AP по запросу вида http://192.168.0.101/ssid?ssid=home2&password=12345678
void handle_Set_Ssid() {
	if(!HTTP.authenticate(_http_user.c_str(), _http_password.c_str())) return HTTP.requestAuthentication();
	_ssid = HTTP.arg("ssid");            // Получаем значение ssid из запроса сохраняем в глобальной переменной
	_password = HTTP.arg("password");    // Получаем значение password из запроса сохраняем в глобальной переменной
	saveConfig();                        // Функция сохранения данных во Flash
	HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

// Установка делителя значения счетчика
void handle_Set_Div() {
	if(!HTTP.authenticate(_http_user.c_str(), _http_password.c_str())) return HTTP.requestAuthentication();
	_Div1 = HTTP.arg("Div1").toInt();
	_Div2 = HTTP.arg("Div2").toInt();
	
	if (_Div1 == 0) _Div1 = 1;
	if (_Div2 == 0) _Div2 = 1;
	
	saveConfig();                 // Функция сохранения данных во Flash
	HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}

//Установка параметров внутренней точки доступа по запросу вида http://192.168.0.101/ssidap?ssidAP=home1&passwordAP=8765439
void handle_Set_Ssidap() {
	if(!HTTP.authenticate(_http_user.c_str(), _http_password.c_str())) return HTTP.requestAuthentication();
	_ssidAP = HTTP.arg("ssidAP");         // Получаем значение ssidAP из запроса сохраняем в глобальной переменной
	_passwordAP = HTTP.arg("passwordAP"); // Получаем значение passwordAP из запроса сохраняем в глобальной переменной
	saveConfig();                         // Функция сохранения данных во Flash
	HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

void handle_Set_Login() {
	if(!HTTP.authenticate(_http_user.c_str(), _http_password.c_str())) return HTTP.requestAuthentication();
	_http_user = HTTP.arg("http_user");         
	_http_password = HTTP.arg("http_password"); 
	saveConfig();                         // Функция сохранения данных во Flash
	HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

// Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
void handle_Restart() {
	if(!HTTP.authenticate(_http_user.c_str(), _http_password.c_str())) return HTTP.requestAuthentication();
	saveConfig();
	String restart = HTTP.arg("device");               // Получаем значение device из запроса
	if (restart == "ok") {                             // Если значение равно Ок
		HTTP.send(200, "text / plain", "Reset OK");    // Oтправляем ответ Reset OK
		ESP.restart();                                 // перезагружаем модуль
	} else {                                           // иначе
		HTTP.send(200, "text / plain", "No Reset");    // Oтправляем ответ No Reset
	}
}

void handle_ConfigJSON() {
	String root = "{}";  // Формировать строку для отправки в браузер json формат
	//{"SSDP":"SSDP-test","ssid":"home","password":"i12345678","ssidAP":"WiFi","passwordAP":"","ip":"192.168.0.101"}
	// Резервируем память для json обекта буфер может рости по мере необходимти, предпочтительно для ESP8266
  JsonDocument json;
// Deserialize the JSON document
  DeserializationError error = deserializeJson(json, root);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
  } 

	// Заполняем поля json
	json["ESPName"] = ESP_Name;
	json["ssidAP"] = _ssidAP;
	json["passwordAP"] = _passwordAP;
	json["ssid"] = _ssid;
	json["password"] = _password;
	json["timezone"] = timezone;
	json["ip"] = WiFi.localIP().toString();
	json["time"] = GetTime();
	json["date"] = GetDate();

	json["mqtt_host"] = _mqtt_host;
	json["mqtt_port"] = _mqtt_port;
	json["mqtt_user"] = _mqtt_user;
	json["mqtt_password"] = _mqtt_password;
	
	json["revision"] = _revision;
	
	json["Div1"] = _Div1;
	json["Div2"] = _Div2;

	json["http_user"] = _http_user;
	json["http_password"] = _http_password;
	
	json["SaveCount"] = SaveCount;
	json["ALERT"] = Alert > 0 ? "Да" : "Нет";
	json["AlertClass"] = Alert > 0 ? "danger" : "success";

	json["ColdWaterCounter"] = ColdWaterCount;
	json["HotWaterCounter"] = HotWaterCount;

	json["Uptime"] = uptime();

	json["Log"] = Log;
	
	// Помещаем созданный json в переменную root
	root = "";
	serializeJson(json,root);
//  serializeJson(json, Serial);
	HTTP.send(200, "text/json", root);
}
