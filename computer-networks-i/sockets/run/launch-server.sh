#! /bin/sh

# launch.sh
# Launches the server (daemonized) and multiple clients with different orders

SCRIPT_DIR=$(dirname "$0")
ROOT="$SCRIPT_DIR/.."
LOG_DIR="$ROOT/log"
SERVER="$ROOT/target/server"
CLIENT="$ROOT/target/client"
MAKE=make

if [ "$(hostname)" = "encina" ] || [ "$(hostname)" = "olivo" ]; then
  MAKE=gmake
fi

if [ ! -f "$SERVER" ] || [ ! -f "$CLIENT" ]; then
  echo "Server or client not found, trying to run make on the root dir"
  "$MAKE" -C "$ROOT"

  if [ ! -f "$SERVER" ] || [ ! -f "$CLIENT" ]; then
    echo "Server or client couldn't be found, aborting..."
    exit
  fi
fi

echo "server: $SERVER"
echo "client: $CLIENT"

"$SERVER" -v -d -l "$LOG_DIR/server.log"

"$CLIENT" -l "$LOG_DIR/tcp-client-0.log" -h "$(hostname)" -f "$SCRIPT_DIR/ordenes.txt" -v &
"$CLIENT" -l "$LOG_DIR/tcp-client-1.log" -h "$(hostname)" -f "$SCRIPT_DIR/ordenes1.txt" -v &
"$CLIENT" -l "$LOG_DIR/tcp-client-2.log" -h "$(hostname)" -f "$SCRIPT_DIR/ordenes2.txt" -v &

"$CLIENT" -l "$LOG_DIR/udp-client-0.log" -h "$(hostname)" -f "$SCRIPT_DIR/ordenes.txt" -udp -v &
"$CLIENT" -l "$LOG_DIR/udp-client-1.log" -h "$(hostname)" -f "$SCRIPT_DIR/ordenes1.txt" -udp -v &
"$CLIENT" -l "$LOG_DIR/udp-client-2.log" -h "$(hostname)" -f "$SCRIPT_DIR/ordenes2.txt" -udp -v &
