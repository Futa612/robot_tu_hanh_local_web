//⠀⠀⠀⠀⣠⣶⡾⠏⠉⠙⠳⢦⡀⠀⠀⠀⢠⠞⠉⠙⠲⡀⠀
//⠀⠀⠀⣴⠿⠏⠀⠀⠀⠀ ⠀⢳⡀⠀ ⡏⠀⠀⠀⠀⠀ ⢷            
//⠀⠀⢠⣟⣋⡀⢀⣀⣀⡀⠀⣀⡀⣧⠀⢸⠀MY⠀   ⡇            Dat Nguyen
//⠀⠀⢸⣯⡭⠁⠸⣛⣟⠆⡴⣻⡲⣿⠀⣸⠀⠀  ⠀ ⡇            
//⠀⠀⣟⣿⡭⠀⠀⠀⠀⠀⢱⠀ ⣿⠀⢹⠀ SAUCE ⡇            Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.       
//⠀⠀⠙⢿⣯⠄⠀⠀⠀⢀⡀⠀⠀⡿⠀⠀⡇⠀⠀⠀⠀ ⡼             The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//⠀⠀⠀⠀⠹⣶⠆⠀⠀⠀⠀⠀⡴⠃⠀⠀⠘⠤⣄⣠⠞⠀
//⠀⠀⠀⠀⠀⢸⣷⡦⢤⡤⢤⣞⣁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀             If you want to know more about project details, please contact me via: nguyenphudat.haui@gmail.com
//⠀⠀⢀⣤⣴⣿⣏⠁⠀⠀⠸⣏⢯⣷⣖⣦⡀⠀⠀⠀⠀⠀⠀            or facebook: https://www.facebook.com/npd6120
//⢀⣾⣽⣿⣿⣿⣿⠛⢲⣶⣾⢉⡷⣿⣿⠵⣿⠀⠀⠀⠀⠀⠀           
//⣼⣿⠍⠉⣿⡭⠉⠙⢺⣇⣼⡏⠀⠀⠀⣄⢸⠀⠀⠀⠀⠀⠀            
//⣿⣿⣧⣀⣿………⣀⣰⣏⣘⣆⣀⠀⠀⠀                   

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
//#include <DHT.h>
#include <SoftwareSerial.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h> 

String s,s1;
long data=0, datas=0;
const long interval = 1000;
unsigned long previousMillis = 0;    // will store last time ENCODER was updated
int pinValue2;
int pinValue3;
String s3;
const char tach[5] = " ,-";
String speed1;
String speed2;

SoftwareSerial mySerial(D1,D2); //RX, TX
// Set to true to define Relay as Normally Open (NO)
#define RELAY_NO    true

// Set number of relays
#define NUM_RELAYS  2

// Assign each GPIO to a relay
int relayGPIOs[NUM_RELAYS] = {2,14};
const char* PARAM_INPUT_1 = "relay";  
const char* PARAM_INPUT_2 = "state";

// Replace with your network credentials
const char* ssid = "hello";
const char* password = "123456789";

AsyncWebServer server(80);
const char index_html[] PROGMEM = R"rawliteral(


<!DOCTYPE html>
<html>
    <head>
        <script src="https://code.highcharts.com/highcharts.js"></script>
        <meta charset="UTF-8">
        <style>
            @import url('https://fonts.googleapis.com/css?family=Poppins:200,300,400,500,600,700,800,900&display=swap');
            *{
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: 'Poppins', sans-serif;
            }
            header{
            position: absolute;
            top: 0;
            left: 0;
            width: 100%;
            padding: 40px 100px;
            z-index: 1000;
            display: flex;
            justify-content: space-between;
            align-items: center;
            }
            header .logo{
            color: #fff;
            text-transform: uppercase;
            cursor: pointer;
            }
            header .logo a{
            text-decoration: none;
            color: #fff;
            text-transform: uppercase;
            cursor: pointer;
            transition: 0.2s;
            }
            .logo :hover{
            letter-spacing: 5px;
            }
            .toggle{
            position: relative;
            width: 60px;
            height: 60px;
            background: url(https://i.ibb.co/HrfVRcx/menu.png);
            background-repeat: no-repeat;
            background-size: 30px;
            background-position: center;
            cursor: pointer;
            }
            .toggle.active{
            background: url(https://i.ibb.co/rt3HybH/close.png);
            background-repeat: no-repeat;
            background-size: 25px;
            background-position: center;
            cursor: pointer;
            z-index: 100;
            }
            .showcase{
            position: absolute;
            right: 0;
            width: 100%;
            min-height: 100vh;
            padding: 100px;
            display: flex;
            justify-content: space-between;
            align-items: center;
            background: #111;
            transition: 0.4s;
            z-index: 2;
            }
            .showcase.active{
            right: 80%;
            }

            .showcase video{
            position: absolute;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            object-fit: cover;
            opacity: 0.8;
            }
            .backgroundimg{
            position: absolute;
            top: 0;
            left: 0;
            object-fit: cover;
            opacity: 0.5;
            width: 100%;
            height: 100%;
            z-index: 0;
            }
            
            .text{
            position: relative;
            z-index: 10;
            }

            .text h2{
            font-size: 5em;
            font-weight: 800;
            color: #fff;
            line-height: 1em;
            text-transform: uppercase;
            }
            .text h3{
            font-size: 4em;
            font-weight: 700;
            color: #fff;
            line-height: 1em;
            text-transform: uppercase;
            }
            .text p{
            font-size: 1.1em;
            color: #fff;
            margin: 20px 0;
            font-weight: 400;
            max-width: 700px;
            }
            .text a{
            display: inline-block;
            font-size: 1em;
            background: #fff;
            padding: 10px 30px;
            text-transform: uppercase;
            text-decoration: none;
            font-weight: 500;
            margin-top: 10px;
            color: #111;
            letter-spacing: 2px;
            transition: 0.2s;
            }
            .text a:hover{
            letter-spacing: 6px;
            }
            .text button{
            display: inline-block;
            font-size: 1em;
            background: #fff;
            padding: 10px 30px;
            text-transform: uppercase;
            text-decoration: none;
            font-weight: 500;
            margin-top: 10px;
            color: #111;
            letter-spacing: 2px;
            transition: 0.2s;
            }
            .text button:hover{
            letter-spacing: 6px;
            }
            .social{
            position: absolute;
            z-index: 100;
            bottom: 20px;
            display: flex;
            justify-content: center;
            align-items: center;
            }
            .social li{
            list-style: none;
            }
            .social li a{
            display: inline-block;
            margin-right: 20px;
            filter: invert(1);
            transform: scale(0.5);
            transition: 0.5s;
            }
            .social li a:hover{
            transform: scale(0.5) translateY(-15px);
            }
            .menu{
            position: absolute;
            top: 0;
            right: 0;
            bottom: 0;
            width: 80%;
            height: 100%;
            background: rgb(255, 255, 255);
            z-index: 0;
            }
            .menu h3{
                position: absolute;
                top: 20px;
                left: 20px;
                align-items: center;
                justify-content: space-between;
                z-index: 1;
            }
            .menu h3 li{
                list-style: none;
                margin-top: 5%;
                
            }
            .menu ul{
            position: absolute;
            top: 20px;
            right: 20px;
            max-height: 40%;
            }
            .menu chart1{
            position: absolute;
            bottom: 2%;
            display: flex;
            left: 0;    
            width: 50%;
            height: 60%;
            justify-content: space-between;
            z-index: 1;
            }
            .menu chart2{
            position: absolute;
            bottom: 2%;
            display: flex;
            right: 0;   
            width: 50%;
            height: 60%;
            justify-content: space-between;
            z-index: 1;
            }
            .menu ul li{
            list-style: none;
            }
            .menu ul li a{
            text-decoration: none;
            font-size: 24px;
            color: #111;
            }
            .menu ul li a:hover{
            color: #03a9f4; 
            }

            @media (max-width: 991px){
            .showcase,
            .showcase header{
                padding: 40px;
            }
            .text h2{
                font-size: 3em;
            }
            .text h3{
                font-size: 2em;
            }
            }
            input:checked+.slider {background-color: #059e8a}
            input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
            input[type=text]{
                width: 60%;
                border: 2px rgb(187, 187, 187)3);
                border-radius: 4px;
                
                outline: none;
                padding: 9px;
                box-sizing: border-box;
                transition: 0.3s;
                right: 0;
                ;
                }
                input[type=text]:focus{
                    border-color: dodgerblue;
                    box-shadow: 0 0 8px 0 darkcyan;
                }
        </style>
    </head>
    <body>
        <!-- Video Source -->
  <!-- https://www.pexels.com/video/aerial-view-of-beautiful-resort-2169880/ -->
  <section class="showcase">
    <header>
      <h2 class="logo"><a href="https://www.haui.edu.vn/vn">HAUI</a></h2>
      <div class="toggle"></div>  
    </header>
    <!-- <video src="https://traversymedia.com/downloads/videos/explore.mp4" muted loop autoplay></video> -->
    <img class="backgroundimg" src="https://images.pexels.com/photos/262325/pexels-photo-262325.jpeg?auto=compress&cs=tinysrgb&dpr=2&h=650&w=940">
    
    <div class="text">
      <h2>Never Stop</h2> 
      <h3>Exploring The World</h3>
      <p>
        Tra cứu thêm tập lệnh của MWR robot tại mục Command List
      </p>
      <input id="sp" name="setpoint" type="text" placeholder="Serial command..."></br>
      <button id="Set" onclick="SET(sp)">Set command</button>
      
      <!-- <a href="#">Explore</a> -->
    </div>
    <ul class="social">
      <li><a href="https://www.facebook.com/npd6120/"><img src="https://i.ibb.co/8061Y2Y/facebook.png"></a></li>
      <li><a href="#"><img src="https://i.ibb.co/Wnxq2Nq/twitter.png"></a></li>
      <li><a href="https://www.instagram.com/l.12.15.14.7/"><img src="https://i.ibb.co/ySwtH4B/instagram.png"></a></li>
      <li><a href="https://discord.gg/kC7xmvK2"><img src="https://i.ibb.co/SdJLDch/discord2.png"></a></li>
    </ul>
  </section>
  
  <div class="menu">
    
    <h3>
        <li>
          <span id="speed1" style="color: rgb(62, 62, 67);">%SPEED1%</span>
          <sup class="units" style="color: rgb(62, 62, 67);">rpm</sup>
        </li>
        <li>
          <span id="speed2" style="color: rgb(123, 180, 236);">%SPEED2%</span>
          <sup class="units" style="color: rgb(123, 180, 236);">rpm</sup>
        </li>
    </h3>
    <ul>
      <li><a href="https://www.haui.edu.vn/vn">Home</a></li>
      <li><a href="https://tinhte.vn/">News</a></li>
      <li><a href="#">Command list</a></li>
      <li><a href="https://linustechtips.com/">Blog</a></li>
      <li><a href="https://www.facebook.com/npd6120/">Contact</a></li>
    </ul>

    <chart1><div id="chart-speed1" class="container"></div></chart1>
    <chart2><div id="chart-speed2" class="container"></div></chart2>
  </div>

    </body>
    <script>
      const menuToggle = document.querySelector('.toggle');
      const showcase = document.querySelector('.showcase');
  
      menuToggle.addEventListener('click', () => {
      menuToggle.classList.toggle('active');
      showcase.classList.toggle('active');
        })
        </script>
    <script>
      setInterval(function ( ) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("speed1").innerHTML = this.responseText;
          }
        };
        xhttp.open("GET", "/speed1", true);
        xhttp.send();
      }, 3000 );
      
      setInterval(function ( ) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("speed2").innerHTML = this.responseText;
          }
        };
        xhttp.open("GET", "/speed2", true);
        xhttp.send();
      }, 3000 ) ;
      </script>
      <script>
        var chartT = new Highcharts.Chart({
          chart:{ renderTo : 'chart-speed1' },
          title: { text: '' },
          series: [{
            showInLegend: false,
            data: []
          }],
          plotOptions: {
            line: { animation: false,
              dataLabels: { enabled: true } 
            },
            series: { color: 'rgb(62, 62, 67)' }
          },
          xAxis: { type: 'datetime',
            dateTimeLabelFormats: { minute: '%H:%M:%S' },
            title: {text:null},
          },
          yAxis: {
            title: { text: 'Tốc độ(rpm)' }
          },
          credits: { enabled: false }
        });
        setInterval(function ( ) {
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 
            200) {
              var x = 'Ngày '+(new Date()).getDate()+'/'+((new Date()).getMonth()+1)+'/'+(new Date()).getFullYear()
                      +': '+(new Date()).getHours()+' giờ '+(new Date()).getMinutes()+' phút '+(new Date()).getSeconds()+' giây ';
              var y = parseFloat(this.responseText);
              //console.log(this.responseText);
              if(chartT.series[0].data.length > 40) {
                chartT.series[0].addPoint([x, y], true, true, true);
              } else {
                chartT.series[0].addPoint([x, y], true, false, true);
              }
            }
          };
          xhttp.open("GET", "/speed1", true);
          xhttp.send();
        }, 1000 ) ;
        </script>
    <script>
          var chartP = new Highcharts.Chart({
            chart:{ renderTo : 'chart-speed2' },
            title: { text: '' },
            series: [{
              showInLegend: false,
              data: []
            }],
            plotOptions: {
              line: { animation: false,
                dataLabels: { enabled: true }
              },
              series: { color: 'rgb(123, 180, 236)' }
            },
            xAxis: { type: 'datetime',
              dateTimeLabelFormats: { minute: '%H:%M:%S' },
              title: {text:null},
            },
            yAxis: {
              title: { text: 'Tốc độ(rpm)' }
            },
            credits: { enabled: false }
          });
          setInterval(function ( ) {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
              if (this.readyState == 4 && this.status == 
              200) {
                var x = 'Ngày '+(new Date()).getDate()+'/'+((new Date()).getMonth()+1)+'/'+(new Date()).getFullYear()
                        +': '+(new Date()).getHours()+' giờ '+(new Date()).getMinutes()+' phút '+(new Date()).getSeconds()+' giây ';
                var y = parseFloat(this.responseText);
                //console.log(this.responseText);
                if(chartP.series[0].data.length > 40) {
                  chartP.series[0].addPoint([x, y], true, true, true);
                } else {
                  chartP.series[0].addPoint([x, y], true, false, true);
                }
              }
            };
            xhttp.open("GET", "/speed2", true);
            xhttp.send();
          }, 1000 ) ;
        </script>
</html>
)rawliteral";

String relayState(int numRelay){
  if(RELAY_NO){
    if(digitalRead(relayGPIOs[numRelay-1])){
      return "";
    }
    else {
      return "checked";
    }
  }
  else {
    if(digitalRead(relayGPIOs[numRelay-1])){
      return "checked";
    }
    else {
      return "";
    }
  }
  return "";
}

// Replaces placeholder with ENCODER values
String processor(const String& var){
  //Serial.println(var);
  if(var == "SPEED"){
    return String(s);
  }
  if(var == "BUTTONPLACEHOLDER"){
    String buttons ="";
    for(int i=1; i<=NUM_RELAYS; i++){
      String relayStateValue = relayState(i);
      buttons+= "<h4>Relay #" + String(i) + " - GPIO " + relayGPIOs[i-1] + "</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"" + String(i) + "\" "+ relayStateValue +"><span class=\"slider\"></span></label>";
    }
    return buttons;
  }
   return String();
   
}
void setup() {
  

   // Serial port for debugging purposes
  Serial.begin(9600);
  mySerial.begin(9600);
 
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/html", index_html);
  });  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/speed1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(speed1).c_str());
  });
  server.on("/speed2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(speed2).c_str());
  });
  server.on("/setpoint", HTTP_GET, [](AsyncWebServerRequest *request){
    int paramsNr = request->params();
    for(int i=0;i<paramsNr;i++){
        AsyncWebParameter* p = request->getParam(i);
//        int pinValue2= (p->name()).toInt();  
//        int pinValue3= (p->value()).toInt(); 
        mySerial.println(p->name());
        Serial.println(p->name());
        String dat=p->name(); 
        if(dat=="dat"){
          Serial.println("OK");
          }
       
   //     mySerial.print(pinValue3);
   //     Serial.println(pinValue3);  
        delay(100);
    }
     request->send_P(200, "text/html", index_html);
     
  });
  
  // Start server
  server.begin();
}

void loop() {
  int e1=1,e2=0,i;
  
  String a1="",a2="";
    
  // put your main code here, to run repeatedly:
  if (mySerial.available())
  {char c = mySerial.read();
  s +=c;
  if (c =='\n')
    {
      s1=("%s",s);
      for (i=0; i<= s1.length();i++)
      {
        if(s1[i]=='a'){
          e2=1;
          e1=0;
          }
        else{
          if(e1==1){
          a1 += s1[i];
          } 
          if(e2==1){
          a2 += s1[i];
          } 
          }   
        }
      speed1=("%f",a1);
      speed2=("%f",a2);
//      Serial.println(s1);
//      Serial.println(speed1);
//      Serial.println(speed2);
      s="";
      }
    }
  }

//Source code này được viết vào dự án cuối năm 2020 sau đó được sửa đổi vài lần và đây là phiên bản 8
//Phiên bản 8 này đã được sử dụng cho đồ án tốt nghiệp k12 HaUI
 
//Hệ thống dùng để giám sát và điều khiển 2 động cơ cho xe tự hành MWR
//Web chỉ có khả năng điều khiển trong môi trường mạng cục bộ và chỉ phục vụ mục đích học tập cơ bản.

