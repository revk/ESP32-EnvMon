set (COMPONENT_SRCS "Env.c" "bleenv.c" "settings.c")
set (COMPONENT_REQUIRES "ESP32-RevK" "ESP32-GFX" "ESP32-IR" "bt" "ds18b20" "onewire_bus")
register_component ()
