
#include <iostream>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"


#include "jsoncpp/value.h"
#include "jsoncpp/json.h"

#include "esp_firebase/app.h"
#include "esp_firebase/rtdb.h"

#include "wifi_utils.h"

#include "firebase_config.h"

#include "esp32_gpio.h"

#include "dht11/dht11.h"

using namespace std;


using namespace ESPFirebase;
extern "C" void app_main(void)
{
    wifiInit(SSID, PASSWORD);  // blocking until it connects

    // Config and Authentication
    user_account_t account = {USER_EMAIL, USER_PASSWORD};

    FirebaseApp app = FirebaseApp(API_KEY);

    app.loginUserAccount(account);

    RTDB db = RTDB(&app, DATABASE_URL);

    

    // R"()" allow us to write string as they are without escaping the characters with backslash

    // We can put a json str directly at /person1
    std::string json_str = R"({"name": "Madjid", "age": 20, "random_float": 8.56})";
    db.putData("/person1", json_str.c_str()); 
    // vTaskDelay(500/portTICK_PERIOD_MS);

    // We can parse the json_str and access the members and edit them
    Json::Value data;
    Json::Reader reader; 
    reader.parse(json_str, data);

    std::string madjid_name = data["name"].asString();  // convert value to primitives (read jsoncpp docs for more of these)

    //ESP_LOGI("MAIN", "name: %s", madjid_name.c_str());   
    
    data["name"] = "Tikou";

    //ESP_LOGI("MAIN", "edited name from %s to: %s", madjid_name.c_str(), data["name"].asString().c_str());   

    data["age"] = 22;
    data["random_float"] = 4.44;
    
    // put json object directly
    db.putData("/person2", data);   

    // vTaskDelay(500/portTICK_PERIOD_MS);
    // Construct a new json object manually
    Json::Value new_data; 
    new_data["name"] = "Lotfi";
    new_data["age"] = 23;
    new_data["random_float"] = 5.95;

    db.putData("person3", new_data);
    // vTaskDelay(500/portTICK_PERIOD_MS);

    // Edit person2 data in the database by patching
    data["age"] = 23;
    db.patchData("person2", data);
    Json::Value root = db.getData("person3"); // retrieve person3 from database, set it to "" to get entire database

    Json::FastWriter writer;
    std::string person3_string = writer.write(root);  // convert it to json string

    ESP_LOGI("MAIN", "person3 as json string: \n%s", person3_string.c_str());

    // You can also print entire Json Value object with std::cout with converting to string 
    // you cant print directly with printf or LOGx because Value objects can have many type. << is overloaded and can print regardless of the type of the Value
    std::cout << root << std::endl;

    // print the members (Value::Members is a vector)
    Json::Value::Members members = root.getMemberNames();  
    for (const auto& member : members)
    {
        std::cout << member << ", ";
    }
    std::cout << std::endl;


    db.deleteData("person3"); // delete person3
    root = db.getData("person3"); // retrieve person3 from database, this time it will be null because person3 doesnt exist in database
    std::cout << root << std::endl;

    
    //zero-initialize the config structure.
    gpio_config_t io_conf = {};
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = 1 << 2;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
    
    DHT11_init(GPIO_NUM_4);

    while (1)
    {
        Json::Value light_status = db.getData("/firebase/light");
        if (light_status == "1")
            gpio_set_level(GPIO_NUM_2, 1);
        else
            gpio_set_level(GPIO_NUM_2, 0);
        if (DHT11_read().status == DHT11_OK)
        {
            db.putData("/firebase/humidity", DHT11_read().humidity); 
            db.putData("/firebase/temperature", DHT11_read().temperature); 
        }
        vTaskDelay(50/portTICK_PERIOD_MS);
    }

}

