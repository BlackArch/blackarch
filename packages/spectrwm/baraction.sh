#!/bin/bash

# Function to get CPU usage
cpu_usage() {
    awk -v FS=" " '/^cpu / {usage=($2+$4)*100/($2+$4+$5); printf "🔥 %.1f%%", usage}' /proc/stat
}

# Function to get memory usage
mem_usage() {
    free -m | awk '/Mem:/ {printf "🧠 %dMB / %dMB", $3, $2}'
}

# Function to get volume level
volume() {
    amixer get Master | awk -F'[][]' '/%/ {print "🔊 " $2; exit}'
}

# Function to get Wi-Fi SSID
wifi() {
    SSID=$(iwgetid -r)
    if [ -z "$SSID" ]; then
        echo "❌ No Wi-Fi"
    else
        echo "📶 $SSID"
    fi
}

# Main loop for Spectrwm bar
while true; do
    echo "CPU: $(cpu_usage) | RAM: $(mem_usage) | Vol: $(volume) | Wi-Fi: $(wifi)"
    sleep 1
done
