set (COMPONENT_SRCS "Env.c" "bleenv.c" "../settings.c")
set (COMPONENT_REQUIRES "ESP32-RevK" "ESP32-GFX" "bt" "ds18b20" "onewire_bus")
register_component ()
