#! /bin/bash

# launch.sh
# Launches the server (daemonized) and multiple clients with different orders

SCRIPT_DIR=$(dirname "$0")
ROOT="$SCRIPT_DIR/.."
LOG_DIR="$ROOT/log"
SERVER="$ROOT/target/server"
CLIENT="$ROOT/target/client"
MAKE=make
CLIENTS=""

# You know
if [ "$(hostname)" = "encina" ] || [ "$(hostname)" = "olivo" ]; then
  MAKE=gmake
fi

if [ ! -f "$SERVER" ] || [ ! -f "$CLIENT" ]; then
  echo "run: Server or client not found, trying to run make on the root dir"
  "$MAKE" -C "$ROOT"

  if [ ! -f "$SERVER" ] || [ ! -f "$CLIENT" ]; then
    echo "run: Server or client couldn't be found, aborting..."
    exit 1
  fi
fi

echo "run: server: $SERVER"
echo "run: client: $CLIENT"

"$SERVER" -v -d -l "$LOG_DIR/server.log"
if [ $? -ne 0 ]; then
  echo "run: server exited, probably another instance is running"
fi

SERVER_PID=`pidof "$SERVER"`
if [ $? -ne 0 ]; then
  echo "run: running server not found, aborting..."
  exit 1
fi

echo "run: server pid: $SERVER_PID"

"$CLIENT" -l "$LOG_DIR/tcp-client-0.log" -h "$(hostname)" -f "$SCRIPT_DIR/ordenes.txt" -v &
CLIENTS+="$! "
"$CLIENT" -l "$LOG_DIR/tcp-client-1.log" -h "$(hostname)" -f "$SCRIPT_DIR/ordenes1.txt" -v &
CLIENTS+="$! "
"$CLIENT" -l "$LOG_DIR/tcp-client-2.log" -h "$(hostname)" -f "$SCRIPT_DIR/ordenes2.txt" -v &
CLIENTS+="$! "

"$CLIENT" -l "$LOG_DIR/udp-client-0.log" -h "$(hostname)" -f "$SCRIPT_DIR/ordenes.txt" -udp -v &
CLIENTS+="$! "
"$CLIENT" -l "$LOG_DIR/udp-client-1.log" -h "$(hostname)" -f "$SCRIPT_DIR/ordenes1.txt" -udp -v &
CLIENTS+="$! "
"$CLIENT" -l "$LOG_DIR/udp-client-2.log" -h "$(hostname)" -f "$SCRIPT_DIR/ordenes2.txt" -udp -v &
CLIENTS+="$! "

echo "run: clients: $CLIENTS"

for client in $CLIENTS; do
  echo "run: waiting for: $client"
  wait $client;
  if [ $? -ne 0 ]; then
    echo "run: couldn't wait for $client"
  fi
done

echo "run: Seems like all the clients have exited, what do you want to do?"
echo "  1) Kill the server"
echo "  2) Keep the server running but dump the data"
echo "  *) Do nothing"
read -p "choose: " option
case $option in
  1) kill $SERVER_PID ;;
  2) kill -SIGUSR1 $SERVER_PID ;;
esac

