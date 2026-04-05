#include "OTA.h"

TaskHandle_t Task0; //will handle everything I want to happen on core 0
TaskHandle_t Task1; //will handle everyting I want to happen on core 1

Wireless laptop = Wireless();

void setup() {
  Serial.begin(115200);
  laptop.init();
  laptop.println("Connected Wirelessly");

  /* xTaskCreatePinnedToCore(
  -pvTaskCode:	The name of the function that contains the code you want running on the assigned processor core.
  -pcName:	The friendly name for the task
  -usStackDepth:	The amount of bytes allocated for the task stack. I don't know how to allocate this, so I just use 10000 which I've seen in several examples.
  -pvParameters:	A pointer pointing to any parameters you want passed to the task code.
  -uxPriority:	The task priority; I always use 1 across all tasks, but if you have long-running tasks with different priorities, you can tune how much attention they get by playing around with priority.
  -pvCreatedTask:	A pointer to the task the sketch can use to reference the task later (for example to terminate it).
  -xCoreID:	The processor core to assign the task to.
  )
  */
  xTaskCreatePinnedToCore(loop0, "Task0", 10000, NULL, 1, &Task0, 0);
  delay(500);
  xTaskCreatePinnedToCore(loop1, "Task1", 10000, NULL, 1, &Task1, 1);
}

void loop() { }

void loop0(void* pvParameters) {
  for (int i = 0; i < 1000000; i++) { //do complicated robot tasks
    float lol = pow(cos(i),i);
  }
  laptop.print("Whoa, loop0 is doing work!");
}
void loop1(void* pvParameters) {
  laptop.handle();

  if (millis() % 1000 == 0) {
    laptop.println(millis());
    Serial.println(millis());
  }
}